#ifndef COMPOUND_STATEMENT
#define COMPOUND_STATEMENT

#include "ParseTreeNode.h"

class CompoundStatement : public Statement {
  public:
    Statement *stm1, *stm2;
    CompoundStatement(Statement *s1, Statement *s2);
    void* accept(Visitor *v);
};

#endif
