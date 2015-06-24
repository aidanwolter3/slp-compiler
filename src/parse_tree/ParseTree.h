#ifndef PARSE_TREE
#define PARSE_TREE

#include "ParseTreeNode.h"
#include "CompoundStatement.h"
#include "AssignStatement.h"
#include "PrintStatement.h"
#include "IdExpression.h"
#include "NumExpression.h"
#include "StrExpression.h"
#include "OperationExpression.h"
#include "PairExpressionList.h"
#include "LastExpressionList.h"
#include "Plus.h"
#include "Minus.h"
#include "Divide.h"
#include "Multiply.h"

class ParseTree {
  private:
    ParseTreeNode **nodes;
    int size;

  public:
    ParseTree();

    ParseTreeNode* get_root();
    ParseTreeNode* pop();
    void push(ParseTreeNode *new_node);
};

#endif
