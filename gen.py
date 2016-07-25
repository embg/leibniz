import numpy as np
def compile(X, y):
    assert(len(X.shape) == 2)

    treeDepth = 2; numNodes = 2**(treeDepth+1)-1

    numOps = 5

    includes = """
    #include "eval.h"
    """

    definitions = """
    #define treeDepth {treeDepth}
    #define numNodes {numNodes}
    #define numPoints {numPoints}
    #define numVars {numVars}
    #define numOps {numOps}
    """.format(treeDepth=treeDepth,
               numNodes=numNodes,
               numPoints=len(y),
               numVars=X.shape[1],
               numOps=numOps)

    def translateToC(array):
        return np.array2string(np.array(array),
                               separator=',').replace('[','{').replace(']','}')

    X_initializer = """
    const byte X[numPoints][numVars] = """ + translateToC(X) + ';'

    y_initializer = """
    const num y[numPoints] = """ + translateToC(y) + ';'

    main = """
    int main(){
    """
    for i in range(0, numNodes):
        main += """
                   nodes[{num}].val = nondet_uchar();
                   nodes[{num}].op = nondet_uchar();
                   __CPROVER_assume(!(nodes[{num}].op == 0 && (nodes[{num}].val >= numVars ||
                                                               nodes[{num}].val < 0)));
                """.format(num=i)
        if i < numNodes//2:
            main += """
                       nodes[{num}].a = {a};
                       nodes[{num}].b = {b};
                       __CPROVER_assume(nodes[{num}].op <= numOps);
                    """.format(num=i , a=2*i+1, b=2*i+2)
        else:
            main += """
                       nodes[{num}].a = 0;
                       nodes[{num}].b = 0;
                       __CPROVER_assume(nodes[{num}].op < 2);
                    """.format(num=i)

    main += """
    #define root nodes[0]
    assert(!(ArithmeticError==false &&
    """
    for i in range(0, len(y)):
        main += """
        eval(root,{num})==y[{num}] &&
        """.format(num=i)
    main += """
    true));}
    """

    return (includes +
            definitions +
            X_initializer +
            y_initializer +
            open("eval.c").read()+
            main + '\n')


X = np.array([np.arange(0,10,1)]).transpose()
y = X[:,0]**4

open('compiled.c','w').write(compile(X, y))
