#ifndef C_TYPES_H
#define C_TYPES_H

#include "tconf.h"  // target types configuration
#include "misc\fixalloc.h"

// Declare enumeration constants
//

typedef enum TYPE_NODE TYPE_NODE;
enum TYPE_NODE
{
  NODE_NONE = 0,
  NODE_BASIC,  // void,int,char,float,double
  NODE_ENUM,
  NODE_STRUCT,
  NODE_UNION,
  NODE_ARRAY,
  NODE_POINTER,
  NODE_FUNCTION,
};
#define TYPE_NODE_BITS 4   

typedef enum BASIC_TYPE BASIC_TYPE;
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
#define BASIC_TYPE_BITS 4  

// This is the width of the attributes field, and so
// the maxmimum number of attributes that are supported
//
#define ATTRIBUTE_BITS  16

// This is the structure that describes the qualifiers for any
// type (TTypeNode)
//
typedef struct TQualNode TQualNode;
struct TQualNode
{
  unsigned         isConst    : 1;
  unsigned         isVolatile : 1;
  unsigned         isRestrict : 1;
  unsigned         attr : ATTRIBUTE_BITS; // Each attr is a bit.
  struct TTypeNode * tnode;         // the type that the qualifiers apply to
  struct TSymbol   * thisTypedef;   // if this is a typedef, point to it
};

#define TYPE TQualNode

typedef struct TTypeNode TTypeNode;
struct TTypeNode
{
  TYPE_NODE     node         : TYPE_NODE_BITS;

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

  unsigned       isImplicit  : 1; // is a function declaration implicit

  // These three only for node == NODE_BASIC
  unsigned       isSigned    : 1;
  unsigned       isShort     : 1;
  unsigned       isLong      : 1;
  unsigned       isBitfield  : 1;
  unsigned       bitFieldSize : 8;

  
  TQualNode     * of; // link to the next node for array/pointer/function

  // Information about the type specifier. It varies depending
  // on the node type
  //
  union
  {
    BASIC_TYPE          basic;      // NODE_BASIC
    struct TStructDef * structDef;  // NODE_ENUM/NODE_STRUCT/NODE_UNION
    /* nothing yet */               // NODE_POINTER
    TARGET_INT          arrayLen;   // NODE_ARRAY
    struct TScope     * funcParams; // NODE_FUNCTION
  } info;
};

#define IS_VOID( pTypeNode ) \
  ((pTypeNode)->node == NODE_BASIC && (pTypeNode)->info.basic == BT_VOID)

#define IS_SPECIFIER( pTypeNode )  ((pTypeNode)->node <  NODE_POINTER)
#define IS_DECLARATOR( pTypeNode ) ((pTypeNode)->node >= NODE_POINTER)




extern const char * NodeName[];
extern const char * BtName[_BT_LAST];

TQualNode * QualNode_New ( void );
void        QualNode_Free ( TQualNode * );
TQualNode * QualNode_Copy ( const TQualNode * );

#define     CombineQualAttr( a, b )  ((a) | (b))

TTypeNode * TypeNode_New ( TYPE_NODE node );
TTypeNode * TypeNode_NewBasic ( BASIC_TYPE basic );
TTypeNode * TypeNode_Copy ( const TTypeNode * src );
void        TypeNode_Free ( TTypeNode * );

HOST_UINT BasicTypeWidth ( const TYPE * type );

bool   IsCompleteType ( const TYPE * qnode );
TYPE * CopyTypeChain ( TYPE * src );
bool   EqualTypeChains ( const TYPE * a, const TYPE * b );

TYPE * PointerTo ( TYPE * to, const TQualNode * qual /* = NULL */ );
TYPE * ArrayToPointer ( TYPE * arrayType );
TYPE * FuncToPointer ( TYPE * funcType );

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

