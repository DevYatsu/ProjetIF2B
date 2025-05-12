#include <stdio.h>
#include "select.h"

int main(void) {
    printf("Hello, World!\n");
    const __uint8_t option = select_option();
    __uint8_t mode = 0;

    switch (option) {
        case 1:
            mode = select_mode();
            break;
        case 2:
            printf("Reprendre une partie\n");
            break;
        case 3:
            printf("Bye\n");
            return 0;
        default:
            // unreachable!
            printf("Choix invalide\n");
            return 1;
    }

    if (mode != 0) {
        printf("Vous avez choisi le mode %d\n", mode);
    }

    const __uint8_t dim = select_dimension();

    return 0;
}