#include "proyecto.h"

int main()
{
    //Crear/Abrir semaforo
    int semid = abrir_sem(SEMAFOROS, 3);
    if(semid == -1)
    {
        printf("\nError al crear el semaforo\n");
        return -1;
    }

    int msgQ = msgget(LLAVEMSG, IPC_CREAT | 0666);
    if(msgQ == -1)
    {
        printf("\nError al abrir la cola de mensajes\n");
        return -1;
    }

    tablero* tab;
    respuesta resp;
    resp.tipo = 2;
    pregunta preg;
    preg.tipo = 1;
    int i, j;
    int turno = 0;
    int puntuacion = 0;
    int x,y;

    //Compartir el tablero
    int tabMemID = shmget(PLAYER1, sizeof(tablero), IPC_CREAT | 0666);
    if(tabMemID == -1)
    {
        printf("\nError al crear el tablero\n");
        return -1;
    }
    tab = (tablero*) shmat(tabMemID, 0, 0);


    for(i = 0;i < 3;i++)
        for(j = 0; j < 3; j++)
            tab->valores[i][j] = 0;

    //Llenar tabla con 1, 2 y 3
    i = 0;
    while(i < 3)
    {
        printf("\nSelecciona la posición del %d: ",i+1);
        scanf("%d,%d",&x, &y);

        if((x > 2) || (y > 2))
        {
            printf("\nPosiciones invalidas (entre 0 y 2");
            continue;
        }

        if(tab->valores[x][y] != 0)
        {
            printf("\nYa hay un numero ahi\n");
            continue;
        }
        else
        {
            tab->valores[x][y] = i+1;
            i++;
        }
    }

    shmdt(tab);

//    printf("Esperando que el Jugador 2 llene su tablero...");
//    signal_sem(semid, SEM_P2);
//    wait_sem(semid, SEM_P1);

    printf("\nEsperando al monitor");
    wait_sem(semid, SEM_P1);

    while(turno < 3)
    {


        /*Modo Activo*/
        printf("\n=================================");
        printf("\n\nTurno %d\n\n", turno+1);
        printf("Escribe la posicion a atacar (x,y): ");
        scanf("%d,%d",&x,&y);


        preg.x = x;
        preg.y = y;

        msgsnd(msgQ, &preg, sizeof(pregunta) - sizeof(long), IPC_NOWAIT);
//        printf("\nEsperando respuesta\n");
        msgrcv(msgQ, &resp, sizeof(respuesta) - sizeof(long), 2, 0);
//        printf("\nRespuesta recibida");

        printf("\nPuntos obtenidos: %d\n\n", resp.valor);
        puntuacion += resp.valor;

        turno++;


        /*Modo Pasivo*/
        printf("Espera tu turno...\n");
        msgrcv(msgQ, &preg, sizeof(pregunta) - sizeof(long), 1, 0);
//        printf("\nPregunta recibida");

        tab = (tablero*) shmat(tabMemID, 0, 0);
        int val = tab->valores[preg.x][preg.y];
        tab->valores[preg.x][preg.y] = -1;
        shmdt(tab);

//        printf("\nSignal SEM_MON");
        signal_sem(semid, SEM_MON);
//        printf("\nWait SEM_P1");
        wait_sem(semid, SEM_P1);
        resp.valor = val;
//        printf("\nEnviando respuesta");
        msgsnd(msgQ, &resp, sizeof(respuesta) - sizeof(long), 0);
//        printf("\nRespuesta enviada");

    }

    printf("\n\nJuego terminado, puntuacion total: %d\n", puntuacion);

    return 0;
}
