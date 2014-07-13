#include "proyecto.h"

void impr_tableros(int IDtab1, int IDtab2)
{
    int i,j;
    tablero *tab1, *tab2;
    tab1 = (tablero*) shmat(IDtab1, 0,0);
    tab2 = (tablero*) shmat(IDtab2, 0,0);

    printf("\n\n\tPlayer 1\t\t\t\tPlayer 2\n\n");

    for(i = 0; i < 3; i++)
    {
        printf("\n");
        for(j = 0; j < 3; j++)
        {
            if(tab1->valores[i][j] < 0)
                printf("[");
            else printf("[ ");

            printf("%d ]\t", tab1->valores[i][j]);
        }
        printf("\t\t");

        for(j = 0; j < 3; j++)
        {
            if(tab2->valores[i][j] < 0)
                printf("[");
            else printf("[ ");

            printf("%d ]\t", tab2->valores[i][j]);
        }
    }

    shmdt(tab1);
    shmdt(tab2);
}


int main()
{
    int turno = 0;
    int terminado = 0;
    tablero * tab1, * tab2;
    int msgQid = msgget(LLAVEMSG, IPC_CREAT | 0666);
    if(msgQid == -1){
        printf("Error al crear la cola de mensajes\n");
        return -1;
    }

    //Crear semaforo jugador1, jugador2, monitor e incializar en 0
    int semid = abrir_sem(SEMAFOROS, 3);
    if(semid == -1){
        printf("Error al crear los semaforos\n");
        return -1;
    }

    ushort valores[] = {0,0,0};
    init_todos(semid, valores);
    printf("\nSemaforos creados");

    //Vincular a las matrices de los jugadores
    int shMem1 = shmget(PLAYER1, sizeof(tablero), IPC_CREAT | 0666);
    if(shMem1 == -1){
        printf("Error shMem1\n");
        return -1;
    }
    printf("\nConectado a memoria1");

    int shMem2 = shmget(PLAYER2, sizeof(tablero), IPC_CREAT | 0666);
    if(shMem2 == -1){
        printf("Error shMem1\n");
        return -1;
    }
    printf("\nConectado a memoria2");

    //    printf("\nHaciendo signal a SEM_P1");
    signal_sem(semid, SEM_P1);
    //    printf("\nEntrando en el bucle");


    while(1)
    {
        printf("\nEsperando");
        wait_sem(semid, SEM_MON);


        impr_tableros(shMem1, shMem2);

        turno++;

        if(turno >= 6) break;

        // El turno dice a quien enviar el signal
        // (al que no tiene el turno)
        if(turno%2 == 0)
        {
            //            printf("\nSignal SEM_P1");
            signal_sem(semid, SEM_P1);
        }
        else
        {
            //            printf("\nSignal SEM_P2");
            signal_sem(semid, SEM_P2);
        }
    }

    printf("\nJuego terminado\n");
    //Limpiar
    msgctl(msgQid, IPC_RMID, NULL);
    borra_sem(semid);
    shmdt(tab1);
    shmdt(tab2);
    shmctl(shMem1, IPC_RMID, NULL);
    shmctl(shMem2, IPC_RMID, NULL);

    return 0;
}
