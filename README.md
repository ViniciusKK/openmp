# Comparação de diferentes métodos de paralelização para somar os valores de um array

## Multiprocessing (Python)
Tempo tomado com 1 processo(s): 6.39 segundos

Tempo tomado com 2 processo(s): 3.64 segundos

Tempo tomado com 4 processo(s): 3.62 segundos

Tempo tomado com 8 processo(s): 4.34 segundos

Tempo tomado com 16 processo(s): 4.79 segundos

## OpenMP (C)

Tempo decorrido (serial): 0.245489 segundos

Tempo decorrido (OpenMP com 2 threads): 0.135882 segundos

Tempo decorrido (OpenMP com 4 threads): 0.073583 segundos

Tempo decorrido (OpenMP com 8 threads): 0.053533 segundos

Tempo decorrido (OpenMP com 16 threads): 0.058977 segundos

## Cuda (C com GPU Nvidia)

Tempo decorrido (Cuda com 256 threads por bloco): 0.020000 segundos