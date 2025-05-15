#include "player.h"
#include <stdlib.h>

char* stringify_player(const Player player) {
    return (player == User ? "User" : "Opponent");
}

Player random_player() {
    return (rand() % 2 == 0 ? User : Opponent);
}

