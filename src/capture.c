#include "capture.h"

static void capture_around_king(const GameState* state, uint8_t x, uint8_t y, Player capturer) {
  const uint8_t dim = state->board.dim;
  const int offsets[8][2] = {
    {-1, 0},  {1, 0},  {0, -1}, {0, 1},
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
  };

  for (int i = 0; i < 8; ++i) {
    int nx = x + offsets[i][0];
    int ny = y + offsets[i][1];
    if (nx >= 0 && ny >= 0 && nx < dim && ny < dim) {
      Tile* t = &state->board.tiles[ny][nx];
      if (!t->some || t->value.player == capturer)
        t->captured_by = player_option(capturer);
    }
  }
}

static void capture_rook_moves(const GameState* state, uint8_t x, uint8_t y, Player capturer) {
  const uint8_t dim = state->board.dim;
  const int directions[4][2] = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1} // horizontal and vertical
  };

  for (int d = 0; d < 4; ++d) {
    int dx = directions[d][0];
    int dy = directions[d][1];
    uint8_t cx = x, cy = y;

    while (true) {
      cx += dx;
      cy += dy;
      if (cx >= dim || cy >= dim)
        break;

      Tile* t = &state->board.tiles[cy][cx];
      if (t->some && t->value.player != capturer)
        break;

      t->captured_by = player_option(capturer);
      if (t->some)
        break;
    }
  }
}

static void capture_bishop_moves(const GameState* state, uint8_t x, uint8_t y, Player capturer) {
  const uint8_t dim = state->board.dim;
  const int directions[4][2] = {
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1} // diagonals
  };

  for (int d = 0; d < 4; ++d) {
    int dx = directions[d][0];
    int dy = directions[d][1];
    uint8_t cx = x, cy = y;

    while (true) {
      cx += dx;
      cy += dy;
      if (cx >= dim || cy >= dim)
        break;

      Tile* t = &state->board.tiles[cy][cx];
      if (t->some && t->value.player != capturer)
        break;

      t->captured_by = player_option(capturer);
      if (t->some)
        break;
    }
  }
}

static void capture_knight_moves(const GameState* state, uint8_t x, uint8_t y, Player capturer) {
  const uint8_t dim = state->board.dim;
  const int jumps[8][2] = {
    {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
    {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
  };

  for (int i = 0; i < 8; ++i) {
    int nx = x + jumps[i][0];
    int ny = y + jumps[i][1];
    if (nx >= 0 && ny >= 0 && nx < dim && ny < dim) {
      Tile* t = &state->board.tiles[ny][nx];
      if (!t->some || t->value.player == capturer)
        t->captured_by = player_option(capturer);
    }
  }
}

static void capture_pawn_forward(const GameState* state, uint8_t x, uint8_t y, Player capturer) {
  const uint8_t dim = state->board.dim;
  int dy = (capturer == User) ? -1 : 1;
  int ny = y + dy;
  if (ny >= 0 && ny < dim) {
    Tile* t = &state->board.tiles[ny][x];
    if (!t->some || t->value.player == capturer)
      t->captured_by = player_option(capturer);
  }
}

void apply_conquest_capture(const GameState* state, uint8_t x, uint8_t y, ChessPiece piece, Player capturer) {
  state->board.tiles[y][x].captured_by = player_option(capturer);

  switch (piece.kind) {
    case King:
      capture_around_king(state, x, y, capturer);
      break;
    case Queen:
      capture_rook_moves(state, x, y, capturer);
      capture_bishop_moves(state, x, y, capturer);
      break;
    case Rook:
      capture_rook_moves(state, x, y, capturer);
      break;
    case Bishop:
      capture_bishop_moves(state, x, y, capturer);
      break;
    case Knight:
      capture_knight_moves(state, x, y, capturer);
      break;
    case Pawn:
      capture_pawn_forward(state, x, y, capturer);
      break;
    default:
      break;
  }
}

// Returns true if any piece of the given kind, captured by the current player, can capture the tile at (x, y)
bool is_tile_captured_by_piece_kind(const GameState *state, uint8_t x, uint8_t y, PieceKind kind) {
  for (uint8_t i = 0; i < state->board.dim; ++i) {
    for (uint8_t j = 0; j < state->board.dim; ++j) {
      Tile t = state->board.tiles[i][j];
      if (t.some && t.value.kind == kind && t.captured_by.some && t.captured_by.player == state->is_turn_of) {
        // Simulate capture from (j, i) to (x, y)
        // Use the same logic as in apply_conquest_capture, but only check if (x, y) would be affected
        switch (kind) {
          case King: {
            const int offsets[8][2] = {{-1, 0},  {1, 0},  {0, -1}, {0, 1},
                                       {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
            for (int k = 0; k < 8; k++) {
              int nx = j + offsets[k][0];
              int ny = i + offsets[k][1];
              if (nx == x && ny == y)
                return true;
            }
            if (j == x && i == y) return true;
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
              if (kind == Rook && d >= 4) continue;
              if (kind == Bishop && d < 4) continue;
              int cx = j, cy = i;
              while (1) {
                cx += directions[d][0];
                cy += directions[d][1];
                if (cx < 0 || cy < 0 || cx >= state->board.dim || cy >= state->board.dim)
                  break;
                if (cx == x && cy == y)
                  return true;
                Tile *tt = &state->board.tiles[cy][cx];
                if (tt->some) break;
              }
            }
            if (j == x && i == y) return true;
            break;
          }
          case Knight: {
            const int jumps[8][2] = {{2, 1},   {1, 2},   {-1, 2}, {-2, 1},
                                     {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
            for (int k = 0; k < 8; k++) {
              int nx = j + jumps[k][0];
              int ny = i + jumps[k][1];
              if (nx == x && ny == y)
                return true;
            }
            if (j == x && i == y) return true;
            break;
          }
          case Pawn: {
            const int forward = (state->is_turn_of == User) ? -1 : 1;
            const int dirs[1][2] = {{forward, 0}};
            const int nx = j + dirs[0][1];
            const int ny = i + dirs[0][0];
            if (nx == x && ny == y)
              return true;

            if (j == x && i == y) return true;
            break;
          }
          default:
            break;
        }
      }
    }
  }
  return false;
}
