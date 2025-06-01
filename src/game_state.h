
#ifndef GAME_STATE_H
#define GAME_STATE_H
#include "player.h"
#include "board.h"
#include "piece_count_tracker.h"

/**
 * @brief Enum représentant les modes de jeu disponibles.
 *
 * - `Conquest` : Mode basé sur la conquête du plateau.
 * - `Connect` : Mode basé sur la connexion de pièces (similaire à puissance 4, par exemple).
 */
typedef enum {
    Conquest = 1, ///< Mode conquête
    Connect   ///< Mode connecte
} GameMode;

/**
 * @brief Structure représentant l'état complet d'une partie.
 *
 * Cette structure regroupe toutes les informations nécessaires pour décrire
 * un instant précis du jeu :
 * - Le mode de jeu sélectionné
 * - L'état du plateau (`Board`)
 * - Le joueur dont c'est le tour (`Player`)
 */
typedef struct {
    GameMode mode;       ///< Mode de jeu actuel
    Board board;         ///< Plateau de jeu avec les positions des pièces
    Player is_turn_of;   ///< Joueur à qui c'est le tour (User ou AI)
    Player is_white;     ///< Joueur qui joue avec les pièces blanches
    PieceCountTracker piece_counter_1; // Compteur de pièces disponibles pour le joueur 1
    PieceCountTracker piece_counter_2; // Compteur de pièces disponibles pour le joueur 2
} GameState;

/**
 * @brief Initialise un état de jeu avec les paramètres spécifiés.
 *
 * Crée un plateau vide de dimension donnée, initialise les compteurs de pièces,
 * et choisit aléatoirement quel joueur commence la partie.
 *
 * @param mode Le mode de jeu (par exemple Conquest ou Connect).
 * @param dim La dimension du plateau (entre 6 et 12 typiquement).
 * @return GameState L'état de jeu initialisé.
 */
GameState init_game_state(GameMode mode, uint8_t dim);

/**
 * @brief Applique la capture de pièces selon les règles du mode Conquest.
 *
 * Lorsque le joueur capturant place une pièce sur une case occupée,
 * toutes les pièces adjacentes (selon les règles de capture) sont capturées.
 *
 * @param state Pointeur vers l'état du jeu actuel.
 * @param x Coordonnée x de la case où la pièce est placée.
 * @param y Coordonnée y de la case où la pièce est placée.
 * @param piece La pièce qui est placée sur le plateau.
 * @param capturer Le joueur qui effectue la capture.
 */
void apply_conquest_capture(const GameState* state, uint8_t x, uint8_t y, ChessPiece piece, Player capturer);

/**
 * @brief Renvoie le nombre de pièces capturées par un joueur spécifique.
 *
 * Parcourt le plateau de jeu et compte les pièces capturées par le joueur indiqué.
 *
 * @param state Pointeur vers l'état de jeu à inspecter.
 * @param player Le joueur dont on veut connaître le nombre de pièces capturées.
 * @return uint8_t Le nombre de pièces capturées par le joueur.
 */
uint8_t get_captured_count_of(const GameState* state, Player player);

/**
 * @brief Inverse le tour du joueur actif.
 *
 * Permet de passer du joueur actuel à l'autre (ex: de l'utilisateur à l'adversaire).
 *
 * @param state Pointeur vers l'état de jeu à modifier.
 */
void toggle_user_turn(GameState* state);

/**
 * @brief Renvoie le nom du joueur dont c'est le tour.
 *
 * Ce nom est retourné sous forme de chaîne de caractères.
 *
 * @param state Pointeur vers l'état de jeu.
 * @return char* Le nom du joueur actuel ("User", "Opponent", etc.).
 */
char* get_user_turn_name(const GameState* state);

/**
 * @brief Renvoie le pointeur vers le compteur de pièces du joueur actuel.
 *
 * Utile pour suivre les pièces capturées ou en jeu selon le joueur actif.
 *
 * @param state Pointeur vers l'état de jeu.
 * @return const PieceCountTracker* Le compteur associé au joueur actif.
 */
const PieceCountTracker* get_user_turn_count_tracker(const GameState* state);

/**
 * @brief Affiche le plateau de jeu en utilisant des représentations ASCII.
 *
 * Représente chaque pièce à l'aide d'un dessin ASCII, en distinguant les joueurs.
 * Ajoute les coordonnées comme dans une vraie partie d'échecs (lettres et chiffres).
 *
 * @param state Pointeur vers l'état de jeu contenant le plateau.
 */
void print_board(const GameState* state);

/**
 * @brief Libère la mémoire allouée pour le plateau de jeu.
 *
 * À utiliser à la fin d'une partie pour éviter les fuites de mémoire.
 *
 * @param state Pointeur vers l'état de jeu à nettoyer.
 */
void free_game_state(const GameState *state);

#endif //GAME_STATE_H
