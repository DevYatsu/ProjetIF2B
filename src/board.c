

#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Board init_board(const uint8_t dim) {
    Board board;
    board.dim = dim;

    // Allocation du tableau de lignes
    board.tiles = malloc(dim * sizeof(Tile *));
    if (board.tiles == NULL) {
        perror("Failed to allocate memory for board rows");
        exit(EXIT_FAILURE);
    }

    // Allocation de chaque ligne (colonnes)
    for (uint8_t i = 0; i < dim; ++i) {
        board.tiles[i] = malloc(dim * sizeof(Tile));
        if (board.tiles[i] == NULL) {
            perror("Failed to allocate memory for board columns");
            // Libère ce qui a déjà été alloué en cas d'erreur
            for (uint8_t j = 0; j < i; ++j)
                free(board.tiles[j]);
            free(board.tiles);
            exit(EXIT_FAILURE);
        }
    }

    return board;
}

Tile init_tile(const ChessPiece piece, const uint8_t x, const uint8_t y) {
    const Tile tile = {piece, x, y};
    return tile;
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
    } else if (strcmp(piece_str, "None") == 0) {
        return None;
    } else {
        printf("Invalid piece string: %s\n", piece_str);
        exit(EXIT_FAILURE);
    }
}

void free_board(const Board *board) {
    for (uint8_t i = 0; i < board->dim; ++i)
        free(board->tiles[i]);  // Libère chaque ligne
    free(board->tiles);         // Puis le tableau de pointeurs
}