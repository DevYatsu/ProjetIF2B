#include "select.h"
#include <stdio.h>
#include <stdlib.h>
#include "game_state.h"
#include "print.h"

Option select_option() {
    int option;

    print_text("Choisissez une option:\n\t1. Démarrer une partie\n\t2. Reprendre une partie\n\t3. Quitter\n");
    print_text("Votre choix : ");
    // voir https://forums.codeguru.com/showthread.php?329329-When-I-input-chars-in-a-scanf
    // scanf returns the number of items successfully read
    if (!scanf("%d", &option)) option = 0;

    while (option < 1 || option > 3) {
        print_text("Choix invalide: ");
        if (!scanf("%d", &option)) option = 0;
    }

    return (Option)(option);
}

GameMode select_mode() {
    unsigned int mode;

    print_text("Choisissez un mode:\n");
    print_text("\t1. Conquête\n");
    print_text("\t2. Connecte\n");

    print_text("Votre choix : ");
    if (!scanf("%d", &mode)) mode = 0;

    while (mode < 1 || mode > 2) {
        print_text("Choix invalide : ");
        if (!scanf("%d", &mode)) mode = 0;
    }

    return (GameMode)(mode);
}

__uint8_t select_dimension() {
    unsigned int dim;

    print_text("Entrer les dimensions de l'échiquier: (ex: 8 pour 8x8) ");
    if (!scanf("%d", &dim)) dim = 0;

    while (dim < 6 || dim > 12) {
        print_text("La taille doit être comprise entre 6 et 12: ");
        if (!scanf("%d", &dim)) dim = 0;
    }

    return (__uint8_t)dim;
}