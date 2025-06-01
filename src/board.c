#include <stdio.h>
#include <stdlib.h>
#include "board.h"

#include <ctype.h>
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

Tile empty_tile()   { return (Tile){ .some = false, .captured_by = no_player() }; }
Tile tile_with_piece(const ChessPiece piece) { return (Tile){ .some = true, .value = piece, .captured_by = no_player() }; }

Tile deserialize_tile(const char* piece_str, const char* player_str, const char* captured_by_str, const bool from_user_input) {
    PieceKind kind;
    PlayerOption player_opt;
    player_opt.some = true;

    if (strcmp(captured_by_str, "User") == 0) {
         player_opt.player = User;
    } else if (strcmp(captured_by_str, "Opponent") == 0) {
        player_opt.player = Opponent;
    } else {
        player_opt = no_player();
    }

    // Normaliser la chaîne de caractères pour la pièce
    // ex: "King" -> "King", "queen" -> "Queen", "ROi" -> "Roi"
    char piece[16];
    piece[0] = toupper(piece_str[0]);
    for (size_t i = 1; i < strlen(piece_str) && i < sizeof(piece) - 1; ++i) {
        piece[i] = tolower(piece_str[i]);
    }
    piece[strlen(piece_str)] = '\0';

    if (strcmp(piece, "King") == 0 || strcmp(piece, "Roi") == 0) {
        kind = King;
    } else if (strcmp(piece, "Queen") == 0 || strcmp(piece, "Reine") == 0 || strcmp(piece, "Dame") == 0) {
        kind = Queen;
    } else if (strcmp(piece, "Rook") == 0 || strcmp(piece, "Tour") == 0) {
        kind = Rook;
    } else if (strcmp(piece, "Bishop") == 0 || strcmp(piece, "Fou") == 0) {
        kind = Bishop;
    } else if (strcmp(piece, "Knight") == 0 || strcmp(piece, "Cavalier") == 0) {
        kind = Knight;
    } else if (strcmp(piece, "Pawn") == 0 || strcmp(piece, "Pion") == 0) {
        kind = Pawn;
    } else if (strcmp(piece, "None") == 0 || from_user_input) {
        return (Tile){.some = false, .captured_by = player_opt };
    } else {
        printf("Invalid piece string: %s\n", piece);
        exit(EXIT_FAILURE);
    }

    Player piece_player;

    if (strcmp(player_str, "User") == 0) {
        piece_player = User;
    } else if (strcmp(player_str, "Opponent") == 0) {
        piece_player = Opponent;
    } else {
        printf("Invalid player string: %s\n", player_str);
        exit(EXIT_FAILURE);
    }

    const ChessPiece value = {.kind = kind, .player = piece_player};

    return (Tile){.some = true, .value = value, .captured_by = player_opt};
}
