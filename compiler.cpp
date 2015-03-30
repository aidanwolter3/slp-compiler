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
//#include "ParseTree.h"

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
  if(argc == 1) {
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

  //create the parse tree
  //ParseTree parseTree = ParseTree();
  //char id_lexem[2];
  //id_lexem[0] = 'a';
  //id_lexem[1] = '0';
  //IdExpression id = IdExpression(id_lexem);
  //parseTree.push(IdExpression);
  //PrettyPrintVisitor v = PrettyPrintVisitor();
  //parseTree.get_root().accept(v);

  //complete the parsing
  syntax_analyzer_parse(infile);

  symbol_table_dump();
  return 0;
}
