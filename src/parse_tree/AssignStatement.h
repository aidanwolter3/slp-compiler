#ifndef ASSIGN_STATEMENT
#define ASSIGN_STATEMENT

#include "ParseTreeNode.h"

class IdExpression;
class AssignStatement : public Statement {
  public:
    IdExpression *id;
    Expression *exp;
    AssignStatement(IdExpression *i, Expression *e);
    void accept(Visitor *v);
};

#endif
