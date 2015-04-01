#include "AssignStatement.h"

AssignStatement::AssignStatement(IdExpression *i, Expression *e) {
  id = i;
  exp = e;
}

void* AssignStatement::accept(Visitor *v) {
  return v->visit(this);
}
