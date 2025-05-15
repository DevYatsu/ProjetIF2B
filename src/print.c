#include "print.h"
#include <stdio.h>

void print_effect(const char* str, const unsigned int delay_ms) {
    for (const char* c = str; *c != '\0'; ++c) {
        putchar(*c);
        fflush(stdout);  // fais apparaitre directement le char
        sleep_ms(delay_ms);
    }
}

void print_text(const char* str) {
    print_effect(str, 15);
}

void erase_effect(const char* str, const unsigned int delay_ms) {
    for (int i = 0; str[i]; ++i) {
        printf("\b \b");
        fflush(stdout);
        sleep_ms(delay_ms);
    }
}

#include <stdlib.h>

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    //     system("@cls||clear");
}

