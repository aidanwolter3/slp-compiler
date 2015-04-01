#include "CompoundStatement.h"

CompoundStatement::CompoundStatement(Statement *s1, Statement *s2) {
  stm1 = s1;
  stm2 = s2;
}

void* CompoundStatement::accept(Visitor *v) {
  return v->visit(this);
}
