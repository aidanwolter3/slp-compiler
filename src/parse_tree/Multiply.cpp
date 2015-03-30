#include "Multiply.h"

void Multiply::accept(Visitor *v) {
  v->visit(this);
}
