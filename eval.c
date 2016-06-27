//IDEA: CBMC IS SHIT, UNROLL LOOPS MANUALLY IN PYTHON
#include "stdint.h"
#include "stdbool.h"
#include "assert.h"
#include "stdio.h"

#define cbmc_assert(x,y) assert(!(x)||!(y))

typedef uint16_t num;
typedef uint8_t byte;
typedef uint8_t opcode; //for use in the equation tree as function identifiers

//Consts, user-defined
#define treeDepth 1
#define numNodes 1<<treeDepth-1
const byte x[1] = {1}; //the input
const num y = 1; //the output

//Define the tree struct
typedef uint8_t ptr;
typedef struct node {
  byte val; //for opcodes const_id and index
  opcode op;
  ptr a;
  ptr b;
} node;

//Opcodes
opcode var_id = 0; //i.e. index of the sequence, the independent variable n
opcode const_id = 1;
opcode add_id = 2;
opcode mul_id = 3;

//Vars
node nodes[numNodes]; //the tree
bool overflow = false; //overflow flag

//Overflow-protected functions
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
  if (highest(a) < 15 && highest(b) < 15)
    return a+b;
  else
    overflow = true;
  return 0;
}

num mul(num a, num b)
{
  if (highest(a) + highest(b) < 15)
    return a*b;
  else
    overflow = true;
  return 0;
}

//Recursive (implicitly stack-based) tree evaluator
num eval(node root)
{
  if (root.op == var_id) return x[root.val];
  if (root.op == const_id) return root.val;

  num a = eval(nodes[root.a]); num b = eval(nodes[root.b]);
  switch (root.op){
    case 2:
      return add(a, b);
    case 3:
      return mul(a, b);
  }
}

//Assertions
//Initialize the tree
int main()
{
  nodes[0].val = nondet_uchar();
  nodes[0].op = nondet_uchar();
  nodes[0].a == 1;
  nodes[0].b == 2;
  __CPROVER_assume(nodes[0].op < 4);
  __CPROVER_assume(!(nodes[0].op == 0 && nodes[0].val > 0));

  nodes[1].val = nondet_uchar();
  nodes[1].op = nondet_uchar();
  __CPROVER_assume(nodes[1].op < 2);

  nodes[2].val = nondet_uchar();
  nodes[2].op = nondet_uchar();
  __CPROVER_assume(nodes[2].op < 2);
  assert(eval(nodes[0])!=1);
}
