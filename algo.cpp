using namespace std;

#include <bits/stdc++.h>

#include "dcel.h"

bool isReflexAngle(Vertex A, Vertex B, Vertex C)
{
    Vertex edge1 = Vertex(B.x-A.x, B.y-A.y);
    Vertex edge2 = Vertex(C.x-B.x, C.y-B.y);
    float angle = edge1.x*edge2.y - edge1.y*edge2.x;
    if (angle < 0) return true;
    return false;
    return true;
}

DCEL p;

void initialize_dcel()
{
    fstream vertices_file("cords.txt", ios_base::in);

    float x, y;
    while (vertices_file >> x)
    {
        vertices_file >> y;
        p.vertices.push_back(Vertex(x,y)); 
    }

    fstream edges_file("edges.txt", ios_base::in);

    int org, dest;
    edges_file >> org;
    edges_file >> dest;
    p.addEdge(&p.vertices[org-1], &p.vertices[dest-1], NULL, NULL);
    while (edges_file >> org)
    {
        edges_file >> dest;
        p.addEdge(&p.vertices[org-1], &p.vertices[dest-1], &p.edges[p.edges.size()-2], NULL);
    }
    p.edges[p.edges.size()-2].next = &p.edges[0];
    p.edges[0].prev = &p.edges[p.edges.size()-2];
    p.edges[p.edges.size()-2].twin->prev = p.edges[0].twin;
    p.edges[0].twin->next = p.edges[p.edges.size()-2].twin;
}    

int main()
{
    initialize_dcel();
    vector<DCEL> convex_polygons;

    while(p.vertices.size()>3)
    {
        DCEL l;
        Edge* p_start = &p.edges[0];
        Edge* p_cursor = p_start;

        l.addVertex(*p_cursor->org);
        l.addVertex(*p_cursor->dest);

        Edge* last_edge = l.addEdge(p_cursor->org, p_cursor->dest, NULL, NULL);

        p_cursor = p_cursor->next;
        Vertex next_vertex = *p_cursor->dest;

        while(l.n<p.n && p_cursor != NULL &&
            isReflexAngle(l.vertices[l.n-2], l.vertices[l.n-1], next_vertex) &&
            isReflexAngle(l.vertices[l.n-1], next_vertex, l.vertices[0]) && 
            isReflexAngle(next_vertex, l.vertices[0], l.vertices[1]))
        {
            l.addVertex(next_vertex);
            last_edge = l.addEdge(p_cursor->org, p_cursor->dest, last_edge, NULL);
            p_cursor = p_cursor->next;
            next_vertex = *p_cursor->dest;
        }

        p_cursor = p_cursor->prev;
        l.addEdge(&l.vertices[l.n-1], &l.vertices[0], p_cursor, p_start);

        if(l.n<p.n)
        {
            vector<Vertex> interiors;
            for(Vertex v:p.vertices)
                if(l.findVertexIndex(v) == -1 && l.isInteriorPoint(v) == true)
                    interiors.push_back(v);

            while(!interiors.empty())
            {
                l.removeVertex(l.vertices[l.n-1]);
                p_cursor = p_cursor->prev;
                last_edge = l.addEdge(&l.vertices[l.n-1], &l.vertices[0], p_cursor, p_start);
                for(int i=interiors.size()-1;i>=0;i--)
                    if(!l.isInteriorPoint(interiors[i]))
                        interiors.erase(interiors.begin() + i);
            }
            p.remove(l, p_cursor->next, p_start->prev);
        }

        convex_polygons.push_back(l);
    }

}
