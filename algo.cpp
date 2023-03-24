using namespace std;

#include <bits/stdc++.h>

#include "dcel.h"

ofstream cfile("dcel_cords.txt");
ofstream efile("dcel_edges.txt");

DCEL p;
map<pair<double, double>, int> vertex_map;
int vertex_index=0;
vector<DCEL> convex_polygons;
bool single_test = true;

void initialize_dcel()
{
    fstream vertices_file("cords.txt", ios_base::in);

    double x, y;
    while (vertices_file >> x)
    {
        vertices_file >> y;
        Vertex v = Vertex(x, y);
        p.addVertex(v);
    }

    fstream edges_file("edges.txt", ios_base::in);

    int org, dest;
    edges_file >> org;
    edges_file >> dest;
    p.addEdge(p.findVertexByIndex(org - 1), p.findVertexByIndex(dest - 1), NULL, NULL, NULL);
    while (edges_file >> org)
    {
        edges_file >> dest;
        p.addEdge(p.findVertexByIndex(org - 1), p.findVertexByIndex(dest - 1), &(*(-- --p.edges.end())), NULL, NULL);
    }
    (*(-- --p.edges.end())).next = &(*(p.edges.begin()));
    (*(p.edges.begin())).prev = &(*(-- --p.edges.end()));
    (*(-- --p.edges.end())).twin->prev = (*(p.edges.begin())).twin;
    (*(p.edges.begin())).twin->next = (*(-- --p.edges.end())).twin;
}

void mp1(Edge *p_start)
{

    // int count = 0;
    // for (auto v : p.vertices)
    //     cout << v.x << " " << v.y << endl;
    // cout << p.edges.size() << endl;
    // for (auto it = p.edges.begin(); it != p.edges.end(); it++)
    // {
    //     cout << count << " " << &*it << " " << it->twin << " " << it->prev << endl;
    //     cout << count++ << " " << it->org->x << " " << it->left_face << " " << it->prev->org->x << endl;
    // }

    DCEL l;

    if(!p_start)    
        for (auto e : (--p.vertices.end())->inc_edges)
            if (e->left_face != &*p.faces.begin())
                p_start = e;
    Edge *p_cursor = p_start;

    l.addVertex(*p_cursor->org);
    l.addVertex(*p_cursor->dest);

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
        l.addVertex(next_vertex);
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
    if(single_test)
    {
        l.save();single_test=false;
    }
    cout << "check 2.1 l size = " << l.n << endl;

    if (l.n < p.n && l.n>2)
    {
        vector<Vertex> interiors;
        for (Vertex v : p.vertices)
            if (l.findVertex(v) == NULL && l.isInteriorPoint(v) == true)
            {
                cout << "found interior point = "<<v.x<<" "<<v.y<<endl;
                l.save();
                interiors.push_back(v);
            }

        cout<<"check 2.1.5"<<endl;

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

    cout << "check 2.3 l size = " << l.n << endl;

    if (l.n > 2)
    {
        Edge* new_start = p.remove(l);
        cout << "check 3" << endl;

        convex_polygons.push_back(l);

        cout << p.n << endl;
        cout << p.vertices.size() << endl;
        cout << l.n << endl;

        for (auto v : l.vertices)
        {
            vertex_map[v.pairup()] = ++vertex_index;
            cfile << v.x << " " << v.y << endl;
        }

        cout << "number of edges " << l.edges.size() << endl;

        for (auto it = l.edges.begin(); it != l.edges.end(); it++)
            efile << vertex_map[(*(it->org)).pairup()] << " " << vertex_map[(*(it++->dest)).pairup()] << endl;

        if(p.n>3)
        {
            if(new_start->left_face != &*p.faces.begin())
                mp1(new_start);
            else
                mp1(new_start->twin);
        }
    }
    else
    {
        if(p.n>3)
            mp1(p_start->next);
    }

    cout << "check 4" << endl;
}

int main()
{
    initialize_dcel();

    if (p.n > 3)
        mp1(NULL);

    for (auto v : p.vertices)
    {
        vertex_map[v.pairup()] = ++vertex_index;
        cfile << v.x << " " << v.y << endl;
    }

    cout << "number of eedges " << p.edges.size() << endl;

    for (auto it = p.edges.begin(); it != p.edges.end(); it++)
        efile << vertex_map[(*(it->org)).pairup()] << " " << vertex_map[(*(it++->dest)).pairup()] << endl;

    cfile.close();
    efile.close();
}
