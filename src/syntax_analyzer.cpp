// Aidan Wolter
// Syntax Analyzer - Program Assignment #2
// Program Translation - COSC 4503
// 2/26/2015

#include "syntax_analyzer.h"
#include <stdlib.h>
#include <string.h>

//#define SYNDEBUG

//get the global lex table
extern struct lex_table_s lex_table;
extern char *cur_line;
extern int line_number;
extern int line_index;

//global parse table
struct parse_table_s parse_table;

//keep track of where the error is even when finishing off the line
int last_line_index = 0;
int last_line_number = 1;

//construct the parse table from the csv
void build_parse_table(FILE *pfile) {
  parse_table.d = parse_csv(pfile, &parse_table.rows, &parse_table.cols);
}

//parse the parse table
int syntax_analyzer_parse(FILE *infile) {

  //initialize the parse tree
  ParseTree parseTree = ParseTree();

  bool error_found = false;

  //continue parsing until error or accept
  struct token_s *t_stack[256]; //token stack
  int s_stack[256]; //state stack
  s_stack[0] = 0;   //start in state 0
  int t_stack_cnt = 0;
  int s_stack_cnt = 1;
  struct token_s t = lex_next_token(infile);
  struct token_s last_t = {};
  char *stopstr;

  if(t.t == 1) {
    symbol_table_add(t.t, t.l, 0); //type = 0 for now
  }

  while(true) {

    //error in lexer, so continue
    if(t.t == -1) {
      error_found = true;

      //save where the error was found
      last_line_index = line_index;

      //reset the stacks
      s_stack[0] = 0;   //start in state 0
      s_stack_cnt = 1;
      t_stack_cnt = 0;

      //get the next token
      memcpy(&last_t, &t, sizeof(t));
      t = lex_next_token(infile);
      if(t.t == 1) {
        symbol_table_add(t.t, t.l, 0); //type = 0 for now
      }
    }

    //newline
    else if(t.t == -2) {

      //get another token
      last_line_index = line_index;
      memcpy(&last_t, &t, sizeof(t));
      t = lex_next_token(infile);
      if(t.t == 1) {
        symbol_table_add(t.t, t.l, 0); //type = 0 for now
      }
    }

    else if(t.t == -3 && error_found) {
      break;
    }

    //follow the table
    else {

      //get the command from the table
      //state+2 because we do not use the headers of the table
      int state = s_stack[s_stack_cnt-1];
      int token_index = parse_table_col_from_token(t);
      char *cmd = parse_table.d[state+2][token_index];

      //cell is empty, so report expecting something else
      if(strlen(cmd) == 0) {
        error_found = true;
        
        //jump to next line by copying the error condition
        struct token_s error_t;
        int error_state = state;
        error_t.t = t.t;
        strcpy(error_t.l, t.l);
        last_line_number = line_number;

        //continue grabbing tokens until or \n or eof
        memcpy(&last_t, &t, sizeof(t));
        t = lex_next_token(infile);
        while(t.t != -2 && t.t != -3) {
          memcpy(&last_t, &t, sizeof(t));
          t = lex_next_token(infile);
        }

        //throw syntax error as long as no lex error
        if(t.t != -1) {
          throw_unexpected_token(error_t, error_state);
        }

        //reset the stacks
        s_stack[0] = 0;   //start in state 0
        s_stack_cnt = 1;
        t_stack_cnt = 0;

        //get another token
        memcpy(&last_t, &t, sizeof(t));
        t = lex_next_token(infile);
        if(t.t == 1) {
          symbol_table_add(t.t, t.l, 0); //type = 0 for now
        }
        continue;
      }

      #ifdef SYNDEBUG
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

          Statement *stm2 = (Statement*)parseTree.pop();
          Statement *stm1 = (Statement*)parseTree.pop();
          CompoundStatement *stm = new CompoundStatement(stm1, stm2);
          parseTree.push(stm);
        }

        //id:=E
        else if(prod == 2) {
          pop_size = 3;
          rep_token.t = 13;
          rep_token.l[0] = 'S';

          char *lexem = (char*)malloc(sizeof(t_stack[t_stack_cnt-3]));
          strcpy(lexem, t_stack[t_stack_cnt-3]->l);
          IdExpression *id = new IdExpression(lexem);
          Expression *exp = (Expression*)parseTree.pop();
          AssignStatement *stm = new AssignStatement(id, exp);
          parseTree.push(stm);
        }

        //print(L)
        else if(prod == 3) {
          pop_size = 4;
          rep_token.t = 13;
          rep_token.l[0] = 'S';

          ExpressionList *explist = (ExpressionList*)parseTree.pop();
          PrintStatement *print = new PrintStatement(explist);
          parseTree.push(print);
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

          char *lexem = (char*)malloc(sizeof(t_stack[t_stack_cnt-1]));
          strcpy(lexem, t_stack[t_stack_cnt-1]->l);
          IdExpression *id = new IdExpression(lexem);
          parseTree.push(id);
        }

        //num
        else if(prod == 6) {
          pop_size = 1;
          rep_token.t = 14;
          rep_token.l[0] = 'E';

          int val = strtod((const char*)&t_stack[t_stack_cnt-1]->l, &stopstr);
          NumExpression *num = new NumExpression(val);
          parseTree.push(num);
        }

        //EBE
        else if(prod == 7) {
          pop_size = 3;
          rep_token.t = 14;
          rep_token.l[0] = 'E';

          Expression *exp2 = (Expression*)parseTree.pop();
          BinaryOperation *op = (BinaryOperation*)parseTree.pop();
          Expression *exp1 = (Expression*)parseTree.pop();
          OperationExpression *exp = new OperationExpression(exp1, exp2, op);
          parseTree.push(exp);
        }

        //(S,E)
        else if(prod == 8) {
          pop_size = 5;
          rep_token.t = 14;
          rep_token.l[0] = 'E';

          Expression *exp = (Expression*)parseTree.pop();
          Statement *stm = (Statement*)parseTree.pop();
          SequenceExpression *seq = new SequenceExpression(stm, exp);
          parseTree.push(seq);
        }

        //E,L
        else if(prod == 9) {
          pop_size = 3;
          rep_token.t = 15;
          rep_token.l[0] = 'L';

          ExpressionList *list = (ExpressionList*)parseTree.pop();
          Expression *exp = (Expression*)parseTree.pop();
          ExpressionList *newlist = new PairExpressionList(exp, list);
          parseTree.push(newlist);
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

          Plus *plus = new Plus();
          parseTree.push(plus);
        }

        //-
        else if(prod == 12) {
          pop_size = 1;
          rep_token.t = 16;
          rep_token.l[0] = 'B';

          Minus *minus = new Minus();
          parseTree.push(minus);
        }
        
        //*
        else if(prod == 13) {
          pop_size = 1;
          rep_token.t = 16;
          rep_token.l[0] = 'B';

          Multiply *multiply = new Multiply();
          parseTree.push(multiply);
        }

        // /
        else if(prod == 14) {
          pop_size = 1;
          rep_token.t = 16;
          rep_token.l[0] = 'B';

          Divide *divide = new Divide();
          parseTree.push(divide);
        }

        //pop off the size of the production
        s_stack_cnt -= pop_size;
        t_stack_cnt -= pop_size;

        //push the replacement token
        struct token_s *tmp = (struct token_s*)malloc(sizeof(struct token_s));
        tmp->t = rep_token.t;
        strcpy(tmp->l, rep_token.l);
        t_stack[t_stack_cnt++] = tmp;

        //push the new state from the goto
        int cur_state = s_stack[s_stack_cnt-1];
        int new_token_index = parse_table_col_from_token(rep_token);
        char *cmd_goto = parse_table.d[cur_state+2][new_token_index];
        s_stack[s_stack_cnt++] = strtod(cmd_goto, &stopstr);
      }

      //shift
      else if(cmd[0] == 's') {
        int newstate = strtod(&cmd[1], &stopstr);
        struct token_s *tmp = (struct token_s*)malloc(sizeof(struct token_s));
        tmp->t = newstate;
        strcpy(tmp->l, t.l);
        t_stack[t_stack_cnt++] = tmp;
        s_stack[s_stack_cnt++] = newstate;

        //get another token
        last_line_index = line_index;
        memcpy(&last_t, &t, sizeof(t));
        t = lex_next_token(infile);

        //if an id
        if(t.t == 1) {
          symbol_table_add(t.t, t.l, 0); //type = 0 for now
        }
      }

      //accept command
      else if(cmd[0] == 'a') {
        if(!error_found) {
          printf("The file has proper syntax\n");
        }

        PrettyPrintVisitor v = PrettyPrintVisitor();
        parseTree.get_root()->accept(&v);

        return 0;
      }

      //invalid command
      else {
        printf("Error: Parse table contains unknown command (%s)\n", cmd);
        return 0;
      }


      #ifdef SYNDEBUG
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
int parse_table_col_from_token(struct token_s t) {
  for(int i = 0; i < parse_table.cols; i++) {
    int tmp_token = strtod(parse_table.d[1][i], NULL);
    if(tmp_token == t.t) {
      return i;
    }
  }
  return -1;
}

//print an error message indicated where the unrecognized token is
void throw_unexpected_token(struct token_s t, int state) {

  //get valid options
  char *options[256];
  int options_len = 0;
  for(int i = 0; i < parse_table.cols; i++) {
    if(strlen(parse_table.d[state+2][i]) > 0) {
      options[options_len++] = parse_table.d[0][i];
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

  printf("Error: Syntax analyzer found '%s' on line %d while expecting one of the set %s\n", t.l, last_line_number, options_str);
  printf("\t%s\n", cur_line);
  printf("\t");
  for(int i = 0; i < last_line_index; i++) {
    printf(" ");
  }
  printf("^\n");
}
