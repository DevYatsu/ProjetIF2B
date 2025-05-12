#include "player.h"
#include <stdlib.h>

Player random_player() {
    return (rand() % 2 == 0 ? User : AI);
}
