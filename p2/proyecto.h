#ifndef PROYECTO_H
#define PROYECTO_H

#include "lib_sem.h"
#include <sys/shm.h>
#define SEMAFOROS 777
#define PLAYER1 778
#define PLAYER2 779

typedef struct _tablero
{
    int valores[3][3];
} tablero;


#endif // PROYECTO_H
