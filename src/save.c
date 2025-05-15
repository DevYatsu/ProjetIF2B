
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "piece.h"

const char* FILENAME = "savegame.dat";

const int MAX_GAME_STATE_STR_LEN = strlen("Conquest ") + // longueur du plus grand entre Conquest et Connect
                              strlen("User ") + // longueur du plus grand entre User et AI
                              3 + // longueur de dim (2 digits et  un espace)
                              (12 * 12 * 10) + // 12 (max dim) et 10 pour chaque piece
                              100; // securité

/**
 * @brief Sérialise un état de jeu (`GameState`) en une chaîne de caractères.
 *
 * Convertit l'état du jeu en une chaîne formatée contenant le mode, le joueur,
 * la dimension du plateau, et les pièces présentes sur le plateau.
 *
 * @param state Un pointeur vers l'état de jeu à sérialiser.
 * @return char* Une chaîne représentant l'état de jeu. Doit être libérée manuellement.
 * @warning Termine le programme avec `exit(EXIT_FAILURE)` en cas d'erreur d'allocation.
 */
char* serialize(const GameState* state) {
    char* str = malloc(sizeof(char) * MAX_GAME_STATE_STR_LEN);
    if (str == NULL) {
        perror("Failed to allocate memory for serialization");
        exit(EXIT_FAILURE);
    }

    // Initialize le premier caractère comme nul pour éviter les problèmes de concaténation
    str[0] = '\0';

    strcat(str, state->mode == Conquest ? "Conquest " : "Connect ");
    strcat(str, state->is_turn_of == User ? "User " : "AI ");

    const Board board = state->board;

    char dim_str[4];
    sprintf(dim_str, "%d ", board.dim);
    strcat(str, dim_str);

    for (uint8_t i = 0; i < board.dim; i++) {
        for (uint8_t j = 0; j < board.dim; j++) {
            const OptionChessPiece option_piece = board.tiles[i][j].piece;

            if (!option_piece.some) {
                strcat(str, "None ");
            } else {
                switch (option_piece.value.kind) {
                    case King: strcat(str, "King-"); break;
                    case Queen: strcat(str, "Queen-"); break;
                    case Rook: strcat(str, "Rook-"); break;
                    case Bishop: strcat(str, "Bishop-"); break;
                    case Knight: strcat(str, "Knight-"); break;
                    case Pawn: strcat(str, "Pawn-"); break;
                    default:
                        // unreachable
                        break;
                }

                strcat(str, stringify_player(option_piece.value.player));
                strcat(str, " ");
            }
        }
    }

    // Resize str pour eviter le gaspillage de mémoire
    char* re = realloc(str, sizeof(char) * (strlen(str) + 1));

    if (re == NULL) {
        printf("Failed to reallocate memory for serialization\n");
        free(str);
        exit(EXIT_FAILURE);
    }

    str = re;
    return str;
}

/**
 * @brief Désérialise une chaîne en un objet `GameState`.
 *
 * Reconstruit un état de jeu complet à partir d'une chaîne générée par `serialize()`.
 *
 * @param str Chaîne sérialisée représentant un `GameState`.
 * @return GameState L'état de jeu reconstruit.
 * @warning Termine le programme avec `exit(1)` en cas de format invalide ou d'erreur.
 */
GameState deserialize(const char* str) {
    if (str == NULL) {
        printf("Invalid serialized game state: NULL\n");
        exit(1);
    }

    GameState state;

    // pour une question de simpliciter
    // nous utiliserons sscanf pour parser la chaine
    // de caractere plutôt que strtok/strtok_r

    char mode[10];
    sscanf(str, "%s ", mode);

    // on avance le pointeur de str de la taille du mode + 1
    // pour ignorer le mode et l'espace qui suit
    str = str + strlen(mode) + 1;

    if (strcmp(mode, "Conquest") == 0) {
        state.mode = Conquest;
    } else if (strcmp(mode, "Connect") == 0) {
        state.mode = Connect;
    } else {
        printf("Invalid deserialized game mode: %s\n", mode);
        exit(1);
    }

    char player[10];
    sscanf(str, "%9s ", player);
    str = str + strlen(player) + 1;

    if (strcmp(player, "User") == 0) {
        state.is_turn_of = User;
    } else if (strcmp(player, "AI") == 0) {
        state.is_turn_of = AI;
    } else {
        printf("Invalid deserialized player: %s\n", mode);
        exit(1);
    }

    int dim;

    if (!sscanf(str, "%2u ", &dim) || dim < 6 || dim > 12) {
        printf("Invalid deserialized dimension: %d\n", dim);
        exit(1);
    }

    const uint8_t dim_len = dim < 10 ? 1 : 2;
    str = str + dim_len + 1;

    const Board board = init_board((uint8_t)dim);

    if (board.tiles == NULL) {
        printf("Failed to allocate memory for board rows\n");
        exit(1);
    }

    char piece_str[10];
    char user_str[7];

    for (uint8_t i = 0; i < board.dim; i++) {
        for (uint8_t j = 0; j < board.dim; j++) {
            // TODO add length constraint for each sscanf and scanf call
            sscanf(str, "%9s-%6s ", piece_str, user_str);
            const OptionChessPiece piece = deserialize_piece(piece_str, user_str, true);
            board.tiles[i][j] = init_tile(piece, i, j);
            str = str + strlen(piece_str) + 1;
        }
    }

    state.board = board;
    return state;
}

bool save_game(const GameState* state) {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("Failed to open file for saving");
        return false;
    }

    char* serialized = serialize(state);

    if (fprintf(file, "%s", serialized) < 0) {
        perror("Failed to write to file");
        free(serialized);
        fclose(file);
        return false;
    }

    free(serialized);
    fclose(file);
    return true;
}

bool save_file_exists() {
    FILE* file = fopen(FILENAME, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}

GameState load_game() {
    FILE* file = fopen(FILENAME, "r");

    if (file == NULL) {
        perror("Failed to open file for loading");
        exit(EXIT_FAILURE);
    }

    char* content = malloc(sizeof(char) * MAX_GAME_STATE_STR_LEN);
    if (content == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (fgets(content, MAX_GAME_STATE_STR_LEN, file) == NULL) {
        perror("Failed to read file content");
        free(content);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    const GameState state = deserialize(content);
    free(content);
    fclose(file);

    return state;
}