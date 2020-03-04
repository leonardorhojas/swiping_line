#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <iterator>
#include <vector>
#include "ConvexHull.h"
#include "GreenArea.h"

// -------------------------------------------------------------------------
typedef CGAL::Exact_predicates_inexact_constructions_kernel TKernel;
typedef TKernel::Point_2      TPoint;
typedef std::vector< TPoint > TPoints;

// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  TPoints points;
  points.push_back( TPoint(  0,  0 ) );
  points.push_back( TPoint( 10,  0 ) );
  points.push_back( TPoint( 10, 10 ) );
  points.push_back( TPoint(  6,  5 ) );
  points.push_back( TPoint(  4,  1 ) );

  // Brute force
  TPoints bf_ch;
  ConvexHull::BruteForce(
    points.begin( ), points.end( ), std::back_inserter( bf_ch )
    );

  // Incremental
  TPoints inc_ch;
  ConvexHull::Incremental(
    points.begin( ), points.end( ), std::back_inserter( inc_ch )
    );

  // Native CGAL implementation
  TPoints cgal_ch;
  CGAL::convex_hull_2(
    points.begin( ), points.end( ), std::back_inserter( cgal_ch )
    );

  // Show results
  std::cout << "BF          : " << bf_ch.size( ) << std::endl << "\t-> ";
  std::copy(
    bf_ch.begin( ), bf_ch.end( ),
    std::ostream_iterator< TPoint >( std::cout, " | " )
    );
  std::cout << std::endl;
  std::cout << "\t-> Area: " << GreenArea( bf_ch.begin( ), bf_ch.end( ) ) << std::endl;
  std::cout << "Incremental : " << inc_ch.size( ) << std::endl << "\t-> ";
  std::copy(
    inc_ch.begin( ), inc_ch.end( ),
    std::ostream_iterator< TPoint >( std::cout, " | " )
    );
  std::cout << std::endl;
  std::cout << "\t-> Area: " << GreenArea( inc_ch.begin( ), inc_ch.end( ) ) << std::endl;
  std::cout << "CGAL        : " << cgal_ch.size( ) << std::endl << "\t-> ";
  std::copy(
    cgal_ch.begin( ), cgal_ch.end( ),
    std::ostream_iterator< TPoint >( std::cout, " | " )
    );
  std::cout << std::endl;
  std::cout << "\t-> Area: " << GreenArea( cgal_ch.begin( ), cgal_ch.end( ) ) << std::endl;
  return( 0 );
}

// eof - test.cxx