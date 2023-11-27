#include "main.h"
#include <stdio.h>


int main() {
    int downwards = 1;
    Hourglass* hourglass = create_hourglass(8, 15);
    print_grid(hourglass);
    int** matrix = as_matrix(hourglass);
    for (int i = 0; i < hourglass->size*2; i++) {
        for (int j = 0; j < hourglass->size; j++) {
            printf("%d ", matrix[i][j]);
        }
        if (i == hourglass->size - 1)
            printf("\n");
        else
            printf("  ");
        printf("\n");
    }
    printf("\n\nNow we are going downwards\n\n");
    for (int i = 0; i < 50; i++){
        one_epoch(hourglass, downwards);
    }
    matrix = as_matrix(hourglass);
    for (int i = 0; i < hourglass->size*2; i++) {
        for (int j = 0; j < hourglass->size; j++) {
            printf("%d ", matrix[i][j]);
        }
        if (i == hourglass->size - 1)
            printf("\n");
        else
            printf("  ");
        printf("\n");
    }
    print_grid(hourglass);
    printf("\n\nNow we are going upwards\n\n");

    downwards = 0;
    for (int i = 0; i < 20; i++)
        one_epoch(hourglass, downwards);
    print_grid(hourglass);
    matrix = as_matrix(hourglass);
    for (int i = 0; i < hourglass->size*2; i++) {
        for (int j = 0; j < hourglass->size; j++) {
            printf("%d ", matrix[i][j]);
        }
        if (i == hourglass->size - 1)
            printf("\n");
        else
            printf("  ");
        printf("\n");
    }
    return 0;
}
