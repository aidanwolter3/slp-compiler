#include "LastExpressionList.h"

LastExpressionList::LastExpressionList(Expression *e) {
  exp = e;
}

void* LastExpressionList::accept(Visitor *v) {
  return v->visit(this);
}
