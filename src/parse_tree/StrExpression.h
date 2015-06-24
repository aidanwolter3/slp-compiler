#ifndef STR_EXPRESSION
#define STR_EXPRESSION

#include "ParseTreeNode.h"

class StrExpression : public Expression {
  public:
    char *val;
    StrExpression(char *val);
    void* accept(Visitor *v);
};

#endif
