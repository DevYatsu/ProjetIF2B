#include <stdio.h>

/**
 * @brief Affiche un menu et lit une option utilisateur comprise entre 1 et 3.
 *
 * Cette fonction présente un menu interactif à l'utilisateur avec trois options :
 *   1. Démarrer une partie
 *   2. Reprendre une partie
 *   3. Quitter
 *
 * Elle lit l'entrée de l'utilisateur depuis le terminal,
 * vérifie que la valeur est comprise entre 1 et 3,
 * et renvoie ce choix sous forme d'un entier non signé sur 8 bits.
 *
 * @return __uint8_t Le choix de l'utilisateur (1, 2 ou 3).
 */
__uint8_t select_option() {
    unsigned int option;

    printf("Choisissez une option :\n");
    printf("\t1. Démarrer une partie\n");
    printf("\t2. Reprendre une partie\n");
    printf("\t3. Quitter\n");

    printf("Votre choix : ");
    scanf(" %u", &option);

    while (option < 1 || option > 3) {
        printf("Choix invalide : ");
        scanf(" %u", &option);
    }

    return (__uint8_t)option;
}

int main(void) {
    printf("Hello, World!\n");
    const __uint8_t option = select_option();

    printf("Vous avez choisi l'option %d\n", option);
    return 0;
}