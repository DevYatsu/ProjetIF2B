#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "piece.h"

ChessPiece deserialize_piece(const char* piece_str) {
    if (strcmp(piece_str, "King") == 0) {
        return King;
    } else if (strcmp(piece_str, "Queen") == 0) {
        return Queen;
    } else if (strcmp(piece_str, "Rook") == 0) {
        return Rook;
    } else if (strcmp(piece_str, "Bishop") == 0) {
        return Bishop;
    } else if (strcmp(piece_str, "Knight") == 0) {
        return Knight;
    } else if (strcmp(piece_str, "Pawn") == 0) {
        return Pawn;
    } else if (strcmp(piece_str, "None") == 0) {
        return None;
    } else {
        printf("Invalid piece string: %s\n", piece_str);
        exit(EXIT_FAILURE);
    }
}

ChessPiece piece_from_string(const char* piece_str) {
    if (strcmp(piece_str, "King") == 0) {
        return King;
    } else if (strcmp(piece_str, "Queen") == 0) {
        return Queen;
    } else if (strcmp(piece_str, "Rook") == 0) {
        return Rook;
    } else if (strcmp(piece_str, "Bishop") == 0) {
        return Bishop;
    } else if (strcmp(piece_str, "Knight") == 0) {
        return Knight;
    } else if (strcmp(piece_str, "Pawn") == 0) {
        return Pawn;
    } else {
        return None;
    }
}
