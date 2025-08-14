#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    int num_points = 10000000;
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) * rank);

    int local_points = num_points / size;
    int local_inside = 0;

    for (int i = 0; i < local_points; i++) {
        double x = (double) rand() / RAND_MAX;
        double y = (double) rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            local_inside++;
        }
    }

    // Modificação: Cada processo imprime seus valores locais
    printf("Processo %d gerou %d pontos, %d dentro do círculo\n", rank, local_points, local_inside);

    int global_inside;
    MPI_Reduce(&local_inside, &global_inside, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = 4.0 * global_inside / num_points;
        printf("Valor estimado de Pi: %f\n", pi);
    }

    MPI_Finalize();
    return 0;
}
