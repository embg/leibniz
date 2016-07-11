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

//Recursive tree evaluator
num eval(node root, byte x_idx)
{
  if (root.op == var_id) return X[x_idx][root.val];
  if (root.op == const_id) return root.val;

  num a = eval(nodes[root.a],x_idx); num b = eval(nodes[root.b],x_idx);
  switch (root.op){
    case 2:
      return add(a, b);
    case 3:
      return mul(a, b);
  }
}
