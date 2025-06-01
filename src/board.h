#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include <stdint.h>

/**
 * @brief Représente une tuile sur le plateau de jeu.
 *
 * Chaque tuile peut contenir une pièce d'échecs ou être vide.
 * Le champ `some` indique si la tuile contient une pièce.
 */
typedef struct {
  bool some;
  ChessPiece value;
  PlayerOption captured_by;
} Tile;

/**
 * @brief Représente le plateau de jeu sous forme de grille de tuiles.
 */
typedef struct {
  Tile **tiles; ///< Tableau 2D de pointeurs vers des tuiles
  uint8_t dim;  ///< Dimension du plateau (dim x dim)
} Board;

/**
 * @brief Initialise un plateau vide de dimension spécifiée.
 *
 * Alloue dynamiquement la mémoire pour un tableau 2D de tuiles.
 *
 * @param dim La dimension du plateau (entre 6 et 12 typiquement).
 * @return Board Le plateau initialisé.
 */
Board init_board(uint8_t dim);

/**
 * @brief Libère la mémoire allouée dynamiquement pour un plateau.
 *
 * @param board Pointeur vers le plateau à libérer.
 */
void free_board(const Board *board);

/**
 * @brief Crée une tuile vide (sans pièce).
 *
 * @return Tile Une tuile vide.
 */
Tile empty_tile();

/**
 * @brief Crée une tuile contenant une pièce d'échecs.
 *
 * @param piece La pièce à placer sur la tuile.
 * @return Tile Une tuile contenant la pièce spécifiée.
 */
Tile tile_with_piece(ChessPiece piece);

/**
 * @brief Désérialise une chaîne de caractères en une tuile.
 *
 * Cette fonction prend une représentation textuelle d'une pièce et la convertit
 * en une structure `Tile`. Elle gère les cas où la tuile est vide ou contient
 * une pièce spécifique, ainsi que le joueur qui a capturé la pièce.
 *
 * @param piece_str Chaîne représentant le type de pièce (ex: "Pawn").
 * @param player_str Chaîne représentant le joueur (ex: "User").
 * @param captured_by_str Chaîne représentant le joueur qui a capturé la pièce
 * (ex: "Opponent").
 * @param from_user_input Indique si l'entrée provient de l'utilisateur (pour
 * des messages d'erreur).
 * @return Tile La tuile désérialisée.
 */
Tile deserialize_tile(const char *piece_str, const char *player_str,
                      const char *captured_by_str, bool from_user_input);

#endif // BOARD_H
