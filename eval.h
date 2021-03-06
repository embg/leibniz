#include "stdint.h"
#include "stdbool.h"

typedef uint16_t num;
typedef uint8_t byte;
typedef uint8_t opcode; //for use in the equation tree as function identifiers

//Define the tree struct
typedef uint8_t ptr;
typedef struct node {
  opcode op;
  byte val; //for opcodes const_id and index
  ptr a;
  ptr b;
} node;
