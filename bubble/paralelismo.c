#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Função Bubble Sort simples
void bubble_sort(int *array, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (array[j] > array[j+1]) {
                int tmp = array[j];
                array[j] = array[j+1];
                array[j+1] = tmp;
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, size, n=256; 
    int *data = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time = MPI_Wtime();

    int local_n = n / size;
    int *local_data = (int*)malloc(local_n * sizeof(int));


    if (rank == 0) {
        data = (int*)malloc(n * sizeof(int));
        printf("Vetor original:\n");
        for (int i = 0; i < n; i++) {
            data[i] = rand() % 100;
            printf("%d ", data[i]);
        }
        printf("\n");
    }


    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    bubble_sort(local_data, local_n);

    MPI_Gather(local_data, local_n, MPI_INT, data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        bubble_sort(data, n);

        double end_time = MPI_Wtime();
        double elapsed_time = end_time - start_time;

        printf("Vetor ordenado final:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");

        printf("Tempo total de execução: %fsegundos\n", elapsed_time);

        free(data);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}
