#include "SequenceExpression.h"

SequenceExpression::SequenceExpression(Statement *s, Expression *e) {
  stm = s;
  exp = e;
}

void* SequenceExpression::accept(Visitor *v) {
  return v->visit(this);
}
