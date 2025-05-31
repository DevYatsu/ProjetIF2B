//
// Created by Yanis on 12/05/2025.
//

#ifndef PLAYER_H
#define PLAYER_H
#include  <stdbool.h>
/**
 * @brief Représente les deux types de joueurs possibles dans la partie.
 */
typedef  enum {
    User,
    Opponent
} Player;

/**
 * @brief Représente une option de joueur, qui peut être soit présente (some = true) avec un joueur spécifique,
 * soit absente (some = false).
 *
 * Cette structure est utilisée pour indiquer si un joueur est sélectionné ou non.
 */
typedef struct {
    bool some;
    Player player;
} PlayerOption;

/**
 * @brief Représente une option de joueur, qui peut être soit présente (some = true) avec un joueur spécifique,
 * soit absente (some = false).
 *
 * Cette structure est utilisée pour indiquer si un joueur est sélectionné ou non.
 */
PlayerOption no_player();

/**
 * @brief Crée une option de joueur avec un joueur spécifique.
 *
 * Cette fonction retourne une `PlayerOption` avec `some` à true et le joueur spécifié.
 *
 * @param player Le joueur à encapsuler dans l'option.
 * @return PlayerOption L'option de joueur contenant le joueur spécifié.
 */
PlayerOption player_option(Player player);

/**
 * @brief Converts a `Player` enum value to its string representation.
 *
 * This function returns "User" for the `User` player and "AI" for the `AI` player.
 *
 * @param player The `Player` enum value to convert.
 * @return A string representing the player.
 */
char* stringify_player(Player player);

/**
 * @brief Converts a `PlayerOption` to its string representation.
 *
 * If the option is present (`some` is true), it returns the string representation of the player.
 * If the option is not present (`some` is false), it returns "_".
 *
 * @param player_opt The `PlayerOption` to convert.
 * @return A string representing the player option.
 */
char* stringify_player_option(PlayerOption player_opt);

/**
 * @brief Parses a string to create a `PlayerOption`.
 *
 * This function checks the input string and returns a `PlayerOption` with `some` set to true
 * if the string matches "User" or "Opponent". If the string does not match either, it returns
 * an option with `some` set to false.
 *
 * @param str The input string to parse.
 * @return PlayerOption The parsed player option.
 */
PlayerOption parse_player_option(const char* str);

/**
 * @brief Génère aléatoirement un identifiant de joueur.
 *
 * Cette fonction utilise `rand()` pour retourner aléatoirement l'un des deux
 * identifiants de joueur : `User` ou `AI`. Elle suppose que `srand()` a déjà été
 * appelé quelque part avant, afin de garantir une génération aléatoire correcte.
 *
 * @return Player Le joueur sélectionné aléatoirement (`User` ou `AI`).
 */
Player random_player();

#endif //PLAYER_H
