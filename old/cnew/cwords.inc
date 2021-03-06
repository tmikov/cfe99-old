static TRESWORD ResWordsTable[] =
{
  { "short", (int)-1 },
  { "__restrict", (int)-14 },
  { "__declspec", (int)DECLSPEC },
  { "signed", (int)-2 },
  { "struct", (int)STRUCT },
  { "static", (int)-17 },
  { "sizeof", (int)SIZEOF },
  { "while", (int)WHILE },
  { "__attr", (int)ATTR },
  { "int", (int)-5 },
  { "void", (int)-4 },
  { "volatile", (int)-12 },
  { "const", (int)-11 },
  { "switch", (int)SWITCH },
  { "if", (int)IF },
  { "continue", (int)CONTINUE },
  { "double", (int)-3 },
  { "case", (int)CASE },
  { "typedef", (int)-19 },
  { "default", (int)DEFAULT },
  { "char", (int)-6 },
  { "float", (int)-9 },
  { "else", (int)ELSE },
  { "for", (int)FOR },
  { "restrict", (int)-13 },
  { "extern", (int)-15 },
  { "_Bool", (int)-10 },
  { "unsigned", (int)-8 },
  { "do", (int)DO },
  { "goto", (int)GOTO },
  { "enum", (int)ENUM },
  { "register", (int)-18 },
  { "auto", (int)-16 },
  { "break", (int)BREAK },
  { "return", (int)RETURN },
  { "union", (int)UNION },
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
     -5, // '_'
  SCHAR_MIN, // '`'
      5, // 'a'
     -7, // 'b'
      6, // 'c'
      7, // 'd'
      8, // 'e'
     10, // 'f'
      8, // 'g'
     19, // 'h'
      4, // 'i'
  SCHAR_MIN, // 'j'
     36, // 'k'
     27, // 'l'
     22, // 'm'
     15, // 'n'
     20, // 'o'
  SCHAR_MIN, // 'p'
  SCHAR_MIN, // 'q'
     12, // 'r'
     -7, // 's'
      5, // 't'
     20, // 'u'
      2, // 'v'
     -3, // 'w'
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
    case 10: { if (opt->bC9X) RET_ITOK( TOK_BOOL, TYPESPEC ); break; };
    case 11: {RET_ITOK( TOK_CONST, TYPEQUAL );};
    case 12: {RET_ITOK( TOK_VOLATILE, TYPEQUAL );};
    case 13: { if (opt->bC9X) RET_ITOK( TOK_RESTRICT, TYPEQUAL ); break; };
    case 14: {RET_ITOK( TOK_RESTRICT, TYPEQUAL );};
    case 15: {RET_ITOK( TOK_EXTERN, SCLASS );};
    case 16: {RET_ITOK( TOK_AUTO, SCLASS );};
    case 17: {RET_ITOK( TOK_STATIC, SCLASS );};
    case 18: {RET_ITOK( TOK_REGISTER, SCLASS );};
    case 19: {RET_ITOK( TOK_TYPEDEF, SCLASS );};
  }
  return 0;
};

