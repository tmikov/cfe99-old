/*
  arena.c : A heap region that is freed all at once.

  It is used to logically separate different allocations into pools,
  so that they can later be quickly freed. Freeing of a single allocated
  block is not supported!

  The idea is that memory is allocated in large chunks (pages), which
  are kept in a linked list. When the arena must be deallocated, the
  pages in the list are freed at once.

  The pages in the list are sorted by the free space in each page.
  The more free space a page has, the closer it is to the head of the
  list. Thus the page with the most free space is always in the beginning
  of the list. So, when we need to allocate a block, it is enough to check
  only whether there is enough free space in the first page. If there isn't,
  a new page must be allocated.
  As the free space in a page decreases, the page is being moved down the
  list. At some point it will eventually become 0 and the page will remain
  for ever in the end of the list.

  Of course if the block requested for allocation is bigger than a page,
  we allocate a a single page big enough exactly to hold the block.

30 Jul 1999:
  It seems to me that when free space in two pages differs just a little,
  unnecessary swapping of the two pages can occur. That's why I introduced
  a small constant EPSILON. The free space in the first page is considered
  less than the free space in the next only if the difference is greater than
  EPSILON. This might lead to a negligible space waste, but hopefully will
  improve performance. I want to avoid page jiggling as much as possible.
*/
#include "port\global.h"
#include "xheap.h"
#include "arena.h"

#define TEST_ARENA 0

typedef struct TPage TPage;

// Alignment suitable for all types
typedef union
{
  long   l;
   double d;
  char   * p;
  void   (*f)(void);
} Align;

struct TPage
{
  TListEntry   link;
  size_t size, free;

  // The data buffer starts here. This is necessary because we want
  // to be sure it is properly aligned.
  Align        buf[1]; 
};

// allign a to nice size
#define ALIGN_SIZE( size )  ((size) + sizeof(Align) - 1) & ~(sizeof(Align) - 1)

// Differences in free space smaller than EPSILON are ignored.
#define EPSILON (sizeof(Align)*2)

//--------------------------------------------------------------------------
// Name         Arena_Init
//
//
//--------------------------------------------------------------------------
void Arena_Init ( TArena * pArena, size_t pageSize )
{
  ASSERT( pageSize > 0 );

  // Align pageSize to sizeof(int) for performance purposes
  //
  pageSize = ALIGN_SIZE( pageSize );

  pArena->pageSize = pageSize;

  INITIALIZE_LIST_HEAD( &pArena->root );
};

//--------------------------------------------------------------------------
// Name         Arena_Done
//
//
//--------------------------------------------------------------------------
void Arena_Done ( TArena * pArena )
{
  Arena_Free( pArena );

#ifdef _DEBUG
  pArena->pageSize = 0;  // mark as destructed
#endif
};

//--------------------------------------------------------------------------
// Name         Arena_Free
//
//
//--------------------------------------------------------------------------
void Arena_Free ( TArena * pArena )
{
  ASSERT( pArena && pArena->pageSize != 0 );

  while (!IS_LIST_EMPTY( &pArena->root ))
  {
    void * page;

    page = REMOVE_HEAD_LIST( &pArena->root );
    xfree( page );
  }
};

#if TEST_ARENA && defined(_DEBUG)
static DumpArena ( TArena * pArena )
{
  TListEntry * p;

  for ( p = pArena->root.Flink; !END_OF_LIST( &pArena->root, p ); p = p->Flink )
    TRACE2( "%u(%u) ", ((TPage *)p)->free, ((TPage *)p)->size );
  TRACE0( "\n" );
};
#endif


//--------------------------------------------------------------------------
// Name         Arena_Alloc
//
//--------------------------------------------------------------------------
void * Arena_Alloc ( TArena * pArena, size_t size )
{
  TPage * first;
  TListEntry * next;
  void * result;
  ASSERT( pArena && pArena->pageSize != 0 );

  size = ALIGN_SIZE( size );

#if TEST_ARENA && defined(_DEBUG)
  TRACE1( "\tAlloc( %u )\n", size );
#endif

  first = (TPage *)pArena->root.Flink; // cache the value

  // Check if we must allocate a new page
  //
  if (IS_LIST_EMPTY( &pArena->root ) || first->free < size)
  {
    size_t pageSize;

    // determine the size of the new page
    pageSize = size > pArena->pageSize ? size : pArena->pageSize;

    // subtract sizeof(Align) from sizeof(TPage) because of TPage.buf.
    //
    if ((first = (TPage *)xmalloc( sizeof(TPage) - sizeof(Align) + pageSize )) == NULL)
      return NULL;

    TRACE1( "Arena_Alloc( %u )\n", sizeof(TPage) - sizeof(Align) + pageSize );

    memset( first->buf, 0, pageSize ); // Zero the memory

    first->size = first->free = pageSize;

    // Add the newly allocated empty page to the start of the list.
    // We know for sure that it has the biggest free space so far.
    //
    INSERT_HEAD_LIST( &pArena->root, &first->link );
  }

  ASSERT( first->free >= size );
  ASSERT( (void *)first == pArena->root.Flink );

  // remember the result value
  result = ((char *)first->buf) + first->size - first->free;
  first->free -= size;

  // Now sort _first_ into the list. Pages with smaller _free_ go to the end
  // of the list

  next = first->link.Flink;

  // Check if we need to move _first_ at all
  //
  if (!END_OF_LIST( &pArena->root, next ) && first->free + EPSILON < ((TPage *)next)->free)
  {
    // yes, we must move _first_. So, detach it from the list,
    // find its new spot and insert it there
    //

    REMOVE_ENTRY_LIST( &first->link );

    // Now, to handle entirely full pages quickly: if the page is full,
    // attach it to the end of the list. Otherwise must locate its new
    // position by walking the list.

    if (first->free <= EPSILON)
    {
      INSERT_TAIL_LIST( &pArena->root, &first->link );
    }
    else
    {
      while (!END_OF_LIST( &pArena->root, next->Flink ) &&
             first->free + EPSILON < ((TPage *)next->Flink)->free)
      {
        next = next->Flink;
      }

      // Insert after _next_
      INSERT_HEAD_LIST( next, &first->link );
    }
  }

#if TEST_ARENA && defined(_DEBUG)
  DumpArena( pArena );
#endif

  return result;
};


#if 0 && TEST_ARENA
void OutOfMemory ( void )
{
  abort();
};

TArena a;

void main ( void )
{
  Arena_Init( &a, 64 );

  Arena_Alloc( &a, 32 );  // 32 
  Arena_Alloc( &a, 40 );  // 24 32
  Arena_Alloc( &a, 32 );  // 32 24 32
  Arena_Alloc( &a, 32 );  // 24 32 0


  Arena_Done( &a );
};

#endif
