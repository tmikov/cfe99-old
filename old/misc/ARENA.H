#ifndef ARENA_H
#define ARENA_H

#include "misc\clist.h"

typedef struct TArena
{
  TListRoot   root;
  size_t      pageSize;
} TArena;

void Arena_Init ( TArena * pArena, size_t pageSize );
void Arena_Done ( TArena * pArena );
void Arena_Free ( TArena * pArena );
void * Arena_Alloc ( TArena * pArena, size_t size );

// Currently the memory is always zeroed
#define Arena_CAlloc( pArena, size )  Arena_Alloc( pArena, size )

#define ARENA_NEW( arena, p )   ((p) = Arena_Alloc( (arena), sizeof *(p) ))
#define ARENA_NEW0( arena, p )  ((p) = Arena_CAlloc( (arena), sizeof *(p) ))

#define ARENA_NEW_ARRAY( arena, p, count ) \
  ((p) = Arena_Alloc( (arena), (sizeof *(p)) * (count) ))
#define ARENA_NEW0_ARRAY( arena, p, count ) \
  ((p) = Arena_CAlloc( (arena), (sizeof *(p)) * (count) ))


#endif // ARENA_H
