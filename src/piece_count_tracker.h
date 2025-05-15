
#ifndef PIECE_COUNTER_H
#define PIECE_COUNTER_H
#include <stdint.h>
#include "piece.h"
#include <stdbool.h>

typedef struct {
    uint8_t pawns;     // 8 max
    uint8_t knights;   // 2 max
    uint8_t bishops;   // 2 max
    uint8_t rooks;     // 2 max
    uint8_t queen;     // 1 max
    uint8_t king;      // 1 max
} PieceCountTracker;

/**
 * @brief Initialise le compteur de pièces disponibles d’un joueur.
 *
 * Cette fonction retourne une structure `PieceCountTracker` contenant le nombre
 * maximum de chaque type de pièce qu’un joueur peut poser en début de partie.
 *
 * Ce compteur ne représente pas les pièces sur le plateau, mais uniquement celles
 * qu’un joueur a encore le droit de poser.
 *
 * Quantités initiales :
 * - 8 pions
 * - 2 cavaliers
 * - 2 fous
 * - 2 tours
 * - 1 reine
 * - 1 roi
 *
 * @return PieceCountTracker Structure initialisée avec toutes les pièces disponibles.
 */
PieceCountTracker init_piece_counter();

/**
 * @brief Tente de consommer une pièce encore disponible dans le compteur.
 *
 * Cette fonction modélise la pose d’une pièce par un joueur.
 * Elle décrémente le compteur associé au type de pièce demandé, si ce compteur est strictement positif.
 *
 * Cela permet de vérifier qu’un joueur ne pose pas plus de pièces qu’il n’en possède selon les règles.
 *
 * @param counter Pointeur vers le compteur de pièces encore disponibles du joueur.
 * @param piece Le type de pièce que le joueur souhaite poser (`PieceKind`).
 * @return true Si le compteur permet de poser la pièce (décrément effectué).
 * @return false Si le joueur a déjà utilisé toutes ses pièces de ce type.
 */
bool add_piece(PieceCountTracker* counter, const PieceKind piece);

#endif //PIECE_COUNTER_H
