#include "NumExpression.h"

NumExpression::NumExpression(int v) {
  val = v;
}

void* NumExpression::accept(Visitor *v) {
  return v->visit(this);
}
