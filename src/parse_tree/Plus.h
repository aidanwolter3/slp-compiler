#ifndef PLUS
#define PLUS

#include "ParseTreeNode.h"

class Plus : public BinaryOperation {
  public:
    void* accept(Visitor *v);
};

#endif
