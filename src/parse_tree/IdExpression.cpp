#include "IdExpression.h"

IdExpression::IdExpression(char *l) {
  data_size = 4;
  lexem = l;
}

void* IdExpression::accept(Visitor *v) {
  return v->visit(this);
}
