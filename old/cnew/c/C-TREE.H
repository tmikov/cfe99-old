#ifndef C_TREE_H
#define C_TREE_H

#include "c-types.h"

#include "exprcode.h"       // the tree codes

// Expression code flags
// TODO: complete them
//

// A commutative operator
//
#define EF_COMMUTATIVE   1

// A structure describing a tree code
//
typedef struct TExprCode
{
  const char * eCodeStr;  // string representation of the enum
  const char * cCodeStr;  // string representation of the code
  int  paramCount;          // number of parameters
  unsigned flags;
} TExprCode;

typedef struct TTreeValue
{
  enum EXPR_CODE ecode; // operation code

  unsigned isLValue : 1;

  TYPE * type;

  // This union contains the data relevant to each specific tree code
  //
  union
  {
    // for unary/binary/ternary operators
    struct
    {
      struct TTreeValue * a1, * a2, * a3;
    } op;

    // for symbols
    //
    struct TSymbol * symbol;

    // for integer constants
    //
    union
    {
      TARGET_LONG  lng;
      TARGET_ULONG ulng;
    } icon;

    // for floating point constants
    TARGET_DOUBLE fcon;

  } data;

} TTreeValue;

// Declare the memory alocators for all types
//
DECLARE_FIXED_ALLOCATOR( ValueHeap, TTreeValue );

extern TExprCode ExprCode[];

// Extract the number of parameters available for an e-code
#define EXPR_PARAM_COUNT( ecode )  (ExprCode[ecode].paramCount)

////////// Functions

void CTree_Init ( void );
void CTree_Done ( void );

// Functions for manipulating tree nodes

// some defines for clarity
//
#define NOTYPE NULL
#define NOARG  NULL

TTreeValue * AllocTree3 (
    enum EXPR_CODE ecode,
    TYPE * type,
    TTreeValue * a1,
    TTreeValue * a2,
    TTreeValue * a3
  );

TTreeValue * AllocEList (
    TTreeValue * item
  );

TTreeValue * EListAppend (
    TTreeValue * head,
    TTreeValue * item
  );

#endif // C_TREE_H
