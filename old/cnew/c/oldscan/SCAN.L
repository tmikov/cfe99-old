%{
#include <io.h>

#include <stdarg.h>

#include "port\global.h"
#include "comp.h"
#include "c_tab.h"

// Global variables accessible from external modules
//
int yycurline = 1;
unsigned yyidhash; // hash value of current ident

// Define several tables for minor issues like trigraphs and
//
#include "lextabs.inc"

#define GET_TRIGRAPH(c) TrigraphTable[(c) - FIRST_TRIGRAPH]
#define GET_ESCAPE(c)   EscapeTable[(c) - FIRST_ESCAPE]

#define RET_E( e_code, lex_code )  yylval.ecode = (e_code); return lex_code

#define RET_ITOK( i_tok, lex_code ) yylval.itok = (i_tok); return lex_code

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

switch          return SWITCH;
struct          return STRUCT;
continue        return CONTINUE;
case            return CASE;
sizeof          return SIZEOF;
default         return DEFAULT;
else            return ELSE;
while           return WHILE;
do              return DO;
enum            return ENUM;
goto            return GOTO;
break           return BREAK;
if              return IF;
for             return FOR;
union           return UNION;
return          return RETURN;

short           RET_ITOK( TOK_SHORT, TYPESPEC );
signed          RET_ITOK( TOK_SIGNED, TYPESPEC );
double          RET_ITOK( TOK_DOUBLE, TYPESPEC );
void            RET_ITOK( TOK_VOID, TYPESPEC );
int             RET_ITOK( TOK_INT, TYPESPEC );
char            RET_ITOK( TOK_CHAR, TYPESPEC );
long            RET_ITOK( TOK_LONG, TYPESPEC );
unsigned        RET_ITOK( TOK_UNSIGNED, TYPESPEC );
float           RET_ITOK( TOK_FLOAT, TYPESPEC );

const           RET_ITOK( TOK_CONST, TYPEQUAL );
volatile        RET_ITOK( TOK_VOLATILE, TYPEQUAL );

extern          RET_ITOK( TOK_EXTERN, SCLASS );
auto            RET_ITOK( TOK_AUTO, SCLASS );
static          RET_ITOK( TOK_STATIC, SCLASS );
register        RET_ITOK( TOK_REGISTER, SCLASS );
typedef         RET_ITOK( TOK_TYPEDEF, SCLASS );

__declspec      return DECLSPEC;
__attr          return ATTR;

"..."           return ELLIPSIS; 
".."            LexerError(".. is invalid"); return '.';

"*="            RET_E( e_asgn_mul, ASSIGN );
"/="            RET_E( e_asgn_div, ASSIGN );
"%="            RET_E( e_asgn_mod, ASSIGN );
"+="            RET_E( e_asgn_plus, ASSIGN );
"-="            RET_E( e_asgn_minus, ASSIGN );
"&="            RET_E( e_asgn_and, ASSIGN );
"|="            RET_E( e_asgn_or, ASSIGN );
"^="            RET_E( e_asgn_xor, ASSIGN );
"<<="           RET_E( e_asgn_shl, ASSIGN );
">>="           RET_E( e_asgn_shr, ASSIGN );

"||"            RET_E( e_log_or, LOG_OR );
"&&"            RET_E( e_log_and, LOG_AND );

"=="            RET_E( e_eq, EQCOMPARE );
"!="            RET_E( e_ne, EQCOMPARE );

"<"             RET_E( e_less, ARITHCOMPARE );
"<="            RET_E( e_le_eq, ARITHCOMPARE );
">"             RET_E( e_greater, ARITHCOMPARE );
">="            RET_E( e_gr_eq, ARITHCOMPARE );

"<<"            RET_E( e_shl, SHIFT );
">>"            RET_E( e_shr, SHIFT );

"++"            return INC;
"--"            return DEC;

"->"            RET_E( e_struct_deref, PTR );

\"              BEGIN( STR_STATE );
L\"             BEGIN( STR_STATE );
\'              BEGIN( CHAR_STATE );
L\'             BEGIN( CHAR_STATE );

{ID}            {
                  return
                    (yylval.sym = FindLocalSymbol(
                                      yytext,
                                      yyidhash=CalcStringHash( yytext ) )
                                    ) != NULL
                    &&
                    yylval.sym->storage->storClass == SC_TYPEDEF
                    ? TYPENAME : IDENT;
                }

0[0-7]*{ISUFFIX}        return NUMBER; // octal number. 
0[0-9]+{ISUFFIX}        LexerError( "Invalid octal number" );

0[xX]{XDIGIT}+{ISUFFIX} return NUMBER; // hex number.
0[xX]{ISUFFIX}          LexerError( "Invalid hex number" );

[1-9][0-9]*{ISUFFIX}    return NUMBER; // dec number

{FRACT}{FSUFFIX}               return F_NUMBER; // floating point without exponent
{FRACT_OR_INT}{EXP}{FSUFFIX}   return F_NUMBER; // floating point with exponent

{FRACT_OR_INT}[eE][+-]?        LexerError( "Number expected after exponent" );

{TRIGRAPH}      return GET_TRIGRAPH( yytext[2] );
{BAD_TRI}       LexerError( "Invalid trigraph" );

.               return *yytext;

<STR_STATE>{
        \"              BEGIN(INITIAL); return STRING;

        \\[\'\"\\\?]    yytext[1];
        {ESCAPE}        GET_ESCAPE(yytext[1]);

        \\{DIGIT}{1,3}   

        \\x{XDIGIT}{0,2}

        \\{NL}          ++yycurline;
        \\.             LexerError("Unrecognized character escape '%c'", yytext[1] );

        {NL}            LexerError("Unterminated string constant"); ++yycurline; BEGIN(INITIAL);

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
        \\.             LexerError("Unrecognized character escape '%c'", yytext[1] );

        {NL}            LexerError("Unterminated character constant"); ++yycurline; BEGIN(INITIAL);

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

void LexerError ( const char * str, ... )
{
  va_list ap;
  va_start( ap, str );

  fprintf( stderr, "line %d:", yycurline );
  vfprintf( stderr, str, ap );
  printf( "\n" );

  va_end( ap );
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
