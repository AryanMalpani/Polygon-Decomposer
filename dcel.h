using namespace std;

#include <bits/stdc++.h>

#define PI 3.1415926535

class Edge;
class Face;

/*!
    Class For Vertices
*/
class Vertex{
    public:
        double x, y;
        set<Edge*> inc_edges;

        //constructor
        Vertex();
        Vertex(double x_value, double y_value);
        pair<double,double> pairup();
};

Vertex::Vertex() {
    x = DBL_MAX;
    y = DBL_MAX;
}

Vertex::Vertex(double x_value, double y_value) {
    x = x_value;
    y = y_value;
}

pair<double,double> Vertex::pairup()
{
    return make_pair(x,y);
}

bool operator==(Vertex const& c1, Vertex const& c2)
{
    return (c1.x==c2.x)&&(c1.y==c2.y);
}

bool operator!=(Vertex const& c1, Vertex const& c2)
{
    return (c1.x!=c2.x)||(c1.y!=c2.y);
}

bool isReflexAngle(Vertex A, Vertex B, Vertex C)
{
    Vertex edge1 = Vertex(B.x - A.x, B.y - A.y);
    Vertex edge2 = Vertex(C.x - B.x, C.y - B.y);
    double angle = edge1.x * edge2.y - edge1.y * edge2.x;
    if (angle < 0)
        return true;
    else
        return false;
}

// returns square of distance b/w two points
double lengthSquare(Vertex X, Vertex Y)
{
    double xDiff = X.x - Y.x;
    double yDiff = X.y - Y.y;
    return xDiff*xDiff + yDiff*yDiff;
}

double getAngleOfFirst(Vertex A, Vertex B, Vertex C)
{
    // Square of lengths be a2, b2, c2
    double a2 = lengthSquare(B,C);
    double b2 = lengthSquare(A,C);
    double c2 = lengthSquare(A,B);
  
    // length of sides be a, b, c
    double a = sqrt(a2);
    double b = sqrt(b2);
    double c = sqrt(c2);
  
    // From Cosine law
    double alpha = acos((b2 + c2 - a2)/(2*b*c));
    double beta = acos((a2 + c2 - b2)/(2*a*c));
    double gamma = acos((a2 + b2 - c2)/(2*a*b));
  
    // Converting to degree
    alpha = alpha * 180 / PI;
    beta = beta * 180 / PI;
    gamma = gamma * 180 / PI;
  
    return alpha;
}


/*!
    Class For Edges
*/
class Edge{
    public:
        Edge* next;
        Edge* prev;
        Edge* twin;
        Vertex* org;
        Vertex* dest;
        Face* left_face;

        //constructor
        Edge(Edge *nextP, Edge *prevP, Edge *twinP, Vertex *origin, Vertex *destination, Face *l_face);
};

Edge::Edge(Edge *nextP, Edge *prevP, Edge *twinP, Vertex *origin, Vertex *destination, Face *l_face) {
    next = nextP;
    prev = prevP;
    twin = twinP;
    org = origin;
    dest = destination;
    left_face = l_face;
}


/*!
    Class For Faces
*/
class Face{
    public:
        Edge* inc_edge;

        //constructor
        Face();
        Face(Edge* i_edge);
};

Face::Face() {
    inc_edge = NULL;
}

Face::Face(Edge* i_edge) {
    inc_edge = i_edge;
}

bool operator==(Face const& f1, Face const& f2)
{
    return ((f1.inc_edge->org==f2.inc_edge->org)&&
            (f1.inc_edge->dest==f2.inc_edge->dest));
}


/*!
    Class For Polygons represented as DCELs
    -edges is a list containing all the edges in the DCEL
    -vertices is a list containing all the vertices in the DCEL
    -faces is a list containing all the faces in the DCEL
    -n is the count for the number of vertices in the DCEL
    -added_diagonals is vector to contain the extra diagonals made during partitioning
     in the MP1 process so that they can be used during the merging process

*/
class DCEL {
    public:
        list<Edge> edges;
        list<Vertex> vertices;
        list<Face> faces;
        int n;
        vector<pair<Vertex, Vertex>> added_diagonals;

    public:
        DCEL();
        Edge* addEdge(Vertex* v1, Vertex* v2, Edge* e1prev, Edge* e1next, Face* l_face);
        void removeEdge(Edge* e1);
        Vertex* addVertex(Vertex v);
        void removeVertex(Vertex v);
        bool isInteriorPoint(Vertex p);
        Edge* remove(DCEL &l);
        Vertex* findVertex(Vertex v);
        Vertex* findVertexByIndex(int i);
        void save();
};

DCEL::DCEL() {
    edges.clear();
    vertices.clear();
    faces.clear();
    n=0;
}

/*!
Vertices v1 and v2 should be passed in clockwise order
e1prev is the previous edge of the edge made from v1 to v2
e1next is the next
both e1prev and e1next are pointers
pass NULL values if prev or next not known
l_face contains the face to the left of the edge v1 to v2
If no face is passed, function checks for the faces of prev & next otherwise creates new faces
for both the edge and its twin 
*/
Edge* DCEL::addEdge(Vertex* v1, Vertex* v2, Edge* e1prev, Edge* e1next, Face* l_face)
{
    //insert vertices if not already present
    v1 = addVertex(*v1);
    v2 = addVertex(*v2);

    //check if edges already exist
    for(auto it=edges.begin(); it!=edges.end();it++)
        if((*it).org == v1 && (*it).dest == v2)
            return &(*it);

    Edge e1 = Edge(NULL, NULL, NULL, v1, v2, l_face);
    Edge e2 = Edge(NULL, NULL, NULL, v2, v1, NULL);

    edges.push_back(e1);
    edges.push_back(e2);

    Edge* e1p = &(*(----edges.end()));
    Edge* e2p = &(*(--edges.end()));

    if(l_face)
    {
        if(!l_face->inc_edge)
            l_face->inc_edge = e1p;
        else
            e2p->left_face = l_face->inc_edge->twin->left_face;
    }

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

        if(e1prev->left_face)
        {
            e1p->left_face = e1prev->left_face;
            e2p->left_face = e1prev->twin->left_face;
        }
    }

    if(e1next != NULL)
    {
        e1p->next = e1next;
        e2p->prev = e1next->twin;
        e1next->prev = e1p;
        e1next->twin->next = e2p;

        if(e1next->left_face)
        {
            e1p->left_face = e1next->left_face;
            e2p->left_face = e1next->twin->left_face;
        }
    }

    if(!e1p->left_face)
    {
        Face f1 = Face(e1p);
        faces.push_back(f1);
        e1p->left_face = &*(--faces.end());
    }
    if(!e2p->left_face)
    {
        Face f2 = Face(e2p);
        faces.push_back(f2);
        e2p->left_face = &*(--faces.end());
    }

    return e1p;
}

/*!
Will remove the edge e1 as well as its twin
*/
void DCEL::removeEdge(Edge* e1)
{
    Edge *e2 = e1->twin;

    e1->org->inc_edges.erase(e1);
    e2->org->inc_edges.erase(e2);

    //Check if it is incident edge of any face
    if(e1->left_face->inc_edge == e1)
        e1->left_face->inc_edge = NULL;
    if(e2->left_face->inc_edge == e2)
        e2->left_face->inc_edge = NULL;

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

    for(auto it=edges.begin(); it!=edges.end();it++)
        if((it->org==e1->org && it->dest==e1->dest)||
            (it->org==e2->org && it->dest==e2->dest))
            {
                edges.erase(it);
                break;
            }
    for(auto it=edges.begin(); it!=edges.end();it++)
        if((it->org==e1->org && it->dest==e1->dest)||
            (it->org==e2->org && it->dest==e2->dest))
            {
                edges.erase(it);
                break;
            }
}

/*!
Will add vertex v to the DCEL
*/
Vertex* DCEL::addVertex(Vertex v)
{
    Vertex* i = findVertex(v);
    if(i!=NULL)
        return i;

    Vertex v_copy = Vertex(v.x,v.y);

    vertices.push_back(v_copy);
    n++;

    return &(*(--vertices.end()));
}

/*!
Will remove the vertex v as well as all the edges associated with it
*/
void DCEL::removeVertex(Vertex v)
{
    for(auto it=vertices.begin();it!=vertices.end();it++)
        if(*it==v)
        {
            while(!it->inc_edges.empty())
                removeEdge(*(it->inc_edges.begin()));
            vertices.erase(it);
            n--;
            break;
        }
}

/*!
For a closed polygon, function will tell if given point is an interior point of the DCEL
*/
bool DCEL::isInteriorPoint(Vertex p)
{
    bool ans = isReflexAngle(*(--vertices.end()),*vertices.begin(),p);

    auto it=vertices.begin();
    while(it!=(--vertices.end()))
    {
        auto curr = it++;
        if(ans != isReflexAngle(*curr,*(it),p))
            return false;
    }

    return true;
}

/*!
Function will partition the DCEL by removing the DCEL l
*/
Edge* DCEL::remove(DCEL &l)
{
    for(auto it=++l.vertices.begin();it!=--l.vertices.end();it++)
        removeVertex(*it);

    cout<<"finished all remove vertices"<<endl;

    Vertex* v1=NULL;
    Vertex* v2=NULL;

    for(auto it=vertices.begin();it!=vertices.end();it++)
        if(it->inc_edges.size()==1)
        {
            if(!v1){cout<<it->x<<" "<<it->y;
                v1 = &*it;}
            else if(!v2){cout<<it->x<<" "<<it->y;
                v2 = &*it;}
            else
                exit(1);
        }

    added_diagonals.push_back(make_pair(*v1,*v2));

    return addEdge(v1, v2, (*(v1->inc_edges.begin()))->twin, (*(v2->inc_edges.begin())), NULL);
}

/*!
Function to check if given vertex is present in the DCEL and if so return the pointer to it
*/
Vertex* DCEL::findVertex(Vertex v)
{
    for(auto it=vertices.begin();it!=vertices.end();it++)
        if(*it==v)
            return &(*it);

    return NULL;    
}

/*!
Function to find given vertex in DCEL by the index number and return the pointer to it
*/
Vertex* DCEL::findVertexByIndex(int i)
{
    int count=0;
    for(auto it=vertices.begin();it!=vertices.end();it++)
        if(count++ == i)
            return &(*it);

    return NULL;
}

/*!
To Save the current state of the DCEL in vertices and edges file format
*/
void DCEL::save()
{
    ofstream cfile("l_cords.txt");
    map<pair<double,double>, int> vertex_map;
    int index=0;
    for(auto v:vertices)
    {
        vertex_map[v.pairup()] = ++index;
        cfile<<v.x<<" "<<v.y<<endl;
    }
    cfile.close();

    ofstream efile("l_edges.txt");
    for(auto it=edges.begin();it!=edges.end();it++)
        efile<<vertex_map[(*(it->org)).pairup()]<<" "<<vertex_map[(*(it++->dest)).pairup()]<<endl;
    efile.close();
}
