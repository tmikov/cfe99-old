%{
#include <io.h>

#include <stdarg.h>

#include "port\global.h"

// Required include files that must be provided/modified
//
#include "c_tab.h"
#include "scan_x.h"

// This macro should have been defined. It stores the exact type of
// the operator (e_code) and returns a more general lexical cathegory
// token (lex_code)
//
#ifndef SCANNER_SET_E
  #define SCANNER_SET_E( e_code, lex_code )  (lex_code)
#endif

//=================================================================
// Global symbols that must be supplied in other modules
//

extern void ScannerError ( const char * str, ... );
extern int ScannerCheckResWord ( const char * szIdent, int len );
extern int ScannerClassifyIdent ( void );

//=================================================================
// Global variables accessible from external modules
//

int yycurline = 1;


//=================================================================
// Local data and definitions

// Define several tables for minor issues like trigraphs and
//
#include "lextabs.inc"

#define GET_TRIGRAPH(c) TrigraphTable[(c) - FIRST_TRIGRAPH]
#define GET_ESCAPE(c)   EscapeTable[(c) - FIRST_ESCAPE]

%}

%option outfile="scan.c"  
%option noyywrap
%option nodefault

/* C identifier */
ID       [[:alpha:]_][[:alnum:]_]*

/* C whitespace */
WS       [ \t]+

NL       \n
DIGIT    [0-9]
XDIGIT   [0-9a-fA-F]

/* integer suffix */
ISUFFIX  (([uU][lL]?)|([lL][uU]?))?

/* floating point suffix */
FSUFFIX  [flFL]?

/* a fractional number */
FRACT    ([0-9]*\.[0-9]+)|([0-9]+\.)

/* a fractional or integer number */
FRACT_OR_INT {FRACT}|[0-9]+

/* exponent */ 
EXP      [eE][+-]?[0-9]+

/* a trigraph */
TRIGRAPH  \?\?[=(/)\'<!>-]
BAD_TRI   \?\?

/* a character escape */
ESCAPE    \\[abfnrtv]

%x COMMENT LINE_COMMENT STR_STATE CHAR_STATE PREPR_DIR

%%

"/*"            BEGIN( COMMENT );
"//"            BEGIN( LINE_COMMENT );
{WS}+           /* eat up whitespace */

{NL}            ++yycurline;

^{WS}*"#"       BEGIN(PREPR_DIR);

"..."           return ELLIPSIS; 
".."            ScannerError(".. is invalid"); return '.';

"*="            return SCANNER_SET_E( e_asgn_mul, ASSIGN );
"/="            return SCANNER_SET_E( e_asgn_div, ASSIGN );
"%="            return SCANNER_SET_E( e_asgn_mod, ASSIGN );
"+="            return SCANNER_SET_E( e_asgn_plus, ASSIGN );
"-="            return SCANNER_SET_E( e_asgn_minus, ASSIGN );
"&="            return SCANNER_SET_E( e_asgn_and, ASSIGN );
"|="            return SCANNER_SET_E( e_asgn_or, ASSIGN );
"^="            return SCANNER_SET_E( e_asgn_xor, ASSIGN );
"<<="           return SCANNER_SET_E( e_asgn_shl, ASSIGN );
">>="           return SCANNER_SET_E( e_asgn_shr, ASSIGN );

"||"            return SCANNER_SET_E( e_log_or, LOG_OR );
"&&"            return SCANNER_SET_E( e_log_and, LOG_AND );

"=="            return SCANNER_SET_E( e_eq, EQCOMPARE );
"!="            return SCANNER_SET_E( e_ne, EQCOMPARE );

"<"             return SCANNER_SET_E( e_less, ARITHCOMPARE );
"<="            return SCANNER_SET_E( e_le_eq, ARITHCOMPARE );
">"             return SCANNER_SET_E( e_greater, ARITHCOMPARE );
">="            return SCANNER_SET_E( e_gr_eq, ARITHCOMPARE );

"<<"            return SCANNER_SET_E( e_shl, SHIFT );
">>"            return SCANNER_SET_E( e_shr, SHIFT );

"++"            return INC;
"--"            return DEC;

"->"            return SCANNER_SET_E( e_struct_deref, PTR );

\"              BEGIN( STR_STATE );
L\"             BEGIN( STR_STATE );
\'              BEGIN( CHAR_STATE );
L\'             BEGIN( CHAR_STATE );

{ID}            {
                  int lex;
                  return (lex = ScannerCheckResWord( yytext, yyleng )) ? 
                    lex : ScannerClassifyIdent();
                }

0[0-7]*{ISUFFIX}        return NUMBER; // octal number. 
0[0-9]+{ISUFFIX}        ScannerError( "Invalid octal number" );

0[xX]{XDIGIT}+{ISUFFIX} return NUMBER; // hex number.
0[xX]{ISUFFIX}          ScannerError( "Invalid hex number" );

[1-9][0-9]*{ISUFFIX}    return NUMBER; // dec number

{FRACT}{FSUFFIX}               return F_NUMBER; // floating point without exponent
{FRACT_OR_INT}{EXP}{FSUFFIX}   return F_NUMBER; // floating point with exponent

{FRACT_OR_INT}[eE][+-]?        ScannerError( "Number expected after exponent" );

{TRIGRAPH}      return GET_TRIGRAPH( yytext[2] );
{BAD_TRI}       ScannerError( "Invalid trigraph" );

.               return *yytext;

<STR_STATE>{
        \"              BEGIN(INITIAL); return STRING;

        \\[\'\"\\\?]    yytext[1];
        {ESCAPE}        GET_ESCAPE(yytext[1]);

        \\{DIGIT}{1,3}   

        \\x{XDIGIT}{0,2}

        \\{NL}          ++yycurline;
        \\.             ScannerError("Unrecognized character escape '%c'", yytext[1] );

        {NL}            ScannerError("Unterminated string constant"); ++yycurline; BEGIN(INITIAL);

        {TRIGRAPH}      GET_TRIGRAPH( yytext[2] );
        {BAD_TRI}       // add to string
        .              
}

<CHAR_STATE>{
        \'              BEGIN(INITIAL); return NUMBER;

        \\[\'\"\\\?]    yytext[1];
        {ESCAPE}        GET_ESCAPE(yytext[1]);

        \\{DIGIT}{1,3}   

        \\x{XDIGIT}{0,2}

        \\{NL}          ++yycurline;
        \\.             ScannerError("Unrecognized character escape '%c'", yytext[1] );

        {NL}            ScannerError("Unterminated character constant"); ++yycurline; BEGIN(INITIAL);

        {TRIGRAPH}      GET_TRIGRAPH( yytext[2] );
        {BAD_TRI}       // add to string
        .               
}

<PREPR_DIR>{
        {NL}            ++yycurline; BEGIN(INITIAL);
        .
}

<COMMENT>{
        "*/"            BEGIN(INITIAL);
        {NL}            ++yycurline;
        .               
}

<LINE_COMMENT>{
        {NL}            ++yycurline; BEGIN(INITIAL); 
        .             
}

%%

#ifdef TEST_YYLEX

void ScannerError ( const char * str, ... )
{
  va_list ap;
  va_start( ap, str );

  fprintf( stderr, "line %d:", yycurline );
  vfprintf( stderr, str, ap );
  printf( "\n" );

  va_end( ap );
};



int ScannerClassifyIdent ( void )
{
  return IDENT;
};

int main ( void )
{
  int c;
  while (c = yylex())
  {
    printf( "%d\n", c );
  }
  return 0;
};
#endif
