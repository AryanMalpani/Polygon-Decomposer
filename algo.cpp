using namespace std;

#include <bits/stdc++.h>

#include "dcel.h"

DCEL p;


bool isReflexAngle(Vertex A, Vertex B, Vertex C)
{
    Vertex edge1 = Vertex(B.x - A.x, B.y - A.y);
    Vertex edge2 = Vertex(C.x - B.x, C.y - B.y);
    double angle = edge1.x * edge2.y - edge1.y * edge2.x;
    if (angle < 0)
        return true;
    return false;
}


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
    cout<<"check 0.1";

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
    cout<<"check 0.5";
}

int main()
{
    cout<<"check 0";
    ofstream cfile("dcel_cords.txt");
    ofstream efile("dcel_edges.txt");
    map<pair<double, double>, int> vertex_map;
    initialize_dcel();
    int index = 0;
    // cout<<"done initialize"<<endl;

    vector<DCEL> convex_polygons;

    while (p.vertices.size() > 3)
    {


        int count = 0;
        for(auto v:p.vertices)
        cout<<v.x<<" "<<v.y<<endl;
        cout<<p.edges.size()<<endl;
        for(auto it=p.edges.begin(); it!=p.edges.end();it++)
        {
            cout<<count<<" "<<&*it<<" "<<it->twin<<" "<<it->prev<<endl;
            cout<<count++<<" "<<it->org->x<<" "<<it->left_face<<" "<<it->prev->org->x<<endl;
        }






        DCEL l;
        Edge *p_start;
        for(auto e:(--p.vertices.end())->inc_edges)
            if(e->left_face != &*p.faces.begin())
                p_start = e;
        Edge *p_cursor = p_start;

        l.addVertex(*p_cursor->org);
        l.addVertex(*p_cursor->dest);

        Edge *last_edge = l.addEdge(p_cursor->org, p_cursor->dest, NULL, NULL, NULL);

        p_cursor = p_cursor->next;
        Vertex next_vertex = *p_cursor->dest;

        cout << "check 1" << endl;

        bool flag = true;

        while (l.n < p.n && p_cursor != NULL && p_cursor != NULL &&
               isReflexAngle(*l.findVertexByIndex(l.n - 2), *l.findVertexByIndex(l.n - 1), next_vertex) &&
               isReflexAngle(*l.findVertexByIndex(l.n - 1), next_vertex, *l.findVertexByIndex(0)) &&
               isReflexAngle(next_vertex, *l.findVertexByIndex(0), *l.findVertexByIndex(1)))
        {
            l.addVertex(next_vertex);
            last_edge = l.addEdge(p_cursor->org, p_cursor->dest, last_edge, NULL, NULL);
            if (p_cursor->next == NULL)
            {
                flag = false;
                break;
            }
            p_cursor = p_cursor->next;
            next_vertex = *p_cursor->dest;
        }

        cout << l.n << endl;
        cout << "check 2" << endl;

        if (flag)
            p_cursor = p_cursor->prev;
        l.addEdge(&*l.findVertexByIndex(l.n - 1), &*l.findVertexByIndex(0), p_cursor, p_start, NULL);

        cout << "check 2.1" << endl;
        cout << "l size = " << l.n << endl;

        if (l.n < p.n)
        {
            vector<Vertex> interiors;
            for (Vertex v : p.vertices)
                if (l.findVertex(v) == NULL && l.isInteriorPoint(v) == true)
                {
                    cout << "found interior point";
                    interiors.push_back(v);
                }

            cout << "check 2.2" << endl;

            while (!interiors.empty())
            {
                l.removeVertex(*l.findVertexByIndex(l.n - 1));
                p_cursor = p_cursor->prev;
                last_edge = l.addEdge(l.findVertexByIndex(l.n - 1), l.findVertexByIndex(0), p_cursor, p_start, NULL);
                for (int i = interiors.size() - 1; i >= 0; i--)
                    if (!l.isInteriorPoint(interiors[i]))
                        interiors.erase(interiors.begin() + i);
            }
            cout << "check 2.3" << endl;

            cout << "l size = " << l.n << endl;
        }

        p.remove(l);

        cout << "check 3" << endl;

        convex_polygons.push_back(l);

        cout << p.n << endl;
        cout << p.vertices.size() << endl;
        cout << l.n << endl;

        for (auto v : l.vertices)
        {
            vertex_map[v.pairup()] = ++index;
            cfile << v.x << " " << v.y << endl;
        }

        cout << "number of eedges " << l.edges.size() << endl;

        for (auto it = l.edges.begin(); it != l.edges.end(); it++)
            efile << vertex_map[(*(it->org)).pairup()] << " " << vertex_map[(*(it++->dest)).pairup()] << endl;

        cout<<"check 4"<<endl;
    }
    cfile.close();
    efile.close();
}
