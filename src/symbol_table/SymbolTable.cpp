// Aidan Wolter
// Program Translation - COSC 4503

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SymbolTable.h"

SymbolTable::SymbolTable() {
  size = 0;
}

//calculate the current size of the table
int SymbolTable::data_size() {
  int s = 0;
  for(int i = 0; i < size; i++) {
    s += symbols[i]->size;
  }
  return s;
}

//calculate the correct locations in the frame for each variable
void SymbolTable::calculate_locations() {
  int cur_ptr = 0;
  for(int i = 0; i < size; i++) {
    cur_ptr += symbols[i]->size;
    symbols[i]->loc = cur_ptr;
  }
}

//dump the symbol table to stdout
void SymbolTable::dump() {
  printf("\nSymbol Table:\n");
  for(int i = 0; i < size; i++) {
    printf("%d %s\n", symbols[i]->t, symbols[i]->name);
  }
}

//add a symbol to the symbol table
int SymbolTable::add(int token, char *lexem, int type, int size) {

  //add the token to the symbol table if needed
  bool sym_found = false;
  for(int i = 0; i < this->size; i++) {
    if(strcmp(symbols[i]->name, lexem) == 0) {
      sym_found = true;
      break;
    }
  }
  if(!sym_found) {
    Symbol *sym = new Symbol();
    sym->t = token;
    sym->type = 0; //initialize to 0 for now
    sym->loc = 0; // set the offset on the stack to zero initially
    sym->name = (char*)malloc(sizeof(lexem));
    strcpy(sym->name, lexem);
    symbols[this->size++] = sym;
  }

  return sym_found ? 0 : -1;
}

Symbol* SymbolTable::get(char *lexem) {
  for(int i = 0; i < size; i++) {
    if(strcmp(symbols[i]->name, lexem) == 0) {
      return symbols[i];
    }
  }
  return new Symbol();
}
