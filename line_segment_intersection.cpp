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

handleEventPoint(Event e){
	
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
		if(y==y1)
			pq.push(Event(Point_2(x,y), Point_2(x1,y1),0));
		else
			pq.push(Event(Point_2(x1,y1), Point_2(x,y),0));
	}

	while (!pq.empty()) { 
        Event e = pq.top(); 
        pq.pop(); 
        cout << e.p1 << " " << e.p2 << "\n"; 
    } 
}