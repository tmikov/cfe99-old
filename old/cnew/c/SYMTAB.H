#ifndef SYMTAB_H
#define SYMTAB_H

#include "c-types.h"

extern TScope     * MainScope;
extern TScope     * CurScope;

// A TSymbol that denotes the ELLIPSIS parameter in all function declarations
extern TSymbol * EllipsisSym;


// Declare the memory alocators for all types
//
DECLARE_FIXED_ALLOCATOR( StructDefHeap,    TStructDef );
DECLARE_FIXED_ALLOCATOR( SymbolHeap,       TSymbol );
DECLARE_FIXED_ALLOCATOR( ScopeHeap,        TScope );


void SymTab_Init ( void );
void SymTab_Done ( void );


// scope functions ////////////////////////////////
//

TScope * NewScope (
     enum SCOPE_TYPE scopeType,
     TScope * parent
  );

TScope * EndCurScope ( void );

void AddSymbolToScope (
     TScope * pScope,
     TSymbol * sym
   );

void AddStructToScope (
     TScope * pScope,
     TStructDef * sym
   );

TLocalIdent * _FindLocalIdent (
    TListRoot * pTab
  );

TLocalIdent * _FindScopeIdent (
    TListRoot * pTab,
    TScope * pScope
  );

// symbol functions ///////////////////////////////
//

void AddSymbolToHash (
    TSymbol * pSym
  );

/*
TSymbol * FindLocalSymbol (
    STRING strName
  );
*/
#define FindLocalSymbol( strName ) \
  ((TSymbol *)_FindLocalIdent( &STRING_DATA(strName)->symbols ))


/*
TSymbol * FindScopeSymbol (
    STRING strName,
    TScope * pScope
  );
*/
#define FindScopeSymbol( strName, pScope ) \
  ((TSymbol *)_FindScopeIdent( &STRING_DATA(strName)->symbols, pScope ))

TSymbol * NewSymbol (
    STRING strName
  );

void FreeSymbol (
    TSymbol * sym
  );

// struct functions ///////////////////////////////
//

void AddStructToHash (
    TStructDef * pSym
  );

/*
TStructDef * FindLocalStruct (
    STRING strName
  );
*/
#define FindLocalStruct( strName ) \
  ((TStructDef *)_FindLocalIdent( &STRING_DATA(strName)->structs ))

TStructDef * NewStruct (
    STRING strName
  );


const char * StructName (
    const TStructDef * 
  );

const char * SymbolName (
    const TSymbol * sym
  );


#endif // SYMTAB_H
