#include <chrono>
#include <iostream>
#include <random>
#include <iterator>
#include <vector>
#include <queue>
#include <typeinfo>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

// -------------------------------------------------------------------------

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef std::vector< Point_2 > Points;
typedef K::Segment_2 Segment_2;
typedef K::Intersect_2 Intersect_2;
typedef std::vector<Segment_2> Segments;

// -------------------------------------------------------------------------

struct Event{
	Segment_2 s;
	int type;
	vector<Segment_2> v;
	Event() {};
	Event(Segment_2 s, int type) : s(s),type(type) {};
	Event(Segment_2 s, int type,vector<Segment_2> v) : s(s),type(type),v(v) {};  //initialization of Event
};


struct node
{
	Segment_2 data;
	node* left;
	node* right;
	int height;
};

struct CompareEvent { 
	bool operator()(Event const& a, Event const& b) 
	{   
		Point_2 ptoCompareA, ptoCompareB;
		if(a.type==0)
			ptoCompareA = a.s.point(0);
		else
			ptoCompareA = a.s.point(1);
		if(b.type==0)
			ptoCompareB = b.s.point(0);
		else
			ptoCompareB = b.s.point(1);

		if(ptoCompareA.y()==ptoCompareB.y())
			return ptoCompareA.x()<ptoCompareB.x();
		else
			return ptoCompareA.y()<ptoCompareB.y();
	}
};

bool compareSegment(Segment_2 s1, Segment_2 s2){
	return s1.point(0).x()>s2.point(0).x();
}

class BST
{
	node* root;

	void makeEmpty(node* t)
	{
		if(t == NULL)
			return;
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}

	node* insert(Segment_2 x, node* t)
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

	node* remove(Segment_2 x, node* t)
	{
		node* temp;

				// Element not found
		if(t!=NULL){
			cout<<"T : "<<t->data<<endl;	
			//impromir derecha e izquierda
				}
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
			cout<<"GOOD BYE"<<temp->data<<endl;
			temp = t;
			if(t->left == NULL)
				t = t->right;
			else if(t->right == NULL)
				t = t->left;
			cout<<"BYE"<<temp->data<<endl;
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
		cout <<"("<< t->data <<")"<< " ";
		inorder(t->right);
	}

	node* search(Segment_2 s, node* root){
		if(root->data==s)
			return root;
		if(!compareSegment(s,root->data))
			return search(s,root->left);
		else
			return search(s,root->right);
		return NULL;   
	}

public:
	BST()
	{
		root = NULL;
	}

	void insert(Segment_2 x)
	{
		root = insert(x, root);
	}

	void remove(Segment_2 x)
	{
		root = remove(x, root);
	}

	void display()
	{
		inorder(root);
		cout << endl;
	}

	node* search(Segment_2 x){
		return search(x,root);
	}
};

/* public:
Populate_tree(){


} */

double rnd_point(){
	// unsigned int n = 10;
	// if( argc > 1 )
	// 	n = std::atoi( argv[ 1 ] );
	std::random_device rd;
	std::mt19937 mt( rd( ) );
	std::uniform_real_distribution< double > dist( -10, 10 );
	double point=dist( mt );
	return point;

}


int main( int argc, char* argv[] )
{
	unsigned int n = 10;
	Points points;
	for( unsigned int i = 0; i < n; ++i )
	{
	
	points.push_back(Point_2(rnd_point(),rnd_point()));
	} 

	Event pe;
	Points result;
	priority_queue<Event,vector<Event>, CompareEvent > pq,test;

	for( unsigned int i = 0; i < n; ++i )
	{
		double x1 = points[i].x();
		double y1 = points[i].y();
		i++;
		double x2 = points[i].x();
		double y2 = points[i].y();

		if(y1==y2){
			if(x1>x2){
				pq.push(Event(Segment_2(Point_2(x1,y1), Point_2(x2,y2)),0));
				pq.push(Event(Segment_2(Point_2(x1,y1), Point_2(x2,y2)),1));

			} else{
				pq.push(Event(Segment_2(Point_2(x2,y2), Point_2(x1,y1)),0));
				pq.push(Event(Segment_2(Point_2(x2,y2), Point_2(x1,y1)),1));
			}
		}else{
			if(y1>y2){
				pq.push(Event(Segment_2(Point_2(x1,y1), Point_2(x2,y2)),0));
				pq.push(Event(Segment_2(Point_2(x1,y1), Point_2(x2,y2)),1));

			} else{
				pq.push(Event(Segment_2(Point_2(x2,y2), Point_2(x1,y1)),0));
				pq.push(Event(Segment_2(Point_2(x2,y2), Point_2(x1,y1)),1));
			}
		}
		if(i==1)
			pe = pq.top();
	}

	std::cout << "Tamaño del arreglo  : " << points.size( ) << std::endl << "\t-> ";
 	std::copy(
    points.begin( ), points.end( ),
    std::ostream_iterator< Point_2 >( std::cout, " | " )
    );
  	std::cout << std::endl;

  	
	return 0;

	// BST t;
	// cout << "BACK : "<<pe.s.point(0)<<"  "<<pe.s.point(1)<<endl;

	// test = pq;
	// for(int i =0;i<pq.size();i++){
	// 	cout<<test.top().s<<endl;
	// 	test.pop();
	// }
	
	// while (!pq.empty()) { 
	// 	Event e = pq.top(); 
	// 	pq.pop();
	// 	//t.display();
	// 	cout <<"P : "<< e.s.point(0) <<"  "<< e.s.point(1) << "  "<<e.type <<endl;
	// 	if(e.type==0)
	// 		t.insert(Segment_2(e.s.point(0),e.s.point(1)));
	
	// }

	// node* a =t.search(Segment_2(pe.s.point(0),pe.s.point(1)));
	// cout <<"SEARCH : "<< a->data.point(0) << "  "<< a->data.point(1) << endl; 
	// t.display();
	// t.remove(Segment_2(a->data.point(0),a->data.point(1)));
	// t.display();

		// RESULT
	/*cout << "============  INSERTSECTION POINTS =================" << endl;
	auto start = chrono::steady_clock::now( );
	for(int i = 0; i < result.size(); i++)
		cout << result[i] << endl;
	auto end = chrono::steady_clock::now( );
	auto t_incremental = chrono::duration_cast< chrono::nanoseconds >( end - start ).count( );
	cout << result.size( ) << " (" << ( double( t_incremental ) / 1e9 ) << " s)" << endl;*/
}