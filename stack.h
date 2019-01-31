// Stack ADT header file ... COMP9024 17s2
#include "BSTree.h"
typedef struct StackRep *stack;

stack newStack();             // set up empty stack
void  dropStack(stack);       // remove unwanted stack
int   StackIsEmpty(stack);    // check whether stack is empty
void  StackPush(stack, Tree);  // insert an int on top of stack
Tree StackPop(stack);        // remove int from top of stack
