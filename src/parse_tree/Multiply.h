#ifndef MULTIPLY
#define MULTIPLY

#include "ParseTreeNode.h"

class Multiply : public BinaryOperation {
  public:
    void* accept(Visitor *v);
};

#endif
