#ifndef TCONF_H
#define TCONF_H


/*
  On the host and the target it is guaramteed that:
    A char is at least 8 bits
    A short or int is at lest 16 bits
    A long is at least 32 bits


  Since we are using COCOM's arithmetic package we are also using
  its representation of values.

  A target value is represented as an array of host characters (hi order
  first). Since the size of the target value may not be exact multiply
  of the host, there may be some padding bits in the last character.
  The maximum target value representable in each unsigned integer type
  is store in a global constant called TargetMax<Type>.

  COCOM's function are not supposed to be called directly outside
  of TCONF.C/.H. Instead the functions and macros provided by TCONF.H
  must be used.
*/

#include "cocom\ammunition\arithm.h"
#include "cocom\ammunition\ieee.h"
#include "util.h"

#define BITS2CHARS( x )   (((x)+CHAR_BIT-1)/CHAR_BIT)

#define TARGET_CHAR_BIT    10u
#define TARGET_CHAR_SIZE_ON_HOST   BITS2CHARS(TARGET_CHAR_BIT)

#define TARGET_SHORT_BIT   20u
#define TARGET_SHORT_SIZE_ON_HOST  BITS2CHARS(TARGET_SHORT_BIT)

#define TARGET_INT_BIT     40u
#define TARGET_INT_SIZE_ON_HOST    BITS2CHARS(TARGET_INT_BIT)

#define TARGET_LONG_BIT    80u
#define TARGET_LONG_SIZE_ON_HOST   BITS2CHARS(TARGET_LONG_BIT)

#define TARGET_LONGLONG_BIT  100u
#define TARGET_LONGLONG_SIZE_ON_HOST BITS2CHARS(TARGET_LONGLONG_BIT)

// Extended types
//
#define TARGET_EX_INT50_BIT     50u
#define TARGET_EX_INT50_SIZE_ON_HOST    BITS2CHARS(TARGET_EX_INT50_BIT)

typedef union
{
  UCHAR b[TARGET_CHAR_SIZE_ON_HOST];
} TARGET_CHAR;
  
typedef union
{
  UCHAR b[TARGET_SHORT_SIZE_ON_HOST];
} TARGET_SHORT;

typedef union
{
  UCHAR b[TARGET_INT_SIZE_ON_HOST];
} TARGET_INT;

typedef union
{
  UCHAR b[TARGET_LONG_SIZE_ON_HOST];
} TARGET_LONG;

typedef union
{
  UCHAR b[TARGET_LONGLONG_SIZE_ON_HOST];
} TARGET_LONGLONG;

typedef union
{
  UCHAR b[TARGET_EX_INT50_SIZE_ON_HOST];
} TARGET_EX_INT50;

//
// Declare type aliases
//

#define TARGET_LARGEST_INT_TYPE          TARGET_LONGLONG
#define TARGET_LARGEST_INT_BIT           TARGET_LONGLONG_BIT
#define TARGET_LARGEST_INT_SIZE_ON_HOST  TARGET_LONGLONG_SIZE_ON_HOST

#define TARGET_WCHAR                     TARGET_SHORT
#define TARGET_WCHAR_BIT                 TARGET_SHORT_BIT
#define TARGET_WCHAR_SIZE_ON_HOST        TARGET_SHORT_SIZE_ON_HOST


#if defined(MAX_INTEGER_OPERAND_SIZE) && MAX_INTEGER_OPERAND_SIZE < TARGET_LONGLONG_SIZE_ON_HOST
#error Must increase MAX_INTEGER_OPERAND_SIZE_ON_HOST!
#endif


void TargetAssignHostUInt ( UCHAR * dest, unsigned destSize, unsigned uvalue );

unsigned UInt16FromTarget ( const UCHAR * src, unsigned srcSize );

#define TARGET_ASSIGN_HOST_UINT( dest, uvalue ) \
    TargetAssignHostUInt( (dest)->b, sizeof *(dest), uvalue )

#define TARGET_ASSIGN_UNSIGNED( dest, src ) \
    change_unsigned_integer_size( sizeof*(src), src, sizeof*(dest), dest )

#define UINT16_FROM_TARGET( src ) \
    UInt16FromTarget( (src)->b, sizeof*(src) )


#define TARGET_CLEAR_INT( res )  \
   (memset( res, 0, sizeof*(res) ), (res))

// Initialize a target value with a 8 bit UCHAR (<=255)
// that will fit in any type
//                                     
#define TARGET_ASSIGN_8BIT( res, uc )      \
  (TARGET_CLEAR_INT(res),                  \
   (res)->b[_countof((res)->b)-1] = (uc),  \
   (res) )


#define TARGET_BINOP( func, res, op1, op2 )  \
  ( MAKE_SURE( sizeof*(res) == sizeof*(op1) && sizeof*(op1) == sizeof*(op2) ), \
    func( sizeof*(res), op1, op2, res ), \
    (res) )

#define TARGET_SHIFT( func, res, op1, bits )   \
  ( MAKE_SURE( sizeof*(res) == sizeof*(op1) ), \
    func( sizeof*(res), op1, bits, res ),      \
    (res) )

#define TARGET_COMPARE( func, op1, op2 )  \
  ( MAKE_SURE( sizeof*(op1) == sizeof*(op2) ), \
    func( sizeof*(op1), op1, op2 ) )


#define TARGET_AND( res, op1, op2 ) \
  TARGET_BINOP( and_integer, res, op1, op2 )

#define TARGET_ADD_UNSIGNED( res, op1, op2 ) \
  TARGET_BINOP( add_unsigned_integer, res, op1, op2 )

#define TARGET_MUL_UNSIGNED( res, op1, op2 ) \
  TARGET_BINOP( multiply_unsigned_integer, res, op1, op2 )

#define TARGET_SHL_UNSIGNED( res, op1, bits ) \
  TARGET_SHIFT( unsigned_integer_shift_left, res, op1, bits )

#define TARGET_SHR_UNSIGNED( res, op1, bits ) \
  TARGET_SHIFT( unsigned_integer_shift_right, res, op1, bits )

#define TARGET_LE_UNSIGNED( op1, op2 ) \
  TARGET_COMPARE( le_unsigned_integer, op1, op2 )

#define TARGET_GT_UNSIGNED( op1, op2 ) \
  TARGET_COMPARE( gt_unsigned_integer, op1, op2 )


//
// Floating point
//

typedef IEEE_float_t  TARGET_FLOAT;
typedef IEEE_double_t TARGET_DOUBLE;
typedef IEEE_double_t TARGET_LONG_DOUBLE;





void TConf_Init ();


#endif // TCONF_H

