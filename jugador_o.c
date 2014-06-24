#include "lib_sem.h"
#include "proyecto.h"

void jugada(int x, int y, tablero* juego);

int main(void) {

    tablero *juego;
    int terminado = 0;

    int shsem_id = abrir_sem(KEY, 2);
    if (shsem_id == -1) {
        printf("Error shsem_id\n");
        exit(-1);
    }
    else {
        printf("shsem_id: %d\n", shsem_id);
    }

    int shmem_id = shmget(KEY, sizeof(tablero), 0666);
    if (shmem_id == -1) {
        printf("Error shmem_id\n");
        exit(-1);
    }

    juego = (tablero *)shmat(shmem_id, 0, 0);

    int i = 1;
    int x,y;
    while (!terminado) {
        wait_sem(shsem_id, sem_o);

        printf("Turno %d de O:\n", i);
        printf("Ingrese la posicion de su jugada: ");
        fflush(stdin);
        scanf("%d,%d", &x, &y);
        jugada(x, y, juego);

        signal_sem(shsem_id, sem_x);
    }
}

void jugada(int x, int y, tablero* juego) {
    int actual = juego->valores[x-1][y-1];
    //Verifica que la jugada sea valida.
    if (actual == -1) {
        juego->valores[x - 1][y - 1] = 1;
    }
}
