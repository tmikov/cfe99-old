#ifndef HEAP_H
#define HEAP_H

void * gcAlloc ( size_t );
void * gcAllocAtomic ( size_t );
void gcFree ( void * );

#define GC_ARRAY_NEW( type, count )         ((type *)gcAlloc( sizeof(type) * (count) ))
#define GC_ARRAY_NEW_ATOMIC( type, count )  ((type *)gcAllocAtomic( sizeof(type) * (count) ))

#define GC_NEW( type )         ((type *)gcAlloc( sizeof(type) ))
#define GC_NEW_ATOMIC( type )  ((type *)gcAllocAtomic( sizeof(type) ))

#define GC_DELETE( p )         gcFree( p )

#endif // HEAP_H

