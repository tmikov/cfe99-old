#ifndef C_TYPES_H
#define C_TYPES_H


// Declare enumeration constants
//

typedef enum TypeNode_t
{
  NODE_NONE = 0,

  NODE_BASIC,  // void,int,char,float,double,complex
  NODE_STRUCT,
  NODE_UNION,
  NODE_ARRAY,
  NODE_POINTER,
  NODE_FUNCTION,

} TypeNode_t;

typedef enum BasicType_t
{
  BT_NONE = 0,

  BT_VOID,
  BT_INTEGER,
  BT_ENUM,
  BT_REAL,
  BT_IMAGINERY,
  BT_COMPLEX,

} BasicType_t;


enum RealType_t
{
  RT_FLOAT, RT_DOUBLE, RT_LONGDOUBLE, RT_LAST
};

typedef struct TIntegerTypeDesc
{
  unsigned rank;      // integer conversion rank. 
  unsigned precision; // number of bits
  unsigned size;      // number of characters

  unsigned char L;           // how long
  bool isExtended; 
  bool isSigned;

  struct TIntegerTypeDesc * brother; // the corresponding signed/unsigned type

  TARGET_LARGEST_INT_TYPE mask;
  TARGET_LARGEST_INT_TYPE minValue, maxValue;
} TIntegerTypeDesc;


// Ordered by conversion rank
enum IntegerType_t
{
  IT_SIGNED_BOOL,        IT_BOOL,  /* signed bool is for completeness */
  IT_SIGNED_CHAR,        IT_UNSIGNED_CHAR,
  IT_SIGNED_SHORT,       IT_UNSIGNED_SHORT,
  IT_SIGNED_INT,         IT_UNSIGNED_INT,
                            
  IT_SIGNED_EXTYPE,      IT_UNSIGNED_EXTYPE,
                            
  IT_SIGNED_LONG,        IT_UNSIGNED_LONG,
  IT_SIGNED_LONGLONG,    IT_UNSIGNED_LONGLONG,

  IT_LAST,
};

// This is the structure that describes the qualifiers for any
// type (TTypeNode)
//
typedef struct TQualNode 
{
  bool isConst    : 1;
  bool isVolatile : 1;
  bool isRestrict : 1;

  unsigned otherAttr; // Each attr is a bit

  struct TTypeNode * tnode;       // the type that the qualifiers apply to
  struct TSymbol   * thisTypedef; // if this is a typedef, point to it
} TQualNode;

typedef struct TQualNode * TYPE:

typedef struct TTypeNode
{
  TListEntry  link; // link in the namespace chain
  struct TTypeNode * pPrev, * pNext; // prev tag in the same scope

  TypeNode_t  nodeKind  : 8;
  BasicType_t basicKind : 8; // only if nodeKind == NODE_BASIC

  TQualNode  * of; // link to the next node for array/pointer/function

  STRING strTag;   // struct/union/enum tag

  union
  {
//==========================
// nodeKind == NODE_BASIC
//
    const TIntegerTypeDesc * pIntDesc; // for BT_INTEGER, BT_ENUM
    RealType_t realType;               // for BT_REAL, BT_IMAGINERY, BT_COMPLEX


//==========================
// nodeKind == NODE_ARRAY
//
    AST arrayLen; // NULL for unknown, non-const for variable

//==========================
// nodeKind == NODE_STRUCT/NODE_UNION/NODE_FUNCTION
//
    struct TScope * memberScope; // struct members of function parameters
  } u;
} TTypeNode;


enum StorageClass_t
{
  SC_NONE,
  SC_EXTERN,
  SC_STATIC,
  SC_AUTO,
  SC_REGISTER,
  SC_TYPEDEF
};

enum Linkage_t
{
  LNK_NONE,
  LNK_EXTERNAL,
  LNK_INTERNAL,
};

enum StorageDuration_t
{
  STORAGE_DURATION_NONE,
  STORAGE_DURATION_STATIC,
  STORAGE_DURATION_AUTOMATIC,
  STORAGE_DURATION_MEMBER,
};

struct TSymbol
{
  TListEntry  link;       // link in the namespace chain
  struct TSymbol * pPrev, * pNext; // idents in the scope

  STRING strName;
  TYPE type;

  struct TScope * scope;

  StorageClass_t     storageClass     : 8;
  Linkage_t          linkage          : 8;
  StorageDuration_t  storageDuration  : 8;

  void * declSpec;

  AST initValue;
};

/*
int a;            SC_NONE,    LNK_EXTERNAL, STORAGE_DURATION_STATIC, 0

static int a;     SC_STATIC,  LNK_INTERNAL, STORAGE_DURATION_STATIC, 0

extern int a;     SC_EXTERN,  LNK_EXTERNAL, STORAGE_DURATION_STATIC, 0

typedef int INT;  SC_TYPEDEF, LNK_NONE,     STORAGE_DURATION_NONE,   0

{
  int a;          SC_NONE,    LNK_NONE,     STORAGE_DURATION_AUTOMATIC, 1
  static int a;   SC_STATIC,  LNK_INTERNAL, STORAGE_DURATION_STATIC,  1
}

enum {
  A               SC_NONE,    LNK_NONE,     STORAGE_DURATION_NONE,   0
}
*/


typedef struct TScope
{
  struct TScope * parentScope;
  int level; // 0 for global

  TSymbol * pFirstSymbol, * pLastSymbol; // symbols in the scope
  TTypeNode * pFirstTag, * pLastTag;     // tags in the scope

} TScope;



//
// Extern declarations
//

extern const TIntegerTypeDesc   IntegerTypesDesc[IT_LAST]; // from TCONF.C

#define Bool_Desc               IntegerTypesDesc[IT_BOOL]          
#define Signed_Char_Desc        IntegerTypesDesc[IT_SIGNED_CHAR]   
#define Unsigned_Char_Desc      IntegerTypesDesc[IT_UNSIGNED_CHAR] 
#define Signed_Short_Desc       IntegerTypesDesc[IT_SIGNED_SHORT]  
#define Unsigned_Short_Desc     IntegerTypesDesc[IT_UNSIGNED_SHORT]
#define Signed_Int_Desc         IntegerTypesDesc[IT_SIGNED_INT]    
#define Unsigned_Int_Desc       IntegerTypesDesc[IT_UNSIGNED_INT]  
#define Signed_Extype_Desc      IntegerTypesDesc[IT_SIGNED_EXTYPE]
#define Unsigned_Extype_Desc    IntegerTypesDesc[IT_UNSIGNED_EXTYPE]
#define Signed_Long_Desc        IntegerTypesDesc[IT_SIGNED_LONG]   
#define Unsigned_Long_Desc      IntegerTypesDesc[IT_UNSIGNED_LONG] 
#define Signed_Long_Desc        IntegerTypesDesc[IT_SIGNED_LONG]   
#define Unsigned_Long_Desc      IntegerTypesDesc[IT_UNSIGNED_LONG] 
#define Signed_LongLong_Desc    IntegerTypesDesc[IT_SIGNED_LONGLONG]
#define Unsigned_LongLong_Desc  IntegerTypesDesc[IT_UNSIGNED_LONGLONG]

#define WChar_Desc              Unsigned_Short_Desc

extern TYPE const IntegerTypes[IT_LAST];
extern TYPE const RealTypes[RT_LAST];

#define Bool_Type               IntegerTypes[IT_BOOL]          
#define Signed_Char_Type        IntegerTypes[IT_SIGNED_CHAR]   
#define Unsigned_Char_Type      IntegerTypes[IT_UNSIGNED_CHAR] 
#define Signed_Short_Type       IntegerTypes[IT_SIGNED_SHORT]  
#define Unsigned_Short_Type     IntegerTypes[IT_UNSIGNED_SHORT]
#define Signed_Int_Type         IntegerTypes[IT_SIGNED_INT]    
#define Unsigned_Int_Type       IntegerTypes[IT_UNSIGNED_INT]  
#define Signed_Extype_Type      IntegerTypes[IT_SIGNED_EXTYPE]
#define Unsigned_Extype_Type    IntegerTypes[IT_UNSIGNED_EXTYPE]
#define Signed_Long_Type        IntegerTypes[IT_SIGNED_LONG]   
#define Unsigned_Long_Type      IntegerTypes[IT_UNSIGNED_LONG] 
#define Signed_Long_Type        IntegerTypes[IT_SIGNED_LONG]   
#define Unsigned_Long_Type      IntegerTypes[IT_UNSIGNED_LONG] 
#define Signed_LongLong_Type    IntegerTypes[IT_SIGNED_LONGLONG]
#define Unsigned_LongLong_Type  IntegerTypes[IT_UNSIGNED_LONGLONG]

#define WChar_Type              Unsigned_Short_Type

#define Float_Type              RealTypes[RT_FLOAT]
#define Double_Type             RealTypes[RT_DOUBLE]
#define LongDouble_Type         RealTypes[RT_LONGDOUBLE]


#endif // C_TYPES_H

