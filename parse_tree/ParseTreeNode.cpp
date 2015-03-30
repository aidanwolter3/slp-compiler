#include "ParseTreeNode.h"

class ParseTreeNode {}

class Statement : ParseTreeNode {}
class CompoundStatement : Statement {
  void accept(Visitor v) {
    v.visit(this);
  }
}
class AssignStatement : Statement {
  void accept(Visitor v) {
    v.visit(this);
  }
}
class PrintStatement : Statement {
  void accept(Visitor v) {
    v.visit(this);
  }
}

class Expression : ParseTreeNode {
  void accept(Visitor v) {
    v.visit(this);
  }
}
class IdExpression : Expression {
  IdExpression(char *l) {
    lexem = l;
  }
  void accept(Visitor v) {
    v.visit(this);
  }
}
class NumExpression : Expression {
  NumExpression(int v) {
    val = v;
  }
  void accept(Visitor v) {
    v.visit(this);
  }
}
class OperationExpression : Expression {
  void accept(Visitor v) {
    v.visit(this);
  }
}
class SequenceExpression : Expression {
  void accept(Visitor v) {
    v.visit(this);
  }
}

class ExpressionList : ParseTreeNode {}
class PairExpressionList : ExpressionList {
  void accept(Visitor v) {
    v.visit(this);
  }
}
class LastExpressionList : ExpressionList {
  void accept(Visitor v) {
    v.visit(this);
  }
}

class BinaryOperation : ParseTreeNode {}
class Plus : BinaryOperation {
  void accept(Visitor v) {
    v.visit(this);
  }
}
class Minus : BinaryOperation {
  void accept(Visitor v) {
    v.visit(this);
  }
}
class Divide : BinaryOperation {
  void accept(Visitor v) {
    v.visit(this);
  }
}
class Multiply : BinaryOperation {
  void accept(Visitor v) {
    v.visit(this);
  }
}
