//
// Created by Yanis on 12/05/2025.
//

#ifndef SELECT_H
#define SELECT_H
#include <stdio.h>
#include <stdint.h>

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
__uint8_t select_option();

/**
 * @brief Demande à l'utilisateur de choisir un mode de jeu.
 *
 * Affiche deux modes disponibles :
 *   1. Conquête
 *   2. Connecte
 *
 * L'utilisateur est invité à faire un choix entre 1 et 2.
 * La saisie est répétée jusqu'à ce qu'un choix valide soit entré.
 *
 * @return __uint8_t Le mode sélectionné par l'utilisateur (1 ou 2).
 */
__uint8_t select_mode();
/**
 * @brief Demande à l'utilisateur de choisir la taille de l'échiquier.
 *
 * L'utilisateur saisit une dimension unique pour créer un plateau carré (par exemple 8 pour un 8x8).
 * La taille doit être comprise entre 6 et 12 inclus.
 * La saisie est répétée jusqu'à ce qu'une valeur valide soit entrée.
 *
 * @return __uint8_t La dimension choisie pour l'échiquier (entre 6 et 12).
 */
__uint8_t select_dimension();
#endif //SELECT_H
