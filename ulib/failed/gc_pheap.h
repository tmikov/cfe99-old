/*
  gc_palloc.h 

  Interface to the internal page heap manager. The collector behaves much
  better if all allocations are sequental. Also it needs page-aligned 
  allocations.
*/
#ifndef GC_PHEAP_H

#define GCPAGE_SIZE_SHIFT  12
#define GCPAGE_SIZE        (1 << GCPAGE_SIZE_SHIFT)

#ifdef __MSDOS__

  // ssss:oooo ->  ssss0 + 0oooo
  #define DOSPTR2UINT( x )    ( ((x & 0xFFFF0000) >> 12) + (x & 0xFFFF) )
  // sssso -> ssss:000o
  #define UINT2DOSPTR( x )    ( ((x << 12) & 0xFFFF0000) | (x & 0xF) )

  #define PTR2UINT( x )   DOSPTR2UINT( (uint32_t)(x) )
  #define UINT2PTR( x )   ((uchar_t _far *)UINT2DOSPTR( (uint32_t)(x) ))

#else // default flat memory case

  #define PTR2UINT( x )   ((uintptr_t)(x))
  #define UINT2PTR( x )   ((uchar_t *)(x))

#endif

extern struct Gc_TPageHeap_public
{
  // every allocated address is within [LoAddress,HiAddress]
  uintptr_t LoAddress;  // >= GCPAGE_SIZE
  uintptr_t HiAddress; 
} Gc_PH;

void Gc_PageHeap_Init ( void );
void Gc_PageHeap_Done ( void );

void * Gc_Page_Alloc ( unsigned pageCount );
void   Gc_Page_Free ( void * ptr, unsigned pageCount );


#endif // GC_PHEAP_H

