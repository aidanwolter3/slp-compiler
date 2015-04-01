#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "VariableTable.h"

VariableTable::VariableTable() {
  entries = (VariableTableEntry**)malloc(100*sizeof(VariableTableEntry*));
  size = 0;
}
void VariableTable::set(char *lexem, int val) {

  //search for variable (do not worry about scoping)
  int i;
  for(i = 0; i < size; i++) {

    //if found a match, break and overwrite
    if(strcmp(entries[i]->lexem, lexem) == 0) {
      break;
    }
  }

  entries[i] = new VariableTableEntry(lexem, val);
  if(i == size) {
    size++;
  }
}
int VariableTable::get(char *lexem) {
  for(int i = 0; i < size; i++) {
    if(strcmp(entries[i]->lexem, lexem) == 0) {
      return entries[i]->val; //return value
    }
  }
  return -1; //error
}
void VariableTable::print() {
  for(int i = 0; i < size; i++) {
    printf("%s: %d\n", entries[i]->lexem, entries[i]->val);
  }
}
