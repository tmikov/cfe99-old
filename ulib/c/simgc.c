#include "ulibpriv.h"
#include "clist.h"
#include "simgc.h"

#ifndef USE_GC
#error Need to define USE_GC to compile simgc
#endif

//=========================================================================
//
// Private constants and types
//
//

#ifndef GC_INITIAL_MARK_STACK_SIZE
  #define GC_INITIAL_MARK_STACK_SIZE  1024
#endif 
#ifndef GC_MAX_MARK_STACK_SIZE
  #define GC_MAX_MARK_STACK_SIZE      (GC_INITIAL_MARK_STACK_SIZE*256)
#endif

typedef struct 
{
  TListEntry   link;
  PGcMarkFunc  markFunc;

  uint_least32_t len  : 24;
  uint_least32_t mark : 1;
  uint_least32_t uc   : 1; // uncollectable/pinned

#ifdef _DEBUG
  const char * dbgType;
  const char * dbgFile;
  unsigned     dbgLine;
#endif

  union
  {
    union AlignAll align;
    unsigned char data[];
  };
} TMemBlock;

#define BLK_SIZE( len )       (offsetof(TMemBlock,data) + (len))
#define BLK_FROM_PTR( ptr )   ((TMemBlock *)((char *)(ptr) - offsetof(TMemBlock,data)))

typedef struct
{
  TMemBlock * blk;
} TMarkEntry;

//=========================================================================
//
// Public data
//
//

unsigned GcIncrementDivisor = 4;
uint_least32_t GcMinIncrement = 64 * 1024;

void (*GcFatalHandler)(void) = NULL;
#ifdef _DEBUG
int GcDebugLevel = 0;
#endif

//=========================================================================
//
// Private data
//
//
static TListEntry  BlkList;
static TListEntry  UcList;  // uncollectable list

static unsigned StackSize;
static TMarkEntry * MarkStackStart, 
                  * MarkStackLimit, 
                  * MarkStackPtr;
static bool     bMarkOverflow; 

static unsigned UsedBlockCount;
static uint_least32_t UsedBlockSize;
static uint_least32_t UsedSpaceThreshold;  
		// After this threshold we should do an incremental collection
																	

static enum 
{
  GC_NOT_INIT,   // not yet initialized
  GC_INIT,       // initialized
  GC_COLLECTING, // currently marking
  GC_DONE,       // shutdown
} GcState = GC_NOT_INIT;

//=========================================================================
//
// Private code
//
//

#define FOR_EACH_BLOCK( list, blk ) \
  TYPED_CLIST_FOR_EACH( TMemBlock, &(list), blk )

//--------------------------------------------------------------------------
// Name         GcOutOfMemory 
//
// 
//--------------------------------------------------------------------------
static void GcOutOfMemory ( const char * err )
{
  TRACE1( "GC: GcOutOfMemory: %s\n", err ? err : "" );

  if (GcFatalHandler != NULL)
    (*GcFatalHandler)();

  abort();
};

//--------------------------------------------------------------------------
// Name         GcFreeBlock 
//
// 
//--------------------------------------------------------------------------
static void GcFreeBlock ( TMemBlock * blk )
{
  REMOVE_ENTRY_LIST( &blk->link );
  --UsedBlockCount;
  UsedBlockSize -= BLK_SIZE(blk->len);

#ifdef _DEBUG
  memset( blk, 0xCC, BLK_SIZE(blk->len) );
#endif

  free( blk );
};

//--------------------------------------------------------------------------
// Name         GcClearMarks 
//
// 
//--------------------------------------------------------------------------
static void GcClearMarks ( void )
{
  TMemBlock * blk;
  FOR_EACH_BLOCK( BlkList, blk )
  {
    blk->mark = false;
  }
};


#ifdef _DEBUG
//--------------------------------------------------------------------------
// Name         DumpBlock 
//
// 
//--------------------------------------------------------------------------
static void DumpBlock ( TMemBlock * blk )
{
  TRACE3( "%p[%5u]%s", blk->data, blk->len, blk->markFunc ? "" : " ATOMIC" );

  if (blk->dbgFile)
    TRACE3( " <%-8s> %s(%u)\n", blk->dbgType ? blk->dbgType : "", blk->dbgFile, blk->dbgLine );
  else
    TRACE1( " <%-8s>\n", blk->dbgType ? blk->dbgType : "" );
};
#endif

//--------------------------------------------------------------------------
// Name         GrowMarkStack 
//
// 
//--------------------------------------------------------------------------
static bool GrowMarkStack ( void )
{
  TMarkEntry * newStack;
  unsigned newStackSize;

  // We do the check in this way to avoid integer overflow
  //
  if (StackSize >= GC_MAX_MARK_STACK_SIZE/2)
  {
    TRACE0( "GC: MarkStack too deep " );
    return false;
  }
  newStackSize = StackSize * 2;

  TRACE2( "GC: GrowMarkStack() from %u to %u\n", StackSize, newStackSize );

  // Reallocate
  //
  newStack = (TMarkEntry *)realloc( MarkStackStart, sizeof(TMarkEntry)*newStackSize );
  if (NULL == newStack)
  {
    TRACE0( "GC: GrowMarkStack() - Out of memory trying to expand MarkStack " );
    return false;
  }

  MarkStackPtr   = newStack + StackSize; // new pointer + old limit

  StackSize      = newStackSize;
  MarkStackStart = newStack;
  MarkStackLimit = MarkStackStart + StackSize;

  return true;
};

//--------------------------------------------------------------------------
// Name         GcPush 
//
// 
//--------------------------------------------------------------------------
static void GcPush ( TMemBlock * blk )
{
  ASSERT( blk->markFunc != NULL );

  // Resize the stack if we've reached the limit
  if (MarkStackPtr == MarkStackLimit)
  {
    if (!GrowMarkStack())
    {
      // If we can't gorw the stack, flag an overflow and empty the stack
      //
      TRACE0( "GC: MarkStack overflow\n" );

      bMarkOverflow = true;
      MarkStackPtr = MarkStackStart;
    }
  }

  (MarkStackPtr++)->blk = blk;
};

//--------------------------------------------------------------------------
// Name         GcScan
//
// 
//--------------------------------------------------------------------------
static void GcScan ( void )
{
  TMemBlock * blk;

  ASSERT( GcState == GC_COLLECTING );

  // First scan the uncollectable blocks
  //
  FOR_EACH_BLOCK( UcList, blk )
  {
    if (blk->markFunc != NULL) // if not an atomic block, mark it
      blk->markFunc( blk->data );
  }

  for(;;)
  {
    // On an overflow rescan the heap from the beginning. Since each push in the stack
    // marks one more unmarked block, we are making progress on each heap scan.
    //
    while (bMarkOverflow)
    {
      bMarkOverflow = false;

      FOR_EACH_BLOCK( BlkList, blk )
      {
        // if the block is marked and is not atomic, mark its children
        //
        if (blk->mark && blk->markFunc != NULL) 
        {
          blk->markFunc( blk->data );

          // On overflow we must abort the current heap scan and start over
          //
          if (bMarkOverflow)       
            break;
        }
      }
    } // while

    // loop until the mark stack is empty
    if (MarkStackPtr == MarkStackStart)
      break;

    blk = (--MarkStackPtr)->blk; // pop a block
    blk->markFunc( blk->data );  // mark it
  }
};

//--------------------------------------------------------------------------
// Name         GcSweep 
//
// 
//--------------------------------------------------------------------------
static void GcSweep ( void )
{
  TMemBlock * blk;
  for ( blk = (TMemBlock *)BlkList.Flink; !END_OF_LIST(&BlkList, blk); )
  {
    TMemBlock * next = (TMemBlock *)blk->link.Flink;

    if (!blk->mark)
    {
#ifdef _DEBUG
      if (GcDebugLevel >= 1)
      {
        TRACE0( "GC: Sweeping " );
        DumpBlock( blk );
      }
#endif
      GcFreeBlock( blk );
    }

    blk = next;
  }
};

//=========================================================================
//
// Public code
//
//


//--------------------------------------------------------------------------
// Name         GcInit 
//
// 
//--------------------------------------------------------------------------
bool GcInit ( void )
{
  ASSERT( GcState == GC_NOT_INIT || GcState == GC_DONE );

  INITIALIZE_LIST_HEAD( &BlkList );
  INITIALIZE_LIST_HEAD( &UcList );

  // Allocate the mark stack
  StackSize = GC_INITIAL_MARK_STACK_SIZE;

  if (NULL == (MarkStackStart = (TMarkEntry *)malloc( sizeof(TMarkEntry)*StackSize )) )
  {
    GcOutOfMemory( "GcInit - MarkStack" );
    return false;
  }

  // Initial guess for the heap
  MarkStackLimit = MarkStackStart + StackSize;
  MarkStackPtr   = MarkStackStart;
  bMarkOverflow  = false;
  UsedBlockCount = 0;
  UsedBlockSize  = 0;
  UsedSpaceThreshold = UsedBlockSize + GcMinIncrement;

  GcState = GC_INIT;

  return true;
};

//--------------------------------------------------------------------------
// Name         GcDone 
//
// 
//--------------------------------------------------------------------------
void GcDone ( void )
{
  if (GcState == GC_INIT || GcState == GC_COLLECTING)
  {
    while (!IS_LIST_EMPTY( &UcList ))
    {
      TMemBlock * blk = (TMemBlock *)REMOVE_TAIL_LIST( &UcList );
      GcFreeBlock( blk );
    }

    while (!IS_LIST_EMPTY( &BlkList ))
    {
      TMemBlock * blk = (TMemBlock *)REMOVE_TAIL_LIST( &BlkList );
      GcFreeBlock( blk );
    }

    free( MarkStackStart );
    MarkStackStart = NULL;
  }
  GcState = GC_DONE;
};

//--------------------------------------------------------------------------
// Name         GcAllocPrim
//
// 
//--------------------------------------------------------------------------
#ifdef _DEBUG
void * GcAllocDbg ( size_t len, bool bAtomic, PGcMarkFunc markFunc, const char * type, const char * file, int line )
#else
void * GcAllocPrim ( size_t len, bool bAtomic, PGcMarkFunc markFunc )
#endif
{
  TMemBlock * blk;

  ASSERT( GcState == GC_INIT );

  ASSERT( !bAtomic || !markFunc ); // Atomic blocks can't have mark functions
  if (bAtomic)
    markFunc = NULL;

  // Make sure the len is reasonable
  //
  if (len > 0x01000000u)
  {
    GcOutOfMemory( "GcAlloc - too large request" );
    return NULL; 
  }

  // Allocate memory
  //
  if (NULL == (blk = (TMemBlock *)malloc( BLK_SIZE(len) )) )
  {
    GcOutOfMemory( "GcAlloc" );
    return NULL;
  }

  // Clear the data of non-atomic blocks
  //
  if (!bAtomic)
    bzero( blk->data, len );
#ifdef _DEBUG
  else
    memset( blk->data, 0xCC, len ); // and fill atomic with garbage
#endif

  // Initialize the header
  //
  blk->markFunc = markFunc;
  blk->len      = len;
  blk->mark     = 0;
  blk->uc       = 0;

#ifdef _DEBUG
  blk->dbgType = type;
  blk->dbgFile = file;
  blk->dbgLine = line;
#endif

  // Add to the memory list
  //
  INSERT_TAIL_LIST( &BlkList, &blk->link );

  ++UsedBlockCount;
  UsedBlockSize += BLK_SIZE(blk->len);

  return blk->data;
};

//--------------------------------------------------------------------------
// Name         GcPin
//
// 
//--------------------------------------------------------------------------
void GcPin ( void * ptr )
{
  TMemBlock * blk;

  ASSERT( GcState == GC_INIT );
  ASSERT( ptr != NULL );

  blk = BLK_FROM_PTR( ptr );

  if (!blk->uc) // if not already pinned
  {
    blk->uc = 1;                             // mark as pinned
    REMOVE_ENTRY_LIST( &blk->link );         // remove from the block list
    INSERT_TAIL_LIST( &UcList, &blk->link ); // add to uncollectable list
  }
  else
    TRACE1( "GC: GcPin(%p) - object already pinned\n", ptr );
};

//--------------------------------------------------------------------------
// Name         GcUnpin 
//
// 
//--------------------------------------------------------------------------
void GcUnpin ( void * ptr )
{
  TMemBlock * blk;

  ASSERT( GcState == GC_INIT );
  ASSERT( ptr != NULL );

  blk = BLK_FROM_PTR( ptr );

  if (blk->uc) // if pinned
  {
    blk->uc = 0;                              // mark as pinned
    REMOVE_ENTRY_LIST( &blk->link );          // remove from uncollectable list
    INSERT_TAIL_LIST( &BlkList, &blk->link ); // add to block list
  }
  else
    TRACE1( "GC: GcUnpin(%p) - object not pinned\n", ptr );
};

//--------------------------------------------------------------------------
// Name         GcFree 
//
// 
//--------------------------------------------------------------------------
void GcFree ( void * mem )
{
  TMemBlock * blk;

  ASSERT( GcState == GC_INIT );

  if (mem == NULL) // ignore NULL
    return;

  blk = BLK_FROM_PTR( mem );

  // TODO: validate the pointer

  GcFreeBlock( blk );
};

//--------------------------------------------------------------------------
// Name         GcGetSize 
//
// 
//--------------------------------------------------------------------------
size_t GcGetSize ( const void * ptr )
{
  ASSERT( GcState == GC_INIT || GcState == GC_COLLECTING );
  ASSERT( ptr != NULL );
  return BLK_FROM_PTR( ptr )->len;
};

//--------------------------------------------------------------------------
// Name         GcMarkAndPush 
//
// 
//--------------------------------------------------------------------------
void GcMarkAndPush ( const void * ptr )
{
  TMemBlock * blk;

  ASSERT( GcState == GC_COLLECTING );

  if (ptr == NULL)
    return;

  blk = BLK_FROM_PTR(ptr);

  if (blk->mark || blk->uc)  // already marked or uncollectable ?
    return;

  blk->mark = 1;

  if (blk->markFunc != NULL) // if not an atomic object, push it
    GcPush( blk );
};

//--------------------------------------------------------------------------
// Name         GcCollect 
//
// 
//--------------------------------------------------------------------------
void GcCollect ( bool bFullCollection, void (*markRoots)(void * ctx), void * ctx )
{
  if (GcCollectStart( bFullCollection ))
  {
    if (markRoots != NULL)
      (*markRoots)(ctx);

    GcCollectEnd();
  }
};


//--------------------------------------------------------------------------
// Name         GcCollectStart 
//
// 
//--------------------------------------------------------------------------
bool GcCollectStart ( bool bFullCollection )
{
  ASSERT( GcState == GC_INIT );

  // If incremental collection is requested, check if we need to do it al all
  //
  if (!bFullCollection && UsedBlockSize < UsedSpaceThreshold)
    return false;

  TRACE2( "GC: GcCollect start - %u blocks, %lu bytes\n", UsedBlockCount, (unsigned long)UsedBlockSize );

  GcClearMarks();

  // Make sure the stack is empty
  //
  ASSERT( MarkStackPtr == MarkStackStart );
  MarkStackPtr = MarkStackStart; // just in case...

  bMarkOverflow = false; // reset the overflow flag

  GcState = GC_COLLECTING;

  return true;
};

//--------------------------------------------------------------------------
// Name         GcCollectEnd 
//
// 
//--------------------------------------------------------------------------
void GcCollectEnd ( void )
{
  ASSERT( GcState == GC_COLLECTING );

  GcScan();
  GcSweep();

  // Calculate the threshold for the next incremental collection
  // as a percentage of the current used heap size, making sure it is
  // not less than GcMinIncrement
  //
  if ((UsedSpaceThreshold = UsedBlockSize / GcIncrementDivisor) < GcMinIncrement)
    UsedSpaceThreshold = GcMinIncrement;
  UsedSpaceThreshold += UsedBlockSize;

  TRACE2( "GC: GcCollect end - %u blocks, %lu bytes\n", UsedBlockCount, (unsigned long)UsedBlockSize );

  GcState = GC_INIT;
};
