#include "proyecto.h"

int main()
{

    tablero * tab1, tab2;

    //Crear semaforo jugador1, jugador2, monitor e incializar en 0
    int semid = abrir_sem(SEMAFOROS, 3);
    if(semid == -1){
        printf("Error al crear los semaforos\n");
        return -1;
    }

    ushort valores = {0,0,0};
    init_todos(semid, valores);

    //Vincular a las matrices de los jugadores
    int shMem1 = shmget(PLAYER1, sizeof(int)*9, 0666);
    if(shMem1 == -1){
        prinft("Error shMem1\n");
        return -1;
    }

    tab1 = (tablero*) shmat(shMem1, 0, 0);


    int shMem2 = shmget(PLAYER2, sizeof(int)*9, 0666);
    if(shMem2 == -1){
        prinft("Error shMem1\n");
        return -1;
    }

    tab2 = (tablero*) shmat(shMem2, 0,0);



    return 0;
}
