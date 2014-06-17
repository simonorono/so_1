#include "lib_sem.h"
#include "proyecto.h"

int main(void)
{
    tablero *juego;

    int shsem_id = abrir_sem(KEY, 1);
    if (shsem_id == -1) {
        printf("Error shsem_id\n");
        exit(-1);
    }
    init_sem(shsem_id, 0, 0);

    int shmem_id = shmget(KEY, sizeof(tablero), 0666);
    if (shmem_id == -1) {
        printf("Error shmem_id\n");
        exit(-1);
    }

    juego = (tablero *)shmat(shmem_id, 0, 0);
}

