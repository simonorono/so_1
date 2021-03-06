#include "lib_sem.h"
#include "proyecto.h"

int jugada(int x, int y, tablero* juego);

int main(void)
{
    tablero *juego;
    mensaje msg;

    int terminado = 0;

    int shsem_id = abrir_sem(KEY, 2);
    if (shsem_id == -1) {
        printf("Error shsem_id\n");
        exit(-1);
    }

    int shmsg_id = msgget(KEY, IPC_CREAT | 0666);
    if (shmsg_id == -1) {
        printf("Error shmmsg_id\n");
        exit(-1);
    }

    init_sem(shsem_id, sem_x, 1);
    init_sem(shsem_id, sem_o, 0);

    int shmem_id = shmget(KEY, sizeof(tablero), IPC_CREAT | 0666);
    if (shmem_id == -1) {
        printf("Error shmem_id\n");
        exit(-1);
    }

    juego = (tablero *)shmat(shmem_id, 0, 0);

    int i = 1;
    int x,y;
    while (!terminado) {
        wait_sem(shsem_id, sem_x);

        int rv = msgrcv(shmsg_id, &msg, msgsize, 2, IPC_NOWAIT);
        if (rv != -1) {
            if (msg.msg == 2) {
                printf("Empate\n");
                break;
            }
            else {
                printf("Gano O\n");
                break;
            }
            msgctl(shmsg_id, IPC_RMID, 0);
        }

        printf("Turno %d de X:\n", i);
        printf("Ingrese la posicion de su jugada: ");
        fflush(stdin);
        scanf("%d,%d", &x, &y);

        if (jugada(x, y, juego)) {
            msg.tipo = 1;
            msg.msg = 0;
            msgsnd(shmsg_id, &msg, msgsize, 0);

            while (msgrcv(shmsg_id, &msg, msgsize, 2, IPC_NOWAIT) == -1) {
                sleep(1);
            }

            if (msg.msg != -1) {
                if (msg.msg == 2) {
                    printf("Empate.\n");
                }
                else {
                    printf("Gano X.\n");
                }
                terminado = 1;
            }
        }

        i++;

        signal_sem(shsem_id, sem_o);
    }
    borra_sem(shsem_id);
}

int jugada(int x, int y, tablero* juego) {
    x = x-1;
    y = y-1;

    if (x > 2 || y > 2 || x < 0 || y < 0) {
        return 0;
    }

    int actual = juego->valores[x][y];
    printf("[%d][%d] Actual: %d\n", x, y, actual);
    //Verifica que la jugada sea valida.
    if (actual == -1) {
        juego->valores[x][y] = 0;
        return 1;
    }
    else
        return 0;
}
