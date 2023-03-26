from polygenerator import (
    random_polygon,
    random_star_shaped_polygon,
    random_convex_polygon,
)


# these two are only for demonstration
import matplotlib.pyplot as plt
import random


def plot_polygon(polygon, out_file_name):
    plt.figure()
    plt.gca().set_aspect("equal")

    # for i, (x, y) in enumerate(polygon):
        # plt.text(x, y, str(i), horizontalalignment="center", verticalalignment="center")

    # just so that it is plotted as closed polygon
    polygon.append(polygon[0])

    xs, ys = zip(*polygon)
    plt.plot(xs, ys, "r-", linewidth=0.4)

    plt.savefig(out_file_name, dpi=300)
    plt.close()


# this is just so that you can reproduce the same results
random.seed(5)

polygon = random_polygon(num_points=200)

gc = open("gen_cords.txt", "w")
ge = open("gen_edges.txt", "w")
count = 1
for line in polygon:
    gc.write(f"{line[0]}     {line[1]}\n")
    if count != len(polygon):
        ge.write(f"{count}     {(count+1)}\n")
    count = count + 1

gc.close()

ge.write(f"{len(polygon)}     {1}\n")
ge.close()

print(polygon)
# [(0.752691110661913, 0.948158571633034), (0.7790276993942304, 0.05437135270534656), ..., (0.633385213909564, 0.7365967958574935)]

plot_polygon(polygon, "random_polygon.png")