#include <stdio.h>
#include <stdlib.h>
#include "hourglass.h"

Hourglass* init_hourglass(int size) {
    Hourglass* hourglass = (Hourglass*)malloc(sizeof(Hourglass));
    hourglass->size = size;
    hourglass->grid = NULL;
    hourglass->grain_positions = NULL;
    return hourglass;
}

void create_grid(Hourglass* hourglass) {
    hourglass->grid = (Cell**)malloc(hourglass->size * sizeof(Cell*));
    for (int i = 0; i < hourglass->size; i++) {
        hourglass->grid[i] = (Cell*)malloc(hourglass->size * sizeof(Cell));
        for (int j = 0; j < hourglass->size; j++)
            hourglass->grid[i][j] = *create_cell();
    }

    int offset = 0;
    for (int i = offset; i < hourglass->size - offset; i++) {
        for (int j = offset; j < hourglass->size - offset; j++)
            hourglass->grid[i][j] = *add_to_hourglass();
        offset++;
    }
    offset = 0;
    for (int i = hourglass->size - offset - 1; i >= offset; i--) {
        for (int j = offset; j < hourglass->size - offset; j++)
            hourglass->grid[i][j] = *add_to_hourglass();
        offset++;
    }
}

void fill_grid(Hourglass* hourglass, int grain_quantity) {
    int offset = 0;
    while (grain_quantity > 0) {
        for (int i = offset; i < hourglass->size - offset; i++) {
            for (int j = offset; j < hourglass->size - offset; j++) {
                if (grain_quantity == 0)
                    break;
                hourglass->grid[i][j] = *fill_cell();
                grain_quantity--;
            }
            offset++;
            if (grain_quantity == 0)
                break;
        }
    }
}

void print_grid(Hourglass* hourglass) {
    for (int i = 0; i < hourglass->size; i++) {
        for (int j = 0; j < hourglass->size; j++) {
            if (hourglass->grid[i][j].IsGrain == 1)
                printf("*");
            else if (hourglass->grid[i][j].is_in_hourglass == 1)
                printf("_");
            else
                printf(" ");
        }
        printf("\n");
    }
}

void one_epoch(Hourglass* hourglass, int downwards) {
    if (downwards == 0) {
        for (int i = 0; i < hourglass->size; i++) {
            for (int j = 0; j < hourglass->size; j++) {
                if (hourglass->grid[hourglass->size-i-1][j].IsGrain == 1) {
                    if (hourglass->grid[hourglass->size-i-1][j].fallen_on_this_epoch == 1)
                        continue;
                    fall(hourglass->size-i-1, j, hourglass, downwards);
                }
            }
        }
    } else {
        for (int i = hourglass->size - 1; i >= 0; i--) {
            for (int j = 0; j < hourglass->size; j++) {
                if (hourglass->grid[i][j].IsGrain == 1) {
                    if (hourglass->grid[i][j].fallen_on_this_epoch == 1)
                        continue;
                    fall(i, j, hourglass, downwards);
                }
            }
        }
    }
    for (int i = 0; i < hourglass->size; i++) {
        for (int j = 0; j < hourglass->size; j++) {
            if (hourglass->grid[i][j].IsGrain == 1)
                hourglass->grid[i][j].fallen_on_this_epoch = 0;
        }
    }
}

void add_grain(Hourglass* hourglass, int row, int col) {
    hourglass->grid[row][col] = *fill_cell();
    hourglass->grid[row][col].is_in_hourglass = 1;
}

Hourglass* create_hourglass(int size, int grain_quantity) {
    Hourglass* hourglass = init_hourglass(size);
    create_grid(hourglass);
    fill_grid(hourglass, grain_quantity);
    return hourglass;
}
