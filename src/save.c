
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game_state.h"
#include "piece.h"

const char* FILENAME = "savegame.dat";

const int MAX_GAME_STATE_STR_LEN = strlen("Conquest ") + // longueur du plus grand entre Conquest et Connect
                              strlen("User ") + // longueur du plus grand entre User et AI
                              3 + // longueur de dim (2 digits et  un espace)
                              (12 * 12 * 10) + // 12 (max dim) et 10 pour chaque piece
                              100; // securité

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
char* serialize(const GameState* state) {
    char* str = malloc(MAX_GAME_STATE_STR_LEN);
    if (!str) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    str[0] = '\0'; // Init buffer

    const char* mode_str = (state->mode == Conquest) ? "Conquest" : "Connect";
    snprintf(str, MAX_GAME_STATE_STR_LEN,
             "mode=%s\nwhite=%s\nturn=%s\ndim=%d\ntiles=",
             mode_str,
             stringify_player(state->is_white),
             stringify_player(state->is_turn_of),
             state->board.dim);

    for (uint8_t i = 0; i < state->board.dim; i++) {
        for (uint8_t j = 0; j < state->board.dim; j++) {
            strcat(str, " ");
            const Tile tile = state->board.tiles[i][j];
            if (!tile.some) {
                strcat(str, "_");
            } else {
                strcat(str, stringify_piece(tile.value.kind));
                strcat(str, ":");
                strcat(str, stringify_player(tile.value.player));
            }
        }
    }

    strcat(str, "\n");

    return str;
}

//> DESERIALIZE
// Error codes for better error handling
typedef enum {
    DESERIALIZE_SUCCESS = 0,
    DESERIALIZE_NULL_INPUT,
    DESERIALIZE_MEMORY_ERROR,
    DESERIALIZE_INVALID_FORMAT,
    DESERIALIZE_INVALID_DIMENSION,
    DESERIALIZE_INVALID_MODE,
    DESERIALIZE_INVALID_PLAYER,
    DESERIALIZE_MISSING_TILES
} DeserializeResult;

// Helper function to safely copy strings with bounds checking
static bool safe_string_copy(char* dest, const char* src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) return false;

    size_t src_len = strlen(src);
    if (src_len >= dest_size) return false;

    strcpy(dest, src);
    return true;
}

// Helper function to parse a field from a line
static bool parse_field(const char* line, const char* prefix, char* output, size_t output_size) {
    size_t prefix_len = strlen(prefix);
    if (strncmp(line, prefix, prefix_len) != 0) return false;

    return safe_string_copy(output, line + prefix_len, output_size);
}

// Helper function to convert string to game mode enum
static bool string_to_mode(const char* mode_str, GameMode* mode) {
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

// Helper function to convert string to player enum
static bool string_to_player(const char* player_str, Player* player) {
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

// Helper function to parse tiles from string
static DeserializeResult parse_tiles(const char* tiles_str, GameState* state, uint8_t dim) {
    const char* current = tiles_str;
    char piece_buf[32];

    for (uint8_t i = 0; i < dim; i++) {
        for (uint8_t j = 0; j < dim; j++) {
            // Skip whitespace
            while (*current == ' ' || *current == '\t') current++;

            // Read piece token
            int k = 0;
            while (*current && *current != ' ' && *current != '\t' &&
                   *current != '\n' && k < sizeof(piece_buf) - 1) {
                piece_buf[k++] = *current++;
            }
            piece_buf[k] = '\0';

            // Parse piece
            if (strcmp(piece_buf, "_") == 0) {
                state->board.tiles[i][j] = empty_tile();
            } else {
                char* separator = strchr(piece_buf, ':');
                if (!separator) {
                    return DESERIALIZE_INVALID_FORMAT;
                }

                *separator = '\0';
                char* piece_str = piece_buf;
                char* player_str = separator + 1;

                Tile tile = deserialize_piece(piece_str, player_str, true);
                state->board.tiles[i][j] = tile;
            }
        }
    }

    return DESERIALIZE_SUCCESS;
}

// Main deserialize function with improved error handling
DeserializeResult deserialize_safe(const char* str, GameState* state) {
    // Input validation
    if (!str || !state) {
        return DESERIALIZE_NULL_INPUT;
    }

    // Initialize state to safe defaults
    memset(state, 0, sizeof(GameState));

    // Create working copy of input string
    char* dup = strdup(str);
    if (!dup) {
        return DESERIALIZE_MEMORY_ERROR;
    }

    // Parsing variables
    char mode_str[16] = {0};
    char white_str[16] = {0};
    char turn_str[16] = {0};
    int dim = 0;
    int parsed_fields = 0;

    // Parse header fields line by line
    char* line = strtok(dup, "\n");
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
            break; // Found tiles section, stop parsing headers
        }
        line = strtok(NULL, "\n");
    }

    // Verify all required fields were found
    if (parsed_fields != 4) {
        free(dup);
        return DESERIALIZE_INVALID_FORMAT;
    }

    // Convert strings to enums with validation
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

    // Find and parse tiles section
    const char* tiles_line = strstr(str, "tiles=");
    if (!tiles_line) {
        free(dup);
        free_board(&state->board);
        return DESERIALIZE_MISSING_TILES;
    }

    tiles_line += 6; // Skip "tiles="
    DeserializeResult tiles_result = parse_tiles(tiles_line, state, (uint8_t)dim);

    free(dup);

    if (tiles_result != DESERIALIZE_SUCCESS) {
        free_board(&state->board);
        return tiles_result;
    }

    // Initialize piece counters (TODO: restore from serialized data)
    state->piece_counter_1 = init_piece_counter();
    state->piece_counter_2 = init_piece_counter();

    return DESERIALIZE_SUCCESS;
}

// Wrapper function that maintains original API but with better error handling
GameState deserialize(const char* str) {
    GameState state;
    const DeserializeResult result = deserialize_safe(str, &state);

    if (result != DESERIALIZE_SUCCESS) {
        // Messages d'erreur associés aux codes
        const char* error_messages[] = {
            "Succès",
            "L'entrée est NULL",
            "Échec de l'allocation mémoire",
            "Format invalide ou champs requis manquants",
            "Dimension invalide (doit être comprise entre 6 et 12)",
            "Mode de jeu invalide",
            "Spécification de joueur invalide",
            "Section des tuiles manquante"
        };

        fprintf(stderr, "Erreur de désérialisation : %s\n", error_messages[result]);
        exit(1);
    }

    return state;
}
//< DESERIALIZE

bool save_game(const GameState* state) {
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        perror("Échec de l'ouverture du fichier pour la sauvegarde");
        return false;
    }

    char* serialized = serialize(state);
    if (!serialized) {
        fprintf(stderr, "Erreur : la sérialisation a échoué\n");
        fclose(file);
        return false;
    }

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
    FILE* file = fopen(FILENAME, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}

GameState load_game() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier de sauvegarde");
        exit(EXIT_FAILURE);
    }

    char* buffer = malloc(MAX_GAME_STATE_STR_LEN);
    if (!buffer) {
        perror("Échec de l'allocation mémoire");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    const size_t total_read = fread(buffer, sizeof(char), MAX_GAME_STATE_STR_LEN - 1, file);
    if (ferror(file)) {
        perror("Erreur lors de la lecture du fichier de sauvegarde");
        free(buffer);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    buffer[total_read] = '\0';
    fclose(file);

    GameState state = deserialize(buffer);
    free(buffer);
    return state;
}
