#include "precomp.h"
#include "cfe_priv.h"

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
  fprintf( stderr, "\n" );

  va_end( ap );

  abort(); 
  TODO1("a better way to exit");
};

//--------------------------------------------------------------------------
// Name         PrintErrLoc 
//
// 
//--------------------------------------------------------------------------
static void PrintErrLoc ( const YYLTYPE * loc, const char * s )
{
  fprintf( stderr, "%s(%d) : %s", 
    loc->fileName != NULL ? C_STR(loc->fileName) : "stdin:",
    loc->line,
    s
  );
};

//--------------------------------------------------------------------------
// Name         CompilerInfoLoc 
//
// TODO: when the error or warning pertaining to this info is disabled,
// we must not print the info either.
//--------------------------------------------------------------------------
void CompilerInfoLoc ( const YYLTYPE * loc, const char * msg, ... )
{
  va_list ap;
  va_start( ap, msg );

  fprintf( stderr, "     " );
  PrintErrLoc( loc, "" );
  vfprintf( stderr, msg, ap );
  fprintf( stderr, "\n" );

  va_end( ap );
};

//--------------------------------------------------------------------------
// Name         CompilerErrorLoc 
//
// 
//--------------------------------------------------------------------------
void CompilerErrorLoc ( const YYLTYPE * loc, const char * msg, ... )
{
  va_list ap;
  va_start( ap, msg );

  PrintErrLoc( loc, "error: " );
  vfprintf( stderr, msg, ap );
  fprintf( stderr, "\n" );

  va_end( ap );
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

  PrintErrLoc( &yylloc, "error: " );
  vfprintf( stderr, msg, ap );
  fprintf( stderr, "\n" );

  va_end( ap );
};

//--------------------------------------------------------------------------
// Name         CompilerStrictErrorLoc 
//
// 
//--------------------------------------------------------------------------
void CompilerStrictErrorLoc ( const YYLTYPE * loc, const char * msg, ... )
{
  va_list ap;
  va_start( ap, msg );

  PrintErrLoc( loc, bStrictC99 ? "error: " : "warning: " );
  vfprintf( stderr, msg, ap );
  fprintf( stderr, "\n" );

  va_end( ap );
}

//--------------------------------------------------------------------------
// Name         CompilerStrictError 
//
// 
//--------------------------------------------------------------------------
void CompilerStrictError (  const char * msg, ... )
{
  va_list ap;
  va_start( ap, msg );

  PrintErrLoc( &yylloc, bStrictC99 ? "error: " : "warning: " );
  vfprintf( stderr, msg, ap );
  fprintf( stderr, "\n" );

  va_end( ap );
}

//--------------------------------------------------------------------------
// Name         CompilerWarning 
//
// 
//--------------------------------------------------------------------------
void CompilerWarning (  const char * msg, ... )
{
  va_list ap;
  va_start( ap, msg );

  PrintErrLoc( &yylloc, "warning: " );
  vfprintf( stderr, msg, ap );
  fprintf( stderr, "\n" );

  va_end( ap );
};


//--------------------------------------------------------------------------
// Name         ShowPrevDeclaration 
//
// 
//--------------------------------------------------------------------------
void ShowPrevDeclaration ( const TSymbol * sym )
{
  if (sym->declLoc.line != 0)                             
  {                                                                 
    TST,CompilerInfoLoc( &sym->declLoc,                       
            "See previous declaration of '%s'", Symbol_Name(sym) );      
  }                                                                 
};

