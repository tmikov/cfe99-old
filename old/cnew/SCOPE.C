#include "port\global.h"
#include "scope.h"
#include "misc\arena.h"

extern TArena GlobalArena;   // a global arena used for the strings as well

// Array with the default storage and output class for each
// scope type
//
TStorageClass DefaultScopeStorageClass[] =
{
  { SC_NONE,  OC_NONE, 0 },         // STRUCT_SCOPE
  { SC_FIXED, OC_PUBLIC, 0 },       // FILE_SCOPE
  { SC_AUTO,  OC_NONE, 0 },         // FUNCTION_SCOPE
  { SC_AUTO,  OC_NONE, 0 },         // FUNC_PARAM_SCOPE
};

//--------------------------------------------------------------------------
// Name         Scope_New
//
// Allocates a new scope. If a parent is supplied, inserts in the the
// parent's list.
//--------------------------------------------------------------------------
TScope * Scope_New ( SCOPE_TYPE scopeType, TScope * parent )
{
  TScope * pScope;

  ARENA_NEW0( &GlobalArena, pScope );

  // if there is a parent, initialize the relevant information
  //
  if (parent)
  {
    INSERT_TAIL_LIST( &parent->subScopes, &pScope->scopeLink );
    pScope->level = parent->level + 1;
    pScope->parentScope = parent;
  }

  pScope->scopeType = scopeType;

  INITIALIZE_LIST_HEAD( &pScope->subScopes );

  return pScope;
};

//--------------------------------------------------------------------------
// Name         Scope_Free
//
// 
//--------------------------------------------------------------------------
void Scope_Free ( TScope * pScope )
{
};

//--------------------------------------------------------------------------
// Name         IdentList_Add
//
// 
//--------------------------------------------------------------------------
void IdentList_Add ( TIdentList * pList, TIdent * id )
{
  id->nextLocal = NULL;

  if (pList->last != NULL) // if the list isn't empty
    pList->last->nextLocal = id;
  else
    pList->first = id;

  pList->last = id;
};


//--------------------------------------------------------------------------
// Name         Scope_AddSymbol
//
// 
//--------------------------------------------------------------------------
void Scope_AddSymbol ( TScope * pScope, TSymbol * sym )
{
  IdentList_Add( &pScope->locSymbols, &sym->id );
  sym->id.parentScope = pScope;
};

//--------------------------------------------------------------------------
// Name         Scope_AddStruct
//
// 
//--------------------------------------------------------------------------
void Scope_AddStruct ( TScope * pScope, TStructDef * sym )
{
  IdentList_Add( &pScope->locStructs, &sym->id );
  sym->id.parentScope = pScope;
};

