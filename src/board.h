#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

/**
 * @brief Types de pièces pouvant se trouver sur une case de l'échiquier (ou None=rien).
 */
typedef enum {
    None,    ///< Case vide
    King,    ///< Roi
    Queen,   ///< Reine
    Rook,    ///< Tour
    Bishop,  ///< Fou
    Knight,  ///< Cavalier
    Pawn,    ///< Pion
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
 * @brief Initialise une tuile avec une pièce et des coordonnées.
 *
 * Cette fonction crée une tuile contenant une pièce d'échecs donnée
 * et la positionne aux coordonnées (x, y) sur le plateau.
 *
 * @param piece La pièce à placer sur la tuile (Roi, Reine, etc.).
 * @param x La coordonnée horizontale de la tuile (colonne).
 * @param y La coordonnée verticale de la tuile (ligne).
 * @return Tile La tuile initialisée avec les valeurs spécifiées.
 */
Tile init_tile(ChessPiece piece, uint8_t x, uint8_t y);


/**
 * @brief Convertit une chaîne en type de pièce d'échecs.
 *
 * Cette fonction prend une chaîne de caractères (sans espaces au début ni à la fin)
 * représentant une pièce d'échecs en anglais, et retourne la valeur correspondante
 * du type `ChessPiece`.
 *
 * Chaînes valides :
 * - `"King"`
 * - `"Queen"`
 * - `"Rook"`
 * - `"Bishop"`
 * - `"Knight"`
 * - `"Pawn"`
 * - `"None"`
 *
 * Si la chaîne correspond exactement à `"None"`, la fonction retourne la pièce `None`.
 * Si la chaîne ne correspond à aucune valeur connue, un message d'erreur est affiché
 * sur la sortie standard d'erreur et le programme est arrêté avec `exit(EXIT_FAILURE)`.
 *
 * @param piece_str Chaîne représentant une pièce d'échecs (sans espaces extérieurs).
 * @return ChessPiece La pièce correspondante (peut-être None).
 */
ChessPiece piece_from_string(const char* piece_str);

/**
 * @brief Libère la mémoire allouée dynamiquement pour un plateau.
 *
 * @param board Pointeur vers le plateau à libérer.
 */
void free_board(const Board *board);

#endif //BOARD_H
