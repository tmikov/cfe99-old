/*
  cfe_targ.h:  Wraps together all target-dependant definitions for the C Front End.
*/
#ifndef CFETARG_H
#define CFETARG_H

/*
  cfeconf.h must define
  ======================

  | 6.2.5 Types
  |
  | 10 There are three real floating types, designated as float, double, and long
  | double.32) The set of values of the type float is a subset of the set of values of the
  | type double; the set of values of the type double is a subset of the set of values of the
  | type long double.

  RealType_t
      Enumeration defining all target floating point types accessible by the CFE,
      ordered by conversion rank.
  RealType_t::rt_Last
      The last value of RealType_t

  | 6.2.5 Types
  |
  | 4 There are five standard signed integer types, designated as signed char, short
  | int, int, long int, and long long int. (These and other types may be
  | designated in several additional ways, as described in 6.7.2.) There may also be
  | implementation-defined extended signed integer types.28) The standard and extended
  | signed integer types are collectively called signed integer types.29)

  IntegerType_t
      Enumeration describing all target integer types accessible by the CFE,
      ordered by conversion rank. All types must be in pairs, signed-unsigned
      (the signed is always even).
      It must define at least the following values required by the C Standard:
        it_Bool, it_SChar, it_UChar, it_Short, it_UShort, it_WChar,
        it_Int, it_UInt, it_Long, it_ULong, it_LongLong, it_UlongLong,
        it_IntMax, it_UIntMax.
      Some of these values may be the same, depending on the types (for example
      on many 32-bit systems Int==Long)

  TARGET_CHAR_T
  TARGET_WCHAR_T
  TARGET_INT_T
  TARGET_INTMAX_T
      Synomims of the correspodning machine types TARGET_Ixx_T.

  TARGET_WCHAR_BIT
  TARGET_INT_BIT
  TARGET_INTMAX_BIT
      Number of bits of target types

  TARGET_INT_SIZE
      Size in characters of target types

  TExtDeclSpec
      A structure containing target-specific extended declspec information. For
      example in Win32/x86 these may be: dllimport,dllexport, etc.

  ExtQualAttr_t
      A value containing extended qualifier attributes like cdecl,pascal,far,near, etc.

  CFE_DEFAULT_CHAR_IS_SIGNED
      true if _char_ is a signed type.

  CFE_DEFAULT_CHAR_GOES_LEFT
      true if consecutive characters in a character constant (e.g. the b and c in 'abc')
      are shifted to the left.
*/
#include "cfeconf.h"              // target-specific CFE definitions

// Define the CFE view of an integer type.
//
typedef struct TIntegerTypeDesc
{
  const char * name;

  char rank;        // integer conversion rank. 
  bool isSigned;
  bool isExtended;
  char L;           // how long (1=long, 2=long long)
  MachIntType_t machId;
  IntegerType_t brother;  // the corresponding signed/unsigned type

  TMachIntTypeDesc * pMachType;
  TARGET_IMAX_T minValue, maxValue;
} TIntegerTypeDesc;


// =====================================================
//
// Public imports from cfeconf.c
//

extern const char * const RealType_Name[rt_Last]; 

//    The types are sorted according to their integer conversion rank
//    in pairs signed-unsigned
//
extern TIntegerTypeDesc IntegerTypesDesc[it_Last]; 

#define Int_Desc  IntegerTypesDesc[it_Int]
#define UInt_Desc IntegerTypesDesc[it_UInt]

//--------------------------------------------------------------------------
// Name         ExtDeclSpec_Make 
//
// Allocates the structure corresponding to the string. On error
// (e.g. invalid qualifier) generates an error message.
//--------------------------------------------------------------------------
TExtDeclSpec * ExtDeclSpec_Make ( STRING ident );

//--------------------------------------------------------------------------
// Name         ExtDeclSpec_Add 
//
// Combines two sets of extended declspec-s. On error (e.g. repeated
// or conflicting declspec-s) generates an error message.
//--------------------------------------------------------------------------
TExtDeclSpec * ExtDeclSpec_Add ( TExtDeclSpec * a, TExtDeclSpec * b );

//--------------------------------------------------------------------------
// Name         CfeConf_ValidateExtDeclSpec 
//
// Validates the extended declaration specifiers assigned to _sym_. Generates
// apropriate error messages and corrects the problem.
//--------------------------------------------------------------------------
void CfeConf_ValidateExtDeclSpec ( TSymbol * sym, bool bInit );

//--------------------------------------------------------------------------
// Name         CfeConf_CompositeExtDeclSpec 
//
// For redeclaration of a symbol, makes sure that the extended declspec-s
// of the two declarations are compatible, generating apropriate error messages
// and fixing any probkels. Combines the two declspec-s in a composite one,
// modifying newSpec inplace!
//--------------------------------------------------------------------------
TExtDeclSpec * CfeConf_CompositeExtDeclSpec ( const TSymbol * prevSym, TExtDeclSpec * newSpec  );


//--------------------------------------------------------------------------
// Name         ExtQualAttr_Make 
//
// Returns a value describing the qualifier corresponding to the string. On error
// (e.g. invalid qualifier) generates an error message.
//--------------------------------------------------------------------------
ExtQualAttr_t  ExtQualAttr_Make ( STRING ident );

//--------------------------------------------------------------------------
// Name         ExtQualAttr_Add 
//
// Combines two sets of extended qualifier attributes. On error (e.g. repeated
// or conflicting attributes) generates an error message.
//--------------------------------------------------------------------------
ExtQualAttr_t  ExtQualAttr_Add ( ExtQualAttr_t a, ExtQualAttr_t b );

//--------------------------------------------------------------------------
// Name         CfeConf_MatchExtendedFloat 
//
// Returns the floating point corresponding to the specified width.
// On error does *NOT* generate an error and returns rt_Last.
//--------------------------------------------------------------------------
RealType_t CfeConf_MatchExtendedFloat ( unsigned width );

//--------------------------------------------------------------------------
// Name         CfeConf_Init 
//
// 
//--------------------------------------------------------------------------
void CfeConf_Init ( void );

//--------------------------------------------------------------------------
// Name         CfeConf_GcMarkRoots 
//
// 
//--------------------------------------------------------------------------
void CfeConf_GcMarkRoots ( void );

#endif // CFETARG_H