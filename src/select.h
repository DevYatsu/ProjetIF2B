#ifndef SELECT_H
#define SELECT_H
#include "game_state.h"

#ifndef _WIN32
typedef uint8_t __uint8_t;
#endif

typedef enum { Start = 1, Restart, Leave } StartOption;

typedef enum { Play = 1, GiveUp, SaveGame } RoundOption;

/**
 * @brief Affiche un menu et lit une option utilisateur comprise entre 1 et 3.
 *
 * Cette fonction présente un menu interactif à l'utilisateur avec trois options
 * :
 *   1. Démarrer une partie
 *   2. Reprendre une partie
 *   3. Quitter
 *
 * Elle lit l'entrée de l'utilisateur depuis le terminal,
 * vérifie que la valeur est comprise entre 1 et 3,
 * et renvoie ce choix sous forme de l'enum StartOption.
 *
 * @return StartOption Le choix de l'utilisateur.
 */
StartOption select_option();

/**
 * @brief Affiche un menu et lit une option utilisateur comprise entre 1 et 3.
 *
 * Cette fonction présente un menu interactif à l'utilisateur avec trois options
 * :
 *   1. Poser une pièce
 *   2. Abandonner
 *   3. Sauvegarder la partie
 *
 * Elle lit l'entrée de l'utilisateur depuis le terminal,
 * vérifie que la valeur est comprise entre 1 et 3,
 * et renvoie ce choix sous forme de l'enum RoundOption.
 *
 * @return RoundOption Le choix de l'utilisateur.
 */
RoundOption select_round_option();

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
 * @return GameMode Le mode sélectionné par l'utilisateur.
 */
GameMode select_mode();
/**
 * @brief Demande à l'utilisateur de choisir la taille de l'échiquier.
 *
 * L'utilisateur saisit une dimension unique pour créer un plateau carré (par
 * exemple 8 pour un 8x8). La taille doit être comprise entre 6 et 12 inclus. La
 * saisie est répétée jusqu'à ce qu'une valeur valide soit entrée.
 *
 * @return __uint8_t La dimension choisie pour l'échiquier (entre 6 et 12).
 */
uint8_t select_dimension();

typedef struct {
    int x;
    int y;
} TargetPosition;


/**
 * @brief Sélectionne une tuile valide à jouer.
 *
 * Cette fonction demande à l'utilisateur de choisir une pièce à jouer. Elle
 * vérifie que la pièce est valide et qu'il en reste suffisamment dans le
 * compteur de pièces du joueur actuel.
 *
 * @param state L'état actuel du jeu.
 * @return Tile La tuile sélectionnée par l'utilisateur.
 */
Tile select_valid_tile(const GameState* state);

/**
 * @brief Sélectionne une position valide sur le plateau de jeu.
 *
 * Cette fonction demande à l'utilisateur de choisir une position (x, y) sur le
 * plateau de jeu. Elle s'assure que la position est valide et n'est pas déjà
 * occupée par une autre pièce.
 *
 * @param state L'état actuel du jeu.
 * @return TargetPosition La position valide sélectionnée par l'utilisateur.
 */
TargetPosition select_valid_target_position(const GameState* state);

/**
 * @brief Sélectionne une tuile valide pour le mode Connect.
 *
 * Cette fonction demande à l'utilisateur de choisir une pièce à jouer dans le
 * mode Connect. Elle vérifie que la pièce est valide et qu'il en reste
 * suffisamment dans le compteur de pièces du joueur actuel.
 *
 * @param state L'état actuel du jeu.
 * @return Tile La tuile sélectionnée par l'utilisateur.
 */
Tile select_valid_tile_for_connect(const GameState* state);

/**
 * @brief Vérifie si une position de placement est valide pour le mode Connect.
 *
 * Cette fonction vérifie si une pièce de type `kind` peut être placée à la
 * position (x, y) sur le plateau de jeu dans le mode Connect.
 *
 * @param state L'état actuel du jeu.
 * @param kind Le type de pièce à placer.
 * @param x La coordonnée x de la position.
 * @param y La coordonnée y de la position.
 * @return bool Vrai si le placement est valide, faux sinon.
 */
bool is_valid_connect_placement(const GameState* state, PieceKind kind, uint8_t x, uint8_t y);

/**
 * @brief Sélectionne une position valide pour placer une pièce dans le mode Connect.
 *
 * Cette fonction demande à l'utilisateur de choisir une position (x, y) pour
 * placer une pièce dans le mode Connect. Elle s'assure que la position est valide et n'est pas déjà
 *
 * @param state L'état actuel du jeu.
 * @param tile La tuile à placer.
 * @return TargetPosition La position valide sélectionnée par l'utilisateur.
 */
TargetPosition select_valid_target_position_for_connect(const GameState* state, const Tile* tile);

#endif // SELECT_H
