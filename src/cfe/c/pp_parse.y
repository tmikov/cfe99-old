%{
#include "precomp.h"
#include "cfe_priv.h"
#include "pp_parse.h"

#define YYDEBUG 1

#if YYDEBUG
#  define YYERROR_VERBOSE 1
#endif

extern int pp_lex ( void );
extern void Prepr_ScanCord ( CORD cord );

void pp_error ( char * err );

%}

%union 
{
  unsigned long num;
  STRING        str;
}

%token PREPR_LINE PREPR_PRAGMA

%token<num> PREPR_NUMBER
%token<str> PREPR_ERROR PREPR_STRING PREPR_IDENT

%%

prepr
  : line           
  | PREPR_ERROR    
      {
        CompilerError( "user error: %s", C_STR($1) );
      }
  | PREPR_PRAGMA   
      {
        CompilerWarning( "Ignoring #pragma" );
        YYACCEPT;
      }
  | /* empty */
  ;

line
  : PREPR_LINE PREPR_NUMBER                
    {
      Scanner_SetLine( $2, NULL );
    }
  | PREPR_LINE PREPR_NUMBER PREPR_STRING   
    {
      Scanner_SetLine( $2, $3 );
    }
  | PREPR_NUMBER                           
    {
      Scanner_SetLine( $1, NULL );
    }
  | PREPR_NUMBER PREPR_STRING              
    {
      Scanner_SetLine( $1, $2 );
    }
  ;

%%

void pp_error ( char * err )
{
  ScannerError( "%s", err );
};



//--------------------------------------------------------------------------
// Name         Prepr_Parse 
//
// 
//--------------------------------------------------------------------------
int Prepr_Parse ( CORD cord )
{
  Prepr_ScanCord( cord );
  return pp_parse();
};
