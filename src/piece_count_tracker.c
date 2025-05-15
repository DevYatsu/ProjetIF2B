#include "piece_count_tracker.h"

PieceCountTracker init_piece_counter() {
    const PieceCountTracker counter = {.pawns = 8,
                                  .knights = 2,
                                  .bishops = 2,
                                  .rooks = 2,
                                  .queen = 1,
                                  .king = 1};
    return counter;
}

bool add_piece(PieceCountTracker* counter, const PieceKind piece) {
    switch (piece) {
        case Pawn:
            if (counter->pawns > 0) {
                counter->pawns--;
                return true;
            }
            break;
        case Knight:
            if (counter->knights > 0) {
                counter->knights--;
                return true;
            }
            break;
        case Bishop:
            if (counter->bishops > 0) {
                counter->bishops--;
                return true;
            }
            break;
        case Rook:
            if (counter->rooks > 0) {
                counter->rooks--;
                return true;
            }
            break;
        case Queen:
            if (counter->queen > 0) {
                counter->queen--;
                return true;
            }
            break;
        case King:
            if (counter->king > 0) {
                counter->king--;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}



