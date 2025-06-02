#include "capture.h"

static void capture_around_king(const GameState* state, uint8_t x, uint8_t y, Player capturer) {
  const uint8_t dim = state->board.dim;
  const int offsets[8][2] = {
    {-1, 0},  {1, 0},  {0, -1}, {0, 1},
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
  };

  for (int i = 0; i < 8; ++i) {
    const int nx = x + offsets[i][0];
    const int ny = y + offsets[i][1];
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
    const int dx = directions[d][0];
    const int dy = directions[d][1];
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
    const int dx = directions[d][0];
    const int dy = directions[d][1];
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
    const int nx = x + jumps[i][0];
    const int ny = y + jumps[i][1];
    if (nx >= 0 && ny >= 0 && nx < dim && ny < dim) {
      Tile* t = &state->board.tiles[ny][nx];
      if (!t->some || t->value.player == capturer)
        t->captured_by = player_option(capturer);
    }
  }
}

static void capture_pawn_forward(const GameState* state, uint8_t x, uint8_t y, Player capturer) {
  const uint8_t dim = state->board.dim;
  const int dy = (capturer == User) ? -1 : 1;
  const int ny = y + dy;
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

static bool king_captures_tile(uint8_t from_x, uint8_t from_y, uint8_t x, uint8_t y) {
  const int offsets[8][2] = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1},
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
  };
  for (int k = 0; k < 8; ++k) {
    if (from_x + offsets[k][0] == x && from_y + offsets[k][1] == y)
      return true;
  }
  return from_x == x && from_y == y;
}

static bool knight_captures_tile(uint8_t from_x, uint8_t from_y, uint8_t x, uint8_t y) {
  const int jumps[8][2] = {
    {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
    {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
  };
  for (int k = 0; k < 8; ++k) {
    if (from_x + jumps[k][0] == x && from_y + jumps[k][1] == y)
      return true;
  }
  return from_x == x && from_y == y;
}

static bool pawn_captures_tile(Player p, uint8_t from_x, uint8_t from_y, uint8_t x, uint8_t y) {
  const int forward = (p == User) ? -1 : 1;
  return (from_x == x && from_y + forward == y) || (from_x == x && from_y == y);
}

static bool sliding_piece_captures_tile(
  const GameState* state,
  uint8_t from_x, uint8_t from_y,
  uint8_t x, uint8_t y,
  PieceKind kind
) {
  const int directions[8][2] = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1},     // Rook
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}    // Bishop
  };
  const uint8_t dim = state->board.dim;

  for (int d = 0; d < 8; ++d) {
    if ((kind == Rook && d >= 4) || (kind == Bishop && d < 4)) continue;

    int cx = from_x, cy = from_y;
    while (true) {
      cx += directions[d][0];
      cy += directions[d][1];
      if (cx < 0 || cy < 0 || cx >= dim || cy >= dim)
        break;
      if (cx == x && cy == y)
        return true;
      if (state->board.tiles[cy][cx].some)
        break;
    }
  }

  return from_x == x && from_y == y;
}

// Returns true if any piece of the given kind, captured by the current player, can capture the tile at (x, y)
bool is_tile_captured_by_piece_kind(const GameState *state, uint8_t x, uint8_t y, PieceKind kind) {
  const uint8_t dim = state->board.dim;

  for (uint8_t i = 0; i < dim; ++i) {
    for (uint8_t j = 0; j < dim; ++j) {
      Tile t = state->board.tiles[i][j];

      if (!t.some || t.value.kind != kind || !t.captured_by.some || t.captured_by.player != state->is_turn_of)
        continue;

      switch (kind) {
        case King:
          if (king_captures_tile(j, i, x, y)) return true;
          break;
        case Knight:
          if (knight_captures_tile(j, i, x, y)) return true;
          break;
        case Pawn:
          if (pawn_captures_tile(state->is_turn_of, j, i, x, y)) return true;
          break;
        case Rook:
        case Bishop:
        case Queen:
          if (sliding_piece_captures_tile(state, j, i, x, y, kind)) return true;
          break;
        default:
          break;
      }
    }
  }

  return false;
}
