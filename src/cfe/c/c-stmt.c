#include "precomp.h"
#include "cfe_priv.h"
#include "c-stmt.h"

//--------------------------------------------------------------------------
// Name         Stmt_Expression 
//
// 
//--------------------------------------------------------------------------
void Stmt_Expression ( TAst * expr )
{
  if (expr == NULL)
    return;

  expr = Expr_GetRValue( expr );
  DumpAst( expr );
}
