
#include "game_state.h"
#include "piece.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *FILENAME = "savegame.dat";

const int MAX_GAME_STATE_STR_LEN =
    9 +                   // strlen("Conquest ")
    5 +                   // strlen("User ")
    3 +                   // longueur de dim (2 digits et un espace)
    (12 * 12 * 10) +      // 12 (max dim) et 10 pour chaque piece
    100;                  // securité

/**
 * @brief Sérialise un `GameState` sous forme de chaîne lisible.
 *
 * Format :
 * mode=Conquest
 * white=User
 * turn=Opponent
 * dim=8
 * tiles=Pawn:User _ Queen:Opponent ...
 *
 * @param state Pointeur vers l'état de jeu à sérialiser.
 * @return char* Chaîne allouée dynamiquement. À libérer avec `free()`.
 */
char *serialize(const GameState *state) {
  char *str = malloc(MAX_GAME_STATE_STR_LEN);
  if (!str) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  str[0] = '\0'; // Init buffer

  const char *mode_str = (state->mode == Conquest) ? "Conquest" : "Connect";
  snprintf(str, MAX_GAME_STATE_STR_LEN,
           "mode=%s\nwhite=%s\nturn=%s\ndim=%d\ntiles=", mode_str,
           stringify_player(state->is_white),
           stringify_player(state->is_turn_of), state->board.dim);

  for (uint8_t i = 0; i < state->board.dim; i++) {
    for (uint8_t j = 0; j < state->board.dim; j++) {
      strcat(str, " ");
      const Tile tile = state->board.tiles[i][j];

      strcat(str, tile.some ? stringify_piece(tile.value.kind) : "_");
      strcat(str, ":");
      strcat(str, tile.some ? stringify_player(tile.value.player) : "_");
      strcat(str, ":");
      strcat(str, tile.captured_by.some
                      ? stringify_player_option(tile.captured_by)
                      : "_");
    }
  }

  strcat(str, "\n");

  return str;
}

// Représente le résultat de la désérialisation
typedef enum {
  DESERIALIZE_SUCCESS = 0,
  DESERIALIZE_NULL_INPUT,
  DESERIALIZE_MEMORY_ERROR,
  DESERIALIZE_INVALID_FORMAT,
  DESERIALIZE_INVALID_DIMENSION,
  DESERIALIZE_INVALID_MODE,
  DESERIALIZE_INVALID_PLAYER,
  DESERIALIZE_MISSING_TILES,
  DESERIALIZE_INVALID_PIECE_COUNT
} DeserializeResult;

// Fonction utilitaire pour copier une chaîne de manière sécurisée
static bool safe_string_copy(char *dest, const char *src, size_t dest_size) {
  if (!dest || !src || dest_size == 0)
    return false;

  const size_t src_len = strlen(src);
  if (src_len >= dest_size)
    return false;

  strcpy(dest, src);
  return true;
}

// Fonction utilitaire pour analyser une ligne et extraire un champ spécifique
static bool parse_field(const char *line, const char *prefix, char *output,
                        size_t output_size) {
  const size_t prefix_len = strlen(prefix);
  if (strncmp(line, prefix, prefix_len) != 0)
    return false;

  return safe_string_copy(output, line + prefix_len, output_size);
}

// Fonction utilitaire pour convertir une chaîne en énumération GameMode
static bool string_to_mode(const char *mode_str, GameMode *mode) {
  if (strcmp(mode_str, "Conquest") == 0) {
    *mode = Conquest;
    return true;
  }
  if (strcmp(mode_str, "Connect") == 0) {
    *mode = Connect;
    return true;
  }
  return false;
}

// Fonction utilitaire pour convertir une chaîne en énumération Player
static bool string_to_player(const char *player_str, Player *player) {
  if (strcmp(player_str, "User") == 0) {
    *player = User;
    return true;
  }
  if (strcmp(player_str, "Opponent") == 0) {
    *player = Opponent;
    return true;
  }
  return false;
}

// Fonction utilitaire pour convertir une chaîne en un Tile
static DeserializeResult parse_tiles(const char *tiles_str, GameState *state,
                                     uint8_t dim) {
  const char *current = tiles_str;
  char tile_buffer[32];

  for (uint8_t i = 0; i < dim; i++) {
    for (uint8_t j = 0; j < dim; j++) {
      // Skip whitespace
      while (*current == ' ' || *current == '\t')
        current++;

      // Read piece token
      int k = 0;
      while (*current && *current != ' ' && *current != '\t' &&
             *current != '\n' && k < sizeof(tile_buffer) - 1) {
        tile_buffer[k++] = *current++;
      }
      tile_buffer[k] = '\0';

      // Découper tile_buf en 3 parties : piece, owner, captured
      char *piece_str = strtok(tile_buffer, ":");
      // se souvient de la chaine gràce à un pointeur static
      char *owner_str = strtok(NULL, ":");
      char *captured_str = strtok(NULL, ":");

      if (!piece_str || !owner_str || !captured_str) {
        return DESERIALIZE_INVALID_FORMAT;
      }

      const Tile tile =
          deserialize_tile(piece_str, owner_str, captured_str, true);
      state->board.tiles[i][j] = tile;
    }
  }

  return DESERIALIZE_SUCCESS;
}

/**
 * @brief Fonction de désérialisation sécurisée d'une chaîne de caractères en un
 * état de jeu.
 *
 * Cette fonction prend une chaîne de caractères représentant l'état du jeu et
 * la convertit en une structure `GameState`. Elle gère les erreurs potentielles
 * et retourne un code d'erreur approprié.
 *
 * @param str La chaîne de caractères à désérialiser.
 * @param state Pointeur vers l'état du jeu à remplir.
 * @return DeserializeResult Le résultat de la désérialisation.
 */
DeserializeResult deserialize_safe(const char *str, GameState *state) {
  // Vérification des entrées
  if (!str || !state) {
    return DESERIALIZE_NULL_INPUT;
  }

  // Initialisation de l'état du jeu
  memset(state, 0, sizeof(GameState));

  // Dupliquer la chaîne pour éviter de modifier l'original
  // car strtok risque de la modifier
  char *dup = strdup(str);
  if (!dup) {
    return DESERIALIZE_MEMORY_ERROR;
  }

  // variables pour stocker les champs d'en-tête
  char mode_str[16] = {0};
  char white_str[16] = {0};
  char turn_str[16] = {0};
  int dim = 0;
  int parsed_fields = 0;

  // On parse les lignes d'en-tête une à une
  const char *line = strtok(dup, "\n");
  while (line && parsed_fields < 4) {
    if (parse_field(line, "mode=", mode_str, sizeof(mode_str))) {
      parsed_fields++;
    } else if (parse_field(line, "white=", white_str, sizeof(white_str))) {
      parsed_fields++;
    } else if (parse_field(line, "turn=", turn_str, sizeof(turn_str))) {
      parsed_fields++;
    } else if (strncmp(line, "dim=", 4) == 0) {
      dim = atoi(line + 4);
      if (dim < 6 || dim > 12) {
        free(dup);
        return DESERIALIZE_INVALID_DIMENSION;
      }
      parsed_fields++;
    } else if (strncmp(line, "tiles=", 6) == 0) {
      break; // Section des tuiles trouvée, arrêt de l'analyse des en-têtes
    }
    line = strtok(NULL, "\n");
  }

  // Vérifie que tous les champs requis ont été trouvés
  if (parsed_fields != 4) {
    free(dup);
    return DESERIALIZE_INVALID_FORMAT;
  }

  // Convertit les chaînes en énumérations avec validation
  if (!string_to_mode(mode_str, &state->mode)) {
    free(dup);
    return DESERIALIZE_INVALID_MODE;
  }

  if (!string_to_player(white_str, &state->is_white)) {
    free(dup);
    return DESERIALIZE_INVALID_PLAYER;
  }

  if (!string_to_player(turn_str, &state->is_turn_of)) {
    free(dup);
    return DESERIALIZE_INVALID_PLAYER;
  }

  // Initialize board
  state->board = init_board((uint8_t)dim);
  if (!state->board.tiles) {
    free(dup);
    return DESERIALIZE_MEMORY_ERROR;
  }

  // on s'occupe de la section des tiles
  const char *tiles_line = strstr(str, "tiles=");
  if (!tiles_line) {
    free(dup);
    free_board(&state->board);
    return DESERIALIZE_MISSING_TILES;
  }

  tiles_line += 6; // On saute "tiles="
  const DeserializeResult tiles_result =
      parse_tiles(tiles_line, state, (uint8_t)dim);

  free(dup);

  if (tiles_result != DESERIALIZE_SUCCESS) {
    free_board(&state->board);
    return tiles_result;
  }

  // Initialisation de piece counters, et on restaure son état
  state->piece_counter_1 = init_piece_counter();
  state->piece_counter_2 = init_piece_counter();

  for (uint8_t i = 0; i < state->board.dim; i++) {
    for (uint8_t j = 0; j < state->board.dim; j++) {
      const Tile tile = state->board.tiles[i][j];
      if (tile.some) {
        PieceCountTracker *counter = (tile.value.player == User)
                                         ? &state->piece_counter_1
                                         : &state->piece_counter_2;
        const bool result = add_piece(counter, tile.value.kind);

        if (!result) {
          // Si on n'a pas pu ajouter la pièce, on libère le plateau et retourne
          // une erreur Cela peut arriver si le nombre de pièces sur le plateau
          // dépasse la limite autorisée
          free_board(&state->board);
          return DESERIALIZE_INVALID_PIECE_COUNT;
        }
      }
    }
  }

  return DESERIALIZE_SUCCESS;
}

// Fonction qui gère la désérialisation d'une chaîne de caractères en un
// `GameState` avec gestion des erreurs
GameState deserialize(const char *str) {
  GameState state;
  const DeserializeResult result = deserialize_safe(str, &state);

  if (result != DESERIALIZE_SUCCESS) {
    // Messages d'erreur associés aux codes
    const char *error_messages[] = {
        "Succès",
        "L'entrée est NULL",
        "Échec de l'allocation mémoire",
        "Format invalide ou champs requis manquants",
        "Dimension invalide (doit être comprise entre 6 et 12)",
        "Mode de jeu invalide",
        "Spécification de joueur invalide",
        "Section des tuiles manquante",
        "Nombre de pièces capturées dépassant les limites autorisées"};

    printf("Erreur de désérialisation : %s\n", error_messages[result]);
    exit(EXIT_FAILURE);
  }

  return state;
}

bool save_game(const GameState *state) {
  FILE *file = fopen(FILENAME, "w");
  if (!file) {
    perror("Échec de l'ouverture du fichier pour la sauvegarde");
    return false;
  }

  char *serialized = serialize(state);

  if (fprintf(file, "%s", serialized) < 0) {
    perror("Échec de l'écriture dans le fichier");
    free(serialized);
    fclose(file);
    return false;
  }

  free(serialized);
  fclose(file);
  return true;
}

bool save_file_exists() {
  FILE *file = fopen(FILENAME, "r");
  if (file != NULL) {
    fclose(file);
    return true;
  }
  return false;
}

GameState load_game() {
  FILE *file = fopen(FILENAME, "r");
  if (!file) {
    perror("Erreur lors de l'ouverture du fichier de sauvegarde");
    exit(EXIT_FAILURE);
  }

  char *buffer = malloc(MAX_GAME_STATE_STR_LEN);
  if (!buffer) {
    perror("Échec de l'allocation mémoire");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  const size_t total_read =
      fread(buffer, sizeof(char), MAX_GAME_STATE_STR_LEN - 1, file);
  if (ferror(file)) {
    perror("Erreur lors de la lecture du fichier de sauvegarde");
    free(buffer);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  buffer[total_read] = '\0';
  fclose(file);

  const GameState state = deserialize(buffer);
  free(buffer);
  return state;
}
