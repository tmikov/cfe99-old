#ifndef C_TYPES_H
#define C_TYPES_H

#include "c-tconf.h"        // target machine types
#include "scantok.h"
#include "strings.h"
#include "misc\fixalloc.h"

enum TYPE_NODE
{
// Note: the order of these value is important. Some checks
// rely on it.

                        NODE_NONE = 0,

//++++ Specifiers
//++++ Integals
                        NODE_BASIC,  // void,int,char,float,double
                        NODE_ENUM,
//---- Integrals

                        NODE_STRUCT,
                        NODE_UNION,
//---- Specifiers

//++++ Declarators
                        NODE_ARRAY,
                        NODE_POINTER,
                        NODE_FUNCTION,
//---- Declarators
};
// Define number of bits necessary to hold a value of that type
#define TYPE_NODE_BITS 4

extern const char * NodeName[];

enum BASIC_TYPE
{
  BT_NONE = 0,

  BT_VOID,
  BT_BOOL,
  BT_CHAR,
  BT_INT,
  BT_FLOAT,
  BT_DOUBLE,

  _BT_LAST,
};
// Define number of bits necessary to hold a value of that type
#define BASIC_TYPE_BITS 4

extern const char * BtName[];

// STORAGE_CLASS and OUTPUT_CLASS together describe a C storage
// class. 
// 

enum STORAGE_CLASS
{
  SC_NONE = 0,

  SC_AUTO,
  SC_REGISTER,
  SC_FIXED,        // it is at a fixed address
  SC_TYPEDEF,

  SC_CONSTANT,     // this is a constant value, not stored in memory
};
// Define number of bits necessary to hold a value of that type
#define STORAGE_CLASS_BITS 4

#define IS_STORAGE_CLASS_LVALUE( sc )  ((sc) >= SC_AUTO && (sc) <= SC_FIXED)

enum OUTPUT_CLASS
{
  OC_NONE = 0,

  OC_PUBLIC,
  OC_PRIVATE,
  OC_EXTERN,
};
// Define number of bits necessary to hold a value of that type
#define OUTPUT_CLASS_BITS 3

// This is the width of the declspec field, and so
// the maxmimum number of declspec-s that are supported
//
#define DECLSPEC_BITS    16

// This describes a storage class. It applies to a symbol
//
typedef struct TStorageClass
{
  enum STORAGE_CLASS storClass  : STORAGE_CLASS_BITS;
  enum OUTPUT_CLASS  outClass   : OUTPUT_CLASS_BITS;

  // information about all declspec-s. Each declspec is represented
  // with a bit.
  unsigned           declSpec   : DECLSPEC_BITS;
} TStorageClass;


// This is the width of the attributes field, and so
// the maxmimum number of attributes that are supported
//
#define ATTRIBUTE_BITS  16

// This is the structure that describes the qualifiers for any
// type (TTypeNode)
//
typedef struct TQualNode
{
  unsigned isConst    : 1;
  unsigned isVolatile : 1;
  unsigned isRestrict : 1;

  // information about all attr-s. Each attr is represented with
  // a bit.
  unsigned       attr : ATTRIBUTE_BITS;

  // this is the type that the qualifiers apply to
  struct TTypeNode * tnode;

  // if this is a typedef, point to it
  //
  struct TSymbol * thisTypedef;
} TQualNode;

#define TYPE TQualNode

typedef struct TTypeNode
{
  enum TYPE_NODE     node       : TYPE_NODE_BITS;

  // Declaring these as bit-fields instead of bit masks
  // is less efficient (although in theory it shouldn't be).
  // However for debugging purposes it is much better. In the
  // final release I may change these into masks.
  //
  unsigned       isClass     : 1; // struct/union
  unsigned       isIntegral  : 1; // char/int/enum
  unsigned       isFloat     : 1; // float/double
  unsigned       isArith     : 1; // isIntegral+isFloat
  unsigned       isScalar    : 1; // arith+a pointer

  // Shows whether a function declaration is implicit
  unsigned       isImplicit  : 1;

  // These three only for node == NODE_BASIC
  unsigned       isSigned    : 1;
  unsigned       isShort     : 1;
  unsigned       isLong      : 1;
  unsigned       isBitfield  : 1;
  unsigned       bitFieldSize : 8;

  // link to the next node for array/pointer/function
  struct TQualNode * of; 

  // Information about the type specifier. It varies depending
  // on the node type
  //
  union
  {
    // NODE_BASIC
    //
    enum BASIC_TYPE  basic;

    // NODE_ENUM/NODE_STRUCT/NODE_UNION
    //
    struct TStructDef * structDef;

    // NODE_POINTER
    //

    // NODE_ARRAY
    //
    TARGET_SIZE_T  arrayLen;

    // NODE_FUNCTION
    //
    struct TScope * funcParams; 

  } info;

} TTypeNode;

#define IS_VOID( pTypeNode ) \
  ((pTypeNode)->node == NODE_BASIC && (pTypeNode)->info.basic == BT_VOID)

#define IS_SPECIFIER( pTypeNode )  ((pTypeNode)->node <  NODE_POINTER)
#define IS_DECLARATOR( pTypeNode ) ((pTypeNode)->node >= NODE_POINTER)

// TLocalIdent duplicates the beginning of TStructDef and TSymbol. This
// way we can use one search function working with TLocalIdent, when we
// want to search for TSymbol or TStructDef
//
typedef struct TLocalIdent
{
  TListEntry  nameLink; // link in the list of symbols with same name
  STRING      strName;  // the name itself

  struct TLocalIdent * nextLocal;   // next symbol in the same scope
  struct TScope      * parentScope; // the scope itself
} TLocalIdent;

#define IS_IDENT_HASHED( pIdent )  ((pIdent)->nameLink.Flink != NULL)

// A wrapper for symbol and struct lists
struct TLocIdentList
{
  struct TLocalIdent * first;
  struct TLocalIdent * last;
};

// This describes a struct/union/enum
//
typedef struct TStructDef
{
  TListEntry  nameLink; // link in the list of symbols with same name
  STRING      strName;  // the name itself

  struct TStructDef * nextLocal;   // next symbol in the same scope
  struct TScope     * parentScope; // the scope itself

  // Defines what this is: struct,union or enum
  //
  enum TYPE_NODE  structType       : TYPE_NODE_BITS;

  // true if the structure is fully defined
  unsigned        isComplete : 1;

  // true if the structure/union has at least one const member, or
  // if a member is a class itself, its _hasConstMembers_ flag is set.
  // In that case the class can't be assigned to.
  //
  unsigned        hasConstMembers : 1;

  union
  {
    // This is the scope that holds all struct's members.
    //
    struct TScope  * structScope;

    TARGET_SIZE_T    enumCount; // the largest value of enum item (so far)

  } info;

} TStructDef;

// The basic structure describing a symbol
//
typedef struct TSymbol
{
  TListEntry  nameLink; // link in the list of symbols with same name
  STRING      strName;  // the name itself

  struct TSymbol    * nextLocal;   // next symbol in the same scope
  struct TScope     * parentScope; // the scope itself

  // info about the storage class (extern,int, etc.)
  // and the declspec-s
  //
  struct TStorageClass * storage;

  // This contains the expression for initializing the symbol
  //
  struct TTreeValue  * init;

  // the type itself
  //
  TQualNode * type;
} TSymbol;


// Type of scope
//
enum SCOPE_TYPE
{
  // this is a scope inside a struct
  STRUCT_SCOPE,     

  // this is the global scope
  FILE_SCOPE,

  // this is a scope inside a function
  FUNCTION_SCOPE,

  // this is a scope iside function parameter declaration
  FUNC_PARAM_SCOPE,
};


// A macro that returns true if a struct declared in a scope should be
// local for that scope
// (for C++ it is always TRUE)
//
#define LOCAL_STRUCTS( scopeType ) ((scopeType) != STRUCT_SCOPE)

// Array with the default storage and output class for each
// scope type
//
extern TStorageClass DefaultScopeStorageClass[4];

typedef struct TScope
{
//  TListEntry     scopeLink;      // link in parent scope
  struct TScope  * parentScope;  // pointer to parent scope

  int            level;          // scope nesting level. 0 for file scope

  // Scope attributes
  //
  enum SCOPE_TYPE scopeType;

  union
  {
    // FUNC_PARAM_SCOPE
    TYPE * funcNode; // points to the function node whose parameters
                     // are being parsed at the moment
  } info;

  // Support the linked list of all idents in the scope
  //
  struct  /* TLocalIdentList */
  {
    TSymbol        * first;
    TSymbol        * last;
  } locSymbols;

  // Linked list of all struct/union/enum in the scope
  struct  /* TLocalIdentList */
  {
    TStructDef     * first;
    TStructDef     * last;
  } locStructs;

  // All sub-scopes
//  TListRoot      subScopes;      // list of all sub-scopes
} TScope;

// Declare the memory alocators for all types
//
DECLARE_FIXED_ALLOCATOR( StorageClassHeap, TStorageClass );
DECLARE_FIXED_ALLOCATOR( QualNodeHeap,     TQualNode );
DECLARE_FIXED_ALLOCATOR( TypeNodeHeap,     TTypeNode );


// Various predefined types. They are used for example in the scanner,
// to specify the type of constant nodes.
//
extern TYPE * const_int_type;
extern TYPE * const_uint_type;
extern TYPE * const_long_type;
extern TYPE * const_ulong_type;
extern TYPE * const_schar_type;
extern TYPE * const_uchar_type;
extern TYPE * const_slchar_type;
extern TYPE * const_ulchar_type;

extern TStorageClass * EnumSClass; // storage class for enum members

// Functions
//
void CTypes_Init ( void );
void CTypes_Done ( void );

TARGET_SIZE_T BasicTypeWidth (
    TYPE * type
  );

bool IsCompleteType (
    const TYPE * qnode
  );

TTypeNode * NewTypeNode (
    enum TYPE_NODE node
  );

TTypeNode * NewBasicTypeNode (
    enum BASIC_TYPE basic
  );

TQualNode * NewQualNode ( void );

#define CombineQualAttr( a, b )  ((a) | (b))
#define CombineDeclSpec( a, b )  ((a) | (b))

void CombineQuals (
    TQualNode * dest,
    const TQualNode * src
  );

TYPE * CopyTypeChain (
    TYPE * src
  );

void FreeTypeChain (
    TYPE * src
  );

bool EqualTypeChains (
    const TYPE * a,
    const TYPE * b
  );

// storage class functions ////////////////////////
//

TStorageClass * NewStorageClass ( void );

void FreeStorageClass (
    TStorageClass * p
  );

#if YYDEBUG
void PrintType ( TYPE * type  );

#define PRINT_TYPE( pref, type, suf )    \
  do {                                   \
    if (pref)                            \
      printf( "%s", pref );              \
    PrintType( type );                   \
    if (suf)                             \
      printf( "%s", suf );               \
  } while (0)

#else
#define PRINT_TYPE( pref, type, suf )
#endif

#endif // C_TYPES_H
