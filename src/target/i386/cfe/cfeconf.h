/*
  cfeconf.h: 
      Provides target-dependant definitions for the C front end.
      It is included by "cfetarg.h" - the required interface is documented there.
      Compiled as part of CFE (C Front End).
*/
#ifndef CFECONF_H
#define CFECONF_H

///////////////////////////////////////////////////////////////////////////////
//  RealType_t
//      Enumeration defining all target floating point types accessible by the CFE,
//      ordered by conversion rank.
//  RealType_t::rt_Last
//      The last value of RealType_t
//
typedef enum RealType_t
{
  rt_Float, rt_Double, rt_LongDouble,  rt_Last
} RealType_t;

///////////////////////////////////////////////////////////////////////////////
//  IntegerType_t
//      Enumeration describing all target integer types accessible by the CFE,
//      ordered by conversion rank. All types must be in pairs, signed-unsigned
//      (the signed is always even).
//      It must define at least the following values required by the C Standard:
//        it_Bool, it_SChar, it_UChar, it_Short, it_UShort, it_WChar,
//        it_Int, it_UInt, it_Long, it_ULong, it_LongLong, it_UlongLong,
//        it_IntMax, it_UIntMax.
//      Some of these values may be the same, depending on the types (for example
//      on many 32-bit systems Int==Long)
//
typedef enum IntegerType_t
{
  it_SBool,              it_Bool,  /* signed bool is for completeness */
  it_Int8,               it_UInt8,
  it_Int16,              it_UInt16,
  it_Int32,              it_UInt32,
  it_Int32L,             it_UInt32L,
  it_Int48,              it_UInt48,
  it_Int64,              it_UInt64,

  it_Last,

  // Here define how standard types map to actual types
  //
  it_SChar = it_Int8,
    it_UChar = it_UInt8,

  it_Short = it_Int16,
    it_UShort = it_UInt16,

  it_WChar = it_UInt16,

  it_Int = it_Int32,
    it_UInt = it_UInt32,

  it_Long = it_Int32L,        
    it_ULong = it_UInt32L,

  it_LongLong = it_Int64,    
    it_ULongLong = it_UInt64,

  it_IntMax = it_Int64,
    it_UIntMax = it_UInt64,
} IntegerType_t;

///////////////////////////////////////////////////////////////////////////////
//  TARGET_CHAR_T
//  TARGET_WCHAR_T
//  TARGET_INT_T
//  TARGET_INTMAX_T
//      Synomims of the correspodning machine types TARGET_Ixx_T.
//
typedef TARGET_I8_T   TARGET_CHAR_T;
typedef TARGET_I16_T  TARGET_WCHAR_T;
typedef TARGET_I32_T  TARGET_INT_T;
typedef TARGET_I64_T  TARGET_INTMAX_T;


///////////////////////////////////////////////////////////////////////////////
//  TARGET_WCHAR_BIT
//  TARGET_INT_BIT
//  TARGET_INTMAX_BIT
//      Number of bits of target types
//
#define TARGET_WCHAR_BIT    16
#define TARGET_INT_BIT      32
#define TARGET_INTMAX_BIT   64

///////////////////////////////////////////////////////////////////////////////
//  TARGET_INT_SIZE
//      Size in characters of target types
//
#define TARGET_INT_SIZE  (TARGET_INT_BIT / TARGET_CHAR_BIT)


///////////////////////////////////////////////////////////////////////////////
//  Extended declararion specifiers
//  -------------------------------
//
//  TExtDeclSpec
//      A structure containing target-specific extended declspec information. For
//      example in Win32/x86 these may be: dllimport,dllexport, etc.
//
typedef struct TExtDeclSpec
{
  uchar dllImportExport : 2; // 0 - no, 1 - import, 2 - export
  bool  noreturn        : 1;
} TExtDeclSpec;

///////////////////////////////////////////////////////////////////////////////
//  Extended qualifier attributes
//  -----------------------------
//
//  ExtQualAttr_t
//      A value containing extended qualifier attributes like cdecl,pascal,far,near, etc.
//
typedef uint_least16_t ExtQualAttr_t;

///////////////////////////////////////////////////////////////////////////////
//  CFE_DEFAULT_CHAR_IS_SIGNED
//      true if _char_ is a signed type.
//
#define CFE_DEFAULT_CHAR_IS_SIGNED  true

///////////////////////////////////////////////////////////////////////////////
//  CFE_DEFAULT_CHAR_GOES_LEFT
//      true if consecutive characters in a character constant (e.g. the b and c in 'abc')
//      are shifted to the left.
//
#define CFE_DEFAULT_CHAR_GOES_LEFT  true

#endif // CFECONF_H
