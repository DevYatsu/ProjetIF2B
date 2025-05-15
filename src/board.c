#include <stdio.h>
#include <stdlib.h>
#include "board.h"

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

    // Initialisation des cases du plateau
    for (uint8_t x = 0; x < dim; ++x) {
        for (uint8_t y = 0; y < dim; ++y) {
            board.tiles[x][y] = init_tile(no_chess_piece(), x, y);
        }
    }

    return board;
}

Tile init_tile(const OptionChessPiece piece, const uint8_t x, const uint8_t y) {
    const Tile tile = {piece, x, y};
    return tile;
}

void free_board(const Board *board) {
    for (uint8_t i = 0; i < board->dim; ++i)
        free(board->tiles[i]);  // Libère chaque ligne
    free(board->tiles);         // Puis le tableau de pointeurs
}