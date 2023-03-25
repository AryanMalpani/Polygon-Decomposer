using namespace std;

#include <bits/stdc++.h>

#include "dcel.h"

ofstream cfile("dcel_cords.txt");
ofstream efile("dcel_edges.txt");

DCEL p, mp;
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
            if (vertex_map.find(v.pairup()) == vertex_map.end())
            {
                vertex_map[v.pairup()] = ++vertex_index;
                cfile << v.x << " " << v.y << endl;
            }
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

void merging()
{
    initialize_dcel(mp, "dcel_cords.txt", "dcel_edges.txt");
    mp.save();

    cout << "Number of diagonals = " << p.added_diagonals.size() << endl;

    vector<pair<Vertex, Vertex>> inessential_diagonals;

    for (auto dig : p.added_diagonals)
    {
        Vertex org = dig.first;
        Vertex *mp_org = mp.findVertex(org);
        Vertex dest = dig.second;
        Vertex *mp_dest = mp.findVertex(dest);

        double p_min = DBL_MAX;
        Vertex vp_min;
        Vertex vn_min;
        Vertex default_v;
        double n_min = -DBL_MAX;

        for (auto e : mp_org->inc_edges)
        {
            if (*(e->dest) == dest)
                continue;
            double slope_diff = (dest.y - org.y) / (dest.x - org.x) - (e->dest->y - org.y) / (e->dest->x - org.x);
            if (slope_diff > 0)
            {
                p_min = min(p_min, slope_diff);
                vp_min = Vertex(e->dest->x, e->dest->y);
            }
            else
            {
                n_min = max(n_min, slope_diff);
                vn_min = Vertex(e->dest->x, e->dest->y);
            }
        }

        if (vp_min == default_v || vn_min == default_v || ((getAngleOfFirst(org, dest, vp_min) + getAngleOfFirst(org, dest, vn_min)) > 180))
            continue;

        p_min = DBL_MAX;
        vp_min = default_v;
        vn_min = default_v;
        default_v = default_v;
        n_min = -DBL_MAX;

        for (auto e : mp_dest->inc_edges)
        {
            if (*(e->dest) == org)
                continue;
            double slope_diff = (org.y - dest.y) / (org.x - dest.x) - (e->dest->y - dest.y) / (e->dest->x - dest.x);
            if (slope_diff > 0)
            {
                p_min = min(p_min, slope_diff);
                vp_min = Vertex(e->dest->x, e->dest->y);
            }
            else
            {
                n_min = max(n_min, slope_diff);
                vn_min = Vertex(e->dest->x, e->dest->y);
            }
        }

        if (vp_min == default_v || vn_min == default_v || ((getAngleOfFirst(dest, org, vp_min) + getAngleOfFirst(dest, org, vn_min)) > 180))
            continue;

        inessential_diagonals.push_back(dig);
    }

    ofstream mcfile("merged_dcel_cords.txt");
    ofstream mefile("merged_dcel_edges.txt");
    vertex_map.clear();
    vertex_index = 0;

    for (auto v : mp.vertices)
    {
        if (vertex_map.find(v.pairup()) == vertex_map.end())
        {
            vertex_map[v.pairup()] = ++vertex_index;
            mcfile << v.x << " " << v.y << endl;
        }
    }

    for (auto it = mp.edges.begin(); it != mp.edges.end(); it++)
    {
        bool flag = true;
        for (auto dig : inessential_diagonals)
            if ((*it->org == dig.first && *it->dest == dig.second) ||
                (*it->org == dig.second && *it->dest == dig.first))
            {
                flag = false;
                break;
            }
        if (flag)
            mefile << vertex_map[(*(it->org)).pairup()] << " " << vertex_map[(*(it++->dest)).pairup()] << endl;
    }

    cout << "inessential size = " << inessential_diagonals.size() << endl;

    mcfile.close();
    mefile.close();
}

int main()
{
    initialize_dcel(p, "cords.txt", "edges.txt");

    int count = 0;
    for (auto v : p.vertices)
        cout << v.x << " " << v.y << endl;
    cout << p.edges.size() << endl;
    for (auto it = p.edges.begin(); it != p.edges.end(); it++)
    {
        cout << count << " " << &*it << " " << it->twin << " " << it->org << endl;
        for (auto e : it->org->inc_edges)
            cout << e->org << " ";
        cout << endl
             << count++ << " " << it->org->x << " " << it->org->y << "  face= " << it->left_face << " " << it->prev->org->x << endl;
    }

    if (p.n > 3)
        mp1(p, NULL);

    for (auto v : p.vertices)
    {
        if (vertex_map.find(v.pairup()) == vertex_map.end())
        {
            vertex_map[v.pairup()] = ++vertex_index;
            cfile << v.x << " " << v.y << endl;
        }
    }

    for (auto it = p.edges.begin(); it != p.edges.end(); it++)
        efile << vertex_map[(*(it->org)).pairup()] << " " << vertex_map[(*(it++->dest)).pairup()] << endl;

    convex_polygons.push_back(p);

    merging();

    cfile.close();
    efile.close();
}
