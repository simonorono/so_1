cmake_minimum_required (VERSION 2.8)

project (so_1)

set (CMAKE_CXX_FLAGS "-O3 -Os")

add_executable (servidor servidor.c)

add_executable (jugador_x jugador_x.c)

add_executable (jugador_o jugador_o.c)
