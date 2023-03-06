using namespace std;

#include <bits/stdc++.h>
#include "dcel.h"

bool isReflexAngle(Vertex A, Vertex B, Vertex C)
{
    //To complete
    return true;
}

int main()
{
    DCEL p;
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
