#ifndef SCOPE_H
#define SCOPE_H

#include "csyms.h" // symbols
#include "misc\fixalloc.h"

typedef struct TIdentList TIdentList;
struct TIdentList
{
  TIdent * first, * last;
};

typedef enum SCOPE_TYPE SCOPE_TYPE;
enum SCOPE_TYPE
{
  STRUCT_SCOPE,     // this is a scope inside a struct 
  FILE_SCOPE,       // this is the global scope
  FUNCTION_SCOPE,   // this is a scope inside a function
  FUNC_PARAM_SCOPE, // this is a scope iside function parameter declaration
};

// A macro that returns true if a struct declared in a scope should be
// local for that scope
// (for C++ it is always TRUE)
//
#define LOCAL_STRUCTS( scopeType ) ((scopeType) != STRUCT_SCOPE)

// Array with the default storage and output class for each
// scope type
//
extern TStorageClass DefaultScopeStorageClass[4];

typedef struct TScope TScope;
struct TScope
{
  TListEntry      scopeLink;      // link in parent scope
  TScope          * parentScope;  // pointer to parent scope
  int             level;          // scope nesting level. 0 for file scope
  TListRoot       subScopes;      // list of all sub-scopes
  SCOPE_TYPE      scopeType;
  TIdentList      locSymbols;     // local symbols
  TIdentList      locStructs;     // local struct/union/enum

  union
  {
    TYPE * funcNode; // points to the function node whose parameters
                     // are being parsed at the moment
  } info;
};

TScope * Scope_New ( SCOPE_TYPE scopeType, TScope * parent );
void     Scope_Free ( TScope * pScope );

void Scope_AddSymbol ( TScope * pScope, TSymbol * sym );
void Scope_AddStruct ( TScope * pScope, TStructDef * sym );

void IdentList_Add ( TIdentList * pList, TIdent * id );

#endif // SCOPE_H

