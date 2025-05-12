#include <stdio.h>
#include "game_state.h"

__uint8_t select_option() {
    int option;

    printf("Choisissez une option:\n");
    printf("\t1. Démarrer une partie\n");
    printf("\t2. Reprendre une partie\n");
    printf("\t3. Quitter\n");

    printf("Votre choix : ");
    // voir https://forums.codeguru.com/showthread.php?329329-When-I-input-chars-in-a-scanf
    // scanf returns the number of items successfully read
    if (!scanf("%u", &option)) option = 0;

    while (option < 1 || option > 3) {
        printf("Choix invalide: ");
        if (!scanf("%u", &option)) option = 0;
    }

    return (__uint8_t)option;
}

GameMode select_mode() {
    unsigned int mode;

    printf("Choisissez un mode:\n");
    printf("\t1. Conquête\n");
    printf("\t2. Connecte\n");

    printf("Votre choix : ");
    if (!scanf("%u", &mode)) mode = 0;

    while (mode < 1 || mode > 2) {
        printf("Choix invalide : ");
        if (!scanf("%u", &mode)) mode = 0;
    }

    return mode == 1 ? Conquest : Connect;
}

__uint8_t select_dimension() {
    unsigned int dim;

    printf("Entrer les dimensions de l'échiquier: (ex: 8 pour 8x8) ");
    if (!scanf("%u", &dim)) dim = 0;

    while (dim < 6 || dim > 12) {
        printf("La taille doit être comprise entre 6 et 12: ");
        if (!scanf("%u", &dim)) dim = 0;
    }

    return (__uint8_t)dim;
}