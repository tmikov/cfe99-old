#ifndef STRINGS_H
#define STRINGS_H

#include "misc\clist.h"

typedef struct TString TString;
struct TString
{
  TListEntry    link;        // link in the hash bucket chain
  unsigned      nHash;       // hash value
  unsigned      strLen;      // string length

  // The lists for each cathegory
  //
  TListRoot     symbols;
  TListRoot     structs;

  //
  //
  char          szString[1]; // the string itself
};

typedef const TString * STRING;

void Strings_Init ( void );
void Strings_Done ( void );
void Strings_FreeHash ( void );

STRING NewString ( const char * str );
STRING NewStringLen ( const char * str, size_t strLen );

#define STRING_LEN( s )    ((s)->strLen)
#define STRING_HASH( s )   ((s)->nHash)
#define C_STR( s )         ((s)->szString)

#ifdef _DEBUG
// In the debug builds we call a function because we want to make
// sure that _s_ is really of type STRING.
//
  TString * StringData ( STRING str );

  #define STRING_DATA( s )   StringData( s )
#else
  #define STRING_DATA( s )   ((TString *)(s))
#endif

unsigned CalcStringHash (
    const char * str
  );

#endif // STRINGS_H


