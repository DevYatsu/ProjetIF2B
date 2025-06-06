#include "game_state.h"
#include "board.h"
#include <stdio.h>

GameState init_game_state(const GameMode mode, const uint8_t dim) {
  GameState state;
  state.mode = mode;
  state.board = init_board(dim);
  state.is_turn_of = state.is_white = random_player();
  state.piece_counter_1 = state.piece_counter_2 = init_piece_counter();
  return state;
}

void toggle_user_turn(GameState *state) {
  if (state->is_turn_of == User) {
    state->is_turn_of = Opponent;
  } else {
    state->is_turn_of = User;
  }
}

char *get_user_turn_name(const GameState *state) {
  return stringify_player(state->is_turn_of);
}
const PieceCountTracker *get_user_turn_count_tracker(const GameState *state) {
  return (state->is_turn_of == User) ? &state->piece_counter_1
                                     : &state->piece_counter_2;
}

uint8_t get_captured_count_of(const GameState *state, const Player player) {
  uint8_t count = 0;
  for (uint8_t i = 0; i < state->board.dim; i++) {
    for (uint8_t j = 0; j < state->board.dim; j++) {
      const Tile tile = state->board.tiles[i][j];
      if (tile.some && tile.captured_by.some &&
          tile.captured_by.player == player) {
        count++;
      }
    }
  }

  return count;
}

/**
 * @brief Affiche dans la console un résumé de l'état de jeu.
 *
 * Inclut les informations sur le mode de jeu, le joueur actif,
 * et le contenu du plateau avec les identifiants des pièces.
 *
 * @param state Pointeur vers l'état de jeu à inspecter.
 */
void debug_game_state(const GameState *state) {
  printf("GameState:\n");
  printf("  Mode: %s\n", state->mode == Conquest ? "Conquest" : "Connect");
  printf("  Player: %s\n", stringify_player(state->is_turn_of));
  printf("  Board:\n");

  if (state->board.tiles == NULL) {
    printf("    L'échiquier est vide.\n");
    return;
  }

  printf("  \tdim=%d\n", state->board.dim);

  for (uint8_t i = 0; i < state->board.dim; i++) {
    for (uint8_t j = 0; j < state->board.dim; j++) {
      const Tile piece = state->board.tiles[i][j];
      printf(" %d ", piece.some ? piece.value.kind : -1);
    }
    printf("\n");
  }
}

void print_board(const GameState *state) {
  const uint8_t dim = state->board.dim;

  printf("\n");
  printf("      ");
  for (uint8_t j = 0; j < dim; j++) {
    printf("  %c  ", 'A' + j);
    printf(" ");
  }
  printf("\n");
  printf("\n");
  for (uint8_t i = 0; i < dim; i++) {
    for (int line = 0; line < 3; line++) {
      if (line == 1) {
        // Affichage du numéro de ligne au centre de la pièce
        printf(" %2d  ", dim - i); // étiquette de la ligne (comme aux échecs)
      } else {
        printf("     ");
      }

      for (uint8_t j = 0; j < dim; j++) {
        const Tile piece = state->board.tiles[i][j];

        AsciiPiece p = {"·····", "·····", "·····"};

        if (piece.some) {
          if (piece.value.player == User) {
            p = piece_as_white_ascii(piece.value.kind);
          } else {
            p = piece_as_black_ascii(piece.value.kind);
          }
        }

        switch (line) {
        case 0:
          printf(" %s", p.line1);
          break;
        case 1:
          printf(" %s", p.line2);
          break;
        case 2:
          printf(" %s", p.line3);
          break;
        default:
            // unreachable
            ;
        }
      }

      if (line == 1) {
        printf("   %2d", dim - i); // Répéter le numéro de ligne à droite
      }

      printf("\n");
    }
    printf("\n"); // separate rows visually
  }

  // Affichage des lettres en bas
  printf("      ");
  for (uint8_t j = 0; j < dim; j++) {
    printf("  %c  ", 'A' + j);
    printf(" ");
  }
  printf("\n");
  printf("\n");
}

void free_game_state(const GameState *state) { free_board(&state->board); }
