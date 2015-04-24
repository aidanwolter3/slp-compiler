// Aidan Wolter
// Program Translation - COSC 4503

#ifndef LEXICAL_ANALYZER
#define LEXICAL_ANALYZER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "csv/csv.h"

class Token {
  public:
    int t;
    char l[256];
    Token(int token, char *lexem) {t = token; strcpy(l, lexem);}
};

class LexicalAnalyzer {
  private:
    CSV *lex_table;
    int table[256][256];
    FILE *infile;
    char *matches[256];
    int rows;
    int cols;

    void throwUnknownToken(char *line, int lin_index, int line_number, Token *t);
  public:
    LexicalAnalyzer(FILE *lex_file, FILE *infile);
    Token* nextToken();
};

#endif
