#ifndef CORD_H
#define CORD_H

#include "hconf.h"

typedef struct TCordSegment
{
  const struct TCordSegment * pNext;
  const struct TCordSegment * pLast;

  size_t totalLen;
  size_t segLen;

  // Make sure the data is aligned 
  union
  {
    AlignAll align;
    char     data[1];
  } u;
} TCordSegment;

typedef const TCordSegment * CORD;

TCordSegment * Cord_AllocSegment ( size_t len );
CORD Cord_AddSegment ( CORD pRoot, TCordSegment * pSeg );
CORD Cord_NewSegment ( CORD pRoot, size_t len, const void * newData );

CORD Cord_AppendCord ( CORD pRoot, CORD toAdd );

#endif
