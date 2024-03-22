import math

def calculate_k(N):
    k = 1
    p_x_equals_k = ((1 - (1-1/N)**k)**N)
    p_x_greater_k = 1 - ((1 - (1-1/N)**k)**N)

    while p_x_equals_k <= p_x_greater_k:
        k += 1
        p_x_equals_k = ((1 - (1-1/N)**k)**N)
        p_x_greater_k = 1 - ((1 - (1-1/N)**k)**N)

    return k

num_values = 6
k_value = calculate_k(num_values)
print("Минимальное k значение:", k_value)
