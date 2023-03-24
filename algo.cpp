using namespace std;

#include <bits/stdc++.h>

#include "dcel.h"

ofstream cfile("dcel_cords.txt");
ofstream efile("dcel_edges.txt");

vector<DCEL> convex_polygons;
map<pair<double, double>, int> vertex_map;
int vertex_index = 0;

/*!
    Function will initialize dcell with the given files for vertices and edges.
    Here the function assumes that the a given edge is the prev for the next edge (and vice versa)
*/
void initialize_dcel(DCEL &d, string points_file, string lines_file)
{
    fstream vertices_file(points_file, ios_base::in);

    double x, y;
    while (vertices_file >> x)
    {
        vertices_file >> y;
        Vertex v = Vertex(x, y);
        d.addVertex(v);
    }

    fstream edges_file(lines_file, ios_base::in);

    int org, dest;
    edges_file >> org;
    edges_file >> dest;
    d.addEdge(d.findVertexByIndex(org - 1), d.findVertexByIndex(dest - 1), NULL, NULL, NULL);
    while (edges_file >> org)
    {
        edges_file >> dest;
        d.addEdge(d.findVertexByIndex(org - 1), d.findVertexByIndex(dest - 1), &(*(-- --d.edges.end())), NULL, NULL);
    }
    (*(-- --d.edges.end())).next = &(*(d.edges.begin()));
    (*(d.edges.begin())).prev = &(*(-- --d.edges.end()));
    (*(-- --d.edges.end())).twin->prev = (*(d.edges.begin())).twin;
    (*(d.edges.begin())).twin->next = (*(-- --d.edges.end())).twin;
}

/*!
    THE function that performs the MP1 algorithm for partitioning recursively
    Here the function assumes that the given vertices are in anti-clockwise order
*/
void mp1(DCEL &p, Edge *p_start)
{

    DCEL l;

    // To find the default starting edge when NULL is passed
    if (!p_start)
        for (auto e : (--p.vertices.end())->inc_edges)
            if (e->left_face != &*p.faces.begin())
                p_start = e;
    Edge *p_cursor = p_start;

    Edge *l_first_edge = l.addEdge(p_cursor->org, p_cursor->dest, NULL, NULL, NULL);
    Edge *l_last_edge = l_first_edge;

    p_cursor = p_cursor->next;
    Vertex next_vertex = *p_cursor->dest;

    bool flag = true;

    while (l.n < p.n && p_cursor != NULL && p_cursor != NULL &&
           isReflexAngle(*l.findVertexByIndex(l.n - 2), *l.findVertexByIndex(l.n - 1), next_vertex) &&
           isReflexAngle(*l.findVertexByIndex(l.n - 1), next_vertex, *l.findVertexByIndex(0)) &&
           isReflexAngle(next_vertex, *l.findVertexByIndex(0), *l.findVertexByIndex(1)))
    {
        l_last_edge = l.addEdge(p_cursor->org, p_cursor->dest, l_last_edge, NULL, NULL);
        if (p_cursor->next == NULL)
        {
            flag = false;
            break;
        }
        p_cursor = p_cursor->next;
        next_vertex = *p_cursor->dest;
    }

    if (flag)
        p_cursor = p_cursor->prev;
    l.addEdge(&*l.findVertexByIndex(l.n - 1), &*l.findVertexByIndex(0), l_last_edge, l_first_edge, NULL);

    if (l.n < p.n && l.n > 2)
    {
        vector<Vertex> interiors;
        for (Vertex v : p.vertices)
            if (l.findVertex(v) == NULL && l.isInteriorPoint(v) == true)
                interiors.push_back(v);

        while (!interiors.empty())
        {
            l_last_edge = l_last_edge->prev;
            l.removeVertex(*l.findVertexByIndex(l.n - 1));
            l_last_edge = l.addEdge(l.findVertexByIndex(l.n - 1), l.findVertexByIndex(0), l_last_edge, l_first_edge, NULL);
            for (int i = interiors.size() - 1; i >= 0; i--)
                if (!l.isInteriorPoint(interiors[i]))
                    interiors.erase(interiors.begin() + i);
        }
    }


    if (l.n > 2)
    {
        Edge *new_start = p.remove(l);

        convex_polygons.push_back(l);

        for (auto v : l.vertices)
        {
            vertex_map[v.pairup()] = ++vertex_index;
            cfile << v.x << " " << v.y << endl;
        }

        for (auto it = l.edges.begin(); it != l.edges.end(); it++)
            efile << vertex_map[(*(it->org)).pairup()] << " " << vertex_map[(*(it++->dest)).pairup()] << endl;

        if (p.n > 3)
        {
            if (new_start->left_face != &*p.faces.begin())
                mp1(p, new_start);
            else
                mp1(p, new_start->twin);
        }
    }
    else
    {
        if (p.n > 3)
            mp1(p, p_start->next);
    }

}

int main()
{
    DCEL p, mp;
    initialize_dcel(p, "cords.txt", "edges.txt");

    if (p.n > 3)
        mp1(p, NULL);

    for (auto v : p.vertices)
    {
        vertex_map[v.pairup()] = ++vertex_index;
        cfile << v.x << " " << v.y << endl;
    }

    for (auto it = p.edges.begin(); it != p.edges.end(); it++)
        efile << vertex_map[(*(it->org)).pairup()] << " " << vertex_map[(*(it++->dest)).pairup()] << endl;

    convex_polygons.push_back(p);

    cfile.close();
    efile.close();
}
