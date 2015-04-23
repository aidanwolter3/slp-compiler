// Aidan Wolter
// Program Translation - COSC 4503
// 2/26/2015

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LexicalAnalyzer.h"
#include "symbol_table/SymbolTable.h"
#include "csv/csv.h"
#include "parse_tree/ParseTree.h"

class SyntaxAnalyzer {
  private:
    colFromToken(Token t);
    LexicalAnalyzer *lexicalAnalyzer;
    SymbolTable *symbolTable;
    ParseTree *parseTree;
    CSV *parseTable;
    void throw_unexpected_token(Token t, int state);
  public:
    int rows;
    int cols;
    SyntaxAnalyzer(FILE *syn_file, LexicalAnalyzer *lexicalAnalyzer, SymbolTable *symbolTable, ParseTree *parseTree);
    int parse();
};

