// Aidan Wolter
// Program Translation - COSC 4503

#ifndef LEXICAL_ANALYZER
#define LEXICAL_ANALYZER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "csv/csv.h"

class LexTable {
  public:
    class Token {
      public:
        int t;
        char l[256];
        Token(int token, char lexem);
    };
    LexTable(FILE *lex_file, FILE *infile);
    Token* nextToken();
  private:

    CSV *table;
    FILE *infile;
    char *matches[256];
    int rows;
    int cols;

    void throwUnknownToken(char *line, int lin_index, int line_number, Token *t);
};

#endif
