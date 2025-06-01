#include "player.h"
#include <stdlib.h>
#include <string.h>

PlayerOption no_player() {
    return (PlayerOption){ .some = false };
}

PlayerOption player_option(const Player player) {
    return (PlayerOption){ .some = true, .player = player };
}


char* stringify_player(const Player player) {
    return (player == User ? "User" : "Opponent");
}

char* stringify_player_option(const PlayerOption player_opt) {
    return (player_opt.some ? stringify_player(player_opt.player) : "_");
}

PlayerOption parse_player_option(const char* str) {
    if (strcmp(str, "User") == 0) return (PlayerOption){ true, User };
    if (strcmp(str, "Opponent") == 0) return (PlayerOption){ true, Opponent };
    return (PlayerOption){ false };
}

Player random_player() {
    return (rand() % 2 == 0 ? User : Opponent);
}

