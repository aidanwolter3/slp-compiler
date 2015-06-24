// Aidan Wolter
// Program Translation - COSC 4503

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ult.h"
#include "LexicalAnalyzer.h"
#include "symbol_table/SymbolTable.h"
#include "csv/csv.h"
#include "parse_tree/ParseTree.h"

class SyntaxAnalyzer {
  private:
    LexicalAnalyzer *lexicalAnalyzer;
    SymbolTable *symbolTable;
    ParseTree *parseTree;

    CSV *parseTable;

    int colFromToken(Token *t);
    void throw_unexpected_token(Token *t, int state, char *cur_line);

    int last_line_index;
    int last_line_number;
  public:
    int rows;
    int cols;
    SyntaxAnalyzer(FILE *syn_file, LexicalAnalyzer *lexicalAnalyzer, SymbolTable *symbolTable, ParseTree *parseTree);
    int parse();
};

