
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "player.h"
#include "board.h"

typedef enum {
    Conquest,
    Connect
} GameMode;

typedef struct {
    GameMode mode;
    Board board;
    Player is_turn_of;
} GameState;

GameState deserialize(const char* str);

/**
 * Initialise un état de jeu complet avec le mode et la taille spécifiée.
 * Le joueur qui commence est choisi aléatoirement.
 */
GameState init_game_state(GameMode mode, uint8_t dim);

/**
 * Libère la mémoire du plateau associée à l'état de jeu.
 */
void free_game_state(const GameState *state);

#endif //GAME_STATE_H
