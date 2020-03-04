#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <queue>
#include <typeinfo>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
typedef vector<Point_2> Points;
typedef K::Intersect_2 Intersect_2;

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

int main( int argc, char* argv[] )
{
	unsigned int n = 10;
	if( argc > 1 )
		n = std::atoi( argv[ 1 ] );
	std::random_device rd;
	std::mt19937 mt( rd( ) );
	std::uniform_real_distribution< double > dist( -100, 100 );

	Event pe;
	Points result;
	priority_queue<Event,vector<Event>, CompareEvent > pq,test;
	for( unsigned int i = 0; i < n; ++i )
	{
		double x1 = dist( mt );
		double y1 = dist( mt );
		double x2 = dist( mt );
		double y2 = dist( mt );

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
	BST t;
	cout << "BACK : "<<pe.s.point(0)<<"  "<<pe.s.point(1)<<endl;

	test = pq;
	for(int i =0;i<pq.size();i++){
		cout<<test.top().s<<endl;
		test.pop();
	}
	
	while (!pq.empty()) { 
		Event e = pq.top(); 
		pq.pop();
		//t.display();
		cout <<"P : "<< e.s.point(0) <<"  "<< e.s.point(1) << "  "<<e.type <<endl;
		if(e.type==0)
			t.insert(Segment_2(e.s.point(0),e.s.point(1)));
		//cout <<"P : "<< e.s <<e.type <<endl;
		/*if(e.type==0){
			//cout<<"TYPE "<<e.type <<endl;
			t.insert(Segment_2(e.s.point(0),e.s.point(1)));
			node* search =t.search(Segment_2(e.s.point(0),e.s.point(1)));
			//cout<<"LEFT"<<search->left<<endl;
			if(search->left){
				CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
				result = intersection(search->data, search->left->data);
				if (result) {
					if (const Segment_2* s = boost::get<Segment_2>(&*result)) {
						//cout <<"INTER SEG" << *s << endl;
					} else {
						const Point_2* p = boost::get<Point_2 >(&*result);
						//cout <<"INTER"<< *p << endl;
						vector<Segment_2> v{search->data, search->left->data};
						pq.push(Event(Segment_2(*p, *p),2,v));
					}
				}
			}
			//cout<<"RIGHT"<<search->right<<endl;
			if(search->right){
				//cout<<"SI ENTRO"<<endl;
				CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
				result = intersection(search->data, search->right->data);
				//cout<<"RESULT"<<endl;
				if (result) {
					if (const Segment_2* s = boost::get<Segment_2>(&*result)) {
						//cout <<"INTER SEG" << *s << endl;
					} else {
						const Point_2* p = boost::get<Point_2 >(&*result);
						//cout <<"INTER"<< *p << endl;
						vector<Segment_2> v{search->data, search->right->data};
						pq.push(Event(Segment_2(*p, *p),2,v));
					}
				}
			}
			//cout<<"NO ENTRO"<<endl;
		}else if(e.type==1){
			node* search =t.search(Segment_2(e.s.point(0),e.s.point(1)));
			if(search==NULL)
				search =t.search(Segment_2(e.s.point(1),e.s.point(0)));
			if(search->left){
				CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
				result = intersection(search->data, search->left->data);
				if (result) {
					const Point_2* p = boost::get<Point_2 >(&*result);
					vector<Segment_2> v{search->data,search->left->data};
					pq.push(Event(Segment_2(*p, *p),2,v));
				}
			}
			//cout<<"RIGHT"<<search->right<<endl;
			if(search->right){
				CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
				result = intersection(search->data, search->right->data);
				if (result) {
					const Point_2* p = boost::get<Point_2 >(&*result);
					vector<Segment_2> v{search->data, search->right->data};
					pq.push(Event(Segment_2(*p, *p),2,v));
				}
				t.remove(search->data);
			}
		}else{
			cout<<"s1 "<<e.v[0]<<endl;
			cout<<"s2 "<<e.v[1]<<endl;
			t.remove(e.v[0]);
			t.remove(e.v[1]);
			t.insert(Segment_2(e.v[0].point(1), e.v[0].point(0)));
			t.insert(Segment_2(e.v[1].point(1), e.v[1].point(0)));
			node* s1 = t.search(Segment_2(e.v[0].point(1), e.v[0].point(0)));
			node* s2 = t.search(Segment_2(e.v[1].point(1), e.v[1].point(0)));
			cout<<"s1"<<s1->data<<endl;
			cout<<"s2"<<s2->data<<endl;
			cout<<"s1 LEFT "<<s1->left<<endl;
			cout<<"s1 RIGHT"<<s1->right<<endl;
			cout<<"s2 LEFT"<<s2->left<<endl;
			cout<<"s2 RIGHT"<<s2->right<<endl;
			if(s1->left){
				//cout<<"ENTRO 1"<<endl;
				CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
				result = intersection(s1->data, s1->left->data);
				if (result) {
					if (const Point_2 *p = boost::get<Point_2>(&*result) ) {
						vector<Segment_2> v{s1->data,s1->left->data};
						pq.push(Event(Segment_2(*p, *p),2,v));
					}
				}
			//	cout<<"SALIO 1"<<endl;
			}
			if(s1->right){
				//cout<<"ENTRO 2"<<endl;
				CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
				result = intersection(s1->data, s1->right->data);
				if (result) {
					if (const Point_2 *p = boost::get<Point_2>(&*result) ) {
						vector<Segment_2> v{s1->data, s1->right->data};
						pq.push(Event(Segment_2(*p, *p),2,v));
					}
				}
				//cout<<"SALIO 2"<<endl;
			}

			if(s2->left){
				//cout<<"ENTRO 3"<<endl;
				CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
				result = intersection(s2->data, s2->left->data);
				if (result) {
					//cout<<"RES "<<endl;
					if (const Point_2 *p = boost::get<Point_2>(&*result) ) {
						//cout<<"POINT "<<p<<endl;
						vector<Segment_2> v{s2->data,s2->left->data};
						pq.push(Event(Segment_2(*p, *p),2,v));
					}
				}
				//cout<<"SALIO 3"<<endl;
			}
			if(s2->right){
				//cout<<"ENTRO 4"<<endl;
				CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
				result = intersection(s2->data, s2->right->data);
				if (result) {
					if (const Point_2 *p = boost::get<Point_2>(&*result) ) {
						vector<Segment_2> v{s2->data, s2->right->data};
						pq.push(Event(Segment_2(*p, *p),2,v));
					}
				}
				//cout<<"SALIO 4"<<endl;
			}
			result.push_back(e.s.point(0));
		}*/
	}

	node* a =t.search(Segment_2(pe.s.point(0),pe.s.point(1)));
	cout <<"SEARCH : "<< a->data.point(0) << "  "<< a->data.point(1) << endl; 
	t.display();
	t.remove(Segment_2(a->data.point(0),a->data.point(1)));
	t.display();
		// RESULT
	/*cout << "============  INSERTSECTION POINTS =================" << endl;
	auto start = chrono::steady_clock::now( );
	for(int i = 0; i < result.size(); i++)
		cout << result[i] << endl;
	auto end = chrono::steady_clock::now( );
	auto t_incremental = chrono::duration_cast< chrono::nanoseconds >( end - start ).count( );
	cout << result.size( ) << " (" << ( double( t_incremental ) / 1e9 ) << " s)" << endl;*/
}