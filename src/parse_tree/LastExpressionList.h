#ifndef LAST_EXPRESSION_LIST
#define LAST_EXPRESSION_LIST

#include "ParseTreeNode.h"

class LastExpressionList : public ExpressionList {
  public:
    Expression *exp;
    LastExpressionList(Expression *e);
    void accept(Visitor *v);
};

#endif
