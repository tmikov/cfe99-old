%{    
#include "precomp.h"
#include "strings.h"
#include "cord.h"
#include "heap.h"
#include "tconf.h"
#include "options.h"
#include "scan.h"
#include "c.tab.h"
#include "comperr.h"

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

#ifndef ScannerError
  extern void ScannerError ( const char * str, ... );
#endif
#ifndef ScannerWarning
  extern void ScannerWarning ( const char * str, ... );
#endif
#ifndef ScannerPrepr
  extern void ScannerPrepr ( CORD );
#endif

extern YYLTYPE yylloc; /*  location data for the lookahead	*/

//=================================================================
// Local data and definitions
//

#define SCANNER_SET_TC( tok ) (tok)
#define TC( tok, code )       (tok)

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
  { "const",           TC(QUAL, C_CONST    ) },
  { "restrict",        TC(QUAL, C_RESTRICT ) },
  { "volatile",        TC(QUAL, C_VOLATILE ) },

// TYPESPEC
//
  { "void",            TC(TYPESPEC, C_VOID      ) },
  { "char",            TC(TYPESPEC, C_CHAR      ) },
  { "short",           TC(TYPESPEC, C_SHORT     ) },
  { "int",             TC(TYPESPEC, C_INT       ) },
  { "long",            TC(TYPESPEC, C_LONG      ) },
  { "signed",          TC(TYPESPEC, C_SIGNED    ) },
  { "unsigned",        TC(TYPESPEC, C_UNSIGNED  ) },
  { "float",           TC(TYPESPEC, C_FLOAT     ) },
  { "double",          TC(TYPESPEC, C_DOUBLE    ) },
  { "_Bool",           TC(TYPESPEC, C__BOOL     ) },
  { "_Complex",        TC(TYPESPEC, C__COMPLEX  ) },
  { "_Imaginary",      TC(TYPESPEC, C__IMAGINARY) },

//
// Our extensions
//
  { "__declspec",      DECLSPEC       },
  { "__attr",          ATTR           },


//
// GNU C extensions
//
  { "__typeof__",      TYPEOF         },
  { "__alignof__",     ALIGNOF        },
  { "typeof",          TYPEOF         },
  { "__label__",       LABEL          },
  { "__inline__",      C_INLINE       },
  { "__attribute__",   GCC_ATTRIBUTE  },

//
// Just for testing
//
  { "typename",        TYPENAME },
};

#undef TC

#ifdef _DEBUG
static bool bWordsAdded = false;
#endif

static bool bWide; // whether we are currently processing a wide string/char

static YYLTYPE CurPos;
#define INC_LINE()     (CurPos.col = 1, ++CurPos.line)
#define UPDATE_COL()   (CurPos.col += yyleng)

#define SET_POS()      (yylloc = CurPos, CurPos.col += yyleng)

//--------------------------------------------------------------------------
// Name         InitReservedWords 
//
// 
//--------------------------------------------------------------------------
static void InitReservedWords ()
{
  int i;
  ASSERT( !bWordsAdded );

  for ( i = 0; i < _countof(ReservedWords); ++i )
  {
    STRING str;
    str = NewString( ReservedWords[i].szText );
    ASSERT( STRING_DATA(str)->namesp.token == 0 );
    STRING_DATA(str)->namesp.token = ReservedWords[i].token;
  }

#ifdef _DEBUG     
  bWordsAdded = true;
#endif
};

//--------------------------------------------------------------------------
// Name         StringFromCord 
//
// 
//--------------------------------------------------------------------------
static STRING StringFromCord ( CORD cord )
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
    res = NewStringLen( (char *)cord->u.data, cord->segLen );
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

    res = NewStringLen( pszBuffer, dest - pszBuffer );

    // We don't really have to do this but just to signify that this buffer
    // is just temporary and will *never* be used again...
    GC_DELETE( pszBuffer ); 
  }

  return res;
};

//--------------------------------------------------------------------------
// Name         AppendChars 
//
// 
//--------------------------------------------------------------------------
static CORD AppendChars ( CORD cord, int leng, const char * text )
{
#if TARGET_CHAR_BIT == CHAR_BIT
  return Cord_NewSegment( cord, leng, text );
#else
  TCordSegment * pSeg = Cord_AllocSegment( sizeof(TARGET_CHAR) * leng );
  TARGET_CHAR * dest = (TARGET_CHAR *)pSeg->u.data;
  int i;

  for ( i = 0; i < leng; ++i )
  {
    if (bCharIsSigned)
    {
      change_integer_size( 
        sizeof(text[i]), &text[i],
        sizeof(dest[i]), &dest[i]
      );
    }
    else
    {
      change_unsigned_integer_size( 
        sizeof(text[i]), &text[i],
        sizeof(dest[i]), &dest[i]
      );
    }
  }

  return Cord_AddSegment( cord, pSeg );
#endif
};

//--------------------------------------------------------------------------
// Name         AppendWchars 
//
// 
//--------------------------------------------------------------------------
static CORD AppendWchars ( CORD cord, int leng, const char * text )
{
  TCordSegment * pSeg = Cord_AllocSegment( sizeof(TARGET_WCHAR) * leng );
  TARGET_WCHAR * dest = (TARGET_WCHAR *)pSeg->u.data;
  int i;

  for ( i = 0; i < leng; ++i )
  {
    if (bCharIsSigned)
    {
      change_integer_size( 
        sizeof(text[i]), &text[i],
        sizeof(dest[i]), &dest[i]
      );
    }
    else
    {
      change_unsigned_integer_size( 
        sizeof(text[i]), &text[i],
        sizeof(dest[i]), &dest[i]
      );
    }
  }

  return Cord_AddSegment( cord, pSeg );
};

//--------------------------------------------------------------------------
// Name         AppendText 
//
// This function convers from our native(host) character set in which
// the input program is written, to the destination character set
//--------------------------------------------------------------------------
static CORD AppendText ( CORD cord, int leng, const char * text )
{
  if (bWide)
    return AppendWchars( cord, leng, text );
  else
    return AppendChars( cord, leng, text );
};

//--------------------------------------------------------------------------
// Name         AppendWChar 
//
// Append a wchar_t to the string cord. 
// Converts teh wchar to char if necessary and checks for overflow 
// in which case issues an error.
//
// overflow_bit must already be set apropriately on entry to this function.
//--------------------------------------------------------------------------
static CORD AppendWChar ( CORD cord, TARGET_WCHAR * pwc )
{
  if (bWide)
  {
    if (!overflow_bit)
      overflow_bit = TARGET_GT_UNSIGNED( pwc, &TargetMaxWChar );

    if (overflow_bit)
      *pwc = TargetMaxWChar;

    cord = Cord_NewSegment( cord, sizeof(*pwc), pwc );
  }
  else
  {
    TARGET_CHAR c;

    if (!overflow_bit)
      TARGET_ASSIGN_UNSIGNED( &c, pwc ); 

    if (!overflow_bit)
      overflow_bit = TARGET_GT_UNSIGNED( &c, &TargetMaxUChar );

    if (overflow_bit)
      c = TargetMaxUChar;

    cord = Cord_NewSegment( cord, sizeof(c), &c );
  }

  if (overflow_bit)
    ScannerError( "Value too big for a character" );

  return cord;
};

//--------------------------------------------------------------------------
// Name         OctalEscape
//
// The parameter is in format \\[0-7]{1,3}
//--------------------------------------------------------------------------
static CORD OctalEscape ( CORD cord, const char * text )
{
  TARGET_WCHAR w;
  unsigned value = 0;

  while (*++text) // preincrement to skip over the '\\'
    value = (value << 3) + *text - '0';

  ASSERT( value < 8u*8u*8u );

  TARGET_ASSIGN_HOST_UINT( &w, value );

  return AppendWChar( cord, &w );
};

//--------------------------------------------------------------------------
// Name         ConvertHex 
//
// Convert a hex string into a wchar_t.
// Sets overflow_bit on overflow.
//--------------------------------------------------------------------------
static void ConvertHex ( const char * text, TARGET_WCHAR * pwc )
{
  TARGET_CLEAR_INT( pwc );

  do
  {
    TARGET_WCHAR tgtDigit;
    UCHAR digit = tolower(*text);

    if (digit >= 'a')
      digit -= 'a' - 10;
    else
      digit -= '0';

    TARGET_SHL_UNSIGNED( pwc, pwc, 4 );
    if (overflow_bit)
      break;

    TARGET_ASSIGN_8BIT( &tgtDigit, digit );
    TARGET_ADD_UNSIGNED( pwc, pwc, &tgtDigit );

    if (overflow_bit)
      break;
  }
  while (isxdigit( *++text ));
};

//--------------------------------------------------------------------------
// Name         HexEscape 
//
// The parameter is in format: \\x{XDIGIT}+
// It is not limited to any length, so it is possible it could overflow
// any integer type. So, we gather it in a TARGET_WCHAR which can hold all
// values of TARGET_CHAR. Then if we are not in wide mode we copy the WCHAR
// to the CHAR.
//--------------------------------------------------------------------------
static CORD HexEscape ( CORD cord, const char * text )
{
  TARGET_WCHAR wc;

  // skip the "\\x"
  ConvertHex( text + 2, &wc );
  return AppendWChar( cord, &wc );
};

//--------------------------------------------------------------------------
// Name         UniCharEscape 
//
// The parameter is in format: \\[uU]{XDIGIT}{1,8}
// It is not limited to any length, so it is possible it could overflow
// any integer type. So, we gather it in a TARGET_WCHAR which can hold all
// values of TARGET_CHAR. Then if we are not in wide mode we copy the WCHAR
// to the CHAR.
//--------------------------------------------------------------------------
static CORD UniCharEscape ( CORD cord, const char * text )
{
  TARGET_WCHAR wc;

  // skip the "\\[uU]"
  ConvertHex( text + 2, &wc );

  if (!overflow_bit)
  {
    // Must validate the character
/*
       [#2]   A  universal  character  name  shall  not  specify  a
       character short identifier in  the  range  00000000  through
       00000020,  0000007F  through  0000009F,  or 0000D800 through
       0000DFFF inclusive.  A universal character  name  shall  not
       designate a character in the required character set.
*/
    unsigned uc = UINT16_FROM_TARGET( &wc );

    if (overflow_bit || uc <= 0x009F || uc >= 0xD800 && uc <= 0xDFFF)
    {
      ScannerError( "Invalid universal character \\u%04x", uc );
      
      TARGET_ASSIGN_8BIT( &wc, ' ' );
      overflow_bit = 0;
    }
  }

  return AppendWChar( cord, &wc );
};

typedef struct TUniRange { unsigned l, h; } TUniRange;

//--------------------------------------------------------------------------
// Name         CmpUniRange 
//
// 
//--------------------------------------------------------------------------
static int CmpUniRange ( const void * _key, const void * _range )
{
  unsigned * key = (unsigned *)_key;
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
void ValidateIdentUniChar ( const char * text )
{
  TARGET_WCHAR wc;

  // skip the "[uU]"
  ConvertHex( text + 1, &wc );

  if (!overflow_bit)
  {
    unsigned uc = UINT16_FROM_TARGET( &wc );
    if (!overflow_bit)
    {
      static const TUniRange ranges[] = 
      {
#include "srtuni\uni.inc"
      };

      if (bsearch( &uc, ranges, _countof(ranges), sizeof(ranges[0]), 
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
// Name         CharConst 
//
// 
//--------------------------------------------------------------------------
static int CharConst ( bool bReportEmpty, CORD cord )
{
  if (cord == NULL || cord->totalLen == 0)
  {
    if (bReportEmpty)
      ScannerError( "Empty character constant" );

    // Make a zero constant
    cord = AppendText( cord, 1, "" );
  }

  yylval.cord = cord;
  return bWide ? LCHAR_CONST : CHAR_CONST;
};

//--------------------------------------------------------------------------
// Name         StringConst 
//
// 
//--------------------------------------------------------------------------
static int StringConst ( CORD cord )
{
  if (cord == NULL)
    cord = AppendText( cord, 1, "" );

  yylval.cord = cord;
  return bWide ? LSTRING_CONST : STRING_CONST;
};

//--------------------------------------------------------------------------
// Name         Number 
//
//
// ISUFFIX  (([uU][lL]?)|([uU](ll|LL))|([lL][uU]?)|((ll|LL)[uU]?))
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
*/
static int Number ( int radix, const char * text )
{
  const char * tail;
  TARGET_LARGEST_INT_TYPE value;
  bool bOctHex;
  bool U;
  char L; // 1 - L, 2 - LL
  IntegerType_t it;

  yylval.ast = New_Ast( AST_INT_CONST, &yylloc );

  if (radix == 10)
  {
    tail = unsigned_integer_from_string( 
      sizeof(value), 
      yytext, 
      &value 
    );
  }
  else
  {
    TARGET_CLEAR_INT( &value );

    // Calculate shift 
    radix = radix == 8 ? 3 : 4;

    for ( tail = yytext; isxdigit(*tail); ++tail )
    {
      TARGET_LARGEST_INT_TYPE tgtDigit;
      UCHAR digit = tolower(*tail);

      if (digit >= 'a')
        digit -= 'a' - 10;
      else
        digit -= '0';

      TARGET_SHL_UNSIGNED( &value, &value, radix );
      if (overflow_bit)
        break;

      TARGET_ASSIGN_8BIT( &tgtDigit, digit );
      TARGET_ADD_UNSIGNED( &value, &value, &tgtDigit );
      if (overflow_bit)
        break;
    }
  }

  if (!overflow_bit)
    overflow_bit = TARGET_GT_UNSIGNED( &value, &TargetMaxLargest );

  if (overflow_bit)
  {
    ScannerError( "Too long integer constant" );
    yylval.ast->type = Signed_Int_Type;

    return NUMBER;
  }

  bOctHex = radix != 10;
  U = false;
  L = 0;

/*
  [uU][lL]?
  [uU](ll|LL)
  [lL][uU]?
  (ll|LL)[uU]?
*/
  if (tail[0] == 'u' || tail[0] == 'U')
  {
    U = true;
    if (tail[1] != 0)
    {
      if (tail[2] != 0)
        L = 2;
      else
        L = 1;
    }
  }
  else
  if (tail[0] != 0)
  {
    if (tail[1] != 'l' && tail[1] != 'L')
    {
      L = 1;

      if (tail[1] != 0)
        U = true;
    }
    else
    {
      L = 2;

      if (tail[2] != 0)
        U = true;
    }
  }

  // Determine the type.
  // First pass over the standard types only
  for ( it = 0; i < IT_LAST; ++i )
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

      if (TARGET_LE_UNSIGNED( &value, IntegerTypesDesc[it].maxValue)
        break;
    }
  }

  if (it == IT_LAST) // if we didn't find a match, search the extended types
    for ( it = 0; i < IT_LAST; ++i )
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

        if (TARGET_LE_UNSIGNED( &value, IntegerTypesDesc[it].maxValue)
          break;
      }
    }

  if (it == IT_LAST) // if we didn't find a match again, try them all
    for ( it = 0; i < IT_LAST; ++i )
    {
      if (TARGET_LE_UNSIGNED( &value, IntegerTypesDesc[it].maxValue)
        break;
    }

  if (it == IT_LAST)
  {
    ScannerError( "Too long integer constant" );
    yylval.ast->type = Signed_Int_Type;

    return NUMBER;
  }

  yylval.ast->type = IntegerTypes[it];
  yylval.ast->u.intConst = value;

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
    yylval.ast = New_Ast( AST_FLOAT_CONST, &yylloc );
    IEEE_single_from_string( yytext, &yylval.ast->u.fltConst );
    break;

  default:
    yylval.ast = New_Ast( AST_DOUBLE_CONST, &yylloc );
    IEEE_double_from_string( yytext, &yylval.ast->u.dblConst );
    break;

  case 'l': case 'L':
    yylval.ast = New_Ast( AST_LONGDOUBLE_CONST, &yylloc );
    IEEE_double_from_string( yytext, &yylval.ast->u.ldblConst );
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

  // skip over "0[xX]"

  if ( (text = HexStringToDouble( text + 2, &res )) == NULL)
  {
    ScannerError( "Hexadecimal foating point overflow" );
    return F_Number( 3, "0.0" );
  }

  switch (*text)
  {
  case 'f': case 'F':
    yylval.ast = New_Ast( AST_FLOAT_CONST, &yylloc );
    yylval.ast->u.fltConst = IEEE_double_to_single( res );
    break;

  default:
    yylval.ast = New_Ast( AST_DOUBLE_CONST, &yylloc );
    yylval.ast->u.dblConst = res;
    break;

  case 'l': case 'L':
    yylval.ast = New_Ast( AST_LONGDOUBLE_CONST, &yylloc );
    yylval.ast->u.ldblConst = res;
    break;
  }

  return F_NUMBER;
};

%}

%option outfile="scan.c"  
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
ISUFFIX  (([uU][lL]?)|([uU](ll|LL))|([lL][uU]?)|((ll|LL)[uU]?))

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
  int  prevState;  // PLAIN_STR_STATE returns to this one instead of INITIAL


"/*"             UPDATE_COL(); BEGIN( COMMENT );
"//"             UPDATE_COL(); BEGIN( LINE_COMMENT );
{WS}             UPDATE_COL(); /* eat up whitespace */
{NL}             INC_LINE();

^{WS}?"#"        SET_POS(); cord = NULL; BEGIN(PREPR_DIR);

\\               SET_POS(); cord = NULL; BEGIN(ID_STATE1);

_Pragma{WS}?    {
                  /* So here we go .. :-) We parse manually _Pragma(string) */
                  int c;

                  SET_POS();

                  c = input();
                  if (c != '(')
                  {
                    ScannerError( "'(' expected after _Pragma" );
                    goto exit;
                  }

                  // Skip the blanks
                  while (isspace( c = input() ))
                    {}

                  if (c == 'l' || c == 'L')
                    c = input();

                  if (c != '"')
                  {
                    ScannerError( "A string expected after _Pragma(" );
                    goto exit;
                  }

                  prevState = PRAGMA_OP_END;
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
                                 STRING str = NewStringLen( yytext, yyleng ); 

                                 SET_POS();

                                 ASSERT( bWordsAdded );
                                 if (STRING_DATA(str)->namesp.token != 0)
                                   return SCANNER_SET_TC( STRING_DATA(str)->namesp.token );
                                 else
                                 {
                                   yylval.str = str;
                                   return IDENT;
                                 }
                               }

0[0-7]*{ISUFFIX}?              SET_POS(); return Number( 8, yytext ); 

0[0-9]+{ISUFFIX}?              {
                                 SET_POS();
                                 ScannerError( "Bad octal number" ); 
                                 return Number( 8, "0" );
                               }

[1-9][0-9]*{ISUFFIX}?          SET_POS(); return Number( 10, yytext ); 

0[xX]{XDIGIT}+{ISUFFIX}?       SET_POS(); return Number( 16, yytext+2 );
0[xX]{ISUFFIX}?                {
                                 SET_POS();
                                 ScannerError( "Invalid hex number" ); 
                                 return Number( 16, "0" );
                               }

{FRACT}{FSUFFIX}?              SET_POS(); return F_Number( yyleng, yytext );

{FRACT_OR_INT}[eE][+-]?[0-9]+{FSUFFIX}?  SET_POS(); return F_Number( yyleng, yytext );

{FRACT_OR_INT}[eE][+-]?        {
                                 SET_POS(); 

                                 ScannerError( "Number expected after exponent" ); 
                                 return F_Number( 3, "0.0" );
                               }

0[xX]{HEXFRACT_OR_INT}[pP][+-]?[0-9]+{FSUFFIX}?   {
                                 SET_POS(); 
                                 return F_HexNumber( yyleng, yytext );
                               }

0[xX]{HEXFRACT_OR_INT}[pP][+-]?  {
                                 SET_POS();
                                 ScannerError( "Bad floating point constant" ); 
                                 return F_Number( 3, "0.0" );
                               }

0[xX]{HEXFRACT}                {
                                 SET_POS();
                                 ScannerError( "Bad floating point constant" ); 
                                 return F_Number( 3, "0.0" );
                               }
0[xX].                         {
                                 SET_POS();
                                 ScannerError( "Bad floating point constant" ); 
                                 return F_Number( 3, "0.0" );
                               }

\'       SET_POS(); bWide = false; BEGIN( CHAR_STATE ); cord = NULL; 
L\'      SET_POS(); bWide = true;  BEGIN( CHAR_STATE ); cord = NULL; 
\"       SET_POS(); bWide = false; BEGIN( STR_STATE ); cord = NULL; 
L\"      SET_POS(); bWide = true;  BEGIN( STR_STATE ); cord = NULL; 

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
".."     SET_POS(); ScannerError( "'..' is invalid. Did you mean '...'?" ); return ELLIPSIS;
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

.        SET_POS(); ScannerError( "Bad character '\\x%02x'", *yytext );


<CHAR_STATE>{
      \'                {
                          UPDATE_COL();
                          BEGIN(INITIAL); 
                          return CharConst( true, cord );
                        }

      \\[\'\"\?\\]      UPDATE_COL(); cord = AppendText( cord, 1, yytext+1 );
      \\a               UPDATE_COL(); cord = AppendText( cord, 1, "\a" );
      \\b               UPDATE_COL(); cord = AppendText( cord, 1, "\b" );
      \\f               UPDATE_COL(); cord = AppendText( cord, 1, "\f" );
      \\n               UPDATE_COL(); cord = AppendText( cord, 1, "\n" );
      \\r               UPDATE_COL(); cord = AppendText( cord, 1, "\r" );
      \\t               UPDATE_COL(); cord = AppendText( cord, 1, "\t" );
      \\v               UPDATE_COL(); cord = AppendText( cord, 1, "\v" );
      \\e               UPDATE_COL(); cord = AppendText( cord, 1, "\x1b" ); // GNU C extension
      {OCTAL_ESC}       UPDATE_COL(); cord = OctalEscape( cord, yytext );
      {HEX_ESC}         UPDATE_COL(); cord = HexEscape( cord, yytext );
      {UCHAR}           UPDATE_COL(); cord = UniCharEscape( cord, yytext );
      {BADUCHAR}        UPDATE_COL(); ScannerError( "Ill-formed universal character" );

      \\{NL}            INC_LINE(); // concat lines
      \\.               UPDATE_COL(); ScannerError( "Bad escape character" );

      <<EOF>>           { 
                          ScannerError( "Unterminated character constant" ); 
                          BEGIN(INITIAL); 
                          return CharConst( false, cord );
                        } 

      {NL}              { 
                          ScannerError( "Unterminated character constant" ); 
                          INC_LINE(); 
                          BEGIN(INITIAL); 
                          return CharConst( false, cord );
                        } 

      [^\'\\\n]+        UPDATE_COL(); cord = AppendText( cord, yyleng, yytext );
      .                 UPDATE_COL(); cord = AppendText( cord, 1, yytext ); ASSERT(0); // Shouldn't happen
}

<STR_STATE>{
      \"                {
                          UPDATE_COL();
                          BEGIN(INITIAL); 
                          return StringConst( cord );
                        }

      \\[\'\"\?\\]      UPDATE_COL(); cord = AppendText( cord, 1, yytext+1 );
      \\a               UPDATE_COL(); cord = AppendText( cord, 1, "\a" );
      \\b               UPDATE_COL(); cord = AppendText( cord, 1, "\b" );
      \\f               UPDATE_COL(); cord = AppendText( cord, 1, "\f" );
      \\n               UPDATE_COL(); cord = AppendText( cord, 1, "\n" );
      \\r               UPDATE_COL(); cord = AppendText( cord, 1, "\r" );
      \\t               UPDATE_COL(); cord = AppendText( cord, 1, "\t" );
      \\v               UPDATE_COL(); cord = AppendText( cord, 1, "\v" );
      \\e               UPDATE_COL(); cord = AppendText( cord, 1, "\x1b" ); // GNU C extension
      {OCTAL_ESC}       UPDATE_COL(); cord = OctalEscape( cord, yytext );
      {HEX_ESC}         UPDATE_COL(); cord = HexEscape( cord, yytext );
      {UCHAR}           UPDATE_COL(); cord = UniCharEscape( cord, yytext );
      {BADUCHAR}        UPDATE_COL(); ScannerError( "Ill-formed universal character" );

      \\{NL}            INC_LINE(); // concat lines
      \\.               UPDATE_COL(); ScannerError( "Bad escape character" );

      <<EOF>>           { 
                          ScannerError( "Unterminated string constant" ); 
                          BEGIN(INITIAL); 
                          return StringConst( cord );
                        } 

      {NL}              { 
                          ScannerError( "Unterminated string constant" ); 
                          INC_LINE(); 
                          BEGIN(INITIAL); 
                          return StringConst( cord );
                        } 

      [^\"\\\n]+        UPDATE_COL(); cord = AppendText( cord, yyleng, yytext );
      .                 UPDATE_COL(); cord = AppendText( cord, 1, yytext ); ASSERT(0); // Shouldn't happen
}

<ID_STATE1>{
      {UCHAR1}          {
                          UPDATE_COL();
                          ValidateIdentUniChar( yytext );
                          cord = Cord_NewSegment( cord, yyleng, yytext );
                          BEGIN(ID_STATE2);
                        }

      {BADUCHAR1}       UPDATE_COL(); ScannerError( "Ill-formed universal character" );BEGIN(ID_STATE2);

      .                 {
                          UPDATE_COL();  
  												ScannerError( "Ill-formed universal character" );
                          yyless(1); // put back the character
													BEGIN(ID_STATE2); 
                        }

      {NL}              {
                          INC_LINE();  
													ScannerError( "Ill-formed universal character" );
													BEGIN(INITIAL); 
                          
                          yylval.str = StringFromCord( cord );

													return IDENT;
                        }
}

<ID_STATE2>{
      {UCHAR}           { 
                          UPDATE_COL(); 
                          ValidateIdentUniChar( yytext+1 );
                          cord = Cord_NewSegment( cord, yyleng, yytext );
                        }

      {BADUCHAR}        UPDATE_COL(); ScannerError( "Ill-formed universal character" );
      \\                UPDATE_COL(); ScannerError( "Ill-formed universal character" );

      [[:alnum:]_]+     UPDATE_COL(); cord = Cord_NewSegment( cord, yyleng, yytext );

      .                 { 
                          UPDATE_COL(); 
                          BEGIN(INITIAL); 
                          
                          yylval.str = StringFromCord( cord );

													return IDENT;
                        }

      {NL}              { 
                          INC_LINE(); 
													BEGIN(INITIAL); 
                          
                          yylval.str = StringFromCord( cord );

													return IDENT;
                        }
}

<PLAIN_STR_STATE>{
      \"                BEGIN(prevState); 

      \\[\'\"\?\\]      cord = Cord_NewSegment( cord, 1, yytext+1 );
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

      \\{NL}            INC_LINE(); // concat lines
      \\.               ScannerError( "Bad escape character" );

      <<EOF>>           { 
                          ScannerError( "Unterminated string constant" ); 
                          BEGIN(prevState); 
                        } 

      {NL}              { 
                          ScannerError( "Unterminated string constant" ); 
                          INC_LINE();
                          BEGIN(prevState); 
                        } 

      [^\"\\\n]+        cord = Cord_NewSegment( cord, yyleng, yytext );
      .                 cord = Cord_NewSegment( cord, 1, yytext ); ASSERT(0); // Shouldn't happen
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
        <<EOF>>                     BEGIN(INITIAL); ScannerPrepr( cord ); return 0;
        {NL}            INC_LINE(); BEGIN(INITIAL); ScannerPrepr( cord );

        \\{NL}          // concat lines 
        .+              cord = Cord_NewSegment( cord, yyleng, yytext );
}

<COMMENT>{
        <<EOF>>         ScannerError( "Comment not closed" ); BEGIN(INITIAL); return 0;
        "*/"            BEGIN(INITIAL);
        {NL}            INC_LINE();
        .+               
}

<LINE_COMMENT>{
        <<EOF>>         BEGIN(INITIAL); return 0;
        \\{NL}          // concat lines 
        {NL}            INC_LINE(); BEGIN(INITIAL); 
        .+             
}


%%

//--------------------------------------------------------------------------
// Name         Scanner_Init 
//
// 
//--------------------------------------------------------------------------
void Scanner_Init ()
{
  InitReservedWords();

  CurPos.fileName = NULL;
  CurPos.line = 1;
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

#ifdef TEST_YYLEX

void ScannerPrepr ( CORD cord )
{
  extern int Prepr_Parse ( CORD cord );

  // Just for now
  if (cord)
    printf( "# %s\n", C_STR( StringFromCord( cord ) ) );

  Prepr_Parse( cord );
};

#include <stdarg.h>
void ScannerError ( const char * str, ... )
{
  va_list ap;
  va_start( ap, str );

  fprintf( stderr, "error line %d:", yylpos.line );
  vfprintf( stderr, str, ap );
  printf( "\n" );

  va_end( ap );
};

void ScannerWarning ( const char * str, ... )
{
  va_list ap;
  va_start( ap, str );

  fprintf( stderr, "warning line %d:", yylpos.line );
  vfprintf( stderr, str, ap );
  printf( "\n" );

  va_end( ap );
};

YYSTYPE yylval;

int main ( void )
{
  int c;

  Strings_Init();
  TConf_Init();
  Scanner_Init();

  while (c = yylex())
  {
    printf( "%s = %d ", yytext, c );
    switch (c)
    {
      case IDENT: 
        printf( " %p=%s", yylval.str, C_STR(yylval.str) ); 
        break;

      case CHAR_CONST: 
      case LCHAR_CONST: 
      case STRING_CONST:
      case LSTRING_CONST:
        if (c == LCHAR_CONST || c == LSTRING_CONST)
          printf( "L " );
        {
          CORD cord;
          for ( cord = yylval.cord; cord != NULL; cord = cord->pNext )
          {
            int i;
            for ( i = 0; i < cord->segLen; ++i )
            {
              TARGET_CHAR * t = (TARGET_CHAR *)cord->u.data + i;
            }
          }
        }
        break;
    }

    printf( "\n" );
  }

  Strings_Done();
  return 0;
};

#endif // TEST_YYLEX
