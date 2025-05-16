#include "select.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "game_state.h"
#include "print.h"

// TODO regler le probleme des scanf et parser manuellement le chiffre
StartOption select_option() {
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

    return (StartOption)(option);
}

RoundOption select_round_option() {
    print_text("Choisissez une option:\n\t1. Poser une pièce\n\t2. Abandonner\n\t3. Sauvegarder la partie\n");
    print_text("Votre choix : ");
    // voir https://forums.codeguru.com/showthread.php?329329-When-I-input-chars-in-a-scanf
    // scanf returns the number of items successfully read

    int option = getchar() - '0';

    while (option < 1 || option > 3) {
        if (!isspace(option)) print_text("Choix invalide: ");
        option = getchar() - '0';
    }

    return (RoundOption)(option);
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

uint8_t select_dimension() {
    unsigned int dim;

    print_text("Entrer les dimensions de l'échiquier: (ex: 8 pour 8x8) ");
    if (!scanf("%d", &dim)) dim = 0;

    while (dim < 6 || dim > 12) {
        print_text("La taille doit être comprise entre 6 et 12: ");
        if (!scanf("%d", &dim)) dim = 0;
    }

    return (uint8_t)dim;
}