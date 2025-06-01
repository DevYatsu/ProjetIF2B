#include "select.h"
#include "game_state.h"
#include "print.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Demande et valide une entrée utilisateur dans une plage de caractères.
 *
 * Affiche une invite, lit un caractère depuis l'entrée standard et vérifie
 * qu'il s'agit d'un chiffre compris entre range_start et range_end. Réaffiche
 * l'invite tant qu'une entrée invalide est saisie.
 *
 * @param range_start Le caractère de début de la plage valide (inclus).
 * @param range_end Le caractère de fin de la plage valide (inclus).
 * @return char Le caractère validé choisi par l'utilisateur.
 */
char validate(const char range_start, const char range_end) {
  char option_char[2]; // Stocke le caractère saisi par l'utilisateur
  int result = 0;      // Indique le nombre de champs lus correctement par scanf

  do {
    print_text("Votre choix : ");
    result = scanf("%1s", option_char);

    int ch;
    // flush l'entrée (enlève les caractères entrés restants jusqu'à la fin de
    // la ligne)
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }

    // Vérifie si l'entrée est valide
    if (!result || !isdigit(option_char[0]) || option_char[0] < range_start ||
        option_char[0] > range_end) {
      print_text("Choix invalide.\n");
    }

  } while (!result || !isdigit(option_char[0]) ||
           option_char[0] < range_start || option_char[0] > range_end);

  // Retourne le numéro validé
  return option_char[0];
}

StartOption select_option() {
  print_text("Choisissez une option:\n\t1. Démarrer une partie\n\t2. Reprendre "
             "une partie\n\t3. Quitter\n");

  const char option = validate('1', '3');

  return (StartOption)(option - '0');
}

RoundOption select_round_option() {
  print_text("Choisissez une option:\n\t1. Poser une pièce\n\t2. "
             "Abandonner\n\t3. Sauvegarder la partie\n");

  const char option = validate('1', '3');

  return (RoundOption)(option - '0');
}

GameMode select_mode() {
  print_text("Choisissez un mode:\n");
  print_text("\t1. Conquête\n");
  print_text("\t2. Connecte\n");

  const char option = validate('1', '2');

  return (GameMode)(option - '0');
}

uint8_t select_dimension() {
  print_text("Choisissez les dimensions de l'échiquier entre 6 et 12.\n");
  char buffer[3];
  int value = 0;

  do {
    print_text("Votre choix : ");
    if (scanf("%2s", buffer) != 1) {
      value = 0;
    } else {
      // ensure string contains only digits
      int valid = 1;
      for (size_t i = 0; buffer[i] != '\0'; ++i) {
        if (!isdigit((unsigned char)buffer[i])) {
          valid = 0;
          break;
        }
      }
      value = valid ? atoi(buffer) : 0;
    }

    // flush remaining input
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }

    if (value < 6 || value > 12) {
      print_text("Choix invalide.\n");
    }

  } while (value < 6 || value > 12);

  return (uint8_t)value;
}

//> CONQUEST MODE
Tile select_valid_tile(const GameState* state) {
  char nom_piece[10];
  const char* current_player_str = stringify_player(state->is_turn_of);
  Tile tile;
  bool piece_allowed = false;

  PieceCountTracker* piece_counter = (state->is_turn_of == User)
      ? (PieceCountTracker*)&state->piece_counter_1
      : (PieceCountTracker*)&state->piece_counter_2;

  do {
    printf("Quelle pièce souhaitez-vous jouer ? ");
    scanf("%9s", nom_piece);
    tile = deserialize_tile(nom_piece, current_player_str, "", true);

    if (!tile.some) {
      printf("Pièce inconnue.\n");
      continue;
    }

    piece_allowed = add_piece(piece_counter, tile.value.kind);
    if (!piece_allowed) {
      printf("Vous n'avez plus de %s à jouer.\n", nom_piece);
    }

  } while (!tile.some || !piece_allowed);

  return tile;
}

TargetPosition select_valid_target_position(const GameState* state) {
  const uint8_t dim = state->board.dim;
  char target_tile[4] = {0};

  while (1) {
    printf("Où souhaitez-vous la placer ? ");
    scanf("%3s", target_tile);

    const size_t len = strlen(target_tile);
    if (len < 2 || len > 3) {
      printf("Erreur : format invalide (ex: A3 ou A10).\n");
      continue;
    }

    target_tile[0] = (char)toupper((unsigned char)target_tile[0]);

    if (!isdigit(target_tile[1]) || (len == 3 && !isdigit(target_tile[2]))) {
      printf("Erreur : les chiffres sont invalides.\n");
      continue;
    }

    const int row = (len == 2)
        ? (target_tile[1] - '0')
        : ((target_tile[1] - '0') * 10 + (target_tile[2] - '0'));

    if (row <= 0 || row > dim) {
      printf("Erreur : ligne invalide (1-%u).\n", dim);
      continue;
    }

    const int col = target_tile[0] - 'A';
    if (col < 0 || col >= dim) {
      printf("Erreur : colonne invalide (A-%c).\n", 'A' + dim - 1);
      continue;
    }

    const uint8_t px = (uint8_t)col;
    const uint8_t py = dim - (uint8_t)row;

    if (state->board.tiles[py][px].some) {
      printf("Erreur : Il y a déjà une pièce en %s.\n", target_tile);
      continue;
    }

    return (TargetPosition){.x = px, .y = py};
  }
}
//< CONQUEST MODE


//> CONNECT MODE
// Vérifie si le joueur actuel a au moins une case capturée par le type de pièce requis
bool has_tile_captured_by_kind_for_current_player(const GameState* state, PieceKind required_kind) {
  const uint8_t dim = state->board.dim;

  for (uint8_t y = 0; y < dim; y++) {
    for (uint8_t x = 0; x < dim; x++) {
      const Tile* tile = &state->board.tiles[y][x];

      // Vérifie si la case est capturée par le joueur actuel
      if (tile->captured_by.some && tile->captured_by.player == state->is_turn_of) {
        // Vérifie si cette case a été capturée par le bon type de pièce
        if (tile->some && tile->value.kind == required_kind &&
            tile->value.player == state->is_turn_of) {
          return true;
        }
      }
    }
  }

  return false;
}

// Sélection d'une pièce valide pour le mode Connect
Tile select_valid_tile_for_connect(const GameState* state) {
  char nom_piece[10];
  const char* current_player_str = stringify_player(state->is_turn_of);
  Tile tile;
  bool piece_allowed = false;

  PieceCountTracker* piece_counter = (state->is_turn_of == User)
      ? (PieceCountTracker*)&state->piece_counter_1
      : (PieceCountTracker*)&state->piece_counter_2;

  while (1) {
    printf("Quelle pièce souhaitez-vous jouer ? ");
    scanf("%9s", nom_piece);
    tile = deserialize_tile(nom_piece, current_player_str, "", true);

    if (!tile.some) {
      printf("Pièce inconnue.\n");
      continue;
    }

    // Vérifie les règles de hiérarchie du mode Connect
    PieceKind kind = tile.value.kind;
    bool hierarchy_allowed = false;

    switch (kind) {
      case Pawn:
        hierarchy_allowed = true; // Les pions peuvent toujours être posés
        break;
      case Knight:
        hierarchy_allowed = has_tile_captured_by_kind_for_current_player(state, Pawn);
        if (!hierarchy_allowed) {
          printf("Erreur : vous devez d'abord avoir des cases capturées par des pions.\n");
        }
        break;
      case Bishop:
        hierarchy_allowed = has_tile_captured_by_kind_for_current_player(state, Knight);
        if (!hierarchy_allowed) {
          printf("Erreur : vous devez d'abord avoir des cases capturées par des cavaliers.\n");
        }
        break;
      case Rook:
        hierarchy_allowed = has_tile_captured_by_kind_for_current_player(state, Bishop);
        if (!hierarchy_allowed) {
          printf("Erreur : vous devez d'abord avoir des cases capturées par des fous.\n");
        }
        break;
      case Queen:
        hierarchy_allowed = has_tile_captured_by_kind_for_current_player(state, Rook);
        if (!hierarchy_allowed) {
          printf("Erreur : vous devez d'abord avoir des cases capturées par des tours.\n");
        }
        break;
      case King:
        hierarchy_allowed = has_tile_captured_by_kind_for_current_player(state, Queen);
        if (!hierarchy_allowed) {
          printf("Erreur : vous devez d'abord avoir des cases capturées par la reine.\n");
        }
        break;
    }

    if (!hierarchy_allowed) {
      continue;
    }

    // Vérifie que le joueur a encore cette pièce disponible
    piece_allowed = add_piece(piece_counter, kind);
    if (!piece_allowed) {
      printf("Vous n'avez plus de %s à jouer.\n", nom_piece);
      continue;
    }

    return tile;
  }
}

// Vérifie si une position est valide pour le placement en mode Connect
bool is_valid_connect_placement(const GameState* state, PieceKind kind, uint8_t x, uint8_t y) {
  const Tile* target_tile = &state->board.tiles[y][x];

  switch (kind) {
    case Pawn:
      // Les pions peuvent être posés n'importe où (case vide)
      return !target_tile->some;

    case Knight:
      // Les cavaliers ne peuvent être placés que sur des cases capturées par des pions du même joueur
      return !target_tile->some &&
             target_tile->captured_by.some &&
             target_tile->captured_by.player == state->is_turn_of &&
             is_tile_captured_by_piece_kind(state, x, y, Pawn);

    case Bishop:
      // Les fous ne peuvent être placés que sur des cases capturées par des cavaliers du même joueur
      return !target_tile->some &&
             target_tile->captured_by.some &&
             target_tile->captured_by.player == state->is_turn_of &&
             is_tile_captured_by_piece_kind(state, x, y, Knight);

    case Rook:
      // Les tours ne peuvent être placées que sur des cases capturées par des fous du même joueur
      return !target_tile->some &&
             target_tile->captured_by.some &&
             target_tile->captured_by.player == state->is_turn_of &&
             is_tile_captured_by_piece_kind(state, x, y, Bishop);

    case Queen:
      // La reine ne peut être placée que sur des cases capturées par des tours du même joueur
      return !target_tile->some &&
             target_tile->captured_by.some &&
             target_tile->captured_by.player == state->is_turn_of &&
             is_tile_captured_by_piece_kind(state, x, y, Rook);

    case King:
      // Le roi ne peut être placé que sur des cases capturées par la reine du même joueur
      return !target_tile->some &&
             target_tile->captured_by.some &&
             target_tile->captured_by.player == state->is_turn_of &&
             is_tile_captured_by_piece_kind(state, x, y, Queen);

    default:
      return false;
  }
}

// Sélection d'une position valide pour le mode Connect
TargetPosition select_valid_target_position_for_connect(const GameState* state, const Tile* tile) {
  const uint8_t dim = state->board.dim;
  char target_tile[10] = {0};

  while (1) {
    printf("Où souhaitez-vous la placer ? ");
    scanf("%9s", target_tile);

    const size_t len = strlen(target_tile);
    if (len < 2 || len > 3) {
      printf("Erreur : format invalide (ex: A3 ou A10).\n");
      continue;
    }

    target_tile[0] = (char)toupper((unsigned char)target_tile[0]);

    if (!isdigit(target_tile[1]) || (len == 3 && !isdigit(target_tile[2]))) {
      printf("Erreur : les chiffres sont invalides.\n");
      continue;
    }

    const int row = (len == 2)
        ? (target_tile[1] - '0')
        : ((target_tile[1] - '0') * 10 + (target_tile[2] - '0'));

    if (row <= 0 || row > dim) {
      printf("Erreur : ligne invalide (1-%u).\n", dim);
      continue;
    }

    int col = target_tile[0] - 'A';
    if (col < 0 || col >= dim) {
      printf("Erreur : colonne invalide (A-%c).\n", 'A' + dim - 1);
      continue;
    }

    const uint8_t px = (uint8_t)col;
    const uint8_t py = dim - (uint8_t)row;

    if (state->board.tiles[py][px].some) {
      printf("Erreur : Il y a déjà une pièce en %s.\n", target_tile);
      continue;
    }

    // Vérifie la validité selon les règles du mode Connect
    if (!is_valid_connect_placement(state, tile->value.kind, px, py)) {
      const char* piece_name = stringify_piece(tile->value.kind);

      switch (tile->value.kind) {
        case Knight:
          printf("Erreur : les cavaliers ne peuvent être placés que sur des cases capturées par vos pions.\n");
          break;
        case Bishop:
          printf("Erreur : les fous ne peuvent être placés que sur des cases capturées par vos cavaliers.\n");
          break;
        case Rook:
          printf("Erreur : les tours ne peuvent être placées que sur des cases capturées par vos fous.\n");
          break;
        case Queen:
          printf("Erreur : la reine ne peut être placée que sur des cases capturées par vos tours.\n");
          break;
        case King:
          printf("Erreur : le roi ne peut être placé que sur des cases capturées par votre reine.\n");
          break;
        default:
          printf("Erreur : vous ne pouvez pas poser de %s ici.\n", piece_name);
          break;
      }
      continue;
    }

    return (TargetPosition){.x = px, .y = py};
  }
}

//< CONNECT MODE