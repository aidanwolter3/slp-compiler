#include "VariableEvaluatorVisitor.h"

VariableEvaluatorVisitor::VariableEvaluatorVisitor() {
  v_table = new VariableTable();
}

void* VariableEvaluatorVisitor::printVariables() {
  v_table->print();
  return 0;
}
void* VariableEvaluatorVisitor::visit(CompoundStatement *s) {
  s->stm1->accept(this);
  s->stm2->accept(this);
  return new IntegerReturn(0, 0);
}
void* VariableEvaluatorVisitor::visit(AssignStatement *stm) {
  stm->id->accept(this);
  IntegerReturn *obj = (IntegerReturn*)stm->exp->accept(this);
  if(obj != 0) {
    v_table->set(stm->id->lexem, obj->val);
  }
  return new IntegerReturn(0, 1);
}
void* VariableEvaluatorVisitor::visit(PrintStatement *stm) {
  stm->exp->accept(this);
  return new IntegerReturn(0, 2);
}
void* VariableEvaluatorVisitor::visit(IdExpression *exp) {
  int val = v_table->get(exp->lexem);
  if(val == -1) {
    v_table->set(exp->lexem, 0);
    val = 0;
  }
  return new IntegerReturn(val, 3);
}
void* VariableEvaluatorVisitor::visit(NumExpression *exp) {
  return new IntegerReturn(exp->val, 4);
}
void* VariableEvaluatorVisitor::visit(OperationExpression *exp) {
  IntegerReturn *val1 = (IntegerReturn*)exp->exp1->accept(this);
  IntegerReturn *op_type = (IntegerReturn*)exp->op->accept(this);
  IntegerReturn *val2 = (IntegerReturn*)exp->exp2->accept(this);

  if(op_type->type == 9) {
    return new IntegerReturn(val1->val + val2->val, 5);
  }
  else if(op_type->type == 10) {
    return new IntegerReturn(val1->val - val2->val, 5);
  }
  else if(op_type->type == 11) {
    return new IntegerReturn(val1->val / val2->val, 5);
  }
  else if(op_type->type == 12) {
    return new IntegerReturn(val1->val * val2->val, 5);
  }
  else {
    return new IntegerReturn(0, 5);
  }
}
void* VariableEvaluatorVisitor::visit(PairExpressionList *exp) {
  exp->exp->accept(this);
  exp->list->accept(this);
  return new IntegerReturn(0, 7);
}
void* VariableEvaluatorVisitor::visit(LastExpressionList *exp) {
  exp->exp->accept(this);
  return new IntegerReturn(0, 8);
}
void* VariableEvaluatorVisitor::visit(Plus *op) {
  return new IntegerReturn(0, 9);
}
void* VariableEvaluatorVisitor::visit(Minus *op) {
  return new IntegerReturn(0, 10);
}
void* VariableEvaluatorVisitor::visit(Divide *op) {
  return new IntegerReturn(0, 11);
}
void* VariableEvaluatorVisitor::visit(Multiply *op) {
  return new IntegerReturn(0, 12);
}
