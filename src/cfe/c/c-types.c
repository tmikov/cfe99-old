#include "precomp.h"
#include "cfe_priv.h"
#include "c-types.h"


//== TIdent ===================================================================

//--------------------------------------------------------------------------
// Name         TIdent_GcMark 
//
// 
//--------------------------------------------------------------------------
void TIdent_GcMark ( const TIdent * id )
{
  struct TIdent_loc
  {
    TListEntry  link;              // link in the namespace chain
    TIdent      * pPrev, * pNext;  // labels in the scope
    STRING      strName;
    TScope      * scope;
  };
  MAKE_SURE( sizeof(struct TIdent) == sizeof(struct TIdent_loc) );

  GC_MARK( id->pPrev );
  GC_MARK( id->pNext );
  GC_MARK( id->strName );
  GC_MARK( id->scope );
};

//--------------------------------------------------------------------------
// Name         STRING_Name 
//
// 
//--------------------------------------------------------------------------
const char * STRING_Name ( STRING str )
{
  return str != NULL ? C_STR(str) : "<unnamed>";
};

//== TQualNode ================================================================

struct TQualNode_loc
{
  bool isConst    : 1;
  bool isVolatile : 1;
  bool isRestrict : 1;

  ExtQualAttr_t extQualAttr; // Each attr is a bit

  TTypeNode * tnode;       // the type that the qualifiers apply to
};

//--------------------------------------------------------------------------
// Name         TQualNode_GcMark 
//
// 
//--------------------------------------------------------------------------
void TQualNode_GcMark ( const TQualNode * qual )
{
  MAKE_SURE( sizeof(struct TQualNode) == sizeof(struct TQualNode_loc) );

  GC_MARK( qual->tnode );
};

//--------------------------------------------------------------------------
// Name         New_TQualNode 
//
// 
//--------------------------------------------------------------------------
TQualNode * New_TQualNode ( const TTypeNode * tnode )
{
  TQualNode * res = GC_NEW( TQualNode );
  res->tnode = tnode;
  return res;
};

//--------------------------------------------------------------------------
// Name         QualNode_Clone 
//
// Closes a qual node, but sets a new tnode
//--------------------------------------------------------------------------
TQualNode * QualNode_Clone ( const TQualNode * qual, const TTypeNode * tnode )
{
  TQualNode * res = GC_NEW( TQualNode );

  MAKE_SURE( sizeof(struct TQualNode) == sizeof(struct TQualNode_loc) );

  *res = *qual;
  res->tnode = tnode;

  return res;
};

//== TTypeNode ================================================================

const char * const TypeKind_Name[] = 
{
  "none",

  "void",
  "bool",
  "integer",
  "enum",
  "real",
  "imaginary",
  "complex",

  "error",

  "struct",
  "union",
  "array",
  "pointer",
  "function",
};

struct TTypeNode_loc
{
  TIdent      id;

  TypeKind_t  typeKind  : 8;

  bool        staticArraySize : 1;  
      // only if typeKind = TYPE_ARRAY or TYPE_POINTER. Signifies
      // the presence of "static" within the array brackets,
      // e.g. int x[static 10]

  bool        bDefined        : 1;
      // for TYPE_STRUCT/UNION or TYPE_ENUM means that it has already been defined
      // or we are in the process of defining it.

  const TQualNode  * of; // link to the next node for array/pointer/function

  union
  {
    const TIntegerTypeDesc * pIntDesc; // for TYPE_BOOL, TYPE_INTEGER, TYPE_ENUM
    RealType_t realType;               // for TYPE_REAL, TYPE_IMAGINERY, TYPE_COMPLEX

    //-------------------------
    // typeKind == TYPE_ARRAY, TYPE_POINTER
    //
    const TAst * arrayLen; // NULL for unknown, non-const for variable

    //-------------------------
    // typeKind == TYPE_STRUCT/TYPE_UNION/TYPE_FUNCTION
    //
    TScope * memberScope; // struct members or function parameters
  } u;
};

//--------------------------------------------------------------------------
// Name         TTypeNode_GcMark 
//
// 
//--------------------------------------------------------------------------
void TTypeNode_GcMark ( const TTypeNode * node )
{
  MAKE_SURE(sizeof(struct TTypeNode) == sizeof(struct TTypeNode_loc) );

  TIdent_GcMark( &node->id );
  GC_MARK( node->of );

  switch (node->typeKind)
  {
  case TYPE_ARRAY:
  case TYPE_POINTER:
    GC_MARK( node->u.arrayLen );
    break;

  case TYPE_STRUCT:
  case TYPE_UNION:
  case TYPE_FUNCTION:
    GC_MARK( node->u.memberScope );
    break;

#ifdef _DEBUG
  case TYPE_VOID:
  case TYPE_BOOL:
  case TYPE_INTEGER:
  case TYPE_ENUM:
  case TYPE_REAL:
  case TYPE_IMAGINARY:
  case TYPE_COMPLEX:
  case TYPE_NONE:
  case TYPE_ERROR:
    break;

  default:
    ASSERT( 0 );
#endif
  }
};

//--------------------------------------------------------------------------
// Name         New_TTypeNode 
//
// 
//--------------------------------------------------------------------------
TTypeNode * New_TTypeNode ( TypeKind_t typeKind )
{
  TTypeNode * res = GC_NEW( TTypeNode );
  res->typeKind = typeKind;
  return res;
};

//--------------------------------------------------------------------------
// Name         New_Tag 
//
// 
//--------------------------------------------------------------------------
TTypeNode * New_Tag ( TypeKind_t typeKind, STRING name )
{ 
  TTypeNode * res = New_TTypeNode( typeKind );
  res->id.strName = name;
  return res;
};

//--------------------------------------------------------------------------
// Name         TypeNode_Clone 
//
// 
//--------------------------------------------------------------------------
TTypeNode * TypeNode_Clone ( const TTypeNode * tnode, const TQualNode * of )
{
  TTypeNode * res = GC_NEW( TTypeNode );

  MAKE_SURE(sizeof(struct TTypeNode) == sizeof(struct TTypeNode_loc) );

  *res = *tnode;
  res->of = of;

  return res;
};

//== TEnumDesc ================================================================

//--------------------------------------------------------------------------
// Name         TEnumDesc_GcMark 
//
// 
//--------------------------------------------------------------------------
void TEnumDesc_GcMark ( const TEnumDesc * edesc )
{
  struct TEnumDesc_loc
  {
    TTypeNode * tag;
    TQualNode * type;
    TARGET_IMAX_T  curVal;
  };
  MAKE_SURE(sizeof(struct TEnumDesc) == sizeof(struct TEnumDesc_loc) );

  GC_MARK( edesc->tag );
  GC_MARK( edesc->type );
};

//--------------------------------------------------------------------------
// Name         New_TEnumDesc 
//
// 
//--------------------------------------------------------------------------
TEnumDesc * New_TEnumDesc ( const TTypeNode * tag )
{
  TEnumDesc * edesc = GC_NEW( TEnumDesc );
  edesc->tag = tag;

  // Allocate the type
  edesc->type = New_TQualNode( tag );
  edesc->type->isConst = true;

  return edesc;
};

//== Type functions ===========================================================

//--------------------------------------------------------------------------
// Name         Type_IsComplete 
//
// 
//--------------------------------------------------------------------------
bool Type_IsComplete ( const TQualNode * qual )
{
  const TTypeNode * tnode;

  for(;;)
  {
    ASSERT( qual != NULL );
    tnode = qual->tnode;

    switch (tnode->typeKind)
    {
    case TYPE_ERROR:
      return true; // the error type is always complete.

    case TYPE_VOID:
      return false;
    case TYPE_BOOL:
    case TYPE_INTEGER:
    case TYPE_ENUM:      TODO1( "possibly check for enum if we allowed forward enum declarations" );
    case TYPE_REAL:
    case TYPE_IMAGINARY:
    case TYPE_COMPLEX:
      return true;

    case TYPE_STRUCT:
    case TYPE_UNION:
      // complete only if the members have been defined
      return tnode->u.memberScope != NULL;

    case TYPE_ARRAY:
      // [] is incomplete
      if (tnode->u.arrayLen == NULL)
        return false;

      // the array is complete if the element type is complete
      qual = tnode->of;
      break;

    case TYPE_POINTER:
      return true;      // pointer type is always complete

    case TYPE_FUNCTION: 
      return false;     // function type is never complete since it has no size

    default:
      CHECK_ASSUME( 0 );
      return false;
    }
  }
};

//--------------------------------------------------------------------------
// Name         TYPE_ValidateOf 
//
// Validates a type being derived from.
//
// Performed validations:
//   - [static] only allowed in the outermost array derivation
//   - [qualifiers] only allowed in the outermost array derivation
//--------------------------------------------------------------------------
static const TQualNode * TYPE_ValidateOf ( const TQualNode * of )
{
  TTypeNode * rnode = NULL;
  const TTypeNode * tnode = of->tnode;

  if (tnode->typeKind == TYPE_ARRAY)
  {
/*
6.7.5.2
1 In addition to optional type qualifiers and the keyword static, the [ and ] may delimit
an expression or *. [...] The optional type qualifiers and the keyword static shall appear 
only in a declaration of a function parameter with an array type, and then only in the outermost
array type derivation.
*/
    if (tnode->staticArraySize)
    {
      CompilerError( "[static] only allowed in outermost array type derivation" );

      // clear the [static] flag
      CLONE_RNODE( rnode, tnode, tnode->of )->staticArraySize = false;
    }

    if (of->isConst || of->isRestrict || of->isVolatile || of->extQualAttr)
    {
      CompilerError( "qualifiers only allowed in outermost array type derivation" );

      // Ignore all qualifiers by creating a blank qual node
      of = New_TQualNode( tnode );
    }
  }

  // If we changed _tnode_, we must create a copy of _of_ using the new _tnode_
  if (of->tnode != tnode)
    of = QualNode_Clone( of, tnode );

  return of;
};

//--------------------------------------------------------------------------
// Name         Type_MakePointer 
//
// Returns  "qual pointer to _to_"
//--------------------------------------------------------------------------
const TQualNode * Type_MakePointer ( const TQualNode * to, TQualNode * qual )
{
  TTypeNode * tnode = New_TTypeNode( TYPE_POINTER );

  // Validate the element type
  //
  if (to != NULL)
    to = TYPE_ValidateOf( to );

  tnode->of = to;

  if (qual == NULL)
    qual = New_TQualNode( NULL );

  ASSERT( qual->tnode == NULL ); // must not be attached

  qual->tnode = tnode;
  return qual;
};

//--------------------------------------------------------------------------
// Name         TYPE_ValidateArrayElement
//
// Validate the type to remove:
//   - TYPE_ValidateOf()
//   - array of functions
//   - array of void
//   - array of incomplete type
//--------------------------------------------------------------------------
static const TQualNode * TYPE_ValidateArrayElement ( const TQualNode * of )
{
/*
6.7.5.2-1 [...] The element type shall not be an incomplete or function
type. [...]
*/
  ASSERT( of != NULL );

  of = TYPE_ValidateOf( of );

  if (of->tnode->typeKind == TYPE_VOID)
  {
    CompilerError( "Array of void is invalid" );

    // array of void is invalid in all contexts
    of = CfeRoots.ErrorType;
  }
  else
  if (of->tnode->typeKind == TYPE_FUNCTION)
  {
    CompilerError( "Array of functions is invalid" );

    // array of function is invalid in all contexts
    of = CfeRoots.ErrorType;
  }
  else 
  if (!Type_IsComplete( of ))
  {
    // Theoretically this check catches the above errors as well, but we prefer to 
    // have more precise error messages.
    CompilerError( "Array of incomplete type is not allowed" );
    of = CfeRoots.ErrorType;
  }

  return of;
};

//--------------------------------------------------------------------------
// Name         Type_MakeArray 
//
// returns "qual array[bStaticSize len] of _of_"
//
// Performed validations:
//   - TYPE_ValidateArrayElement()
//   - [*] can be used only in parameter scope
//   - [static] can be used only in parameter scope
//   - array size must be integer type
//   - constant array size must be positive
//   - variable length array allowed only in param and block scope
//--------------------------------------------------------------------------
const TQualNode * Type_MakeArray ( 
    const TQualNode * of, 
    TQualNode       * qual,
    const TAst      * arrayLen, 
    bool            bStaticSize
  )
{
  TTypeNode * tnode;

  // Validate the element type
  //
  if (of != NULL)
    of = TYPE_ValidateArrayElement( of );

/*
6.7.5.2
1 In addition to optional type qualifiers and the keyword static, the [ and ] may delimit
an expression or *. If they delimit an expression (which specifies the size of an array), the
expression shall have an integer type. If the expression is a constant expression, it shall
have a value greater than zero. [...]
*/
  // Check the size
  //
  if (arrayLen == NULL)
  {
    // no size specified  "[]"
  }
  else
  if (arrayLen->code == ast_error)
  {
    // there was an error in the size expression
    bStaticSize = false; // ignore [static] which may have been specified
  }
  else
  if (arrayLen->code == ast_unspecifiedVariableArraySize)
  {
    // "[*]"
    
    // 6.7.5.2-4 If the size is * instead of being an expression, the array 
    // type is a variable length array type of unspecified size, which can 
    // only be used in declarations with function prototype scope;

    if (CfeRoots.CurScope->disposition != SCOPE_PARAMS)
    {
      CompilerError( "[*] can only be used in function prototype" );

      arrayLen = Ast_error( &arrayLen->loc );
    }
  }
  else
  {
    // an expression

    // Check [static] 
    if (bStaticSize && CfeRoots.CurScope->disposition != SCOPE_PARAMS) 
    {
      CompilerError( "[static] only allowed in parameter declaration" );

      bStaticSize = false;
    }


    if (!Type_IsInteger( arrayLen->exprType ))
    {
      CompilerError( "Array size must be integer" );

      // Non-integer array size is an error in all contexts
      arrayLen = Ast_error( &arrayLen->loc );
    }
    else
    if (IsConstantExpression( arrayLen )) // Constant expression must be greater than 0
    {
      bool  bPositive;

      // We already know it is integer. Check for > 0
      //
      if (arrayLen->exprType->tnode->u.pIntDesc->isSigned)
        bPositive = TARGET_GT_SIGNED( &arrayLen->u.intConst, &Const0 );
      else
        bPositive = TARGET_GT_UNSIGNED( &arrayLen->u.intConst, &Const0 );

      if (!bPositive)
      {
        CompilerError( "Array size must be positive" );

        arrayLen = Ast_error( &arrayLen->loc );
      }
    }
/* This looks nice, but in practice we can't get to the storage class!

    else
    if (CfeRoots.CurScope->disposition != SCOPE_PARAMS &&
        CfeRoots.CurScope->disposition != SCOPE_BLOCK ||
        sclass == SC_EXTERN || sclass == SC_STATIC) 
    {
      CompilerError( "Variable length array is not allowed here" );

      arrayLen = Ast_error( &arrayLen->loc );
    }
*/
  }

  // Initialize the node
  //
  tnode = New_TTypeNode( TYPE_ARRAY );

  tnode->staticArraySize = bStaticSize;
  tnode->of = of;
  tnode->u.arrayLen = arrayLen;

  if (qual == NULL)
    qual = New_TQualNode( NULL );

  ASSERT( qual->tnode == NULL ); // must not be attached

  qual->tnode = tnode;
  return qual;
};

//--------------------------------------------------------------------------
// Name         TYPE_ValidateFunctionResult
//
// Performed validations:
//  - TYPE_ValidateOf()
//  - no function returning function
//  - no function returning array
//--------------------------------------------------------------------------
static const TQualNode * TYPE_ValidateFunctionResult ( const TQualNode * of )
{
  ASSERT( of != NULL );

  of = TYPE_ValidateOf( of );

  if (of->tnode->typeKind == TYPE_FUNCTION)
  {
    CompilerError( "Function returning a function is invalid" );

    // Function returning function is invalid in all contexts
    of = CfeRoots.ErrorType;
  }
  else
  if (of->tnode->typeKind == TYPE_ARRAY)
  {
    CompilerError( "Function returning an array is invalid" );

    // Function returning array is invalid in all contexts
    of = CfeRoots.ErrorType;
  }

  return of;
};

//--------------------------------------------------------------------------
// Name         Type_MakeFunction 
//
//
// Performed validations:
//   - TYPE_ValidateFunctionResult()
//   - Converts empty parameter scopes to NULL ident list scopes
//   - no initialized parameters
//   - "...' must be the last parameter and must not be the first  
//   - void is not an allowed parameter type, expect in func(void)
//--------------------------------------------------------------------------
const TQualNode * Type_MakeFunction ( const TQualNode * of, TScope * params )
{
  TTypeNode * tnode;

  // Validate the result type
  //
  if (of != NULL)
    of = TYPE_ValidateFunctionResult( of );

  // Check the parameters
  //
  if (params != NULL && params->disposition == SCOPE_PARAMS)
  {
    // An empty parameter scope (that doesn't even contain void) is in 
    // fact an empty ident list scope.
    // It can happen only in error recovery, but we have to support it 
    // anyway.
    //
    if (params->pFirstSymbol == NULL)
    {
      // Detach the scope, implying that the function has an empty ident-list
      // scope. This modification is valid in all contexts.
      params = NULL;
    }
    else
    {
      TSymbol * sym = params->pFirstSymbol;

      // check for the special case of (void)
      if (sym->id.pNext == NULL &&     // the only symbol
          sym->id.strName == NULL &&   // unnamed
          Type_IsVoid(sym->type))      // of void type
      {
        // function without parameters
      }
      else
      {
        // Validate the parameters
        //
        for ( ; sym != NULL; sym = (TSymbol *)sym->id.pNext )
        {
          if (sym->initValue != NULL)
          {
            CompilerErrorLoc( &sym->declLoc, "'%s': parameter cannot be initialized", Symbol_Name(sym) );

            // initialized parameter is an error in all contexts
            sym->initValue = NULL;
          }

          // Handle "..."
          if (sym->id.strName == CfeRoots.EllipsisName)
          {
            // Ellipsis cannot be the first parameter and it always must
            // be the last one
            if (sym->id.pPrev == NULL)
            {
              CompilerErrorLoc( &sym->declLoc, "'...' must not be the first parameter" );

              // func (...) is an error in all contexts
              Symbol_ChangeName( sym, CfeRoots.ErrorName );
              sym->type = CfeRoots.ErrorType;
            }
            else
            if (sym->id.pNext != NULL)
            {
              CompilerErrorLoc( &sym->declLoc, "'...' must always be the last parameter" );

              // func ( ..., int i ) is an error in all contexts
              Symbol_ChangeName( sym, CfeRoots.ErrorName );
              sym->type = CfeRoots.ErrorType;
            }
          }
          else
          // disallow _void_ parameter type
          if (Type_IsVoid( sym->type ))
          {
            CompilerErrorLoc( &sym->declLoc, "'%s': parameter cannot have void type", Symbol_Name(sym) );
            sym->type = CfeRoots.ErrorType;
          }
        }
      }
    }
  }

  // Initialize the type node
  //
  tnode = New_TTypeNode( TYPE_FUNCTION );
  tnode->of = of;
  tnode->u.memberScope = params;

  return New_TQualNode( tnode );
};

//--------------------------------------------------------------------------
// Name         TYPE_ChainType 
//
// Attach two types, validatiing the "derived from" type.
//
// This function is used in declarations, when brackets have been used to 
// change the normal declaration priority.
//
// For example 
//   "int (*a)[10]".
// At the time we are building the pointer descriptor, we haven't parsed
// the array, so we call Type_MakePointer with of==NULL. Thus Type_MakePointer()
// could not perform any validations of _of_ and we have to perform them here.
//
// Note that the type in _head_ is modifiable since it has been freshly
// allocated in the declarator and we know that it is not shared (while
// _of_ may be a typedef, for example). That is why _head_ is not pointer
// to const TQualNode - in practice it makes no difference, but it is an 
// indication that the _head_ type chain will be modified.
//--------------------------------------------------------------------------
const TQualNode * Type_ChainType ( TQualNode * head, const TQualNode * of )
{
  const TTypeNode * tnode;

  ASSERT( of != NULL );

  // if there is no head, we have no validations to perform. Just return 
  // the tail
  //
  if (head == NULL)
    return of;

  // Find the end of the _head_ type chain
  //
  for ( tnode = head->tnode; tnode->of != NULL; tnode = tnode->of->tnode )
  {
    ASSERT( tnode->of->tnode != NULL );
  }

  switch (tnode->typeKind)
  {
  case TYPE_ARRAY:
    of = TYPE_ValidateArrayElement( of );
    break;

  case TYPE_POINTER:
    of = TYPE_ValidateOf( of );
    break;

  case TYPE_FUNCTION:
    of = TYPE_ValidateFunctionResult( of );
    break;

  case TYPE_ERROR: // if head ends with TYPE_ERROR, just leave it that way
    goto exitFunc;

  default:
    CHECK_ASSUME( 0 ); // No other type can be chained
  }

  MODIFY_TNODE(tnode)->of = of;

exitFunc:
  return head;
};

//--------------------------------------------------------------------------
// Name         Type_Unqual 
//
// Returns an unqualified version of a type
//--------------------------------------------------------------------------
const TQualNode * Type_Unqual ( const TQualNode * typ )
{
  return New_TQualNode( typ->tnode );
}

//== TSymbol ==================================================================

const char * const StorageClass_Name[] =
{
  "none",
  "extern",
  "static",
  "auto",
  "register",
  "typedef",
};

const char * const StorageDuration_Name[] = 
{
  "none",
  "static",
  "automatic",
  "member",
  "code"
};

struct TSymbol_loc
{
  TIdent      id;
  TIdentNode  * identNode; 
      // identNode is the location in the identTable. Can be used
      // for fast removal. 

  // NOTE: THIS MUST BE THE THIRD MEMBER. SEE Symbol_Clone() 
  const TQualNode * type;

  const TQualNode * defType;
    /* This is the symbol type at the moment of its
       definition. This is necessary since the type of an already defined symbol
       could change when new declarations augment it. For example this is valid C:
      
         void func ( void (*f)() )        { (*f)(); }
         void func ( void (*g)(int a) );

       The type of func() changes (even the names of the parameters), but it has 
       already been defined!
       Another example:

         void (*f)() = 0;
         void (*f)(int);
    */

  TSymbol   * declaresGlobal; 
      // block scope extern definitions are a bit of a contradiction. They 
      // have a block scope themselves, but they declare a global symbol. 
      // _declaresGlobal_ points to the symbol declared in global scope.
      //
      // In file scope, when we have a second declaration of a symbol,
      // potentially with a slightly different type, 
      // _declaresGlobal_ points to the first symbol.


  StorageClass_t     storageClass     : 8;
  StorageDuration_t  storageDuration  : 8;
  bool               bBitfield        : 1;
  bool               bIdentDefined    : 1;
    // true if an old-style identifier in an identifier list has been
    // defined in the old-style declaration list.

  uint_least16_t     bitfieldWidth;

  TExtDeclSpec      * extDeclSpec;

  const TAst        * initValue;

  YYLTYPE    declLoc; // declaration location
};

//--------------------------------------------------------------------------
// Name         TSymbol_GcMark 
//
// 
//--------------------------------------------------------------------------
void TSymbol_GcMark ( const TSymbol * sym )
{
  MAKE_SURE( sizeof(struct TSymbol) == sizeof(struct TSymbol_loc) );

  TIdent_GcMark( &sym->id );
  GC_MARK( sym->identNode );
  GC_MARK( sym->type );
  GC_MARK( sym->defType );
  GC_MARK( sym->declaresGlobal );
  GC_MARK( sym->extDeclSpec );
  GC_MARK( sym->initValue );

  YYLTYPE_GcMark( &sym->declLoc );
};

//--------------------------------------------------------------------------
// Name         New_TSymbol 
//
// 
//--------------------------------------------------------------------------
TSymbol * New_TSymbol ( STRING strName, const TQualNode * type )
{
  TSymbol * res = GC_NEW( TSymbol );
  ASSERT( res ); // we use an abortive allocator

  res->id.strName = strName;
  res->type = type;

  return res;
};

//--------------------------------------------------------------------------
// Name         Symbol_Copy
//
// Copy everything excepe the name (id and identNode)
//--------------------------------------------------------------------------
void Symbol_Copy ( TSymbol * res, const TSymbol * sym )
{
  // Copy everything after _identNode_
  //
  memcpy( 
    (char *)res       + offsetof( TSymbol, type ),
    (const char *)sym + offsetof( TSymbol, type ),
    sizeof(TSymbol)   - offsetof( TSymbol, type )
  );
}

//--------------------------------------------------------------------------
// Name         Symbol_Clone 
//
// 
//--------------------------------------------------------------------------
TSymbol * Symbol_Clone ( const TSymbol * sym )
{
  TSymbol * res;

  MAKE_SURE( sizeof(struct TSymbol) == sizeof(struct TSymbol_loc) );
  ASSERT( sym );

  res = New_TSymbol( sym->id.strName, NULL );

  Symbol_Copy( res, sym );

  return res;
};

//--------------------------------------------------------------------------
// Name         Symbol_ChainType 
//
// Returns the symbol for convenience
//--------------------------------------------------------------------------
TSymbol * Symbol_ChainType ( TSymbol * sym, const TQualNode * qual )
{
  ASSERT( sym != NULL );

  sym->type = Type_ChainType( MODIFY_QUAL(sym->type), qual );

  return sym;
};

//--------------------------------------------------------------------------
// Name         Symbol_Hide 
//
// 
//--------------------------------------------------------------------------
void Symbol_Hide ( TSymbol * sym )
{
  ASSERT( Symbol_IsVisible( sym ) );

  REMOVE_ENTRY_LIST( &sym->id.link );
  sym->id.link.Flink = NULL;
};

//--------------------------------------------------------------------------
// Name         Symbol_ChangeName
//
// 
//--------------------------------------------------------------------------
void Symbol_ChangeName ( TSymbol * sym, STRING newName )
{
  if (Symbol_IsVisible(sym))
    Symbol_Hide( sym );

  sym->id.strName = newName;
};

//=============================================================================

//--------------------------------------------------------------------------
// Name         BuildIntegerTypes 
//
// 
//--------------------------------------------------------------------------
static void BuildIntegerTypes ( void )
{
  IntegerType_t i;
  for ( i = 0; i < it_Last; ++i )
  {
    TTypeNode * tn = New_TTypeNode( TYPE_INTEGER );
    tn->u.pIntDesc = &IntegerTypesDesc[i];

    CfeRoots.IntegerTypes[i] = New_TQualNode( tn );
  }
};

//--------------------------------------------------------------------------
// Name         BuildFloatingTypes 
//
// 
//--------------------------------------------------------------------------
static void BuildFloatingTypes ( void )
{
  TypeKind_t tk;

  for ( tk = TYPE_REAL; tk <= TYPE_COMPLEX; ++tk )
  {
    RealType_t rt;

    for ( rt = 0; rt < rt_Last; ++rt )
    {
      TTypeNode * tn = New_TTypeNode( tk );
      tn->u.realType = rt;

      CfeRoots.FloatingTypes[tk][rt] = New_TQualNode( tn );
    }
  }
};

//--------------------------------------------------------------------------
// Name         CTypes_Init 
//
// 
//--------------------------------------------------------------------------
void CTypes_Init ( void )
{
  MAKE_SURE( NELEM( TypeKind_Name ) == TYPE_LAST );
  MAKE_SURE( NELEM( StorageClass_Name ) == SC_LAST );
  MAKE_SURE( NELEM( StorageDuration_Name ) == STORAGE_DURATION_LAST );

  BuildIntegerTypes();
  BuildFloatingTypes();

  {
    TTypeNode * tn;
    TQualNode * qn;
    
    // void
    tn = New_TTypeNode( TYPE_VOID );
    CfeRoots.VoidType = New_TQualNode( tn );

    // error type
    CfeRoots.ErrorType = New_TQualNode( New_TTypeNode( TYPE_ERROR ) );

    // typeof(...)
    CfeRoots.EllipsisType = New_TQualNode( New_TTypeNode( TYPE_NONE ) );

    // char
    CfeRoots.CharType = bCharIsSigned ? CfeRoots.SChar_Type : CfeRoots.UChar_Type;

    // const char
    CfeRoots.ConstCharType = qn = New_TQualNode( CfeRoots.CharType->tnode );
    qn->isConst = true;

    // const wchar_t
    CfeRoots.ConstWCharType = qn = New_TQualNode( CfeRoots.WChar_Type->tnode );
    qn->isConst = true;
  }
};

