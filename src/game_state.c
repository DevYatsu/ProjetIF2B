#include <stdio.h>
#include <string.h>
#include "game_state.h"

#include <stdlib.h>

#include "board.h"

GameState init_game_state(const GameMode mode, const uint8_t dim) {
    GameState state;
    state.mode = mode;
    state.board = init_board(dim);
    state.is_turn_of = random_player();
    return state;
}

void free_game_state(const GameState *state) {
    free_board(&state->board);
}

void debug_game_state(const GameState* state) {
    printf("GameState:\n");
    printf("  Mode: %s\n", state->mode == Conquest ? "Conquest" : "Connect");
    printf("  Player: %s\n", state->is_turn_of == User ? "User" : "AI");
    printf("  Board:\n");

    if (state->board.tiles == NULL) {
        printf("    Board is NULL\n");
        return;
    }

    printf("  \tdim:%d\n", state->board.dim);

    for (uint8_t i = 0; i < state->board.dim; i++) {
        for (uint8_t j = 0; j < state->board.dim; j++) {
            printf(" %d ", state->board.tiles[i][j].piece);
        }
        printf("\n");
    }
}

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
    sscanf(str, "%s ", player);

    if (strcmp(player, "User") == 0) {
        state.is_turn_of = User;
    } else if (strcmp(player, "AI") == 0) {
        state.is_turn_of = AI;
    } else {
        printf("Invalid deserialized player: %s\n", mode);
        exit(1);
    }

    int dim;
    sscanf(str, "%u ", &dim);

    if (dim < 6 || dim > 12) {
        printf("Invalid deserialized dimension: %d\n", dim);
        exit(1);
    }

    Board board;

    board.dim = (uint8_t)dim;
    board.tiles = malloc(dim * sizeof(Tile*));

    if (board.tiles == NULL) {
        printf("Failed to allocate memory for board rows\n");
        exit(1);
    }

    debug_game_state(&state);

    exit(0);
    return state;
}
