#include "port\global.h"
#include "comp.h"
#include "parsetyp.h"
#include "decl.h"

// We can't have too many TParseType structures living at the same time,
// because we use right recursion and combine the structures as soon
// as possible. Even a smaller number (4?) seems reasonable.
//
IMPLEMENT_FIXED_ALLOCATOR( ParseTypeHeap, TParseType, 16 );

// We can't have too many TDeclData living at the same time.
// They can be gathered only in nested declarations (structures,
// functions).
// 
IMPLEMENT_FIXED_ALLOCATOR( DeclDataHeap,  TDeclData, 256 );

// A table containing static TParseType structures for various
// INFO_TOKEN-s. This lets us construct and return them very quickly.
//
static TParseType InfoToks[] =
{
// TOK_NONE
  { 1 },

//  TOK_SHORT,
  { 1, BT_NONE,   0, 0, 1, 0 },
  
//  TOK_LONG,    
  { 1, BT_NONE,   0, 0, 0, 1 },

//  TOK_SIGNED,  
  { 1, BT_NONE,   1, 0, 0, 0 },

//  TOK_UNSIGNED,
  { 1, BT_NONE,   0, 1, 0, 0 },

//  TOK_VOID,    
  { 1, BT_VOID,   0, 0, 0, 0 },

//  TOK_BOOL,    
  { 1, BT_BOOL,   0, 0, 0, 0 },

//  TOK_CHAR,    
  { 1, BT_CHAR,   0, 0, 0, 0 },

//  TOK_INT,     
  { 1, BT_INT,    0, 0, 0, 0 },

//  TOK_FLOAT,   
  { 1, BT_FLOAT,  0, 0, 0, 0 },

//  TOK_DOUBLE,  
  { 1, BT_DOUBLE, 0, 0, 0, 0 },

//  TOK_CONST,    
  { 1, BT_NONE,   0, 0, 0, 0,  1, 0, 0 },

//  TOK_VOLATILE, 
  { 1, BT_NONE,   0, 0, 0, 0,  0, 1, 0 },

//  TOK_RESTRICT, 
  { 1, BT_NONE,   0, 0, 0, 0,  0, 0, 1 },

//  TOK_EXTERN,   
  { 1, BT_NONE,   0, 0, 0, 0,  0, 0, 0,  TOK_EXTERN },

//  TOK_AUTO,     
  { 1, BT_NONE,   0, 0, 0, 0,  0, 0, 0,  TOK_AUTO },

//  TOK_STATIC,   
  { 1, BT_NONE,   0, 0, 0, 0,  0, 0, 0,  TOK_STATIC },

//  TOK_REGISTER, 
  { 1, BT_NONE,   0, 0, 0, 0,  0, 0, 0,  TOK_REGISTER },

//  TOK_TYPEDEF,  
  { 1, BT_NONE,   0, 0, 0, 0,  0, 0, 0,  TOK_TYPEDEF },
};

const char * TokName[] =
{
// TOK_NONE
  "",

//  TOK_SHORT,
  "short",
  
//  TOK_LONG,
  "long",

//  TOK_SIGNED,
  "signed", 

//  TOK_UNSIGNED,
  "unsigned",

//  TOK_VOID,
  "void",

//  TOK_BOOL,
  "bool",

//  TOK_CHAR,
  "char",

//  TOK_INT,
  "int",

//  TOK_FLOAT,
  "float",

//  TOK_DOUBLE,
  "double",

//  TOK_CONST,
  "const",

//  TOK_VOLATILE,
  "volatile",

//  TOK_RESTRICT,
  "__restrict",

//  TOK_EXTERN,
  "extern",

//  TOK_AUTO,
  "auto",

//  TOK_STATIC,
  "static",

//  TOK_REGISTER,
  "register",

//  TOK_TYPEDEF,
  "typedef",
};

// Translate a storage class info tok into a TStorageClass struct
static const TStorageClass ITok2SClass[TOK_TYPEDEF - TOK_EXTERN + 1] =
{
//  TOK_EXTERN,
  { SC_FIXED, OC_EXTERN },

//  TOK_AUTO,
  { SC_AUTO, OC_NONE },

//  TOK_STATIC,
  { SC_FIXED, OC_PRIVATE },

//  TOK_REGISTER,
  { SC_REGISTER, OC_NONE },

//  TOK_TYPEDEF,
  { SC_TYPEDEF, OC_NONE },
};

//--------------------------------------------------------------------------
// Name         ParseType_Init
//
//
//--------------------------------------------------------------------------
void ParseType_Init ( void )
{
  ParseTypeHeap_Init();
  DeclDataHeap_Init();
};

//--------------------------------------------------------------------------
// Name         ParseType_Done
//
//
//--------------------------------------------------------------------------
void ParseType_Done ( void )
{
  DeclDataHeap_Done();
  ParseTypeHeap_Done();
};

//--------------------------------------------------------------------------
// Name         FreeParseType
//
// Frees the passed parse type, if it is dynamic.
//--------------------------------------------------------------------------
void FreeParseType ( TParseType * t )
{
  if (t != NULL && t->staticData == FALSE)
    ParseTypeHeap_Free( t );
};

//--------------------------------------------------------------------------
// Name         NewTokenParseType
//
//
//--------------------------------------------------------------------------
TParseType * NewTokenParseType ( INFO_TOKEN itok )
{
  ASSERT( itok < _countof( InfoToks ) );

  return &InfoToks[itok];
};

//--------------------------------------------------------------------------
// Name         NewTypenameParseType
//
//
//--------------------------------------------------------------------------
TParseType * NewTypenameParseType ( TSymbol * typename )
{
  ASSERT( typename != NULL ); // to see if it happens

  if (typename == NULL)
    return NULL;
  else
  {
    TParseType * parstyp = ParseTypeHeap_CAlloc();
    parstyp->typename = typename;
    return parstyp;
  }
};

//--------------------------------------------------------------------------
// Name         NewStructParseType
//
//
//--------------------------------------------------------------------------
TParseType * NewStructParseType (
    TStructDef * structDef
  )
{
  if (structDef == NULL)
    return NULL;
  else
  {
    TParseType * parstyp = ParseTypeHeap_CAlloc();
    parstyp->structDef = structDef;
    return parstyp;
  }
};

//--------------------------------------------------------------------------
// Name         NewDeclspecParseType
//
//
//--------------------------------------------------------------------------
TParseType * NewDeclspecParseType ( unsigned declspec )
{
  ASSERT( declspec != 0 );

  if (declspec == 0)
    return NULL;
  else
  {
    TParseType * parstyp = ParseTypeHeap_CAlloc();
    parstyp->declspec = declspec;
    return parstyp;
  }
};

//--------------------------------------------------------------------------
// Name         CombineHelper
//
// Combines two TParseType structs into one, issuing error messages
// as necessary.
//--------------------------------------------------------------------------
static void CombineHelper ( TParseType * a, TParseType * b )
{
  ASSERT( a->staticData == FALSE ); // a is already dynamic

  // Step 1: combine the parts of the two TParseType-s without looking
  // at other parts

  // basic
  if (b->basic != BT_NONE)
  {
    if (a->basic != BT_NONE)
      Error( "'%s %s' is illegal", BtName[a->basic], BtName[b->basic] );
    else
      a->basic = b->basic;
  }

  // isSigned
  if (b->isSigned)
  {
    if (a->isSigned)
      Warning( "'signed' used more than once" );
    else
    if (a->isUnsigned)
      Error( "'unsigned signed' is illegal" );
    else
      a->isSigned = b->isSigned;
  }

  // isUnsigned
  if (b->isUnsigned)
  {
    if (a->isUnsigned)
      Warning( "'unsigned' used more than once" );
    else
    if (a->isSigned)
      Error( "'signed unsigned' is illegal" );
    else
      a->isUnsigned = b->isUnsigned;
  }

  // isShort
  if (b->isShort)
  {
    if (a->isShort)
      Error( "'short short' is invalid" );
    else
    if (a->isLong)
      Error( "'long short' is invalid" );
    else
      a->isShort = b->isShort;
  }

  // isLong
  if (b->isLong)
  {
    if (a->isLong)
      Error( "'long long' is invalid" );
    else
    if (a->isShort)
      Error( "'short long' is invalid" );
    else
      a->isLong = b->isLong;
  }

  // isConst
  if (b->isConst)
  {
    if (a->isConst)
      Warning( "'const' used more than once" );
    else
      a->isConst = b->isConst;
  }

  // isVolatile
  if (b->isVolatile)
  {
    if (a->isVolatile)
      Warning( "'volatile' used more than once" );
    else
      a->isVolatile = b->isVolatile;
  }

  // isRestrict
  if (b->isRestrict)
  {
    if (a->isRestrict)
      Warning( "'__restrict' used more than once" );
    else
      a->isRestrict = b->isRestrict;
  }

  // declspec: combine them
  // TODO: issue warnings for incompatible ones?
  //
  a->declspec |= b->declspec;

  // SCLASS
  if (b->sclass != TOK_NONE)
  {
    if (a->sclass != TOK_NONE)
      Error( "'%s %s' is illegal", TokName[a->sclass], TokName[b->sclass] );
    else
      a->sclass = b->sclass;
  }

  // typename
  if (b->typename != NULL)
  {
    ASSERT( a->typename == NULL );
    // we take grammar precautions against this, but still, let's support it

    if (a->typename != NULL)
      Error( "Using two typedef-s in a declaration is invalid" );
    else
      a->typename = b->typename;
  }

  // structspec
  if (b->structDef != NULL)
  {
    ASSERT( a->structDef == NULL );
    // we take grammar precautions against this, but still, let's support it

    if (a->structDef != NULL)
      Error( "Using two struct-s in a declaration is invalid" );
    else
      a->structDef = b->structDef;
  } 

  // Step 2: Now look at several parts and make sure there aren't any
  // invalid combinations.

  // check for float/void and short/signed modifiers
  if ((a->basic == BT_FLOAT || a->basic == BT_DOUBLE || a->basic == BT_VOID) &&
      (a->isSigned || a->isUnsigned || a->isShort || a->isLong))
  {
    Error( "Sign/size modifiers applied to '%s' are invalid", BtName[a->basic] );
    a->isSigned = FALSE;
    a->isUnsigned = FALSE;
    a->isShort = FALSE;
    a->isLong = FALSE;
  }

  // Now check for combinations that are illegal: e.g. short typename
  if ((a->basic || a->isSigned || a->isUnsigned || a->isShort || a->isLong) &&
      (a->typename || a->structDef))
  {
    Error( "Mixing basic type and struct/typedef is invalid" );
    // fix the type as far as we can
    a->typename = NULL;
    a->structDef = NULL;
  }

  // Now check for "typename struct"
  if (a->typename && a->structDef)
  {
    Error( "Mixing typedef and struct is invalid" );
    a->structDef = NULL;
  }
};

//--------------------------------------------------------------------------
// Name         CombineParseType
//
// if _b_ is NULL, just returns _a_ (and vica versa)
//--------------------------------------------------------------------------
TParseType * CombineParseType ( TParseType * a, TParseType * b )
{
  if (a == NULL)
    return b;
  if (b == NULL)
    return a;

  // if a is static, make it dynamic
  if (a->staticData)
  {
    TParseType * parstyp = ParseTypeHeap_Alloc();
    
    *parstyp = *a;               // Copy the data
    parstyp->staticData = FALSE; // mark it as "dynamic"
    a = parstyp;                 // forget about the static
  }

  // Perform the combining
  CombineHelper( a, b );

  ASSERT( a->staticData == FALSE );
  FreeParseType( b );

  return a;
};

//--------------------------------------------------------------------------
// Name         ParseType
//
// Build a TYPE from a parse_type
//--------------------------------------------------------------------------
TYPE * ParseType ( const TParseType * t )
{
  TTypeNode * tnode;
  TQualNode * q1;      // typedef's qnode
  TQualNode * qnode;

  q1 = NULL;
  // Allocate and initialize the typenode
  //
  if (t->structDef) // a struct/union/enum
  {
    tnode = TypeNode_New( t->structDef->structType );
    tnode->info.structDef = t->structDef;
  }
  else
  if (t->typename) // a typedef
  {
    q1 = t->typename->type;  // save typedef's quals
    tnode = q1->tnode;
  }
  else // a basic type
  {
    tnode = TypeNode_NewBasic( t->basic );  // will default to BT_INT, on BT_NONE

    if (t->isSigned)
      tnode->isSigned = TRUE;
    else
    if (t->isUnsigned)
      tnode->isSigned = FALSE;
    else
    {
      // default case: no sign specified
      /*
        Determine tnode->isSigned: we have to be careful about the
        defaults.
          The default for INT is SIGNED,
          the default for CHAR is in the opt->bSignedChars.
      */
      if (tnode->info.basic == BT_INT)
        tnode->isSigned = TRUE;
      else
      if (tnode->info.basic == BT_CHAR)
        tnode->isSigned = opt->bSignedChars;
      else
        tnode->isSigned = FALSE;
    }

    tnode->isShort  = t->isShort;
    tnode->isLong   = t->isLong;
  }

  // Allocate and initialize the qual node
  //
  qnode = QualNode_New();
  qnode->isConst = t->isConst;
  qnode->isVolatile = t->isVolatile;
  qnode->isRestrict = t->isRestrict;
  CombineQuals( qnode, q1 );
  qnode->tnode = tnode;

  return qnode;
};

//--------------------------------------------------------------------------
// Name         NewDeclData
//
// This function doesn't really do anyhting, but it is a convinient
// wrapper, in case we need to put some initialization here in the future.
//--------------------------------------------------------------------------
TDeclData * NewDeclData ( void )
{
  return DeclDataHeap_CAlloc();
};

//--------------------------------------------------------------------------
// Name         FreeDeclData
//
// Frees the associated qualifier node.
//--------------------------------------------------------------------------
void FreeDeclData ( TDeclData * p )
{
  // free the ident if any
  if (p->ident)
    Symbol_Free( p->ident );

  DeclDataHeap_Free( p );
};

//--------------------------------------------------------------------------
// Name         CombineQualToken
//
// Combines a qualifier specified as an INFO_TOKEN (TOK_CONST|TOK_VOLATILE)
// with an existing TQualNode.
//--------------------------------------------------------------------------
void  CombineQualToken ( TQualNode * qnode, INFO_TOKEN itok )
{
  ASSERT( itok == TOK_CONST || itok == TOK_VOLATILE || itok == TOK_RESTRICT );

  if (itok == TOK_CONST)
  {
    if (qnode->isConst)
      Warning( "'const' used more than once" );
    qnode->isConst = true;
  }
  else
  if (itok == TOK_VOLATILE)
  {
    if (qnode->isVolatile)
      Warning( "'volatile' used more than once" );
    qnode->isVolatile = true;
  }
  else
  if (itok == TOK_RESTRICT)
  {
    if (qnode->isRestrict)
      Warning( "'__restrict' used more than once" );
    qnode->isRestrict = true;
  }
};

//--------------------------------------------------------------------------
// Name         AttachToDecl
//
// Attach a new qnode->tnode pair to the chain in the declaration
//--------------------------------------------------------------------------
TDeclData * AttachToDecl (
    TDeclData * decl,
    TYPE * typ
  )
{
  if (decl->type == NULL) // if this is the first
    decl->type = typ;
  else
    decl->etype->of = typ;

  decl->etype = typ->tnode;

  // transfer the attributes
  //
  if (decl->attrs)
  {
    typ->attr |= decl->attrs;
    decl->attrs = 0;
  }

  return decl;
};

//--------------------------------------------------------------------------
// Name         ParsedDecl
//
// If there is no ident assigned to the declaration, creates an anonymous
// one.
// 
// Frees _decl_.
//--------------------------------------------------------------------------
void ParsedDecl ( 
    const TParseType * typeStart,
    TDeclData  * decl
  )
{
  TYPE * typ;
  TStorageClass * sClass;

  // if there is no symbol assigned to the declaration, create an
  // anonymous one
  //
  if (decl->ident == NULL)
    decl->ident = Symbol_New( NULL );

  if (typeStart == NULL) // for empty type, assume int
    typeStart = &InfoToks[TOK_INT];

  // Ok, first we build the type chain by combining the specifiers
  // in _typeStart_ with the declarators in _decl_
  //

  // Copy the typeStart
  //
  typ = ParseType( typeStart );

  // Combine the attributes that we gathered in the declaration
  //
  typ->attr = CombineQualAttr( typ->attr, decl->attrs ); // combine the preserved attrs

  // Build the complete type chain
  //
  AttachToDecl( decl, typ );

  // Create the storage class and initialize it
  //
  sClass = StorageClass_New();

  if (typeStart->sclass != TOK_NONE) // is there explicit storage class specified?
  {
    // Translate the token into TStorageClass
    *sClass = ITok2SClass[typeStart->sclass - TOK_EXTERN];
  }
  else
  {
    // no, set the default for the current scope
    *sClass = DefaultScopeStorageClass[CurScope->scopeType];
  }

  sClass->declSpec = typeStart->declspec; // copy the declspec

  // Perform the declaration
  //
  Decl( sClass, decl->ident, decl->type );

  // Free the decl data which we don't need any more
  //
  decl->ident = NULL;  // FreeDeclData() must not free the ident
  FreeDeclData( decl );
};

//--------------------------------------------------------------------------
// Name         CombineQuals
//
// Combines the qualifiers of dest and src into dest.
// if src == NULL, does nothing;
// Generates error messages, if the qualifiers can't be combined (only
// for incompatible attributes), and retains dest's attributes.
//--------------------------------------------------------------------------
void CombineQuals ( TQualNode * dest, const TQualNode * src )
{
  if (src == NULL)
    return; // nothing to combine with

  dest->isConst    |= src->isConst;
  dest->isVolatile |= src->isVolatile;
  dest->isRestrict |= src->isRestrict;

  dest->attr       = CombineQualAttr( dest->attr, src->attr );
};

//////////////////////////

//--------- Functions which add nodes to the current declaration --------
//
// Some of them accept _decl_ == NULL. This is in case of a type_cast.
// In that case a new decldata structure is allocated.

TDeclData * DeclAddIdent ( STRING str )
{
  TDeclData * decl;

  decl = NewDeclData();
  decl->ident = Symbol_New( str );

  return decl;
};

TDeclData * DeclAddAttribute ( unsigned attr, TDeclData * decl )
{
  decl->attrs |= attr;
  return decl;
};

TDeclData * DeclAddPointer ( TQualNode * qual, TDeclData * decl )
{
  qual->tnode = TypeNode_New( NODE_POINTER );
  return AttachToDecl( decl ? decl : NewDeclData(), qual );
};

TDeclData * DeclAddArray ( TDeclData * decl, TExprAst * nelem )
{
  TQualNode * qnode;

  qnode = QualNode_New(); // an empty qual node
  qnode->tnode = TypeNode_New( NODE_ARRAY );

  if (nelem != NULL)
  {
    // TODO: support sized arrays
    Error( "Unsupported" );
    qnode->tnode->info.arrayLen = Int1; // just for debugging
  }

  return AttachToDecl( decl ? decl : NewDeclData(), qnode );
};

TDeclData * DeclAddFunction ( TDeclData * decl )
{
  TQualNode * qnode;

  qnode = QualNode_New(); // an empty qual node
  qnode->tnode = TypeNode_New( NODE_FUNCTION );
  qnode->tnode->isImplicit = true; // implicit by default

// The new scope must have already been created
//  NewActiveScope( FUNC_PARAM_SCOPE );
//

  ASSERT( CurScope->scopeType == FUNC_PARAM_SCOPE );

  qnode->tnode->info.funcParams = CurScope;  // attach the parameters
  CurScope->info.funcNode = qnode;

  return AttachToDecl( decl ? decl : NewDeclData(), qnode );
};


