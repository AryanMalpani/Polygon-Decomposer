import turtle as ttl

def generatePoints(edgeInp,cordsInp):
    """ generate points for a square of any size """
    points = []
    edges = []
    to_plot = []
    fv = open("edges.txt".format(edges = edgeInp), "r")
    fc = open("cords.txt".format(cords = cordsInp), "r")

    fc_arr = fc.readlines()

    for cord in fc_arr:
        a = cord.split()
        for i in range(len(a)):
            a[i] = float(a[i])*500
        points.append(tuple(a))

    fv_arr = fv.readlines()

    for vert in fv_arr:
        a = vert.split()
        for i in range(len(a)):
            print(a[i])
            a[i] = int(a[i])
        edges.append(tuple(a))
    print(edges)
    points.reverse()

    for j in range(len(edges)):
        b1 = edges[j][0]
        b2 = edges[j][1]
        to_plot.append(points[b1-1])
        to_plot.append(points[b2-1])

    return points, to_plot


def drawpoints(start, points, to_plot, lineColor = "black", fillColor = "white"):
    ttl.pencolor(lineColor)
    ttl.fillcolor(fillColor)

    ttl.penup()
    ttl.goto(start)
    #for points
    ttl.pendown()
    ttl.begin_fill()
    x,y = start
    ttl.dot()
    ttl.pencolor("white")


    for point in points:
        dx, dy = point
        ttl.pencolor("white")
        ttl.goto(x + dx, y + dy)
        ttl.pencolor("black")
        ttl.dot()
    ttl.pencolor("white")
    ttl.goto(start)

    #for edges
    ttl.pendown()
    ttl.begin_fill()
    x,y = start
    ttl.pencolor("white")

    for k in range(len(to_plot)):
        dx, dy = to_plot[k]

        if (k%2 == 0):
            ttl.pencolor("white")
        ttl.goto(x + dx, y + dy)
        ttl.pencolor("black")

    # ttl.goto(start)

    ttl.end_fill()
    input()
    ttl.penup()

a, b = generatePoints("edges","cords")

drawpoints((0, 0), a, b)
ttl.done()
