#include "port\global.h"
#include "actsym.h"


//////////////////////// public data ///////////////////////////////

TScope * CurScope = NULL;

///////////////////////// local functions /////////////////////////

//--------------------------------------------------------------------------
// Name         ActivateIdent
//
// Adds an identifier to the list, keeping the list sorted by scope nesting
// level. Higher nesting levels are at start of list, smaller at the end.
//
// Note: for debugging purposes we make sure that there is no already inserted
// ident with the same scope nesting. 
//--------------------------------------------------------------------------
static void ActivateIdent (
    TListRoot * pList,
    TIdent * pIdent
  )
{
  int identLevel; // nesting level of the ident being added
  TIdent * pCur;

  identLevel = pIdent->parentScope->level;

  // Skip all idents with greater scope nesting
  //

  for ( pCur = (TIdent *)pList->Flink;
        !END_OF_LIST( pList, pCur ) && pCur->parentScope->level > identLevel;
        pCur = (TIdent *)pCur->nameLink.Flink )
    {};


  ASSERT( END_OF_LIST( pList, pCur ) || pCur->parentScope->level < identLevel ); // they can't be equal

  // Insert before _pCur_
  //
  INSERT_TAIL_LIST( &pCur->nameLink, &pIdent->nameLink );
};

//--------------------------------------------------------------------------
// Name         DeactivateIdent
//
// Remove an ident from the hash table if it is there
//--------------------------------------------------------------------------
static void DeactivateIdent ( TIdent * pIdent )
{
  if (IS_IDENT_HASHED( pIdent ))
  {
    REMOVE_ENTRY_LIST( &pIdent->nameLink );
  }  
};

//--------------------------------------------------------------------------
// Name         DeactivateIdentList
//
// Removes a list of local idents from the hash table
//--------------------------------------------------------------------------
static void DeactivateIdentList ( TIdent * first )
{
  for ( ; first != NULL; first = first->nextLocal )
    DeactivateIdent( first );
};


//--------------------------------------------------------------------------
// Name         FindActiveIdent
//
// Find the most nested ident with this name.
//
// Note: The most nested ident is always on the top.
//--------------------------------------------------------------------------
static TIdent * FindActiveIdent ( TListRoot * pTab )
{
  if (IS_LIST_EMPTY( pTab ))
    return NULL;
  else
    return (TIdent *)pTab->Flink;
};

//--------------------------------------------------------------------------
// Name         FindScopeIdent
//
// Finds an active ident with this name and specified scope.
//--------------------------------------------------------------------------
static TIdent * FindScopeIdent (
    TListRoot * pTab,
    TScope * pScope
  )
{
  TIdent * pCur;
  int scopeLevel;

  scopeLevel = pScope->level;

  // Iterate through all symbols. The scopes are ordered decreasingly.
  // So, as soon as we reach a smaller scope, we can abort the search.
  // 
  for ( pCur = (TIdent *)pTab->Flink;
        !END_OF_LIST( pTab, pCur );
        pCur = (TIdent *)pCur->nameLink.Flink
     )
  {
    int lev;
    ASSERT( pCur->parentScope );

    lev = pCur->parentScope->level;

    if (lev == scopeLevel)
    {
      ASSERT( pCur->parentScope == pScope );
      return pCur;
    }
    else
    if (lev < scopeLevel)
      break;
  }

  return NULL;
};


///////////////////////// public functions /////////////////////////

//--------------------------------------------------------------------------
// Name         ActivateSymbol
//
// 
//--------------------------------------------------------------------------
void ActivateSymbol ( TSymbol * sym )
{
  if (sym->id.strName == NULL)
    return;

  ASSERT( sym->id.strName );
  ASSERT( !IS_IDENT_HASHED( &sym->id ) );

  ActivateIdent( &STRING_DATA(sym->id.strName)->symbols, &sym->id );
};

//--------------------------------------------------------------------------
// Name         ActivateStruct
//
// 
//--------------------------------------------------------------------------
void ActivateStruct ( TStructDef * sym )
{
  if (sym->id.strName == NULL)
    return;

  ASSERT( sym->id.strName );
  ASSERT( !IS_IDENT_HASHED( &sym->id ) );

  ActivateIdent( &STRING_DATA(sym->id.strName)->structs, &sym->id );
};

//--------------------------------------------------------------------------
// Name         FindActiveSymbol
//
// 
//--------------------------------------------------------------------------
TSymbol * FindActiveSymbol ( STRING strName )
{
  return 
    strName 
      ? ID_GET_SYMBOL( FindActiveIdent( &STRING_DATA(strName)->symbols ) )
      : NULL;
};

//--------------------------------------------------------------------------
// Name         FindScopeSymbol
//
// 
//--------------------------------------------------------------------------
TSymbol * FindScopeSymbol ( STRING strName, TScope * pScope )
{
  return 
    strName
      ? ID_GET_SYMBOL( FindScopeIdent( &STRING_DATA(strName)->symbols, pScope ) )
      : NULL;
};

//--------------------------------------------------------------------------
// Name         FindActiveStruct
//
// 
//--------------------------------------------------------------------------
TStructDef * FindActiveStruct ( STRING strName )
{
  return 
    strName 
    ? ID_GET_STRUCT( FindActiveIdent( &STRING_DATA(strName)->structs ) )
    : NULL;
};

//--------------------------------------------------------------------------
// Name         FindScopeStruct
//
// 
//--------------------------------------------------------------------------
TStructDef * FindScopeStruct ( STRING strName, TScope * pScope )
{
  return 
    strName
    ? ID_GET_STRUCT( FindScopeIdent( &STRING_DATA(strName)->structs, pScope ) )
    : NULL;
};

//--------------------------------------------------------------------------
// Name         DeactivateScope
//
// 
//--------------------------------------------------------------------------
void DeactivateScope ( TScope * pScope )
{
  DeactivateIdentList( pScope->locSymbols.first );
  DeactivateIdentList( pScope->locStructs.first );
};

//--------------------------------------------------------------------------
// Name         NewActiveScope 
//
// 
//--------------------------------------------------------------------------
void NewActiveScope ( SCOPE_TYPE scopeType )
{
  CurScope = Scope_New( scopeType, CurScope );
};

//--------------------------------------------------------------------------
// Name         EndCurScope 
//
// 
//--------------------------------------------------------------------------
TScope * EndCurScope ( void )
{
  TScope * res;

  res = CurScope;

  ASSERT( res );

  DeactivateScope( res );
  CurScope = res->parentScope;

  return res;
};


//--------------------------------------------------------------------------
// Name         EndScopesTo 
//
// Close all scopes up to a pre-recorded one. This is used for error 
// recovery.
//--------------------------------------------------------------------------
void EndScopesTo ( TScope * to )
{
  ASSERT( to->level <= CurScope->level );

  while (CurScope != to)
  {
    TRACE1( "EndScopesTo(): closing scope level %d\n", CurScope->level );
    EndCurScope();
  }
};