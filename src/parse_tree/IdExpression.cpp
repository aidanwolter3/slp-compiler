#include "IdExpression.h"

IdExpression::IdExpression(char *l) {
  lexem = l;
}

void IdExpression::accept(Visitor *v) {
  v->visit(this);
}
