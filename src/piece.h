
#ifndef PIECE_H
#define PIECE_H

/**
 * @brief Types de pièces pouvant se trouver sur une case de l'échiquier (ou None=rien).
 */
typedef enum {
    None,    ///< Case vide
    King,    ///< Roi
    Queen,   ///< Reine
    Rook,    ///< Tour
    Bishop,  ///< Fou
    Knight,  ///< Cavalier
    Pawn,    ///< Pion
} ChessPiece;


/**
 * @brief Convertit une chaîne de caractères en type de pièce d’échecs (`ChessPiece`).
 *
 * Cette fonction analyse une chaîne (ex. `"King"`, `"Queen"`, etc.) et retourne
 * la valeur correspondante de l'énumération `ChessPiece`.
 *
 * La chaîne `"None"` est également prise en charge.
 *
 * Si la chaîne ne correspond à aucune pièce valide, la fonction imprime une erreur
 * et termine le programme avec `exit(EXIT_FAILURE)`.
 *
 * @param piece_str Une chaîne sans espaces représentant une pièce d’échecs.
 * @return ChessPiece La pièce correspondante.
 *
 * @warning Quitte le programme si la chaîne est invalide.
 */
ChessPiece deserialize_piece(const char* piece_str);

/**
 * @brief Convertit une chaîne de caractères en type de pièce d’échecs (`ChessPiece`), avec valeur par défaut.
 * VOIR `deserialize_piece` pour la version stricte.
 *
 * Cette fonction retourne la pièce correspondante à une chaîne (ex. `"King"`, `"Queen"`, etc.).
 * Si la chaîne ne correspond à aucune pièce connue, elle retourne `None` sans interrompre le programme.
 *
 * @param piece_str Une chaîne sans espaces représentant une pièce d’échecs.
 * @return ChessPiece La pièce correspondante, ou `None` si la chaîne est invalide.
 */
ChessPiece piece_from_string(const char* piece_str);

#endif //PIECE_H
