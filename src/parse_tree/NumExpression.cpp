#include "NumExpression.h"

NumExpression::NumExpression(int v) {
  data_size = 4;
  val = v;
}

void* NumExpression::accept(Visitor *v) {
  return v->visit(this);
}
