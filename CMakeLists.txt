cmake_minimum_required(VERSION 3.30.5..3.31)
project(ProjetIF2B C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjetIF2B src/main.c
        src/select.c
        src/select.h
        src/player.c
        src/player.h
        src/save.c
        src/game_state.c
        src/game_state.h
        src/board.c
        src/board.h
        src/save.h
        src/print.c
        src/print.h
)
