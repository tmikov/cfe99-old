/*
  An implementation and target specific module. It defines the various
  __declspec and __attr identifiers.
*/
#include "port\global.h"
#include "strings.h"

struct TDeclSpec
{
  const char * szName;
  unsigned   flag;
  STRING     strName;
};

static struct TDeclSpec DeclSpecs[] =
{
  { "dllimport",        1 },
  { "dllexport",        2 },
  { "naked",            4 },
  { "noreturn",         8 },
};

static struct TDeclSpec Attrs[] =
{
  { "cdecl",            1 },
  { "stdcall",          2 },
  { "pascal",           4 },
  { "far",              8 },
  { "near",            16 },

  // for 8051
  // xseg, cseg, dseg, etc.
};

//--------------------------------------------------------------------------
// Name         CmpDeclSpecs
//
//
//--------------------------------------------------------------------------
static int _cdecl CmpDeclSpecs ( const void * _a, const void * _b )
{
  const struct TDeclSpec * const a = (const struct TDeclSpec * )_a;
  const struct TDeclSpec * const b = (const struct TDeclSpec * )_b;

  if (a->strName < b->strName)
    return -1;
  else
  if (a->strName > b->strName)
    return 1;
  else
    return 0;
};

//--------------------------------------------------------------------------
// Name         DeclSpec_Init
//
// Calculates the hash values and sorts the tables with declspec-s and
// attributes.
//--------------------------------------------------------------------------
void DeclSpec_Init ( void )
{
  int i;

  // First calculate the hash values
  //
  for ( i = 0; i < _countof( DeclSpecs ); ++i )
    DeclSpecs[i].strName = NewString( DeclSpecs[i].szName );

  for ( i = 0; i < _countof( Attrs ); ++i )
    Attrs[i].strName = NewString( Attrs[i].szName );

  // Then sort the tables
  //
  qsort( DeclSpecs, _countof( DeclSpecs ), sizeof( DeclSpecs[0] ), CmpDeclSpecs );
  qsort( Attrs, _countof( Attrs ), sizeof( Attrs[0] ), CmpDeclSpecs );
};

void DeclSpec_Done ( void )
{
};

//--------------------------------------------------------------------------
// Name         FindDeclSpec
//
// Returns 0 on error
//--------------------------------------------------------------------------
unsigned FindDeclSpec ( STRING strName )
{
  struct TDeclSpec key, * found;

  key.strName = strName;

  found = (struct TDeclSpec *)bsearch( &key, DeclSpecs, _countof( DeclSpecs ), 
                                       sizeof( DeclSpecs[0] ), CmpDeclSpecs );

  return found ? found->flag : 0;
};

//--------------------------------------------------------------------------
// Name         FindAttr
//
//
//--------------------------------------------------------------------------
unsigned FindAttr ( STRING strName )
{
  struct TDeclSpec key, * found;

  key.strName = strName;

  found = (struct TDeclSpec *)bsearch( &key, Attrs, _countof( Attrs ), 
                                       sizeof( Attrs[0] ), CmpDeclSpecs );

  return found ? found->flag : 0;
};
