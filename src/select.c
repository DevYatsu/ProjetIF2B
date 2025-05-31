#include "select.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "game_state.h"
#include "print.h"

char validate(const char range_start, const char range_end) {
    char option_char[2];
    int result = 0;

    do {
        print_text("Votre choix : ");
        result = scanf("%1s", option_char);

        // flush le reste de la ligne
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}

        if (!result || !isdigit(option_char[0]) || option_char[0] < range_start || option_char[0] > range_end) {
            print_text("Choix invalide.\n");
        }

    } while (!result || !isdigit(option_char[0]) || option_char[0] < range_start || option_char[0] > range_end);

    return option_char[0];
}

StartOption select_option() {
    print_text("Choisissez une option:\n\t1. Démarrer une partie\n\t2. Reprendre une partie\n\t3. Quitter\n");

    const char option = validate('1', '3');

    return (StartOption)(option - '0');
}

RoundOption select_round_option() {
    print_text("Choisissez une option:\n\t1. Poser une pièce\n\t2. Abandonner\n\t3. Sauvegarder la partie\n");

    const char option = validate('1', '3');

    return (RoundOption)(option - '0');
}


GameMode select_mode() {
    print_text("Choisissez un mode:\n");
    print_text("\t1. Conquête\n");
    print_text("\t2. Connecte\n");

    const char option = validate('1', '2');

    return (GameMode)(option - '0');
}

uint8_t select_dimension() {
    print_text("Choisissez les dimensions de l'échiquier entre 6 et 12.\n");
    char buffer[3];
    int value = 0;

    do {
        print_text("Votre choix : ");
        if (scanf("%2s", buffer) != 1) {
            value = 0;
        } else {
            // ensure string contains only digits
            int valid = 1;
            for (size_t i = 0; buffer[i] != '\0'; ++i) {
                if (!isdigit((unsigned char)buffer[i])) {
                    valid = 0;
                    break;
                }
            }
            value = valid ? atoi(buffer) : 0;
        }

        // flush remaining input
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}

        if (value < 6 || value > 12) {
            print_text("Choix invalide.\n");
        }

    } while (value < 6 || value > 12);

    return (uint8_t)value;
}