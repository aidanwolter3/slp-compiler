// Aidan Wolter
// Syntax Analyzer - Program Assignment #2
// Program Translation - COSC 4503
// 2/26/2015

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse_csv.h"

//a token including the decimal value 't' and the lexem 'l'
struct token_s {
  int t;
  char l[256];
};

//the lex structure that defines valid strings of characters for their tokens
struct lex_table_s {
  int table[256][256];
  char *matches[256];
  int rows;
  int cols;
};

void build_lex(FILE *sfile);
struct token_s lex_next_token(FILE *infile);
void throw_unknown_token(char *line, int line_index, int line_number, struct token_s t);
