from matplotlib import pyplot as plt
import numpy as np
import sys
import math

# =========================================
def N_Digits (n):

    i = 0
    while n > 0:
        n = n // 10
        i += 1

    return i
# =========================================

# =========================================
def Get_Axis_Step (val):

    n_digits = N_Digits (val)
    pow_10 = 10 ** (n_digits - 1)

    if val > 5 * pow_10:
        step = pow_10
    elif val > 2.5 * pow_10:
        step = pow_10 // 2
    else:
        step = pow_10 // 4

    return step
# =========================================


# ===============================================================================
def main (argv):

    graph_path = str(argv[1])
    graph_name = str(argv[2])

    data_file = open(graph_path + graph_name + ".txt", "r")
    data = data_file.readlines()
    data_file.close()

    n_values = len(data)

    x = []
    for i in range(n_values):
        x.append(i)
        data[i] = int(data[i])

    plt.figure (figsize = (16, 10), dpi = 80)

    plt.title(graph_name, fontsize = 40, loc = 'center')

    plt.xlabel("Bucket number", fontsize = 20)
    plt.xlim(-10, n_values + 10)

    step_x = Get_Axis_Step (n_values)
    plt.xticks(np.arange(0, n_values + 1, step = step_x), fontsize = 16)

    max_y = max(data)
    plt.ylabel("Number of elements", fontsize = 20)
    plt.ylim(0, 1.1 * max_y)

    step_y = Get_Axis_Step (max_y)
    plt.yticks(np.arange(0, max_y, step = step_y), fontsize = 16)

    plt.bar(x, data, width = 1, label = "Max collisions: %d" %max_y)
    plt.legend(loc = 'best', fontsize = 20)

    plt.savefig(graph_path + graph_name + ".png")

    plt.clf()
# ===============================================================================

main (sys.argv[0:])
