#include "ParseTreeNode.h"

class ParseTree {
  private:
    ParseTreeNode *nodes;
    int size;

  public:
    ParseTree();

    ParseTreeNode get_root();
    ParseTreeNode pop();
    void push(ParseTreeNode new_node);
};
