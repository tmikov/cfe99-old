/*
  lextabs.c: generates help tables for the lexer:
    - the trigraph table:
    - the escape character table
*/

#include <stdio.h>

char array[128];

//--------------------------------------------------------------------------
// Name         GenTrigraph
//
// Generates and prints the trigraph table. It maps from a trigraph
// character "??x" to its equivalent.
// The table is optimized: it does not contain any characters before
// the first valid trigraph and any characters after the last valid one.
//--------------------------------------------------------------------------
static void GenTrigraph ( void )
{
  int i, first, last;

  for ( i = 0; i < 128; ++i )
    array[i] = 0;

  array['='] = '#';
  array['('] = '[';
  array['/'] = '\\';
  array[')'] = ']';
  array['\''] = '^';
  array['<'] = '{';
  array['!'] = '|';
  array['>'] = '}';
  array['-'] = '~';

  // find the first non-zero
  for ( first = 0; array[first] == 0; ++first )

  // find the last non-zero
  for ( last = 127; array[last] == 0; --last );

  printf( "#define FIRST_TRIGRAPH %d\n", first );
  printf( "#define NUM_TRIGRAPHS  %d\n", last - first + 1 );
  printf( "static char TrigraphTable[NUM_TRIGRAPHS] =\n{\n  " );
  for ( i = first; i <= last; ++i )
  {
    if (array[i] == '\\' || array[i] == '\'')
      printf( "'\\%c'", array[i] );
    else
    if (array[i])
      printf( "'%c'", array[i] );
    else
      printf( "0");
    if (i != last)
      printf( "," );
  }
  printf( "\n};\n\n" );
};

//--------------------------------------------------------------------------
// Name         GenEscape
//
// Generates the escape character table. It maps from an escape character
// '\x' to its equivalent.
// The table is optimized: it does not contain any characters before
// the first valid escape and any characters after the last valid one.
//--------------------------------------------------------------------------
static void GenEscape ( void )
{
  int i, first, last;

  for ( i = 0; i < 128; ++i )
    array[i] = 0;

  array['a'] = 'a';
  array['b'] = 'b';
  array['f'] = 'f';
  array['n'] = 'n';
  array['r'] = 'r';
  array['t'] = 't';
  array['v'] = 'v';

  // find the first non-zero
  for ( first = 0; array[first] == 0; ++first )

  // find the last non-zero
  for ( last = 127; array[last] == 0; --last );

  printf( "#define FIRST_ESCAPE %d\n", first );
  printf( "#define NUM_ESCAPES  %d\n", last - first + 1 );
  printf( "static char EscapeTable[NUM_ESCAPES] =\n{\n  " );
  for ( i = first; i <= last; ++i )
  {
    if (array[i])
      printf( "'\\%c'", array[i] );
    else
      printf( "0");
    if (i != last)
      printf( "," );
  }
  printf( "\n};\n\n" );
};

//--------------------------------------------------------------------------
// Name         main
//
//
//--------------------------------------------------------------------------
void main ( void )
{
  GenTrigraph();
  GenEscape();
};

