#include "precomp.h"
#include "comperr.h"
#include "scan.h"

//--------------------------------------------------------------------------
// Name         FatalError 
//
// 
//--------------------------------------------------------------------------
void FatalError ( const char * msg, ... )
{
  va_list ap;
  va_start( ap, msg );

  fprintf( stderr, "** Fatal error: " );
  vfprintf( stderr, msg, ap );
  printf( "\n" );

  va_end( ap );

  abort(); // TODO:
};

//--------------------------------------------------------------------------
// Name         CompilerError 
//
// 
//--------------------------------------------------------------------------
void CompilerError (  const char * msg, ... )
{
  va_list ap;
  va_start( ap, msg );

  fprintf( stderr, "Error in line %d: ", yycurline );
  vfprintf( stderr, msg, ap );
  printf( "\n" );

  va_end( ap );
};

//--------------------------------------------------------------------------
// Name         CompilerWarning 
//
// 
//--------------------------------------------------------------------------
void CompilerWarning (  const char * msg, ... )
{
  va_list ap;
  va_start( ap, msg );

  fprintf( stderr, "Warning in line %d: ", yycurline );
  vfprintf( stderr, msg, ap );
  printf( "\n" );

  va_end( ap );
};
