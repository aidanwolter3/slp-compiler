#include "PrettyPrintVisitor.h"

void* PrettyPrintVisitor::visit(CompoundStatement *s) {
  s->stm1->accept(this);
  printf(";\n");
  s->stm2->accept(this);
  return 0;
}
void* PrettyPrintVisitor::visit(AssignStatement *stm) {
  stm->id->accept(this);
  printf(" := ");
  stm->exp->accept(this);
  return 0;
}
void* PrettyPrintVisitor::visit(PrintStatement *stm) {
  printf("print(");
  stm->exp->accept(this);
  printf(")");
  return 0;
}
void* PrettyPrintVisitor::visit(IdExpression *exp) {
  printf("%s", exp->lexem);
  return 0;
}
void* PrettyPrintVisitor::visit(NumExpression *exp) {
  printf("%d", exp->val);
  return 0;
}
void* PrettyPrintVisitor::visit(OperationExpression *exp) {
  exp->exp1->accept(this);
  printf(" ");
  exp->op->accept(this);
  printf(" ");
  exp->exp2->accept(this);
  return 0;
}
void* PrettyPrintVisitor::visit(PairExpressionList *exp) {
  exp->exp->accept(this);
  printf(", ");
  exp->list->accept(this);
  return 0;
}
void* PrettyPrintVisitor::visit(LastExpressionList *exp) {
  exp->exp->accept(this);
  return 0;
}
void* PrettyPrintVisitor::visit(Plus *op) {
  printf("+");
  return 0;
}
void* PrettyPrintVisitor::visit(Minus *op) {
  printf("-");
  return 0;
}
void* PrettyPrintVisitor::visit(Divide *op) {
  printf("/");
  return 0;
}
void* PrettyPrintVisitor::visit(Multiply *op) {
  printf("*");
  return 0;
}
