#include <conio.h>
#include <stdio.h>
#include <mem.h>

typedef int BOOL;

#define TRUE  1
#define FALSE 0

#define MAX_PARTS  100

int lo, hi;
int numParts;
int Parts[MAX_PARTS];
BOOL Used[MAX_PARTS];
BOOL BestUsed[MAX_PARTS];
int maxSum;
int lastBestSum;
int attempts;

BOOL Test ( int sum )
{
  int i;
  ++attempts;
  for( i = 0; i < numParts; ++i )
    if (!Used[i])
    {
      int t = sum + Parts[i];

      if (t <= hi)
      {
        Used[i] = TRUE;
        if (t >= lo)
        {
          maxSum = t;
          return TRUE;
        }
        if (Test( t ))
          return TRUE;
        if (t > lastBestSum)
        {
          lastBestSum = t;
          memcpy( BestUsed, Used, numParts * sizeof( BOOL ) );
        }
        Used[i] = FALSE;
      }
    }
  return FALSE;
};

static int CmpInt ( const int * a, const int * b )
{
  return a < b ? -1 : (a > b ? 1 : 0);
};

void main ( void )
{
  clrscr();

  printf( "Enter lo:" );
  scanf( "%d", &lo );
  printf( "Enter hi:" );
  scanf( "%d", &hi );

  numParts = 0;
  for(;;)
  {
    Used[numParts] = FALSE;
    printf( "part[%d]=", numParts + 1 );
    scanf( "%d", &Parts[numParts] );
    if (Parts[numParts] == 0)
      break;
    ++numParts;
  }

  // Sort the array to enforce best cases come first
  qsort( Parts, numParts, sizeof( Parts[0] ), CmpInt );

  attempts = 0;
  lastBestSum = 0;
  if (Test( 0 ))
  {
    int i;
    printf( "MaxSum = %d Attempts = %d\n", maxSum, attempts );
    for( i = 0; i < numParts; ++i )
      if (Used[i])
        printf( "%5d", Parts[i] );
    putchar( '\n' );
  }
  else
  {
    int i;
    printf( "No decision. Last best = %d  Attempts = %d\n", lastBestSum, attempts );
    for( i = 0; i < numParts; ++i )
      if (BestUsed[i])
        printf( "%5d", Parts[i] );
    putchar( '\n' );
  }
};