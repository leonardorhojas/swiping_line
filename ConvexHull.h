#ifndef __ConvexHull__h__
#define __ConvexHull__h__

namespace ConvexHull
{
  //! Convex hull, brute force version
  template< class _TInIt, class _TOutIt >
  void BruteForce( _TInIt in_b, _TInIt in_e, _TOutIt out );

  //! Convex hull, incremental version
  template< class _TInIt, class _TOutIt >
  void Incremental( _TInIt in_b, _TInIt in_e, _TOutIt out );
} // end namespace

#include "ConvexHull.hxx"

#endif // __ConvexHull__h__

// eof - ConvexHull.h