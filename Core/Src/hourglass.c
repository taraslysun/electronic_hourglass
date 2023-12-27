#include <stdio.h>
#include <stdlib.h>
#include "hourglass.h"

Hourglass* init_hourglass(int size_x, int size_y) {
    Hourglass* hourglass = (Hourglass*)malloc(sizeof(Hourglass));
    hourglass->size_x = size_x;
    hourglass->size_y = size_y;
    hourglass->grid = NULL;
    hourglass->grain_positions = NULL;
    return hourglass;
}

void create_grid(Hourglass* hourglass) {
    hourglass->grid = (Cell**)malloc(hourglass->size_y * sizeof(Cell*));
    for (int i = 0; i < hourglass->size_y; i++) {
        hourglass->grid[i] = (Cell*)malloc(hourglass->size_x * sizeof(Cell));
        for (int j = 0; j < hourglass->size_x; j++)
            hourglass->grid[i][j] = *create_cell();
    }

    int start = hourglass->size_y / 2 - hourglass->size_x / 2;

    for (int i = 0; i < start; i++) {
        for (int j = 0; j < hourglass->size_x; j++)
            hourglass->grid[i][j] = *add_to_hourglass();
    }
    for (int i = hourglass->size_y - 1; i >= hourglass->size_y - start; i--) {
        for (int j = 0; j < hourglass->size_x; j++)
            hourglass->grid[i][j] = *add_to_hourglass();
    }
    int offset = 0;
    for (int i = offset; i < hourglass->size_y - offset; i++) {
        for (int j = offset; j < hourglass->size_x - offset; j++)
            hourglass->grid[i+start][j] = *add_to_hourglass();
        offset++;
    }
    offset = 0;
    for (int i = hourglass->size_y - offset - 1; i >= offset; i--) {
        for (int j = offset; j < hourglass->size_x - offset; j++)
            hourglass->grid[i-start][j] = *add_to_hourglass();
        offset++;
    }
}

void fill_grid(Hourglass* hourglass, int grain_quantity) {
    while (grain_quantity > 0) {
        for (int i = 0; i < hourglass->size_y; i++) {
            for (int j = 0; j < hourglass->size_x; j++) {
                if (grain_quantity == 0)
                    break;
                if (hourglass->grid[i][j].is_in_hourglass == 1 && hourglass->grid[i][j].IsGrain == 0){
                    hourglass->grid[i][j].IsGrain = 1;
                    grain_quantity--;
                }
            }
            if (grain_quantity == 0)
                break;
        }
    }
}

void print_grid(Hourglass* hourglass) {
    for (int i = 0; i < hourglass->size_y; i++) {
        for (int j = 0; j < hourglass->size_x; j++) {
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

void one_epoch(Hourglass* hourglass, int x, int y) {
    for (int i = hourglass->size_y-1; i >=0 ; i--) {
        for (int j=0;j < hourglass->size_x ; j++) {
            if (hourglass->grid[i][j].IsGrain == 1) {
                if (hourglass->grid[i][j].fallen_on_this_epoch == 1) continue;
                fall(i, j, hourglass, x, y);
            }
        }
    }
    for (int i = 0; i < hourglass->size_y; i++) {
        for (int j = 0; j < hourglass->size_x; j++) {
            if (hourglass->grid[i][j].IsGrain == 1)
                hourglass->grid[i][j].fallen_on_this_epoch = 0;
        }
    }
}

void add_grain(Hourglass* hourglass, int row, int col) {
    hourglass->grid[row][col] = *fill_cell();
    hourglass->grid[row][col].is_in_hourglass = 1;
}

Hourglass* create_hourglass(int size_x, int size_y, int grain_quantity) {
    Hourglass* hourglass = init_hourglass(size_x, size_y);
    create_grid(hourglass);
    fill_grid(hourglass, grain_quantity);
    return hourglass;
}


int **as_matrix(Hourglass *hourglass) {
    int** matrix = (int**)malloc(hourglass->size_y * sizeof(int*));
    for (int i = 0; i < hourglass->size_y; i++) {
        matrix[i] = (int*)malloc(hourglass->size_x * sizeof(int));
        for (int j = 0; j < hourglass->size_x; j++) {
            if (hourglass->grid[i][j].IsGrain == 1)
                matrix[i][j] = 1;
            else if (hourglass->grid[i][j].is_in_hourglass == 1)
                matrix[i][j] = 0;
            else
                matrix[i][j] = -1;
        }
    }
    return matrix;
}