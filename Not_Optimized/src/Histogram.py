from matplotlib import pyplot as plt
import numpy as np
import sys

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
plt.xticks(np.arange(0, n_values + 1, step = 200), fontsize = 16)

max_y = max(data)
plt.ylabel("Number of elements", fontsize = 20)
plt.ylim(0, 1.1 * max_y)

plt.yticks(np.arange(0, max_y, step = int(max_y // 10)), fontsize = 16)

plt.savefig(graph_path + graph_name + ".png")

plt.clf()
