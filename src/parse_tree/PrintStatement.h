#ifndef PRINT_STATEMENT
#define PRINT_STATEMENT

#include "ParseTreeNode.h"

class PrintStatement : public Statement {
  public:
    ExpressionList *list;
    PrintStatement(ExpressionList *l);
    void accept(Visitor *v);
};

#endif
