

#include "board.h"

#include <stdio.h>
#include <stdlib.h>

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

void free_board(const Board *board) {
    for (uint8_t i = 0; i < board->dim; ++i)
        free(board->tiles[i]);  // Libère chaque ligne
    free(board->tiles);         // Puis le tableau de pointeurs
}