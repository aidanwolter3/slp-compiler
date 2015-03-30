#include "LastExpressionList.h"

LastExpressionList::LastExpressionList(Expression *e) {
  exp = e;
}

void LastExpressionList::accept(Visitor *v) {
  v->visit(this);
}
