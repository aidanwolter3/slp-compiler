#ifndef MINUS
#define MINUS

#include "ParseTreeNode.h"

class Minus : public BinaryOperation {
  public:
    void accept(Visitor *v);
};

#endif
