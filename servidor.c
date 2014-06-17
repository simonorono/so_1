#include "proyecto.h"

int verificar_tablero(tablero* tab)
{
    int full = 1;
    int i;
    int* column = tab->valores[0]; //Primera columna
    int diagonal, cdiagonal;
    diagonal = tab->valores[0][0]; //Empiezan las diagonales
    cdiagonal = tab->valores[0][2];

    for(i = 0; i < 3; i++)
    {
        if((tab->valores[i][0] == tab->valores[i][1]) &&
                (tab->valores[i][0] == tab->valores[i][2]) &&
                (tab->valores[i][0] != -1))
            return tab->valores[i][0]; //Detectar linea horizontal
        else
        {
            if((tab->valores[i][0] == -1) ||
                    (tab->valores[i][1] == -1) ||
                    (tab->valores[i][2] == -1))
                full = 0; //Se puede continuar

            if(i != 0)
            {
                diagonal = (diagonal == tab->valores[i][i]) ? diagonal : -1; //Verificar diagonales
                cdiagonal = (cdiagonal == tab->valores[i][2-i]) ? cdiagonal : -1;

                //Una columna se vuelve -1 si ya se perdió la victoria
                if(column[0] != tab->valores[i][0]) column[0] = -1;
                if(column[1] != tab->valores[i][1]) column[1] = -1;
                if(column[2] != tab->valores[i][2]) column[2] = -1;
            }
        }
    }

    //Buscar ganador
    if(diagonal != -1) return diagonal;
    else if(cdiagonal != -1) return cdiagonal;
    else
    {
        for(i = 0; i < 3; i++)
        {
            if(column[i] != -1) return column[i];
        }
    }

    if(full)
        return 2;
    else
        return -1;
}

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

