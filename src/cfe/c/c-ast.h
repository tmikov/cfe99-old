#ifndef C_AST_H
#define C_AST_H

typedef enum AstCode_t
{
  ast_empty = 0,

#define AST_OP( x )  ast_##x,
#  include "c-ast_ops.inc"
#undef AST_OP
} AstCode_t;

/*
  TAst * nodes are mutable. A node can be modified in place after it has been
  created. I thought long and hard about leaving them immutable and using
  functional programming style, but saw no practical advantage. 
*/

struct TAst
{
  AstCode_t code;
  TAst * child, * sibling;

  YYLTYPE loc;

  const TQualNode * exprType; 

  union
  {
    TARGET_IMAX_T             intConst;
    TARGET_FLOAT_T            floatConst;
    TARGET_DOUBLE_T           doubleConst;
    TARGET_LONG_DOUBLE_T      longDoubleConst;

    TSymbol                   * sym;
  } u;
};

struct TEmptyAstNode
{
  AstCode_t _empty; 
  TAst * _null, * sibling;
};

#define KID1( node )  ((node)->child)
#define KID2( node )  (KID1(node)->sibling)
#define KID3( node )  (KID2(node)->sibling)
#define KID4( node )  (KID3(node)->sibling)

#define IS_AST_EMPTY( node )   ((node)->code == AST_EMPY)

typedef TAst * KID_POS;

#define FIRST_KID_POS( node )         ((node)->child)
#define NEXT_KID_POS( node, pos )     ((pos)->sibling)
#define END_KIDS( node, pos )         ((pos) == NULL)
#define GET_KID( pos )                (pos)

/*
  TAst * a;

  for ( KID_POS pos = FIRST_KID_POS( a ); !END_KIDS( pos ); pos = NEXT_KID_POS( pos ) )
  {
    TAst * kid = GET_KID( pos );
  }
*/

TAst * Ast ( AstCode_t code, const YYLTYPE * loc );

#define Ast_empty()       Ast( ast_empty, NULL )

TAst * Ast_error ( const YYLTYPE * loc );

TAst * Ast_intConst ( const YYLTYPE * loc, const TQualNode * exprType, TARGET_IMAX_T intConst );
TAst * Ast_floatConst ( const YYLTYPE * loc, TARGET_FLOAT_T floatConst );
TAst * Ast_doubleConst ( const YYLTYPE * loc, TARGET_DOUBLE_T doubleConst );
TAst * Ast_longDoubleConst( const YYLTYPE * loc, TARGET_LONG_DOUBLE_T longDoubleConst );

TAst * Ast_cast ( const YYLTYPE * loc, const TQualNode * exprType, TAst * op );

TAst * Ast_binOp ( AstCode_t code, const YYLTYPE * loc, const TQualNode * exprType, TAst * l, TAst * r );


//--------------------------------------------------------------------------
// Name         Ast_isError
//
// 
//--------------------------------------------------------------------------
inline bool Ast_isError ( const TAst * ast )
{
  return ast->code == ast_error || ast->exprType == CfeRoots.ErrorType;
}

extern const char * Ast_Name[];

#endif // C_AST_H
