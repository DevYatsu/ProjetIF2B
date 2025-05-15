#include <ctype.h>
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

void print_title_screen() {
    clear_screen();

    print_text("     | |\n");
    print_text("  ___| |__   ___  ___ ___\n");
    print_text(" / __| '_ \\ / _ \\/ __/ __|\n");
    print_text("| (__| | | |  __/\\__ \\__ \\\n");
    print_text(" \\___|_| |_|\\___||___/___/\n\n\n");

    print_text("\n⚠️  Ce jeu est conçu pour être joué dans un vrai terminal.\n");
    print_text("   Évitez d'utiliser la sortie CLion / IDE (Run) pour un affichage correct.\n\n\n");

    print_text("Appuyez sur <Entrer> pour commencer la partie.\n\n\n");

    while(getchar() != '\n') {
        // Attendre que l'utilisateur appuie sur une touche
    }

    clear_screen();
}

// questions pour le prof
// est-ce que les deux joueurs sont humains ou bien l'un est un bot ?
int main(void) {
    srand(time(0));
    print_title_screen();

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
            sleep_ms(750);
            clear_screen();
            print_text("Partie chargée!\n");
            break;
        }
        case Leave:
            print_text("Bye\n");
            return 0;
        default:
            // unreachable
            return 1;
    }

    if (game_state.is_white == User) {
        print_text("Vous êtes les blancs!\n");
    } else {
        print_text("Vous êtes les noirs!\n");
    }

    sleep_ms(500);
    clear_screen();
    sleep_ms(200);

    bool game_over = false;

    while (!game_over) {
        print_board(&game_state);

        const RoundOption round_option = select_round_option();

        switch (round_option) {
            case Play: {
                printf("Vous avez choisi de jouer.\n");

                switch (game_state.mode) {
                    case Conquest: {
                        char nom_piece[10];
                        printf("Quelle pièce souhaitez-vous jouer ? ");
                        scanf("%9s", nom_piece);

                        const char* current_player_str = stringify_player(game_state.is_turn_of);
                        OptionChessPiece option_piece = deserialize_piece(nom_piece, current_player_str, true);

                        while (!option_piece.some) {
                            printf("Pièce invalide: ");
                            scanf("%9s", nom_piece);
                            option_piece = deserialize_piece(nom_piece, current_player_str, true);
                        }

                        const uint8_t dim = game_state.board.dim;
                        uint8_t x = dim;
                        uint8_t y = dim;

                        while (x >= dim || y >= dim) {
                            char target_tile[4];
                            printf("Où souhaitez-vous la placer ? ");
                            scanf("%3s", target_tile);

                            if (!isupper(target_tile[0])) {
                                printf("Erreur : le premier caractère doit être une lettre (ex: A3).\n");
                                continue;
                            }

                            if (!isdigit(target_tile[1])) {
                                printf("Erreur : la ligne doit commencer par un chiffre (ex: A3).\n");
                                continue;
                            }

                            if (target_tile[2] != '\0' && !isdigit(target_tile[2])) {
                                printf("Erreur : mauvais format (ex: A3 ou A10).\n");
                                continue;
                            }

                            const int row = (target_tile[2] == '\0')
                                        ? (target_tile[1] - '0')
                                        : ((target_tile[1] - '0') * 10 + (target_tile[2] - '0'));

                            if (row < 0 || row > dim) {
                                printf("Erreur : ligne invalide (max %u).\n", dim);
                                continue;
                            }

                            x = toupper(target_tile[0]) - 'A';
                            y = dim - row;

                            if (x >= dim || y >= dim) {
                                printf("Erreur : coordonnées hors des limites (%ux%u).\n", dim, dim);
                                continue;
                            }

                            if (game_state.board.tiles[y][x].piece.some) {
                                printf("Erreur : Il y a déjà une pièce en %s.\n", target_tile);
                                x = y = dim;
                            }
                        }

                        game_state.board.tiles[y][x] = init_tile(option_piece);
                        clear_screen();
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