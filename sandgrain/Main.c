#include "main.h"
#include <stdio.h>
#include <stdbool.h>


int main() {
    int downwards = 1;
    Hourglass* hourglass = create_hourglass(7, 10);

    for (int i = 0; i < 10; i++){
        one_epoch(hourglass, downwards);
        print_grid(hourglass);
        printf("\n");}

    downwards = 0;
    for (int i = 0; i < 1000; i++)
        one_epoch(hourglass, downwards);
    print_grid(hourglass);

    return 0;
}
