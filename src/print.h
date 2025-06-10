
#ifndef PRINT_H
#define PRINT_H

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(x) Sleep(x)
#else
#include <unistd.h>
#define sleep_ms(x) usleep(x * 1000)
#endif

/**
 * @brief Affiche une chaîne de caractères avec un effet machine à écrire,
 * ajoutant un délai entre chaque caractère.
 * @param str La chaîne à afficher.
 * @param delay_ms Le délai en millisecondes entre chaque caractère.
 */
void print_effect(const char *str, unsigned int delay_ms);

/**
 * @brief Affiche une chaîne de caractères à la sortie standard avec un effet
 * machine à écrire prédéfini.
 * @param str La chaîne à afficher.
 */
void print_text(const char *str);

/**
 * @brief Efface le contenu de la console.
 */
void clear_screen();

/**
 * @brief Affiche un message d'au revoir.
 */
void print_bye();

/**
 * @brief Affiche l'ecran d'accueil du jeu.
 */
void print_title_screen();

#endif // PRINT_H
