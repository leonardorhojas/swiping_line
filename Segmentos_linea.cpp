#include <iostream>
#include <set> 
#include<stdlib.h> 
#include<stdio.h> 
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <chrono>
#include <random>
#include <vector>
#include <CGAL/point_generators_2.h>
#include <CGAL/hilbert_sort.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/spatial_sort.h>
#include <string>
#include <algorithm>
#include <functional>
#include <cassert>
#include <iomanip>
#include <numeric>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
typedef std::vector<Point_2> Points;
typedef std::vector<Segment_2> Segments;


//--------------------------------------------------------------------------
template< class _TPrint >
void Print_Vector (const _TPrint& p)
{
for(int i=0; i<p.size(); i++)std::cout<<p[i]<<std::endl;//output
}


template< class _TPrint >
double GreenArea (const _TPrint& p)
{
    double area;
for(int i=0; i<p.size(); i++)
{
    if(i<p.size()-1){
area=(p[i].x()*p[i+1].y() - p[i].y()*p[i+1].x())+ area;
    }else{
        area=(p[i].x()+p[0].y() - p[i].y()*p[0].x() )+area;
    }
}
return area;
}


// -------------------------------------------------------------------------
template< class _TPoint >
bool IsPointAtRight( const _TPoint& p, const _TPoint& q, const _TPoint& r )
{
  double qpX = double( q[ 0 ] - p[ 0 ] );
  double qpY = double( q[ 1 ] - p[ 1 ] );
  double rpX = double( r[ 0 ] - p[ 0 ] );
  double rpY = double( r[ 1 ] - p[ 1 ] );

  return( qpX * rpY > rpX * qpY );
}


template <typename T, typename Compare>
void getSortPermutation(
    std::vector<unsigned>& out,
    const std::vector<T>& v,
    Compare compare = std::less<T>())
{
    out.resize(v.size());
    std::iota(out.begin(), out.end(), 0);
 
    std::sort(out.begin(), out.end(),
        [&](unsigned i, unsigned j){ return compare(v[i], v[j]); });
}
 
template <typename T>
void applyPermutation(
    const std::vector<unsigned>& order,
    std::vector<T>& t)
{
    assert(order.size() == t.size());
    std::vector<T> st(t.size());
    for(unsigned i=0; i<t.size(); i++)
    {
        st[i] = t[order[i]];
    }
    t = st;
}
 
template <typename T, typename... S>
void applyPermutation(
    const std::vector<unsigned>& order,
    std::vector<T>& t,
    std::vector<S>&... s)
{
    applyPermutation(order, t);
    applyPermutation(order, s...);
}
 
// sort multiple vectors using the criteria of the first one
template<typename T, typename Compare, typename... SS>
void sortVectors(
    const std::vector<T>& t,
    Compare comp,
    std::vector<SS>&... ss)
{
    std::vector<unsigned> order;
    getSortPermutation(order, t, comp);
    applyPermutation(order, ss...);
}
 
// make less verbose for the usual ascending order
template<typename T, typename... SS>
void sortVectorsAscending(
    const std::vector<T>& t,
    std::vector<SS>&... ss)
{
    sortVectors(t, std::less<T>(), ss...);
}
 

int main(int argc, char* argv[])
{
	
	/* Generar puntos aleatorios y los asigna a un Vector de puntos points */
	 Points points, results,results2,results3;
	 Segments resulting_segments;
     double area1,area2;
/*	 
Generación puntos aleatorios segun profesor Leonardo
*/

  unsigned long n = 10;
  if( argc > 1 )
    n = std::atoi( argv[ 1 ] );
  std::random_device rd;
  std::mt19937 mt( rd( ) );
  std::uniform_real_distribution< double > dist( -10, 10 );

 /* for( unsigned long i = 0; i < n; ++i )
  {
    double x1 = dist( mt );
    double y1 = dist( mt );
    double x2 = dist( mt );
    double y2 = dist( mt );
    resulting_segments.push_back( Segment_2(Point_2( x1, y1 ) ,Point_2( x2, y2 ) ));	
    //Segments.push_back( Point_2( x1, y1 ),Point_2( x2, y2 ) );
  } // end for
  */
	resulting_segments.push_back( Segment_2(Point_2( -0.5, 5.2 ),Point_2(1.5, 5.5 )));
	resulting_segments.push_back( Segment_2(Point_2( 7.5,  0.5 ),Point_2(-7.5, 4  )));
	resulting_segments.push_back( Segment_2(Point_2( 3 ,  -3.5 ),Point_2(3.3, -6.4)));
	resulting_segments.push_back( Segment_2(Point_2( -4,  -5 )  ,Point_2(1.5, 1   )));
//for(int i=0; i<results.size(); i++)std::cout<<"("<<results[i]<<  "),("<< results2[i]<<")"<<std::endl;//output
std::cout<<"***Segmentos de Linea***"<<std::endl;
Print_Vector(resulting_segments);

//sort(resulting_segments.begin(),resulting_segments.end());
//Print_Vector(resulting_segments);

return 0;
}

