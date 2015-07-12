#include <limits.h>
#include <string.h>
#include "StrExpression.h"

StrExpression::StrExpression(char *val) {
  data_size = strlen(val)*4;
  this->val = strdup(val);
}

void* StrExpression::accept(Visitor *v) {
  return v->visit(this);
}
