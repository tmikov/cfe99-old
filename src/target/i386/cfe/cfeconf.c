#include "precomp.h"
#include "cfe_priv.h"

/*
6.2.5 Types

2 An object declared as type _Bool is large enough to store the values 0 and 1.

3 An object declared as type char is large enough to store any member of the basic
execution character set. If a member of the basic execution character set is stored in a
char object, its value is guaranteed to be positive. If any other character is stored in a
char object, the resulting value is implementation-defined but shall be within the range
of values that can be represented in that type.

4 There are five standard signed integer types, designated as signed char, short
int, int, long int, and long long int. (These and other types may be
designated in several additional ways, as described in 6.7.2.) There may also be
implementation-defined extended signed integer types.28) The standard and extended
signed integer types are collectively called signed integer types.29)

5 An object declared as type signed char occupies the same amount of storage as a
‘‘plain’’ char object. A ‘‘plain’’ int object has the natural size suggested by the
architecture of the execution environment (large enough to contain any value in the range
INT_MIN to INT_MAX as defined in the header <limits.h>).

6 For each of the signed integer types, there is a corresponding (but different) unsigned
integer type (designated with the keyword unsigned) that uses the same amount of
storage (including sign information) and has the same alignment requirements. The type
_Bool and the unsigned integer types that correspond to the standard signed integer
types are the standard unsigned integer types. The unsigned integer types that
correspond to the extended signed integer types are the extended unsigned integer types.
The standard and extended unsigned integer types are collectively called unsigned integer
types.30)

7 The standard signed integer types and standard unsigned integer types are collectively
called the standard integer types, the extended signed integer types and extended
unsigned integer types are collectively called the extended integer types.

8 For any two integer types with the same signedness and different integer conversion rank
(see 6.3.1.1), the range of values of the type with smaller integer conversion rank is a
subrange of the values of the other type.

9 The range of nonnegative values of a signed integer type is a subrange of the
corresponding unsigned integer type, and the representation of the same value in each
type is the same.31) A computation involving unsigned operands can never overflow,
because a result that cannot be represented by the resulting unsigned integer type is
reduced modulo the number that is one greater than the largest value that can be
represented by the resulting type.


6.3.1.1 Boolean, characters, and integers

1 Every integer type has an integer conversion rank defined as follows:


  — No two signed integer types shall have the same rank, even if they hav e the same
representation.

  — The rank of a signed integer type shall be greater than the rank of any signed integer
type with less precision.

  — The rank of long long int shall be greater than the rank of long int, which
shall be greater than the rank of int, which shall be greater than the rank of short
int, which shall be greater than the rank of signed char.

  — The rank of any unsigned integer type shall equal the rank of the corresponding
signed integer type, if any.

  — The rank of any standard integer type shall be greater than the rank of any extended
integer type with the same width.

  — The rank of char shall equal the rank of signed char and unsigned char.

  — The rank of _Bool shall be less than the rank of all other standard integer types.

  — The rank of any enumerated type shall equal the rank of the compatible integer type
(see 6.7.2.2).

  — The rank of any extended signed integer type relative to another extended signed
integer type with the same precision is implementation-defined, but still subject to the
other rules for determining the integer conversion rank.

  — For all integer types T1, T2, and T3, if T1 has greater rank than T2 and T2 has
greater rank than T3, then T1 has greater rank than T3.
*/

static struct CfeConfRoots_t
{
  STRING str_dllimport;
  STRING str_dllexport;
  STRING str_noreturn;

  STRING str_cdecl;
  STRING str_stdcall;
  STRING str_fastcall;
  STRING str_near;
  STRING str_far;
} CfeConfRoots;

// ExtQualAttr layout
// ================================================
//
// Bits 0-1: calling convention
//
#define EXTATTR_CALLCONV_MASK  3
  #define EXTATTR_CALLCONV_NONE     (0 << 0)
  #define EXTATTR_CALLCONV_CDECL    (1 << 0)
  #define EXTATTR_CALLCONV_STDCALL  (2 << 0)
  #define EXTATTR_CALLCONV_FASTCALL (3 << 0)
//
// Bits 2-3: pointer size
//
#define EXTATTR_PTR_MASK       (3 << 2)
  #define EXTATTR_PTR_NONE          (0 << 2)
  #define EXTATTR_PTR_NEAR          (1 << 2)
  #define EXTATTR_PTR_FAR           (2 << 2)

#define nil 0  // signify a value to be initialized later

const char * const RealType_Name[rt_Last] = 
{
  "float",
  "double",
  "long double"
};


// The types are sorted according to their integer conversion rank
// in pairs signed-unsigned
TIntegerTypeDesc IntegerTypesDesc[] = 
{
  // rank, isSigned, isExtended, L, machType, brother, minValue, maxValue

  { "sbool",  1, nil, false, 0, mt_I8  }, { "bool",    nil },
  { "int8",   2, nil, false, 0, mt_I8  }, { "uint8",   nil },
  { "int16",  3, nil, false, 0, mt_I16 }, { "uint16",  nil },
  { "int32",  4, nil, false, 0, mt_I32 }, { "uint32",  nil },
  { "int32L", 5, nil, false, 1, mt_I32 }, { "uint32L", nil },
  { "int48",  6, nil, true,  0, mt_I48 }, { "uint48",  nil },
  { "int64",  7, nil, false, 2, mt_I64 }, { "uint64",  nil },
};

//--------------------------------------------------------------------------
// Name         InitTypes
//
// Complete the type descriptions by filling the fields that can easily
// be calculated at runtime. 
//--------------------------------------------------------------------------
static void InitTypes ( void )
{
  unsigned i;

  MAKE_SURE( NELEM( IntegerTypesDesc ) == it_Last );
  MAKE_SURE( NELEM( IntegerTypesDesc ) % 2 == 0 );

  for ( i = 0; i < NELEM( IntegerTypesDesc ); i += 2 )
  {
    const char * name; // to save the name before copying

    IntegerTypesDesc[i].pMachType = &MachIntTypesDesc[IntegerTypesDesc[i].machId];

    // Copy the data which stays the same
    // (but preserve the name)
    //
    name = IntegerTypesDesc[i+1].name;
    IntegerTypesDesc[i+1] = IntegerTypesDesc[i];
    IntegerTypesDesc[i+1].name = name;

    // Set the signedness
    //
    IntegerTypesDesc[i].isSigned = true;
    IntegerTypesDesc[i+1].isSigned = false;

    // Set the brother values
    //
    IntegerTypesDesc[i].brother = i + 1;
    IntegerTypesDesc[i+1].brother = i;

    // Set min/maxValue for the signed
    //
    IntegerTypesDesc[i].minValue = IntegerTypesDesc[i].pMachType->minValue;
    IntegerTypesDesc[i].maxValue = IntegerTypesDesc[i].pMachType->maxValue;

    // Set min/maxValue for the unsigned
    //
    TARGET_CLEAR_INT( &IntegerTypesDesc[i+1].minValue );
    IntegerTypesDesc[i+1].maxValue = IntegerTypesDesc[i].pMachType->mask;
  }
};


//--------------------------------------------------------------------------
// Name         CfeConf_Init 
//
// 
//--------------------------------------------------------------------------
void CfeConf_Init ( void )
{
  InitTypes();

  CLEAR( &CfeConfRoots );

  // Allocate __declspec strings
  //
  CfeConfRoots.str_dllimport = NewString( "dllimport" );
  CfeConfRoots.str_dllexport = NewString( "dllexport" );
  CfeConfRoots.str_noreturn  = NewString( "noreturn" );

  // Allocate __attr strings
  //
  CfeConfRoots.str_cdecl    = NewString( "cdecl" );
  CfeConfRoots.str_stdcall  = NewString( "stdcall" );
  CfeConfRoots.str_fastcall = NewString( "fastcall" );
  CfeConfRoots.str_near     = NewString( "near" );
  CfeConfRoots.str_far      = NewString( "far" );
};

//--------------------------------------------------------------------------
// Name         CfeConf_GcMarkRoots
//
// 
//--------------------------------------------------------------------------
void CfeConf_GcMarkRoots ( void )
{
  struct CfeConfRoots_t_loc
  {
    STRING str_dllimport;
    STRING str_dllexport;
    STRING str_noreturn;

    STRING str_cdecl;
    STRING str_stdcall;
    STRING str_fastcall;
    STRING str_near;
    STRING str_far;
  };
  MAKE_SURE( sizeof(struct CfeConfRoots_t_loc) == sizeof(struct CfeConfRoots_t) );

  GC_MARK( CfeConfRoots.str_dllimport );
  GC_MARK( CfeConfRoots.str_dllexport );
  GC_MARK( CfeConfRoots.str_noreturn );
  GC_MARK( CfeConfRoots.str_cdecl );
  GC_MARK( CfeConfRoots.str_stdcall );
  GC_MARK( CfeConfRoots.str_fastcall );
  GC_MARK( CfeConfRoots.str_near );
  GC_MARK( CfeConfRoots.str_far );
};

//--------------------------------------------------------------------------
// Name         CfeConf_MatchExtendedFloat 
//
// returns the RealType_t corresponding to the specified floating
// point type width
//
// On error returns rt_Last
//--------------------------------------------------------------------------
RealType_t CfeConf_MatchExtendedFloat ( unsigned width )
{
  switch (width)
  {
  case 32: return rt_Float;
  case 64: return rt_Double;
  case 80: return rt_LongDouble;
  default: return rt_Last;
  }
};


//--------------------------------------------------------------------------
// Name         ExtQualAttr_Make 
//
// 
//--------------------------------------------------------------------------
ExtQualAttr_t  ExtQualAttr_Make ( STRING ident )
{
  if (ident == CfeConfRoots.str_cdecl)
    return EXTATTR_CALLCONV_CDECL;
  if (ident == CfeConfRoots.str_stdcall)
    return EXTATTR_CALLCONV_STDCALL;
  if (ident == CfeConfRoots.str_fastcall)
    return EXTATTR_CALLCONV_FASTCALL;

  if (ident == CfeConfRoots.str_near)
    return EXTATTR_PTR_NEAR;
  if (ident == CfeConfRoots.str_far)
    return EXTATTR_PTR_FAR;

  TST,CompilerError( "'__attr(%s)': unrecognized __attr", STRING_Name(ident) );
  return 0;
};

//--------------------------------------------------------------------------
// Name         ExtQualAttr_Add
//
// Combines target-dependent type attributes (e.g. _near, _far),
// generating an apropriate error message if necessary.
// 
// Note: attributes are kept in TQualNode::extQualAttr
//--------------------------------------------------------------------------
ExtQualAttr_t ExtQualAttr_Add ( ExtQualAttr_t a, ExtQualAttr_t b )
{
  // Check for more than one calling convention attribute
  //
  if ((a & EXTATTR_CALLCONV_MASK) != 0 && (b & EXTATTR_CALLCONV_MASK) != 0)
  {
    TST,CompilerError( "More than one calling convention attribute" );

    // Ignore the second one
    b &= ~EXTATTR_CALLCONV_MASK;
  }

  // Check for more than one pointer size
  //
  if ((a & EXTATTR_PTR_MASK) != 0 && (b & EXTATTR_PTR_MASK) != 0)
  {
    CompilerError( "More than one pointer size attribute" );

    // Ignore the second one
    b &= ~EXTATTR_PTR_MASK;
  }
  
  return a | b;
};

//--------------------------------------------------------------------------
// Name         New_TExtDeclSpec 
//
// 
//--------------------------------------------------------------------------
static TExtDeclSpec * New_TExtDeclSpec ( void )
{
  TExtDeclSpec * res = GC_NEW_ATOMIC( TExtDeclSpec );
  CLEAR( res );
  return res;
};

//--------------------------------------------------------------------------
// Name         ExtDeclSpec_Make 
//
// On error reports the error and returns NULL.
//--------------------------------------------------------------------------
TExtDeclSpec * ExtDeclSpec_Make ( STRING ident )
{
  TExtDeclSpec * res = NULL;

  if (ident == CfeConfRoots.str_dllimport)
  {
    (res = New_TExtDeclSpec())->dllImportExport = 1;
  }
  else
  if (ident == CfeConfRoots.str_dllexport)
  {
    (res = New_TExtDeclSpec())->dllImportExport = 2;
  }
  else
  if (ident == CfeConfRoots.str_noreturn)
  {
    (res = New_TExtDeclSpec())->noreturn = true;
  }
  else
    TST,CompilerError( "'%s': unrecognized __declspec", STRING_Name(ident) );

  return res;
};

//--------------------------------------------------------------------------
// Name         ExtDeclSpec_Add 
//
// 
//--------------------------------------------------------------------------
TExtDeclSpec * ExtDeclSpec_Add ( TExtDeclSpec * a, TExtDeclSpec * b )
{
  if (a == NULL)
    return b;
  if (b == NULL)
    return a;

  if (a->dllImportExport != 0 && b->dllImportExport != 0)
    TST,CompilerError( "More than one __declspec(dllimport/dllexport) in declaration" );
  else
    a->dllImportExport |= b->dllImportExport;

  if (a->noreturn != 0 && b->noreturn != 0)
    TST,CompilerError( "More than one __declspec(noreturn) in declaration" );
  else
    a->noreturn |= b->noreturn;

  return a;
};

//--------------------------------------------------------------------------
// Name         CfeConf_ValidateExtDeclSpec 
//
// Note: we are free to modify extDeclSpec, since it was created for and 
// is attached to the particluar declaration.
//--------------------------------------------------------------------------
void CfeConf_ValidateExtDeclSpec ( TSymbol * sym, bool bInit )
{
  TExtDeclSpec * extDeclSpec;

  ASSERT( sym->type );

  // If no ExtDeclSpec
  if (NULL == (extDeclSpec = sym->extDeclSpec) )
    return;

  // noreturn only apples to functions
  //
  if (extDeclSpec->noreturn && 
      sym->type->tnode->typeKind != TYPE_FUNCTION)
  {
    TST,CompilerError( "'%s': __declspec(noreturn) only applies to functions", Symbol_Name(sym) );
    extDeclSpec->noreturn = false;
  }

  // can't apply dllimport to a function definition
  //
  if (extDeclSpec->dllImportExport == 1 && bInit)
  {
    TST,CompilerError( "'%s': __declspec(dllimport) used with a function definition", Symbol_Name(sym) );
    extDeclSpec->dllImportExport = 0;
  }

  // dllimport only applies to extern
  if (extDeclSpec->dllImportExport == 1 && sym->storageClass != SC_EXTERN)
  {
    TST,CompilerError( "'%s': __declspec(dllimport) must be used on external definitions", Symbol_Name(sym) );
    extDeclSpec->dllImportExport = 0;
  }

  // dllimport/dllexport only apply to public symbols with static storage
  // duration
  //
  if (extDeclSpec->dllImportExport != 0 &&
       (sym->storageDuration != STORAGE_DURATION_STATIC &&
          sym->storageDuration != STORAGE_DURATION_CODE ||
        sym->storageClass == SC_STATIC) )
  {
    TST,CompilerError( "'%s': __declspec(dllimport/dllexport) only applies to public symbols", Symbol_Name(sym) );
    extDeclSpec->dllImportExport = 0;
  }
};

//--------------------------------------------------------------------------
// Name         CfeConf_CompositeExtDeclSpec 
//
// For redeclaration of a symbol, makes sure that the extended declspec-s
// of the two declarations are compatible, generating apropriate error messages
// and fixing any probkels. Combines the two declspec-s in a composite one,
// modifying newSpec inplace!
//--------------------------------------------------------------------------
TExtDeclSpec * CfeConf_CompositeExtDeclSpec ( const TSymbol * prevSym, TExtDeclSpec * newSpec  )
{
  const TExtDeclSpec * pPrevSpec;
  TExtDeclSpec emptySpec;           // we use this if prev->extDeclSpec==NULL

  ASSERT( prevSym != NULL );

  // If both have no extDeclSpec, we have nothing to combine
  //
  if (newSpec == NULL && prevSym->extDeclSpec == NULL)
    return newSpec;

  // Obtain a pointer to the previous extDeclSpec, handling the case when
  // the previous declaration had no extDeclSpec at all
  //
  if (NULL == (pPrevSpec = prevSym->extDeclSpec) )
  {
    CLEAR( &emptySpec );
    pPrevSpec = &emptySpec;
  }

  // Obtain a pointer to the result extDeclSpec, creating it if we have to
  //
  if (NULL == newSpec )
    newSpec = New_TExtDeclSpec();

  // Simply combine "noreturn"
  //
  newSpec->noreturn |= pPrevSpec->noreturn;

  /*
    dllexport/dllimport valid cases:

    prev       sym
    ----------------
    nothing    nothing
    dllexport  dllexport
    dllimport  dllimport
    dllexport  nothing
  */
  if (pPrevSpec->dllImportExport != newSpec->dllImportExport &&
      (pPrevSpec->dllImportExport != 2 || newSpec->dllImportExport != 0))
  {
    TST,CompilerError( "'%s': __declspec(dllimport/dllexport) conflict with previous declaration" , Symbol_Name(prevSym) );
    ShowPrevDeclaration( prevSym );

    newSpec->dllImportExport = 0;
  }
  else
    newSpec->dllImportExport = pPrevSpec->dllImportExport;

  return newSpec;
};