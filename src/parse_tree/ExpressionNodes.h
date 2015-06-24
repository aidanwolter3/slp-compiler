#ifndef EXPRESSION_NODES
#define EXPRESSION_NODES

#include "ParseTreeNode.h"
#include "BinaryOperationNodes.h"

//expressions
class IdExpression : Expression {
  char *lexem;
  IdExpression(char *l);
};
class NumExpression : Expression {
  int val;
  NumExpression(int v);
};
class OperationExpression : Expression {
  Expression exp1, exp2;
  BinaryOperation op;
  OperationExpression(Expression exp1, Expression exp2, BinaryOperation op);
};

#endif
