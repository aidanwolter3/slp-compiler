#ifndef CODE_GENERATOR_X86
#define CODE_GENERATOR_X86

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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

class CodeGenerator_x86 : public Visitor {
  private:
    char *code;
    int len;

    class CodeReturn {
      public:
        int type;
        char *tmp;
        CodeReturn(int size, int t) {
          tmp = (char*)malloc(3*sizeof(char));
          type = t;
        };
    };
  public:
    CodeGenerator_x86();
    void print_code();
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
