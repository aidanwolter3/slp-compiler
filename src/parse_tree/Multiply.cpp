#include "Multiply.h"

void* Multiply::accept(Visitor *v) {
  return v->visit(this);
}
