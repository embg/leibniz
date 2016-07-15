//Opcodes
#define var_id 0 //i.e. index of the sequence, the independent variable 
#define const_id 1
#define add_id 2
#define mul_id 3
#define neg_id 4
#define pow_id 5

//Vars
node nodes[numNodes]; //the tree
bool ArithmeticError = false; //ArithmeticError flag

//ArithmeticError-protected functions
byte highest(num a) //highest bit of a
{
  if (a >> 15) return 15; if (a >> 14) return 14;
  if (a >> 13) return 13; if (a >> 12) return 12;
  if (a >> 11) return 11; if (a >> 10) return 10;
  if (a >> 9)  return 9;  if (a >> 8)  return 8;
  if (a >> 7)  return 7;  if (a >> 6)  return 6;
  if (a >> 5)  return 5;  if (a >> 4)  return 4;
  if (a >> 3)  return 3;  if (a >> 2)  return 2;
  if (a >> 1)  return 1;
  return 0;
}

num add(num a, num b)
{
  if (highest(a) < 63 && highest(b) < 63)
    return a+b;
  else
    ArithmeticError = true;
  return 0;
}

num mul(num a, num b)
{
  if (highest(a) + highest(b) < 63)
    return a*b;
  else
    ArithmeticError = true;
  return 0;
}

num exp_by_squaring(num a, num b){
  if (b == 0) return 1;
  if (b == 1) return a;
  if (b && 1) return mul(a,exp_by_squaring(mul(a,a), (b-1)>>1));
  else return exp_by_squaring(mul(a,a), b>>1);
}

num pow(num a, num b)
{
  if (b>=0)
    return exp_by_squaring(a, b);
  else 
    ArithmeticError = true;
  return 0;
}

//Recursive tree evaluator
num eval(node root, byte x_idx)
{
  if (root.op == var_id) return X[x_idx][root.val];
  if (root.op == const_id) return root.val;

  num a = eval(nodes[root.a],x_idx);
  if (root.op == neg_id) return -a;

  num b = eval(nodes[root.b],x_idx);
  switch (root.op){
  case add_id:
    return add(a, b);
  case mul_id:
    return mul(a, b);
  case pow_id:
    return pow(a, b);
  }
}
