#include "capture.h"

#include <stdio.h>

#include "select.h"

static void capture_around_king(const GameState *state, uint8_t x, uint8_t y, Player capturer) {
    const uint8_t dim = state->board.dim;
    const int offsets[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (int i = 0; i < 8; ++i) {
        const int nx = x + offsets[i][0];
        const int ny = y + offsets[i][1];
        if (nx >= 0 && ny >= 0 && nx < dim && ny < dim) {
            Tile *t = &state->board.tiles[ny][nx];
            if (!t->some)
                t->captured_by = player_option(capturer);
        }
    }
}

static void capture_rook_moves(const GameState *state, uint8_t x, uint8_t y, Player capturer) {
    const uint8_t dim = state->board.dim;
    const int directions[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1} // horizontal and vertical
    };

    for (int d = 0; d < 4; ++d) {
        const int dx = directions[d][0];
        const int dy = directions[d][1];
        uint8_t cx = x + dx;
        uint8_t cy = y + dy;

        while (cx < dim && cy < dim) {
            Tile *t = &state->board.tiles[cy][cx];

            if (t->some) {
                // stop capturing if a piece is encountered
                cx = dim; // force exit condition
                cy = dim;
            } else {
                t->captured_by = player_option(capturer);
                cx += dx;
                cy += dy;
            }
        }
    }
}

static void capture_bishop_moves(const GameState *state, uint8_t x, uint8_t y, Player capturer) {
    const uint8_t dim = state->board.dim;
    const int directions[4][2] = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1} // diagonales
    };

    for (int d = 0; d < 4; ++d) {
        const int dx = directions[d][0];
        const int dy = directions[d][1];
        uint8_t cx = x + dx;
        uint8_t cy = y + dy;

        while (cx < dim && cy < dim) {
            Tile *t = &state->board.tiles[cy][cx];

            if (t->some) {
                // stop capturing if a piece is encountered
                cx = dim; // force exit condition
                cy = dim;
            } else {
                t->captured_by = player_option(capturer);
                cx += dx;
                cy += dy;
            }
        }
    }
}

static void capture_knight_moves(const GameState *state, uint8_t x, uint8_t y, Player capturer) {
    const uint8_t dim = state->board.dim;
    const int jumps[8][2] = {
        {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
        {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
    };

    for (int i = 0; i < 8; ++i) {
        const int nx = x + jumps[i][0];
        const int ny = y + jumps[i][1];
        if (nx >= 0 && ny >= 0 && nx < dim && ny < dim) {
            Tile *t = &state->board.tiles[ny][nx];
            if (!t->some)
                t->captured_by = player_option(capturer);
        }
    }
}

static void capture_pawn_forward(const GameState *state, uint8_t x, uint8_t y, Player capturer) {
    const uint8_t dim = state->board.dim;
    const int dy = (capturer == User) ? -1 : 1;
    const int ny = y + dy;
    if (ny >= 0 && ny < dim) {
        Tile *t = &state->board.tiles[ny][x];
        if (!t->some || t->value.player == capturer)
            t->captured_by = player_option(capturer);
    }
}

void apply_conquest_capture(const GameState *state, uint8_t x, uint8_t y, ChessPiece piece, Player capturer) {
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

static bool rook_captures_tile(const GameState *state, uint8_t from_x, uint8_t from_y, uint8_t x, uint8_t y) {
    const int directions[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    const uint8_t dim = state->board.dim;

    for (int d = 0; d < 4; ++d) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        int cx = from_x + dx;
        int cy = from_y + dy;

        while (cx >= 0 && cy >= 0 && cx < dim && cy < dim) {
            if (cx == x && cy == y)
                return true;
            if (state->board.tiles[cy][cx].some)
                break;

            cx += dx;
            cy += dy;
        }

        if (cx == x && cy == y)
            return true;
    }

    return from_x == x && from_y == y;
}

static bool bishop_captures_tile(const GameState *state, uint8_t from_x, uint8_t from_y, uint8_t x, uint8_t y) {
    const int directions[4][2] = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    const uint8_t dim = state->board.dim;

    for (int d = 0; d < 4; ++d) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        int cx = from_x + dx;
        int cy = from_y + dy;

        while (cx >= 0 && cy >= 0 && cx < dim && cy < dim) {
            if (cx == x && cy == y)
                return true;
            if (state->board.tiles[cy][cx].some)
                break;

            cx += dx;
            cy += dy;
        }

        if (cx == x && cy == y)
            return true;
    }

    return from_x == x && from_y == y;
}

// Renvoie true si la pièce peut capturer le Tile (x,y)
bool is_tile_captured_by_piece_kind(const GameState *state, uint8_t x, uint8_t y, PieceKind kind) {
    const uint8_t dim = state->board.dim;

    for (uint8_t i = 0; i < dim; ++i) {
        for (uint8_t j = 0; j < dim; ++j) {
            const Tile t = state->board.tiles[i][j];

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
                    if (rook_captures_tile(state, j, i, x, y)) return true;
                    break;
                case Bishop:
                    if (bishop_captures_tile(state, j, i, x, y)) return true;
                    break;
                case Queen:
                    if (bishop_captures_tile(state, j, i, x, y) || rook_captures_tile(state, j, i, x, y)) return true;
                    break;
                default:
                    break;
            }
        }
    }

    return false;
}

void play_conquest_turn(const GameState *game_state) {
    Tile tile = select_valid_tile(game_state);
    const TargetPosition pos = select_valid_target_position(game_state);

    tile.captured_by = player_option(game_state->is_turn_of);
    game_state->board.tiles[pos.y][pos.x] = tile;
    apply_conquest_capture(game_state, pos.x, pos.y, tile.value, game_state->is_turn_of);
}

void play_connect_turn(GameState *game_state) {
    Tile tile = select_valid_tile_for_connect(game_state);
    const TargetPosition pos = select_valid_target_position_for_connect(game_state, &tile);

    tile.captured_by = player_option(game_state->is_turn_of);
    game_state->board.tiles[pos.y][pos.x] = tile;
    apply_conquest_capture(game_state, pos.x, pos.y, tile.value, game_state->is_turn_of);

    // La partie se termine dès qu'un joueur pose son roi
    if (tile.value.kind == King) {
        printf("Le roi a été placé par le joueur %s. La partie est terminée !\n",
               stringify_player(game_state->is_turn_of));
        set_all_to_zero(&game_state->piece_counter_1);
        set_all_to_zero(&game_state->piece_counter_2);
    }
}
