#include <limits.h>
#include <string.h>
#include "StrExpression.h"

StrExpression::StrExpression(char *val) {
  this->val = strdup(val);
}

void* StrExpression::accept(Visitor *v) {
  return v->visit(this);
}
