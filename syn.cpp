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
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 6; j++) {
      printf("%s ", parse_table[i][j]);
    }
    printf("\n");
  }

  //while not eof (-3)
  //int line = 1;
  //int state = 0;
  //int token = lex_next_token(lex, infile);
  //while(token != -3) {

  //  //error in lexer
  //  if(token == -1) {
  //    printf("Error: unrecognized token\n");
  //    break;
  //  }

  //  //newline
  //  else if(token == -2) {
  //    line++;
  //  }

  //  //follow the table
  //  else {
  //    
  //  }

  //  //get another token
  //  token = lex_next_token(lex, infile);
  //}

  return 0;
}

char*** build_parse_table(FILE *pfile) {

  //terrible usage of memory allocation
  char ***ptable = (char***)malloc(8*sizeof(void*));
  for(int i = 0; i < 8; i++) {
    ptable[i] = (char**)malloc(6*sizeof(void*));
    for(int j = 0; j < 6; j++) {
      ptable[i][j] = (char*)malloc(256*sizeof(char*));
    }
  }

  char line[256];
  int cols = 6;
  int rows = 8;
  fgets(line, sizeof(line), pfile);

  //get the rest of the table
  char *ptr = xstrtok(line, (char*)",\r");
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
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
