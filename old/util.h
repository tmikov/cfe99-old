#ifndef UTIL_H
#define UTIL_H


#define CLIST_FOR_EACH( type, head, it )                     \
    for ( (it) = (type *)(head)->Flink;                      \
          !END_OF_LIST( (head), (it) );                      \
          (it) = (type *)((TListEntry *)(it))->FLink )


#define CLEAR( ptr )  memset( (ptr), 0, sizeof(*(ptr)) )

#ifdef _DEBUG
#define MAKE_SURE( cond )  (void)(1/((cond) != 0))
#else
#define MAKE_SURE( cond )  ((void)0)
#endif

#endif


