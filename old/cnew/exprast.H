#ifndef EXPRAST_H
#define EXPRAST_H

#include "exprcode.h"       // the expression tree codes
#include "csyms.h"          // symbols and types

// A commutative operator
//
#define EF_COMMUTATIVE   1

// A structure describing a tree code
//
typedef struct TExprCode TExprCode;
struct TExprCode
{
  const char * eCodeStr;   // string representation of the enum
  const char * cCodeStr;   // string representation of the code
  int          paramCount; // number of parameters
  unsigned     flags;
};


typedef struct TExprAst TExprAst;
struct TExprAst
{
  EXPR_CODE   ecode;         // operation code
  unsigned    isLValue : 1;
  TYPE      * type;

  union                      // data relevant to each specific tree code
  {
    TExprAst       * op[3];  // for unary/binary/ternary operators
    struct TSymbol * symbol; // for symbols
    TARGET_DOUBLE    fcon;   // for floating point constants
    TARGET_LONG      lcon;
    TARGET_INT       icon;
    TARGET_CHAR      ccon;
  } data;
};

DECLARE_FIXED_ALLOCATOR( ExprAstHeap, TExprAst );


#endif // EXPRAST_H
