#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 100000000  // 100 milhões

int main() {
    int *array = malloc(N * sizeof(int));
    if (array == NULL) {
        printf("Falha na alocação de memória\n");
        return 1;
    }

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Preenche o array com números aleatórios de 1 a 100
    for (int i = 0; i < N; i++) {
        array[i] = (rand() % 100) + 1;  // Gera números de 1 a 100
    }

    double inicio, fim;
    long long soma = 0;

    // Cálculo da soma usando um loop simples com for
    inicio = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        soma += array[i];
    }
    fim = omp_get_wtime();
    printf("Soma (serial): %lld\n", soma);
    printf("Tempo decorrido (serial): %f segundos\n", fim - inicio);

    // Cálculo da soma usando OpenMP com diferentes números de threads
    int num_threads[] = {2, 4, 8, 16};
    int num_configs = sizeof(num_threads) / sizeof(num_threads[0]);

    for (int idx = 0; idx < num_configs; idx++) {
        int n_threads = num_threads[idx];
        soma = 0;  // Reinicia a soma
        omp_set_num_threads(n_threads);

        inicio = omp_get_wtime();
        #pragma omp parallel for reduction(+:soma)
        for (int i = 0; i < N; i++) {
            soma += array[i];
        }
        fim = omp_get_wtime();

        printf("Soma (OpenMP com %d threads): %lld\n", n_threads, soma);
        printf("Tempo decorrido (OpenMP com %d threads): %f segundos\n", n_threads, fim - inicio);
    }

    free(array);

    return 0;
}
