#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_state.h"
#include "select.h"
#include "print.h"
#include "save.h"

// #include <sys/ioctl.h>
// #include <unistd.h>

//
// void move_cursor(int row, int col) {
//     printf("\033[%d;%dH", row, col);
//     fflush(stdout);
// }
//
// int get_terminal_size(int* rows, int* cols) {
//     struct winsize w;
//     if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) return -1;
//     *rows = w.ws_row;
//     *cols = w.ws_col;
//     return 0;
// }
//
// void center_cursor() {
//     int rows, cols;
//     if (get_terminal_size(&rows, &cols) == -1) {
//         perror("Failed to get terminal size");
//         return;
//     }
//
//     int center_row = rows / 2;
//     int center_col = (cols - (cols / 3)) / 2;
//
//     move_cursor(center_row, center_col);
//
//     print_effect("", 20);
//     fflush(stdout);
// }

void test() {
    printf("test\n");       // Line 1
    printf("test");         // Line 2 (cursor is here)
    fflush(stdout);

    sleep_ms(1000);         // Pause to see output

    // Move up at the start of line 1
    printf("\033[A\r");
    printf("line 1 replaced\n");
    fflush(stdout);

    // Overwrite line 2
    printf("line 2 replaced\n");
    fflush(stdout);

    exit(0);
}

int main(void) {
    srand(time(0));
    print_effect("=== Bienvenue dans le jeu ===  ", 50);
    sleep_ms(500);
    erase_effect("=== Bienvenue dans le jeu ===  ", 25);
    sleep_ms(500);

    // print_effect("=== Appuyez sur <Entrer> pour continuer ===  ", 50);
    //
    // while (getchar() != '\n') {
    //     // Attendre que l'utilisateur appuie sur une touche
    // }
    //
    // erase_effect("\n=== Appuyez sur une touche pour continuer ===  ", 25);

    const Option option = select_option();
    clear_screen();

    GameState game_state;

    switch (option) {
        case Start: {
            const uint8_t mode = select_mode();
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
            printf("Partie chargée!\n");
            break;
        }
        case Leave:
            printf("Bye\n");
            return 0;
    }

    printf("Game state initialized\n");

    free_game_state(&game_state);
    return 0;
}