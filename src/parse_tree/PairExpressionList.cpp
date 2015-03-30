#include "PairExpressionList.h"

PairExpressionList::PairExpressionList(Expression *e, ExpressionList *l) {
  exp = e;
  list = l;
}

void PairExpressionList::accept(Visitor *v) {
  v->visit(this);
}
