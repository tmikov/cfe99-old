#ifndef C_SYMTAB_H
#define C_SYMTAB_H

//== STRING ===================================================================

// This is the data that can optionally be associated with a string.
// It contains info relevant for all possible namespaces that an identifier
// can be in and that are active globally at one time.  
//
struct TStringData
{
  int token;          // token code
  TListRoot tags;     // a list of struct/union/enum tags
  TListRoot symbols;  // a list of C symbols (variables and identifiers)
  TListRoot labels;   // a list of labels in a function
};

STRING NewStringWithData ( const char * str );
STRING NewStringLenWithData ( const char * str, size_t len );

//== SymTab ===================================================================

TScope    * SymTab_PushNewScope ( ScopeDisposition_t disposition );
TScope    * SymTab_PopScope ( void );
TSymbol   * SymTab_LookupSymbol ( STRING str );
TTypeNode * SymTab_LookupTag ( STRING str );

void      SymTab_Init ( void );

//== Scope ===================================================================

extern const char * const ScopeDisposition_Name[];

TScope *  New_TScope ( TScope * parent, ScopeDisposition_t disposition );
void      TScope_GcMark ( const TScope * scope );

inline bool Scope_IsIdentList ( const TScope * scope );
TScope  * Scope_Clone ( const TScope * scope );
void      Scope_AddSymbol ( TScope * scope, TSymbol * sym );
void      Scope_MakeSymbolVisible ( TScope * scope, TSymbol * sym );
void      Scope_MakeTagVisible ( TScope * scope, TTypeNode * tag );
void      Scope_AddToIdentTable ( TScope * scope, TSymbol * sym );
void      Scope_RemoveFromIdentTable ( TScope * scope, TSymbol * sym );
TSymbol * Scope_LookupSymbol ( TScope * scope, STRING name );

//--------------------------------------------------------------------------
// Name         Scope_IsIdentList 
//
// 
//--------------------------------------------------------------------------
inline bool Scope_IsIdentList ( const TScope * scope )
{
  return scope == NULL || scope->disposition == SCOPE_IDENT_LIST;
};



#endif // C_SYMTAB_H