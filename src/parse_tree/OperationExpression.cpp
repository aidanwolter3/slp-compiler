#include "OperationExpression.h"

OperationExpression::OperationExpression(Expression *e1, Expression *e2, BinaryOperation *o) {
  exp1 = e1;
  exp2 = e2;
  op = o;
}

void OperationExpression::accept(Visitor *v) {
  v->visit(this);
}
