// Aidan Wolter
// Program Translation - COSC 4503

#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

class Symbol {
  public:
    char *name;
    int t;
    int type;
    int loc;
};

class SymbolTable {
  public:
    Symbol *symbols[100];
    int size;

    SymbolTable();
    void dump();
    int add(int token, char *lexem, int type);
    Symbol* get(char *lexem);
};

#endif
