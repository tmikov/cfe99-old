%{
#include "precomp.h"
#include "cord.h"
#include "heap.h"
#include "comperr.h"

#define YYINITDEPTH 50

#define YYDEBUG 1

#if YYDEBUG
#  define YYERROR_VERBOSE 1
#endif
#define YYSTDERR          stdout

void pperror ( char * err );
int pplex ( void );

#define alloca( n )   gcAlloc( n )

%}

%union 
{
  unsigned long num;
  CORD cord;
}

%token PREPR_LINE PREPR_PRAGMA

%token<num> PREPR_NUMBER
%token<cord> PREPR_ERROR PREPR_STRING

%%

prepr
  : line           
  | PREPR_ERROR    {}
  | PREPR_PRAGMA   {}
  | /* empty */
  ;

line
  : PREPR_LINE PREPR_NUMBER                {}
  | PREPR_LINE PREPR_NUMBER PREPR_STRING   {}
  | PREPR_NUMBER                           {}
  | PREPR_NUMBER PREPR_STRING              {}
  ;

%%

void pperror ( char * err )
{
  ScannerError( "%s", err );
};


extern void Prepr_ScanCord ( CORD cord );

//--------------------------------------------------------------------------
// Name         Prepr_Parse 
//
// 
//--------------------------------------------------------------------------
int Prepr_Parse ( CORD cord )
{
  Prepr_ScanCord( cord );
  return ppparse();
};
