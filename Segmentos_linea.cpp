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

  for( unsigned long i = 0; i < n; ++i )
  {
    double x = dist( mt );
    double y = dist( mt );
    points.push_back( Point_2( x, y ) );
  } // end for
	
	
/*Crear vector de Segmentos de linea p-q*/
	for (int p=0; p<points.size();p++)
	{
		for (int q=0; q<points.size();q++)
		{
			if (p!=q){
				bool valid=true;
			for (int r=0;r<points.size();r++)
				if(r!=p and r!=q){
				 valid &= IsPointAtRight( points[p], points[q], points[r] );
					
				}

/*Genera vector de segmentos de linea*/       
			if (valid==true){
			//resulting_segments.push_back( Segment_2(points[p] ,points[q]) )	;
      results.push_back(points[p]);
      results.push_back(points[q]);
     // results2.push_back(points[q]);
				}
			}
		}
	}

//for(int i=0; i<results.size(); i++)std::cout<<"("<<results[i]<<  "),("<< results2[i]<<")"<<std::endl;//output
std::cout<<"***Naive Algorithm***"<<std::endl;

//crea un set para dejar sin repeticiones los puntos del convex hull
std::set <Point_2> result_set(results.begin(), results.end());
results.assign(result_set.begin(),result_set.end());

 //CGAL::hilbert_sort (results.begin(),results.end());
  //CGAL::spatial_sort(results.begin(),results.end());
  sort(results.begin(),results.end());


Print_Vector(results);
area1=GreenArea(results);
std::cout<<"======="<<std::endl;
std::cout<<"GREEN Area Naive algorithm:"<<area1<<std::endl;
//sort(results.begin(),results.end());
std::cout<<"******"<<std::endl;
 
//Organiza vectores según la coordenada X  https://tuket.github.io/2017-09-30-sorting-multiple-vectors-in-the-same-way-using-c-variadic-templates/
//sortVectors(results, std::less<Point_2>(), results, results2);

std::cout<<"***CGAL Yarvis Algorithm***"<<std::endl;
CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter(results3) );

//for(int i=0; i<results.size(); i++)std::cout<<results[i]<<  ", "<< results2[i]<<std::endl;//output
Print_Vector(results3);
area2=GreenArea(results3);
std::cout<<"======="<<std::endl;
std::cout<<"GREEN Area Yarvis algorithm:"<<area2<<std::endl;

return 0;
}

