#include "PrintStatement.h"

PrintStatement::PrintStatement(ExpressionList *l) {
  list = l;
}

void* PrintStatement::accept(Visitor *v) {
  return v->visit(this);
}
