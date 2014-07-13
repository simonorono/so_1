cmake_minimum_required(VERSION 2.8)

project(ProyectoSO1)

set (CMAKE_CXX_FLAGS "-03 -0s")

add_executable(monitor monitor.c)

add_executable(jugador1 jugador1.c)

add_executable(jugador2 jugador2.c)
