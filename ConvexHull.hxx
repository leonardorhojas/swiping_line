#ifndef __ConvexHull__hxx__
#define __ConvexHull__hxx__

#include <set>
#include <CGAL/Line_2.h>

// -------------------------------------------------------------------------
template< class _TInIt, class _TOutIt >
void ConvexHull::
BruteForce( _TInIt in_b, _TInIt in_e, _TOutIt out )
{
  typedef typename _TInIt::value_type                     _TPoint;
  typedef typename CGAL::Kernel_traits< _TPoint >::Kernel _TKernel;
  typedef typename _TKernel::Line_2                       _TLine;

  for( _TInIt i = in_b; i != in_e; ++i )
  {
    for( _TInIt j = in_b; j != in_e; ++j )
    {
      if( i != j )
      {
        _TLine l( *i, *j );
        bool all_right = true;
        for( _TInIt k = in_b; k != in_e; ++k )
        {
          if( i != k && j != k )
            all_right &= ( l.oriented_side( *k ) != CGAL::ON_NEGATIVE_SIDE );
        } // end for

        if( all_right )
        {
          *out = *i;
          *out = *j;
        } // end if

      } // end if
    } // end for
  } // end for
}

// -------------------------------------------------------------------------
template< class _TInIt, class _TOutIt >
void ConvexHull::
Incremental( _TInIt in_b, _TInIt in_e, _TOutIt out )
{
  typedef typename _TInIt::value_type                     _TPoint;
  typedef typename CGAL::Kernel_traits< _TPoint >::Kernel _TKernel;
  typedef typename _TKernel::Line_2                       _TLine;
  struct _TPointCmp
  {
    bool operator()( _TInIt a, _TInIt b ) const
      {
        if( ( *a )[ 0 ] == ( *b )[ 0 ] )
          return( ( *b )[ 1 ] < ( *a )[ 1 ] );
        else
          return( ( *a )[ 0 ] < ( *b )[ 0 ] );
      }
  };
  typedef std::set< _TInIt, _TPointCmp > _TOrderedPoints;

  // Order points
  _TOrderedPoints ord_points;
  for( _TInIt i = in_b; i != in_e; ++i )
    ord_points.insert( i );

  // Upper hull
  typename _TOrderedPoints::const_iterator uIt = ord_points.begin( );
  std::vector< _TInIt > uh;
  uh.push_back( *( uIt++ ) );
  uh.push_back( *( uIt++ ) );

  for( ; uIt != ord_points.end( ); ++uIt )
  {
    uh.push_back( *uIt );
    while(
      uh.size( ) > 2 &&
      !( CGAL::right_turn(
           **( uh.rbegin( ) + 2 ),
           **( uh.rbegin( ) + 1 ),
           **( uh.rbegin( ) )
           )
        )
      )
      uh.erase( ( uh.rbegin( ) + 2 ).base( ) );
  } // end for

  // Lower hull
  typename _TOrderedPoints::const_reverse_iterator lIt = ord_points.rbegin( );
  std::vector< _TInIt > lh;
  lh.push_back( *( lIt++ ) );
  lh.push_back( *( lIt++ ) );

  for( ; lIt != ord_points.rend( ); ++lIt )
  {
    lh.push_back( *lIt );
    while(
      lh.size( ) > 2 &&
      !( CGAL::right_turn(
           **( lh.rbegin( ) + 2 ),
           **( lh.rbegin( ) + 1 ),
           **( lh.rbegin( ) )
           )
        )
      )
      lh.erase( ( lh.rbegin( ) + 2 ).base( ) );
  } // end for

  // Copy hulls into output
  for( _TInIt p: uh )
    *out = *p;
  for( unsigned long i = 1; i < lh.size( ) - 1; ++i )
    *out = *lh[ lh.size( ) - i ];
}

#endif // __ConvexHull__hxx__

// eof - ConvexHull.hxx
