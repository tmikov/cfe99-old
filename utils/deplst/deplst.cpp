#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#pragma warning (disable:4786)

#include <set>
#include <string>

typedef std::set<std::string> CStringSet;

CStringSet AllFiles;
bool All = false;

class CScan
{
  CStringSet ToDoSet;

  //--------------------------------------------------------------------------
  // Name         scanfile 
  //
  // 
  //--------------------------------------------------------------------------
  void scanfile ( const std::string & strFilename, bool first )
  {
    char buf[512];
    const char * p;
    char file[512], * d;
    FILE * f;

    // If file is already process, ignore it
    if (FileSet.insert( strFilename ).second == false)
      return;

    // If file is in project list, stop recursion
    if (!All && !first && AllFiles.find( strFilename ) != AllFiles.end())
      return;

    f = fopen( strFilename.c_str(), "rt" );
    if (f == NULL)
      return;

    while (p = fgets( buf, sizeof(buf), f ))
    {
      while (isspace( *p )) ++p;
      if (*p != '#') continue;
      ++p;
      while (isspace( *p )) ++p;
      if (strncmp( p, "include", 7 ) != 0) continue;
      p += 7;
      while (isspace( *p )) ++p;
      if (*p != '"') continue;
      d = file;
      ++p;
      while (*p != 0 && *p != '"')
        *d++ = *p++;
      *d = 0;

      if (file[0] != 0)
      {
        scanfile( std::string( file ), false );
      }
    }

    fclose( f );
  };

public:
  CStringSet FileSet;

  //--------------------------------------------------------------------------
  // Name         CScan
  //
  // 
  //--------------------------------------------------------------------------
  CScan ( const std::string & filename )
  {
    scanfile( filename, true );

    FileSet.erase( filename );
  };
};



//--------------------------------------------------------------------------
// Name         main
//
// 
//--------------------------------------------------------------------------
int main ( int argc, char ** argv )
{
  --argc;
  ++argv;

  if (argc > 0)
  {
    if (strcmp( *argv, "-a" ) == 0 || strcmp( *argv, "/a" ) == 0)
    {
      All = true;
      --argc;
      ++argv;
    }
  }

  for ( ; argc-- > 0; ++argv )
  {
    FILE * f;
    if (NULL != (f = fopen( *argv, "rt")) )
    {
      fclose( f );
      AllFiles.insert( std::string( *argv ) );
    }
  }


  for ( CStringSet::const_iterator fname = AllFiles.begin(); fname != AllFiles.end(); ++fname )
  {
    CScan scan( *fname );

    printf( "%s:\n", fname->c_str() );
    for ( CStringSet::const_iterator i = scan.FileSet.begin(); i != scan.FileSet.end(); ++i )
      printf( "  %s\n", i->c_str() );
    printf( "\n" );
  }

  return EXIT_SUCCESS;
};
