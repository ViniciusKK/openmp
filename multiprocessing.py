import random
import time
import multiprocessing as mp

def chunk_sum(data_slice):
    return sum(data_slice)

def main():
    num_elements = 100_000_000
    num_processes_list = [1, 2, 4, 8, 16]  # Você pode ajustar conforme o número de núcleos disponíveis

    # Gerar a lista de números aleatórios
    print("Gerando números aleatórios...")
    start_time = time.time()
    data = [random.randint(1, 100) for _ in range(num_elements)]
    end_time = time.time()
    print(f"Tempo para gerar números aleatórios: {end_time - start_time:.2f} segundos\n")

    for num_processes in num_processes_list:
        print(f"Calculando a soma usando {num_processes} processo(s)...")

        start_time = time.time()

        # Dividir os dados em chunks
        chunk_size = num_elements // num_processes
        chunks = [data[i * chunk_size : (i + 1) * chunk_size] for i in range(num_processes)]

        # Ajustar o último chunk se houver sobra
        if num_elements % num_processes != 0:
            chunks[-1].extend(data[num_processes * chunk_size:])

        # Criar um Pool de processos
        with mp.Pool(processes=num_processes) as pool:
            results = pool.map(chunk_sum, chunks)

        total_sum = sum(results)
        end_time = time.time()

        print(f"Soma Total: {total_sum}")
        print(f"Tempo tomado com {num_processes} processo(s): {end_time - start_time:.2f} segundos\n")

if __name__ == '__main__':
    main()
