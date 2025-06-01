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

bool is_tile_captured_by_kind(const GameState *state, uint8_t x, uint8_t y,
                              PieceKind required_kind) {
  for (uint8_t i = 0; i < state->board.dim; ++i) {
    for (uint8_t j = 0; j < state->board.dim; ++j) {
      Tile t = state->board.tiles[i][j];
      if (t.some && t.value.kind == required_kind && t.captured_by.some) {
        if (t.captured_by.player == state->is_turn_of) {
          return true;
        }
      }
    }
  }
  return false;
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

void apply_conquest_capture(const GameState *state, const uint8_t x,
                            const uint8_t y, const ChessPiece piece,
                            const Player capturer) {
  const uint8_t dim = state->board.dim;
  Tile *center = &state->board.tiles[y][x];
  // la case sur laquelle est la pièce est capturée
  center->captured_by = player_option(capturer);

  switch (piece.kind) {
  case King: {
    const int offsets[8][2] = {{-1, 0},  {1, 0},  {0, -1}, {0, 1},
                               {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (int i = 0; i < 8; i++) {
      const int nx = x + offsets[i][0];
      const int ny = y + offsets[i][1];
      if (nx >= 0 && ny >= 0 && nx < dim && ny < dim) {
        Tile *t = &state->board.tiles[ny][nx];
        if (!t->some || t->value.player == capturer)
          t->captured_by = player_option(capturer);
      }
    }
    break;
  }

  case Queen:
  case Rook:
  case Bishop: {
    const int directions[8][2] = {
        {-1, 0},  {1, 0},  {0, -1}, {0, 1}, // rook
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}  // bishop
    };

    for (int d = 0; d < 8; d++) {
      // on skip les diagonaless pour la tour
      if (piece.kind == Rook && d >= 4)
        continue;
      // on skip les lignes pour le fou
      if (piece.kind == Bishop && d < 4)
        continue;

      const int dx = directions[d][0];
      const int dy = directions[d][1];
      uint8_t cx = x, cy = y;

      while (true) {
        cx += dx;
        cy += dy;

        if (cx < 0 || cy < 0 || cx >= dim || cy >= dim)
          break;

        Tile *t = &state->board.tiles[cy][cx];
        if (t->some && t->value.player != capturer)
          break;

        t->captured_by = player_option(capturer);

        if (t->some)
          break;
      }
    }
    break;
  }

  case Knight: {
    const int jumps[8][2] = {{2, 1},   {1, 2},   {-1, 2}, {-2, 1},
                             {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
    for (int i = 0; i < 8; i++) {
      int nx = x + jumps[i][0];
      int ny = y + jumps[i][1];
      if (nx >= 0 && ny >= 0 && nx < dim && ny < dim) {
        Tile *t = &state->board.tiles[ny][nx];
        if (!t->some || t->value.player == capturer)
          t->captured_by = player_option(capturer);
      }
    }
    break;
  }

  case Pawn: {
    const int forward = (capturer == User) ? -1 : 1;
    const int dirs[2][2] = {{forward, -1}, {forward, 1}};
    for (int i = 0; i < 2; i++) {
      int nx = x + dirs[i][1];
      int ny = y + dirs[i][0];

      if (nx >= 0 && ny >= 0 && nx < dim && ny < dim) {
        Tile *t = &state->board.tiles[ny][nx];
        if (!t->some || t->value.player == capturer) {
          t->captured_by = player_option(capturer);
        }
      }
    }
    break;
  }
  default: {
    // unreachable
    break;
  }
  }
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
