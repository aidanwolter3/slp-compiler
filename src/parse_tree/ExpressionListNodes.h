#ifndef EXPRESSION_LIST_NODES
#define EXPRESSION_LIST_NODES

#include "ParseTreeNode.h"
#include "ExpressionNodes.h"

//expression list
class PairExpressionList : ExpressionList {
  Expression exp;
  ExpressionList exp_list;
  PairExpressionList(Expression exp, ExpressionList exp_list);
};
class LastExpressionList : ExpressionList {
  Expression exp;
  LastExpressionList(Expression exp);
};

#endif
