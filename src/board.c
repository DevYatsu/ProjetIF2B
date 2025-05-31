#include <stdio.h>
#include <stdlib.h>
#include "board.h"
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

    // Initialisation des cases du plateau
    for (uint8_t x = 0; x < dim; ++x) {
        for (uint8_t y = 0; y < dim; ++y) {
            board.tiles[x][y] = empty_tile();
        }
    }

    return board;
}

void free_board(const Board *board) {
    for (uint8_t i = 0; i < board->dim; ++i)
        free(board->tiles[i]);  // Libère chaque ligne
    free(board->tiles);         // Puis le tableau de pointeurs
}

Tile empty_tile()   { return (Tile){ .some = false }; }
Tile tile_with_piece(const ChessPiece piece) { return (Tile){ .some = true, .value = piece }; }

Tile deserialize_piece(const char* piece_str, const char* player_str, const bool from_user_input) {
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
        return (Tile){.some = false };
    } else {
        printf("Invalid piece string: %s\n", piece_str);
        exit(EXIT_FAILURE);
    }

    if (strcmp(player_str, "User") == 0) {
        player = User;
    } else if (strcmp(player_str, "Opponent") == 0) {
        player = Opponent;
    } else {
        printf("Invalid player string: %s\n", player_str);
        exit(EXIT_FAILURE);
    }

    piece.player = player;
    return (Tile){.some = true, .value = piece};
}
