#include <stdio.h>
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

void toggle_user_turn(GameState* state) {
    if (state->is_turn_of == User) {
        state->is_turn_of = AI;
    } else {
        state->is_turn_of = User;
    }
}

char* get_user_turn_name(const GameState* state) {
    return state->is_turn_of == User ? "User" : "AI";
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

    printf("  \tdim=%d\n", state->board.dim);

    for (uint8_t i = 0; i < state->board.dim; i++) {
        for (uint8_t j = 0; j < state->board.dim; j++) {
            printf(" %d ", state->board.tiles[i][j].piece);
        }
        printf("\n");
    }
}


void free_game_state(const GameState *state) {
    debug_game_state(state);
    free_board(&state->board);
}
