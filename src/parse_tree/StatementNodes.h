#ifndef STATEMENT_NODES
#define STATEMENT_NODES

#include "ParseTreeNode.h"
#include "ExpressionNodes.h"
#include "ExpressionListNodes.h"

//statements
class CompoundStatement : Statement {
  Statement stm1, stm2;
  CompoundStatement(Statement stm1, Statement stm2);
};
class AssignStatement : Statement {
  IdExpression id;
  Expression exp;
  AssignStatement(IdExpression id, Expression exp);
};
class PrintStatement : Statement {
  ExpressionList list;
  PrintStatement(ExpressionList list);
};

#endif
