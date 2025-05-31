
#ifndef PIECE_H
#define PIECE_H
#include "player.h"
#include <stdbool.h>

/**
 * @brief Types de pièces pouvant se trouver sur une case de l'échiquier.
 */
typedef enum {
    King,    ///< Roi
    Queen,   ///< Reine
    Rook,    ///< Tour
    Bishop,  ///< Fou
    Knight,  ///< Cavalier
    Pawn,    ///< Pion
} PieceKind;

/**
 * @brief Représente une pièce d’échecs sur le plateau.
 *
 * Cette structure contient le type de la pièce (Roi, Reine, etc.) et
 * l'identifiant du joueur auquel elle appartient (Joueur humain ou IA).
 */
typedef struct {
    PieceKind kind; ///< Type de la pièce
    Player player; ///< Joueur auquel appartient la pièce
} ChessPiece;

/**
 * @brief Représente une pièce d’échecs sous forme de chaîne de caractères.
 */
typedef struct {
    const char* line1;
    const char* line2;
    const char* line3;
} AsciiPiece;

/**
 * @brief Convertit un type de pièce en sa représentation ASCII pour les pièces blanches.
 *
 * Cette fonction retourne une structure `AsciiPiece` contenant les lignes ASCII
 * représentant la pièce spécifiée pour le joueur blanc.
 *
 * @param kind Le type de la pièce à convertir.
 * @return AsciiPiece La représentation ASCII de la pièce pour le joueur blanc.
 */
AsciiPiece piece_as_white_ascii(PieceKind kind);

/**
 * @brief Convertit un type de pièce en sa représentation ASCII pour les pièces noires.
 *
 * Cette fonction retourne une structure `AsciiPiece` contenant les lignes ASCII
 * représentant la pièce spécifiée pour le joueur noir.
 *
 * @param kind Le type de la pièce à convertir.
 * @return AsciiPiece La représentation ASCII de la pièce pour le joueur noir.
 */
AsciiPiece piece_as_black_ascii(PieceKind kind);

/**
 * @brief Convertit une chaîne de caractères en un type de pièce.
 *
 * Cette fonction prend une chaîne représentant le type de pièce (par exemple "King", "Queen", etc.)
 * et retourne l'énumération correspondante `PieceKind`.
 *
 * @param piece_kind_str Chaîne représentant le type de la pièce.
 * @return PieceKind L'énumération correspondant au type de la pièce.
 * @warning Termine le programme avec `exit(EXIT_FAILURE)` si la chaîne n'est pas reconnue.
 */
PieceKind piece_kind_from_string(const char* piece_kind_str);

/**
 * @brief Convertit un type de pièce en sa représentation sous forme de chaîne.
 *
 * Cette fonction prend un pointeur vers un type de pièce et retourne une chaîne
 * représentant ce type (par exemple "King", "Queen", etc.).
 *
 * @param kind Pointeur vers le type de la pièce.
 * @return const char* Chaîne représentant le type de la pièce.
 */
const char* stringify_piece(PieceKind kind);


#endif //PIECE_H
