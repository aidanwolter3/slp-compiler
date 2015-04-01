#include "Minus.h"

void* Minus::accept(Visitor *v) {
  return v->visit(this);
}
