#ifndef FIXALLOC_H
#define FIXALLOC_H

#include "misc\pageheap.h"

/*
#ifdef __cplusplus

template <class T, unsigned NumInPage>
class fixed_allocator
{
  TPagedHeap m_heap;
public:
  void Init ();
  bool PreAllocate ( unsigned count );
  void Done ();
  T * Alloc ();
  T * CAlloc ();
  void Free ( T * );
  void Compact ();
};

#endif

DECLARE_FIXED_ALLOCATOR( Name, T );
IMPLEMENT_FIXED_ALLOCATOR( Name, T, NumInPage );
*/

#define DECLARE_FIXED_ALLOCATOR( Name, T )            \
  extern TPagedHeap Name##_Heap;                      \
  void Name##_Init ( void );                          \
  bool Name##_PreAllocate ( unsigned count );         \
  void Name##_Done ( void );                          \
  T *  Name##_Alloc ( void );                         \
  T *  Name##_CAlloc ( void );                         \
  void Name##_Free ( T * );                           \
  void Name##_Compact ( void )


#define IMPLEMENT_FIXED_ALLOCATOR( Name, T, NumInPage )       \
  TPagedHeap Name##_Heap = { 0 }; /* mark as non-inited */    \
  void Name##_Init ( void )                                   \
  {                                                           \
    InitPagedHeap( &Name##_Heap, sizeof( T ), NumInPage, 0 ); \
  };                                                          \
  bool Name##_PreAllocate ( unsigned count )                  \
  {                                                           \
    return                                                    \
      PreallocatePagedHeap( &Name##_Heap, (count + NumInPage - 1) / NumInPage )\
      != -1;                                       \
  };                                               \
  void Name##_Done ( void )                        \
  {                                                \
    DonePagedHeap( &Name##_Heap );                 \
  };                                               \
  T *  Name##_Alloc ( void )                       \
  {                                                \
    return (T *)AllocPagedBlock( &Name##_Heap );   \
  };                                               \
  T *  Name##_CAlloc ( void )                      \
  {                                                \
    return (T *)CAllocPagedBlock( &Name##_Heap );  \
  };                                               \
  void Name##_Free ( T * p )                       \
  {                                                \
    FreePagedBlock( &Name##_Heap, p );              \
  };                                               \
  void Name##_Compact ( void )                     \
  {                                                \
    CompactPagedHeap( &Name##_Heap );              \
  }                                               
                                                   

#endif // FIXALLOC_H
