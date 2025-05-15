
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

typedef struct {
    bool some;
    ChessPiece value;
} OptionChessPiece;

typedef struct {
    const char* line1;
    const char* line2;
    const char* line3;
} AsciiPiece;

OptionChessPiece no_chess_piece();
OptionChessPiece some_chess_piece(int v);

OptionChessPiece deserialize_piece(const char* piece_str, const char* player_str, bool from_user_input);

PieceKind piece_kind_from_string(const char* piece_kind_str);

AsciiPiece piece_as_white_ascii(PieceKind kind);

AsciiPiece piece_as_black_ascii(PieceKind kind);

#endif //PIECE_H
