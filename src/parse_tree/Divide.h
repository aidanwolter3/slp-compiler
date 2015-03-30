#ifndef DIVIDE
#define DIVIDE

#include "ParseTreeNode.h"

class Divide : public BinaryOperation {
  public:
    void accept(Visitor *v);
};

#endif
