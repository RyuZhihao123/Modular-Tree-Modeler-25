import numpy as np

color = np.random.rand(500,3) *0.9

print(color)

txt = ""

for i in range(color.shape[0]):
    txt += "{0} {1} {2}\n".format(color[i][0],color[i][1],color[i][2])

f = open("leaf_color.txt", "w")
f.write(txt)