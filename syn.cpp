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

struct lex_syn build_lex(FILE *sfile);
int lex_next_token(struct lex_syn lex);

//main method
int main(int argc, char *argv[]) {

  //create the lexical syntax structure
  FILE *sfile = fopen("lex_syn.slp", "r");
  struct lex_syn lex = build_lex(sfile);
  fclose(sfile);

  //while not eof (-3)
  int token = lex_next_token(lex);
  while(token != -3) {

    if(token == -1) {
      printf("Error!\n");
      break;
    }
    else if(token == -2) {
      printf("\n");
    }
    else {
      printf("%d ", token);
    }

    //get another token
    token = lex_next_token(lex);
  }

  return 0;
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

int lex_next_token(struct lex_syn lex) {

  //keep track of the current character
  static char c = 0;

  //if a character has not been passed from the previous stage, get another
  if(c == 0) {
    c = getc(stdin);
  }

  //check for newline
  if(c == '\n') {
    c = getc(stdin);
    return -2; //return newline token
  }

  //check for eof
  if(c == EOF) {
    c = getc(stdin);
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
      c = getc(stdin);
    }

  } while(match_found);

  return -1;
}
