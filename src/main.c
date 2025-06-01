#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_state.h"
#include "select.h"
#include "print.h"
#include "save.h"
#include "piece.h"

void print_bye() {
    // cf https://emojicombos.com/bye-ascii-art
    puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡤⠒⠒⠢⢄⡀⠀⠀⢠⡏⠉⠉⠉⠑⠒⠤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡞⠀⠀⠀⠀⠀⠙⢦⠀⡇⡇⠀⠀⠀⠀⠀⠀⠈⠱⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠊⠉⠉⠙⠒⢤⡀⠀⣼⠀⠀⢀⣶⣤⠀⠀⠀⢣⡇⡇⠀⠀⢴⣶⣦⠀⠀⠀⢳⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    puts("⠀⠀⠀⢀⣠⠤⢄⠀⠀⢰⡇⠀⠀⣠⣀⠀⠀⠈⢦⡿⡀⠀⠈⡟⣟⡇⠀⠀⢸⡇⡆⠀⠀⡼⢻⣠⠀⠀⠀⣸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    puts("⠀⢀⠖⠉⠀⠀⠀⣱⡀⡞⡇⠀⠀⣿⣿⢣⠀⠀⠈⣧⣣⠀⠀⠉⠋⠀⠀⠀⣸⡇⠇⠀⠀⠈⠉⠀⠀⠀⢀⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    puts("⣠⠏⠀⠀⣴⢴⣿⣿⠗⢷⡹⡀⠀⠘⠾⠾⠀⠀⠀⣿⣿⣧⡀⠀⠀⠀⢀⣴⠇⣇⣆⣀⢀⣀⣀⣀⣀⣤⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    puts("⣿⠀⠀⢸⢻⡞⠋⠀⠀⠀⢿⣷⣄⠀⠀⠀⠀⠀⣠⡇⠙⢿⣽⣷⣶⣶⣿⠋⢰⣿⣿⣿⣿⣿⣿⠿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    puts("⡿⡄⠀⠈⢻⣝⣶⣶⠀⠀⠀⣿⣿⣱⣶⣶⣶⣾⡟⠀⠀⠀⢈⡉⠉⢩⡖⠒⠈⠉⡏⡴⡏⠉⠉⠉⠉⠉⠉⠉⠉⡇⠀⠀⢀⣴⠒⠢⠤⣀");
    puts("⢣⣸⣆⡀⠀⠈⠉⠁⠀⠀⣠⣷⠈⠙⠛⠛⠛⠉⢀⣴⡊⠉⠁⠈⢢⣿⠀⠀⠀⢸⠡⠀⠁⠀⠀⠀⣠⣀⣀⣀⣀⡇⠀⢰⢁⡇⠀⠀⠀⢠");
    puts("⠀⠻⣿⣟⢦⣤⡤⣤⣴⣾⡿⢃⡠⠔⠒⠉⠛⠢⣾⢿⣿⣦⡀⠀⠀⠉⠀⠀⢀⡇⢸⠀⠀⠀⠀⠀⠿⠿⠿⣿⡟⠀⢀⠇⢸⠀⠀⠀⠀⠘");
    puts("⠀⠀⠈⠙⠛⠿⠿⠿⠛⠋⢰⡋⠀⠀⢠⣤⡄⠀⠈⡆⠙⢿⣿⣦⣀⠀⠀⠀⣜⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⢀⠃⠀⡸⠀⠇⠀⠀⠀⠀⠀");
    puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⢣⠀⠀⠈⠛⠁⠀⢴⠥⡀⠀⠙⢿⡿⡆⠀⠀⢸⠀⢸⢰⠀⠀⠀⢀⣿⣶⣶⡾⠀⢀⠇⣸⠀⠀⠀⠀⠀⠀");
    puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⡀⢇⠀⠀⠀⢀⡀⠀⠀⠈⢢⠀⠀⢃⢱⠀⠀⠀⡇⢸⢸⠀⠀⠀⠈⠉⠉⠉⢱⠀⠼⣾⣿⣿⣷⣦⠴⠀⠀");
    puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢱⠘⡄⠀⠀⢹⣿⡇⠀⠀⠈⡆⠀⢸⠈⡇⢀⣀⣵⢨⣸⣦⣤⣤⣄⣀⣀⣀⡞⠀⣠⡞⠉⠈⠉⢣⡀⠀⠀");
    puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢃⠘⡄⠀⠀⠉⠀⠀⣠⣾⠁⠀⠀⣧⣿⣿⡿⠃⠸⠿⣿⣿⣿⣿⣿⣿⠟⠁⣼⣾⠀⠀⠀⠀⢠⠇⠀⠀");
    puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡄⠹⣀⣀⣤⣶⣿⡿⠃⠀⠀⠀⠈⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠁⠀⠀⢻⣿⣷⣦⣤⣤⠎⠀⠀⠀");
    puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣤⣿⡿⠟⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠀⠀⠀⠀⠀");
}

void print_title_screen() {
    // cf https://ascii.co.uk/art/chess
    clear_screen();

    print_text("     | |\n");
    print_text("  ___| |__   ___  ___ ___\n");
    print_text(" / __| '_ \\ / _ \\/ __/ __|\n");
    print_text("| (__| | | |  __/\\__ \\__ \\\n");
    print_text(" \\___|_| |_|\\___||___/___/\n\n\n\n\n");

    print_text("\n⚠️  Ce jeu est conçu pour être joué dans un vrai terminal.\n");
    print_text("   Évitez d'utiliser la sortie CLion / IDE (Run) pour un affichage correct.\n\n\n\n\n");

    print_text("Appuyez sur <Entrer> pour commencer la partie.\n\n\n\n\n");

    while(getchar() != '\n') {
        // Attendre que l'utilisateur appuie sur une touche
    }

    clear_screen();
}

// questions pour le prof
// est-ce que les deux joueurs sont humains ou bien l'un est un bot ?
int main(void) {
    srand(time(0));
    // on skip pr le moment c'est trop long
    // print_title_screen();

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
            print_bye();
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

    bool game_gave_up = false;

    while (!game_gave_up && !has_no_pieces_left(get_user_turn_count_tracker(&game_state))) {
        // pour centrer le titre
        for (int i = 0; i < game_state.board.dim / 2 + 1; i++) { printf("   ");}
        printf("Au tour du joueur %d\n", game_state.is_turn_of == User ? 1 : 2);

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
                        Tile tile = deserialize_tile(nom_piece, current_player_str, "", true);

                        PieceCountTracker *piece_counter = (game_state.is_turn_of == User)
                            ? &game_state.piece_counter_1
                            : &game_state.piece_counter_2;

                        bool piece_allowed = tile.some ? add_piece(piece_counter, tile.value.kind) : false;

                        while (!tile.some || !piece_allowed) {
                            if (!piece_allowed) {
                                printf("Vous n'avez plus de %s à jouer: ", nom_piece);
                                piece_allowed = true;
                            } else {
                                printf("Pièce inconnue: ");
                            }
                            scanf("%9s", nom_piece);
                            tile = deserialize_tile(nom_piece, current_player_str, "", true);

                            if (tile.some) {
                                piece_allowed = add_piece(piece_counter, tile.value.kind);
                            }
                        }

                        const uint8_t dim = game_state.board.dim;
                        uint8_t x = dim;
                        uint8_t y = dim;

                        // TODO: gros probleme, exit automatique parfois lorsque l'utilisateur entre le case ou il veut placer la pièce

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

                            if (game_state.board.tiles[y][x].some) {
                                printf("Erreur : Il y a déjà une pièce en %s.\n", target_tile);
                                x = y = dim;
                            }
                        }

                        tile.captured_by = player_option(game_state.is_turn_of);
                        game_state.board.tiles[y][x] = tile;
                        apply_conquest_capture(&game_state, x, y, tile.value, game_state.is_turn_of);

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

                game_state.is_turn_of = (game_state.is_turn_of == User) ? Opponent : User;
                break;
            }
            case GiveUp: {
                printf("Le joueur %s abandonne la partie. Partie terminée!\n", get_user_turn_name(&game_state));
                game_gave_up = true;
                break;
            }
            case SaveGame: {
                const bool success = save_game(&game_state);

                if (!success) {
                    printf("Une erreur est survenue, nous n'avons pas pu sauvegarder la partie\n");
                }
                clear_screen();
                print_text("Sauvegarde en cours...");
                sleep_ms(500);
                print_text("La partie a été sauvegardée avec succès.\n");
                sleep_ms(400);

                clear_screen();
                break;
            }
        }
    }

    const PieceCountTracker *current_tracker = get_user_turn_count_tracker(&game_state);
    if (has_no_pieces_left(current_tracker)) {
        print_text("Toutes les pièces ont été jouées.\n");
        print_text("La partie est terminée!\n");
        const uint8_t user_count = get_captured_count_of(&game_state,User);
        const uint8_t opponent_count = get_captured_count_of(&game_state,Opponent);

        if (user_count > opponent_count) {
            printf("Victoire de l'utilisateur ! (%d cases contre %d)\n", user_count, opponent_count);
        } else if (opponent_count > user_count) {
            printf("Victoire de l'adversaire ! (%d cases contre %d)\n", opponent_count, user_count);
        } else {
            printf("Égalité parfaite : %d cases chacun !\n", user_count);
        }
    }

    free_game_state(&game_state);
    return 0;
}