#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "piece.h"

AsciiPiece piece_as_white_ascii(const PieceKind kind) {
    const char* line_bottom = "/___\\";
    switch (kind) {
        case King: return (AsciiPiece){ .line1 = " \\+/ ", .line2 = " ) ( ", .line3 = line_bottom };
        case Queen: return (AsciiPiece){ .line1 = " \\^/ ", .line2 = " ) ( ", .line3 = line_bottom };
        case Rook: return (AsciiPiece){ .line1 = " (V) ", .line2 = " ) ( ", .line3 = line_bottom };
        case Bishop: return (AsciiPiece){ .line1 = " (\"\\ ", .line2 = "  (#)  ", .line3 = line_bottom };
        case Knight: return (AsciiPiece){ .line1 = " [-] ", .line2 = " (#/)  ", .line3 = line_bottom };
        case Pawn: return (AsciiPiece){ .line1 = "  _  ", .line2 = " |##|  ", .line3 = line_bottom };
        default: return (AsciiPiece){ .line1 = "  ?  ", .line2 = "  ???  ", .line3 = "?????" };
    }
}

AsciiPiece piece_as_black_ascii(const PieceKind kind) {
    const char* line_bottom = "/###\\";
    switch (kind) {
        case King: return (AsciiPiece){ .line1 = " \\+/ ", .line2 = " )#( ", .line3 = line_bottom };
        case Queen: return (AsciiPiece){ .line1 = " \\^/ ", .line2 = " )#( ", .line3 = line_bottom };
        case Rook: return (AsciiPiece){ .line1 = " (V) ", .line2 = " )#( ", .line3 = line_bottom };
        case Bishop: return (AsciiPiece){ .line1 = " (\"\\ ", .line2 = " )#' ", .line3 = line_bottom };
        case Knight: return (AsciiPiece){ .line1 = " [-] ", .line2 = " |#| ", .line3 = line_bottom };
        case Pawn: return (AsciiPiece){ .line1 = "  _  ", .line2 = " (#) ", .line3 = line_bottom };
        default: return (AsciiPiece){ .line1 = "  ?  ", .line2 = " ?B? ", .line3 = "?????" };
    }
}

OptionChessPiece deserialize_piece(const char* piece_str, const char* player_str, const bool from_user_input) {
    ChessPiece piece;
    Player player;

    if (strcmp(piece_str, "King") == 0 || strcmp(piece_str, "Roi") == 0) {
        piece.kind = King;
    } else if (strcmp(piece_str, "Queen") == 0 || strcmp(piece_str, "Reine") == 0) {
        piece.kind = Queen;
    } else if (strcmp(piece_str, "Rook") == 0 || strcmp(piece_str, "Tour") == 0) {
        piece.kind = Rook;
    } else if (strcmp(piece_str, "Bishop") == 0 || strcmp(piece_str, "Fou") == 0) {
        piece.kind = Bishop;
    } else if (strcmp(piece_str, "Knight") == 0 || strcmp(piece_str, "Cavalier") == 0) {
        piece.kind = Knight;
    } else if (strcmp(piece_str, "Pawn") == 0 || strcmp(piece_str, "Pion") == 0) {
        piece.kind = Pawn;
    } else if (strcmp(piece_str, "None") == 0 || from_user_input) {
        return (OptionChessPiece){.some = false };
    } else {
        printf("Invalid piece string: %s\n", piece_str);
        exit(EXIT_FAILURE);
    }

    if (strcmp(player_str, "User") == 0) {
        player = User;
    } else if (strcmp(player_str, "AI") == 0) {
        player = AI;
    } else {
        printf("Invalid player string: %s\n", player_str);
        exit(EXIT_FAILURE);
    }

    piece.player = player;
    return (OptionChessPiece){.some = true, .value = piece};
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

OptionChessPiece no_chess_piece()   { return (OptionChessPiece){ .some = false }; }
OptionChessPiece some_chess_piece(const int v) { return (OptionChessPiece){ .some = true, .value = v }; }
