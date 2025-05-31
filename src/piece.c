#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "piece.h"

AsciiPiece piece_as_white_ascii(const PieceKind kind) {
    // cf https://ascii.co.uk/art/chess

    const char* line_bottom = "/___\\";
    switch (kind) {
        case King: return (AsciiPiece){ .line1 = " \\+/ ", .line2 = " ) ( ", .line3 = line_bottom };
        case Queen: return (AsciiPiece){ .line1 = " \\^/ ", .line2 = " ) ( ", .line3 = line_bottom };
        case Rook: return (AsciiPiece){ .line1 = " (V) ", .line2 = " ) ( ", .line3 = line_bottom };
        case Bishop: return (AsciiPiece){ .line1 = " (\"\\ ", .line2 = " ) ' ", .line3 = line_bottom };
        case Knight: return (AsciiPiece){ .line1 = " [-] ", .line2 = " | | ", .line3 = line_bottom };
        case Pawn: return (AsciiPiece){ .line1 = "  _  ", .line2 = " |#| ", .line3 = line_bottom };
        default: // unreachable
            return (AsciiPiece){ .line1 = "  ?  ", .line2 = "  ???  ", .line3 = "?????" };
    }
}

AsciiPiece piece_as_black_ascii(const PieceKind kind) {
    // cf https://ascii.co.uk/art/chess

    const char* line_bottom = "/###\\";
    switch (kind) {
        case King: return (AsciiPiece){ .line1 = " \\+/ ", .line2 = " )#( ", .line3 = line_bottom };
        case Queen: return (AsciiPiece){ .line1 = " \\^/ ", .line2 = " )#( ", .line3 = line_bottom };
        case Rook: return (AsciiPiece){ .line1 = " (V) ", .line2 = " )#( ", .line3 = line_bottom };
        case Bishop: return (AsciiPiece){ .line1 = " (\"\\ ", .line2 = " )#' ", .line3 = line_bottom };
        case Knight: return (AsciiPiece){ .line1 = " [-] ", .line2 = " |#| ", .line3 = line_bottom };
        case Pawn: return (AsciiPiece){ .line1 = "  _  ", .line2 = " (#) ", .line3 = line_bottom };
        default: // unreachable
            return (AsciiPiece){ .line1 = "  ?  ", .line2 = " ?B? ", .line3 = "?????" };
    }
}

PieceKind piece_kind_from_string(const char* piece_kind_str) {
    PieceKind piece_kind;

    if (strcmp(piece_kind_str, "King") == 0) {
        piece_kind = King;
    } else if (strcmp(piece_kind_str, "Queen") == 0) {
        piece_kind = Queen;
    } else if (strcmp(piece_kind_str, "Rook") == 0) {
        piece_kind = Rook;
    } else if (strcmp(piece_kind_str, "Bishop") == 0) {
        piece_kind = Bishop;
    } else if (strcmp(piece_kind_str, "Knight") == 0) {
        piece_kind = Knight;
    } else if (strcmp(piece_kind_str, "Pawn") == 0) {
        piece_kind = Pawn;
    } else {
        perror("Invalid piece kind string");
        exit(EXIT_FAILURE);
    }

    return piece_kind;
}

const char* stringify_piece(const PieceKind kind) {
    switch (kind) {
        case King:   return "King";
        case Queen:  return "Queen";
        case Rook:   return "Rook";
        case Bishop: return "Bishop";
        case Knight: return "Knight";
        case Pawn:   return "Pawn";
        default:
            return "Unknown";
    }
}