// Compilação:
// gcc -fopenmp -o maior_numero maior_numero.c
// Execução:
// ./maior_numero

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <limits.h>

#define N 100000000  // 100 milhões

int main() {
    int *array = malloc(N * sizeof(int));
    if (array == NULL) {
        printf("Falha na alocação de memória\n");
        return 1;
    }

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Preenche o array com números aleatórios de 1 a N
    for (int i = 0; i < N; i++) {
        array[i] = (rand() % N) + 1;  // Gera números de 1 a N
    }

    double inicio, fim;
    int maior = INT_MIN;  // Usa o menor inteiro possível

    // Achando o maior usando um loop simples com for
    inicio = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        if (array[i] > maior) {
            maior = array[i];
        }
    }
    fim = omp_get_wtime();
    printf("Maior número (serial): %d\n", maior);
    printf("Tempo decorrido (serial): %f segundos\n", fim - inicio);

    // Achando o maior usando OpenMP com diferentes números de threads
    int num_threads[] = {2, 4, 8, 16};
    int num_configs = sizeof(num_threads) / sizeof(num_threads[0]);

    for (int idx = 0; idx < num_configs; idx++) {
        int n_threads = num_threads[idx];
        maior = INT_MIN;  // Reinicia o maior
        omp_set_num_threads(n_threads);

        inicio = omp_get_wtime();
        #pragma omp parallel for reduction(max:maior)
        for (int i = 0; i < N; i++) {
            if (array[i] > maior) {
                maior = array[i];
            }
        }
        fim = omp_get_wtime();

        printf("Maior (OpenMP com %d threads): %d\n", n_threads, maior);
        printf("Tempo decorrido (OpenMP com %d threads): %f segundos\n", n_threads, fim - inicio);
    }

    free(array);
return 0;
}