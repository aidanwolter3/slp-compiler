// Aidan Wolter
// Syntax Analyzer - Program Assignment #2
// Program Translation - COSC 4503
// 2/26/2015

#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

//a single symbol for the symbol table
struct symbol_s {
  char *name;
  int t;
  int type;
};

//symbol table
struct symbol_table_s {
  struct symbol_s d[100];
  int size;
};

//methods
void symbol_table_init();
void symbol_table_dump();
int symbol_table_add(int token, char *lexem, int type);
struct symbol_s symbol_table_get_last();

#endif
