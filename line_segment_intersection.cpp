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

<<<<<<< HEAD
bool compare(Point_2 p1,Point_2 p2){
	if(p1.y()==p2.y())
		return p1.x()>p2.x();
	else
		return p1.y()>p2.y();
}

=======
>>>>>>> fc142eade93dc0db8587d201cba903d97ad0edb3
struct Event{
	Point_2 p1,p2;
	int type;
	Event() {};
    Event(Point_2 p1,Point_2 p2, int type) : p1(p1), p2(p2),type(type) {};  //initialization of Event
};

struct CompareEvent { 
	bool operator()(Event const& a, Event const& b) 
    {
        return a.p1.y()<b.p1.y();
    } 
};

<<<<<<< HEAD
struct Segment{
	Point_2 p1,p2;
	Segment() {};
    Segment(Point_2 p1,Point_2 p2) : p1(p1), p2(p2) {}; 
}

struct Node{
	Segment data;
	Node *left, *right;
};

Node* newNode(Segment data){
	Node* node = new Node();
	node->p1=data.p1;
	node->p2=data.p2;
	node->left = node->right = nullptr;
	return node;
}
+
void inorder(Node* root)
{
	if (root == nullptr)
		return;

	inorder(root->left);
	cout << root->data << " ";
	inorder(root->right);
}

Node* insert(Node* root, Segment data){
	if(root==nullptr)
		return newNode(data);

	if(compare(data,root->data))
		root->left = insert(root->right,data);
	else
		root->right = insert(root->right,data);
	return root;
}

handleEventPoint(Event e){

=======
handleEventPoint(Event e){
	
>>>>>>> fc142eade93dc0db8587d201cba903d97ad0edb3
}

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
		double x = dist( mt );
		double y = dist( mt );
		double x1 = dist( mt );
		double y1 = dist( mt );
<<<<<<< HEAD
		if(y==y1){
			pq.push(Event(Point_2(x,y), Point_2(x1,y1),0));
			pq.push(Event(Point_2(x1,y1), Point_2(x,y),1));
		}
		else{
			pq.push(Event(Point_2(x1,y1), Point_2(x,y),0));
			pq.push(Event(Point_2(x,y), Point_2(x1,y1),0));
		}
=======
		if(y==y1)
			pq.push(Event(Point_2(x,y), Point_2(x1,y1),0));
		else
			pq.push(Event(Point_2(x1,y1), Point_2(x,y),0));
>>>>>>> fc142eade93dc0db8587d201cba903d97ad0edb3
	}

	while (!pq.empty()) { 
        Event e = pq.top(); 
<<<<<<< HEAD
        pq.pop();
        cout << e.p1 << " " << e.p2 << "\n";
        root = newNode(new Segment(e.p1, e.p2));
        inorder(root);
=======
        pq.pop(); 
        cout << e.p1 << " " << e.p2 << "\n"; 
>>>>>>> fc142eade93dc0db8587d201cba903d97ad0edb3
    } 
}