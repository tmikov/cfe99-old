#include "ulibpriv.h"
#include "cord.h"

//--------------------------------------------------------------------------
// Name         TCordSegment_GcMark 
//
// 
//--------------------------------------------------------------------------
void TCordSegment_GcMark ( const TCordSegment * seg )
{
  GC_MARK( seg->pNext );
  //GC_MARK( seg->pLast );
};

//--------------------------------------------------------------------------
// Name         Cord_AllocSegment 
//
// 
//--------------------------------------------------------------------------
TCordSegment * Cord_AllocSegment ( size_t len )
{
  TCordSegment * pSeg;

  if (len == 0)
    pSeg = NULL;
  else
  {
    pSeg = GC_ALLOC( TCordSegment, offsetof(TCordSegment,u.data) + len );
    pSeg->segLen = len;
  }

  return pSeg;
};

//--------------------------------------------------------------------------
// Name         Cord_AddSegment 
//
// 
//--------------------------------------------------------------------------
CORD Cord_AddSegment ( CORD pRoot, TCordSegment * pSeg )
{
  if (pSeg != NULL)
  {
    if (pRoot != NULL)
      ((TCordSegment *)pRoot->pLast)->pNext = pSeg;
    else
      pRoot = pSeg;

    ((TCordSegment *)pRoot)->pLast = pSeg;
    ((TCordSegment *)pRoot)->totalLen += pSeg->segLen;
  }
  return pRoot;
};

//--------------------------------------------------------------------------
// Name         Cord_NewSegment 
//
// 
//--------------------------------------------------------------------------
CORD Cord_NewSegment ( CORD pRoot, size_t len, const void * newData )
{
  if (len != 0)
  {
    TCordSegment * pSeg = Cord_AllocSegment( len );
    memcpy( pSeg->u.data, newData, len );
    pRoot = Cord_AddSegment( pRoot, pSeg );
  }

  return pRoot;
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

  if (toAdd->totalLen != 0)
  {
    ((TCordSegment *)pRoot->pLast)->pNext = toAdd;

    ((TCordSegment *)pRoot)->pLast = toAdd->pLast;
    ((TCordSegment *)pRoot)->totalLen += toAdd->totalLen;
  }

  return pRoot;
};


//--------------------------------------------------------------------------
// Name         CordIterator_GetData 
//
// 
//--------------------------------------------------------------------------
bool CordIterator_GetData ( TCordIterator * iter, void * pChunk, size_t len )
{
  char * pDest = (char *)pChunk;
  // TODO: optimize
  ASSERT( len > 0 );
  do
  {
    if (!CordIterator_GetNextChar( iter, pDest++ ))
      return false;
  }
  while (--len);
  return true;
};
