
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "piece.h"

const char* FILENAME = "savegame.dat";

const unsigned int MAX_GAME_STATE_STR_LEN = strlen("Conquest ") + // longueur du plus grand entre Conquest et Connect
                              strlen("User ") + // longueur du plus grand entre User et AI
                              3 + // longueur de dim (2 digits et  un espace)
                              (12 * 12 * 10) + // 12 (max dim) et 10 pour chaque piece
                              100; // securité

/**
 * @brief Sérialise un état de jeu (`GameState`) en chaîne de caractères.
 *
 * Cette fonction convertit l'ensemble de l'état du jeu en une chaîne formatée, incluant :
 * - Le mode de jeu (`Conquest` ou `Connect`)
 * - Le joueur à qui c'est le tour (`User` ou `AI`)
 * - La dimension du plateau
 * - La liste de toutes les pièces sur le plateau (de gauche à droite, ligne par ligne)
 *
 * La chaîne générée contient les éléments séparés par des espaces, par exemple :
 * `"Conquest User 8 King Queen ... None "` (jusqu'à `dim × dim` pièces).
 *
 * La mémoire est initialement allouée avec une marge de sécurité, puis réduite à la taille exacte
 * avec `realloc()` avant d'être retournée.
 *
 * @param state Un pointeur vers l'état de jeu à sérialiser.
 * @return char* Une chaîne de caractères représentant l'état de jeu.
 *         Doit être libérée manuellement avec `free()`.
 *
 * @warning Termine le programme avec `exit(EXIT_FAILURE)` si une allocation échoue.
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
            const ChessPiece piece = board.tiles[i][j].piece;
            switch (piece) {
                case King: strcat(str, "King "); break;
                case Queen: strcat(str, "Queen "); break;
                case Rook: strcat(str, "Rook "); break;
                case Bishop: strcat(str, "Bishop "); break;
                case Knight: strcat(str, "Knight "); break;
                case Pawn: strcat(str, "Pawn "); break;
                case None: strcat(str, "None "); break;
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

char* serialize(const GameState* state);

/**
 * @brief Désérialise une chaîne en un objet `GameState`.
 *
 * Cette fonction prend une chaîne générée par `serialize()` et reconstruit
 * un état de jeu complet à partir de celle-ci.
 *
 * La chaîne est supposée avoir un format valide, sans caractères invalides ou inattendus.
 * Elle doit contenir au minimum :
 * - Un mode (`Conquest` ou `Connect`)
 * - Un joueur (`User` ou `AI`)
 * - Une dimension (`6` à `12`)
 * - Une séquence de `dim × dim` pièces (`King`, `Queen`, ..., `None`)
 *
 * Les champs sont lus dans l'ordre à l'aide de `sscanf()` et la position de lecture
 * dans la chaîne est mise à jour manuellement après chaque lecture.
 *
 * @param str Chaîne sérialisée représentant un `GameState`, au format attendu.
 * @return GameState L'état de jeu reconstruit à partir de la chaîne.
 *
 * @warning Termine le programme avec `exit(1)` en cas de format invalide,
 *          de contenu inattendu ou d'erreur d'allocation mémoire.
 */
GameState deserialize(const char* str) {
    GameState state;

    // pour une question de simpliciter
    // nous utiliserons sscanf pour parser la chaine
    // de caractere plutôt que strtok/strtok_r

    char mode[10];
    sscanf(str, "%s ", mode);

    if (strcmp(mode, "Conquest") == 0) {
        state.mode = Conquest;
    } else if (strcmp(mode, "Connect") == 0) {
        state.mode = Connect;
    } else {
        printf("Invalid deserialized game mode: %s\n", mode);
        exit(1);
    }

    // on avance le pointeur de str de la taille du mode + 1
    // pour ignorer le mode et l'espace qui suit
    str = str + strlen(mode) + 1;

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

    for (uint8_t i = 0; i < board.dim; i++) {
        for (uint8_t j = 0; j < board.dim; j++) {
            // TODO add length constraint for each sscanf and scanf call
            sscanf(str, "%9s ", piece_str);
            const ChessPiece piece = deserialize_piece(piece_str);
            board.tiles[i][j] = init_tile(piece, i, j);
            str = str + strlen(piece_str) + 1;
        }
        printf(" str: %s\n", str);
    }

    state.board = board;
    return state;
}

bool save_game(const GameState* state) {
    FILE* file = fopen(FILENAME, "w+");

    if (file == NULL) {
        perror("Failed to open file for saving");
        return true;
    }

    char* serialized = serialize(state);

    if (!fprintf(file, "%s", serialized)) {
        perror("Failed to write to file");
        fclose(file);
        return true;
    }

    free(serialized);
    fclose(file);
    return false;
}

bool save_file_exists() {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        return false; // Le fichier n'existe pas
    }

    fclose(file);
    return true; // Le fichier existe
}

GameState load_game() {
    FILE* file = fopen(FILENAME, "r");

    if (file == NULL) {
        perror("Failed to open file for loading");
        exit(EXIT_FAILURE);
    }

    char* content = malloc(sizeof(char) * MAX_GAME_STATE_STR_LEN);
    fscanf(file, "%s", content);

    const GameState state = deserialize(content);
    free(content);
    fclose(file);

    return state;
}