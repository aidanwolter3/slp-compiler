// Aidan Wolter
// Syntax Analyzer - Program Assignment #2
// Program Translation - COSC 4503
// 2/26/2015

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lexical_analyzer.h"
#include "symbol_table.h"
#include "parse_csv.h"

struct parse_table_s {
  int rows;
  int cols;
  char ***d;
};

void build_parse_table(FILE *pfile);
int syntax_analyzer_parse(FILE *infile);
int parse_table_col_from_token(struct token_s t);
void throw_unexpected_token(struct token_s t, int state);
