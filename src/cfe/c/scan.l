%{    
#include "precomp.h"
#include "cfe_priv.h"

// This macro should have been defined. It stores the exact type of
// the operator (e_code) and returns a more general lexical cathegory
// token (lex_code)
//
#ifndef SCANNER_SET_E
  #define SCANNER_SET_E( e_code, lex_code )  (lex_code)
#endif

//=================================================================
// Local data and definitions
//

const static struct {
  const char * szText;
  int token;
} ReservedWords[] = 
{
//
// Standard words
//
  { "auto",            C_AUTO         },
  { "break",           BREAK          },
  { "case",            CASE           },
  { "continue",        CONTINUE       },
  { "default",         DEFAULT        },
  { "do",              DO             },
  { "else",            ELSE           },
  { "enum",            ENUM           },
  { "extern",          C_EXTERN       },
  { "for",             FOR            },
  { "goto",            GOTO           },
  { "if",              IF             },
  { "inline",          C_INLINE       },
  { "register",        C_REGISTER     },
  { "return",          RETURN         },
  { "sizeof",          SIZEOF         },
  { "static",          C_STATIC       },
  { "struct",          STRUCT         },
  { "switch",          SWITCH         },
  { "typedef",         C_TYPEDEF      },
  { "union",           UNION          },
  { "while",           WHILE          },

// QUAL
//
  { "const",           C_CONST     },
  { "restrict",        C_RESTRICT  },
  { "volatile",        C_VOLATILE  },

// TYPESPEC
//
  { "void",            C_VOID       },
  { "char",            C_CHAR       },
  { "short",           C_SHORT      },
  { "int",             C_INT        },
  { "long",            C_LONG       },
  { "signed",          C_SIGNED     },
  { "unsigned",        C_UNSIGNED   },
  { "float",           C_FLOAT      },
  { "double",          C_DOUBLE     },
  { "_Bool",           C__BOOL      },
  { "_Complex",        C__COMPLEX   },
  { "_Imaginary",      C__IMAGINARY },

//
// Our extensions
//
  { "__declspec",      DECLSPEC       },
  { "__attr",          ATTR           },
  { "__int",           C_EXT_INT      },
  { "__float",         C_EXT_FLOAT    },

//
// GNU C extensions
//
  { "__typeof__",      TYPEOF         },
  { "__alignof__",     ALIGNOF        },
  { "typeof",          TYPEOF         },
  { "__label__",       LABEL          },
  { "__inline__",      C_INLINE       },
  { "__attribute__",   GCC_ATTRIBUTE  },
};

#ifdef _DEBUG
static bool bWordsAdded = false;
#endif


static TARGET_INTMAX_T CharValue;
static bool CharIsWide;
static int  CharShift; // if bCharGoesLeft==true, this is teh shift for the next char 
static int  CharLen;  // -1 means an error has been reported
static TAst * StringFirst, * StringLast;
static unsigned StringLen;


static YYLTYPE CurPos;
#define NL()           (CurPos.col = 1, ++CurPos.line)
#define COL()          (CurPos.col += yyleng)

#define SET_POS()      (yylloc = CurPos, CurPos.col += yyleng)

static int Ident ( STRING str );
static int Number ( int radix, const char * text );
static int F_Number ( int leng, const char * yytext );
static int F_HexNumber ( int leng, const char * text );
static void ValidateIdentUniChar ( const char * text );

static void CharConst_Begin ( bool isWide );
static void CharConst_NewChar ( char ch );
static void CharConst_OctalEscape ( const char * text );
static void CharConst_HexEscape ( const char * text );
static void CharConst_UniCharEscape ( const char * text );
static int  CharConst ( bool bReportEmpty );

static void StringConst_Begin ( bool isWide );
static void StringConst_AddChar ( void );
static void StringConst_NewChar ( char ch );
static void StringConst_OctalEscape ( const char * text );
static void StringConst_HexEscape ( const char * text );
static void StringConst_UniCharEscape ( const char * text );
static int  StringConst ( void );

%}

%option noyywrap
%option nodefault

/* C whitespace */
WS       [ \f\v\t]+
NL       \n

/* Digits */
DIGIT    [0-9]
XDIGIT   [0-9a-fA-F]


/* Universal character name */
HEXQUAD    ({XDIGIT}{XDIGIT}{XDIGIT}{XDIGIT})
UCHAR1     (u{HEXQUAD})|(U{HEXQUAD}{HEXQUAD})
UCHAR      (\\{UCHAR1})

BADUCHAR1  ((u{XDIGIT}{0,3})|(U{XDIGIT}{0,7}))
BADUCHAR   (\\{BADUCHAR1})


/* escapes */
SIMPLE_ESC (\\\'|\\\"|\\\?|\\\\|\\a|\\b|\\f|\\n|\\r|\\t|\\v\\e)
OCTAL_ESC  (\\[0-7]{1,3})
HEX_ESC    (\\x{XDIGIT}+)
ESCAPE     ({SIMPLE_ESC}|{OCTAL_ESC}|{HEX_ESC}|{UCHAR})

/* character character */
CCHAR      ([^\'\\\n]|{ESCAPE})

/* string character */
SCHAR      ([^\"\\\n]|{ESCAPE})

/* C identifier */
IDNONDIGIT  ([[:alpha:]_]|{UCHAR})
ID          ({IDNONDIGIT}({DIGIT}|{IDNONDIGIT})*)

/* An identifier without universal characters */
SIMPLEID       ([[:alpha:]_][[:alnum:]_]*)


/* integer suffix */
ISUFFIX  (([uU][lL]?)|([uU](ll|LL))|([lL][uU]?)|((ll|LL)[uU]?)|([uU]?[iI]{DIGIT}*))


  

/* floating point suffix */
FSUFFIX  [flFL]

/* a fractional constant */
FRACT    (({DIGIT}*\.{DIGIT}+)|({DIGIT}+\.))

FRACT_OR_INT  ({FRACT}|{DIGIT}+)

/* hex fractional constant */
HEXFRACT (({XDIGIT}*\.{XDIGIT}+)|({XDIGIT}+\.))

HEXFRACT_OR_INT  ({HEXFRACT}|{XDIGIT}+)

/* exponent part */
EXP      ([eE][+-]?[0-9]+)                      

/* binary exponent part */
BINEXP   ([pP][+-]?[0-9]+)

%x COMMENT LINE_COMMENT PREPR_DIR CHAR_STATE STR_STATE ID_STATE1 ID_STATE2
%x PLAIN_STR_STATE PRAGMA_OP_END

%%

  CORD cord = NULL;
  unsigned startLine;


"/*"             startLine = CurPos.col; COL(); BEGIN( COMMENT );
"//"             COL(); BEGIN( LINE_COMMENT );
{WS}             COL(); /* eat up whitespace */
\n               NL();

^{WS}?"#"        SET_POS(); cord = NULL; BEGIN(PREPR_DIR);

\\               SET_POS(); cord = NULL; BEGIN(ID_STATE1);

_Pragma{WS}?    {
                  /* So here we go .. :-) We parse manually _Pragma(string) */
                  int c;

                  SET_POS();

                  c = input();
                  if (c != '(')
                  {
                    TST,ScannerError( "'(' expected after _Pragma" );
                    goto exit;
                  }

                  // Skip the blanks
                  while (isspace( c = input() ))
                    {}

                  if (c == 'l' || c == 'L')
                    c = input();

                  if (c != '"')
                  {
                    TST,ScannerError( "A string expected after _Pragma(" );
                    goto exit;
                  }

                  cord = NULL;
                  BEGIN( PLAIN_STR_STATE ); // parse the string

                exit:;
                }

{SIMPLEID}\\                   {
                                 SET_POS();

                                 // Put the initial text in the cord without the '\\'
                                 cord = Cord_NewSegment( NULL, yyleng, yytext );
                                 BEGIN(ID_STATE1);
                               }   

{SIMPLEID}                     {
                                 SET_POS();
                                 return Ident( NewStringLenWithData( yytext, yyleng ) );
                               }

0[0-7]*{ISUFFIX}?              SET_POS(); return Number( 8, yytext ); 

0[0-9]+{ISUFFIX}?              {
                                 SET_POS();
                                 TST,ScannerError( "Bad octal number" ); 
                                 return Number( 8, "0" );
                               }

[1-9][0-9]*{ISUFFIX}?          SET_POS(); return Number( 10, yytext ); 

0[xX]{XDIGIT}+{ISUFFIX}?       SET_POS(); return Number( 16, yytext+2 );
0[xX]{ISUFFIX}?                {
                                 SET_POS();
                                 TST,ScannerError( "Invalid hex number" ); 
                                 return Number( 16, "0" );
                               }

{FRACT}{FSUFFIX}?              SET_POS(); return F_Number( yyleng, yytext );

{FRACT_OR_INT}[eE][+-]?[0-9]+{FSUFFIX}?  SET_POS(); return F_Number( yyleng, yytext );

{FRACT_OR_INT}[eE][+-]?        {
                                 SET_POS(); 

                                 TST,ScannerError( "Number expected after exponent" ); 
                                 return F_Number( 3, "0.0" );
                               }

0[xX]{HEXFRACT_OR_INT}[pP][+-]?[0-9]+{FSUFFIX}?   {
                                 SET_POS(); 
                                 return F_HexNumber( yyleng, yytext );
                               }

0[xX]{HEXFRACT_OR_INT}[pP][+-]?  {
                                 SET_POS();
                                 TST,ScannerError( "Bad floating point constant" ); 
                                 return F_Number( 3, "0.0" );
                               }

0[xX]{HEXFRACT}                {
                                 SET_POS();
                                 TST,ScannerError( "Bad floating point constant" ); 
                                 return F_Number( 3, "0.0" );
                               }
0[xX]\.                        {
                                 SET_POS();
                                 TST,ScannerError( "Bad floating point constant" ); 
                                 return F_Number( 3, "0.0" );
                               }

\'       SET_POS(); CharConst_Begin( false ); BEGIN( CHAR_STATE ); 
L\'      SET_POS(); CharConst_Begin( true ); BEGIN( CHAR_STATE ); 
\"       SET_POS(); StringConst_Begin( false ); BEGIN( STR_STATE ); 
L\"      SET_POS(); StringConst_Begin( true ); BEGIN( STR_STATE );

"["      SET_POS(); return *yytext;
"]"      SET_POS(); return *yytext;
"("      SET_POS(); return *yytext;
")"      SET_POS(); return *yytext;
"{"      SET_POS(); return *yytext;
"}"      SET_POS(); return *yytext;
"\."     SET_POS(); return *yytext;

"->"     SET_POS(); return PTR;
"++"     SET_POS(); return INC;
"--"     SET_POS(); return DEC;

"&"      SET_POS(); return *yytext;
"*"      SET_POS(); return *yytext;
"+"      SET_POS(); return *yytext;
"-"      SET_POS(); return *yytext;
"~"      SET_POS(); return *yytext;
"!"      SET_POS(); return *yytext;
"/"      SET_POS(); return *yytext;
"%"      SET_POS(); return *yytext;

"<<"     SET_POS(); return SCANNER_SET_E( e_shl, SHIFT );
">>"     SET_POS(); return SCANNER_SET_E( e_shr, SHIFT );

"<"      SET_POS(); return SCANNER_SET_E( e_less, ARITHCOMPARE );
">"      SET_POS(); return SCANNER_SET_E( e_greater, ARITHCOMPARE );
"<="     SET_POS(); return SCANNER_SET_E( e_le_eq, ARITHCOMPARE );
">="     SET_POS(); return SCANNER_SET_E( e_gr_eq, ARITHCOMPARE );

"=="     SET_POS(); return SCANNER_SET_E( e_eq, EQCOMPARE );
"!="     SET_POS(); return SCANNER_SET_E( e_ne, EQCOMPARE );

"^"      SET_POS(); return *yytext;
"|"      SET_POS(); return *yytext;
"&&"     SET_POS(); return LOG_AND;
"||"     SET_POS(); return LOG_OR;
"?"      SET_POS(); return *yytext;
":"      SET_POS(); return *yytext;
";"      SET_POS(); return *yytext;
"..."    SET_POS(); return ELLIPSIS;
".."     SET_POS(); TST,ScannerError( "'..' is invalid. Did you mean '...'?" ); return ELLIPSIS;
"="      SET_POS(); return *yytext;

"*="     SET_POS(); return SCANNER_SET_E( e_asgn_mul, ASSIGN );
"/="     SET_POS(); return SCANNER_SET_E( e_asgn_div, ASSIGN );
"%="     SET_POS(); return SCANNER_SET_E( e_asgn_mod, ASSIGN );
"+="     SET_POS(); return SCANNER_SET_E( e_asgn_plus, ASSIGN );
"-="     SET_POS(); return SCANNER_SET_E( e_asgn_minus, ASSIGN );
"&="     SET_POS(); return SCANNER_SET_E( e_asgn_and, ASSIGN );
"|="     SET_POS(); return SCANNER_SET_E( e_asgn_or, ASSIGN );
"^="     SET_POS(); return SCANNER_SET_E( e_asgn_xor, ASSIGN );
"<<="    SET_POS(); return SCANNER_SET_E( e_asgn_shl, ASSIGN );
">>="    SET_POS(); return SCANNER_SET_E( e_asgn_shr, ASSIGN );

","      SET_POS(); return *yytext;
"<:"     SET_POS(); return '[';
":>"     SET_POS(); return ']';
"<%"     SET_POS(); return '{';
"%>"     SET_POS(); return '}';

.        SET_POS(); TST,ScannerError( "Bad character '\\x%02x'", *yytext );


<CHAR_STATE>{
      <<EOF>>           { 
                          ScannerError( "Unterminated character constant" ); 
                          BEGIN(INITIAL); 
                          return CharConst( false );
                        } 

      \n                { 
                          ScannerError( "Unterminated character constant" ); 
                          NL(); 
                          BEGIN(INITIAL); 
                          return CharConst( false );
                        } 

      \'                {
                          COL(); 
                          BEGIN(INITIAL); 
                          return CharConst( true );
                        }

      \\\n              NL(); // concat lines

      \\[\'\"\?\\]      COL(); CharConst_NewChar( yytext[1] );
      \\a               COL(); CharConst_NewChar( '\a' );
      \\b               COL(); CharConst_NewChar( '\b' );
      \\f               COL(); CharConst_NewChar( '\f' );
      \\n               COL(); CharConst_NewChar( '\n' );
      \\r               COL(); CharConst_NewChar( '\r' );
      \\t               COL(); CharConst_NewChar( '\t' );
      \\v               COL(); CharConst_NewChar( '\v' );
      \\e               COL(); CharConst_NewChar( '\x1b' ); // GNU C extension
      {OCTAL_ESC}       COL(); CharConst_OctalEscape( yytext );
      {HEX_ESC}         COL(); CharConst_HexEscape( yytext );
      {UCHAR}           COL(); CharConst_UniCharEscape( yytext );
      {BADUCHAR}        {
                          COL(); 
                          ScannerError( "Ill-formed universal character" );
                          CharConst_NewChar( ' ' ); // append anything
                        }

      \\.?              {
                          COL(); 
                          ScannerError( "Bad escape character" );
                          CharConst_NewChar( ' ' ); // append anything
                        }

      [^\n'\\]          COL(); CharConst_NewChar( yytext[0] );
}

<STR_STATE>{
      <<EOF>>           { 
                          ScannerError( "Unterminated string constant" ); 
                          BEGIN(INITIAL); 
                          return StringConst();
                        }

      \n                { 
                          ScannerError( "Unterminated string constant" ); 
                          NL(); 
                          BEGIN(INITIAL); 
                          return StringConst();
                        } 

      \"                { 
                          COL();
                          BEGIN(INITIAL); 
                          return StringConst();
                        }

      \\\n              NL(); // concat lines

      \\[\'\"\?\\]      COL(); StringConst_NewChar( yytext[1] );                            
      \\a               COL(); StringConst_NewChar( '\a' );                                 
      \\b               COL(); StringConst_NewChar( '\b' );                                 
      \\f               COL(); StringConst_NewChar( '\f' );                                 
      \\n               COL(); StringConst_NewChar( '\n' );                                 
      \\r               COL(); StringConst_NewChar( '\r' );                                 
      \\t               COL(); StringConst_NewChar( '\t' );                                 
      \\v               COL(); StringConst_NewChar( '\v' );                                 
      \\e               COL(); StringConst_NewChar( '\x1b' ); // GNU C extension            
      {OCTAL_ESC}       COL(); StringConst_OctalEscape( yytext );                           
      {HEX_ESC}         COL(); StringConst_HexEscape( yytext );                             
      {UCHAR}           COL(); StringConst_UniCharEscape( yytext );                         
      {BADUCHAR}        COL(); ScannerError( "Ill-formed universal character" );

      \\.?              COL(); ScannerError( "Bad escape character" );

      [^\n"\\]          COL(); StringConst_NewChar( yytext[0] );
}

<ID_STATE1>{
      {UCHAR1}          {
                          COL();
                          ValidateIdentUniChar( yytext );
                          cord = Cord_NewSegment( cord, yyleng, yytext );
                          BEGIN(ID_STATE2);
                        }

      {BADUCHAR1}       { 
                          COL(); 
                          ScannerError( "Ill-formed universal character" );
                          BEGIN(ID_STATE2);
                        }

      .                 {
                          COL();  
  												ScannerError( "Ill-formed universal character" );
                          yyless(1); // put back the character
													BEGIN(ID_STATE2); 
                        }

      \n                {
                          NL();  
													ScannerError( "Ill-formed universal character" );
													BEGIN(INITIAL); 
                          
													return Ident( StringFromCord( cord ) );
                        }
}

<ID_STATE2>{
      {UCHAR}           { 
                          COL(); 
                          ValidateIdentUniChar( yytext+1 );
                          cord = Cord_NewSegment( cord, yyleng, yytext );
                        }

      {BADUCHAR}        COL(); ScannerError( "Ill-formed universal character" );
      \\                COL(); ScannerError( "Ill-formed universal character" );

      [[:alnum:]_]+     COL(); cord = Cord_NewSegment( cord, yyleng, yytext );

      .                 { 
                          yyless(1);
                          BEGIN(INITIAL); 
                          
													return Ident( StringFromCord( cord ) );
                        }

      \n                { 
                          NL(); 
													BEGIN(INITIAL); 
                          
													return Ident( StringFromCord( cord ) );
                        }
}

<PLAIN_STR_STATE>{
      <<EOF>>           { 
                          ScannerError( "Unterminated string constant" ); 
                          BEGIN(PRAGMA_OP_END); 
                        }

      \n                { 
                          ScannerError( "Unterminated string constant" ); 
                          NL();
                          BEGIN(PRAGMA_OP_END); 
                        } 

      [^\n"\\]*\"       { // sequence without '\\' ending with '"'
                          COL();
                          cord = Cord_NewSegment( cord, yyleng-1, yytext );
                          BEGIN(PRAGMA_OP_END);
                        }

      [^\n"\\]+         { // sequence without '\\'
                          COL();
                          cord = Cord_NewSegment( cord, yyleng, yytext );
                        }

      \\\n              NL(); // concat lines

      \\[\'\"\?\\]      COL(); cord = Cord_NewSegment( cord, 1, yytext+1 );
      \\a               COL(); cord = Cord_NewSegment( cord, 1, "\a" );
      \\b               COL(); cord = Cord_NewSegment( cord, 1, "\b" );
      \\f               COL(); cord = Cord_NewSegment( cord, 1, "\f" );
      \\n               COL(); cord = Cord_NewSegment( cord, 1, "\n" );
      \\r               COL(); cord = Cord_NewSegment( cord, 1, "\r" );
      \\t               COL(); cord = Cord_NewSegment( cord, 1, "\t" );
      \\v               COL(); cord = Cord_NewSegment( cord, 1, "\v" );
      \\e               COL(); cord = Cord_NewSegment( cord, 1, "\x1b" ); // GNU C extension

      {OCTAL_ESC}       {
                          unsigned value;

                          COL();

                          if ( (value = strtol( yytext+1, NULL, 8 )) > UCHAR_MAX)
                            ScannerError( "Value %u too big for a character", value );
                          else
                          {
                            char c = (char)value;
                            cord = Cord_NewSegment( cord, 1, &c );
                          }
                        }
      
      {HEX_ESC}         {
                          unsigned value;

                          COL();

                          // The number of hex digits must not be more than
                          // what is necessary to fill a char. No need to check 
                          // further in that case (strtol is a bitch in that respect)
                          //
                          if (yyleng > (CHAR_BIT+3)/4+2 ||
                              (value = strtol( yytext+2, NULL, 16 )) > UCHAR_MAX)
                            ScannerError( "Value %u too big for a character", value );
                          else
                          {
                            char c = (char)value;
                            cord = Cord_NewSegment( cord, 1, &c );
                          }
                        }

      \\.?              COL(); ScannerError( "Bad escape character" );
}

<PRAGMA_OP_END>{
        {WS}             // ignore blanks

        <<EOF>>         {
                          ScannerError( "')' expected after _Pragma(string" ); 

                          BEGIN(INITIAL); 
                          // Simulate a #pragma ....
                          ScannerPrepr( Cord_AppendCord( 
                                          Cord_NewSegment( NULL, 7, "pragma " ),
                                          cord ) );
                          return 0;
                        }
        ")"|\n|.        {
                          if (*yytext != ')')
                          {
                            ScannerError( "')' expected after _Pragma(string" ); 
                            yyless(0);
                          }

                          BEGIN(INITIAL); 
                          // Simulate a #pragma ....
                          ScannerPrepr( Cord_AppendCord( 
                                          Cord_NewSegment( NULL, 7, "pragma " ),
                                          cord ) );
                        }  
}

<PREPR_DIR>{
        <<EOF>>         BEGIN(INITIAL); ScannerPrepr( cord ); return 0;
        \n              NL(); BEGIN(INITIAL); ScannerPrepr( cord );

        \\\n            // concat lines 
        .+              cord = Cord_NewSegment( cord, yyleng, yytext );
}

<COMMENT>{
        <<EOF>>         {
                          ScannerError( "Comment started on line %u not closed", startLine ); 
                          BEGIN(INITIAL); 
                          yyterminate();
                        }

        [^*\n]+         COL(); /* eat anything that's not a '*' */
        "*"+[^*/\n]*    COL(); /* eat up '*'s not followed by '/'s */
        \n              NL();
        "*"+"/"         BEGIN(INITIAL);
}

<LINE_COMMENT>{
        <<EOF>>         BEGIN(INITIAL); yyterminate();
        \\\n            // concat lines 
        \n              NL(); BEGIN(INITIAL); 
        .+             
}


%%

//--------------------------------------------------------------------------
// Name         InitReservedWords 
//
// 
//--------------------------------------------------------------------------
static void InitReservedWords ()
{
  int i;
  ASSERT( !bWordsAdded );

  for ( i = 0; i < NELEM(ReservedWords); ++i )
  {
    STRING str;
    str = NewStringWithData( ReservedWords[i].szText );
    ASSERT( STRING_DATA(str)->token == 0 );
    STRING_DATA(str)->token = ReservedWords[i].token;
  }

#ifdef _DEBUG     
  bWordsAdded = true;
#endif
};

//--------------------------------------------------------------------------
// Name         Scanner_Init 
//
// 
//--------------------------------------------------------------------------
void Scanner_Init ( void )
{
  InitReservedWords();

  CurPos.fileName = NULL;
  CurPos.line = 1;
  CurPos.col = 1;
};

//--------------------------------------------------------------------------
// Name         Scanner_SetLine 
//
// 
//--------------------------------------------------------------------------
void Scanner_SetLine ( int line, STRING filename )
{
  if (filename != NULL)
    CurPos.fileName = filename;
  CurPos.line = line;
  CurPos.col = 1;
};

//--------------------------------------------------------------------------
// Name         ScannerPrepr 
//
// 
//--------------------------------------------------------------------------
void ScannerPrepr ( CORD cord )
{
  extern int Prepr_Parse ( CORD cord );

  Prepr_Parse( cord );
};

//--------------------------------------------------------------------------
// Name         StringFromCord 
//
// 
//--------------------------------------------------------------------------
STRING StringFromCord ( CORD cord )
{
  STRING res;

  if (cord == NULL)
  {
    res = NULL;
  }
  else
  // A little optimization. If teh cord has only one segment
  // we can use it directly instead of allocating a buffer
  //
  if (cord->pNext == NULL)
  {
    res = NewStringLenWithData( (char *)cord->u.data, cord->segLen );
  }
  else
  {
    char * pszBuffer, * dest;

    // Don't forget to reserve for the 0
    dest = pszBuffer = GC_ARRAY_NEW_ATOMIC( char, cord->totalLen + 1 );
    do
    {
      memcpy( dest, cord->u.data, cord->segLen );
      dest += cord->segLen;
      cord = cord->pNext;
    }
    while (cord != NULL);

    // no need to terminate the string fo NewStringLen

    res = NewStringLenWithData( pszBuffer, dest - pszBuffer );

    // We don't really have to do this but just to signify that this buffer
    // is just temporary and will *never* be used again...
    GC_DELETE( pszBuffer ); 
  }

  return res;
};


//--------------------------------------------------------------------------
// Name         CharConst_Begin 
//
// 
//--------------------------------------------------------------------------
static void CharConst_Begin ( bool isWide )
{
  TARGET_CLEAR_INT( &CharValue );
  CharIsWide = isWide;
  CharLen = 0;
  CharShift = 0;
};

//--------------------------------------------------------------------------
// Name         CharConst_NewValue 
//
// 
//--------------------------------------------------------------------------
static void CharConst_NewValue ( TARGET_INTMAX_T expanded )
{
  if (CharLen == -1)  // Did we already report an error ?
    goto exitFunc;

  if (CharIsWide)
  {
    if (CharLen > 0) // if a charecter has been added already
    {
      ScannerError( "Too long wide character constant" );
      CharLen = -1;
      goto exitFunc;
    }

    if (TARGET_GT_UNSIGNED( &expanded, &Target_WChar_Max ))
    {
      ScannerError( "Too large wide character constant" );
      TARGET_ASSIGN_HOST_CHAR( &expanded, ' ' ); // assign anything
    }

    CharValue = expanded;
  }
  else
  {
    if (CharLen == TARGET_INT_BIT / TARGET_CHAR_BIT)
    {
      ScannerError( "Too long character constant" );
      CharLen = -1;
      goto exitFunc;
    }

    if (TARGET_GT_UNSIGNED( &expanded, &Target_UChar_Max ))
    {
      ScannerError( "Too large character constant" );
      TARGET_ASSIGN_HOST_CHAR( &expanded, ' ' ); // assign anything
    }

    if (bCharGoesLeft)
    {
      TARGET_SHL_UNSIGNED( &expanded, &expanded, CharShift );
      CharShift += TARGET_CHAR_BIT;
    }
    else
      TARGET_SHL_UNSIGNED( &CharValue, &CharValue, TARGET_CHAR_BIT );

    TARGET_OR( &CharValue, &CharValue, &expanded );
  }
  ++CharLen;

exitFunc:
  ;
};

//--------------------------------------------------------------------------
// Name         CharConst_NewChar 
//
// 
//--------------------------------------------------------------------------
static void CharConst_NewChar ( char ch )
{
  TARGET_INTMAX_T expanded;

  TARGET_ASSIGN_HOST_CHAR( &expanded, ch );
  CharConst_NewValue( expanded );
};

//--------------------------------------------------------------------------
// Name         CharConst_OctalEscape
//
// The parameter is in format \\[0-7]{1,3}
//--------------------------------------------------------------------------
static void CharConst_OctalEscape ( const char * text )
{
  TARGET_INTMAX_T expanded;
  uint_fast16_t value = 0;

  while (*++text) // preincrement to skip over the '\\'
    value = (value << 3) + *text - '0';

  ASSERT( value < 8u*8u*8u );

  TARGET_ASSIGN_UINT16( &expanded, value );
  CharConst_NewValue( expanded );
};

//--------------------------------------------------------------------------
// Name         ConvertHex 
//
// Convert a hex string into a TARGET_INTMAX_T.
// On overflow error, reports it, corrects it and returns false
//--------------------------------------------------------------------------
static bool ConvertHex ( const char * text, TARGET_INTMAX_T * pres )
{
  TARGET_CLEAR_INT( pres );

  do
  {
    TARGET_INTMAX_T tgtDigit;
    uchar digit = tolower(*text);

    if (digit >= 'a')
      digit -= 'a' - 10;
    else
      digit -= '0';

    TARGET_SHL_UNSIGNED( pres, pres, 4 );
    if (overflow_bit)
      break;

    TARGET_ASSIGN_SMALL( &tgtDigit, digit );
    TARGET_ADD_UNSIGNED( pres, pres, &tgtDigit );

    if (overflow_bit)
      break;
  }
  while (isxdigit( *++text ));

  if (overflow_bit)
  {
    ScannerError( "Too large hexadecimal escape" );
    TARGET_ASSIGN_HOST_CHAR( pres, ' ' ); // Assign anything
    return false;
  }
  else
    return true;
};

//--------------------------------------------------------------------------
// Name         CharConst_HexEscape 
//
// The parameter is in format: \\x{XDIGIT}+
// It is not limited to any length, so it is possible it could overflow
// any integer type. So, we gather it in a TARGET_INTMAX_T which can hold all
// values of TARGET_CHAR_T. 
//--------------------------------------------------------------------------
static void CharConst_HexEscape ( const char * text )
{
  TARGET_INTMAX_T expanded;

  // skip the "\\x"
  ConvertHex( text + 2, &expanded );
  CharConst_NewValue( expanded );
};

//--------------------------------------------------------------------------
// Name         CharConst_UniCharEscape 
//
// The parameter is in format: \\[uU]{XDIGIT}{1,8}
// It is not limited to any length, so it is possible it could overflow
// any integer type. So, we gather it in a TARGET_INTMAX_T which can hold all
// values of TARGET_CHAR_T. 
//--------------------------------------------------------------------------
static void CharConst_UniCharEscape ( const char * text )
{
  TARGET_INTMAX_T expanded;

  // skip the "\\[uU]"
  ConvertHex( text + 2, &expanded );
  CharConst_NewValue( expanded );
};

//--------------------------------------------------------------------------
// Name         CharConst 
//
// bReportEmpty is set to _false_ when we are called after an error to 
// return teh correct kind of token, even though the string itself my be empty
//--------------------------------------------------------------------------
static int CharConst ( bool bReportEmpty )
{
  if (CharLen == 0 && bReportEmpty)
    ScannerError( "Empty character constant" );

  yylval.ast = Ast_intConst( &yylloc, CharIsWide ? CfeRoots.WChar_Type : CfeRoots.Int_Type, CharValue );
  return NUMBER;
};


//--------------------------------------------------------------------------
// Name         StringConst_Begin 
//
// 
//--------------------------------------------------------------------------
static void StringConst_Begin ( bool isWide )
{
  CharIsWide = isWide;
  StringFirst = StringLast = NULL;
  StringLen = 0;
};

//--------------------------------------------------------------------------
// Name         StringConst_AddChar 
//
// 
//--------------------------------------------------------------------------
static void StringConst_AddChar ( void )
{
  TAst * ast;

  ast = Ast_intConst( &yylloc, CharIsWide ? CfeRoots.WChar_Type : CfeRoots.CharType, CharValue );

  if (StringLast != NULL)
    StringLast->sibling = ast;
  else
    StringFirst = ast;

  StringLast = ast;
  ++StringLen;
};

//--------------------------------------------------------------------------
// Name         StringConst_NewChar 
//
// 
//--------------------------------------------------------------------------
static void StringConst_NewChar ( char ch )
{
  CharConst_Begin( CharIsWide );
  CharConst_NewChar( ch );
  StringConst_AddChar();
}

//--------------------------------------------------------------------------
// Name         StringConst_OctalEscape 
//
// 
//--------------------------------------------------------------------------
static void StringConst_OctalEscape ( const char * text )
{
  CharConst_Begin( CharIsWide );
  CharConst_OctalEscape( text );
  StringConst_AddChar();
}

//--------------------------------------------------------------------------
// Name         StringConst_HexEscape 
//
// 
//--------------------------------------------------------------------------
static void StringConst_HexEscape ( const char * text )
{
  CharConst_Begin( CharIsWide );
  CharConst_HexEscape( text );
  StringConst_AddChar();
}

//--------------------------------------------------------------------------
// Name         StringConst_UniCharEscape 
//
// 
//--------------------------------------------------------------------------
static void StringConst_UniCharEscape ( const char * text )
{
  CharConst_Begin( CharIsWide );
  CharConst_UniCharEscape( text );
  StringConst_AddChar();
}


//--------------------------------------------------------------------------
// Name         StringConst 
//
// 
//--------------------------------------------------------------------------
static int StringConst ( void )
{
  TAst * lit;

  lit = Ast( ast_stringLiteral, &yylloc );
  lit->child = StringFirst;

  lit->exprType = 
    Type_MakeArray( 
      CharIsWide ? CfeRoots.ConstWCharType : CfeRoots.ConstCharType,
      NULL, // qual
      NULL, // arrayLen
      false // bStaticSize
    );

  yylval.ast = lit;
  return CharIsWide ? LSTRING_CONST : STRING_CONST;
};

typedef struct TUniRange { unsigned l, h; } TUniRange;

//--------------------------------------------------------------------------
// Name         CmpUniRange 
//
// 
//--------------------------------------------------------------------------
static int CmpUniRange ( const void * _key, const void * _range )
{
  uint_fast16_t * key = (uint_fast16_t *)_key;
  TUniRange * range = (TUniRange *)_range;

  if (*key < range->l)
    return -1;
  if (*key <= range->h)
    return 0;
  return 1;
};

//--------------------------------------------------------------------------
// Name         ValidateIdentUniChar 
//
// The parameter is a [uU]{XDIGIT}{1,8}
// This function verifies whether the character can be used in an identifier
// and reports an error otherwise.
//--------------------------------------------------------------------------
static void ValidateIdentUniChar ( const char * text )
{
  TARGET_INTMAX_T wc;

  // skip the "[uU]"
  ConvertHex( text + 1, &wc );

  if (!overflow_bit)
  {
    uint_fast16_t uc = UINT16_FROM_TARGET( &wc );
    if (!overflow_bit)
    {
      static const TUniRange ranges[] = 
      {
#include "srtuni\uni.inc"
      };

      if (bsearch( &uc, ranges, NELEM(ranges), sizeof(ranges[0]), 
                   CmpUniRange ) != NULL)
      {
        return;
      }

      ScannerError( "Invalid universal character \\u%04x in identifier", uc );
      return;
    }
  }

  ScannerError( "Invalid universal character in identifier" );
};

//--------------------------------------------------------------------------
// Name         Number 
//
//
// ISUFFIX  ([uU][lL]?)|([uU](ll|LL))|([lL][uU]?)|((ll|LL)[uU]?)|([uU]?[iI]{DIGIT}*)
//--------------------------------------------------------------------------
/*
                    ||                       |
                    ||                       |  Octal or Hexadecimal
       Suffix       ||   Decimal Constant    |        Constant
       -------------++-----------------------+------------------------
       none         ||int                    | int
                    ||long int               | unsigned int
                    ||long long int          | long int
                    ||                       | unsigned long int
                    ||                       | long long int
                    ||                       | unsigned long long int
       -------------++-----------------------+------------------------
       u or U       ||unsigned int           | unsigned int
                    ||unsigned long int      | unsigned long int
                    ||unsigned long long int | unsigned long long int
       -------------++-----------------------+------------------------
       l or L       ||long int               | long int
                    ||long long int          | unsigned long int
                    ||                       | long long int
                    ||                       | unsigned long long int
       -------------++-----------------------+------------------------
       Both u or U  ||unsigned long int      | unsigned long int
       and l or L   ||unsigned long long int | unsigned long long int
       -------------++-----------------------+------------------------
       ll or LL     ||long long int          | long long int
                    ||                       | unsigned long long int
       -------------++-----------------------+------------------------
       Both u or U  ||unsigned long long int | unsigned long long int
       and ll or LL ||                       |

       If  an integer constant cannot be represented by any type in
       its list, it may have  an  extended  integer  type,  if  the
       extended  integer  type  can represent its value.  If all of
       the types in the list  for  the  constant  are  signed,  the
       extended  integer type shall be signed.  If all of the types
       in the list for the  constant  are  unsigned,  the  extended
       integer  type  shall be unsigned.  If the list contains both
       signed and unsigned types, the extended integer type may  be
       signed or unsigned.

  And our MS-compatible extended suffixes:
  i32,ui32  (of course 32 can be another number).
*/
static int Number ( int radix, const char * text )
{
  bool bResult = false;
  const char * tail;
  TARGET_IMAX_T value;
  const TQualNode * type;
  bool bOctHex;
  bool U;
  char L; // 1 - L, 2 - LL
  ulong ilen;  // length of the MS-compatible suffix (e.g. for i32 it is 2)
  IntegerType_t it;

  if (radix == 10)
  {
    tail = unsigned_integer_from_string( 
      sizeof(value), 
      text, 
      &value 
    );
  }
  else
  {
    TARGET_CLEAR_INT( &value );

    // Calculate shift 
    radix = radix == 8 ? 3 : 4;

    // We use isxdigit() to control the loop, since that encompasses all the 
    /// possibilities. We know that the number contains only valid digits.
    //
    for ( tail = text; isxdigit(*tail); ++tail )
    {
      TARGET_IMAX_T tgtDigit;
      uchar digit = tolower(*tail);

      if (digit >= 'a')
        digit -= 'a' - 10;
      else
        digit -= '0';

      TARGET_SHL_UNSIGNED( &value, &value, radix );
      if (overflow_bit)
        break;

      TARGET_ASSIGN_SMALL( &tgtDigit, digit );
      TARGET_ADD_UNSIGNED( &value, &value, &tgtDigit );
      if (overflow_bit)
        break;
    }
  }

  if (!overflow_bit)
    overflow_bit = TARGET_GT_UNSIGNED( &value, &Target_UIntMax_Max );

  if (overflow_bit)
  {
    ScannerError( "Too long integer constant" );
    goto exitFunc;
  }

  bOctHex = radix != 10;
  U = false;
  L = 0;
  ilen = 0;

/*
  [uU][lL]?
  [uU](ll|LL)
  [uU][iI]{DIGIT}+
  [iI]{DIGIT}+
  [lL][uU]?
  (ll|LL)[uU]?
*/
  switch (*tail)
  {
  case 'u': case 'U':
    U = true; ++tail;
    if (*tail != 'i' && *tail != 'I')
    {
      if (*tail == 'l' || *tail == 'L') { ++L; ++tail; }
      if (*tail == 'l' || *tail == 'L') { ++L; }
      break;
    }
    else 
      goto case_i;

  case_i:
  case 'i': case 'I':
    ++tail;
    ilen = strtoul( tail, NULL, 10 );
    if (ilen < 1 || ilen >  TARGET_INTMAX_BIT)
    {
      ScannerError( "Invalid integer suffix" );
      goto exitFunc;
    }
    break;

  case 'l': case 'L':
    ++L; ++tail;
    if (*tail == 'l' || *tail == 'L') { ++L; ++tail; }
    if (*tail == 'u' || *tail == 'U') { U = true; }
    break;

  case 0:
    break;

  default:
    ASSERT( 0 );
  }



  // Determine the type.
  //
  if (ilen != 0)
  {
    for ( it = it_Bool + 1; it < it_Last; ++it )
    {
      bool isSigned = IntegerTypesDesc[it].isSigned;

      if (U)
      {
        if (isSigned)
          continue;
      }
      else
      {
        if (!bOctHex && !isSigned)
          continue;
      }

      if (IntegerTypesDesc[it].pMachType->precision == ilen)
        goto foundType;
    }

    ScannerError( "Invalid integer suffix" );
  }
  else
  {
    //
    // First pass over the standard types only
    for ( it = it_Int; it < it_Last; ++it )
    {
      if (!IntegerTypesDesc[it].isExtended)
      {
        bool isSigned = IntegerTypesDesc[it].isSigned;

        if (U)
        {
          if (isSigned)
            continue;
        }
        else
        {
          if (!bOctHex && !isSigned)
            continue;
        }
        if (L > IntegerTypesDesc[it].L)
          continue;

        if (TARGET_LE_UNSIGNED( &value, &IntegerTypesDesc[it].maxValue ))
          goto foundType;
      }
    }

    // we didn't find a match; search the extended types
    for ( it = it_Int; it < it_Last; ++it )
    {
      if (IntegerTypesDesc[it].isExtended)
      {
        bool isSigned = IntegerTypesDesc[it].isSigned;

        if (U)
        {
          if (isSigned)
            continue;
        }
        else
        {
          if (!bOctHex && !isSigned)
            continue;
        }
        if (L > IntegerTypesDesc[it].L)
          continue;

        if (TARGET_LE_UNSIGNED( &value, &IntegerTypesDesc[it].maxValue ))
          goto foundType;
      }
    }

    // we didn't find a match again; try them all
    for ( it = it_Int; it < it_Last; ++it )
    {
      if (TARGET_LE_UNSIGNED( &value, &IntegerTypesDesc[it].maxValue ))
        goto foundType;
    }

    it = it_UIntMax;
  }

foundType:
  if (TARGET_GT_UNSIGNED( &value, &IntegerTypesDesc[it].maxValue ))
  {
    TARGET_AND( &value, &value, &IntegerTypesDesc[it].pMachType->mask );
    ScannerWarning( "Too long integer constant" );
  }

  type = CfeRoots.IntegerTypes[it];

  bResult = true;
exitFunc:
  if (!bResult)
  {
    TARGET_CLEAR_INT( &value );
    type = CfeRoots.Int_Type;
  }

  yylval.ast = Ast_intConst( &yylloc, type, value );
  return NUMBER;
};



//--------------------------------------------------------------------------
// Name         F_Number 
//
// 
//--------------------------------------------------------------------------
static int F_Number ( int leng, const char * yytext )
{
  int oldRound = IEEE_set_round( IEEE_RN ); // round to nearest

  switch (yytext[leng-1])
  {
  case 'f': case 'F':
    {
      TARGET_FLOAT_T v;
      IEEE_single_from_string( yytext, &v );
      yylval.ast = Ast_floatConst( &yylloc, v );
    }
    break;

  default:
    {
      TARGET_DOUBLE_T v;
      IEEE_double_from_string( yytext, &v );
      yylval.ast = Ast_doubleConst( &yylloc, v );
    }
    break;

  case 'l': case 'L':
    {
      // TODO: proper long double
      TARGET_LONG_DOUBLE_T v;
      IEEE_double_from_string( yytext, &v );
      yylval.ast = Ast_longDoubleConst( &yylloc, v );
    }
    break;
  }

  IEEE_set_round( oldRound );

  return F_NUMBER;
};

//--------------------------------------------------------------------------
// Name         F_HexNumber 
//
// 0[xX]{HEXFRACT_OR_INT}[pP][+-]?[0-9]+{FSUFFIX}?  
//--------------------------------------------------------------------------
static int F_HexNumber ( int leng, const char * text )
{
  IEEE_double_t res;
  extern const char * HexStringToDouble ( const char * text, IEEE_double_t * res );

  //TODO: proper hex floats and proper long double

  // skip over "0[xX]"

  if ( (text = HexStringToDouble( text + 2, &res )) == NULL)
  {
    ScannerError( "Hexadecimal foating point overflow" );
    return F_Number( 3, "0.0" );
  }

  switch (*text)
  {
  case 'f': case 'F':          
    {
      IEEE_float_t v = IEEE_double_to_single( res );
      yylval.ast = Ast_floatConst( &yylloc, v );
    }
    break;

  default:
    yylval.ast = Ast_doubleConst( &yylloc, res );
    break;

  case 'l': case 'L':
    yylval.ast = Ast_longDoubleConst( &yylloc, res );
    break;
  }

  return F_NUMBER;
};

//--------------------------------------------------------------------------
// Name         Ident 
//
// 
//--------------------------------------------------------------------------
static int Ident ( STRING str )
{
  TSymbol * sym;

  ASSERT( bWordsAdded );

  // Check for a reserved word
  //
  if (STRING_DATA(str)->token != 0)
  {
    return yylval.token = STRING_DATA(str)->token;
  } 
  else  // Check if for a typedef
  if (NULL != (sym = SymTab_LookupSymbol( str )) && sym->storageClass == SC_TYPEDEF)
  {
    yylval.sym = sym;
    return TYPENAME;
  }
  else
  {
    yylval.str = str;
    return IDENT;
  }
};

