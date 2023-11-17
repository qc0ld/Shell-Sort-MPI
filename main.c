#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void shell_sort(int* array, int n);

int main(int argc, char** argv) {
    const int count = 10000000;
    const int random_seed = 920215;

    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = (count + size - 1) / size;

    int* array = malloc(count * sizeof(int));
    int* sub_array = malloc(n * sizeof(int));
    int* sorted_array = malloc(count * sizeof(int));

    if (!rank) {
        srand(random_seed);
        for (int i = 0; i < count; i++) { array[i] = rand(); }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    MPI_Scatter(array, n, MPI_INT, sub_array, n, MPI_INT, 0, MPI_COMM_WORLD);

    shell_sort(sub_array, n);

    MPI_Gather(sub_array, n, MPI_INT, sorted_array, n, MPI_INT, 0, MPI_COMM_WORLD);

    if (!rank) {
        shell_sort(sorted_array, count);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (!rank) {
        printf("Threads: %d, Execution time: %f seconds\n", size, end_time - start_time);
    }

    free(array);
    free(sub_array);
    free(sorted_array);

    MPI_Finalize();

    return 0;
}

void shell_sort(int* array, int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = array[i];
            int j;
            for (j = i; j >= gap && array[j - gap] > temp; j -= gap)
                array[j] = array[j - gap];

            array[j] = temp;
        }
    }
}