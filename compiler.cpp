// Aidan Wolter
// Syntax Analyzer - Program Assignment #2
// Program Translation - COSC 4503
// 2/26/2015
// 
// Description:
//  This program completes lexical analysis, syntax analysis, and preliminary
//  construction of a symbol table. The input file type is a predefined straight
//  line programming language. The program requires two language definition files:
//  lex_table.csv & parse_table.csv to be located in the same directory. The program
//  checks for proper syntax and indicates where errors are if they exist.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "src/symbol_table/symbol_table.h"
#include "src/lexical_analyzer.h"
#include "src/syntax_analyzer.h"

#include "src/parse_tree/ParseTree.h"
#include "src/parse_tree/PrettyPrintVisitor.cpp"
#include "src/parse_tree/VariableEvaluatorVisitor.h"
#include "src/parse_tree/CodeGenerator_x86.h"

extern struct symbol_table_s sym_table;


//main method
int main(int argc, char *argv[]) {

  symbol_table_init();

  //create the lexical syntax structure
  FILE *sfile = fopen("lex_table.csv", "r");
  if(sfile == NULL) {
    printf("Could not find lex syntax file!\n");
    printf("The file should be placed in the same directory and named 'lex_table.csv'\n");
    return 0;
  }
  build_lex_table(sfile);
  fclose(sfile);


  //get the input file
  if(argc < 2) {
    printf("Please add the input file to the arguments\n");
    printf("(example ./compiler test)\n");
    return 0;
  }
  FILE *infile = fopen(argv[1], "r");
  if(infile == NULL) {
    printf("Could not find the input file: %s\n", argv[2]);
    printf("Please ensure that it exists\n");
    return 0;
  }


  //create the parse table
  FILE *pfile = fopen("parse_table.csv", "r");
  if(pfile == NULL) {
    printf("Could not find the parse table file!\n");
    printf("The file should be placed in the same directory and named 'parse_table.csv'\n");
    return 0;
  }
  build_parse_table(pfile);
  fclose(pfile);

  //initialize the parse tree
  ParseTree parseTree = ParseTree();

  //complete the parsing
  int ret = syntax_analyzer_parse(infile, parseTree);
  if(ret == 0) {
    printf("The file has proper syntax\n");

    printf("\nPrettyPrintVisitor results:\n");
    PrettyPrintVisitor v = PrettyPrintVisitor();
    parseTree.get_root()->accept(&v);

    //printf("\n\nVariableEvaluatorVisitor results:\n");
    //VariableEvaluatorVisitor v2 = VariableEvaluatorVisitor();
    //parseTree.get_root()->accept(&v2);
    //v2.printVariables();

    printf("\n\nStarting target code generation...\n");
    const char *target_mac = "mac"+0;
    if(argc >= 3) {
      CodeGenerator_x86 *c = new CodeGenerator_x86(argv[2]);
      parseTree.get_root()->accept(c);
      c->print_code();
    }
    else { //default to mac
      CodeGenerator_x86 *c = new CodeGenerator_x86(target_mac);
      parseTree.get_root()->accept(c);
      c->print_code();
    }
  }

  //symbol_table_dump();
  return 0;
}
