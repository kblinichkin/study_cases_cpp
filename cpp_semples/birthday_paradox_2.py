import math

def calculate_k(N):
    k = math.sqrt(2 * N * math.log(2))
    return math.ceil(k)

num_values = 6
k_value = calculate_k(num_values)
print("Минимальное k значение:", k_value)
