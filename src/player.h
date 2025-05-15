//
// Created by Yanis on 12/05/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

/**
 * @brief Représente les deux types de joueurs possibles dans la partie.
 */
typedef  enum {
    User, ///< Joueur humain
    AI    ///< Intelligence artificielle
} Player;


/**
 * @brief Converts a `Player` enum value to its string representation.
 *
 * This function returns "User" for the `User` player and "AI" for the `AI` player.
 *
 * @param player The `Player` enum value to convert.
 * @return A string representing the player ("User" or "AI").
 */
char* stringify_player(Player player);

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
