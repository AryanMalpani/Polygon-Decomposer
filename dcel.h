using namespace std;

#include <bits/stdc++.h>

class Edge;

/*
    Class For Vertices
*/
class Vertex{
    public:
        float x, y;
        set<Edge*> inc_edges;

        //constructor
        Vertex(float x_value, float y_value);
};

Vertex::Vertex(float x_value, float y_value) {
    x = x_value;
    y = y_value;
}


/*
    Class For Edges
*/
class Edge{
    public:
        Edge *next;
        Edge *prev;
        Edge *twin;
        Vertex *org;
        Vertex *dest;

        //constructor
        Edge(Edge *nextP, Edge *prevP, Edge *twinP, Vertex *origin, Vertex *destination);
};

Edge::Edge(Edge *nextP, Edge *prevP, Edge *twinP, Vertex *origin, Vertex *destination) {
    next = nextP;
    prev = prevP;
    twin = twinP;
    org = origin;
    dest = destination;
}


/*
    Class For Polygons represented as DCELs
*/
class DCEL {
    public:
        list<Edge> edges;
        vector<Vertex> vertices;
        int n;

    public:
        DCEL();
        Edge* addEdge(Vertex* v1, Vertex* v2, Edge* e1prev, Edge* e1next);
        void removeEdge(Edge* e1);
        Vertex* addVertex(Vertex &v);
        void removeVertex(Vertex &v);
        bool isInteriorPoint(Vertex &p);
        void remove(DCEL &l, Edge* before_start, Edge* before_end);
        Vertex* findVertex(Vertex &v);
        int findVertexIndex(Vertex& v);
};

DCEL::DCEL() {
    edges.clear();
    vertices.clear();
    n=0;
}

/*
vertices v1 and v2 should be passed in clockwise order
e1prev is the previous of the edge made from v1 to v2
e1next is the next
both e1prev and e1next are pointers
pass NULL values if prev or next not known
*/
Edge* DCEL::addEdge(Vertex* v1, Vertex* v2, Edge* e1prev, Edge* e1next)
{
    //insert vertices if not already present
    v1 = addVertex(*v1);
    v2 = addVertex(*v2);

    //check if edges already exist
    for(auto it=edges.begin(); it!=edges.end();it++)
        if((*it).org == v1 && (*it).dest == v2)
            return &(*it);

    Edge e1 = Edge(NULL, NULL, NULL, v1, v2);
    Edge e2 = Edge(NULL, NULL, NULL, v2, v1);

    edges.push_back(e1);
    edges.push_back(e2);

    Edge* e1p = &(*(----edges.end()));
    Edge* e2p = &(*(--edges.end()));

    cout<<"og address "<<e1p<<" "<<e2p<<endl;

    e1p->twin = e2p;
    e2p->twin = e1p;

    v1->inc_edges.insert(e1p);
    v2->inc_edges.insert(e2p);

    if(e1prev != NULL)
    {
        e1p->prev = e1prev;
        e2p->next = e1prev->twin;
        e1prev->next = e1p;
        e1prev->twin->prev = e2p;
        cout<<e1p->prev->next->org->x<<endl;
    }

    if(e1next != NULL)
    {
        e1p->next = e1next;
        e2p->prev = e1next->twin;
        e1next->prev = e1p;
        e1next->twin->next = e2p;
        cout<<"e1next";
    }

    return e1p;
}

/*
will remove the edge e1 as well as its twin
*/
void DCEL::removeEdge(Edge* e1)
{
    Edge *e2 = e1->twin;

    e1->org->inc_edges.erase(e1);
    e2->org->inc_edges.erase(e2);

    if(e1->prev != NULL)
    {
        e1->prev->next = NULL;
        e2->next->prev = NULL;
    }

    if(e2->prev != NULL)
    {
        e2->prev->next = NULL;
        e1->next->prev = NULL;
    }

    for(auto it=--edges.end(); it!=--edges.begin();it--)
        if((it->org==e1->org && it->org==e1->dest)||
            (it->org==e2->org && it->org==e2->dest))
            edges.erase(it);
}

/*
will add vertex v to the DCEL
*/
Vertex* DCEL::addVertex(Vertex &v)
{
    int i = findVertexIndex(v);
    if(i!=-1)
        return &vertices[i];

    vertices.push_back(v);
    n++;

    return &vertices[vertices.size()-1];
}

/*
will remove the vertex v as well as all the edges associated with it
*/
void DCEL::removeVertex(Vertex &v)
{
    for(auto e:v.inc_edges)
        removeEdge(e);

    int i = findVertexIndex(v);
    if(i==-1)
        return ;

    vertices.erase(vertices.begin() + i);

    n--;
}

/*
For a closed polygon, function will tell if give point is an interior point of the DCEL
*/
bool DCEL::isInteriorPoint(Vertex &p)
{
    int count = 0;
    for(auto e:edges)
        if((p.y < e.org->y != p.y < e.dest->y) && 
        (p.x < (e.dest->x-e.org->x) * (p.y-e.org->y) / (e.dest->y-e.org->y) + e.org->x))
            count++;

    return count%2 == 1;
}

void DCEL::remove(DCEL &l, Edge* before_start, Edge* after_end)
{
    addEdge(after_end->org, before_start->dest, after_end->prev, before_start->next);

    for(Vertex v:l.vertices)
        removeVertex(v);
}

Vertex* DCEL::findVertex(Vertex& v)
{
    int i;
    for(i=0;i<vertices.size();i++)
        if(vertices[i].x==v.x && vertices[i].y==v.y)
            break;
    
    return i==vertices.size()?NULL:&vertices[i];    
}

int DCEL::findVertexIndex(Vertex& v)
{
    int i;
    for(i=0;i<vertices.size();i++)
        if(vertices[i].x==v.x && vertices[i].y==v.y)
            break;
    
    return i==vertices.size()?-1:i;    
}
