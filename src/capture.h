#ifndef CAPTURE_H
#define CAPTURE_H
#include "game_state.h"

/**
 * @brief Applique la capture de pièces selon les règles du mode Conquest.
 *
 * Lorsque le joueur capturant place une pièce sur une case occupée,
 * toutes les pièces adjacentes (selon les règles de capture) sont capturées.
 *
 * @param state Pointeur vers l'état du jeu actuel.
 * @param x Coordonnée x de la case où la pièce est placée.
 * @param y Coordonnée y de la case où la pièce est placée.
 * @param piece La pièce qui est placée sur le plateau.
 * @param capturer Le joueur qui effectue la capture.
 */
void apply_conquest_capture(const GameState *state, uint8_t x, uint8_t y,
                            ChessPiece piece, Player capturer);

/**
 * @brief Checks if a tile at the given coordinates is captured by a piece of the specified kind.
 *
 * This function iterates through all tiles on the board to determine if any piece of the given kind,
 * captured by the current player, can capture the tile at the specified coordinates `(x, y)`.
 * The logic simulates the capture rules for each piece type.
 *
 * @param state Pointer to the current game state.
 * @param x The x-coordinate of the target tile.
 * @param y The y-coordinate of the target tile.
 * @param kind The type of piece to check for capture.
 * @return bool `true` if the tile is captured by a piece of the specified kind, `false` otherwise.
 */
bool is_tile_captured_by_piece_kind(const GameState *state, uint8_t x, uint8_t y, PieceKind kind);

/// Joue un tour dans le mode "Conquest".
///
/// Ce mode consiste à sélectionner une pièce capturée, choisir une position valide
/// où la poser, puis appliquer l'effet de capture de cette pièce.
///
/// @param game_state Pointeur vers l’état actuel du jeu.
void play_conquest_turn(const GameState *game_state);

/// Joue un tour dans le mode "Connect".
///
/// Ce mode est basé sur une logique de placement conditionnelle :
/// la validité dépend des pièces déjà posées.
/// Si un roi est placé, la partie prend fin immédiatement.
///
/// @param game_state Pointeur vers l’état actuel du jeu.
void play_connect_turn(GameState *game_state);


#endif //CAPTURE_H
