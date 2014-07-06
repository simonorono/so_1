#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define KEY 15658734
#define sem_x 0
#define sem_o 1

typedef struct {
    int valores[3][3];
} tablero;

typedef struct {
    long tipo;
    int msg;
} mensaje;

int msgsize = (sizeof(mensaje) - sizeof(long));

#endif // PROJECT_H
