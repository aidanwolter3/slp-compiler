#ifndef PRINT_STATEMENT
#define PRINT_STATEMENT

#include "ParseTreeNode.h"

class PrintStatement : public Statement {
  public:
    Expression *exp;
    PrintStatement(Expression *e);
    void* accept(Visitor *v);
};

#endif
