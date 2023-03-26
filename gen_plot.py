import turtle as ttl

def generatePoints(scale):
        """ generate points for a square of any size """
        points = []
        edges = []
        to_plot = []
        fv = open("gen_edges.txt", "r")
        fc = open("gen_cords.txt", "r")

        fc_arr = fc.readlines()
        for cord in fc_arr:
            a = cord.split()
            for i in range(len(a)):
                a[i] = float(a[i])*scale
            points.append(tuple(a))

        fv_arr = fv.readlines()

        for vert in fv_arr:
            a = vert.split()
            for i in range(len(a)):
                a[i] = int(a[i])
            edges.append(tuple(a))
        # print(edges)

        # points.reverse()

        for j in range(len(edges)):
            b1 = edges[j][0]
            b2 = edges[j][1]
            to_plot.append(points[b1-1])
            to_plot.append(points[b2-1])
        print(to_plot)
        return points, to_plot

def drawpoints(start, points, to_plot, lineColor="black", fillColor = "white"):
        plotted = []
        ttl.pencolor(lineColor)
        ttl.fillcolor(fillColor)

        ttl.penup()
        ttl.goto(start)
        # for points
        ttl.pendown()
        ttl.begin_fill()
        x, y = start
        ttl.pencolor("white")
        ttl.dot()

        for point in points:
            dx, dy = point
            ttl.pencolor("white")
            ttl.goto(x + dx, y + dy)
            ttl.pencolor("black")
            ttl.dot()
        ttl.pencolor("white")
        ttl.goto(start)

        # for edges
        ttl.pendown()
        ttl.begin_fill()
        x, y = start
        ttl.pencolor("white")

        for k in range(len(to_plot)):
            dx, dy = to_plot[k]

            if (k %2 == 0) and (tuple([to_plot[k-1], to_plot[k]]) not in plotted):
                ttl.pencolor("white")
            else:
                ttl.pencolor("black")
                plotted.append(tuple([to_plot[k-1], to_plot[k]]))
                plotted.append(tuple([to_plot[k], to_plot[k-1]]))

            ttl.goto(x + dx, y + dy)

        # ttl.goto(start)

        ttl.end_fill()
        input()
        ttl.penup()

sc = open("gen_cords.txt", "r")
sc_arr = sc.readlines()
scal_cord = sc_arr[0].split()
scal_cord2 = sc_arr[1].split()
print(scal_cord)
scaling_fact = (float(scal_cord[0]) + float(scal_cord[1]) +
                float(scal_cord2[0]) + float(scal_cord2[1]))/4
print(scaling_fact)
scale = float(1/scaling_fact)*300
sc.close()

a, b = generatePoints(scale)

drawpoints((-scale, -scale), a, b)
ttl.done()
