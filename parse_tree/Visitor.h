#ifndef VISITOR
#define VISITOR

#include "StatementNodes.h"
#include "ExpressionNodes.h"
#include "ExpressionListNodes.h"
#include "BinaryOperationNodes.h"

class Visitor {
  void visit(CompoundStatement stm);
  void visit(AssignStatement stm);
  void visit(PrintStatement stm);
  void visit(IdExpression exp);
  void visit(NumExpression exp);
  void visit(OperationExpression exp);
  void visit(SequenceExpression exp);
  void visit(PairExpressionList exp);
  void visit(LastExpressionList exp);
  void visit(Plus op);
  void visit(Minus op);
  void visit(Divide op);
  void visit(Multiply op);
};

#endif
