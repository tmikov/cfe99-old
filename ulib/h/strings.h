#ifndef STRINGS_H
#define STRINGS_H

#ifndef CLIST_H
  #include "clist.h"
#endif
#ifndef SIMGC_H
  #include "simgc.h"
#endif

#ifndef TSTRING_DATA_DEFINED
typedef struct TStringData TStringData;
#endif

typedef struct TString_tag
{
  TListEntry    link;        // link in the hash bucket chain
  unsigned      nHash;       // hash value
  unsigned      strLen;      // string length

  TStringData   * data;

  //
  //
  char          szString[1]; // the string itself
} TString;

typedef const TString * STRING;

void TString_GcMark ( const TString * str );
#define STRING_GcMark TString_Mark

extern unsigned Strings_Count;

bool Strings_Init ( void );
void Strings_GcPin ( void );
void Strings_GcUnpin ( void );
void Strings_GcMark ( void );
void Strings_Detach ( void );
void Strings_Free ( void );

STRING NewString ( const char * pStr );
STRING NewStringLen ( const char * pStr, size_t len );

#define STRING_LEN( s )    ((s)->strLen)
#define STRING_HASH( s )   ((s)->nHash)
#define C_STR( s )         ((s)->szString)
#define STRING_DATA( s )   ((s)->data)

#ifdef _DEBUG
// In the debug builds we call a function because we want to make
// sure that _s_ is really of type STRING.
//
  TString * String_Get ( STRING str );

  #define STRING_GET( s )   String_Get( s )
#else
  #define STRING_GET( s )   ((TString *)(s))
#endif

#define STRING_SET_DATA( s, d ) (STRING_GET( s )->data = (d))

unsigned CalcStringLenHash (
    const char * str,
    size_t strLen
  );

#endif

