#include <stdlib.h>
#include "ParseTree.h"

ParseTree::ParseTree() {
  nodes = (ParseTreeNode*)malloc(100*sizeof(ParseTreeNode));
    //should change to be dynamic or something
  size = 0;
}

ParseTreeNode ParseTree::get_root() {
  return nodes[0];
}

ParseTreeNode ParseTree::pop() {
  size--;
  return nodes[size];
}

void ParseTree::push(ParseTreeNode new_node) {
  nodes[size] = new_node;
  size++;
}
