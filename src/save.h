
#ifndef SAVE_H
#define SAVE_H
#include "game_state.h"
#include <stdbool.h>

/**
 * @brief Enregistre l'état du jeu dans un fichier.
 *
 * Cette fonction sérialise l'état du jeu et l'écrit dans un fichier de sauvegarde.
 * Si le fichier n'existe pas, il est créé. Si l'ouverture échoue, un message d'erreur est affiché.
 *
 * @param state Pointeur vers l'état de jeu à enregistrer.
 * @return bool Indique si l'opération a réussi (0) ou échoué (1).
 */
bool save_game(const GameState* state);

/**
 * @brief Vérifie si le fichier de sauvegarde existe.
 *
 * Cette fonction tente d'ouvrir le fichier de sauvegarde en mode lecture.
 * Si le fichier existe, la fonction retourne vrai (1), sinon elle retourne faux (0).
 *
 * @return bool Indique si le fichier de sauvegarde existe (1) ou non (0).
 */
bool save_file_exists();

/**
 * @brief Charge l'état du jeu à partir du fichier de sauvegarde.
 *
 * Cette fonction lit l'état du jeu à partir d'un fichier nommé `savegame.dat`.
 * Si le fichier n'existe pas ou si l'ouverture échoue, un message d'erreur est affiché
 * et le programme se termine.
 *
 * @return GameState L'état de jeu chargé depuis le fichier.
 *
 * @warning Cette fonction doit être appelée uniquement si le fichier existe,
 *          sinon elle provoquera un crash.
 */
GameState load_game();

#endif //SAVE_H
