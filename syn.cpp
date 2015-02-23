// Aidan Wolter
// Syntax Analyzer - Program Assignment #2
// Program Translation - COSC 4503
// 2/26/2015
// 
// Description:

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//the lex structure that defines valid strings of characters for their tokens
struct lex_syn {
  int table[256][256];
  char *matches[256];
  int rows;
  int cols;
};

struct token {
  int t;
  char l[256];
};

char*** parse_csv(FILE *file, int *rows, int *cols);
int parse_table_col_from_token(struct token t, char ***parse_table, int cols);
char* xstrtok(char *line, char *delims);
struct lex_syn build_lex(FILE *sfile);
struct token lex_next_token(struct lex_syn lex, FILE *infile);

//main method
int main(int argc, char *argv[]) {

  //create the lexical syntax structure
  FILE *sfile = fopen("lex_table.csv", "r");
  if(sfile == NULL) {
    printf("Could not find lex syntax file!\n");
    printf("The file should be placed in the same directory and named 'lex_table.csv'\n");
    return 0;
  }
  struct lex_syn lex = build_lex(sfile);
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
  struct token t = lex_next_token(lex, infile);
  while(true) {

    //error in lexer
    if(t.t == -1) {
      printf("Error: Lexer found unrecognized token '%s' on line %d\n", t.l, line);
      break;
    }

    //newline
    else if(t.t == -2) {
      line++;
    }

    //follow the table
    else {

      //get the command from the table
      //state+1 because we do not use the header of the table
      //token-1 because token 0 is invalid
      int state = s_stack[s_stack_cnt-1];
      int token_index = parse_table_col_from_token(t, parse_table, parse_table_cols);
      char *cmd = parse_table[state+1][token_index];

      //cell is empty, so report expecting something else
      if(strlen(cmd) == 0) {

        //get valid options
        char *options[256];
        int options_len = 0;
        for(int i = 0; i < parse_table_cols; i++) {
          if(strlen(parse_table[state+1][i]) > 0) {
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

      //reduction
      if(cmd[0] == 'r') {

        char *stopstr;
        int prod = strtod(&cmd[1], &stopstr);
        int pop_size; //how much to pop off the stacks
        struct token rep_token;//what to push onto the token stack

        //check each production
        if(prod == 0) {
          pop_size = 5; //print();
          rep_token.t = 5;
          rep_token.l[0] = 'S';
        }
        else if(prod == 1) {
          pop_size = 0; //epsilon
          rep_token.t = 5;
          strcpy(rep_token.l, "epsilon");
        }
        
        //pop off the size of the production
        s_stack_cnt -= pop_size;
        t_stack_cnt -= pop_size;

        //push the replacement token
        t_stack[t_stack_cnt++] = rep_token.t;

        //push the new state from the goto
        int cur_state = s_stack[s_stack_cnt-1];
        int new_token_index = parse_table_col_from_token(rep_token, parse_table, parse_table_cols);
        char *cmd_goto = parse_table[cur_state+1][new_token_index];
        s_stack[s_stack_cnt++] = strtod(cmd_goto, &stopstr);
      }

      //shift
      else if(cmd[0] == 's') {
        char *stopstr;
        int newstate = strtod(&cmd[1], &stopstr);
        t_stack[t_stack_cnt++] = t.t;
        s_stack[s_stack_cnt++] = newstate;
      }

      //accept command
      else if(cmd[0] == 'a') {
        printf("The file has proper syntax\n");
        return 0;
      }

      //invalid command
      else {
        printf("Error: Parse table contains unknown command (%s)\n", cmd);
        return 0;
      }
    }

    //get another token
    t = lex_next_token(lex, infile);
  }

  return 0;
}

//search for the token in the parse table and return the col index
int parse_table_col_from_token(struct token t, char ***parse_table, int cols) {
  for(int i = 0; i < cols; i++) {
    int tmp_token = strtod(parse_table[0][i], NULL);
    if(tmp_token == t.t) {
      return i;
    }
  }
  return -1;
}

//parse a csv with a variable size
char*** parse_csv(FILE *file, int *rows, int *cols) {
  char ***table;
  char *line = (char*)malloc(1024*sizeof(char*)); //bigger?
  *rows = 1;
  *cols = 1;
  while(fgets(line, 1024, file)) {

    //count the number of rows and columns
    bool cols_state = false;
    for(int i = 0; i < strlen(line); i++) {

      //increment the rows
      if(line[i] == '\r') {
        (*rows)++;
      }

      //increment the cols if on the first row
      if(*rows == 1) {

        //take care of all the quote nastiness with a state machine
        if(cols_state == false) {
          if(line[i] == '"') {
            cols_state = true;
          }
          if(line[i] == ',') {
            (*cols)++;
          }
        }
        else if(cols_state == true) {
          if(line[i] == '"') {
            cols_state = false;
          }
        }
      }
    }

    //allocate enough memory
    table = (char***)malloc(*rows*sizeof(void*));
    for(int i = 0; i < *rows; i++) {
      table[i] = (char**)malloc(*cols*sizeof(void*));
      for(int j = 0; j < *cols; j++) {
        table[i][j] = (char*)malloc(256*sizeof(char*));
      }
    }


    //read a single cell in the row
    for(int i = 0; i < *rows; i++) {

      //logical row separated by CRs
      char *row = (char*)malloc(256*sizeof(char*));
      char *ptr;
      ptr = xstrtok(line, (char*)"\r");
      line += strlen(ptr)+1;
      memcpy(row, ptr, strlen(ptr));

      for(int j = 0; j < *cols; j++) {
        char cell[256];
        int cell_len = 0;

        //if quote found, be careful with commas
        if(row[0] == '"') {
          row++;
          while(true) {

            //double quote found
            if(row[0] == '"' && row[1] == '"') {
              cell[cell_len++] = '"';
              row += 2;
            }

            //end of cell
            else if(row[0] == '"' && row[1] == ',') {
              row += 2;
              break;
            }

            //anything else
            else {
              cell[cell_len++] = row[0];
              row++;
            }
          }
        }

        //normal cell (no quote)
        else {
          char *ptr = (char*)malloc(256*sizeof(char*));
          if(strlen(row) != 0) {
            char *tmp = xstrtok(row, (char*)",");
            memcpy(ptr, tmp, strlen(tmp));
          }
          row += strlen(ptr)+1;
          strcpy(cell, ptr);
        }

        //copy the discovered cell into the table
        strcpy(table[i][j], cell);
      }
    }
  }

  return table;
}

struct lex_syn build_lex(FILE *sfile) {
  struct lex_syn lex;
  char ***table = parse_csv(sfile, &lex.rows, &lex.cols);

  //subtract the top row
  lex.rows--;

  //copy the matches header
  for(int i = 0; i < lex.cols; i++) {
    lex.matches[i] = (char*)malloc(256*sizeof(char*));
    strcpy(lex.matches[i], table[0][i]);
  }

  //copy the rest and cast as integers
  for(int i = 0; i < lex.rows; i++) {
    for(int j = 0; j < lex.cols; j++) {
      lex.table[i][j] = strtod(table[i+1][j], NULL);
    }
  }

  return lex;
}

struct token lex_next_token(struct lex_syn lex, FILE *infile) {

  //build the default token to return
  struct token t;
  t.t = -1;
  memset(t.l, 0, sizeof(t.l));

  //keep track of the length of the lexem
  int lexem_size = 0;

  //keep track of the current character
  static char c = 0;

  //if a character has not been passed from the previous stage, get another
  if(c == 0) {
    c = fgetc(infile);
  }

  //check for newline
  if(c == '\n') {
    t.l[lexem_size++] = c;
    t.t = -2;
    c = fgetc(infile);
    return t; //return newline token
  }

  //check for eof
  if(c == EOF) {
    t.l[lexem_size++] = c;
    t.t = -3;
    c = fgetc(infile);
    return t; //return eof token
  }

  //start at the first stage and continue
  int stage = 0;
  int newstage = 0;
  int match_found = 0;
  
  //continue to check for matches until entire token is found or error
  do {

    //check for match
    match_found = 0;
    for(int col = 0; col < lex.cols; col++) { //each col
      for(char *m = lex.matches[col]; *m != '\0'; m++) { //each char in match

        //compare matches[c][n] to input[i]
        //also treat newlines and eofs as spaces
        if((*m == ' ' && (c == '\n' || c == EOF)) || *m == c) {
          newstage = lex.table[stage][col];
          match_found = 1;
          break;
        }
      }

      //already found, so break
      if(match_found == 1) {
        break;
      }
    }

    //if a match was found
    if(match_found == 1) {
      //printf("match found in stage: %d, newstage: %d\n", stage, newstage);
      
      //as long as not cycling in stage 0 (whitespace only)
      if(stage != 0 || newstage != 0) {

        //check if success (returning to stage 0)
        if(newstage == 0) {
          t.t = lex.table[stage][lex.cols-1];
          return t;
        }

        //move to the new stage
        stage = newstage;

        //check if error stage
        if(stage == -1) {
          t.t = -1;
          return t;
        }
      }

      //get another character
      t.l[lexem_size++] = c;
      c = fgetc(infile);
    }

  } while(match_found);

  t.l[lexem_size++] = c;
  return t;
}


//modified strtok that takes into account empty strings
//courtesy of http://www.tek-tips.com/viewthread.cfm?qid=294161
char* xstrtok(char *line, char *delims) {
  static char *saveline = NULL;
  char *p;
  int n;

  if(line != NULL) {
    saveline = line;
  }

  /*
  *see if we have reached the end of the line 
  */
  if(saveline == NULL || *saveline == '\0') {
    return(NULL);
  }

  /*
  *return the number of characters that aren't delims 
  */
  n = strcspn(saveline, delims);
  p = saveline; /*save start of this token*/

  saveline += n; /*bump past the delim*/

  /*trash the delim if necessary*/
  if(*saveline != '\0') {
    *saveline++ = '\0';
  }

  return p;
}
