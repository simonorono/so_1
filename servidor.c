#include "proyecto.h"

int verificar_tablero(tablero* tab)
{
    int full = 1;
    int i;
    int column[3]; //Primera columna
    for (i = 0; i < 3; i++) column[i] = tab->valores[0][i]; //Deep Copy
    int diagonal, cdiagonal;
    diagonal = tab->valores[0][0]; //Empiezan las diagonales
    cdiagonal = tab->valores[0][2];

    //X = 0
    //O = 1

    for (i = 0; i < 3; i++)
    {
    if ((tab->valores[i][0] == tab->valores[i][1]) &&
            (tab->valores[i][0] == tab->valores[i][2]) &&
            (tab->valores[i][0] != -1))
            return tab->valores[i][0]; //Si los 3 valores son iguales y distintos de -1 en una fila hay ganador
        else
        {
            if ((tab->valores[i][0] == -1) ||
                (tab->valores[i][1] == -1) ||
                (tab->valores[i][2] == -1))
                full = 0; //Se encontró un -1 => Se puede continuar

            //Si no es la primera vez se pueden verificar las diagonales
            if (i != 0)
            {
                diagonal = (diagonal == tab->valores[i][i]) ? diagonal : -1; //Si en las lineas diagonales el valor cambia, ya no hay ganador diagonal
                cdiagonal = (cdiagonal == tab->valores[i][2 - i]) ? cdiagonal : -1;

                //Una columna se vuelve -1 si ya se perdió la victoria
                if (column[0] != tab->valores[i][0]) column[0] = -1;
                if (column[1] != tab->valores[i][1]) column[1] = -1;
                if (column[2] != tab->valores[i][2]) column[2] = -1;
            }
        }
    }

    //Buscar ganador, si alguna variable NO es -1 entonces hay ganador, y si no hay espacios en la matriz con -1 entonces esta full => Hay empate
    if (diagonal != -1) return diagonal;
    else if (cdiagonal != -1) return cdiagonal;
    else
    {
        for (i = 0; i < 3; i++)
        {
            if (column[i] != -1) return column[i];
        }
    }

    if (full)
        return 2;
    else
        return -1;
}

void print_tablero(tablero* buff) {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++)
            printf("[%d]", buff->valores[i][j]);
        printf("\n");
    }
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
        printf("Error shmmsg_id\n");
        exit(-1);
    }

    buffer = (tablero *)shmat(shmem_id, 0, 0);
    /*
     * Se inicializa todo en -1.
     * Una jugada de X se llena con 0.
     * Una jugada de Y se llena con 1.
     */
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            buffer->valores[i][j] = -1;

    i = 1;
    while (!terminado) {
        msgrcv(shmsg_id, (void *)&msg, sizeof(mensaje), 1, 0);

        int estado = verificar_tablero(buffer);

        if (estado != -1)
            terminado = 1;
        printf("Estado: %d\n", estado);

        printf("Turno: %d\nTerminado: %d\n", i, terminado);
        print_tablero(buffer);
        i++;

        (&msg)->tipo = 2;
        (&msg)->msg = estado;
        msgsnd(shmsg_id, (void *)&msg, msgsize, 0);

        if (terminado) {
            msgsnd(shmsg_id, (void *)&msg, msgsize, 0);
        }
    }

    printf("Partida terminada\n");
    shmctl(shmem_id, IPC_RMID, 0);

    return 0;
}

