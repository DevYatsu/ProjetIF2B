#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "piece.h"

// TODO simplifier Tiles car un seul champs
/**
 * @brief Représente une case du plateau avec une pièce et ses coordonnées.
 */
typedef struct {
    OptionChessPiece piece; ///< La pièce d'échecs sur la tuile (ou aucune)
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
 * @brief Initialise une tuile avec une pièce et des coordonnées.
 *
 * Cette fonction crée une tuile contenant une pièce d'échecs donnée
 * et la positionne aux coordonnées (x, y) sur le plateau.
 *
 * @param piece Une option de la pièce à placer sur la tuile (Roi, Reine, etc.).
 * @return Tile La tuile initialisée avec les valeurs spécifiées.
 */
Tile init_tile(OptionChessPiece piece);

/**
 * @brief Libère la mémoire allouée dynamiquement pour un plateau.
 *
 * @param board Pointeur vers le plateau à libérer.
 */
void free_board(const Board *board);

#endif //BOARD_H
