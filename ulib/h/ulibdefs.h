#ifndef ULIBDEFS_H
#define ULIBDEFS_H

typedef union AlignAll
{
  long double ld;
  long x;
  void * p;
  void (*pFunc)(void);
} AlignAll;

typedef unsigned long ulong;
typedef unsigned char uchar;

#define bzero( p, n ) memset( p, 0, n )

#define CLEAR( ptr )  bzero( (ptr), sizeof(*(ptr)) )

#define MAKE_SURE( cond ) COMPILE_TIME_EXPR_ASSERT( cond )

// fall is used as an indication of fallthough in switch cases
#define fall          ((void)0)


#endif // ULIBDEFS_H
