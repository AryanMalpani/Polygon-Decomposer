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
    cout<<"done initialize"<<endl;
    int count = 0;
    for(auto v:p.vertices)
        cout<<v.x<<endl;
    cout<<p.edges.size()<<endl;
    for(int f=58;f<p.edges.size();f+=2)
        cout<<count++<<" "<<p.edges[f].prev->next->org->x<<endl;
    vector<DCEL> convex_polygons;

    while(p.vertices.size()>3)
    {
        DCEL l;
        Edge* p_start = &p.edges[0];
        Edge* p_cursor = p_start;

        l.addVertex(*p_cursor->org);
        l.addVertex(*p_cursor->dest);

        Edge* last_edge = l.addEdge(p_cursor->org, p_cursor->dest, NULL, NULL);

    }

}
