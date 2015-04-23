// Aidan Wolter
// Program Translation - COSC 4503

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SymbolTable.h"

SymbolTable::SymbolTable() {
  size = 0;
}

//dump the symbol table to stdout
SymbolTable::dump() {
  printf("\nSymbol Table:\n");
  for(int i = 0; i < size; i++) {
    printf("%d %s\n", symbols[i]->t, symbols[i]->name);
  }
}

//add a symbol to the symbol table
int SymbolTable::add(int token, char *lexem, int type) {

  //add the token to the symbol table if needed
  bool sym_found = false;
  for(int i = 0; i < size; i++) {
    if(strcmp(symbols[i]->name, lexem) == 0) {
      sym_found = true;
      break;
    }
  }
  if(!sym_found) {
    Symbol *sym = new Symbol();
    sym->t = token;
    sym->type = 0; //initialize to 0 for now
    sym->loc = (sym_table.size+1)*4; // set the offset on the stack
    sym->name = (char*)malloc(sizeof(lexem));
    strcpy(sym->name, lexem);
    symbols[sym_table.size++] = sym;
  }

  return sym_found ? 0 : -1;
}

//int symbol_table_get_symbol_loc(char *lexem) {
//  bool sym_found = false;
//  for(int i = 0; i < sym_table.size; i++) {
//    if(strcmp(sym_table.d[i].name, lexem) == 0) {
//      return sym_table.d[i].loc;
//    }
//  }
//  return -1;
//}

