/*
  c-dspecs.c

  Allocate and maniuplaue a edclaration specifiers structures (TDeclSpecs)
*/
#include "precomp.h"
#include "cfe_priv.h"


//--------------------------------------------------------------------------
// Name         TDeclSpecs_GcMark 
//
// 
//--------------------------------------------------------------------------
void TDeclSpecs_GcMark ( const TDeclSpecs * ds )
{
  struct TDeclSpecs_loc
  {
    enum DeclBaseType_t baseType;   

    uchar isComplex  : 2; // 0 - no, 1 - imaginary, 2 - complex
    uchar isSigned   : 2; // 0 - no, 1 - unsigned, 2 - signed
    uchar isLong     : 2; // 0 - no, 1 - short, 2 - long, 3 - long long
    uchar isConst    : 1;
    uchar isVolatile : 1;
    uchar isRestrict : 1;
    uchar isInline   : 1;

    union
    {
      IntegerType_t  extInt;    // DT_EXT_INT: index of the type
      RealType_t     extFloat;  // DT_EXT_FLOAT: index of the type
      TQualNode    * typeName;  // DT_TYPENAME
      TTypeNode    * tag;       // DT_TAG
    } u;

    ExtQualAttr_t   extQualAttr; 
    StorageClass_t  sclass;
    TExtDeclSpec  * extDeclSpec;

    TQualNode * type; // set by DSpecs_GenType()
  };
  MAKE_SURE( sizeof(struct TDeclSpecs) == sizeof(struct TDeclSpecs_loc) );

  switch (ds->baseType)
  {
  case DT_TYPENAME:
    GC_MARK( ds->u.typeName );
    break;

  case DT_TAG:
    GC_MARK( ds->u.tag );
    break;
  }

  GC_MARK( ds->extDeclSpec );
  GC_MARK( ds->type );
};

//--------------------------------------------------------------------------
// Name         New_DSpecs
//
// 
//--------------------------------------------------------------------------
TDeclSpecs * New_DSpecs ( int token )
{
  TDeclSpecs * ds = GC_NEW( TDeclSpecs );

  switch (token)
  {
  // baseType
  //
  case C_VOID:   ds->baseType = DT_VOID;   break;
  case C__BOOL:  ds->baseType = DT_BOOL;   break;
  case C_CHAR:   ds->baseType = DT_CHAR;   break;
  case C_INT:    ds->baseType = DT_INT;    break;

  //C_EXT_INT is handled in New_DSpecs_ExtInt
  //case C_EXT_INT: ds->baseType = DT_EXT_INT; break;

  case C_FLOAT:  ds->baseType = DT_FLOAT;  break;
  case C_DOUBLE: ds->baseType = DT_DOUBLE; break;

  //C_EXT_FLOAT  is handled in New_DSpecs_ExitFloat
  //case C_EXT_FLOAT: ds->baseType = DT_EXT_FLOAT; break;

  // isComplex
  //
  case C__IMAGINARY: ds->isComplex = 1; break;
  case C__COMPLEX:   ds->isComplex = 2; break;

  // isSigned
  //
  case C_UNSIGNED: ds->isSigned = 1; break;
  case C_SIGNED:   ds->isSigned = 2; break;

  // isLong
  //
  case C_SHORT: ds->isLong = 1; break;
  case C_LONG:  ds->isLong = 2; break;

  // attributes
  //
  case C_CONST:    ds->isConst = true;    break;
  case C_VOLATILE: ds->isVolatile = true; break;
  case C_RESTRICT: ds->isRestrict = true; break;

  case C_INLINE:   ds->isInline = true; break;

  // storage class
  //
  case C_EXTERN:   ds->sclass = SC_EXTERN;   break;
  case C_STATIC:   ds->sclass = SC_STATIC;   break;
  case C_AUTO:     ds->sclass = SC_AUTO;     break;
  case C_REGISTER: ds->sclass = SC_REGISTER; break;
  case C_TYPEDEF:  ds->sclass = SC_TYPEDEF;  break;

  default:
    ASSERT( 0 );
    CompilerError( "Internal compiler error: unexpected declaration token" );
  }

  return ds;
};

//--------------------------------------------------------------------------
// Name         New_DSpecs_ExtInt 
//
// Locates a signed integer type with the precision specified in astNum,
// using the IntegerTypesDesc[] table.
//
// astNum must be ast_intConst.
//
// On any error generates an apropriate error message and returns "int".
//--------------------------------------------------------------------------
TDeclSpecs * New_DSpecs_ExtInt ( const TAst * astNum )
{
  uint_fast16_t  width;
  IntegerType_t  it;
  TDeclSpecs     * ds;

  if (astNum->code != ast_intConst)
  {
    // This should never happen since we are only being called from
    // C_EXT_INT '(' NUMBER ')'
    ASSERT( 0 ); 
    goto error;
  }

  width = UINT16_FROM_TARGET( &astNum->u.intConst );
  if (overflow_bit)
  {
    TST,CompilerErrorLoc( &astNum->loc, "Too large __int width" );
    goto error;
  }

  // Find a type with correct width
  for ( it = it_Bool + 1; it < it_Last; ++it )
  {
    // We check only signed types
    if (IntegerTypesDesc[it].isSigned && 
        IntegerTypesDesc[it].pMachType->precision == width)
    {
      break;
    }
  }

  if (it == it_Last)
  {
    TST,CompilerErrorLoc( &astNum->loc, "Invalid __int width (%u)", width );
    goto error;
  }

  // Here we use a witty construct, so that we skip the error label
  // in the good cases, but still have only one function exit point
  //
  if (1)
  {
    ds = GC_NEW( TDeclSpecs );
    ds->baseType = DT_EXT_INT;
    ds->u.extInt = it;
  }
  else
  {
error:
    ds = New_DSpecs( C_INT ); // In error cases return "int"
  }

  return ds;
};

//--------------------------------------------------------------------------
// Name         New_DSpecs_ExtFloat 
//
// Locates a signed integer type with the precision specified in astNum.
// astNum must be ast_intConst.
//
// On any error generates an apropriate error message and returns "float".
//--------------------------------------------------------------------------
TDeclSpecs * New_DSpecs_ExtFloat ( const TAst * astNum )
{
  uint_fast16_t  width;
  RealType_t     rt;
  TDeclSpecs     * ds;

  if (astNum->code != ast_intConst)
  {
    // This should never happen since we are only being called from
    // C_EXT_INT '(' NUMBER ')'
    ASSERT( 0 ); 
    goto error;
  }

  width = UINT16_FROM_TARGET( &astNum->u.intConst );
  if (overflow_bit)
  {
    TST,CompilerErrorLoc( &astNum->loc, "Too large __float width" );
    goto error;
  }

  // Find a type with correct width
  rt = CfeConf_MatchExtendedFloat( width );

  if (rt == rt_Last)
  {
    TST,CompilerErrorLoc( &astNum->loc, "Invalid __float width (%u)", width );
    goto error;
  }

  // Here we use a witty construct, so that we skip the error label
  // in the good cases, but still have only one function exit point
  //
  if (1)
  {
    ds = GC_NEW( TDeclSpecs );
    ds->baseType = DT_EXT_FLOAT;
    ds->u.extFloat = rt;
  }
  else
  {
error:
    ds = New_DSpecs( C_FLOAT ); // In error cases return "float"
  }

  return ds;
};

//--------------------------------------------------------------------------
// Name         New_DSpecs_Typename 
//
// 
//--------------------------------------------------------------------------
TDeclSpecs * New_DSpecs_Typename ( const TQualNode * type )
{
  TDeclSpecs * ds = GC_NEW( TDeclSpecs );

  ASSERT( type != NULL );

  ds->baseType = DT_TYPENAME;
  ds->u.typeName = type;

  return ds;
}

//--------------------------------------------------------------------------
// Name         New_DSpecs_Tag 
//
// 
//--------------------------------------------------------------------------
TDeclSpecs * New_DSpecs_Tag ( const TTypeNode * tag )
{
  TDeclSpecs * ds = GC_NEW( TDeclSpecs );

  ASSERT( tag != NULL );

  ds->baseType = DT_TAG;
  ds->u.tag = tag;

  return ds;
}

//--------------------------------------------------------------------------
// Name         New_DSpecs_ExtDeclSpec 
//
// Note: if _extDeclSpec_ is NULL, does not allocate anything
//--------------------------------------------------------------------------
TDeclSpecs * New_DSpecs_ExtDeclSpec ( TExtDeclSpec * extDeclSpec )
{ 
  TDeclSpecs * ds = NULL;

  if (extDeclSpec != NULL)
  {
    ds = GC_NEW( TDeclSpecs );
    ds->extDeclSpec = extDeclSpec;
  }

  return ds;
};

//--------------------------------------------------------------------------
// Name         New_DSpecs_ExtQualAttr 
//
// Note: if extQualAttr is NULL, does not allocate anything
//--------------------------------------------------------------------------
TDeclSpecs * New_DSpecs_ExtQualAttr ( ExtQualAttr_t extQualAttr )
{
  TDeclSpecs * ds = NULL;

  if (extQualAttr != 0)
  {
    ds = GC_NEW( TDeclSpecs );
    ds->extQualAttr = extQualAttr;
  }

  return ds;
};

//--------------------------------------------------------------------------
// Name         DSpecs_Add 
//
//--------------------------------------------------------------------------
TDeclSpecs * DSpecs_Add ( TDeclSpecs * a, TDeclSpecs * b )
{
  if (a == NULL)
    return b;
  if (b == NULL)
    return a;

  // base type 
  //
  if (a->baseType != 0 && b->baseType != 0)
    TST,CompilerError( "More than one type in declaration" );
  else
  {
    a->baseType |= b->baseType;

    if (b->baseType != DT_NONE) // Copy the base type data
      a->u = b->u;
  }

  // complex/imaginary
  //
  if (a->isComplex != 0 && b->isComplex != 0)
    TST,CompilerError( "'_Complex'/'_Imaginary' conflict in declaration" );
  else
    a->isComplex |= b->isComplex;

  // signed/unsigned
  //
  if (a->isSigned != 0 && b->isSigned != 0)
    TST,CompilerError( "'signed'/'unsigned' conflict in declaration" );
  else
    a->isSigned |= b->isSigned;

  // short/long/long long
  //
  if (a->isLong != 0 && b->isLong != 0)
  {
    // the only valid combination is long + long
    if (a->isLong == 2 && b->isLong == 2)
      a->isLong = 3;
    else
      TST,CompilerError( "'short'/'long'/'long long' conflict in declaration" );
  }
  else
    a->isLong |= b->isLong;

  // const
  //
  if ((a->isConst & b->isConst) != 0)
    TST,CompilerStrictError( "More than one 'const' in declaration" ); 
  else
    a->isConst |= b->isConst;

  // volatile
  //
  if ((a->isVolatile & b->isVolatile) != 0)
    TST,CompilerStrictError( "More than one 'volatile' in declaration" ); 
  else
    a->isVolatile |= b->isVolatile;

  // restrict
  //
  if ((a->isRestrict & b->isRestrict) != 0)
    CompilerStrictError( "More than one 'restrict' in declaration" ); 
  else
    a->isRestrict |= b->isRestrict;

  // Combine qualifier attributes
  //
  a->extQualAttr = ExtQualAttr_Add( a->extQualAttr, b->extQualAttr );

  if (a->isInline & b->isInline)
    CompilerError( "More than one 'inline' in declaration" );
  a->isInline |= b->isInline;

  // Storage class
  //
  if (a->sclass != 0 && b->sclass != 0)
    TST,CompilerError( "More than one storage class in declaration" );
  else
    a->sclass |= b->sclass;

  // Extended declspec
  //
  a->extDeclSpec = ExtDeclSpec_Add( a->extDeclSpec, b->extDeclSpec );

  return a;
};


//--------------------------------------------------------------------------
// Name         CombineQuals 
//
// Combine qualifiers from _a_ and _b_ into _a_, generating apropriate
// error messages.
//--------------------------------------------------------------------------
static void CombineQuals ( TQualNode * a, const TQualNode * b )
{
  if (a->isConst & b->isConst)
    CompilerStrictError( "More than one 'const' in type" );
  a->isConst |= b->isConst;

  if (a->isVolatile & b->isVolatile)
    CompilerStrictError( "More than one 'volatile' in type" );
  a->isVolatile |= b->isVolatile;

  if (a->isRestrict & b->isRestrict)
    CompilerStrictError( "More than one 'restrict' in type" );
  a->isRestrict |= b->isRestrict;

  a->extQualAttr = ExtQualAttr_Add( a->extQualAttr, b->extQualAttr );
};


//--------------------------------------------------------------------------
// Name         DSpecs_GenQual
//
// Constructs a qual node from the passed declspec.
// If the DSPEC is NULL, generates an empty qual node
//
//--------------------------------------------------------------------------
TQualNode * DSpecs_GenQual ( TDeclSpecs * ds )
{
  TQualNode * qual = New_TQualNode( NULL );

  if (ds != NULL)
  {
    ASSERT( ds->baseType == DT_NONE );

    qual->isConst    = ds->isConst;
    qual->isVolatile = ds->isVolatile;
    qual->isRestrict = ds->isRestrict;

    qual->extQualAttr  = ds->extQualAttr;
  }

  return qual;
};


//--------------------------------------------------------------------------
// Name         DSpecs_Fix 
//
// Reports errors and normalizes the structure, so that it always contains
// a valid type. 
// Sets the default signedness.
/*
  List of valid types:

  void
  bool
  char,          unsigned char,          signed char
  short int,     unsigned short int,     signed short int
  int,           unsigned int,           signed int
  long int,      unsigned long int,      signed long int
  long long int, unsigned long long int, signed long long int
  ext_int,       unsigned ext_int,       signed ext_int

  float,         imaginary float,        complex float
  double,        imaginary double,       complex double
  long double,   imaginary long double,  complex long double
  ext_float,     imaginary ext_float,    complex ext_float

  typedef
  enum
  struct
  union
*/
//--------------------------------------------------------------------------
static TDeclSpecs * DSpecs_Fix ( TDeclSpecs * ds )
{
  ASSERT( ds != NULL );
  if (ds == NULL)
    ds = New_DSpecs( C_INT );

  // implicit int
  //
  if (ds->baseType == DT_NONE)
  {
    if (ds->isLong == 0 && ds->isSigned == 0)
      TST,CompilerStrictError( "C99 forbids declarations without a specifier" );

    ds->baseType = DT_INT;
  }

  // Allow complex only on FP types
  //
  if (ds->isComplex != 0 && !(ds->baseType >= DT_FLOAT && ds->baseType <= DT_EXT_FLOAT))
  {
    TST,CompilerError( "'_Complex'/'_Imaginary' only apply to floating point types" );
    ds->isComplex = 0;
  }

  // Allow sign only on integer types
  //
  if (ds->isSigned != 0 && !(ds->baseType >= DT_CHAR && ds->baseType <= DT_EXT_INT))
  {
    TST,CompilerError( "'signed'/'unsigned' only apply to 'int' and 'char'" );
    ds->isSigned = 0;
  }

  // Allow 'short' only on int
  //
  if (ds->isLong == 1 && !(ds->baseType == DT_INT))
  {
    TST,CompilerError( "'short' only applies to 'int'" );
    ds->isLong = 0;
  }

  // Allow 'long' only on int and double
  //
  if (ds->isLong == 2 && !(ds->baseType == DT_INT || ds->baseType == DT_DOUBLE))
  {
    TST,CompilerError( "'long' only applies to 'int' and 'double'" );
    ds->isLong = 0;
  }

  // Allow 'long long' only on int
  //
  if (ds->isLong == 3 && ds->baseType != DT_INT)
  {
    TST,CompilerError( "'long long' only applies to 'int'" );
    ds->isLong = 0;
  }


  // Set the default sign on char
  //
  if (ds->baseType == DT_CHAR)
  {
    if (ds->isSigned == 0)
    {
      // false becomes 1=unsigned, true becomes 2=signed
      ds->isSigned = (uchar)bCharIsSigned + 1; 
    }
  }
  else
  // Set the default sign on integer types
  //
  if (ds->baseType >= DT_INT && ds->baseType <= DT_EXT_INT)
  {
    if (ds->isSigned == 0)
      ds->isSigned = 2;
  }

  return ds;
};
//--------------------------------------------------------------------------
// Name         DSpecs_GenType 
//
//--------------------------------------------------------------------------
TDeclSpecs * DSpecs_GenType ( TDeclSpecs * ds )
{
  const TTypeNode * tnode = NULL;
  TQualNode * qual = NULL;

  // Validate the type
  //
  ds = DSpecs_Fix( ds );

  // DSpecs_GenType should not have been called on this structure
  //
  ASSERT( ds->type == NULL );

  // Allocate the qual node
  //
  qual = New_TQualNode( NULL );

  qual->isConst    = ds->isConst;
  qual->isVolatile = ds->isVolatile;
  qual->isRestrict = ds->isRestrict;

  qual->extQualAttr  = ds->extQualAttr;

  // Generate the type
  //
  switch (ds->baseType)
  {
    IntegerType_t it;

  // void
  //
  case DT_VOID: 
    tnode = CfeRoots.VoidType->tnode;
    break;

  // Integer
  //
  case DT_BOOL: 
    it = it_SBool;
    goto intType;

  case DT_CHAR: 
    it = it_SChar;
    goto intType;

  case DT_INT:
    {
      static IntegerType_t tab[4] = { it_Int, it_Short, it_Long, it_LongLong };
      it = tab[ds->isLong];
    }
    goto intType;

  case DT_EXT_INT: 
    it = ds->u.extInt;

intType:
    if (ds->isSigned != 2) // if not signed
      ++it; // increment the value which turns it from signed to unsigned

    tnode = CfeRoots.IntegerTypes[it]->tnode;
    break;

  // Floating
  //
  case DT_FLOAT:      
    tnode = CfeRoots.FloatingTypes[ds->isComplex + TYPE_REAL][rt_Float]->tnode;
    break;
  case DT_DOUBLE:     
    tnode = CfeRoots.FloatingTypes[ds->isComplex + TYPE_REAL][ ds->isLong == 2 ? rt_LongDouble : rt_Double ]->tnode;
    break;
  case DT_EXT_FLOAT:  
    tnode = CfeRoots.FloatingTypes[ds->isComplex + TYPE_REAL][ ds->u.extFloat ]->tnode;
    break;

  // typedef
  case DT_TYPENAME:
    // Comine the type qualifiers from the typedef and the declaration
    CombineQuals( qual, ds->u.typeName );
    tnode = ds->u.typeName->tnode;
    break;

  // enum, struct, union
  case DT_TAG:
    tnode = ds->u.tag;
    break;

  default:
    CHECK_ASSUME( 0 );
  }

  qual->tnode = tnode;

  ds->type = qual;

  return ds;
};

