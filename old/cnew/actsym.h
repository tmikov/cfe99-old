#include "strings.h"
#include "csyms.h"
#include "scope.h"

extern TScope * CurScope;

void ActivateSymbol ( TSymbol * sym );
void ActivateStruct ( TStructDef * sym );
void DeactivateScope ( TScope * pScope );

void NewActiveScope ( SCOPE_TYPE scopeType );

TScope * EndCurScope ( void );
void EndScopesTo ( TScope * to );

TSymbol * FindActiveSymbol ( STRING strName );
TSymbol * FindScopeSymbol ( STRING strName, TScope * pScope );
TStructDef * FindActiveStruct ( STRING strName );
TStructDef * FindScopeStruct ( STRING strName, TScope * pScope );

