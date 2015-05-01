// Aidan Wolter
// Program Translation - COSC 4503

#include "LexicalAnalyzer.h"

//#define LEXDEBUG

//build the lex_table table from a file
LexicalAnalyzer::LexicalAnalyzer(FILE *lex_file, FILE *infile) {

  //setup the current line
  cur_line = (char*)malloc(1024*sizeof(char*));
  cur_line_cnt = 0;
  line_index = 0;
  line_number = 1;

  lex_table = new CSV(lex_file, &rows, &cols);
  this->infile = infile;

  //subtract the top row
  rows--;

  //copy the matches header
  for(int i = 0; i < cols; i++) {
    matches[i] = (char*)malloc(256*sizeof(char*));
    strcpy(matches[i], lex_table->get(0, i));
  }

  //copy the rest and cast as integers
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      table[i][j] = strtod(lex_table->get(i+1, j), NULL);
    }
  }
}

//return the next valid token from the given file
Token* LexicalAnalyzer::nextToken() {

  //the current token we are discovering
  char *nullstr = (char*)malloc(1*sizeof(char*));
  Token *t = new Token(-1, nullstr);
  int lexem_size = 0;

  //the last read character, initialize to a new read
  static char c = 0;
  if(c == 0) {

    //reset the current line
    memset(cur_line, 0, 1024);
    cur_line_cnt = 0;

    c = fgetc(infile);
    if(c != '\n' && c != EOF) {
      cur_line[cur_line_cnt++] = c;
    }
  }

  //check for newline
  if(c == '\n') {
    line_number++;
    line_index = 0;

    //return newline token
    t->l[lexem_size++] = c;
    t->t = -2;

    c = 0;

    #ifdef LEXDEBUG
    printf("t: %d %s\n", t->t, t->l);
    #endif

    return t;
  }

  //check for eof
  if(c == EOF) {
    line_index = 0;

    t->l[lexem_size++] = c;
    t->t = -3;

    c = 0;

    #ifdef LEXDEBUG
    printf("t: %d %s\n", t->t, t->l);
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
    for(int col = 0; col < cols; col++) { //each col
      for(char *m = matches[col]; *m != '\0'; m++) { //each char in match

        //compare matches[c][n] to input[i]
        //also treat newlines and eofs as spaces
        if((*m == ' ' && (c == '\n' || c == EOF)) || *m == c) {
          newstage = table[stage][col];
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
      if(lexem_size == 0) {
        t->l[lexem_size++] = c;
      }
      break;
    }

    //cycling in stage 0 so get another character (0 to 0)
    else if(newstage == 0 && stage == 0) {
      line_index++;
      c = fgetc(infile);
      if(c != '\n' && c != EOF) {
        cur_line[cur_line_cnt++] = c;
      }
    }

    //jumped to stage 0 so success (# to 0)
    else if(newstage == 0) {
      line_index += lexem_size;

      t->t = table[stage][cols-1];

      #ifdef LEXDEBUG
      printf("t: %d %s\n", t->t, t->l);
      #endif

      return t;
    }

    //started token (0 to #)
    else if(stage == 0) {
      stage = newstage;
      t->l[lexem_size++] = c;
      c = fgetc(infile);
      if(c != '\n' && c != EOF) {
        cur_line[cur_line_cnt++] = c;
      }
    }

    //still reading token (# to #)
    else {
      stage = newstage;
      t->l[lexem_size++] = c;
      c = fgetc(infile);
      if(c != '\n' && c != EOF) {
        cur_line[cur_line_cnt++] = c;
      }
    }

  } while(match_found);

  //read char until \n or eof
  //c = fgetc(infile);
  while(c != '\n' && c != EOF) {
    c = fgetc(infile);
    if(c != '\n' && c != EOF) {
      cur_line[cur_line_cnt++] = c;
    }
  }

  throwUnknownToken(cur_line, line_index, line_number, t);

  #ifdef LEXDEBUG
  printf("t: %d %s\n", t->t, t->l);
  #endif

  return t;
}

//print an error message indicated where the unrecognized token is
void LexicalAnalyzer::throwUnknownToken(char *line, int line_index, int line_number, Token *t) {
  printf("Error: Lexer found unrecognized token '%s' on line %d\n", t->l, line_number);
  printf("\t%s\n", line);
  printf("\t");
  for(int i = 0; i < line_index; i++) {
    printf(" ");
  }
  printf("^\n");
}

char* LexicalAnalyzer::getCurLine() {
  return cur_line;
}
int LexicalAnalyzer::getLineIndex() {
  return line_index;
}
int LexicalAnalyzer::getLineNumber() {
  return line_number;
}
