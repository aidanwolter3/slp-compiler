#include "IdExpression.h"

IdExpression::IdExpression(char *l) {
  lexem = l;
}

void* IdExpression::accept(Visitor *v) {
  return v->visit(this);
}
