#include "precomp.h"
#include "cfe_priv.h"
#include "c-ast.h"

//--------------------------------------------------------------------------
// Name         TAst_GcMark 
//
// 
//--------------------------------------------------------------------------
static void TAst_GcMark ( const TAst * ast )
{
  struct TAst_loc
  {
    AstCode_t code;
    TAst * child, * sibling;

    YYLTYPE loc;

    const TQualNode * exprType; 
      // type of the expression. It is never ErrorType. If the type would
      // have to be ErrorType then the expression is ast_error.

    union
    {
      TARGET_IMAX_T             intConst;
      TARGET_FLOAT_T            floatConst;
      TARGET_DOUBLE_T           doubleConst;
      TARGET_LONG_DOUBLE_T      longDoubleConst;

      TSymbol                   * sym;
    } u;
  };

  MAKE_SURE( sizeof(struct TAst_loc) == sizeof(struct TAst) );

  GC_MARK( ast->child );
  GC_MARK( ast->sibling );
  YYLTYPE_GcMark( &ast->loc );
  GC_MARK( ast->exprType );

  switch (ast->code)
  {
  case ast_symbol:
    GC_MARK( ast->u.sym ); break;
  }
};

//--------------------------------------------------------------------------
// Name         Ast 
//
// TODO: allocate different length depending on the code
//--------------------------------------------------------------------------
TAst * Ast ( AstCode_t code, const YYLTYPE * loc )
{
  TAst * res = GC_NEW( TAst );

  res->code = code;
  if (loc)
    res->loc = *loc;

  return res;
};

/*
//--------------------------------------------------------------------------
// Name         FreeAst 
//
// 
//--------------------------------------------------------------------------
void FreeAst ( TAst * ast )
{
  TAst * kid;

  if (ast == NULL)
    return;

  ASSERT( ast->sibling == NULL );

  kid = ast->child; 
  while (kid != NULL)
  {
    TAst * tmp = kid;
    kid = kid->sibling;
#ifdef _DEBUG
    tmp->sibling = NULL;
#endif
    FreeAst( tmp );
  }

  GC_DELETE( ast );
};
*/

//--------------------------------------------------------------------------
// Name         Ast_error 
//
// 
//--------------------------------------------------------------------------
TAst * Ast_error ( const YYLTYPE * loc )
{
  TAst * ast = Ast( ast_error, loc );
  ast->exprType = CfeRoots.ErrorType;
  return ast;
}

//--------------------------------------------------------------------------
// Name         Ast_IntConst
//
// 
//--------------------------------------------------------------------------
TAst * Ast_intConst( const YYLTYPE * loc, const TQualNode * exprType, TARGET_IMAX_T intConst )
{
  TAst * ast = Ast( ast_intConst, loc );
  ast->exprType = exprType;
  ast->u.intConst = intConst;
  return ast;
};

//--------------------------------------------------------------------------
// Name         Ast_floatConst
//
// 
//--------------------------------------------------------------------------
TAst * Ast_floatConst ( const YYLTYPE * loc, TARGET_FLOAT_T floatConst )
{
  TAst * ast = Ast( ast_floatConst, loc );
  ast->exprType = CfeRoots.Float_Type;
  ast->u.floatConst = floatConst;
  return ast;
};

//--------------------------------------------------------------------------
// Name         Ast_doubleConst
//
// 
//--------------------------------------------------------------------------
TAst * Ast_doubleConst ( const YYLTYPE * loc, TARGET_DOUBLE_T doubleConst )
{
  TAst * ast = Ast( ast_doubleConst, loc );
  ast->exprType = CfeRoots.Double_Type;
  ast->u.doubleConst = doubleConst;
  return ast;
};

//--------------------------------------------------------------------------
// Name         Ast_longDoubleConst
//
// 
//--------------------------------------------------------------------------
TAst * Ast_longDoubleConst ( const YYLTYPE * loc, TARGET_LONG_DOUBLE_T longDoubleConst )
{
  TAst * ast = Ast( ast_longDoubleConst, loc );
  ast->exprType = CfeRoots.LongDouble_Type;
  ast->u.longDoubleConst = longDoubleConst;
  return ast;
};

//--------------------------------------------------------------------------
// Name         Ast_cast 
//
// 
//--------------------------------------------------------------------------
TAst * Ast_cast ( const YYLTYPE * loc, const TQualNode * exprType, TAst * op )
{
  TAst * ast = Ast( ast_cast, loc );
  ast->exprType = exprType;

  ASSERT( op->sibling == NULL );
  ast->child = op;

  return ast;
};

//--------------------------------------------------------------------------
// Name         Ast_binOp 
//
// 
//--------------------------------------------------------------------------
TAst * Ast_binOp ( AstCode_t code, const YYLTYPE * loc, const TQualNode * exprType, TAst * l, TAst * r )
{
  TAst * ast = Ast( code, loc );
  ast->exprType = exprType;

  ASSERT( l->sibling == NULL && r->sibling == NULL );
  ast->child = l;
  l->sibling = r;

  return ast;
};


const char * Ast_Name[] = 
{
  "empty",
#define AST_OP( x )  #x,
#  include "c-ast_ops.inc"
#undef AST_OP
};


