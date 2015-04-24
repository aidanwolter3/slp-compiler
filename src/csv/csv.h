// Aidan Wolter
// Program Translation - COSC 4503

#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class CSV {
  private:
    char ***table;
  public:
    CSV(FILE *file, int *rows, int *cols);
    char* get(int row, int col);
    void set(int row, int col, char *cell);
};

#endif
