#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_state.h"
#include "select.h"
#include "print.h"
#include "save.h"
#include "piece.h"

// #include <sys/ioctl.h>
// #include <unistd.h>

//
// void move_cursor(int row, int col) {
//     printf("\033[%d;%dH", row, col);
//     fflush(stdout);
// }
//
//
// int get_terminal_size(int* rows, int* cols) {
//     struct winsize w;
//     if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) return -1;
//     *rows = w.ws_row;
//     *cols = w.ws_col;
//     return 0;
// }
//
// void center_cursor() {
//     int rows, cols;
//     if (get_terminal_size(&rows, &cols) == -1) {
//         perror("Failed to get terminal size");
//         return;
//     }
//
//     int center_row = rows / 2;
//     int center_col = (cols - (cols / 3)) / 2;
//
//     move_cursor(center_row, center_col);
//
//     print_effect("", 20);
//     fflush(stdout);
// }

void test() {
    printf("test\n");       // Line 1
    printf("test");         // Line 2 (cursor is here)
    fflush(stdout);

    sleep_ms(1000);         // Pause to see output

    // Move up at the start of line 1
    printf("\033[A\r");
    printf("line 1 replaced\n");
    fflush(stdout);

    // Overwrite line 2
    printf("line 2 replaced\n");
    fflush(stdout);

    exit(0);
}

// questions pour le prof
// est-ce que les deux joueurs sont humains ou bien l'un est un bot ?
int main(void) {
    /*
     * imprimer cela au debut
     *_
     | |
  ___| |__   ___  ___ ___
 / __| '_ \ / _ \/ __/ __|
| (__| | | |  __/\__ \__ \
 \___|_| |_|\___||___/___/
 */

    srand(time(0));
    print_effect("=== Bienvenue dans le jeu ===  ", 10);
    sleep_ms(200);
    erase_effect("=== Bienvenue dans le jeu ===  ", 10);
    sleep_ms(200);

    // print_effect("=== Appuyez sur <Entrer> pour continuer ===  ", 50);
    //
    // while (getchar() != '\n') {
    //     // Attendre que l'utilisateur appuie sur une touche
    // }
    //
    // erase_effect("\n=== Appuyez sur une touche pour continuer ===  ", 25);

    const StartOption option = select_option();
    clear_screen();

    GameState game_state;

    // 1 fonction == 1 fonctionnalité

    switch (option) {
        case Start: {
            const GameMode mode = select_mode();
            clear_screen();

            const uint8_t dim = select_dimension();
            clear_screen();

            game_state = init_game_state(mode, dim);
            break;
        }
        case Restart: {
            if (!save_file_exists()) {
                print_effect("Aucune partie sauvegardée trouvée.\n", 50);
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
        default:
            // unreachable
            return 1;
    }

    printf("Game state initialized \n");

    if (game_state.is_white == User) {
        printf("Vous êtes les blancs!\n");
    } else {
        printf("Vous êtes les noirs!\n");
    }

    print_board(&game_state);

    bool game_over = false;

    while (!game_over) {
        const RoundOption round_option = select_round_option();

        switch (round_option) {
            case Play: {
                printf("Vous avez choisi de jouer.\n");

                switch (game_state.mode) {
                    case Conquest: {
                        char nom_piece[10];
                        printf("Quelle pièce souhaitez-vous jouer ? ");
                        scanf("%s", nom_piece);

                        const char* current_player_str = stringify_player(game_state.is_turn_of);
                        OptionChessPiece option_piece = deserialize_piece(nom_piece, current_player_str, true);

                        while (!option_piece.some) {
                            printf("Pièce invalide: ");
                            scanf("%s", nom_piece);
                            option_piece = deserialize_piece(nom_piece, current_player_str, true);
                        }

                        printf("Où souhaitez-vous la placer ? ");


                        break;
                    }
                    case Connect: {

                        break;
                    }
                    default:
                        // unreachable
                        return 1;
                }
                break;
            }
            case GiveUp: {
                printf("Le joueur %s abandonne la partie. Partie terminée!\n", get_user_turn_name(&game_state));
                game_over = true;
                break;
            }
            case SaveGame: {
                const bool success = save_game(&game_state);

                if (!success) {
                    printf("Une erreur est survenue, nous n'avons pas pu sauvegarder la partie\n");
                }

                break;
            }
        }
    }

    free_game_state(&game_state);
    return 0;
}