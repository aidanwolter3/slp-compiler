// Aidan Wolter
// Program Translation - COSC 4503

#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

class SymbolTable {
  private:

    class Symbol {
      public:
        char *name;
        int t;
        int type;
        int loc;
    };

  public:
    Symbol *symbols[100];
    int size;

    SymbolTable();
    dump();
    int add(int token, char *lexem, int type);
};

#endif
