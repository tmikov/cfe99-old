#include "precomp.h"
#include "heap.h"

//--------------------------------------------------------------------------
// Name         New_Ast 
//
// TODO: allocate different length depending on the code
//--------------------------------------------------------------------------
AST New_Ast ( AstCode_t code, const yyltype * loc )
{
  TAstNode * res = GC_NEW( TAstNode );

  res->code = code;
  if (loc)
    res->loc = *loc;
};
