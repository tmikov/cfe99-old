/*
 $Header: /Datapark/GenLib/pageheap.cpp 1     1/23/99 10:09p Ceco $

 $Log: /Datapark/GenLib/pageheap.cpp $
 
 1     1/23/99 10:09p Ceco
 
 1     12/11/98 3:43p Ceco
 
 4     11/15/98 3:07p Ceco
 
 3     11/12/98 8:14p Ceco
 BUGFIX: AllocateVirginPage() can be used to allocate empty pages, but
 it assumed that it is always called to allocate the first block in the
 page.
 
 2     11/03/98 8:28p Ceco
 
 1     11/03/98 8:27p Ceco
 Improved from C51 version. Added maxPages and PreallocatePagedHeap()


  pageheap.c: Fast fixed block heap.

  Implements a fast fixed size block heap. The memory is reserved in
  chunks (pages). Each page is holding a lot of fixed sized blocks. So,
  allocation of a single block is faster, because we rarely fall back
  to the standard heap routines.

  All the pages are kept in two lists. The virgin lists contains pages
  that still have blocks that have never been allocated. The index of the
  first of such "virgin" blocks for a page is stored in page's virginAt field.

  The whore list contains pages that blocks have been allocated at least
  once. They still may contain free blocks. However such blocks have been
  allocated and freed at least once.

  All the blocks that the user frees are kept in a list.
  (!not true since 13 Feb 1999; see the note from 13 Feb 1999)
  When the user
  wants to allocate a new block, first the blocks in the free list are checked.
  If there are no "freed" blocks, then the first "virgin" block of the first "virgin"
  page is used. If there are no "virgin" pages a new "virgin" page is allocated.

  Each block - free or used - contains a pointer to the page it belongs to.
  For the free blocks this is the _page_ member of the TFreeBlock structure.
  For the used blocks, a pointer to the page is stored in the first word of the
  block, just before the address returned to the user.

  The pages are not freed when the blocks are freed even if a page contains
  only free blocks. I did this because all the free blocks in a page would
  have first to be removed from the free blocks list.

  Such pages are freed by CompactPagedHeap(). It may be occasionally called
  to optimize heap usage whan lots of blocks have been freed.

  All the pages used are freed when DonePagedHeap() is called, so in many
  cases it is good never to free the blocks, because they will be at once,
  very fast, by DonePagedHeap().

28 Aug 1998:
  I studied this implementation (which I did several months ago) and I have
  an idea how to optimize the overall memory usage with a small speed loss.
  I am not sure that it is worth it, but at least I will describe the idea.
  Basicly the idea is instead of keeping a global freed-block list to keep
  such list on per-page basis.
   
  The purpose is to free a "whore" page whenever all of its blocks are
  free. Instead of a freed-block list we shall keep a list of pages
  which contain at least one freed block - let's call it FreeInWhoreList.
  Each such page in term will have a list of the freed blocks in it.
  So to extract a freed block we get the first page from the FreeInWhoreList
  and get its first freed block. If this was the page's last freed block we
  shall have to remove the page itself from the FreeInWhoreList. So instead
  of one list extraction we have two, which is not much slower.

  What we gain is that a whore page that contains only freed blocks can be
  returned to the system very easily by just removing it from all the lists
  (this way effectively converting it to a "virgin page" :-).

03 Nov 1998:
  Here is a little improvement I need to do today. Sometimes for embedded
  applications it is also necessary to have a fixed time for allocating
  a block. To achieve this we have to do these things:
    - Limit the maximum number of pages to a predefined number. This way
    we make sure that the required memory will not grow for ever.
    - Make sure pages are not occasionally freed. (This is the current
    implementation - pages are freed only in CompactPagedHeap())
    - Preallocate a specified amount of virgin pages. If we preallocate
    the entire amount of pages a new page will never be allocated, so malloc()
    will never be called wen we will have linear time for allocating a new
    block.

13 Feb 1999:
  Currently for each allocated block we have a little overhead - this is
  the pointer to the page the block belongs to.
  Having in mind that we are working with fixed size blocks, for a large
  number of small blocks this seems to be a significant (and unnecessary)
  overhead.
  There is a way to avoid this overhead with a small performance penalty.
  Why do we need to know the page that the block belongs to ? In order
  to update the used block counter in that page (numUsedBlocks). Ok, why
  do we need this counter? It is used only in CompactPagedHeap() to detect
  pages that contain only empty blocks.
  We can postpone updating the counter until CompactPagedHeap() is called.
  In that case when freeing the block we don;t know its page, so we set
  page = NULL. We shall call all such free blocks "orphan" blocks.
  It will scan all free blocks looking for orphans, "un-orphan" the
  found blocks, locate their pages and update the pages' counters.
  Well, finding the page is relatively slow because all pages must be scanned
  and compared against the address of the block. However CompactPagedHeap()
  is generally considered to be slow, so this is not a problem.

  One possible optimization is to keep a separate list of orphan blocks,
  so that we don't need to go through all free blocks to locate them.

  - Another optimization that comes to mind under Win32 is to use
  virtual memory. To reserve large amount of space and to commit is
  as necessary. This is just an idea - have to think about it in more
  depth.

*/
#include "port\global.h"
#include "xheap.h"
#include "pageheap.h"

// This function must be provided by the user. It is called when we run
// out of memory. In most implementations it should abort the program
//
extern void OutOfMemory( void );

typedef struct TPage
{
  TListEntry   link;
  unsigned     virginAt;      // the first virgin block of the page
  unsigned     numUsedBlocks;
  BYTE         blocks[1];
} TPage;

typedef struct TFreeBlock
{
  TListEntry link;
  TPage * page;
} TFreeBlock;

//--------------------------------------------------------------------------
// Name         InitPagedHeap
//
// Description
//--------------------------------------------------------------------------
void InitPagedHeap ( TPagedHeap * pHeap, unsigned blkSize, unsigned numInPage,
                     unsigned maxPages    )
{
  ASSERT( pHeap->blockSize == 0 ); // must not be initialized

  // Align blkSize to sizeof(int) for performance purposes
  //
  blkSize = (blkSize + sizeof(int) - 1) & ~(sizeof( int ) - 1);

  pHeap->blockSize = max( blkSize, sizeof( TFreeBlock ) );
  pHeap->pageLen = numInPage;
  pHeap->maxPages = maxPages ? maxPages : ~0u;
  pHeap->curPages = 0;

  INITIALIZE_LIST_HEAD( &pHeap->freeBlocks );
  INITIALIZE_LIST_HEAD( &pHeap->orphanBlocks );
  INITIALIZE_LIST_HEAD( &pHeap->virginPages );
  INITIALIZE_LIST_HEAD( &pHeap->whores );
};

//--------------------------------------------------------------------------
// Name         DonePagedHeap
//
// Description  
//--------------------------------------------------------------------------
void DonePagedHeap ( TPagedHeap * pHeap )
{
  TPage * page, * nextPage;

  if (pHeap->blockSize == 0)
  {
    // Not initialized
    return;
  }

  // First check the virgin list
  //
  for ( page = (TPage *)pHeap->virginPages.Flink;
        !END_OF_LIST( &pHeap->virginPages, page );
      )
  {
    nextPage = (TPage *)page->link.Flink;
    xfree( page );
    page = nextPage;
  }

  for ( page = (TPage *)pHeap->whores.Flink;
        !END_OF_LIST( &pHeap->whores, page );
      )
  {
    nextPage = (TPage *)page->link.Flink;
    xfree( page );
    page = nextPage;
  }

  // Make the lists empty in case DonePagedHeap() is called again
  // for the same heap
  INITIALIZE_LIST_HEAD( &pHeap->freeBlocks );
  INITIALIZE_LIST_HEAD( &pHeap->orphanBlocks );
  INITIALIZE_LIST_HEAD( &pHeap->virginPages );
  INITIALIZE_LIST_HEAD( &pHeap->whores );

  pHeap->curPages = 0;
};

//--------------------------------------------------------------------------
// Name         AllocateVirgin
//
// Description  Allocates and returns a virgin page. Returns NULL on out of
//              memory or if max number of pacges has been reached.
//--------------------------------------------------------------------------
static TPage * AllocateVirginPage ( TPagedHeap * pHeap )
{
  TPage * page;

  if (pHeap->curPages >= pHeap->maxPages)  // check if max number has been reached
    return NULL;

  TRACE1( "PageHeap_AllocateVirginPage( %u )\n", 
          pHeap->blockSize * pHeap->pageLen + offsetof( TPage, blocks ) );

  if (page = (TPage *)xmalloc( pHeap->blockSize * pHeap->pageLen + offsetof( TPage, blocks ) ))
  {
    INSERT_TAIL_LIST( &pHeap->virginPages, &page->link );
    page->virginAt = 0;
    page->numUsedBlocks = 0;
    ++pHeap->curPages;
  }

  return page;
};

//--------------------------------------------------------------------------
// Name         PreallocatePagedHeap
//
// Description  Allocates a specified amount of virgin pages, so that
//              subsequent calls to AllocPagedBlock() will take linear
//              time (no need for malloc()).
//
//              Returns -1 on if specified amount of pages could not be
//              allocated.
//--------------------------------------------------------------------------
int PreallocatePagedHeap ( TPagedHeap * pHeap, unsigned numPages )
{
  ASSERT( pHeap->blockSize != 0 ); // must be initialized

  for ( ; numPages != 0 && AllocateVirginPage( pHeap ); --numPages );
  return numPages != 0 ? -1 : 0;
};


//--------------------------------------------------------------------------
// Name         AllocPagedBlock
//
// Description  
//--------------------------------------------------------------------------
void * AllocPagedBlock ( TPagedHeap * pHeap )
{
  TFreeBlock * pBlock;
  TPage * page;

  ASSERT( pHeap->blockSize != 0 ); // must be initialized

  // First the easier cases: check for orphaned and freed blocks
  //
  if (!IS_LIST_EMPTY( &pHeap->orphanBlocks ))
  {
    pBlock = (TFreeBlock *)REMOVE_TAIL_LIST( &pHeap->orphanBlocks );
    ASSERT( pBlock->page == NULL );
    page = NULL;

    ASSUME( page == NULL );
  }
  else
  if (!IS_LIST_EMPTY( &pHeap->freeBlocks ))
  {
    pBlock = (TFreeBlock *)REMOVE_TAIL_LIST( &pHeap->freeBlocks );
    ASSERT( pBlock->page != NULL );
    page = pBlock->page;

    ASSUME( page != NULL );
  }
  else
  {
    // If necessary make a new virgin page
    //
    if (IS_LIST_EMPTY( &pHeap->virginPages ))
    {
      if ((page = AllocateVirginPage( pHeap )) == NULL)
        return NULL;  // out of memory, or max number of pages reached
    }
    else
    {
      // Get the first virgin page
      //
      page = (TPage *)pHeap->virginPages.Flink;
    }

    // get the first virgin block in page
    pBlock = (TFreeBlock *)(page->blocks + page->virginAt * pHeap->blockSize);  

    // check if the page has no virgin space any more
    //
    if (++page->virginAt == pHeap->pageLen)
    {
      REMOVE_ENTRY_LIST( &page->link );    // remove it from the virgin list
      INSERT_TAIL_LIST( &pHeap->whores, &page->link ); // add it to whore list
    }

    ASSUME( page != NULL );
  }

  if (page) // if we know the page, update the block counter
    ++page->numUsedBlocks;

  return pBlock;
};

void * CAllocPagedBlock ( TPagedHeap * pHeap )
{
  void * p;
  if (p = AllocPagedBlock( pHeap ))
    memset( p, 0, pHeap->blockSize );
  return p;
};

//--------------------------------------------------------------------------
// Name         FreePagedBlock
//
// Description  
//--------------------------------------------------------------------------
void FreePagedBlock ( TPagedHeap * pHeap, void * usedBlock )
{
  TFreeBlock * pBlock;

  ASSERT( pHeap->blockSize != 0 ); // must be initialized

  if (usedBlock == NULL)  // ignore NULL blocks
    return;

  pBlock = (TFreeBlock *)usedBlock;
  pBlock->page = NULL;  // we don't know its page yet
  INSERT_TAIL_LIST( &pHeap->orphanBlocks, &pBlock->link );
};

//--------------------------------------------------------------------------
// Name         AdoptOrphanBlocks
//
// Iterate through all orphan blocks (free blocks with page = NULL, which means
// that  we haven't found their pages yet), find their pages and update the
// used block counters in the pages.
//--------------------------------------------------------------------------
static void AdoptOrphanBlocks ( TPagedHeap * pHeap )
{
  while (!IS_LIST_EMPTY( &pHeap->orphanBlocks ))
  {
    TFreeBlock * pBlock;
    TPage * page;
    bool bFound;

    pBlock = (TFreeBlock *)REMOVE_TAIL_LIST( &pHeap->orphanBlocks );
    ASSERT( pBlock == NULL );

    // Find its page
    //

    bFound = false;

    for ( page = (TPage *)pHeap->virginPages.Flink;
          !END_OF_LIST( &pHeap->virginPages, page );
          page = (TPage *)page->link.Flink )
    {
      if ((BYTE *)pBlock > (BYTE *)page &&
          (BYTE *)pBlock < (BYTE *)page->blocks + pHeap->pageLen * pHeap->blockSize)
      {
        bFound = true;
        break;
      }
    }

    if (!bFound)
    {
      for ( page = (TPage *)pHeap->whores.Flink;
            !END_OF_LIST( &pHeap->whores, page );
            page = (TPage *)page->link.Flink )
      {
        if ((BYTE *)pBlock > (BYTE *)page &&
            (BYTE *)pBlock < (BYTE *)page->blocks + pHeap->pageLen * pHeap->blockSize)
        {
          bFound = true;
          break;
        }
      }
    }

    ASSERT( bFound );
    ASSERT( ((BYTE *)pBlock - page->blocks) % pHeap->blockSize == 0 );

    pBlock->page = page;
    ASSERT( page->numUsedBlocks > 0 );
    --page->numUsedBlocks;

    // Now add to the free block list
    //
    INSERT_TAIL_LIST( &pHeap->freeBlocks, &pBlock->link );
  }
};

//--------------------------------------------------------------------------
// Name         CompactPagedHeap
//
// Description  Free all pages that contain only free blocks
//--------------------------------------------------------------------------
void CompactPagedHeap ( TPagedHeap * pHeap )
{
  TPage * page, * nextPage;
  unsigned i;

  ASSERT( pHeap->blockSize != 0 ); // must be initialized

  AdoptOrphanBlocks( pHeap );

  // First check the virgin list
  //
  for ( page = (TPage *)pHeap->virginPages.Flink;
        !END_OF_LIST( &pHeap->virginPages, page );
      )
  {
    nextPage = (TPage *)page->link.Flink;

    if (page->numUsedBlocks == 0)
    {
      for ( i = 0; i < page->virginAt; ++i )
        REMOVE_ENTRY_LIST( &((TFreeBlock *)(page->blocks + i * pHeap->blockSize))->link );
      REMOVE_ENTRY_LIST( &page->link );
      xfree( page );
      --pHeap->curPages;
    }

    page = nextPage;
  }

  for ( page = (TPage *)pHeap->whores.Flink;
        !END_OF_LIST( &pHeap->whores, page );
      )
  {
    nextPage = (TPage *)page->link.Flink;

    if (page->numUsedBlocks == 0)
    {
      for ( i = 0; i < page->virginAt; ++i )
        REMOVE_ENTRY_LIST( &((TFreeBlock *)(page->blocks + i * pHeap->blockSize))->link );
      REMOVE_ENTRY_LIST( &page->link );
      xfree( page );
      --pHeap->curPages;
    }

    page = nextPage;
  }
};

