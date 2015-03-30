#include "Plus.h"

void Plus::accept(Visitor *v) {
  v->visit(this);
}
