#ifndef COMPOUND_STATEMENT
#define COMPOUND_STATEMENT

#include "ParseTreeNode.h"

class CompoundStatement : Statement {
  Statement stm1, stm2;
  CompoundStatement(Statement stm1, Statement stm2);
};

#endif
