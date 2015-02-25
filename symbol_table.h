// Aidan Wolter
// Syntax Analyzer - Program Assignment #2
// Program Translation - COSC 4503
// 2/26/2015

//a single symbol for the symbol table
struct symbol {
  char *name;
  int t;
  int type;
};

//symbol table
struct symbol_table {
  struct symbol d[100];
  int size;
};

//methods
void symbol_table_init();
void symbol_table_dump();
int symbol_table_add(int token, char *lexem, int type);
