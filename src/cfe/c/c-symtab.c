/*
  c-symtab.c
  Maintain the C frontend symbol table, observing scoping rules, etc.

 */
#include "precomp.h"
#include "cfe_priv.h"
#include "c-symtab.h"

//== STRING ===================================================================

//--------------------------------------------------------------------------
// Name         TStringData_GcMark 
//
// 
//--------------------------------------------------------------------------
static void TStringData_GcMark ( const TStringData * data )
{
  struct TStringData_loc
  {
    int token;          // token code
    TListRoot tags;     // a list of struct/union/enum tags
    TListRoot symbols;  // a list of C symbols (variables and identifiers)
    TListRoot labels;   // a list of labels in a function
  };
  MAKE_SURE( sizeof(struct TStringData) == sizeof(struct TStringData_loc) );

  TListRoot_GcMark( &data->tags );
  TListRoot_GcMark( &data->symbols );
  TListRoot_GcMark( &data->labels );
};

//--------------------------------------------------------------------------
// Name         StringData_Init 
//
// 
//--------------------------------------------------------------------------
static void TStringData_Init ( TStringData * data )
{
  data->token = 0;
  INITIALIZE_LIST_HEAD( &data->tags );
  INITIALIZE_LIST_HEAD( &data->symbols );
  INITIALIZE_LIST_HEAD( &data->labels );
};

//--------------------------------------------------------------------------
// Name         NewStringLenWithData 
//
// 
//--------------------------------------------------------------------------
STRING NewStringLenWithData ( const char * str, size_t len )
{
  STRING res = NewStringLen( str, len );
  ASSERT( res ); // We use an abortive allocator

  // if no data has already been allocated to this string, allocate it here
  //
  if (STRING_DATA( res ) == NULL)
  {
    TStringData * data = GC_NEW( TStringData );
    ASSERT( data ); // We use an abortive allocator

    TStringData_Init( data );

    STRING_SET_DATA( res, data );
  }

  return res;
};

//--------------------------------------------------------------------------
// Name         NewStringWithData 
//
// Allocates a string 
//--------------------------------------------------------------------------
STRING NewStringWithData ( const char * str )
{
  return NewStringLenWithData( str, strlen( str ) );
};

//== TScope ===================================================================

const char * const ScopeDisposition_Name[] = 
{
  "file",
  "block",
  "struct",
  "params",
  "ident_list"
};

struct TScope_loc
{
  TScope * parentScope;

  ScopeDisposition_t disposition;
  bool bHaveEllipsis : 1;

  TLabel    * pFirstLabel,  * pLastLabel;
  TSymbol   * pFirstSymbol, * pLastSymbol; // symbols in the scope
  TTypeNode * pFirstTag,    * pLastTag;    // tags in the scope

  TIdentTab * idTab;
};

//--------------------------------------------------------------------------
// Name         InsertIdentInList 
//
// 
//--------------------------------------------------------------------------
static void InsertIdentInList ( TIdent ** ppFirst, TIdent ** ppLast, TIdent * id )
{
  TIdent * pLast;

  // Must not already be in a list 
  ASSERT( id->scope == NULL );

  pLast = *ppLast; // cache the value

  if (pLast == NULL)
    *ppFirst = id;
  else
    pLast->pNext = id;

  id->pPrev = pLast;
  *ppLast = id;
};

//--------------------------------------------------------------------------
// Name         RemoveIdentFromList 
//
// 
//--------------------------------------------------------------------------
static void RemoveIdentFromList ( TIdent ** ppFirst, TIdent ** ppLast, TIdent * id )
{
  if (id->pPrev != NULL)
    id->pPrev->pNext = id->pNext;
  else
    *ppFirst = id->pNext;

  if (id->pNext != NULL)
    id->pNext->pPrev = id->pPrev;
  else
    *ppLast = id->pPrev;
};


//--------------------------------------------------------------------------
// Name         TScope_GcMark 
//
// 
//--------------------------------------------------------------------------
void TScope_GcMark ( const TScope * scope )
{
  MAKE_SURE( sizeof(struct TScope) == sizeof(struct TScope_loc) );

  GC_MARK( scope->parentScope );
  GC_MARK( scope->pFirstLabel );
  GC_MARK( scope->pLastLabel );
  GC_MARK( scope->pFirstSymbol );
  GC_MARK( scope->pLastSymbol );
  GC_MARK( scope->pFirstTag );
  GC_MARK( scope->pLastTag );
  GC_MARK( scope->idTab );
};

//--------------------------------------------------------------------------
// Name         New_Scope 
//
// 
//--------------------------------------------------------------------------
TScope * New_TScope ( TScope * parent, ScopeDisposition_t disposition )
{
  TScope * res = GC_NEW( TScope );
  ASSERT( res ); // we use an abortive allocator

  res->parentScope = parent;
  res->disposition = disposition;

  return res;
};

//--------------------------------------------------------------------------
// Name         Scope_Clone 
//
// Does not copy the ident lists and the ident tab
//--------------------------------------------------------------------------
TScope * Scope_Clone ( const TScope * scope )
{
  TScope * res = GC_NEW( TScope );

  MAKE_SURE( sizeof(struct TScope) == sizeof(struct TScope_loc) );

  ASSERT( scope != NULL );

  *res = *scope;
  
  res->pFirstLabel  = res->pLastLabel  = NULL;
  res->pFirstSymbol = res->pLastSymbol = NULL;
  res->pFirstTag    = res->pLastTag    = NULL;

  res->idTab = NULL;

  return res;
};

//--------------------------------------------------------------------------
// Name         Scope_AddSymbol 
//
// Add a symbol to the scope symbol list
//--------------------------------------------------------------------------
void Scope_AddSymbol ( TScope * scope, TSymbol * sym )
{
  // The symbol must not already be inserted in another scope.
  ASSERT( sym->id.scope == NULL );

  InsertIdentInList( 
    (TIdent **)&scope->pFirstSymbol, 
    (TIdent **)&scope->pLastSymbol, 
    &sym->id 
  );
  sym->id.scope = scope;
};

#ifdef _DEBUG
//--------------------------------------------------------------------------
// Name         Scope_IsActive 
//
// Verifies whether a scope is on the active chain
//--------------------------------------------------------------------------
static bool Scope_IsActive ( TScope * scope )
{
  TScope * cur;

  for ( cur = CfeRoots.CurScope; cur != NULL; cur = cur->parentScope )
  {
    if (cur == scope)
      return true;
  }

  return false;
};
#endif

//--------------------------------------------------------------------------
// Name         Scope_MakeSymbolVisible
//
// 
//--------------------------------------------------------------------------
void Scope_MakeSymbolVisible ( TScope * scope, TSymbol * sym )
{
  // Only an active scope can make tags visible
  //
  ASSERT( Scope_IsActive( scope ) );

  // The symbol should not already be visible
  //
  ASSERT( !Symbol_IsVisible( sym ) );

  // It must not already be inserted in another scope.
  // It could be already in this scope.
  ASSERT( sym->id.scope == NULL || sym->id.scope == scope );

  // Insert in the scope, if it isn't already there
  // (Q: when could it already be there ?)
  if (sym->id.scope == NULL)
    Scope_AddSymbol( scope, sym );

  // Make visible
  //
  if (sym->id.strName != NULL)
  {
    TStringData * strData = STRING_DATA( sym->id.strName );
    ASSERT( strData != NULL );

    INSERT_TAIL_LIST( &strData->symbols, &sym->id.link );
  }
};

//--------------------------------------------------------------------------
// Name         Scope_AddTag 
//
// 
//--------------------------------------------------------------------------
static void Scope_AddTag ( TScope * scope, TTypeNode * tag )
{
  // The tag must not already be inserted in another scope.
  ASSERT( tag->id.scope == NULL );

  InsertIdentInList( 
    (TIdent **)&scope->pFirstTag, 
    (TIdent **)&scope->pLastTag, 
    &tag->id 
  );
  tag->id.scope = scope;
}

//--------------------------------------------------------------------------
// Name         Scope_MakeTagVisible 
//
// 
//--------------------------------------------------------------------------
void Scope_MakeTagVisible ( TScope * scope, TTypeNode * tag )
{
  // Only an active scope can make tags visible
  //
  ASSERT( Scope_IsActive( scope ) );

  // The symbol should not already be visible
  //
  ASSERT( !Ident_IsVisible( &tag->id ) );

  // Insert in the scope, if it isn't already there
  //
  Scope_AddTag( scope, tag );

  // Make visible
  //
  if (tag->id.strName != NULL)
  {
    TStringData * strData = STRING_DATA( tag->id.strName );
    ASSERT( strData != NULL );

    INSERT_TAIL_LIST( &strData->tags, &tag->id.link );
  }
};

//--------------------------------------------------------------------------
// Name         Scope_AddToIdentTable 
//
// 
//--------------------------------------------------------------------------
void Scope_AddToIdentTable ( TScope * scope, TSymbol * sym )
{
  // The symbol must belong to the same scope
  //
  ASSERT( sym->id.scope == scope );

  // Must not already be in an ident table
  //
  ASSERT( sym->identNode == NULL );

  // The first time create the ident table
  if (scope->idTab == NULL)
    scope->idTab = New_IdentTab();

  // Must never insert the same symbol twice
  ASSERT( IdentTab_Find( scope->idTab, sym->id.strName, NULL ) == NULL );

  IdentTab_Insert( scope->idTab, &sym->id, &sym->identNode );
};

//--------------------------------------------------------------------------
// Name         Scope_RemoveFromIdentTable 
//
// 
//--------------------------------------------------------------------------
void Scope_RemoveFromIdentTable ( TScope * scope, TSymbol * sym )
{
  ASSERT( scope->idTab != NULL );
  ASSERT( sym->identNode != NULL );

  IdentTab_Remove( scope->idTab, sym->identNode );
  sym->identNode = NULL;
}

//--------------------------------------------------------------------------
// Name         Scope_LookupSymbol
//
// Lookate a symbol in the scope's ident table.
//
// name == NULL is valid, in which case we return NULL.
//--------------------------------------------------------------------------
TSymbol * Scope_LookupSymbol ( TScope * scope, STRING name )
{
  return (TSymbol *)IdentTab_Find( scope->idTab, name, NULL );
};

/*
//--------------------------------------------------------------------------
// Name         Scope_Done
//
// 
//--------------------------------------------------------------------------
void Scope_Done ( TScope * scope )
{
  if (scope == NULL)
    return;

  // If we are destroying the active scope, pop it
  if (scope == CfeRoots.CurScope)
  {
    TRACE0( "Warning: Scope_Done() called on the active scope\n" );
    SymTab_PopScope();
  }
  else
    HideScope( scope );

  // Free the ident table
  IdentTab_Done( scope->idTab );

  // Clear all dependancies so that GC can free all we have allocated, even
  // if we can't yet free us
  //
  CLEAR( scope );
};
*/

//== SymTab ===================================================================

//--------------------------------------------------------------------------
// Name         HideIdentList 
//
// Removes a list of identifiers from the visible name map by removing
// each one from the string's namespace.
//--------------------------------------------------------------------------
static void HideIdentList ( TIdent * id )
{
  for ( ; id != NULL; id = id->pNext )
  {
    if (id->link.Flink != NULL)
    {
      REMOVE_ENTRY_LIST( &id->link );
      id->link.Flink = NULL;
    }
  }
};

//--------------------------------------------------------------------------
// Name         HideScope 
//
// Remove all scope symbols from their namespaces
//--------------------------------------------------------------------------
static void HideScope ( TScope * scope )
{
  HideIdentList( &scope->pFirstLabel->id );
  HideIdentList( &scope->pFirstSymbol->id );
  HideIdentList( &scope->pFirstTag->id );
};

//--------------------------------------------------------------------------
// Name         SymTab_PushNewScope 
//
// Creates a new scope and makes it the current one
//--------------------------------------------------------------------------
TScope * SymTab_PushNewScope ( ScopeDisposition_t disposition )
{
  TScope * res = New_TScope( CfeRoots.CurScope, disposition );
  return CfeRoots.CurScope = res;
};

//--------------------------------------------------------------------------
// Name         SymTab_PopScope 
//
// 
//--------------------------------------------------------------------------
TScope * SymTab_PopScope ( void )
{
  TScope * res;

  ASSERT( CfeRoots.CurScope != NULL );
  res = CfeRoots.CurScope;
  HideScope( res );
  CfeRoots.CurScope = res->parentScope;

  return res;
};

//--------------------------------------------------------------------------
// Name         SymTab_LookupSymbol
//
// 
//--------------------------------------------------------------------------
TSymbol * SymTab_LookupSymbol ( STRING str )
{
  if (str != NULL)
  {
    const TStringData * strData = STRING_DATA( str );

    if (!IS_LIST_EMPTY( &strData->symbols ))
      return (TSymbol *)strData->symbols.Blink;
  }

  return NULL;
};

//--------------------------------------------------------------------------
// Name         SymTab_LookupTag 
//
// 
//--------------------------------------------------------------------------
TTypeNode * SymTab_LookupTag ( STRING str )
{
  if (str != NULL)
  {
    const TStringData * strData = STRING_DATA( str );

    if (!IS_LIST_EMPTY( &strData->tags ))
      return (TTypeNode *)strData->tags.Blink;
  }

  return NULL;
};

//--------------------------------------------------------------------------
// Name         SymTab_Init 
//
// 
//--------------------------------------------------------------------------
void SymTab_Init ( void )
{
  MAKE_SURE( NELEM(ScopeDisposition_Name) == SCOPE_DISPOSITION_LAST );

  CfeRoots.CurScope = NULL;

  // Create the main scope
  //
  CfeRoots.GlobalScope = SymTab_PushNewScope( SCOPE_FILE );
};


