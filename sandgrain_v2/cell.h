#ifndef SANDGRAIN_CELL_H
#define SANDGRAIN_CELL_H

typedef struct {
    int row;
    int col;
    int IsGrain;
    int is_in_hourglass;
    int fallen_on_this_epoch;
} Cell;


typedef struct {
    int size;
    Cell** grid;
    int** grain_positions;
}Hourglass;


Cell* create_cell();
Cell* add_to_hourglass();
Cell* fill_cell();
void clear_cell(Cell* cell);
int* find_position_to_fall(int row,int col, Hourglass* grid, int downwards);
void fall(int row, int col, Hourglass* grid, int downwards);
char char_repr(Cell* cell);

#endif //SANDGRAIN_CELL_H
