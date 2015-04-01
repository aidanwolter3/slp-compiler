#ifndef VARIABLE_EVALUATOR_VISITOR
#define VARIABLE_EVALUATOR_VISITOR

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

#include "VariableTable.h"

class VariableEvaluatorVisitor : public Visitor {
  private:
    VariableTable *v_table;

    class IntegerReturn {
      public:
        int val; //integer value to return
        int type;//what specific class we are returning from
        IntegerReturn(int v, int t) {val = v; type = t;}
    };

  public:
    VariableEvaluatorVisitor();
    void* printVariables();
    void* visit(CompoundStatement *s);
    void* visit(AssignStatement *stm);
    void* visit(PrintStatement *stm);
    void* visit(IdExpression *exp);
    void* visit(NumExpression *exp);
    void* visit(OperationExpression *exp);
    void* visit(SequenceExpression *exp);
    void* visit(PairExpressionList *exp);
    void* visit(LastExpressionList *exp);
    void* visit(Plus *op);
    void* visit(Minus *op);
    void* visit(Divide *op);
    void* visit(Multiply *op);
};

#endif
