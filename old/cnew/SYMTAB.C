#include "port\global.h"
#include "symtab.h"


//////////////////////// public data ///////////////////////////////

TScope * MainScope;

// A TSymbol that denotes the ELLIPSIS parameter in all function declarations
TSymbol * EllipsisSym;

// storage class for enum members
TStorageClass StaticEnumSClass = 
{
  SC_CONSTANT, OC_NONE, 0
};

IMPLEMENT_FIXED_ALLOCATOR( ExprAstHeap,      TExprAst,       256 );

//////////////////////// public functions //////////////////////////

void SymTab_Init ( void )
{
  ExprAstHeap_Init();

  //
  //
  MainScope = Scope_New( FILE_SCOPE, NULL );
  CurScope = MainScope;

  EllipsisSym = Symbol_New( NULL );
};

void SymTab_Done ( void )
{
  ExprAstHeap_Done();
};


