#include "Minus.h"

void Minus::accept(Visitor *v) {
  v->visit(this);
}
