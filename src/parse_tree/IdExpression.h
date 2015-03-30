#ifndef ID_EXPRESSION
#define ID_EXPRESSION

#include "ParseTreeNode.h"

class IdExpression : public Expression {
  public:
    char *lexem;
    IdExpression(char* l);
    void accept(Visitor *v);
};

#endif
