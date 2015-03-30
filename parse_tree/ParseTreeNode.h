#ifndef PARSE_TREE_NODE
#define PARSE_TREE_NODE

#include "Visitor.h"

//root class
class ParseTreeNode {
  void accept(Visitor v);
};
class Statement : ParseTreeNode {};
class ExpressionList : ParseTreeNode {};
class BinaryOperation : ParseTreeNode {};
class Expression : ParseTreeNode {};

#endif
