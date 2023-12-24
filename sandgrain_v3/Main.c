#include "main.h"
#include <stdio.h>
#include <stdbool.h>


int main() {
    Hourglass* hourglass = create_hourglass(30, 40, 100);
    printf("initial\n");
    print_grid(hourglass);

    int **  matrix = as_matrix(hourglass);
    for (int i = 0; i < hourglass->size_y; i++) {
        for (int j = 0; j < hourglass->size_x; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }



    int x = 0;
    int y = 1;
    for (int i = 0; i < 1000; i++) one_epoch(hourglass, x, y);
    printf("\n");
    printf("down\n");
    print_grid(hourglass);

    x = 0;
    y = -1;
    for (int i = 0; i < 1000; i++) one_epoch(hourglass, x, y);
    printf("\nup\n");
    print_grid(hourglass);


    x = -1;
    y = 0;
    for (int i = 0; i < 1000; i++) {
        one_epoch(hourglass, x, y);
    }
    printf("\n");
    printf("left\n");
    print_grid(hourglass);

    x = 1;
    y = 0;
    for (int i = 0; i < 1000; i++) {
        one_epoch(hourglass, x, y);
    }
    printf("\n");
    printf("right\n");
    print_grid(hourglass);

    x = 1;
    y = 1;
    for (int i = 0; i < 1000; i++) {
        one_epoch(hourglass, x, y);
    }
    printf("\n");
    printf("down right\n");
    print_grid(hourglass);

    x = -1;
    y = -1;
    for (int i = 0; i < 1000; i++) {
        one_epoch(hourglass, x, y);
    }
    printf("\n");
    printf("up left\n");
    print_grid(hourglass);

    x = 1;
    y = -1;
    for (int i = 0; i < 1000; i++) {
        one_epoch(hourglass, x, y);
    }
    printf("\n");
    printf("up right\n");
    print_grid(hourglass);

    x = -1;
    y = 1;
    for (int i = 0; i < 1000; i++) {
        one_epoch(hourglass, x, y);
    }
    printf("\n");
    printf("down left\n");
    print_grid(hourglass);


    return 0;
}
