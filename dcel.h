using namespace std;

#include <bits/stdc++.h>


/*
    Class For Vertexes
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
        Edge(Edge *nextP, Edge *prevP, Edge *twinP, Vertex &origin, Vertex &destination);
};

Edge::Edge(Edge *nextP, Edge *prevP, Edge *twinP, Vertex &origin, Vertex &destination) {
    next = nextP;
    prev = prevP;
    twin = twinP;
    org = &origin;
    dest = &destination;
}


/*
    Class For Polygons represented as DCELs
*/
class DCEL {
    public:
        set<Edge*> edges;
        set<Vertex*> vertices;
        int numberOfItems;

    public:

        DCEL();

        void addEdge(Vertex &v1, Vertex &v2, Edge* e1prev, Edge* e1next);
        void removeEdge(Edge &e1);
        void addVertex(Vertex &v);
        void removeVertex(Vertex &v);
        bool isInteriorPoint(Vertex &p);
};

DCEL::DCEL() {
    edges.clear();
    vertices.clear();
    numberOfItems=0;
}

/*
vertices v1 and v2 should be passed in clockwise order
e1prev is the previous of the edge made from v1 to v2
e1next is the next
both e1prev and e1next are pointers use & operator for passing normal Edge datatype 
pass NULL values if prev or next not known
*/
void DCEL::addEdge(Vertex& v1, Vertex& v2, Edge* e1prev, Edge* e1next)
{
    //insert vertices if not already present
    vertices.insert(&v1);
    vertices.insert(&v2);

    //check if edges already exist
    for(auto e:edges)
        if(e->org == &v1 && e->dest == &v2)
            return ;

    Edge* e1 = &Edge(NULL, NULL, NULL, v1, v2);
    Edge* e2 = &Edge(NULL, NULL, NULL, v2, v1);

    e1->twin = e2;
    e2->twin = e1;

    v1.inc_edges.insert(e1);
    v2.inc_edges.insert(e2);

    if(e1prev != NULL)
    {
        e1->prev = e1prev;
        e2->next = e1prev->twin;
        e1prev->next = e1;
        e1prev->twin->prev = e2;
    }

    if(e1next != NULL)
    {
        e1->next = e1next;
        e2->prev = e1next->twin;
        e1next->prev = e1;
        e1next->twin->next = e2;
    }

    edges.erase(e1);
    edges.erase(e2);
}

/*
will remove the edge e as well as its twin
*/
void DCEL::removeEdge(Edge &e)
{
    Edge* e1 = &e;
    Edge* e2 = e1->twin;

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

    edges.erase(e1);
    edges.erase(e2);

    free(e1);
    free(e2);
}

/*
will add vertex v to the DCEL
*/
void DCEL::addVertex(Vertex &v)
{
    vertices.insert(&v);
}

/*
will remove the vertex v as well as all the edges associated with it
*/
void DCEL::removeVertex(Vertex &v)
{
    for(auto e:v.inc_edges)
        removeEdge(*e);

    vertices.erase(&v);

    free(&v);
}

/*
For a closed polygon, function will tell if give point is an interior point of the DCEL
*/
bool DCEL::isInteriorPoint(Vertex &p)
{
    int count = 0;
    for(auto e:edges)
        if((p.y < e->org->y != p.y < e->dest->y) && 
        (p.x < (e->dest->x-e->org->x) * (p.y-e->org->y) / (e->dest->y-e->org->y) + e->org->x))
            count++;

    return count%2 == 1;
}
