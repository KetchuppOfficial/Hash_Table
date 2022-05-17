from matplotlib import pyplot as plt
import numpy as np
import sys
import math

# ========================================
def Get_Round_Step (max_val, mult_base, n_ticks):

    step = max_val / n_ticks
    step = mult_base * (step // mult_base)

    if step == 0:
        return 1
    
    return step
# ========================================

graph_path = str(sys.argv[1])
graph_name = str(sys.argv[2])

data_file = open(graph_path + graph_name + ".txt", "r")
data = data_file.readlines()
data_file.close()

n_values = len(data)

x = []
for i in range(n_values):
    x.append(i)
    data[i] = int(data[i])

plt.figure (figsize = (16, 10), dpi = 80)

plt.bar(x, data, width = 1)

plt.title(graph_name, fontsize = 40, loc = 'center')

plt.xlabel("Bucket number", fontsize = 20)
plt.xlim(-10, n_values + 10)

step_x = Get_Round_Step (n_values, 10, 10)
plt.xticks(np.arange(0, n_values + 1, step = step_x), fontsize = 16)

max_y = max(data)
plt.ylabel("Number of elements", fontsize = 20)
plt.ylim(0, 1.1 * max_y)

step_y = Get_Round_Step (max_y, 10, 10)
plt.yticks(np.arange(0, max_y, step = step_y), fontsize = 16)

plt.text(n_values - 500, max_y, "Max collisions: %d" %max_y, fontsize = 20)

plt.savefig(graph_path + graph_name + ".png")

plt.clf()
