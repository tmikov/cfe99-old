#include "precomp.h"
#include "heap.h"
#include "c-types.h"


TScope * CurScope = NULL;

TYPE const IntegerTypes[IT_LAST_TYPE];
TYPE const RealTypes[RT_LAST];

//--------------------------------------------------------------------------
// Name         New_Scope 
//
// 
//--------------------------------------------------------------------------
TScope * New_TScope ( TScope * parent )
{
  TScope * res = GC_NEW( TScope );
  if ((res->parentScope = parent) != NULL)
    res->level = parent->level + 1;
  return res;
};


//--------------------------------------------------------------------------
// Name         New_TSymbol 
//
// 
//--------------------------------------------------------------------------
TSymbol * New_TSymbol ( STRING strName )
{
  TSymbol * res = GC_NEW( TSymbol );
  res->strName = strName;
  return res;
};


//--------------------------------------------------------------------------
// Name         New_TTypeNode 
//
// 
//--------------------------------------------------------------------------
TTypeNode * New_TTypeNode ( TypeNode_t nodeKind )
{
  TTypeNode * res = GC_NEW( TTypeNode );
  res->nodeKind = nodeKind;
  return res;
};

//--------------------------------------------------------------------------
// Name         New_TQualNode 
//
// 
//--------------------------------------------------------------------------
TQualNode * New_TQualNode ( TTypeNode * tnode )
{
  TQualNode * res = GC_NEW( TQualNode );
  res->tnode = tnode;
  return res;
};


//--------------------------------------------------------------------------
// Name         BuildIntegerTypes 
//
// 
//--------------------------------------------------------------------------
static void BuildIntegerTypes ( void )
{
  IntegerType_t i;
  for ( i = 0; i < IT_LAST; ++i )
  {
    TTypeNode * tn = New_TTypeNode( NODE_BASIC );
    tn->basicKind = BT_INTEGER;
    tn->u.pIntDesc = &IntegerTypesDesc[i];

    IntegerTypes[i] = New_TQualNode( tn );
  }
};

//--------------------------------------------------------------------------
// Name         BuildRealTypes 
//
// 
//--------------------------------------------------------------------------
static void BuildRealTypes ( void )
{
  RealType_t rt;

  for ( rt = 0; rt < RT_LAST; ++rt )
  {
    TTypeNode * tn = New_TTypeNode( NODE_BASIC );
    tn->basicKind = BT_REAL;
    tn->u.realType = rt;

    RealTypes[rt] = New_TQualNode( tn );
  }
};

//--------------------------------------------------------------------------
// Name         CTypes_Init 
//
// 
//--------------------------------------------------------------------------
void CTypes_Init ( void )
{
  CurScope = NULL;

  BuildIntegerTypes();
  BuilRealTypes();
};