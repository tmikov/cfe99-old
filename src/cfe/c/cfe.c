#include "precomp.h"
#include "cfe_priv.h"

struct TCfeRoots CfeRoots;  // CFE GC roots
struct TCfeGlob  CfeGlob;   // Other CFE global data
//--------------------------------------------------------------------------
// Name         Cfe_GcMarkRoots 
//
// Mark the roots of the front-end data.
//--------------------------------------------------------------------------
void Cfe_GcMarkRoots ( void )
{
  unsigned i, j;

  struct TCfeRoots_loc
  {
    const TQualNode * IntegerTypes[it_Last];
    const TQualNode * FloatingTypes[TYPE__BT_LAST][rt_Last];  // real,imaginary,complex * float,double,ldouble
    const TQualNode * VoidType;
    const TQualNode * ErrorType;
    const TQualNode * EllipsisType;
    const TQualNode * CharType;       // signed or unsigned character type
    const TQualNode * ConstCharType;  // const char
    const TQualNode * ConstWCharType; // const wchar_t

    STRING      ErrorName;   
    STRING      EllipsisName;

    TScope * CurScope;
    TScope * GlobalScope;
  };
  MAKE_SURE( sizeof(struct TCfeRoots_loc) == sizeof(struct TCfeRoots) );

  for ( i = 0; i < NELEM(CfeRoots.IntegerTypes); ++i )
    GC_MARK( CfeRoots.IntegerTypes[i] );

  for ( i = 0; i < NELEM(CfeRoots.FloatingTypes); ++i )
    for ( j = 0; j < NELEM(CfeRoots.FloatingTypes[i]); ++j )
      GC_MARK( CfeRoots.FloatingTypes[i][j] );

  GC_MARK( CfeRoots.VoidType );
  GC_MARK( CfeRoots.ErrorType );
  GC_MARK( CfeRoots.EllipsisType );
  GC_MARK( CfeRoots.CharType );
  GC_MARK( CfeRoots.ConstCharType );
  GC_MARK( CfeRoots.ConstWCharType );

  GC_MARK( CfeRoots.ErrorName );
  GC_MARK( CfeRoots.EllipsisName );

  GC_MARK( CfeRoots.CurScope );
  GC_MARK( CfeRoots.GlobalScope );

  CfeConf_GcMarkRoots();

  Strings_GcMark();
};

//--------------------------------------------------------------------------
// Name         Cfe_Init 
//
// Clears the CFE data area (CfeRoots).
//--------------------------------------------------------------------------
void Cfe_Init ( void )
{
  // Clear the global data area
  //
  CLEAR( &CfeRoots ); 

  // Initialize the symbol table
  //
  SymTab_Init();

  // Initialize the target-dependant front-end representation of the types
  // 
  CfeConf_Init();

  // Initialize the C type system. 
  //
  CTypes_Init();

  // Prepare the scanner
  //
  Scanner_Init();

  // Initialize CFE data
  //
  CfeRoots.ErrorName    = NewStringWithData( "?error?" );
  CfeRoots.EllipsisName = NewStringWithData( "..." );
};