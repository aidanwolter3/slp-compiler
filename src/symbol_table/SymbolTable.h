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
    int size;
};

class SymbolTable {
  public:
    Symbol *symbols[100];
    int size;

    SymbolTable();
    int data_size();
    void calculate_locations();
    void dump();
    int add(int token, char *lexem, int type, int size);
    Symbol* get(char *lexem);
};

#endif
