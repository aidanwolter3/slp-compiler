// Aidan Wolter
// Program Translation - COSC 4503

#include "csv.h"

char* xstrtok(char *line, char *delims);

//parse a csv with a variable size
CSV::CSV(FILE *file, int *rows, int *cols) {
  char *row = (char*)malloc(10000*sizeof(char*)); //bigger?

  //allocate enough memory
  table = (char***)malloc(CSV_DEFAULT_NUM_ROWS*sizeof(void*));
  for(int i = 0; i < CSV_DEFAULT_NUM_ROWS; i++) {
    table[i] = (char**)malloc(CSV_DEFAULT_NUM_COLS*sizeof(void*));
    for(int j = 0; j < CSV_DEFAULT_NUM_COLS; j++) {
      table[i][j] = (char*)malloc(256*sizeof(char));
    }
  }

  //initialize the size to zero
  *rows = 0;
  *cols = 0;

  //read the file one line at a time
  while(fgets(row, 10000, file)) {
    (*rows)++;

    for(int i = 0; i < CSV_DEFAULT_NUM_COLS; i++) {
      if(i > *cols) {
        (*cols) = i;
      }

      char *cell = (char*)malloc(256*sizeof(char));
      int cell_len = 0;

      //if found end of line, go to the next row
      if(strlen(row) <= 0) {
        break;
      }

      //if quote found, be careful with commas
      else if(row[0] == '"') {
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
        char *ptr = (char*)malloc(256*sizeof(char));
        if(strlen(row) != 0) {
          char *tmp = xstrtok(row, (char*)",");
          memcpy(ptr, tmp, strlen(tmp));
        }
        row += strlen(ptr)+1;
        strcpy(cell, ptr);
      }

      //copy the discovered cell into the table
      strcpy(table[(*rows)-1][i], cell);
    }
  }
}

char* CSV::get(int row, int col) {
  return table[row][col];
}

void CSV::set(int row, int col, char *cell) {
  strcpy(table[row][col], cell);
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
