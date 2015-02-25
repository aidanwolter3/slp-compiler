// Aidan Wolter
// Syntax Analyzer - Program Assignment #2
// Program Translation - COSC 4503
// 2/26/2015

#include "parse_csv.h"

//parse a csv with a variable size
char*** parse_csv(FILE *file, int *rows, int *cols) {
  char ***table;
  char *line = (char*)malloc(10000*sizeof(char*)); //bigger?
  *rows = 1;
  *cols = 1;
  while(fgets(line, 10000, file)) {

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
