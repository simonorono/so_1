#include "proyecto.h"

int main(void)
{
    mensaje msg;
    tablero *buffer;
    int i, j, terminado = 0;

    int shmem_id = shmget(KEY, sizeof(tablero), IPC_CREAT | 0666);
    if (shmem_id == -1) {
        printf("Error shmget\n");
        exit(-1);
    }

    int shmsg_id = msgget(KEY, IPC_CREAT | 0666);
    if (shmsg_id == -1) {
        printf("Error shmmsg\n");
        exit(-1);
    }

    buffer = (tablero *)shmat(shmem_id, 0, 0);
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            buffer->valores[i][j] = -1;

    while (!terminado) {
        msgrcv(shmsg_id, (void *)&msg, sizeof(mensaje), 1, 0);
        int estado = 1;//VERIFICAR TABLERO

        if (estado != -1)
            terminado = 1;

        (&msg)->tipo = 2;
        (&msg)->msg = estado;
        msgsnd(shmsg_id, (void *)&msg, msgsize, 0);

        if (terminado) {
            msgsnd(shmsg_id, (void *)&msg, msgsize, 0);
        }
    }

    printf("Partida terminada\n");
    return 0;
}

