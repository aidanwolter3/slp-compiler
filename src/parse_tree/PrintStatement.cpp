#include "PrintStatement.h"

PrintStatement::PrintStatement(Expression *e) {
  exp = e;
}

void* PrintStatement::accept(Visitor *v) {
  return v->visit(this);
}
