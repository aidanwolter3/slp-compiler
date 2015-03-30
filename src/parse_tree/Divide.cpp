#include "Divide.h"

void Divide::accept(Visitor *v) {
  v->visit(this);
}
