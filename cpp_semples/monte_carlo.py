import random

def calculate_k(N, num_simulations):
    total_k = 0

    for _ in range(num_simulations):
        values = set()
        k = 0

        while len(values) < N:
            k += 1
            values.add(random.randint(1, N))

        total_k += k

    average_k = total_k / num_simulations
    return average_k

num_values = 6
num_simulations = 100000  # Количество симуляций
k_value = calculate_k(num_values, num_simulations)
print("Приближенное среднее значение k:", k_value)
