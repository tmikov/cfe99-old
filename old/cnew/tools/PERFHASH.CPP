#include "global.h"

#define NO_VALUE   (int)(-1000)

/*
  In our terms First and Last can be any letter.
  Len can be any parameter of the word 0 < len < 32
*/

struct Word
{
  char text[16];
  int  len;

#if ORIGINAL_PHASH
  int  First ( void ) const { return text[0]; };
  int  Last ( void ) const  { return text[len - 1]; };
  int  Len ( void ) const   { return len; };
#else
  int  First ( void ) const { return text[0]; };
  int  Last ( void ) const  { return text[len - 1]; };
  int  Len ( void ) const   { return (text[1] - '_' + 1) & 7; };
#endif
};

extern Word words[];

const char * fileName;

UINT NumWords;
int  value[CHAR_MAX+1]; // = NO_VALUE
int maxTableSize;
BOOL buckets[1000]; // = FALSE
int minVal; // = -MaxLen
int maxVal; // = maxTableSize - MinLen

int MaxLen, MinLen;

BOOL stopFirst;  // = TRUE
BOOL stopMinimal; // = FALSE
BOOL stopNow;   // = FALSE

void GenFile ( const char * fileName )
{
  int i, maxStrLen;
  FILE * f;

  f = fopen( fileName, "wt" );
  if (f == NULL)
    return;

  fprintf( f, "static struct TResWord ResWordsTable[] =\n{\n" );
  for( maxStrLen = i = 0; i < NumWords; ++i )
  {
    char buf[20];
    if (strlen( words[i].text ) > maxStrLen)
      maxStrLen = strlen( words[i].text );
    strupr( strcpy( buf, words[i].text ) );
    fprintf( f, "  { \"%s\", LEX_C_%s },\n", words[i].text, buf );
  }
  fprintf( f, "};\n\n" );
  fprintf( f, "#define RES_C_WORD_MAX_LEN  %d\n\n", maxStrLen );

  fprintf( f, "static signed char HashCharValue['z' - '0' + 1]=\n{\n" );
  for( i = '0'; i <= 'z'; ++i )
    if (value[i] != NO_VALUE)
      fprintf( f, "  %5d, // '%c'\n", value[i], i );
    else
      fprintf( f, "  SCHAR_MIN, // '%c'\n", i );
  fprintf( f, "};\n\n" );

#if 0
  for( i = 0; i < NumWords; ++i )
  {
    char buf[20];
    strupr( strcpy( buf, words[i].text ) );
    fprintf( f, "#define LEX_C_%-20s LEX_CBASE + %2d\n", buf, i );
  }  

  fprintf( f, "\n#define LEX_CLAST LEX_CBASE + %2d\n\n", NumWords - 1 );
#endif

  fclose( f );
};

void InitWords ( void )
{
  MaxLen = -10000;
  MinLen = 10000;
  for( int i = 0; i < NumWords; ++i )
    words[i].len = strlen( words[i].text );

  for( i = 0; i < NumWords; ++i )
  {
    if (words[i].Len() > MaxLen)
      MaxLen = words[i].Len();
    else
    if (words[i].Len() < MinLen)
      MinLen = words[i].Len();
  }

#ifdef _DEBUG
  TRACE0( "Initial listing of words:\n" );
  for( i = 0; i < NumWords; ++i )
    TRACE1( "  %s\n", words[i].text );
#endif
};

int Hash ( const Word & w )
{
  return value[w.First()] + value[w.Last()] + w.Len();
};

BOOL CheckWords ( void )
{
  for ( int i = 0; i < NumWords; ++i )
  {
    for ( int j = 0; j < i; ++j )
      if (words[i].First() == words[j].First() &&
          words[i].Last() == words[j].Last() &&
          words[i].Len() == words[j].Len())
      {
        printf( "Conflicting hash for '%s' - '%s'\n",
                words[i].text, words[j].text );
        return FALSE;
      }
  }
  return TRUE;    
};

int WordsCard ( void )
{
  return NumWords;
};

int CmpWords ( const void * a, const void * b )
{
  int h1 = Hash( *((const Word *)a) );
  int h2 = Hash( *((const Word *)b) );

  if (h1 < h2)
    return -1;
  if (h1 == h2)
    return 0;
  return 1;
};

int FirstFreeBucket ( void )
{
  for( int i = 0; i < maxTableSize; ++i )
    if (!buckets[i])
      return i;
  ASSERT( FALSE );
  return 0;
};

int LastFreeBucket ( void )
{
  for( int i = maxTableSize - 1; i >= 0; --i )
    if (!buckets[i])
      return i;
  ASSERT( FALSE );
  return maxTableSize - 1;
};

void MarkBucket ( int i )
{
  ASSERT( !buckets[i] );
  buckets[i] = TRUE;
};

void FreeBucket ( int i )
{
  ASSERT( buckets[i] );
  buckets[i] = FALSE;
};

#define MAX( a, b )  ((a) > (b) ? (a) : (b))

/*
  Calculate the freq. of first and last letters
  Order the keys in decreasing order of the sum of the freq-s
*/
float freqFirst[CHAR_MAX+1];
float freqLast[CHAR_MAX+1];

int CmpWordsFreq ( const void * a, const void * b )
{
  Word * w1 = (Word *)a;
  Word * w2 = (Word *)b;
  float f1 = freqFirst[w1->First()] + freqLast[w1->Last()];
  float f2 = freqFirst[w2->First()] + freqLast[w2->Last()];

  if (f1 < f2)
    return 1;
  if (f1 == f2)
    return 0;
  return -1;
};

void ReorderFreq ( void )
{
  UINT  countFirst[CHAR_MAX+1];
  UINT  countLast[CHAR_MAX+1];

  memset( countFirst, 0, sizeof( countFirst ) );
  memset( countLast, 0, sizeof( countLast ) );

  for( int i = 0; i < NumWords; ++i )
  {
    ++countFirst[words[i].First()];
    ++countLast[words[i].Last()];
  }

  for( i = 'A'; i <= 'z'; ++i )
  {
    freqFirst[i] = (double)countFirst[i] / NumWords;
    freqLast[i] = (double)countLast[i] / NumWords;
  }

  qsort( words, NumWords, sizeof( words[0] ), CmpWordsFreq );

#ifdef _DEBUG
  TRACE0( "\n\nFreq reordered listing of words:\n" );
  for( i = 0; i < NumWords; ++i )
    TRACE1( "  %s\n", words[i].text );
#endif
};

void Reorder ( void )
{
  const unsigned NO_OCCURENCE = INT_MAX;
  for ( UINT i = 1; i < NumWords; ++i )
  {
    const Word & w = words[i];
    int first = w.First();
    int last  = w.Last();
    UINT firstOccIndex = NO_OCCURENCE;
    UINT lastOccIndex = NO_OCCURENCE;

    for( UINT j = 0; j < i; ++j )
    {
      const Word & prev = words[j];

      if (lastOccIndex == NO_OCCURENCE &&
          (prev.Last() == last || prev.First() == last))
         lastOccIndex = j;
      if (firstOccIndex == NO_OCCURENCE &&
          (prev.Last() == first || prev.First() == first))
         firstOccIndex = j;
      if (firstOccIndex != NO_OCCURENCE && lastOccIndex != NO_OCCURENCE)
      {
        UINT firstToMoveDown = MAX( firstOccIndex, lastOccIndex ) + 1;
        UINT lastToMoveDown = i - 1;
        Word itemToMoveUp = words[i];
        for( UINT k = lastToMoveDown; k >= firstToMoveDown; --k )
          words[k+1] = words[k];
        words[firstToMoveDown] = itemToMoveUp;
        break;
      }
    }
  }
#ifdef _DEBUG
  TRACE0( "\n\nReordered listing of words:\n" );
  for( i = 0; i < NumWords; ++i )
    TRACE1( "  %s\n", words[i].text );
#endif
};

BOOL Search ( UINT index )
{
  const Word & w = words[index];

  if (value[w.First()] != NO_VALUE && value[w.Last()] != NO_VALUE)
  {
    int h = Hash( w );
    if (h < 0 || h >= maxTableSize || buckets[h] )
      return FALSE;
    else
    {
      MarkBucket( h );
      ++index;

      int card = WordsCard();

      if (index == card)
      {
        puts( "------Perfect hashing found----\n" );
        for( int i = 0; i <= CHAR_MAX; ++i )
          if (value[i] != NO_VALUE)
            printf( "%c = %d  ", i, value[i] );
        puts("");
        for( i = 0; i < card; ++i )
          printf( "%s = %d\n", words[i].text, Hash( words[i] ) );
        int used = 0;
        for( i = 0; i < maxTableSize; ++i )
          if (buckets[i])
            ++used;
        printf( "loading factor: %f\n", (float)used/maxTableSize );

        if (fileName)
        {
          qsort( words, NumWords, sizeof( words[0] ), CmpWords );
          GenFile( fileName );
          stopNow = TRUE;
          return TRUE;      // my little patch here...
        }

        stopNow = stopFirst || (used == maxTableSize && stopMinimal);
        FreeBucket( h );
        return TRUE;
      }
      else
      {
        BOOL res = Search( index );
        FreeBucket( h );
        return res;
      }
    }
  }
  else
  if (value[w.First()] == NO_VALUE && value[w.Last()] == NO_VALUE )
  {
    int firstValToAssign = w.First();
    int lastValToAssign  = w.Last();
    BOOL found = FALSE;
    for( int i = minVal; i < maxVal && !stopNow; ++i )
    {
      value[firstValToAssign] = i;
      int minVal2 = FirstFreeBucket() - i - w.Len();
      int maxVal2 = LastFreeBucket() - i - w.Len() + 1;
      for( int j = minVal2; j < maxVal2 && !stopNow; ++j )
      {
        value[lastValToAssign] = j;
        BOOL res = Search( index );
        found = found || res;
      }
    }
    value[firstValToAssign] = NO_VALUE;
    value[lastValToAssign] = NO_VALUE;
    return found;
  }
  else
  {
    int valToAssign;
    int valAssigned;

    if (value[ w.First() ] == NO_VALUE)
    {
      valToAssign = w.First();
      valAssigned = w.Last();
    }
    else
    {
      valToAssign = w.Last();
      valAssigned = w.First();
    }

    int minVal2 = FirstFreeBucket() - value[valAssigned] - w.Len();
    int maxVal2 = LastFreeBucket() - value[valAssigned] - w.Len() + 1;

    ASSERT( value[ valToAssign ] == NO_VALUE );
    BOOL found = FALSE;

    for( int i = minVal2; i < maxVal2 && !stopNow; ++i )
    {
      value[valToAssign] = i;
      BOOL res = Search( index );
      found = found || res;
    }

    value[ valToAssign ] = NO_VALUE;
    return found;
  }
};

Word words[] =
{
  { "auto" },
  { "break" },
  { "case" },
  { "char" },
  { "const" },
  { "continue" },
  { "default" },
  { "do" },
  { "double" },
  { "else" },
  { "enum" },
  { "extern" },
  { "float" },
  { "for" },
  { "goto" },
  { "if" },
  { "int" },
  { "long" },
  { "register" },
  { "return" },
  { "short" },
  { "signed" },
  { "sizeof" },
  { "static" },
  { "struct" },
  { "switch" },
  { "typedef" },
  { "union" },
  { "unsigned" },
  { "void" },
  { "volatile" },
  { "while" },
  { "_cseg" },
  { "_dseg" },
  { "_bseg" },
  { "_iseg" },
  { "_xseg" },
};

void main ( int argc, char ** argv )
{
  int i;

  for( i = 0; i < _countof( value ); ++i )
    value[i] = NO_VALUE;
  for( i = 0; i < _countof( buckets ); ++i )
    buckets[i] = FALSE;

  NumWords = _countof( words );
  maxTableSize = NumWords;

  InitWords();
  if (!CheckWords())
    return;

  minVal = -MaxLen;
  maxVal = maxTableSize - MinLen;

  printf( "minVal = %d maxVal = %d\n", minVal, maxVal );

  stopFirst = TRUE;
  stopMinimal = FALSE;
  stopNow = FALSE;

  fileName = argc == 2 ? argv[1] : NULL;

  ReorderFreq();
  Reorder();
  Search( 0 );
};

