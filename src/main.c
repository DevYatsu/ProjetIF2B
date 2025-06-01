#include "game_state.h"
#include "piece.h"
#include "print.h"
#include "save.h"
#include "select.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_bye() {
  // cf https://emojicombos.com/bye-ascii-art
  puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡤⠒⠒⠢⢄⡀⠀⠀⢠⡏⠉⠉⠉⠑⠒⠤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
  puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡞⠀⠀⠀⠀⠀⠙⢦⠀⡇⡇⠀⠀⠀⠀⠀⠀⠈⠱⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
  puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠊⠉⠉⠙⠒⢤⡀⠀⣼⠀⠀⢀⣶⣤⠀⠀⠀⢣⡇⡇⠀⠀⢴⣶⣦⠀⠀⠀⢳⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
  puts("⠀⠀⠀⢀⣠⠤⢄⠀⠀⢰⡇⠀⠀⣠⣀⠀⠀⠈⢦⡿⡀⠀⠈⡟⣟⡇⠀⠀⢸⡇⡆⠀⠀⡼⢻⣠⠀⠀⠀⣸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
  puts("⠀⢀⠖⠉⠀⠀⠀⣱⡀⡞⡇⠀⠀⣿⣿⢣⠀⠀⠈⣧⣣⠀⠀⠉⠋⠀⠀⠀⣸⡇⠇⠀⠀⠈⠉⠀⠀⠀⢀⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
  puts("⣠⠏⠀⠀⣴⢴⣿⣿⠗⢷⡹⡀⠀⠘⠾⠾⠀⠀⠀⣿⣿⣧⡀⠀⠀⠀⢀⣴⠇⣇⣆⣀⢀⣀⣀⣀⣀⣤⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
  puts("⣿⠀⠀⢸⢻⡞⠋⠀⠀⠀⢿⣷⣄⠀⠀⠀⠀⠀⣠⡇⠙⢿⣽⣷⣶⣶⣿⠋⢰⣿⣿⣿⣿⣿⣿⠿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
  puts("⡿⡄⠀⠈⢻⣝⣶⣶⠀⠀⠀⣿⣿⣱⣶⣶⣶⣾⡟⠀⠀⠀⢈⡉⠉⢩⡖⠒⠈⠉⡏⡴⡏⠉⠉⠉⠉⠉⠉⠉⠉⡇⠀⠀⢀⣴⠒⠢⠤⣀");
  puts("⢣⣸⣆⡀⠀⠈⠉⠁⠀⠀⣠⣷⠈⠙⠛⠛⠛⠉⢀⣴⡊⠉⠁⠈⢢⣿⠀⠀⠀⢸⠡⠀⠁⠀⠀⠀⣠⣀⣀⣀⣀⡇⠀⢰⢁⡇⠀⠀⠀⢠");
  puts("⠀⠻⣿⣟⢦⣤⡤⣤⣴⣾⡿⢃⡠⠔⠒⠉⠛⠢⣾⢿⣿⣦⡀⠀⠀⠉⠀⠀⢀⡇⢸⠀⠀⠀⠀⠀⠿⠿⠿⣿⡟⠀⢀⠇⢸⠀⠀⠀⠀⠘");
  puts("⠀⠀⠈⠙⠛⠿⠿⠿⠛⠋⢰⡋⠀⠀⢠⣤⡄⠀⠈⡆⠙⢿⣿⣦⣀⠀⠀⠀⣜⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⢀⠃⠀⡸⠀⠇⠀⠀⠀⠀⠀");
  puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⢣⠀⠀⠈⠛⠁⠀⢴⠥⡀⠀⠙⢿⡿⡆⠀⠀⢸⠀⢸⢰⠀⠀⠀⢀⣿⣶⣶⡾⠀⢀⠇⣸⠀⠀⠀⠀⠀⠀");
  puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⡀⢇⠀⠀⠀⢀⡀⠀⠀⠈⢢⠀⠀⢃⢱⠀⠀⠀⡇⢸⢸⠀⠀⠀⠈⠉⠉⠉⢱⠀⠼⣾⣿⣿⣷⣦⠴⠀⠀");
  puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢱⠘⡄⠀⠀⢹⣿⡇⠀⠀⠈⡆⠀⢸⠈⡇⢀⣀⣵⢨⣸⣦⣤⣤⣄⣀⣀⣀⡞⠀⣠⡞⠉⠈⠉⢣⡀⠀⠀");
  puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢃⠘⡄⠀⠀⠉⠀⠀⣠⣾⠁⠀⠀⣧⣿⣿⡿⠃⠸⠿⣿⣿⣿⣿⣿⣿⠟⠁⣼⣾⠀⠀⠀⠀⢠⠇⠀⠀");
  puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡄⠹⣀⣀⣤⣶⣿⡿⠃⠀⠀⠀⠈⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠁⠀⠀⢻⣿⣷⣦⣤⣤⠎⠀⠀⠀");
  puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣤⣿⡿⠟⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠀⠀⠀⠀⠀");
}

void print_title_screen() {
  // cf https://ascii.co.uk/art/chess
  clear_screen();

  print_text("     | |\n");
  print_text("  ___| |__   ___  ___ ___\n");
  print_text(" / __| '_ \\ / _ \\/ __/ __|\n");
  print_text("| (__| | | |  __/\\__ \\__ \\\n");
  print_text(" \\___|_| |_|\\___||___/___/\n\n\n\n\n");

  print_text("\n⚠️  Ce jeu est conçu pour être joué dans un vrai terminal.\n");
  print_text("   Évitez d'utiliser la sortie CLion / IDE (Run) pour un "
             "affichage correct.\n\n\n\n\n");

  print_text("Appuyez sur <Entrer> pour commencer la partie.\n\n\n\n\n");

  while (getchar() != '\n') {
    // Attendre que l'utilisateur appuie sur une touche
  }

  clear_screen();
}

void play_conquest_turn(const GameState *game_state) {
  Tile tile = select_valid_tile(game_state);
  const TargetPosition pos = select_valid_target_position(game_state);

  tile.captured_by = player_option(game_state->is_turn_of);
  game_state->board.tiles[pos.y][pos.x] = tile;
  apply_conquest_capture(game_state, pos.x, pos.y, tile.value, game_state->is_turn_of);
}

void play_connect_turn(GameState *game_state) {
  Tile tile = select_valid_tile_for_connect(game_state);
  const TargetPosition pos = select_valid_target_position_for_connect(game_state, &tile);

  tile.captured_by = player_option(game_state->is_turn_of);
  game_state->board.tiles[pos.y][pos.x] = tile;
  apply_conquest_capture(game_state, pos.x, pos.y, tile.value, game_state->is_turn_of);

  // La partie se termine dès qu'un joueur pose son roi
  if (tile.value.kind == King) {
    printf("Le roi a été placé par le joueur %s. La partie est terminée !\n",
           stringify_player(game_state->is_turn_of));
    set_all_to_zero(&game_state->piece_counter_1);
    set_all_to_zero(&game_state->piece_counter_2);
  }
}

// questions pour le prof
// est-ce que les deux joueurs sont humains ou bien l'un est un bot ?
int main(void) {
  srand(time(0));
  // on skip pr le moment c'est trop long
  // print_title_screen();

  // print_effect("=== Appuyez sur <Entrer> pour continuer ===  ", 50);
  //
  // while (getchar() != '\n') {
  //     // Attendre que l'utilisateur appuie sur une touche
  // }
  //
  // erase_effect("\n=== Appuyez sur une touche pour continuer ===  ", 25);

  const StartOption option = select_option();
  clear_screen();

  GameState game_state;

  // 1 fonction == 1 fonctionnalité

  switch (option) {
  case Start: {
    const GameMode mode = select_mode();
    clear_screen();

    const uint8_t dim = select_dimension();
    clear_screen();

    game_state = init_game_state(mode, dim);
    break;
  }
  case Restart: {
    if (!save_file_exists()) {
      print_effect("Aucune partie sauvegardée trouvée.\n", 50);
      return 0;
    }

    printf("Chargement de la partie...\n");
    game_state = load_game();
    sleep_ms(750);
    clear_screen();
    print_text("Partie chargée!\n");
    break;
  }
  case Leave:
    print_bye();
    return 0;
  default:
    // unreachable
    return 1;
  }

  if (game_state.is_white == User) {
    print_text("Vous êtes les blancs!\n");
  } else {
    print_text("Vous êtes les noirs!\n");
  }

  sleep_ms(500);
  clear_screen();
  sleep_ms(200);

  bool game_stopped = false;

  while (!game_stopped &&
         !has_no_pieces_left(get_user_turn_count_tracker(&game_state))) {
    // pour centrer le titre
    for (int i = 0; i < game_state.board.dim / 2 + 1; i++) {
      printf("   ");
    }
    printf("Au tour du joueur %d\n", game_state.is_turn_of == User ? 1 : 2);

    print_board(&game_state);

    const RoundOption round_option = select_round_option();

    switch (round_option) {
    case Play: {
      printf("Vous avez choisi de jouer.\n");

      switch (game_state.mode) {
      case Conquest: {
        play_conquest_turn(&game_state);
        break;
      }
      case Connect: {
        play_connect_turn(&game_state);
        break;
      }
      default:
        // unreachable
        return 1;
      }

      game_state.is_turn_of = (game_state.is_turn_of == User) ? Opponent : User;
      clear_screen();
      break;
    }
    case GiveUp: {
      printf("Le joueur %s abandonne la partie. Partie terminée!\n",
             get_user_turn_name(&game_state));
      game_stopped = true;
      break;
    }
    case SaveGame: {
      const bool success = save_game(&game_state);

      if (!success) {
        printf("Une erreur est survenue, nous n'avons pas pu sauvegarder la "
               "partie\n");
      }
      clear_screen();
      print_text("Sauvegarde en cours...");
      sleep_ms(500);
      print_text("La partie a été sauvegardée avec succès.\n");
      sleep_ms(400);
      game_stopped = true;
      break;
    }
    }
  }

  if (game_stopped) {
    clear_screen();
    print_bye();
    return 0;
  }

  const PieceCountTracker *current_tracker =
      get_user_turn_count_tracker(&game_state);
  if (has_no_pieces_left(current_tracker)) {
    clear_screen();
    sleep_ms(500);
    print_text("Toutes les pièces ont été jouées.\n");
    print_text("La partie est terminée!\n");
    const uint8_t user_count = get_captured_count_of(&game_state, User);
    const uint8_t opponent_count = get_captured_count_of(&game_state, Opponent);

    if (user_count > opponent_count) {
      printf("Victoire de l'utilisateur ! (%d cases contre %d)\n", user_count,
             opponent_count);
    } else if (opponent_count > user_count) {
      printf("Victoire de l'adversaire ! (%d cases contre %d)\n",
             opponent_count, user_count);
    } else {
      printf("Égalité parfaite : %d cases chacun !\n", user_count);
    }
  }

  free_game_state(&game_state);
  return 0;
}