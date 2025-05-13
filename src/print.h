
#ifndef PRINT_H
#define PRINT_H

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(x) Sleep(x)
#else
#include <unistd.h>
#define sleep_ms(x) usleep(x * 1000)
#endif

void print_effect(const char* str, const unsigned int delay_ms);
void print_text(const char* str);
void erase_effect(const char* str, const unsigned int delay_ms);
void clear_screen();

#endif //PRINT_H
