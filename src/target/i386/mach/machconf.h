/*
  machconf.h:  
    Define machine types and provide operators for delaing with them. Front-end
    independant.
    This file is included by "mach.h". The required definitions are documented 
    there.
*/
#ifndef MACHCONF_H
#define MACHCONF_H

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
  of machconf.c/.h. Instead, the functions and macros provided by machconf.h
  must be used.
*/
#include "cocom\ammunition\arithm.h"
#include "cocom\ammunition\ieee.h"

///////////////////////////////////////////////////////////////////////////////
//  MachIntType_t 
//      Enumeration defining all separate integer types supported by the target
//
typedef enum MachIntType_t
{
  mt_I8, 
  mt_I16,
  mt_I32,
  mt_I48,
  mt_I64,

  mt_Last,
} MachIntType_t;


///////////////////////////////////////////////////////////////////////////////
//  TARGET_CHAR_BIT
//      Number of bits in a target _char_
//
#define TARGET_CHAR_BIT  8


///////////////////////////////////////////////////////////////////////////////
//  TARGET_Ixx_T
//      Opaque types holding a value of a target type on the host
//

#define priv_BITS2CHARS( x )   (((x)+CHAR_BIT-1)/CHAR_BIT)

// Numbers are represented in *Big Endian* - hi order first. I would
// prefer little endian, but COCOM uses big endian.
//

typedef struct
{
  uchar b[priv_BITS2CHARS(8)];
} TARGET_I8_T;

typedef struct
{
  uchar b[priv_BITS2CHARS(16)];
} TARGET_I16_T;

typedef struct
{
  uchar b[priv_BITS2CHARS(32)];
} TARGET_I32_T;

typedef struct
{
  uchar b[priv_BITS2CHARS(64)];
} TARGET_I64_T;

#undef priv_BITS2CHARS

///////////////////////////////////////////////////////////////////////////////
//  TARGET_IMAX_T
//      A synonim for the largest integer type of TARGET_Ixx_T, capable
//      of holding values of all other integer types.
//
typedef TARGET_I64_T TARGET_IMAX_T;


///////////////////////////////////////////////////////////////////////////////
//  TARGET_FLOAT_T
//  TARGET_DOUBLE_T
//  TARGET_LONG_DOUBLE_T
//      Opaque types holding a value of a target IEEE floating point type on the host.
//
typedef IEEE_float_t  TARGET_FLOAT_T;
typedef IEEE_double_t TARGET_DOUBLE_T;
typedef IEEE_double_t TARGET_LONG_DOUBLE_T;

///////////////////////////////////////////////////////////////////////////////
//  Operations on TARGET_Ixx_T values
//  ---------------------------------
//
void          priv_TargetAssignUInt16 ( uchar * dest, unsigned destSize, uint_fast16_t uvalue );
uint_fast16_t priv_UInt16FromTarget ( const uchar * src, unsigned srcSize );

#define TARGET_ASSIGN_HOST_CHAR( res, uc )   \
  (TARGET_CLEAR_INT(res),                    \
   (res)->b[NELEM((res)->b)-1] = (uchar)(uc),\
   (res) )

// Initialize a target value with a small number (<= 255 ?)
// fitting in any type except bool.
//
// The width of the maximum type must fit in a small number)
//                                     
#define TARGET_ASSIGN_SMALL( res, uc )      TARGET_ASSIGN_HOST_CHAR( res, uc )

#define SMALL_FROM_TARGET( src )   ((src)->b[NELEM((src)->b)-1])

#define TARGET_CLEAR_INT( res )    (CLEAR( res ))


#define TARGET_ASSIGN_UNSIGNED( dest, src ) \
    change_unsigned_integer_size( sizeof*(src), src, sizeof*(dest), dest )

#define UINT16_FROM_TARGET( src ) \
    priv_UInt16FromTarget( (src)->b, sizeof*(src) )

#define TARGET_ASSIGN_UINT16( dest, uvalue ) \
    priv_TargetAssignUInt16( (dest)->b, sizeof *(dest), uvalue )

#define priv_TARGET_BINOP( func, res, op1, op2 )  \
  ( MAKE_SURE( sizeof*(res) == sizeof*(op1) && sizeof*(op1) == sizeof*(op2) ), \
    func( sizeof*(res), op1, op2, res ), \
    (res) )

#define priv_TARGET_SHIFT( func, res, op1, bits )   \
  ( MAKE_SURE( sizeof*(res) == sizeof*(op1) ), \
    func( sizeof*(res), op1, bits, res ),      \
    (res) )

#define priv_TARGET_COMPARE( func, op1, op2 )  \
  ( MAKE_SURE( sizeof*(op1) == sizeof*(op2) ), \
    func( sizeof*(op1), op1, op2 ) )


#define TARGET_AND( res, op1, op2 ) \
  priv_TARGET_BINOP( integer_and, res, op1, op2 )

#define TARGET_OR( res, op1, op2 ) \
  priv_TARGET_BINOP( integer_or, res, op1, op2 )

#define TARGET_ADD_SIGNED( res, op1, op2 ) \
  priv_TARGET_BINOP( add_integer, res, op1, op2 )

#define TARGET_ADD_UNSIGNED( res, op1, op2 ) \
  priv_TARGET_BINOP( add_unsigned_integer, res, op1, op2 )

#define TARGET_MUL_UNSIGNED( res, op1, op2 ) \
  priv_TARGET_BINOP( multiply_unsigned_integer, res, op1, op2 )

#define TARGET_SHL_UNSIGNED( res, op1, bits ) \
  priv_TARGET_SHIFT( unsigned_integer_shift_left, res, op1, bits )

#define TARGET_SHR_UNSIGNED( res, op1, bits ) \
  priv_TARGET_SHIFT( unsigned_integer_shift_right, res, op1, bits )

#define TARGET_EQ( op1, op2 ) \
  priv_TARGET_COMPARE( eq_integer, op1, op2 )

#define TARGET_LE_UNSIGNED( op1, op2 ) \
  priv_TARGET_COMPARE( le_unsigned_integer, op1, op2 )

#define TARGET_GT_UNSIGNED( op1, op2 ) \
  priv_TARGET_COMPARE( gt_unsigned_integer, op1, op2 )

#define TARGET_LT_SIGNED( op1, op2 ) \
  priv_TARGET_COMPARE( lt_integer, op1, op2 )

#define TARGET_GT_SIGNED( op1, op2 ) \
  priv_TARGET_COMPARE( gt_integer, op1, op2 )



#endif // MACHCONF_H
