// Target configuration file.
//

#ifndef TCONF_H
#define TCONF_H

#include "hconf.h" // host configuration

// Define sizes of target types in characters 
//
#define TARGET_CHAR_SIZE     1   // it is always 1
#define TARGET_WCHAR_SIZE    2   
#define TARGET_SHORT_SIZE    2
#define TARGET_INT_SIZE      4
#define TARGET_LONG_SIZE     4
#define TARGET_FLOAT_SIZE    4
#define TARGET_DOUBLE_SIZE   8
#define TARGET_POINTER_SIZE  4   // could be greater than TARGET_LONG_SIZE
#define TARGET_PTR_DIFF_SIZE 4   // should be representable in an integral type

typedef union 
{ 
  unsigned HOST_8BIT uc; 
  signed   HOST_8BIT sc; 
  BYTE b[TARGET_CHAR_SIZE];   
} TARGET_CHAR;

typedef union 
{ 
  unsigned HOST_16BIT uw; 
  signed   HOST_16BIT sw; 
  BYTE b[TARGET_WCHAR_SIZE];   
} TARGET_WCHAR;


typedef union 
{ 
  unsigned HOST_16BIT us; 
  signed   HOST_16BIT ss; 
  BYTE b[TARGET_SHORT_SIZE];   
} TARGET_SHORT;

typedef union 
{ 
  unsigned HOST_32BIT ui; 
  signed   HOST_32BIT si; 
  BYTE b[TARGET_INT_SIZE];   
} TARGET_INT;

typedef union 
{ 
  unsigned HOST_32BIT ul; 
  signed   HOST_32BIT sl; 
  BYTE b[TARGET_LONG_SIZE];   
} TARGET_LONG;

typedef union 
{ 
  HOST_FLOAT32 f; 
  BYTE b[TARGET_FLOAT_SIZE];   
} TARGET_FLOAT;

typedef union 
{ 
  HOST_FLOAT64 d; 
  BYTE b[TARGET_DOUBLE_SIZE];   
} TARGET_DOUBLE;


// we need host types that are big enough to hold "reasonable" values
// of target-s size_t
//
typedef unsigned HOST_32BIT HOST_UINT;
typedef signed HOST_32BIT   HOST_INT;

extern const TARGET_LONG   Long0,   
                           Long1;
extern const TARGET_INT    Int0,    
                           Int1;
extern const TARGET_DOUBLE Double0, 
                           Double1;
extern const TARGET_LONG   IntMask, 
                           ShortMask, 
                           CharMask, 
                           WCharMask,
                           HostUIntMax;

//////////////// TARGET_LONG operations /////////////////////////////

#define TGT_LONG_EQ( a, b )          ((a).ul == (b).ul)
#define TGT_LONG_EQ_0( a )           ((a).ul == 0)
#define TGT_LONG_LT( a, b )          ((a).sl < (b).sl)
#define TGT_ULONG_LT( a, b )         ((a).ul < (b).ul)

#define TGT_LONG_ADD( res, a, b )    ((void)((res).ul = (a).ul + (b).ul))
#define TGT_LONG_SUB( res, a, b )    ((void)((res).ul = (a).ul - (b).ul))
#define TGT_LONG_OR( res, a, b )     ((void)((res).ul = (a).ul | (b).ul))
#define TGT_LONG_AND( res, a, b )    ((void)((res).ul = (a).ul & (b).ul))
#define TGT_LONG_XOR( res, a, b )    ((void)((res).ul = (a).ul ^ (b).ul))
#define TGT_ULONG_SHL( res, a, b )   ((void)((res).ul = (a).ul << (b).ui))
#define TGT_ULONG_SHR( res, a, b )   ((void)((res).ul = (a).ul >> (b).ui))
#define TGT_LONG_SHL( res, a, b )    ((void)((res).sl = (a).sl << (b).ui))
#define TGT_LONG_SHR( res, a, b )    ((void)((res).sl = (a).sl >> (b).ui))
#define TGT_ULONG_MUL( res, a, b )   ((void)((res).ul = (a).ul * (b).ul))
#define TGT_LONG_MUL( res, a, b )    ((void)((res).sl = (a).sl * (b).sl))

#define TGT_ULONG_DIV( res, a, b )  \
  ( TGT_LONG_EQ( b, Long0 ) ? false : ((res).ul = (a).ul / (b).ul, true) )

#define TGT_LONG_DIV( res, a, b )   \
  ( TGT_LONG_EQ( b, Long0 ) ? false : ((res).sl = (a).sl / (b).sl, true) )


//////////////// TARGET_INT operations /////////////////////////////

#define TGT_INT_EQ( a, b )          ((a).ui == (b).ui)
#define TGT_INT_EQ_0( a )           ((a).ui == 0)
#define TGT_INT_LT( a, b )          ((a).si < (b).si)
#define TGT_UINT_LT( a, b )         ((a).ui < (b).ui)

#define TGT_INT_ADD( res, a, b )    ((void)((res).ui = (a).ui + (b).ui))
#define TGT_INT_SUB( res, a, b )    ((void)((res).ui = (a).ui - (b).ui))
#define TGT_INT_OR( res, a, b )     ((void)((res).ui = (a).ui | (b).ui))
#define TGT_INT_AND( res, a, b )    ((void)((res).ui = (a).ui & (b).ui))
#define TGT_INT_XOR( res, a, b )    ((void)((res).ui = (a).ui ^ (b).ui))
#define TGT_UINT_SHL( res, a, b )   ((void)((res).ui = (a).ui << (b).ui))
#define TGT_UINT_SHR( res, a, b )   ((void)((res).ui = (a).ui >> (b).ui))
#define TGT_INT_SHL( res, a, b )    ((void)((res).si = (a).si << (b).ui))
#define TGT_INT_SHR( res, a, b )    ((void)((res).si = (a).si >> (b).ui))
#define TGT_UINT_MUL( res, a, b )   ((void)((res).ui = (a).ui * (b).ui))
#define TGT_INT_MUL( res, a, b )    ((void)((res).si = (a).si * (b).si))

#define TGT_UINT_DIV( res, a, b )  \
  ( TGT_INT_EQ( b, INT0 ) ? false : ((res).ui = (a).ui / (b).ui, true) )

#define TGT_INT_DIV( res, a, b )   \
  ( TGT_INT_EQ( b, INT0 ) ? false : ((res).si = (a).si / (b).si, true) )


//////////////// TARGET_DOUBLE operations /////////////////////////////

#define TGT_DOUBLE_EQ( a, b )          ((a).d == (b).d)
#define TGT_DOUBLE_EQ_0( a )           ((a).d == 0)
#define TGT_DOUBLE_LT( a, b )          ((a).d < (b).d)
#define TGT_UDOUBLE_LT( a, b )         ((a).d < (b).d)

#define TGT_DOUBLE_ADD( res, a, b )    ((void)((res).d = (a).d + (b).d))
#define TGT_DOUBLE_SUB( res, a, b )    ((void)((res).d = (a).d - (b).d))


////////////////// Conversions /////////////////////////////////////////

// char -> int
#define TGT_SCHAR_2_INT( res, c )      ((void)((res).si = (c).sc))
#define TGT_UCHAR_2_INT( res, c )      ((void)((res).si = (c).uc))

// short -> int
#define TGT_SHORT_2_INT( res, s )      ((void)((res).si = (c).ss))
#define TGT_USHORT_2_INT( res, s )     ((void)((res).si = (c).us))

// int -> long
#define TGT_INT_2_LONG( res, i )       ((void)((res).sl = (i).si))
#define TGT_UINT_2_LONG( res, i )      ((void)((res).sl = (i).ui))

// long -> int
#define TGT_LONG_2_INT( res, l )       ((void)((res).ui = (l).ul))

// int -> short
#define TGT_INT_2_SHORT( res, i )      ((void)((res).us = (i).ui))

// int -> char
#define TGT_INT_2_CHAR( res, i )       ((void)((res).uc = (i).ui))

// host int to target int
#define HOST_INT_2_TGT_INT( res, i )   ((void)((res).ui = (i)))

// target int to host int
#define TGT_INT_2_HOST_INT( t )        ((t).si)
#define TGT_UINT_2_HOST_INT( t )       ((t).ui)

// check if the value is representable inside the host's
#define CHECK_HOST_UINT_RANGE( x )     TGT_UINT_LE( x, HostUIntMax )


////////////////// These definitions do not change with the target/host

#define TGT_LONG_NE_0( a )           (!TGT_LONG_EQ_0( a ))
#define TGT_LONG_NE( a, b )          (!TGT_LONG_EQ( a, b ))
#define TGT_LONG_GE( a, b )          (!TGT_LONG_LT( a, b ))
#define TGT_LONG_GT( a, b )          TGT_LONG_LT( b, a )
#define TGT_LONG_LE( a, b )          (!TGT_LONG_LT( b, a ))
#define TGT_ULONG_GE( a, b )         (!TGT_ULONG_LT( a, b ))
#define TGT_ULONG_GT( a, b )         TGT_ULONG_LT( b, a )
#define TGT_ULONG_LE( a, b )         (!TGT_ULONG_LT( b, a ))

#define TGT_INT_NE_0( a )           (!TGT_INT_EQ_0( a ))
#define TGT_INT_NE( a, b )          (!TGT_INT_EQ( a, b ))
#define TGT_INT_GE( a, b )          (!TGT_INT_LT( a, b ))
#define TGT_INT_GT( a, b )          TGT_INT_LT( b, a )
#define TGT_INT_LE( a, b )          (!TGT_INT_LT( b, a ))
#define TGT_UINT_GE( a, b )         (!TGT_UINT_LT( a, b ))
#define TGT_UINT_GT( a, b )         TGT_UINT_LT( b, a )
#define TGT_UINT_LE( a, b )         (!TGT_UINT_LT( b, a ))

#define TGT_DOUBLE_NE_0( a )           (!TGT_DOUBLE_EQ_0( a ))
#define TGT_DOUBLE_NE( a, b )          (!TGT_DOUBLE_EQ( a, b ))
#define TGT_DOUBLE_GE( a, b )          (!TGT_DOUBLE_LT( a, b ))
#define TGT_DOUBLE_GT( a, b )          TGT_DOUBLE_LT( b, a )
#define TGT_DOUBLE_LE( a, b )          (!TGT_DOUBLE_LT( b, a ))
#define TGT_UDOUBLE_GE( a, b )         (!TGT_UDOUBLE_LT( a, b ))
#define TGT_UDOUBLE_GT( a, b )         TGT_UDOUBLE_LT( b, a )
#define TGT_UDOUBLE_LE( a, b )         (!TGT_UDOUBLE_LT( b, a ))


#endif //TCONF_H
