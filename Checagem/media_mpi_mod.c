#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            printf("Execute com pelo menos 2 processos.\n");
        }
        MPI_Finalize();
        return 0;
    }

    // Seed para números aleatórios
    srand(time(NULL) + rank);

    // Gerar dados locais (5 números por processo)
    int local_data[5];
    for (int i = 0; i < 5; i++) {
        local_data[i] = rand() % 100; // Números entre 0 e 99
    }

    // Calcular média local
    double local_sum = 0;
    for (int i = 0; i < 5; i++) {
        local_sum += local_data[i];
    }
    double local_avg = local_sum / 5;

    // Encontrar maior valor local
    int local_max = local_data[0];
    for (int i = 1; i < 5; i++) {
        if (local_data[i] > local_max) {
            local_max = local_data[i];
        }
    }

    // Arrays para coletar médias e maiores valores no processo 0
    double* all_avgs = NULL;
    int* all_maxes = NULL;
    if (rank == 0) {
        all_avgs = (double*)malloc(size * sizeof(double));
        all_maxes = (int*)malloc(size * sizeof(int));
    }

    // Coletar médias usando MPI_Gather
    MPI_Gather(&local_avg, 1, MPI_DOUBLE, all_avgs, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Coletar maiores valores usando MPI_Gather
    MPI_Gather(&local_max, 1, MPI_INT, all_maxes, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Imprimir resultados no processo 0
    if (rank == 0) {
        printf("Resultados coletados no processo 0:\n");
        for (int i = 0; i < size; i++) {
            printf("Processo %d: Média = %.2f, Maior valor = %d\n", i, all_avgs[i], all_maxes[i]);
        }
        free(all_avgs);
        free(all_maxes);
    }

    MPI_Finalize();
    return 0;
}
