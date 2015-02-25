// Aidan Wolter
// Syntax Analyzer - Program Assignment #2
// Program Translation - COSC 4503
// 2/26/2015

#include "lexical_analyzer.h"

//#define LEXDEBUG

//global lex table
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

  //keep track of the entire line
  static char *cur_line = (char*)malloc(1024*sizeof(char*));
  static int cur_line_cnt = 0;

  //keep track of the line number
  static int line_number = 1;

  //the current token we are discovering
  struct token_s t;
  t.t = -1;
  memset(t.l, 0, sizeof(t.l));
  int lexem_size = 0;

  //the last read character, initialize to a new read
  static char c = 0;
  if(c == 0) {
    c = fgetc(infile);
    cur_line[cur_line_cnt++] = c;
  }

  //hold how far in the line we have parsed
  static int line_index = 1;

  //check for newline
  if(c == '\n') {
    line_number++;
    line_index = 0;

    //reset the current line
    memset(cur_line, 0, 1024);
    cur_line_cnt = 0;

    //return newline token
    t.l[lexem_size++] = c;
    t.t = -2;
    c = fgetc(infile);
    cur_line[cur_line_cnt++] = c;

    #ifdef LEXDEBUG
    printf("t: %d %s\n", t.t, t.l);
    #endif

    return t;
  }

  //check for eof
  if(c == EOF) {
    t.l[lexem_size++] = c;
    t.t = -3;
    c = fgetc(infile);
    cur_line[cur_line_cnt++] = c;

    #ifdef LEXDEBUG
    printf("t: %d %s\n", t.t, t.l);
    #endif

    return t; //return eof token
  }


  //as long as we can find a match in the lex_table
  bool match_found;
  int stage = 0;
  int newstage = 0;
  do {

    //check for match
    match_found = false;
    for(int col = 0; col < lex_table.cols; col++) { //each col
      for(char *m = lex_table.matches[col]; *m != '\0'; m++) { //each char in match

        //compare matches[c][n] to input[i]
        //also treat newlines and eofs as spaces
        if((*m == ' ' && (c == '\n' || c == EOF)) || *m == c) {
          newstage = lex_table.table[stage][col];
          //printf("match found in stage: %d, newstage: %d\n", stage, newstage);
          match_found = true;
          break;
        }
      }

      //already found, so break
      if(match_found) {
        break;
      }
    }

    //error
    if(newstage == -1 || !match_found) {
      break;
    }

    //cycling in stage 0 so get another character (0 to 0)
    else if(newstage == 0 && stage == 0) {
      c = fgetc(infile);
      cur_line[cur_line_cnt++] = c;
      line_index++;
    }

    //jumped to stage 0 so success (# to 0)
    else if(newstage == 0) {
      t.t = lex_table.table[stage][lex_table.cols-1];

      #ifdef LEXDEBUG
      printf("t: %d %s\n", t.t, t.l);
      #endif

      return t;
    }

    //started token (0 to #)
    else if(stage == 0) {
      stage = newstage;
      t.l[lexem_size++] = c;
      c = fgetc(infile);
      cur_line[cur_line_cnt++] = c;
      line_index++;
    }

    //still reading token (# to #)
    else {
      stage = newstage;
      t.l[lexem_size++] = c;
      c = fgetc(infile);
      cur_line[cur_line_cnt++] = c;
      line_index++;
    }

  } while(match_found);

  //no match so return error
  t.l[lexem_size++] = c;

  //read char until \n or eof
  c = fgetc(infile);
  while(c != '\n' && c != EOF) {
    cur_line[cur_line_cnt++] = c;
    c = fgetc(infile);
  }

  throw_unknown_token(cur_line, line_index, line_number, t);

  #ifdef LEXDEBUG
  printf("t: %d %s\n", t.t, t.l);
  #endif

  return t;
}

//print an error message indicated where the unrecognized token is
void throw_unknown_token(char *line, int line_index, int line_number, struct token_s t) {
  printf("Error: Lexer found unrecognized token '%s' on line %d\n", t.l, line_number);
  printf("\t%s\n", line);
  printf("\t");
  for(int i = 0; i < line_index; i++) {
    printf(" ");
  }
  printf("^\n");
}
