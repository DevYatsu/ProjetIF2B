#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_state.h"
#include "player.h"
#include "select.h"
#include  "game_state.h"
#include "save.h"

int main(void) {
    srand(time(0));
    printf("Hello, World!\n");

    const Option option = select_option();

    GameState game_state;

    switch (option) {
        case Start: {
            const __uint8_t mode = select_mode();
            const __uint8_t dim = select_dimension();

            game_state = init_game_state(mode, dim);
            break;
        }
        case Restart: {
            if (!save_file_exists()) {
                printf("Aucune partie sauvegardée trouvée.\n");
                return 0;
            }

            printf("Chargement de la partie...\n");
            game_state = load_game();
            printf("Partie chargée!\n");
            break;
        }
        case Leave:
            printf("Bye\n");
            return 0;
    }

    printf("Game state initialized\n");
    
    free_game_state(&game_state);
    return 0;
}