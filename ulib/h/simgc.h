#ifndef SIMGC_H
#define SIMGC_H

typedef void (*PGcMarkFunc)(const void *);

bool   GcInit ( void );
void   GcDone ( void );

#ifdef _DEBUG

void * GcAllocDbg ( size_t len, bool bAtomic, PGcMarkFunc markFunc, const char * type, const char * file, int line );

#else

void * GcAllocPrim ( size_t len, bool bAtomic, PGcMarkFunc markFunc );
#define GcAllocDbg( len, bAtomic, markFunc, type, file, line )   GcAllocPrim( len, bAtomic, markFunc )

#endif

#define GcAlloc( len, markFunc )   GcAllocDbg( len, false, markFunc, NULL, __FILE__, __LINE__ )
#define GcAllocAtomic( len )       GcAllocDbg( len, true,  NULL,     NULL, __FILE__, __LINE__ )


void   GcPin ( void * ptr );
void   GcUnpin ( void * ptr );

void   GcFree ( void * mem );

bool   GcCollectStart ( bool bFullCollection );
    // If bFullCollection == false, GcCollectStart() uses internal
    // criteria (see GcIncrementDivisor) to determine whether a 
    // collection is necessary.   
    // Returns true if collection is necessary. 

void   GcCollectEnd ( void );

void   GcCollect ( bool bFullCollection, void (*markRoots)(void * ctx), void * ctx );
		// Equivalent to:
    //   if (GcCollectStart( bFullCollection )) {
    //     if (markRoots != NULL) (*markRoots)(ctx);
    //     GcCollectEnd();
    //   }
  
void   GcMarkAndPush ( const void * ptr );

size_t GcGetSize ( const void * ptr );


extern unsigned GcIncrementDivisor;
extern uint_least32_t GcMinIncrement;
    //
    // We will perform an incremental collection when at least 
    // N/GcIncrementDivisor or GcMinIvcrement (whichever is greater) bytes have 
    // beel allocated since the last collection. N is the used space in the 
    // heap at the end of the last collection. 
    //
    // By default GcIncrementDivisor = 4, GcMinIncrement = 64K. That means that
    // we perform an incremental collection when the used space has increased
    // by 25% or 64K (which ever is greater).


extern void (*GcFatalHandler)(void);
#ifdef _DEBUG
extern int GcDebugLevel;
#endif

#define GC_ALLOC(            type, size )    ((type *)GcAllocDbg( size, false, type##_GcMark, #type, __FILE__, __LINE__ ))
#define GC_ALLOC_ATOMIC(     type, size )    ((type *)GcAllocDbg( size, true,  NULL,          #type, __FILE__, __LINE__ ))

#define GC_ARRAY_NEW(        type, count )   ((type *)GcAllocDbg( sizeof(type) * (count), false, type##_Array_GcMark, #type "_Array_GcMark", __FILE__, __LINE__ ))
#define GC_ARRAY_NEW_ATOMIC( type, count )   ((type *)GcAllocDbg( sizeof(type) * (count), true,  NULL,                #type "_Array_GcMark", __FILE__, __LINE__ ))

#define GC_NEW(        type )                GC_ALLOC       ( type, sizeof(type) )
#define GC_NEW_ATOMIC( type )                GC_ALLOC_ATOMIC( type, sizeof(type) )

// With a GC it is always a good idea to clear a pointer after it has been freed.
#define GC_DELETE( p )                          ((void)(GcFree( p ), (p) = NULL))

#define GC_MARK( val )                          GcMarkAndPush( val )


#define DECLARE_ARRAY_GC_MARK( type )  void type##_Array_GcMark ( const type * array )

#define IMPLEMENT_ARRAY_GC_MARK( type )              \
  void type##_Array_GcMark ( const type * array )    \
  {                                                  \
    size_t size;                                     \
                                                     \
    for ( size = GcGetSize( array );                 \
          size > sizeof(array[0]);                   \
          ++array, size -= sizeof(array[0]) )        \
    {                                                \
      type##_GcMark( array );                        \
    }                                                \
  }

#endif // SIMGC_H
