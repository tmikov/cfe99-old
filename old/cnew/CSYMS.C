#include "port\global.h"
#include "csyms.h"
#include "misc\arena.h"

extern TArena GlobalArena;   // a global arena used for the strings as well

//--------------------------------------------------------------------------
// Name         StorageClass_New
//
// 
//--------------------------------------------------------------------------
TStorageClass * StorageClass_New ( void )
{
  TStorageClass * res;
  ARENA_NEW0( &GlobalArena, res );
  return res;
};

//--------------------------------------------------------------------------
// Name         StorageClass_Free
//
// 
//--------------------------------------------------------------------------
void StorageClass_Free ( TStorageClass * p )
{
};

//--------------------------------------------------------------------------
// Name         Ident_Name
//
// 
//--------------------------------------------------------------------------
const char * Ident_Name ( const TIdent * id )
{
  return id->strName ? C_STR(id->strName) : "<unnamed>";
};

//--------------------------------------------------------------------------
// Name         Symbol_New
//
// 
//--------------------------------------------------------------------------
TSymbol * Symbol_New ( STRING strName )
{
  TSymbol * pnew;
  ARENA_NEW0( &GlobalArena, pnew );
  pnew->id.strName = strName;
  return pnew;
};

//--------------------------------------------------------------------------
// Name         Symbol_Free
//
// 
//--------------------------------------------------------------------------
void Symbol_Free ( TSymbol * sym )
{
  ASSERT( sym );
  ASSERT( !IS_IDENT_HASHED(&sym->id) ); // make sure it is not hashed
};

//--------------------------------------------------------------------------
// Name         Struct_New
//
// 
//--------------------------------------------------------------------------
TStructDef * Struct_New ( STRING strName )
{
  TStructDef * pnew;
  ARENA_NEW0( &GlobalArena, pnew );
  pnew->id.strName   = strName;
  return pnew;
};

//--------------------------------------------------------------------------
// Name         Struct_Free
//
// 
//--------------------------------------------------------------------------
void Struct_Free ( TStructDef * sym )
{
  ASSERT( sym );
  ASSERT( !IS_IDENT_HASHED(&sym->id) ); // make sure it is not hashed
};

#ifdef _DEBUG

// These two are wrappers for type-safety when compiling a debug version

TSymbol * IdGetSymbol ( TIdent * id )
{
  return (TSymbol *)id;
};

TStructDef * IdGetStruct ( TIdent * id )
{
  return (TStructDef *)id;
};

#endif

