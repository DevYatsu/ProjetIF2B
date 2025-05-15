
#ifndef SAVE_H
#define SAVE_H
#include "game_state.h"
#include <stdbool.h>

/**
 * @brief Sauvegarde l'état actuel du jeu dans un fichier.
 *
 * Sérialise l'état du jeu et l'écrit dans un fichier nommé `savegame.dat`.
 *
 * @param state Un pointeur vers l'état de jeu à sauvegarder.
 * @return bool `true` si la sauvegarde a réussi, `false` sinon.
 */
bool save_game(const GameState* state);

/**
 * @brief Vérifie si un fichier de sauvegarde existe.
 *
 * Ouvre le fichier `savegame.dat` en mode lecture pour vérifier son existence.
 *
 * @return bool `true` si le fichier existe, `false` sinon.
 */
bool save_file_exists();

/**
 * @brief Charge un état de jeu à partir d'un fichier de sauvegarde.
 *
 * Lit le fichier `savegame.dat`, désérialise son contenu et reconstruit l'état de jeu.
 *
 * @return GameState L'état de jeu chargé depuis le fichier.
 * @warning Termine le programme avec `exit(EXIT_FAILURE)` en cas d'erreur de lecture ou d'allocation.
 */
GameState load_game();

#endif //SAVE_H
