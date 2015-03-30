#include "NumExpression.h"

NumExpression::NumExpression(int v) {
  val = v;
}

void NumExpression::accept(Visitor *v) {
  v->visit(this);
}
