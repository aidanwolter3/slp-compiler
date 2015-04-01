#include "Divide.h"

void* Divide::accept(Visitor *v) {
  return v->visit(this);
}
