static struct TResWord ResWordsTable[] =
{
  { "short", LEX_C_SHORT },
  { "signed", LEX_C_SIGNED },
  { "switch", LEX_C_SWITCH },
  { "static", LEX_C_STATIC },
  { "struct", LEX_C_STRUCT },
  { "_xseg", LEX_C__XSEG },
  { "_iseg", LEX_C__ISEG },
  { "_bseg", LEX_C__BSEG },
  { "_cseg", LEX_C__CSEG },
  { "_dseg", LEX_C__DSEG },
  { "const", LEX_C_CONST },
  { "continue", LEX_C_CONTINUE },
  { "double", LEX_C_DOUBLE },
  { "case", LEX_C_CASE },
  { "sizeof", LEX_C_SIZEOF },
  { "void", LEX_C_VOID },
  { "volatile", LEX_C_VOLATILE },
  { "default", LEX_C_DEFAULT },
  { "else", LEX_C_ELSE },
  { "int", LEX_C_INT },
  { "char", LEX_C_CHAR },
  { "while", LEX_C_WHILE },
  { "long", LEX_C_LONG },
  { "extern", LEX_C_EXTERN },
  { "unsigned", LEX_C_UNSIGNED },
  { "do", LEX_C_DO },
  { "typedef", LEX_C_TYPEDEF },
  { "auto", LEX_C_AUTO },
  { "enum", LEX_C_ENUM },
  { "float", LEX_C_FLOAT },
  { "goto", LEX_C_GOTO },
  { "break", LEX_C_BREAK },
  { "if", LEX_C_IF },
  { "for", LEX_C_FOR },
  { "union", LEX_C_UNION },
  { "register", LEX_C_REGISTER },
  { "return", LEX_C_RETURN },
};

#define RES_C_WORD_MAX_LEN  8

static signed char HashCharValue['z' - '0' + 1]=
{
  SCHAR_MIN, // '0'
  SCHAR_MIN, // '1'
  SCHAR_MIN, // '2'
  SCHAR_MIN, // '3'
  SCHAR_MIN, // '4'
  SCHAR_MIN, // '5'
  SCHAR_MIN, // '6'
  SCHAR_MIN, // '7'
  SCHAR_MIN, // '8'
  SCHAR_MIN, // '9'
  SCHAR_MIN, // ':'
  SCHAR_MIN, // ';'
  SCHAR_MIN, // '<'
  SCHAR_MIN, // '='
  SCHAR_MIN, // '>'
  SCHAR_MIN, // '?'
  SCHAR_MIN, // '@'
  SCHAR_MIN, // 'A'
  SCHAR_MIN, // 'B'
  SCHAR_MIN, // 'C'
  SCHAR_MIN, // 'D'
  SCHAR_MIN, // 'E'
  SCHAR_MIN, // 'F'
  SCHAR_MIN, // 'G'
  SCHAR_MIN, // 'H'
  SCHAR_MIN, // 'I'
  SCHAR_MIN, // 'J'
  SCHAR_MIN, // 'K'
  SCHAR_MIN, // 'L'
  SCHAR_MIN, // 'M'
  SCHAR_MIN, // 'N'
  SCHAR_MIN, // 'O'
  SCHAR_MIN, // 'P'
  SCHAR_MIN, // 'Q'
  SCHAR_MIN, // 'R'
  SCHAR_MIN, // 'S'
  SCHAR_MIN, // 'T'
  SCHAR_MIN, // 'U'
  SCHAR_MIN, // 'V'
  SCHAR_MIN, // 'W'
  SCHAR_MIN, // 'X'
  SCHAR_MIN, // 'Y'
  SCHAR_MIN, // 'Z'
  SCHAR_MIN, // '['
  SCHAR_MIN, // '\'
  SCHAR_MIN, // ']'
  SCHAR_MIN, // '^'
     -7, // '_'
  SCHAR_MIN, // '`'
      1, // 'a'
     -7, // 'b'
      4, // 'c'
      5, // 'd'
      6, // 'e'
     18, // 'f'
     10, // 'g'
      8, // 'h'
     14, // 'i'
  SCHAR_MIN, // 'j'
     34, // 'k'
     11, // 'l'
     22, // 'm'
     15, // 'n'
     19, // 'o'
  SCHAR_MIN, // 'p'
  SCHAR_MIN, // 'q'
     14, // 'r'
     -7, // 's'
      5, // 't'
     19, // 'u'
      9, // 'v'
     13, // 'w'
  SCHAR_MIN, // 'x'
  SCHAR_MIN, // 'y'
  SCHAR_MIN, // 'z'
};

