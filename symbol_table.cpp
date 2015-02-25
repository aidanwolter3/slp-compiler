// Aidan Wolter
// Syntax Analyzer - Program Assignment #2
// Program Translation - COSC 4503
// 2/26/2015

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "symbol_table.h"

//global symbol table
struct symbol_table_s sym_table;

void symbol_table_init() {
  sym_table.size = 0;
}

//dump the symbol table to stdout
void symbol_table_dump() {
  printf("\nSymbol Table:\n");
  for(int i = 0; i < sym_table.size; i++) {
    printf("%d %s\n", sym_table.d[i].t, sym_table.d[i].name);
  }
}

//add a symbol to the symbol table
int symbol_table_add(int token, char *lexem, int type) {

  //add the token to the symbol table if needed
  bool sym_found = false;
  for(int i = 0; i < sym_table.size; i++) {
    if(strcmp(sym_table.d[i].name, lexem) == 0) {
      sym_found = true;
      break;
    }
  }
  if(!sym_found) {
    struct symbol_s sym;
    sym.t = token;
    sym.type = 0; //initialize to 0 for now
    sym.name = (char*)malloc(sizeof(lexem));
    strcpy(sym.name, lexem);
    sym_table.d[sym_table.size++] = sym;
  }

  return sym_found ? 0 : -1;
}
