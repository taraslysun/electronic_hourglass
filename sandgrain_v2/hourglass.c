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
    hourglass->grid = (Cell**)malloc(hourglass->size * 2 * sizeof(Cell*));
    for (int i = 0; i < hourglass->size * 2 ; i++) {
        hourglass->grid[i] = (Cell*)malloc(hourglass->size * sizeof(Cell));
        for (int j = 0; j < hourglass->size; j++){
            hourglass->grid[i][j] = *create_cell();
            hourglass->grid[i][j].is_in_hourglass = 1;}

    }
}

void fill_grid(Hourglass* hourglass, int grain_quantity) {
    int row = 0;
    int col = hourglass->size - 1;

    for (int i = 0; i < hourglass->size; i++) {
        for (int j = 0; j <= hourglass->size/2; j++) {
            if (grain_quantity == 0)
                break;
            add_grain(hourglass, row + i, col - j);
            grain_quantity--;
        }
    }
}

void print_grid(Hourglass* hourglass) {
    for (int i = 0; i < hourglass->size*2; i++) {
        for (int j = 0; j < hourglass->size; j++) {
            if (hourglass->grid[i][j].IsGrain == 1)
                printf(" * ");
            else if (hourglass->grid[i][j].is_in_hourglass == 1)
                printf(" . ");
            else
                printf(" ");
        }
        if (i == hourglass->size - 1)
            printf("\n ______________________");
        printf("\n");
    }
}

void one_epoch(Hourglass* hourglass, int downwards) {
    if (downwards == 0) {
        for (int i = hourglass->size*2-1; i >= 0; i--) {
            for (int j = 0; j < hourglass->size; j++) {
                if (hourglass->grid[i][j].IsGrain == 1) {
                    if (hourglass->grid[i][j].fallen_on_this_epoch == 1)
                        continue;
                    fall(i, j, hourglass, downwards);
                }
            }
        }
    } else {
        for (int i = 0; i < hourglass->size*2; i++) {
            for (int j = 0; j < hourglass->size; j++) {
                if (hourglass->grid[i][j].IsGrain == 1) {
                    if (hourglass->grid[i][j].fallen_on_this_epoch == 1)
                        continue;
                    fall(i, j, hourglass, downwards);
                }
            }
        }
    }
    for (int i = 0; i < hourglass->size*2; i++) {
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

int**  as_matrix(Hourglass* hourglass) {
    int rows = hourglass->size * 2;
    int cols = hourglass->size;
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            if (hourglass->grid[i][j].IsGrain == 1)
                matrix[i][j] = 1;
            else
                matrix[i][j] = 0;
        }
    }
    return matrix;
}