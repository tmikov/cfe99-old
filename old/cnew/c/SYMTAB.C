#include "port\global.h"
#include "symtab.h"

///////////////// Global data /////////////////////////////////

// Implement the allocators for the various data structures that
// this module supports
//

//TODO: supply proper values for NumInPage
IMPLEMENT_FIXED_ALLOCATOR( StructDefHeap,    TStructDef,    1024 );
IMPLEMENT_FIXED_ALLOCATOR( SymbolHeap,       TSymbol,       1024 );
IMPLEMENT_FIXED_ALLOCATOR( ScopeHeap,        TScope,        1024 );

TScope     * MainScope = NULL;
TScope     * CurScope; 

TSymbol    * EllipsisSym;

////////////////////////// Global functions /////////////////////////

//--------------------------------------------------------------------------
// Name         SymTab_Init
//
//
//--------------------------------------------------------------------------
void SymTab_Init ( void )
{
  StructDefHeap_Init();
  SymbolHeap_Init();
  ScopeHeap_Init();

  MainScope = NewScope( FILE_SCOPE, NULL );
  CurScope = MainScope;

  EllipsisSym = NewSymbol( NULL );
};

//--------------------------------------------------------------------------
// Name         SymTab_Done
//
//
//--------------------------------------------------------------------------
void SymTab_Done ( void )
{
  StructDefHeap_Done();
  SymbolHeap_Done();
  ScopeHeap_Done();
};

//--------------------------------------------------------------------------
// Name         NewScope
//
// Allocates a new scope. If a parent is supplied, inserts in the the
// parent's list.
//--------------------------------------------------------------------------
TScope * NewScope ( enum SCOPE_TYPE scopeType, TScope * parent )
{
  TScope * pScope = ScopeHeap_CAlloc();

  // if there is a parent, initialize the relevant information
  //
  if (parent)
  {
//    INSERT_TAIL_LIST( &parent->subScopes, &pScope->scopeLink );
    pScope->level = parent->level + 1;
    pScope->parentScope = parent;
  }

  pScope->scopeType = scopeType;

//  INITIALIZE_LIST_HEAD( &pScope->subScopes );

  return pScope;
};

//--------------------------------------------------------------------------
// Name         UnhashIdent
//
// Remove an ident from the hash table if it is there
//--------------------------------------------------------------------------
static void UnhashIdent ( TLocalIdent * pIdent )
{
  if (IS_IDENT_HASHED( pIdent ))
  {
    REMOVE_ENTRY_LIST( &pIdent->nameLink );
  }  
};

//--------------------------------------------------------------------------
// Name         UnhashLocalIdents
//
// Removes a list of local idents from the hash table
//--------------------------------------------------------------------------
static void UnhashLocalIdents ( TLocalIdent * first )
{
  for ( ; first != NULL; first = first->nextLocal )
    UnhashIdent( first );
};

//--------------------------------------------------------------------------
// Name         EndCurScope
//
// Pops to the previous scope that was active. 
// This consists of removing all identifiers local for the scope, from
// the hash tables.
//
// returns the scope that is being ended.
//
// Note: this doesn't free the current scope. Only makes it "inactive".
//--------------------------------------------------------------------------
TScope * EndCurScope ( void )
{
  TScope * res;

  ASSERT( CurScope );

  res = CurScope; // save the result

  // Remove all items identifiers from the hash tables:
  //
  // the symbols
  UnhashLocalIdents( (TLocalIdent *)res->locSymbols.first );
  // the structs
  UnhashLocalIdents( (TLocalIdent *)res->locStructs.first ); 


  // Move to the previous scope
  CurScope = res->parentScope;

  return res;
};

//--------------------------------------------------------------------------
// Name         EndScopesUpTo
//
// Ends all scopes until _scope_ is reached.
//
// This function may be useful when recovering from parse errors.
//--------------------------------------------------------------------------
void EndScopesUpTo ( TScope * scope )
{
  ASSERT( scope == NULL && CurScope == NULL ||
          scope == NULL && CurScope != NULL ||
          scope != NULL && CurScope != NULL && scope->level <= CurScope->level );

  while (CurScope != scope)
    EndCurScope();
};

//--------------------------------------------------------------------------
// Name         AddLocalId
//
//
//--------------------------------------------------------------------------
static void AddLocalId (
    struct TLocIdentList * pList,
    TLocalIdent * sym
  )
{
  sym->nextLocal = NULL;

  if (pList->last != NULL) // if the list isn't empty
    pList->last->nextLocal = sym;
  else
    pList->first = sym;

  pList->last = sym;
};

//--------------------------------------------------------------------------
// Name         AddSymbolToScope
//
// Adds a symbol to the list of local symbols inside a scope.
//--------------------------------------------------------------------------
void AddSymbolToScope (
     TScope * pScope,
     TSymbol * sym
   )
{
  AddLocalId( (struct TLocIdentList *)&pScope->locSymbols, (TLocalIdent *)sym );
  sym->parentScope = pScope;
};

//--------------------------------------------------------------------------
// Name         AddStructToScope
//
// Adds a struct to the list of local structs inside a scope.
//--------------------------------------------------------------------------
void AddStructToScope (
     TScope * pScope,
     TStructDef * sym
   )
{
  AddLocalId( (struct TLocIdentList *)&pScope->locStructs, (TLocalIdent *)sym );
  sym->parentScope = pScope;
};

//--------------------------------------------------------------------------
// Name         NewSymbol
//
// Allocates a new symbol.
//
// Note: it is allowed to pass NULL for szName.
//--------------------------------------------------------------------------
TSymbol * NewSymbol (
    STRING strName
  )
{
  TSymbol * pnew = SymbolHeap_CAlloc();

  pnew->strName  = strName;

  return pnew;
};

//--------------------------------------------------------------------------
// Name         NewStruct
//
// Allocates a new structdef.
//
//--------------------------------------------------------------------------
TStructDef * NewStruct (
    STRING strName
  )
{
  TStructDef * pnew = StructDefHeap_CAlloc();

  pnew->strName   = strName;

  return pnew;
};

//--------------------------------------------------------------------------
// Name         FreeSymbol
//
// Frees a symbol and its name.
//--------------------------------------------------------------------------
void FreeSymbol (
    TSymbol * sym
  )
{
  ASSERT( sym );
  ASSERT( !IS_IDENT_HASHED(sym) ); // make sure it is not hashed

  SymbolHeap_Free( sym );
};

//--------------------------------------------------------------------------
// Name         StructName 
//
// returns the name of a struct or "<noname>" if it is anonymous
//--------------------------------------------------------------------------
const char * StructName ( const TStructDef * sDef )
{
  return sDef->strName ? C_STR(sDef->strName) : "<noname>";
};

//--------------------------------------------------------------------------
// Name         SymbolName
//
// 
//--------------------------------------------------------------------------
const char * SymbolName (
    const TSymbol * sym
  )
{
  return sym->strName ? C_STR(sym->strName) : "";
};

//--------------------------------------------------------------------------
// Name         AddIdentToHash
//
// Adds an identifier to the list, keeping the list sorted by scope nesting
// level. Higher nesting levels are at start of list, smaller at the end.
//
// Note: for debugging purposes we make sure that there is no already inserted
// ident with the same scope nesting. 
//--------------------------------------------------------------------------
static void AddIdentToHash (
    TListRoot * pList,
    TLocalIdent * pIdent
  )
{
  int identLevel; // nesting level of the ident being added
  TLocalIdent * pCur;

  identLevel = pIdent->parentScope->level;

  // Skip all idents with greater scope nesting
  //

  for ( pCur = (TLocalIdent *)pList->Flink;
        !END_OF_LIST( pList, pCur ) && pCur->parentScope->level > identLevel;
        pCur = (TLocalIdent *)pCur->nameLink.Flink )
    {};


  ASSERT( END_OF_LIST( pList, pCur ) || pCur->parentScope->level < identLevel ); // they can't be equal

  // Insert before _pCur_
  //
  INSERT_TAIL_LIST( &pCur->nameLink, &pIdent->nameLink );
};

//--------------------------------------------------------------------------
// Name         AddSymbolToHash
//
// 
//--------------------------------------------------------------------------
void AddSymbolToHash (
    TSymbol * pSym
  )
{
  if (pSym->strName == NULL)
    return;

  ASSERT( pSym->strName );
  ASSERT( !IS_IDENT_HASHED( pSym ) );

  AddIdentToHash( &STRING_DATA(pSym->strName)->symbols, (TLocalIdent *)pSym );
};

//--------------------------------------------------------------------------
// Name         AddStructToHash
//
// 
//--------------------------------------------------------------------------
void AddStructToHash (
    TStructDef * pSym
  )
{
  if (pSym->strName == NULL)
    return;

  ASSERT( pSym->strName );
  ASSERT( !IS_IDENT_HASHED( pSym ) );

  AddIdentToHash( &STRING_DATA(pSym->strName)->structs, (TLocalIdent *)pSym );
};

//--------------------------------------------------------------------------
// Name         FindLocalIdent
//
// Find the most nested ident with this name.
//
// Note: The most nested ident is always on the top.
//--------------------------------------------------------------------------
TLocalIdent * _FindLocalIdent (
    TListRoot * pTab
  )
{
  if (IS_LIST_EMPTY( pTab ))
    return NULL;
  else
    return (TLocalIdent *)pTab->Flink;
};

//--------------------------------------------------------------------------
// Name         FindScopeIdent
//
// Finds an ident with this name and specified scope.
//--------------------------------------------------------------------------
TLocalIdent * _FindScopeIdent (
    TListRoot * pTab,
    TScope * pScope
  )
{
  TLocalIdent * pCur;
  int scopeLevel;

  scopeLevel = pScope->level;

  // Iterate through all symbols. The scopes are ordered decreasingly.
  // So, as soon as we reach a smaller scope, we can abort the search.
  // 
  for ( pCur = (TLocalIdent *)pTab->Flink;
        !END_OF_LIST( pTab, pCur );
        pCur = (TLocalIdent *)pCur->nameLink.Flink
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

