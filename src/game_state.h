
#ifndef GAME_STATE_H
#define GAME_STATE_H
#include "player.h"
#include "board.h"
#include "piece_count_tracker.h"

/**
 * @brief Enum représentant les modes de jeu disponibles.
 *
 * - `Conquest` : Mode basé sur la conquête du plateau.
 * - `Connect` : Mode basé sur la connexion de pièces (similaire à puissance 4, par exemple).
 */
typedef enum {
    Conquest = 1, ///< Mode conquête
    Connect   ///< Mode connecte
} GameMode;

/**
 * @brief Structure représentant l'état complet d'une partie.
 *
 * Cette structure regroupe toutes les informations nécessaires pour décrire
 * un instant précis du jeu :
 * - Le mode de jeu sélectionné
 * - L'état du plateau (`Board`)
 * - Le joueur dont c'est le tour (`Player`)
 */
typedef struct {
    GameMode mode;       ///< Mode de jeu actuel
    Board board;         ///< Plateau de jeu avec les positions des pièces
    Player is_turn_of;   ///< Joueur à qui c'est le tour (User ou AI)
    Player is_white;     ///< Joueur qui joue avec les pièces blanches
    PieceCountTracker piece_counter_1; // Compteur de pièces disponibles pour le joueur 1
    PieceCountTracker piece_counter_2; // Compteur de pièces disponibles pour le joueur 2
} GameState;

/**
 * Initialise un état de jeu complet avec le mode et la taille spécifiée.
 * Le joueur qui commence est choisi aléatoirement.
 */
GameState init_game_state(GameMode mode, uint8_t dim);

void toggle_user_turn(GameState* state);

char* get_user_turn_name(const GameState* state);
const PieceCountTracker* get_user_turn_count_tracker(const GameState* state);

void print_board(const GameState *state);

/**
 * Libère la mémoire du plateau associée à l'état de jeu.
 */
void free_game_state(const GameState *state);

#endif //GAME_STATE_H
