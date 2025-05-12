
#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

/**
 * @brief Types de pièces pouvant se trouver sur une case de l'échiquier (ou None=rien).
 */
typedef enum {
    King,    ///< Roi
    Queen,   ///< Reine
    Rook,    ///< Tour
    Bishop,  ///< Fou
    Knight,  ///< Cavalier
    Pawn,    ///< Pion
    None     ///< Case vide
} ChessPiece;

/**
 * @brief Représente une case du plateau avec une pièce et ses coordonnées.
 */
typedef struct {
    ChessPiece piece; ///< Type de pièce sur la case (ou Rien)
    uint8_t x;      ///< Coordonnée horizontale (colonne)
    uint8_t y;      ///< Coordonnée verticale (ligne)
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

#endif //BOARD_H
