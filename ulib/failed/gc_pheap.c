/*
  gc_pheap.c

  Page heap manager. The purpose is to allocate page-aligned, page-sized data,
  hopefully within a sequental memory region.

  * We allocate heap in big chunks (TBigBlock) because malloc() rarely (if ever)
  returns page-aligned addresses. We have to align the start address ourselves,
  so it is beneficial to allocate as mush as possible to make the lost memory negligible.
  We always keep itt below 1%. For 4K pages we allocate in chunks of 1MB, which wastes
  only about 0.4%. 
  Also if we detect that we have to allocate too many chunks (an applications requires
  lots of memory), we double the size of the chunk.



*/
#include "precomp.h"
#include "gc_pheap.h"
#include "common\h\clist.h"
#include "common\h\hconf.h"

#define INIT_BBREF_LIMIT 32 // initial size of the BBRef[] array


typedef struct TBBHdr
{
  TListEntry   link;     // link in the list of blocks containing free pages

  uchar_t    * blk;      // pointer to allocated block

  TListRoot    freeList; // list of free pages inside the block

  uchar_t    * start, * end;  // [start, end)  page aligned
  uchar_t    * usedLimit;     // one character after last used byte
} TBBHdr;


static TListRoot FreeBBList;
static TBBRef *  BBRef;
static unsigned  BBRefLimit;  // size of BBRef (in elements of course)
static unsigned  BBCount;     // number of elements in BBRef

#ifdef _DEBUG
static bool bInit = false;
#endif

struct Gc_TPageHeap_public Gc_PH = { GCPAGE_SIZE, 0 };


//--------------------------------------------------------------------------
// Name         Gc_PageHeap_Init 
//
// 
//--------------------------------------------------------------------------
void Gc_PageHeap_Init ( void )
{
  ASSERT( bInit == false );

  BBRef = NULL;
  BBCount = 0;

#ifdef _DEBUG
  bInit = true;
#endif
};

//--------------------------------------------------------------------------
// Name         Gc_PageHeap_Done 
//
// 
//--------------------------------------------------------------------------
void Gc_PageHeap_Done ( void );
{
  unsigned i;

  ASSERT( bInit == true );

  // Free all blocks
  for ( i = 0; i < BBCount; ++i )
    free( BBRef[i].block );
  free( BBRef );

  BBRef = NULL;
  BBRefLimit = BBCount = 0;

#ifdef _DEBUG
  bInit = false;
#endif
};

//--------------------------------------------------------------------------
// Name         Gc_Page_Alloc 
//
// 
//--------------------------------------------------------------------------
void * Gc_Page_Alloc ( unsigned pageCount );
{
};

//--------------------------------------------------------------------------
// Name         Gc_Page_Free 
//
// 
//--------------------------------------------------------------------------
void   Gc_Page_Free ( void * ptr, unsigned pageCount );
{


};

//--------------------------------------------------------------------------
// Name         TryAllocBigBlock
//
// Attempts to allocate a [maxSize] block. If it can't, tries to allocate
// smaller sizes down to [minSize].
//
// Even if maxSize < minSize, allocates at least minSize.
//
// Allocates GCPAGE_SIZE more bytes for alignment.
//--------------------------------------------------------------------------
static void * TryAllocBigBlock ( size_t minSize, size_t maxSize, size_t * pSize )
{
  for(;;)
  {
    void * res;

    // Make sure we allocate at least minSize
    //
    if (maxSize < minSize)
      maxSize = minSize;

    // On success return immediately
    //
    if (NULL != (res = malloc( maxSize + GCPAGE_SIZE )) )
    {
      *pSize = maxSize + GCPAGE_SIZE;
      return res;
    }

    if (maxSize == minSize) // nothing left to try
      break;

    // decrease the size twice
    maxSize /= 2;
  }

  TRACE1( "GC: No memory to allocate a BigBlock of size %u\n", minSize );

  return NULL;
};

//--------------------------------------------------------------------------
// Name         GrowBBref 
//
// Increase the size of BBRef for at least one more element.
//--------------------------------------------------------------------------
static bool GrowBBref ( void )
{
  unsigned newLimit;
  TBBRef * newArray;

  // Calculate the new larger size, considering that the current size
  // may be 0 and handling integer overflow
  //
  if (BBRefLimit > 0)
  {
    newLimit = BBRefLimit * 2;
    if (newLimit <= BBRefLimit)   // overflow ?
    {
      newLimit = BBRefLimit + 1;  // try a smaller increase
      if (newLimit <= BBRefLimit) // overflow again ?
      {
        TRACE1( "CG: BigBlock array is too large: %u\n", BBRefLimit );
        return false;             // give up
      }
    }
  }
  else
    newLimit = INIT_BBREF_LIMIT;

  // Reallocate
  newArray = (TBBRef * )realloc( BBRef, newLimit );

  // If we failed, try again with a smaller size
  if (newArray == NULL) 
  {
    if (newLimit > BBRefLimit + 1) // if we are not already at the smallest possible size
    {
      newLimit = BBRefLimit + 1;
      newArray = (TBBRef * )realloc( BBRef, newLimit );
    }

    if (newArray == NULL)
    {
      TRACE1( "CG: No memory to grow the BigBlock array: %u\n", BBRefLimit );
      return false;
    }
  }

  BBRefLimit = newLimit;
  BBRef = newArray;

  return true;
};

//--------------------------------------------------------------------------
// Name         NewBigBlock 
//
// Allocates a new Big Block. 
// Updates LoAddress and HiAddress.
//--------------------------------------------------------------------------
static TBBRef * NewBigBlock ( size_t minSize, size_t maxSize )
{
  uchar_t * blk;      // start of block
  size_t    blkSize;
  unsigned  alignSpace; // characters to add to _blk_ to align it to GCPAGE_SIZE
  unsigned  bbindex; // index in BBref[]

  // First make sure we have space in BBRef[].
  //
  if (BBRefLimit == BBCount) // do we have to enlarge the array ?
    if (!GrowBBRef())        // try to enlarge it
      return NULL;           // we failed

  // Allocate the block
  //
  if (NULL == (blk = (uchar_t *)TryAllocBigBlock( minSize. maxSize, &blkSize )) )
    return NULL;             // no memory

  // Insert an item in BBRef[]
  //
  ASSERT( BBCount < BBRefLimit ); // teh array must be big enough

  bbindex = WhereToInsertBlock( blk );

  memmove( BBRef+bbindex+1, BBRef+bbindex, BBCount - bbindex );
  ++BBCount;

  /*
    Alignment:
    x + (S - x % S) % S  => x + (S - (x & (S-1))) & (S-1) => x + ((-x)&(S-1))
  */
#if 0
  alignSpace = (GCPAGE_SIZE - PTR2UINT(blk) % GCPAGE_SIZE) % GCPAGE_SIZE;
#else
  alignSpace = ( (-PTR2UINT(blk)) & (GCPAGE_SIZE-1) );
#endif

  // Adjust the size of the data area to compensate for the alignment
  blkSize -= alignSpace;

  // Initialize the header
  //
  BBRef[bbindex].blk = blk;

  INITIALIZE_LIST_HEAD( &BBRef[bbindex].freeList );

  BBRef[bbindex].start = blk + alignSpace;
  BBRef[bbindex].end   = BBRef[bbindex].start + blkSize;
  BBRef[bbindex].usedLimit = BBRef[bbindex].start;

  return &BBref[bbindex];
};

//--------------------------------------------------------------------------
// Name         FindBlockContaining
//
// 
//--------------------------------------------------------------------------
static TBBRef * FindBlockContaining ( void * paddr )
{
  unsigned l, h;   // [l..h)
  uintptr_t addr = PTR2UINT( paddr );

  ASSERT( paddr != NULL );

  l = 0;
  h = BBCount;

  // Invariant: 
  //  *l <= addr < *h
  //
  while (l < h)
  {
    unsigned mid = (h + l) / 2; // l <= mid < h

    if (addr < PTR2UINT(BBRef[mid].start)) // addr < *mid
      h = mid; 
    else
    if (addr < PTR2UINT(BBRef[mid].end))   // *mid == addr
      return &BBRef[mid];
    else                                   // *mid < addr 
      l = mid + 1;               
  }

  return NULL;
};


//--------------------------------------------------------------------------
// Name         WhereToInsertBlock 
//
// Returns the index
//--------------------------------------------------------------------------
static unsigned WhereToInsertBlock ( void * pstart )
{
  unsigned l, h;   // [l..h)
  uintptr_t addr = PTR2UINT( pstart );

  ASSERT( pstart != NULL );

  l = 0;
  h = BBCount;

  // Invariant: 
  //  *l <= addr < *h
  //
  while (l < h)
  {
    unsigned mid = (h + l) / 2; // l <= mid < h

    if (addr < PTR2UINT(BBRef[mid].start))  // addr < *mid
      h = mid;                    
    else                          // *l < addr
    {
      ASSERT( addr > PTR2UINT(BBRef[mid].end) ); // check for duplicate block
      l = mid+1;                  
    }
  }

  return h;
};

