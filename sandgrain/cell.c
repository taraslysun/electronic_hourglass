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

int* find_position_to_fall(int row, int col, Hourglass* grid, int downwards) {
    int* position = (int*)malloc(2 * sizeof(int));
    if (downwards == 0) {
        if (row == 0) {
            position[0] = row;
            position[1] = col;
            return position;
        }
        else if (grid->grid[row - 1][col].IsGrain == 0 && grid->grid[row - 1][col].is_in_hourglass == 1) {
            position[0] = row - 1;
            position[1] = col;
            return position;
        }
        else if (grid->grid[row - 1][col - 1].IsGrain == 0 && grid->grid[row - 1][col - 1].is_in_hourglass == 1) {
            position[0] = row - 1;
            position[1] = col - 1;
            return position;
        }
        else if (grid->grid[row - 1][col + 1].IsGrain == 0 && grid->grid[row - 1][col + 1].is_in_hourglass == 1) {
            position[0] = row - 1;
            position[1] = col + 1;
            return position;
        }
        else {
            position[0] = row;
            position[1] = col;
            return position;
        }
    }
    else
    if (row == grid->size - 1) {
        position[0] = row;
        position[1] = col;
        return position;
    }
    else if (grid->grid[row + 1][col].IsGrain == 0 && grid->grid[row + 1][col].is_in_hourglass == 1) {
        position[0] = row + 1;
        position[1] = col;
        return position;
    }
    else if (grid->grid[row + 1][col - 1].IsGrain == 0 && grid->grid[row + 1][col - 1].is_in_hourglass == 1) {
        position[0] = row + 1;
        position[1] = col - 1;
        return position;
    }
    else if (grid->grid[row + 1][col + 1].IsGrain == 0 && grid->grid[row + 1][col + 1].is_in_hourglass == 1) {
        position[0] = row + 1;
        position[1] = col + 1;
        return position;
    }
    else {
        position[0] = row;
        position[1] = col;
        return position;
    }

}

void fall(int row, int col, Hourglass* grid, int downwards) {
    int* position = find_position_to_fall(row, col, grid, downwards);
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