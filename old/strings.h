#ifndef STRINGS_H
#define STRINGS_H

#include "misc\clist.h"

// This is the data that can optionally be associated with a string.
// It contains info relevant for all possible namespaces that an identifier
// can be in and that are active globally at one time.  
// Perhaps it would be better if this structure was separate from the string
// structure and later we may move it out of here.
//
typedef struct 
{
  int token;          // token code
  TListRoot tags;     // a list of struct/union/enum tags
  TListRoot symbols;  // a list of C symbols (variables and identifiers)
  TListRoot labels;   // a list of labels in a function
} TIdentNamesp;

typedef struct TString
{
  TListEntry    link;        // link in the hash bucket chain
  unsigned      nHash;       // hash value
  unsigned      strLen;      // string length

  TIdentNamesp  namesp;

  //
  //
  char          szString[1]; // the string itself
} TString;

typedef const TString * STRING;

void Strings_Init ();
void Strings_Done ();
void Strings_Free ();

STRING NewString ( const char * pStr );
STRING NewStringLen ( const char * pStr, size_t len );

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

#endif

