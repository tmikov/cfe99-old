#include "port\global.h"
#include "comp.h"
#include "parsetyp.h"
#include "declspec.h"

TArena GlobalArena;

// private externs (that are not declared in header files because
// they are used only here)
//
extern int yycurline; // current line
void yyparse ( void );

// global data
//
struct TCompileOptions * opt = NULL;

// Local data
//
static char * pszCurFileName = "file"/*NULL*/;

static bool bAnyErrors = false;
static int  NumErrors = 0;
static int  NumWarnings = 0;

// Error routines ////////////////

//--------------------------------------------------------------------------
// Name         VPrintError
//
//
//--------------------------------------------------------------------------
static void VPrintError ( enum ERROR_SEVERITY severity, const char * msg, va_list ap )
{
  static char * szErrTitle[] =
  {
    "Fatal  ",
    "Error  ",
    "Info   ",
    "Warning"
  };

  if (severity < ERR_WARN0)
    bAnyErrors = true;

  if (pszCurFileName != NULL)
  {
    printf( "%s %s %d: ",
            severity < _countof( szErrTitle ) ?
               szErrTitle[ severity ] :
               szErrTitle[ _countof( szErrTitle ) - 1 ],
            pszCurFileName, yycurline );
  }
  else
  {
    printf( "%s: ",
            severity < _countof( szErrTitle ) ?
            szErrTitle[ severity ] :
            szErrTitle[ _countof( szErrTitle ) - 1 ] );
  }

  vprintf( msg, ap );

/* Print current function
  if (pszCurFileName != NULL && pCurFunc && pCurFunc->pFuncIdent->szName &&
      pCurFunc->pFuncIdent->szName[0])
  {
    printf( " in function %s()", pCurFunc->pFuncIdent->szName );
  }
*/

  putchar( '\n' );

  va_end( ap );

  NumErrors   += severity == ERR_ERROR;
  NumWarnings += severity == ERR_WARN0;

  if (severity == ERR_FATAL)
    ErrorJump( 1 );

  if (NumErrors >= opt->maxErrors)
    Fatal( "Too many errors" );
  if (NumWarnings >= opt->maxWarnings)
    Fatal( "Too many warnings" );
};


//--------------------------------------------------------------------------
// Name         PrintErrorStr
//
//
//--------------------------------------------------------------------------
void PrintErrorStr ( int severity, const char * msg, ... )
{
  va_list ap;
  va_start( ap, msg );
  VPrintError( severity, msg, ap );
  va_end( ap );
};

//--------------------------------------------------------------------------
// Name         Error
//
//
//--------------------------------------------------------------------------
void Error ( const char * fmt, ... )
{
  va_list ap;
  va_start( ap, fmt );
  VPrintError( ERR_ERROR, fmt, ap );
  va_end( ap );
};

//--------------------------------------------------------------------------
// Name         Warning
//
//
//--------------------------------------------------------------------------
void Warning ( const char * fmt, ... )
{
  va_list ap;
  va_start( ap, fmt );
  VPrintError( ERR_WARN0, fmt, ap );
  va_end( ap );
};

//--------------------------------------------------------------------------
// Name         Fatal
//
//
//--------------------------------------------------------------------------
void Fatal ( const char * fmt, ... )
{
  va_list ap;
  va_start( ap, fmt );
  VPrintError( ERR_FATAL, fmt, ap );
  va_end( ap );
};

//--------------------------------------------------------------------------
// Name         OutOfMemory
//
// Called by xheap when there is no more memory.
//--------------------------------------------------------------------------
void OutOfMemory ( void )
{
  Fatal( strerror( ENOMEM ) );
};

//////////////////////////////

//--------------------------------------------------------------------------
// Name         Init
//
//
//--------------------------------------------------------------------------
static void Init ( void )
{
  Arena_Init( &GlobalArena, 16384 );

  Strings_Init();
  DeclSpec_Init();
  SymTab_Init();
  ParseType_Init();
};

//--------------------------------------------------------------------------
// Name         Done
//
//
//--------------------------------------------------------------------------
static void Done ( void )
{
  ParseType_Done();
  SymTab_Done();
  Strings_Done();

  Arena_Done( &GlobalArena );
};

void Compile ( void )
{
  if (setjmp( ErrJmpBuf ) == 0)
  {
    Init();
    yyparse();
  }
  Done();
};
