#include "player.h"
#include <stdlib.h>

char* stringify_player(const Player player) {
    return (player == User ? "User" : "AI");
}

Player random_player() {
    return (rand() % 2 == 0 ? User : AI);
}

