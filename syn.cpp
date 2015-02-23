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
  char *matches[256];
  int stages[256][256];
  int tokens[256];
  int rows;
  int cols;
};

#define PT_ROWS 8
#define PT_COLS 6

int parse_table_col_from_token(int token, char ***parse_table);
char* xstrtok(char *line, char *delims);
char*** build_parse_table(FILE *pfile);
struct lex_syn build_lex(FILE *sfile);
int lex_next_token(struct lex_syn lex, FILE *infile);

//main method
int main(int argc, char *argv[]) {

  //create the lexical syntax structure
  FILE *sfile = fopen("lex_syn", "r");
  if(sfile == NULL) {
    printf("Could not find lex syntax file!\n");
    printf("The file should be placed in the same directory and named 'lex_syn'\n");
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
  char ***parse_table = build_parse_table(pfile);

  //continue parsing until error or accept
  int line = 1;
  int t_stack[256]; //token stack
  int s_stack[256]; //state stack
  s_stack[0] = 0;   //start in state 0
  int t_stack_cnt = 0;
  int s_stack_cnt = 1;
  int token = lex_next_token(lex, infile);
  while(true) {

    //error in lexer
    if(token == -1) {
      printf("Error: unrecognized token\n");
      break;
    }

    //newline
    else if(token == -2) {
      line++;
    }

    //follow the table
    else {

      //get the command from the table
      //state+1 because we do not use the header of the table
      //token-1 because token 0 is invalid
      int state = s_stack[s_stack_cnt-1];
      int token_index = parse_table_col_from_token(token, parse_table);
      char *cmd = parse_table[state+1][token_index];

      //cell is empty, so report expecting something else
      if(strlen(cmd) == 0) {
        printf("cell is empty. expecting something else\n");
        return 0;
      }

      //reduction
      if(cmd[0] == 'r') {

        char *stopstr;
        int prod = strtod(&cmd[1], &stopstr);
        int pop_size; //how much to pop off the stacks
        int rep_token;//what to push onto the token stack

        //check each production
        if(prod == 0) {
          pop_size = 5; //print();
          rep_token = 5;
        }
        else if(prod == 1) {
          pop_size = 0; //epsilon
          rep_token = 5;
        }
        
        //pop off the size of the production
        s_stack_cnt -= pop_size;
        t_stack_cnt -= pop_size;

        //push the replacement token
        t_stack[t_stack_cnt++] = rep_token;

        //push the new state from the goto
        int cur_state = s_stack[s_stack_cnt-1];
        int new_token_index = parse_table_col_from_token(rep_token, parse_table);
        char *cmd_goto = parse_table[cur_state+1][new_token_index];
        s_stack[s_stack_cnt++] = strtod(cmd_goto, &stopstr);
      }

      //shift
      else if(cmd[0] == 's') {
        char *stopstr;
        int newstate = strtod(&cmd[1], &stopstr);
        t_stack[t_stack_cnt++] = token;
        s_stack[s_stack_cnt++] = newstate;
      }

      //accept command
      else if(cmd[0] == 'a') {
        printf("The file has proper syntax\n");
        return 0;
      }

      //invalid command
      else {
        printf("Parse table is invalid!\n");
        return 0;
      }
    }

    //get another token
    token = lex_next_token(lex, infile);
  }

  return 0;
}

//search for the token in the parse table and return the col index
int parse_table_col_from_token(int token, char ***parse_table) {
  for(int i = 0; i < PT_COLS; i++) {
    int tmp_token = strtod(parse_table[0][i], NULL);
    if(tmp_token == token) {
      return i;
    }
  }
  return -1;
}

//build the parse table from a file
//TODO: make the dimensions change depending on the actual size in the csv
char*** build_parse_table(FILE *pfile) {

  //terrible usage of memory allocation
  char ***ptable = (char***)malloc(PT_ROWS*sizeof(void*));
  for(int i = 0; i < PT_ROWS; i++) {
    ptable[i] = (char**)malloc(PT_COLS*sizeof(void*));
    for(int j = 0; j < PT_COLS; j++) {
      ptable[i][j] = (char*)malloc(256*sizeof(char*));
    }
  }

  char line[256];
  fgets(line, sizeof(line), pfile);

  //get the rest of the table
  char *ptr = xstrtok(line, (char*)",\r");
  for(int i = 0; i < PT_ROWS; i++) {
    for(int j = 0; j < PT_COLS; j++) {
      if(ptr == NULL) {
        ptable[i][j] = (char*)"";
      }
      else {
        strcpy(ptable[i][j], ptr);
      }
      ptr = xstrtok(NULL, (char*)",\r");
    }
  }
  
  return ptable;
}

struct lex_syn build_lex(FILE *sfile) {
  struct lex_syn lex;
  lex.rows = 0;
  lex.cols = 0;

  //parse the syntax file one line at a time
  char line[256];
  while(fgets(line, sizeof(line), sfile)) {
    
    //get the matches at the top of the file
    char *ptr;
    if(lex.rows == 0) {
      ptr = strtok(line, "\\\n");
      while(ptr != NULL) {
        lex.matches[lex.cols] = (char*)malloc(256*sizeof(char));
        strcpy(lex.matches[lex.cols], ptr);
        lex.cols++;
        ptr = strtok(NULL, "\\\n");
      }
    }

    //get the stages and their tokens
    else {
      int tmpcols = 0;
      char *stopstr;
      lex.stages[lex.rows-1][tmpcols] = strtod(strtok(line, "\\\n"), &stopstr);
      tmpcols++;
      while(tmpcols < lex.cols) {
        lex.stages[lex.rows-1][tmpcols] = strtod(strtok(NULL, "\\\n"), &stopstr);
        tmpcols++;
      }

      //get the tokens
      ptr = strtok(NULL, "\\\n");
      if(ptr == NULL) {
        lex.tokens[lex.rows-1] = 0;
      }
      else {
        lex.tokens[lex.rows-1] = strtod(ptr, &stopstr);
      }
    }

    //move to the next line
    lex.rows++;
  }

  return lex;
}

int lex_next_token(struct lex_syn lex, FILE *infile) {

  //keep track of the current character
  static char c = 0;

  //if a character has not been passed from the previous stage, get another
  if(c == 0) {
    c = fgetc(infile);
  }

  //check for newline
  if(c == '\n') {
    c = fgetc(infile);
    return -2; //return newline token
  }

  //check for eof
  if(c == EOF) {
    c = fgetc(infile);
    return -3; //return eof token
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
          newstage = lex.stages[stage][col];
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
          return lex.tokens[stage];
        }

        //move to the new stage
        stage = newstage;

        //check if error stage
        if(stage == -1) {
          return -1;
        }
      }

      //get another character
      c = fgetc(infile);
    }

  } while(match_found);

  return -1;
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
