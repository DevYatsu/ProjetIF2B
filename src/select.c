#include <stdio.h>
#include <stdint.h>

__uint8_t select_option() {
    int option;

    printf("Choisissez une option:\n");
    printf("\t1. Démarrer une partie\n");
    printf("\t2. Reprendre une partie\n");
    printf("\t3. Quitter\n");

    printf("Votre choix : ");
    scanf("%ui", &option);

    while (option < 1 || option > 3) {
        printf("Choix invalide: ");
        scanf("%ui", &option);
    }

    return (__uint8_t)option;
}

__uint8_t select_mode() {
    unsigned int mode;

    printf("Choisissez un mode:\n");
    printf("\t1. Conquête\n");
    printf("\t2. Connecte\n");

    printf("Votre choix : ");
    scanf("%ui", &mode);

    while (mode < 1 || mode > 2) {
        printf("Choix invalide : ");
        scanf("%ui", &mode);
    }

    return (__uint8_t)mode;
}

__uint8_t select_dimension() {
    unsigned int dim;

    printf("Entrer les dimensions de l'échiquier: (ex: 8 pour 8x8) ");
    scanf("%ui", &dim);

    while (dim < 6 || dim > 12) {
        printf("La taille doit être comprise entre 6 et 12: ");
        scanf("%ui", &dim);
    }

    return (__uint8_t)dim;
}