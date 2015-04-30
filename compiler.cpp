// Aidan Wolter
// Program Translation - COSC 4503
// 
// Description:

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "src/symbol_table/SymbolTable.h"
#include "src/LexicalAnalyzer.h"
#include "src/SyntaxAnalyzer.h"

#include "src/parse_tree/ParseTree.h"
#include "src/parse_tree/VariableEvaluatorVisitor.h"
#include "src/parse_tree/PrettyPrintVisitor.h"
#include "src/parse_tree/CodeGenerator_x86.h"

//main method
int main(int argc, char *argv[]) {

  //initialize the symbol table and parse tree
  SymbolTable *symbolTable = new SymbolTable();
  ParseTree *parseTree = new ParseTree();

  //get the lexical analyzer input file
  FILE *lex_file = fopen("lex_table.csv", "r");
  if(lex_file == NULL) {
    printf("Could not find lex syntax file!\n");
    printf("The file should be placed in the same directory and named 'lex_table.csv'\n");
    return 0;
  }

  //get the code input file
  if(argc < 2) {
    printf("Please add the input file to the arguments\n");
    printf("(example ./compiler test)\n");
    return 0;
  }
  FILE *infile = fopen(argv[1], "r");
  if(infile == NULL) {
    printf("Could not find the input file: %s\n", argv[1]);
    printf("Please ensure that it exists\n");
    return 0;
  }

  //get the syntax analyzer input file
  FILE *syn_file = fopen("parse_table.csv", "r");
  if(syn_file == NULL) {
    printf("Could not find the parse table file!\n");
    printf("The file should be placed in the same directory and named 'parse_table.csv'\n");
    return 0;
  }

  //create the lexical analyzer and syntax analyzer
  LexicalAnalyzer *lexicalAnalyzer = new LexicalAnalyzer(lex_file, infile);
  SyntaxAnalyzer *syntaxAnalyzer = new SyntaxAnalyzer(syn_file, lexicalAnalyzer, symbolTable, parseTree);

  //close the opened files
  fclose(lex_file);
  fclose(syn_file);

  //complete the parsing
  int ret = syntaxAnalyzer->parse();
  if(ret == 0) {
    printf("The file has proper syntax\n");

    printf("\nPrettyPrintVisitor results:\n");
    PrettyPrintVisitor *v = new PrettyPrintVisitor();
    parseTree->get_root()->accept(v);

    //printf("\n\nVariableEvaluatorVisitor results:\n");
    //VariableEvaluatorVisitor v2 = VariableEvaluatorVisitor();
    //parseTree.get_root()->accept(&v2);
    //v2.printVariables();

    printf("\n\nStarting target code generation...\n");
    const char *target_mac = "mac";
    if(argc >= 3) {
      CodeGenerator_x86 *c = new CodeGenerator_x86(argv[2], symbolTable);
      parseTree->get_root()->accept(c);
      c->print_code();
    }
    else { //default to mac
      CodeGenerator_x86 *c = new CodeGenerator_x86(target_mac, symbolTable);
      parseTree->get_root()->accept(c);
      c->print_code();
    }
  }

  fclose(infile);

  //symbol_table_dump();
  return 0;
}
