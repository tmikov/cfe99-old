static TRESWORD ResWordsTable[] =
{
  { "short", (int)-1 },
  { "static", (int)-14 },
  { "signed", (int)-2 },
  { "continue", (int)CONTINUE },
  { "struct", (int)STRUCT },
  { "case", (int)CASE },
  { "else", (int)ELSE },
  { "double", (int)-3 },
  { "const", (int)-10 },
  { "sizeof", (int)SIZEOF },
  { "char", (int)-6 },
  { "volatile", (int)-11 },
  { "switch", (int)SWITCH },
  { "while", (int)WHILE },
  { "int", (int)-5 },
  { "do", (int)DO },
  { "extern", (int)-12 },
  { "void", (int)-4 },
  { "default", (int)DEFAULT },
  { "register", (int)-15 },
  { "for", (int)FOR },
  { "typedef", (int)-16 },
  { "if", (int)IF },
  { "unsigned", (int)-8 },
  { "float", (int)-9 },
  { "__declspec", (int)DECLSPEC },
  { "goto", (int)GOTO },
  { "return", (int)RETURN },
  { "auto", (int)-13 },
  { "__attr", (int)ATTR },
  { "enum", (int)ENUM },
  { "union", (int)UNION },
  { "break", (int)BREAK },
  { "long", (int)-7 },
};

#define RES_C_WORD_MIN_LEN  2

#define RES_C_WORD_MAX_LEN  10

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
     22, // '_'
  SCHAR_MIN, // '`'
     13, // 'a'
     -7, // 'b'
      2, // 'c'
      6, // 'd'
      0, // 'e'
     13, // 'f'
     17, // 'g'
     18, // 'h'
      9, // 'i'
  SCHAR_MIN, // 'j'
     35, // 'k'
     15, // 'l'
     30, // 'm'
     14, // 'n'
      8, // 'o'
  SCHAR_MIN, // 'p'
  SCHAR_MIN, // 'q'
      6, // 'r'
     -7, // 's'
      5, // 't'
     17, // 'u'
     10, // 'v'
     11, // 'w'
  SCHAR_MIN, // 'x'
  SCHAR_MIN, // 'y'
  SCHAR_MIN, // 'z'
};

#ifndef RESWORDACTION_FUNC
#define RESWORDACTION_FUNC static int ResWordAction ( int index )
#endif
RESWORDACTION_FUNC
{
  switch (index)
  {
    case 1: {RET_ITOK( TOK_SHORT, TYPESPEC );};
    case 2: {RET_ITOK( TOK_SIGNED, TYPESPEC );};
    case 3: {RET_ITOK( TOK_DOUBLE, TYPESPEC );};
    case 4: {RET_ITOK( TOK_VOID, TYPESPEC );};
    case 5: {RET_ITOK( TOK_INT, TYPESPEC );};
    case 6: {RET_ITOK( TOK_CHAR, TYPESPEC );};
    case 7: {RET_ITOK( TOK_LONG, TYPESPEC );};
    case 8: {RET_ITOK( TOK_UNSIGNED, TYPESPEC );};
    case 9: {RET_ITOK( TOK_FLOAT, TYPESPEC );};
    case 10: {RET_ITOK( TOK_CONST, TYPEQUAL );};
    case 11: {RET_ITOK( TOK_VOLATILE, TYPEQUAL );};
    case 12: {RET_ITOK( TOK_EXTERN, SCLASS );};
    case 13: {RET_ITOK( TOK_AUTO, SCLASS );};
    case 14: {RET_ITOK( TOK_STATIC, SCLASS );};
    case 15: {RET_ITOK( TOK_REGISTER, SCLASS );};
    case 16: {RET_ITOK( TOK_TYPEDEF, SCLASS );};
  }
  return 0;
};

