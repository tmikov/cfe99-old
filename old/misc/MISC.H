#ifndef MISC_H
#define MISC_H

#include "misc\xheap.h"
#include "misc\clist.h"

#define XMALLOC( type )               ((type *)xmalloc( sizeof( type ) ))
#define XMALLOC_ARRAY( type, count )  ((type *)xmalloc( sizeof( type )*(count) ))

#endif // MISC_H
