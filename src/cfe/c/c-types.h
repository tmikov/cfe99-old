#ifndef C_TYPES_H
#define C_TYPES_H

typedef struct TScope TScope;
typedef struct TTypeNode TTypeNode;

//== TIdent ===================================================================

/*
  TIdent: all C identifiers share this header structure. It contains the info
  of a named identifier linked in a scope.
*/
struct TIdent
{
  TListEntry  link;              // link in the namespace chain
  TIdent      * pPrev, * pNext;  // link in the scope symbol list
  STRING      strName;
  TScope      * scope;           // if scope != NULL, pPrev and pNext 
                                 // are valid 
};

void TIdent_GcMark ( const TIdent * id );

const char * STRING_Name ( STRING str );

#define Ident_IsVisible( id )   ((id)->link.Flink != NULL)
#define Ident_Name( id )        STRING_Name( (id)->strName )

//== TQualNode ================================================================

// This is the structure that describes the qualifiers for any
// type (TTypeNode)
//
typedef struct TQualNode TQualNode;
struct TQualNode
{
  bool isConst    : 1;
  bool isVolatile : 1;
  bool isRestrict : 1;

  ExtQualAttr_t extQualAttr; // Each attr is a bit

  const TTypeNode * tnode;   // the type that the qualifiers apply to
};

void TQualNode_GcMark ( const TQualNode * qual );

TQualNode * New_TQualNode ( const TTypeNode * tnode );
TQualNode * QualNode_Clone ( const TQualNode * qual, const TTypeNode * tnode );

//== TTypeNode ================================================================

typedef enum TypeKind_t
{
  TYPE_NONE = 0,

  // In lots of tables we use the fact that the basic types start from 1
  TYPE_VOID = 1,
  TYPE_BOOL,
  TYPE_INTEGER,
  TYPE_ENUM,
  TYPE_REAL,
  TYPE_IMAGINARY,
  TYPE_COMPLEX,
  TYPE__BT_LAST, // end of basic types

  TYPE_ERROR = TYPE__BT_LAST, // don't waste any numbers

  TYPE_STRUCT,
  TYPE_UNION,
  TYPE_ARRAY,
  TYPE_POINTER,
  TYPE_FUNCTION,

  TYPE_LAST
} TypeKind_t;

extern const char * const TypeKind_Name[];

inline bool TK_IsInteger ( TypeKind_t typeKind );

typedef struct TTypeNode TTypeNode;
struct TTypeNode
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

void TTypeNode_GcMark ( const TTypeNode * node );

TTypeNode * New_TTypeNode ( TypeKind_t typeKind );
TTypeNode * New_Tag ( TypeKind_t typeKind, STRING name );
TTypeNode * TypeNode_Clone ( const TTypeNode * tnode, const TQualNode * of );

//== TEnumDesc ================================================================

typedef struct TEnumDesc TEnumDesc;
struct TEnumDesc
{
  const TTypeNode * tag;
  TQualNode       * type;
  TARGET_IMAX_T   curVal;
};

void TEnumDesc_GcMark ( const TEnumDesc * edesc );

TEnumDesc * New_TEnumDesc ( const TTypeNode * tag );

//== Type functions ===========================================================


inline bool Type_IsInteger ( const TQualNode * typ );
inline bool Type_IsFloating ( const TQualNode * typ );
inline bool Type_IsVoid ( const TQualNode * typ );
inline bool Type_IsArithmetic ( const TQualNode * typ );
inline bool Type_IsError ( const TQualNode * typ );
bool Type_IsComplete ( const TQualNode * qual );

const TQualNode * Type_MakePointer ( const TQualNode * to, TQualNode * qual );

const TQualNode * Type_MakeArray ( 
    const TQualNode * of, 
    TQualNode       * qual,
    const TAst      * len, 
    bool            bStaticSize
  );
const TQualNode * Type_MakeFunction ( const TQualNode * of, TScope * params );
const TQualNode * Type_ChainType ( TQualNode * head, const TQualNode * of );
const TQualNode * Type_Unqual ( const TQualNode * typ );

//== TSymbol ==================================================================

typedef enum StorageClass_t
{
  SC_NONE,
  SC_EXTERN,
  SC_STATIC,
  SC_AUTO,
  SC_REGISTER,
  SC_TYPEDEF,

  SC_LAST
} StorageClass_t;

extern const char * const StorageClass_Name[];

typedef enum StorageDuration_t
{
  STORAGE_DURATION_NONE,         // no storage at all
  STORAGE_DURATION_STATIC,       // in the data segment
  STORAGE_DURATION_AUTO,         // in the stack
  STORAGE_DURATION_MEMBER,       // in a structure
  STORAGE_DURATION_CODE,         // in the code segment

  STORAGE_DURATION_LAST
} StorageDuration_t;

extern const char * const StorageDuration_Name[];

struct TSymbol
{
  TIdent      id;
  TIdentNode  * identNode; 
      // identNode is the location in the identTable. Can be used
      // for fast removal. 

  // NOTE: THIS MUST BE THIRD MEMBER. SEE Symbol_Clone() 
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

void TSymbol_GcMark ( const TSymbol * sym );

#define Symbol_IsVisible( sym ) Ident_IsVisible( &(sym)->id )
#define Symbol_Name( sym )      Ident_Name( &(sym)->id )

TSymbol * New_TSymbol ( STRING strName, const TQualNode * type );
void      Symbol_Copy ( TSymbol * res, const TSymbol * src );
TSymbol * Symbol_Clone ( const TSymbol * sym );
TSymbol * Symbol_ChainType ( TSymbol * sym, const TQualNode * qual );
void      Symbol_Hide ( TSymbol * sym );
void      Symbol_ChangeName ( TSymbol * sym, STRING newName );


//== TLabel ===================================================================

typedef struct TLabel TLabel;
struct TLabel
{
  TIdent      id;
};


//== TScope ===================================================================

typedef enum ScopeDisposition_t
{
  SCOPE_FILE,       // file scope
  SCOPE_BLOCK,      // function block scope
  SCOPE_STRUCT,     // inside a struct or union
  SCOPE_PARAMS,     // function parameters scope
  SCOPE_IDENT_LIST, // old-style function identifier list

  SCOPE_DISPOSITION_LAST
} ScopeDisposition_t;


typedef struct TScope TScope;
struct TScope
{
  TScope    * parentScope;

  ScopeDisposition_t disposition;
  bool bHaveEllipsis : 1;

  TLabel    * pFirstLabel,  * pLastLabel;
  TSymbol   * pFirstSymbol, * pLastSymbol; // symbols in the scope
  TTypeNode * pFirstTag,    * pLastTag;    // tags in the scope

  TIdentTab * idTab;
};


//== CTypes module ============================================================

void CTypes_Init ( void );

//== Type modification ========================================================

/*
  What to do when we must modify a type ?
  ------------------------------------------

  Why is this a problem at all ? Because th etype that we are modifying might be a part
  of a typedef which is also used in other declarations. If we modified the typedef, we are
  would incorrectly modify other declarations. At first it seems that there aren't any such 
  cases, but I can think of at least one:

    void func ( int i )
    {
      typedef int va_t[i];
      va_t va1;
      static va_t va2;
    }

  va2 can't be a variable-modified type, so in order to fix the error we will modify the
  array size, thus affecting va1 !

  To avoid this, the parameters of the ValidateXXXType() functions are const. In order
  to modify the type, we must obtain a non-const pointer by calling the macro CLONE_RNODE()
  defined below. It clones the type only if it hasn't been cloned already and assigns it to
  _rnode_. _rnode_ must naturally be defined as non-const.

  There are cases when it is safe to modify the type in-place. This is when
  the exact same modification would have to be done in every declaration - so
  there is no sense of creating a local copy of the type. When we are 
  absolutely certain that this is the case, we use MODIFY_TNODE() to obtain
  a writable pointer to _tnode_.
*/

#define CLONE_RNODE( rnode, tnode, of )  \
  ((TTypeNode *)((rnode) != NULL ? (rnode) : ( (tnode) = (rnode) = TypeNode_Clone( tnode, of ) ) ))

#define CLONE_RQUAL( rqual, qual, tnode ) \
  ((TQualNode *)( (rqual) != NULL ? (rqual) : ( (qual) = (rqual) = QualNode_Clone( qual, tnode ) ) ))

#define CLONE_RSYM( rsym, sym ) \
  ((TSymbol *)( (rsym) != NULL ? (rsym) : ( (sym) = (rsym) = Symbol_Clone( sym ) ) ))


inline TQualNode * MODIFY_QUAL( const TQualNode * qual )  
{
  return (TQualNode *)qual;
}

inline TTypeNode * MODIFY_TNODE( const TTypeNode * tnode )  
{
  return (TTypeNode *)tnode;
}

inline TSymbol * MODIFY_SYM( const TSymbol * sym )
{
  return (TSymbol *)sym;
}

//**************************************************************************
//
//  Inline functions implementations
//

//--------------------------------------------------------------------------
// Name         TK_IsInteger 
//
//6.2.5-17 The type char, the signed and unsigned integer types, and the enumerated types are
//collectively called integer types. The integer and real floating types are collectively called
//real types.
//--------------------------------------------------------------------------
inline bool TK_IsInteger ( TypeKind_t tk )
{
  return 
    tk == TYPE_BOOL ||
    tk == TYPE_INTEGER || 
    tk == TYPE_ENUM;
};

//--------------------------------------------------------------------------
// Name         Type_IsInteger 
//
// 
//--------------------------------------------------------------------------
inline bool Type_IsInteger ( const TQualNode * typ )
{
  ASSERT( typ && typ->tnode );

  // Note: we can safely check _basicType_, because it would be BT_NONE
  // for non-basic types
  return TK_IsInteger( typ->tnode->typeKind );
};


//--------------------------------------------------------------------------
// Name         Type_IsFloating 
//
// 
//--------------------------------------------------------------------------
inline bool Type_IsFloating ( const TQualNode * typ )
{
  ASSERT( typ && typ->tnode );

  return 
    typ->tnode->typeKind == TYPE_REAL ||
    typ->tnode->typeKind == TYPE_IMAGINARY ||
    typ->tnode->typeKind == TYPE_COMPLEX;
};

//--------------------------------------------------------------------------
// Name         Type_IsVoid 
//
// 
//--------------------------------------------------------------------------
inline bool Type_IsVoid ( const TQualNode * typ )
{
  ASSERT( typ && typ->tnode );
  return typ->tnode->typeKind == TYPE_VOID;
};

/* 
*/
//--------------------------------------------------------------------------
// Name         Type_IsArithmetic 
//
// 6.2.5-18 Integer and floating types are collectively called arithmetic types. Each arithmetic type
// belongs to one type domain: the real type domain comprises the real types, the complex
// type domain comprises the complex types.
//--------------------------------------------------------------------------
inline bool Type_IsArithmetic ( const TQualNode * typ )
{
  return Type_IsInteger( typ ) || Type_IsFloating( typ );
};

//--------------------------------------------------------------------------
// Name         Type_IsError 
//
// 
//--------------------------------------------------------------------------
inline bool Type_IsError ( const TQualNode * typ )
{
  return typ->tnode->typeKind == TYPE_ERROR;
};

#endif // C_TYPES_H

