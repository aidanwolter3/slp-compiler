#ifndef OPERATION_EXPRESSION
#define OPERATION_EXPRESSION

#include "ParseTreeNode.h"

class OperationExpression : public Expression {
  public:
    Expression *exp1, *exp2;
    BinaryOperation *op;
    OperationExpression(Expression *e1, Expression *e2, BinaryOperation *o);
    void* accept(Visitor *v);
};

#endif
