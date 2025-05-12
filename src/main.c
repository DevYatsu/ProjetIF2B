#include <stdio.h>
#include <stdlib.h>
#include <time.h>2
#include "game_state.h"
#include "player.h"
#include "select.h"
#include  "game_state.h"

int main(void) {
    srand(time(0));
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
    const GameState game_state = init_game_state(mode, dim);

    printf("Game state initialized\n");

    deserialize(" Conquest User");

    free_game_state(&game_state);
    return 0;
}