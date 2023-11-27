#ifndef HOURGLASS_H
#define HOURGLASS_H

#include "cell.h"


Hourglass* create_hourglass(int size, int grain_quantity);
void create_grid(Hourglass* hourglass);
void fill_grid(Hourglass* hourglass, int grain_quantity);
void print_grid(Hourglass* hourglass);
void one_epoch(Hourglass* hourglass, int downwards);
void add_grain(Hourglass* hourglass, int row, int col);
Hourglass* init_hourglass(int size);
int** as_matrix(Hourglass* hourglass);


#endif
