// Aidan Wolter
// Syntax Analyzer - Program Assignment #2
// Program Translation - COSC 4503
// 2/26/2015

#include "lexical_analyzer.h"

struct lex_table_s lex_table;

//build the lex_table table from a file
void build_lex(FILE *sfile) {
  char ***table = parse_csv(sfile, &lex_table.rows, &lex_table.cols);

  //subtract the top row
  lex_table.rows--;

  //copy the matches header
  for(int i = 0; i < lex_table.cols; i++) {
    lex_table.matches[i] = (char*)malloc(256*sizeof(char*));
    strcpy(lex_table.matches[i], table[0][i]);
  }

  //copy the rest and cast as integers
  for(int i = 0; i < lex_table.rows; i++) {
    for(int j = 0; j < lex_table.cols; j++) {
      lex_table.table[i][j] = strtod(table[i+1][j], NULL);
    }
  }

  lex_table = lex_table;
}

//return the next valid token from the given file
struct token_s lex_next_token(FILE *infile) {

  //build the default token to return
  struct token_s t;
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
    for(int col = 0; col < lex_table.cols; col++) { //each col
      for(char *m = lex_table.matches[col]; *m != '\0'; m++) { //each char in match

        //compare matches[c][n] to input[i]
        //also treat newlines and eofs as spaces
        if((*m == ' ' && (c == '\n' || c == EOF)) || *m == c) {
          newstage = lex_table.table[stage][col];
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
    bool not_cycling = false;
    if(match_found == 1) {
      //printf("match found in stage: %d, newstage: %d\n", stage, newstage);
      
      //as long as not cycling in stage 0 (usually whitespace only)
      if(stage != 0 || newstage != 0) {

        //check if success (returning to stage 0)
        if(newstage == 0) {
          t.t = lex_table.table[stage][lex_table.cols-1];
          return t;
        }

        //still getting chars so push the character onto the lexem
        else {
          t.l[lexem_size++] = c;
        }

        //move to the new stage
        stage = newstage;

        //check if error stage
        if(stage == -1) {
          t.t = -1;
          return t;
        }
      }
    }

    //get another character
    c = fgetc(infile);

  } while(match_found);

  //if match wasn't found flush out the chars until a newline

  t.l[lexem_size++] = c;
  return t;
}
