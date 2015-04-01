#include "PairExpressionList.h"

PairExpressionList::PairExpressionList(Expression *e, ExpressionList *l) {
  exp = e;
  list = l;
}

void* PairExpressionList::accept(Visitor *v) {
  return v->visit(this);
}
