#include "precomp.h"
#include "cord.h"
#include "heap.h"


//--------------------------------------------------------------------------
// Name         Cord_AllocSegment 
//
// 
//--------------------------------------------------------------------------
TCordSegment * Cord_AllocSegment ( size_t len )
{
  TCordSegment * pSeg;

  pSeg = (TCordSegment *)gcAlloc( offsetof(TCordSegment,u.data) + len );
  pSeg->segLen = len;

  return pSeg;
};

//--------------------------------------------------------------------------
// Name         Cord_AddSegment 
//
// 
//--------------------------------------------------------------------------
CORD Cord_AddSegment ( CORD pRoot, TCordSegment * pSeg )
{
  if (pRoot != NULL)
    ((TCordSegment *)pRoot->pLast)->pNext = pSeg;
  else
    pRoot = pSeg;

  ((TCordSegment *)pRoot)->pLast = pSeg;
  ((TCordSegment *)pRoot)->totalLen += pSeg->segLen;
  return pRoot;
};

//--------------------------------------------------------------------------
// Name         Cord_NewSegment 
//
// 
//--------------------------------------------------------------------------
CORD Cord_NewSegment ( CORD pRoot, size_t len, const void * newData )
{
  TCordSegment * pSeg = Cord_AllocSegment( len );
  memcpy( pSeg->u.data, newData, len );
  return Cord_AddSegment( pRoot, pSeg );
};

//--------------------------------------------------------------------------
// Name         Cord_AppendCord 
//
// 
//--------------------------------------------------------------------------
CORD Cord_AppendCord ( CORD pRoot, CORD toAdd )
{
  if (pRoot == NULL)
    return toAdd;

  ((TCordSegment *)pRoot->pLast)->pNext = toAdd;

  ((TCordSegment *)pRoot)->pLast = toAdd->pLast;
  ((TCordSegment *)pRoot)->totalLen += toAdd->totalLen;

  return pRoot;
};
