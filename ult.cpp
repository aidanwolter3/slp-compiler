// Aidan Wolter
// Program Translation - COSC 4503
// 
// Description:

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "src/symbol_table/SymbolTable.h"
#include "src/LexicalAnalyzer.h"
#include "src/SyntaxAnalyzer.h"

#include "src/parse_tree/ParseTree.h"
#include "src/parse_tree/VariableEvaluatorVisitor.h"
#include "src/parse_tree/PrettyPrintVisitor.h"
#include "src/parse_tree/CodeGenerator_macho32_osx.h"
#include "src/parse_tree/CodeGenerator_macho32_ubu.h"
#include "src/parse_tree/CodeGenerator_macho64_osx.h"

//main method
int main(int argc, char *argv[]) {

  //set the default arguments
  char target[10] = "osx";
  char abi[10] = "macho64";
  char output[PATH_MAX] = "output.asm";
  char *input;

  //parse the arguments
  for(int i = 1; i < argc; i++) {

    //format
    if(strcmp(argv[i], "-f") == 0) {
      if(i+1 >= argc) {
        printf("Format not specified\n");
        return 0;
      }
      strcpy(abi, argv[i+1]);
      i++;
      if(strcmp(abi, "macho") != 0 &&
         strcmp(abi, "macho32") != 0 &&
         strcmp(abi, "macho64") != 0) {
        printf("Unknown format: %s\n", abi);
        return 0;
      }
    }

    //target
    else if(strcmp(argv[i], "-t") == 0 ||
            strcmp(argv[i], "--target") == 0) {
      if(i+1 >= argc) {
        printf("Target not specified\n");
        return 0;
      }
      strcpy(target, argv[i+1]);
      i++;
      if(strcmp(target, "osx") != 0 &&
         strcmp(target, "win") != 0 &&
         strcmp(target, "ubu") != 0) {
        printf("Unknown target: %s\n", target);
        return 0;
      }
    }

    //output file
    else if(strcmp(argv[i], "-o") == 0 ||
            strcmp(argv[i], "--output") == 0) {
      if(i+1 >= argc) {
        printf("Output file not specified\n");
        return 0;
      }
      strcpy(output, argv[i+1]);
      i++;
    }

    //input file
    else if(input == NULL) {
      input = (char*)malloc(sizeof(argv[i]));
      strcpy(input, argv[i]);
    }

    //invalid
    else {
      printf("Unknown argument: %s\n", argv[i]);
      return 0;
    }
  }

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
  FILE *infile = fopen(input, "r");
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
    if(strcmp(target, "osx") == 0) {
      if(strcmp(abi, "macho32") == 0) {
        CodeGenerator_macho32_osx *c = new CodeGenerator_macho32_osx(symbolTable, output);
        parseTree->get_root()->accept(c);
        c->write_exit();
        c->write_code();
      }
      else if(strcmp(abi, "macho64") == 0) {
        CodeGenerator_macho64_osx *c = new CodeGenerator_macho64_osx(symbolTable, output);
        parseTree->get_root()->accept(c);
        c->write_exit();
        c->write_code();
      }
      else {
        printf("Currently, the compatible Ubuntu formats are:\n\tmacho32\n\tmacho64\n");
      }
    }
    else if(strcmp(target, "win") == 0) {
      printf("Windows is not currently supported\n");
    }
    else if(strcmp(target, "ubu") == 0) {
      if(strcmp(abi, "macho32") == 0) {
        CodeGenerator_macho32_ubu *c = new CodeGenerator_macho32_ubu(symbolTable, output);
        parseTree->get_root()->accept(c);
        c->write_exit();
        c->write_code();
      }
      else {
        printf("Currently, the compatible Ubuntu formats are:\n\tmacho32\n");
      }
    }
  }

  fclose(infile);
  return 0;
}
