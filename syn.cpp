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

#include "symbol_table.h"
#include "lexical_analyzer.h"
#include "parse_csv.h"

extern struct symbol_table_s sym_table;
extern struct lex_table_s lex_table;

int parse_table_col_from_token(struct token_s t, char ***parse_table, int cols);

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
  build_lex(sfile);
  fclose(sfile);


  //get the input file
  if(argc == 1) {
    printf("Please add the input file to the arguments\n");
    printf("(example ./syn test)\n");
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
  int parse_table_rows;
  int parse_table_cols;
  char ***parse_table = parse_csv(pfile, &parse_table_rows, &parse_table_cols);

  //continue parsing until error or accept
  int line = 1;
  int t_stack[256]; //token stack
  int s_stack[256]; //state stack
  s_stack[0] = 0;   //start in state 0
  int t_stack_cnt = 0;
  int s_stack_cnt = 1;
  struct token_s t = lex_next_token(infile);

  if(t.t == 1) {
    symbol_table_add(t.t, t.l, 0); //type = 0 for now
  }

  while(true) {

    //error in lexer
    if(t.t == -1) {
      symbol_table_dump();
      break;
    }

    //newline
    else if(t.t == -2) {
      line++;

      //get another token
      t = lex_next_token(infile);
      if(t.t == 1) {
        symbol_table_add(t.t, t.l, 0); //type = 0 for now
      }
    }

    //follow the table
    else {

      //get the command from the table
      //state+1 because we do not use the header of the table
      //token-1 because token 0 is invalid
      int state = s_stack[s_stack_cnt-1];
      int token_index = parse_table_col_from_token(t, parse_table, parse_table_cols);
      char *cmd = parse_table[state+2][token_index];

      //cell is empty, so report expecting something else
      if(strlen(cmd) == 0) {

        //get valid options
        char *options[256];
        int options_len = 0;
        for(int i = 0; i < parse_table_cols; i++) {
          if(strlen(parse_table[state+2][i]) > 0) {
            options[options_len++] = parse_table[0][i];
          }
        }

        //build the options string
        char options_str[256];
        options_str[0] = '{';
        int options_str_len = 1;
        for(int i = 0; i < options_len; i++) {

          //add the option
          strcpy(&options_str[options_str_len], options[i]);
          options_str_len += strlen(options[i]);

          //only add a comma if not the last option
          if(i < options_len-1) {
            options_str[options_str_len++] = ',';
          }
        }
        options_str[options_str_len++] = '}';
        options_str[options_str_len] = '\0';

        printf("Error: Syntax analyzer found '%s' on line %d while expecting one of the set %s\n", t.l, line, options_str);
        return 0;
      }

      #ifdef DEBUG
      printf("cmd: %s\n", cmd);
      #endif

      //reduction
      if(cmd[0] == 'r') {

        char *stopstr;
        int prod = strtod(&cmd[1], &stopstr);
        int pop_size; //how much to pop off the stacks
        struct token_s rep_token;//what to push onto the token stack

        //check each production

        //S;S
        if(prod == 1) {
          pop_size = 3;
          rep_token.t = 13;
          rep_token.l[0] = 'S';
        }

        //id:=E
        else if(prod == 2) {
          pop_size = 3;
          rep_token.t = 13;
          rep_token.l[0] = 'S';
        }

        //print(L)
        else if(prod == 3) {
          pop_size = 4;
          rep_token.t = 13;
          rep_token.l[0] = 'S';
        }

        //epsilon
        else if(prod == 4) {
          pop_size = 0;
          rep_token.t = 13;
          rep_token.l[0] = 'S';
        }

        //id
        else if(prod == 5) {
          pop_size = 1;
          rep_token.t = 14;
          rep_token.l[0] = 'E';
        }

        //num
        else if(prod == 6) {
          pop_size = 1;
          rep_token.t = 14;
          rep_token.l[0] = 'E';
        }

        //EBE
        else if(prod == 7) {
          pop_size = 3;
          rep_token.t = 14;
          rep_token.l[0] = 'E';
        }

        //(S,E)
        else if(prod == 8) {
          pop_size = 5;
          rep_token.t = 14;
          rep_token.l[0] = 'E';
        }

        //E,L
        else if(prod == 9) {
          pop_size = 3;
          rep_token.t = 15;
          rep_token.l[0] = 'L';
        }

        //E
        else if(prod == 10) {
          pop_size = 1;
          rep_token.t = 15;
          rep_token.l[0] = 'L';
        }

        //+
        else if(prod == 11) {
          pop_size = 1;
          rep_token.t = 16;
          rep_token.l[0] = 'B';
        }

        //-
        else if(prod == 12) {
          pop_size = 1;
          rep_token.t = 16;
          rep_token.l[0] = 'B';
        }
        
        //*
        else if(prod == 13) {
          pop_size = 1;
          rep_token.t = 16;
          rep_token.l[0] = 'B';
        }

        // /
        else if(prod == 14) {
          pop_size = 1;
          rep_token.t = 16;
          rep_token.l[0] = 'B';
        }

        //pop off the size of the production
        s_stack_cnt -= pop_size;
        t_stack_cnt -= pop_size;

        //push the replacement token
        t_stack[t_stack_cnt++] = rep_token.t;

        //push the new state from the goto
        int cur_state = s_stack[s_stack_cnt-1];
        int new_token_index = parse_table_col_from_token(rep_token, parse_table, parse_table_cols);
        char *cmd_goto = parse_table[cur_state+2][new_token_index];
        s_stack[s_stack_cnt++] = strtod(cmd_goto, &stopstr);
      }

      //shift
      else if(cmd[0] == 's') {
        char *stopstr;
        int newstate = strtod(&cmd[1], &stopstr);
        t_stack[t_stack_cnt++] = t.t;
        s_stack[s_stack_cnt++] = newstate;

        //get another token
        t = lex_next_token(infile);
        if(t.t == 1) {
          symbol_table_add(t.t, t.l, 0); //type = 0 for now
        }
      }

      //accept command
      else if(cmd[0] == 'a') {
        printf("The file has proper syntax\n");
        symbol_table_dump();
        return 0;
      }

      //invalid command
      else {
        printf("Error: Parse table contains unknown command (%s)\n", cmd);
        symbol_table_dump();
        return 0;
      }


      #ifdef DEBUG
      printf("t_stack: ");
      for(int i = 0; i < t_stack_cnt; i++) {
        printf("%d, ", t_stack[i]);
      }
      printf("\ns_stack: ");
      for(int i = 0; i < s_stack_cnt; i++) {
        printf("%d, ", s_stack[i]);
      }
      printf("\n");
      #endif
    }

    
  }

  return 0;
}

//search for the token in the parse table and return the col index
int parse_table_col_from_token(struct token_s t, char ***parse_table, int cols) {
  for(int i = 0; i < cols; i++) {
    int tmp_token = strtod(parse_table[1][i], NULL);
    if(tmp_token == t.t) {
      return i;
    }
  }
  return -1;
}
