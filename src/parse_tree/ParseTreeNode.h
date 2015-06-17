#ifndef VISITOR
#define VISITOR

class CompoundStatement;
class AssignStatement;
class PrintStatement;
class IdExpression;
class NumExpression;
class OperationExpression;
class PairExpressionList;
class LastExpressionList;
class Plus;
class Minus;
class Divide;
class Multiply;
class Visitor {
  public:
    virtual void* visit(CompoundStatement *s) = 0;
    virtual void* visit(AssignStatement *s) = 0;
    virtual void* visit(PrintStatement *s) = 0;
    virtual void* visit(IdExpression *s) = 0;
    virtual void* visit(NumExpression *s) = 0;
    virtual void* visit(OperationExpression *s) = 0;
    virtual void* visit(PairExpressionList *s) = 0;
    virtual void* visit(LastExpressionList *s) = 0;
    virtual void* visit(Plus *s) = 0;
    virtual void* visit(Minus *s) = 0;
    virtual void* visit(Divide *s) = 0;
    virtual void* visit(Multiply *s) = 0;
};

#endif

#ifndef PARSE_TREE_NODE
#define PARSE_TREE_NODE

//abstract root node class
class ParseTreeNode {
  public:
    virtual void* accept(Visitor *v) = 0;
};

//branches off root node class
//has virtual functions to be overridden
class Statement : public ParseTreeNode {
  public:
    void* accept(Visitor *v) {return 0;}
};
class ExpressionList : public ParseTreeNode {
  public:
    void* accept(Visitor *v) {return 0;}
};
class BinaryOperation : public ParseTreeNode {
  public:
    void* accept(Visitor *v) {return 0;}
};
class Expression : public ParseTreeNode {
  public:
    void* accept(Visitor *v) {return 0;}
};

#endif
