#include "precomp.h"
#include "gc\gc.h"

//--------------------------------------------------------------------------
// Name         gcAlloc 
//
// 
//--------------------------------------------------------------------------
void * gcAlloc ( size_t n )
{
  void * p;
  if ((p = GC_MALLOC( n )) == NULL)
    abort(); // TODO:
  return p;
};

//--------------------------------------------------------------------------
// Name         gcAllocAtomic 
//
// 
//--------------------------------------------------------------------------
void * gcAllocAtomic ( size_t n )
{
  void * p;
  if ((p = GC_MALLOC_ATOMIC( n )) == NULL)
    abort(); // TODO:
  return p;
};

//--------------------------------------------------------------------------
// Name         gcFree 
//
// 
//--------------------------------------------------------------------------
void gcFree ( void * p )
{
  GC_FREE( p );
};


