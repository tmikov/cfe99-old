%{    
#include "precomp.h"
#include "cord.h"
#include "prepr.tab.h"
#include "comperr.h"

//=================================================================
// Global symbols that must be supplied in other modules
//

#ifndef ScannerError
  extern void ScannerError ( const char * str, ... );
#endif
#ifndef ScannerWarning
  extern void ScannerWarning ( const char * str, ... );
#endif
%}


%option outfile="scanp.c"  
%option prefix="pp"
%option nodefault

/* C whitespace */
WS       [ \f\v\t]+
NL       \n

/* Digits */
DIGIT    [0-9]
XDIGIT   [0-9a-fA-F]

/* escapes */
SIMPLE_ESC (\\\'|\\\"|\\\?|\\\\|\\a|\\b|\\f|\\n|\\r|\\t|\\v\\e)
OCTAL_ESC  (\\[0-7]{1,3})
HEX_ESC    (\\x{XDIGIT}+)
ESCAPE     ({SIMPLE_ESC}|{OCTAL_ESC}|{HEX_ESC})

%x STR_STATE

%%
  CORD cord = NULL;

{WS}      // ignore spaces

[0-9]+    {
            errno = 0;
            pplval.num = strtol( pptext, NULL, 10 );

            if (errno != 0)
            {
              ScannerError( "Too long integer" );
              pplval.num = 0;
            }

            return PREPR_NUMBER;
          }

line      return PREPR_LINE;
pragma    return PREPR_PRAGMA;

error{WS}.*   { 
                // For the error token we want to return everything until
                // the end of the line because it must be printed.
                //
                pplval.cord = Cord_NewSegment( NULL, ppleng-5, pptext+5 );
                return PREPR_ERROR;
              }               

[A-Za-z_][A-Za-z_0-9]* ScannerError( "Urecognized preprocessor identifier '%s'", pptext );

L?\"        BEGIN( STR_STATE ); cord = NULL; 

\n        // ignore embedded new lines

.         ScannerError( "Unrecognized character \\x%02x", *pptext );

<STR_STATE>{
      \"                {
                          BEGIN(INITIAL); 
                          pplval.cord = cord;
                          return PREPR_STRING;
                        }

      \\[\'\"\?\\]      cord = Cord_NewSegment( cord, 1, pptext+1 );
      \\a               cord = Cord_NewSegment( cord, 1, "\a" );
      \\b               cord = Cord_NewSegment( cord, 1, "\b" );
      \\f               cord = Cord_NewSegment( cord, 1, "\f" );
      \\n               cord = Cord_NewSegment( cord, 1, "\n" );
      \\r               cord = Cord_NewSegment( cord, 1, "\r" );
      \\t               cord = Cord_NewSegment( cord, 1, "\t" );
      \\v               cord = Cord_NewSegment( cord, 1, "\v" );
      \\e               cord = Cord_NewSegment( cord, 1, "\x1b" ); // GNU C extension

      {OCTAL_ESC}       {
                          unsigned value;

                          if ( (value = strtol( pptext+1, NULL, 8 )) > UCHAR_MAX)
                            ScannerError( "Value %u too big for a character", value );
                          else
                          {
                            char c = (char)value;
                            cord = Cord_NewSegment( cord, 1, &c );
                          }
                        }
      
      {HEX_ESC}         {
                          unsigned value;

                          // The number of hex digits must not be more than
                          // what is necessary to fill a char. No need to check 
                          // further in that case (strtol is a bitch in that respect)
                          //
                          if (ppleng > (CHAR_BIT+3)/4+2 ||
                              (value = strtol( pptext+2, NULL, 16 )) > UCHAR_MAX)
                            ScannerError( "Value %u too big for a character", value );
                          else
                          {
                            char c = (char)value;
                            cord = Cord_NewSegment( cord, 1, &c );
                          }
                        }

      \\.               ScannerError( "Bad escape character" );

      <<EOF>>           { 
                          ScannerError( "Unterminated string constant" ); 
                          BEGIN(INITIAL); 
                          pplval.cord = cord;
                          return PREPR_STRING;
                        } 

      [^\"\\\n]+        cord = Cord_NewSegment( cord, ppleng, pptext );
      .|\n              cord = Cord_NewSegment( cord, 1, pptext ); 
}


%%

static YY_BUFFER_STATE CurBuffer = NULL;
static const TCordSegment * pCurSeg = NULL;

//--------------------------------------------------------------------------
// Name         Prepr_ScanCord 
//
// 
//--------------------------------------------------------------------------
void Prepr_ScanCord ( CORD cord )
{
  BEGIN(INITIAL);

  if (CurBuffer)
  {
    yy_delete_buffer( CurBuffer );
    CurBuffer = NULL;
  }

  pCurSeg = cord;

  if (pCurSeg)
    CurBuffer = yy_scan_bytes( (const char *)pCurSeg->u.data, pCurSeg->segLen );
  else
    CurBuffer = yy_scan_string( "" );
};

//--------------------------------------------------------------------------
// Name         Prepr_Done
//
// 
//--------------------------------------------------------------------------
void Prepr_Done ( void )
{
  if (CurBuffer)
  {
    yy_delete_buffer( CurBuffer );
    CurBuffer = NULL;
  }
  pCurSeg = NULL;
};

//--------------------------------------------------------------------------
// Name         yywrap 
//
// 
//--------------------------------------------------------------------------
int yywrap ( void )
{
  yy_delete_buffer( CurBuffer );
  CurBuffer = NULL;

  if (pCurSeg == NULL)
    return 1;
  if ((pCurSeg = pCurSeg->pNext) == NULL)
    return 1; // no more data

  CurBuffer = yy_scan_bytes( (const char *)pCurSeg->u.data, pCurSeg->segLen );

  return 0;
};
