#ifndef CORD_H
#define CORD_H

#ifndef ULIBDEFS_H
  #include "ulibdefs.h"
#endif
#ifndef SIMGC_H
  #include "simgc.h"
#endif

typedef struct TCordSegment_tag
{
  const struct TCordSegment_tag * pNext;
  const struct TCordSegment_tag * pLast;

  size_t totalLen;
  size_t segLen;     // can never be 0 on a subsequent segment

  // Make sure the data is aligned 
  union
  {
    AlignAll align;
    char     data[1];
  } u;
} TCordSegment;

typedef const TCordSegment * CORD;

void TCordSegment_GcMark ( const TCordSegment * seg );
#define CORD_GcMark TCordSegment_GcMark

TCordSegment * Cord_AllocSegment ( size_t len );
CORD Cord_AddSegment ( CORD pRoot, TCordSegment * pSeg );
CORD Cord_NewSegment ( CORD pRoot, size_t len, const void * newData );

CORD Cord_AppendCord ( CORD pRoot, CORD toAdd );


/*
  Cord iteration typical usage:

  CORD cord;
  TCordIterator iter;
  char c;

  CordIterator_Init( &iter, cord );
  while (CordIterator_GetNextChar( &iter, &c )
    printf( "%c", c );
*/

typedef struct TCordIterator_tag
{
  const TCordSegment * pCurSeg;
  size_t index;
} TCordIterator;

//--------------------------------------------------------------------------
// Name         CordIterator_Init 
//
// 
//--------------------------------------------------------------------------
#define CordIterator_Init( pIter, cord ) \
  ((void)( (pIter)->pCurSeg = cord, (pIter)->index = 0 ))

//--------------------------------------------------------------------------
// Name         CordIterator_GetNextChar
//
// 
//--------------------------------------------------------------------------
#define CordIterator_GetNextChar( pIter, pChar )                   \
  (                                                                \
    ASSERT( (pIter)->pCurSeg != NULL ),                            \
                                                                   \
    ((pIter)->index == (pIter)->pCurSeg->segLen &&                 \
      ((pIter)->index = 0, true) &&                                \
      ((pIter)->pCurSeg = (pIter)->pCurSeg->pNext) == NULL)        \
    ?                                                              \
    (                                                              \
      false                                                        \
    ) :                                                            \
    (                                                              \
      *(pChar) = (pIter)->pCurSeg->u.data[(pIter)->index++],       \
      true                                                         \
    )                                                              \
  )

//--------------------------------------------------------------------------
// Name         CordIterator_GetNextChunk
//
// This takes a chunk of data, possibly longer than 1 character. 
//--------------------------------------------------------------------------
#define CordIterator_GetNextChunk( pIter, pChunk, len )            \
  ((len) == 1 ? CordIterator_GetNextChar( pIter, (char *)pChunk ) :\
                CordIterator_GetData( pIter, pChunk, len ))

bool CordIterator_GetData ( TCordIterator * iter, void * pChunk, size_t len );


#endif
