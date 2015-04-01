#include "Plus.h"

void* Plus::accept(Visitor *v) {
  return v->visit(this);
}
