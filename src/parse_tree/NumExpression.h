#ifndef NUM_EXPRESSION
#define NUM_EXPRESSION

#include "ParseTreeNode.h"

class NumExpression : public Expression {
  public:
    int val;
    NumExpression(int v);
    void accept(Visitor *v);
};

#endif
