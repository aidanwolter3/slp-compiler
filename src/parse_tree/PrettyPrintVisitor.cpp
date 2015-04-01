#include "stdio.h"
#include "CompoundStatement.h"
#include "AssignStatement.h"
#include "PrintStatement.h"
#include "IdExpression.h"
#include "NumExpression.h"
#include "OperationExpression.h"
#include "SequenceExpression.h"
#include "PairExpressionList.h"
#include "LastExpressionList.h"
#include "Plus.h"
#include "Minus.h"
#include "Divide.h"
#include "Multiply.h"

class PrettyPrintVisitor : public Visitor {
  void* visit(CompoundStatement *s) {
    s->stm1->accept(this);
    printf(";\n");
    s->stm2->accept(this);
    return 0;
  }
  void* visit(AssignStatement *stm) {
    stm->id->accept(this);
    printf(" := ");
    stm->exp->accept(this);
    return 0;
  }
  void* visit(PrintStatement *stm) {
    printf("print(");
    stm->list->accept(this);
    printf(")");
    return 0;
  }
  void* visit(IdExpression *exp) {
    printf("%s", exp->lexem);
    return 0;
  }
  void* visit(NumExpression *exp) {
    printf("%d", exp->val);
    return 0;
  }
  void* visit(OperationExpression *exp) {
    exp->exp1->accept(this);
    printf(" ");
    exp->op->accept(this);
    printf(" ");
    exp->exp2->accept(this);
    return 0;
  }
  void* visit(SequenceExpression *exp) {
    printf("(");
    exp->stm->accept(this);
    printf(", ");
    exp->exp->accept(this);
    printf(")");
    return 0;
  }
  void* visit(PairExpressionList *exp) {
    exp->exp->accept(this);
    printf(", ");
    exp->list->accept(this);
    return 0;
  }
  void* visit(LastExpressionList *exp) {
    exp->exp->accept(this);
    return 0;
  }
  void* visit(Plus *op) {
    printf("+");
    return 0;
  }
  void* visit(Minus *op) {
    printf("-");
    return 0;
  }
  void* visit(Divide *op) {
    printf("/");
    return 0;
  }
  void* visit(Multiply *op) {
    printf("*");
    return 0;
  }
};
