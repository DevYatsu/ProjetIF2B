#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "piece.h"

/**
 * @brief Représente une tuile sur le plateau de jeu.
 *
 * Chaque tuile peut contenir une pièce d'échecs ou être vide.
 * Le champ `some` indique si la tuile contient une pièce.
 */
typedef struct {
    bool some;
    ChessPiece value;
} Tile;

/**
 * @brief Représente le plateau de jeu sous forme de grille de tuiles.
 */
typedef struct {
    Tile **tiles;   ///< Tableau 2D de pointeurs vers des tuiles
    uint8_t dim;    ///< Dimension du plateau (dim x dim)
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
 * @brief Désérialise une chaîne de caractères en une tuile avec une pièce.
 *
 * Cette fonction convertit une représentation textuelle d'une pièce et de son joueur
 * en une tuile contenant la pièce correspondante.
 *
 * @param piece_str Chaîne représentant le type de la pièce (ex: "King", "Queen", etc.).
 * @param player_str Chaîne représentant le joueur auquel appartient la pièce (ex: "User", "AI").
 * @param from_user_input Indique si la désérialisation provient d'une entrée utilisateur, dans quel cas on ne renvoie pas d'erreur arrêtant le programme.
 * @return Tile La tuile contenant la pièce désérialisée.
 */
Tile deserialize_piece(const char* piece_str, const char* player_str, bool from_user_input);


#endif //BOARD_H
