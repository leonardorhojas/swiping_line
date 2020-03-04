#ifndef __GreenArea__h__
#define __GreenArea__h__

//! Convex hull, brute force version
template< class _TInIt >
double GreenArea( _TInIt in_b, _TInIt in_e )
{
  double a = 0;
  _TInIt i = in_b;
  _TInIt j = i;
  for( j++; j != in_e; ++i, ++j )
    a += ( *i )[ 0 ] * ( *j )[ 1 ] - ( *i )[ 1 ] * ( *j )[ 0 ];
  a += ( *i )[ 0 ] * ( *in_b )[ 1 ] - ( *i )[ 1 ] * ( *in_b )[ 0 ];
  return( a / 2.0 );
}

#endif // __GreenArea__h__

// eof - GreenArea.h