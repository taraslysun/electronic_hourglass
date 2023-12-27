#include "cell.h"
#include <stdlib.h>

Cell* create_cell() {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    cell->IsGrain = 0;
    return cell;
}

Cell* add_to_hourglass() {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    cell->is_in_hourglass = 1;
    return cell;
}

Cell* fill_cell() {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    cell->is_in_hourglass = 1;
    cell->IsGrain = 1;
    return cell;
}

void clear_cell(Cell* cell) {
    cell->IsGrain = 0;
}

int valid_position(Hourglass* grid, int row, int col){
    if (row>=0 && col>=0)
        if (row<grid->size_y && col<grid->size_x)
            if (grid->grid[row][col].is_in_hourglass==1)
                if (grid->grid[row][col].IsGrain!=1)
                    return 1;
    return 0;
}

int* find_position_to_fall(int row, int col, Hourglass* grid, int x, int y) {
    int* position = (int*)malloc(2 * sizeof(int));
    if (valid_position(grid, row+y, col+x)){
        position[0] = row+y;
        position[1] = col+x;
        return position;
    }
    int random = rand() % 2;
    if (random == 0) random = -1;


    if (valid_position(grid, row+y + x*random, col + x + y*random)){
        position[0] = row+ y + x*random;
        position[1] = col + x + y*random;
        return position;
    }
    else if (valid_position(grid, row+y - x*random, col + x + y*random)){
        position[0] = row+y - x*random;
        position[1] = col + x + y*random;
        return position;
    }


    if (valid_position(grid, row, col+random)){
        position[0] = row;
        position[1] = col+random;
        return position;
    }

    if (valid_position(grid, row+random, col)){
        position[0] = row+random;
        position[1] = col;
        return position;
    }
    position[0] = row;
    position[1] = col;
    return position;
}


void fall(int row, int col, Hourglass* grid, int x, int y) {
    int* position = find_position_to_fall(row, col, grid, x, y);
    grid->grid[row][col].IsGrain = 0;
    grid->grid[position[0]][position[1]].IsGrain = 1;
    grid->grid[position[0]][position[1]].fallen_on_this_epoch = 1;
}

char char_repr(Cell* cell) {
    if (cell->IsGrain == 0)
        return ' ';
    else
        return 'o';
}