
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
bool save_game(const GameState *state);

/**
 * @brief Vérifie si un fichier de sauvegarde existe.
 *
 * Ouvre le fichier `savegame.dat` en mode lecture pour vérifier son existence.
 *
 * @return bool `true` si le fichier existe, `false` sinon.
 */
bool save_file_exists();

/**
 * @brief Charge l'état du jeu à partir du fichier de sauvegarde.
 *
 * Cette fonction lit le contenu du fichier `savegame.dat`, désérialise les
 * données et retourne l'état du jeu correspondant. En cas d'erreur (fichier
 * introuvable, lecture ou allocation mémoire échouée), le programme s'arrête
 * avec un message d'erreur.
 *
 * @return GameState L'état du jeu restauré depuis la sauvegarde.
 */
GameState load_game();

#endif // SAVE_H
