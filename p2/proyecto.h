#ifndef PROYECTO_H
#define PROYECTO_H

#include "lib_sem.h"
#include <sys/shm.h>
#include <sys/msg.h>
#include <stdio.h>

#define SEMAFOROS 777
#define PLAYER1 778
#define PLAYER2 779
#define SEM_MON 2
#define SEM_P1  0
#define SEM_P2  1
#define LLAVEMSG 780

typedef struct _tablero
{
    int valores[3][3];
} tablero;

typedef struct _pregunta
{
    long tipo;
    int x;
    int y;
} pregunta;

typedef struct _respuesta
{
    long tipo;
    int valor;
} respuesta;

#endif // PROYECTO_H
