#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static char szOutFile[_MAX_PATH] = { 0 };
static char szOldFile[_MAX_PATH];

static void Replace ( FILE * in, FILE * out, const char * search, const char * repl )
{
  char   buf[512];
  char * p;
  size_t slen = strlen( search );

  if (slen == 0)
    return; // should never happen

  while (p = fgets( buf, sizeof(buf), in ))
  {
    char * str;

    while (str = strstr( p, search ))
    {
      *str = 0;
      fputs( p, out );
      fputs( repl, out );
      p = str + slen;
    }

    fputs( p, out );
  } 
};


static void PrintHelp ( void )
{
  printf( "Usage: repl file search_str [replace_str]\n" );
};


int main ( int argc, char ** argv )
{
  FILE * in = NULL, * out = NULL;
  int exitCode = EXIT_FAILURE;

  printf( "Replacer Version 0.1\n" );

  if (argc < 3 || argc > 4 || strlen( argv[2] ) == 0)
  {
    PrintHelp();
    goto exitFunc;
  }

  if ( (in = fopen( argv[1], "rt" )) == NULL)
  {
    perror( argv[1] );
    goto exitFunc;
  }  

  sprintf( szOutFile, "%s.repl", argv[2] );

  if ( (out = fopen( szOutFile, "wt" )) == NULL)
  {
    perror( szOutFile );
    goto exitFunc;
  }

  Replace( in, out, argv[2], argc == 4 ? argv[3] : "" );

  {
    int err;

    err = fclose( in );
    in = NULL;
    if (err != 0)
    {
      perror( argv[1] );
      goto exitFunc;
    }

    err = fclose( out );
    out = NULL;

    if (err != 0)
    {
      perror( szOutFile );
      goto exitFunc;
    }

    sprintf( szOldFile, "%s.orig", argv[1] );

    remove( szOldFile );
    if (rename( argv[1], szOldFile ) != 0)
    {
      perror( szOldFile );
      goto exitFunc;
    }

    err = rename( szOutFile, argv[1] );
    szOutFile[0] = 0;

    if (err != 0)
      goto exitFunc;
  }

  exitCode = EXIT_SUCCESS;

exitFunc:
  if (in != NULL)
    fclose( in );
  if (out != NULL)
    fclose( out );
  if (szOutFile[0] != 0)
    remove( szOutFile );

  return exitCode;
};

