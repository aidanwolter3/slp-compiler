#ifndef PAIR_EXPRESSION_LIST
#define PAIR_EXPRESSION_LIST

#include "ParseTreeNode.h"

class PairExpressionList : public ExpressionList {
  public:
    Expression *exp;
    ExpressionList *list;
    PairExpressionList(Expression *e, ExpressionList *l);
    void* accept(Visitor *v);
};

#endif
