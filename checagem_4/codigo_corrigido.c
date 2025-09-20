#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N 5
enum { THINKING, HUNGRY, EATING };

int estado;

int esquerda(int rank) { return (rank + N - 1) % N; }
int direita(int rank) { return (rank + 1) % N; }

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != N) {
        if (rank == 0) printf("Execute com exatamente %d processos.\n", N);
        MPI_Finalize();
        return 0;
    }

    srand(time(NULL) + rank);
    estado = THINKING;

    double tempo_inicio_espera = 0.0;
    double tempo_total_espera = 0.0;
    int vezes_comendo = 0;

    for (int step = 0; step < 10; step++) {
        printf("Filósofo %d pensando.\n", rank);
        sleep(rand() % 2);

        estado = HUNGRY;
        printf("Filósofo %d com fome.\n", rank);
        tempo_inicio_espera = MPI_Wtime();  // Marca início da espera

        // Comunicação não bloqueante
        int estado_esq = -1, estado_dir = -1;
        MPI_Request requests[4];

        // Envia estado para vizinhos
        MPI_Isend(&estado, 1, MPI_INT, esquerda(rank), 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Isend(&estado, 1, MPI_INT, direita(rank), 0, MPI_COMM_WORLD, &requests[1]);

        // Recebe estado dos vizinhos
        MPI_Irecv(&estado_esq, 1, MPI_INT, esquerda(rank), 0, MPI_COMM_WORLD, &requests[2]);
        MPI_Irecv(&estado_dir, 1, MPI_INT, direita(rank), 0, MPI_COMM_WORLD, &requests[3]);

        // Espera até todas as comunicações terminarem
        MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);

        if (estado_esq != EATING && estado_dir != EATING) {
            estado = EATING;
            double tempo_fim_espera = MPI_Wtime();
            tempo_total_espera += (tempo_fim_espera - tempo_inicio_espera);
            vezes_comendo++;

            printf("Filósofo %d comendo.\n", rank);
            sleep(rand() % 2);
        } else {
            printf("Filósofo %d esperando.\n", rank);
        }

        estado = THINKING;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    // Cada filósofo reporta seu tempo médio de espera
    if (vezes_comendo > 0) {
        double media = tempo_total_espera / vezes_comendo;
        printf("Filósofo %d: tempo médio de espera = %.6f segundos em %d refeições.\n",
               rank, media, vezes_comendo);
    }

    MPI_Finalize();
    return 0;
}
