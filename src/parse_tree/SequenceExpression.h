#ifndef SEQUENCE_EXPRESSION
#define SEQUENCE_EXPRESSION

#include "ParseTreeNode.h"

class SequenceExpression : public Expression {
  public:
    Statement *stm;
    Expression *exp;
    SequenceExpression(Statement *s, Expression *e);
    void accept(Visitor *v);
};

#endif
