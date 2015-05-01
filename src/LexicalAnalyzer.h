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

    //current line buffer
    char *cur_line;
    int cur_line_cnt;
    int line_index;
    int line_number;

    //table from lex csv
    CSV *lex_table;

    //table with only productions
    int table[256][256];
    int rows;
    int cols;

    //input file
    FILE *infile;

    //strings to match lexems with
    char *matches[256];

    void throwUnknownToken(char *line, int lin_index, int line_number, Token *t);
  public:
    LexicalAnalyzer(FILE *lex_file, FILE *infile);
    Token* nextToken();

    //fetch methods for private vars
    char *getCurLine();
    int getLineIndex();
    int getLineNumber();
};

#endif
