#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef vector<Point_2> Points;

struct Event{
    Point_2 p1,p2;
    // Lista de segmentos
    int type;
    Event() {};
    Event(Point_2 p1,Point_2 p2, int type) : p1(p1), p2(p2),type(type) {};  //initialization of Event
};


struct CompareEvent { 
    bool operator()(Event const& a, Event const& b) 
    {   
        Point_2 ptoCompareA, ptoCompareB;
        if(a.type==0)
            ptoCompareA = a.p1;
        else
            ptoCompareA = a.p2;
        if(b.type==0)
            ptoCompareB = b.p1;
        else
            ptoCompareB = b.p2;

        if(ptoCompareA.y()==ptoCompareB.y())
            return ptoCompareA.x()>ptoCompareB.x();
        else
            return ptoCompareA.y()>ptoCompareB.y();
    }
};

struct Segment{
   Point_2 p1,p2;
   Segment() {};
   Segment(Point_2 p1,Point_2 p2) : p1(p1), p2(p2) {}; 
};

bool compareSegment(Segment s1, Segment s2){
        return s1.p1.x()>s1.p2.x();
}

class BST
{
    struct node
    {
        Segment data;
        node* left;
        node* right;
        int height;
    };

    node* root;

    void makeEmpty(node* t)
    {
        if(t == NULL)
            return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }

    node* insert(Segment x, node* t)
    {
        if(t == NULL)
        {
            t = new node;
            t->data = x;
            t->height = 0;
            t->left = t->right = NULL;
        }
        else if(!compareSegment(x,t->data))
        {
            t->left = insert(x, t->left);
            if(height(t->left) - height(t->right) == 2)
            {
                if(!compareSegment(x,t->left->data))
                    t = singleRightRotate(t);
                else
                    t = doubleRightRotate(t);
            }
        }
        else if(compareSegment(x,t->data))
        {
            t->right = insert(x, t->right);
            if(height(t->right) - height(t->left) == 2)
            {
                if(compareSegment(x,t->right->data))
                    t = singleLeftRotate(t);
                else
                    t = doubleLeftRotate(t);
            }
        }

        t->height = max(height(t->left), height(t->right))+1;
        return t;
    }

    node* singleRightRotate(node* &t)
    {
        node* u = t->left;
        t->left = u->right;
        u->right = t;
        t->height = max(height(t->left), height(t->right))+1;
        u->height = max(height(u->left), t->height)+1;
        return u;
    }

    node* singleLeftRotate(node* &t)
    {
        node* u = t->right;
        t->right = u->left;
        u->left = t;
        t->height = max(height(t->left), height(t->right))+1;
        u->height = max(height(t->right), t->height)+1 ;
        return u;
    }

    node* doubleLeftRotate(node* &t)
    {
        t->right = singleRightRotate(t->right);
        return singleLeftRotate(t);
    }

    node* doubleRightRotate(node* &t)
    {
        t->left = singleLeftRotate(t->left);
        return singleRightRotate(t);
    }

    node* findMin(node* t)
    {
        if(t == NULL)
            return NULL;
        else if(t->left == NULL)
            return t;
        else
            return findMin(t->left);
    }

    node* findMax(node* t)
    {
        if(t == NULL)
            return NULL;
        else if(t->right == NULL)
            return t;
        else
            return findMax(t->right);
    }

    node* remove(Segment x, node* t)
    {
        node* temp;

        // Element not found
        if(t == NULL)
            return NULL;

        // Searching for element
        else if(!compareSegment(x,t->data))
            t->left = remove(x, t->left);
        else if(compareSegment(x,t->data))
            t->right = remove(x, t->right);

        // Element found
        // With 2 children
        else if(t->left && t->right)
        {
            temp = findMin(t->right);
            t->data = temp->data;
            t->right = remove(t->data, t->right);
        }
        // With one or zero child
        else
        {
            temp = t;
            if(t->left == NULL)
                t = t->right;
            else if(t->right == NULL)
                t = t->left;
            delete temp;
        }
        if(t == NULL)
            return t;

        t->height = max(height(t->left), height(t->right))+1;

        // If node is unbalanced
        // If left node is deleted, right case
        if(height(t->left) - height(t->right) == 2)
        {
            // right right case
            if(height(t->left->left) - height(t->left->right) == 1)
                return singleLeftRotate(t);
            // right left case
            else
                return doubleLeftRotate(t);
        }
        // If right node is deleted, left case
        else if(height(t->right) - height(t->left) == 2)
        {
            // left left case
            if(height(t->right->right) - height(t->right->left) == 1)
                return singleRightRotate(t);
            // left right case
            else
                return doubleRightRotate(t);
        }
        return t;
    }

    int height(node* t)
    {
        return (t == NULL ? -1 : t->height);
    }

    int getBalance(node* t)
    {
        if(t == NULL)
            return 0;
        else
            return height(t->left) - height(t->right);
    }

    void inorder(node* t)
    {
        if(t == NULL)
            return;
        inorder(t->left);
        cout <<"("<< t->data.p1 <<")"<< " ";
        inorder(t->right);
    }

public:
    BST()
    {
        root = NULL;
    }

    void insert(Segment x)
    {
        root = insert(x, root);
    }

    void remove(Segment x)
    {
        root = remove(x, root);
    }

    void display()
    {
        inorder(root);
        cout << endl;
    }
};

int main( int argc, char* argv[] )
{
    unsigned int n = 10;
    if( argc > 1 )
        n = std::atoi( argv[ 1 ] );
    std::random_device rd;
    std::mt19937 mt( rd( ) );
    std::uniform_real_distribution< double > dist( -100, 100 );

    Points P;
    priority_queue<Event,vector<Event>, CompareEvent > pq;
    for( unsigned int i = 0; i < n; ++i )
    {
        double x1 = dist( mt );
        double y1 = dist( mt );
        double x2 = dist( mt );
        double y2 = dist( mt );
        if(y1==y2){
            if(x1<x2){
                pq.push(Event(Point_2(x1,y1), Point_2(x2,y2),0));
                pq.push(Event(Point_2(x1,y1), Point_2(x2,y2),1));

            } else{
                pq.push(Event(Point_2(x2,y2), Point_2(x1,y1),0));
                pq.push(Event(Point_2(x2,y2), Point_2(x1,y1),1));
            }
        }else{
            if(y1<y2){
                pq.push(Event(Point_2(x1,y1), Point_2(x2,y2),0));
                pq.push(Event(Point_2(x1,y1), Point_2(x2,y2),1));

            } else{
                pq.push(Event(Point_2(x2,y2), Point_2(x1,y1),0));
                pq.push(Event(Point_2(x2,y2), Point_2(x1,y1),1));
            }
        }
    }
    BST t;

    while (!pq.empty()) { 
        Event e = pq.top(); 
        pq.pop();
        //cout << e.p1 << " " << e.p2 << "\n";
        t.insert(Segment(e.p1,e.p2));
        //cout << "Inorder traversal for the AVL tree is: \n";  
        //inOrder(root);
    } 
    t.display();

    //printTree(first, nullptr, false);
}