/*
  strings.c: Keeps a global table of all strings. Each string is
  inserted only once. In this way in order to compare two strings for
  equality it is enough just to compare the pointers.

  TODO: 
    - grow the hash table when it fills up
    - no need to use a doubly-linked list since we never remove
    entries from the table
    - perhaps use a general purpose hash table module.
    - research the possiblity of using a plain hash table (just an 
    array).
*/
#include "precomp.h"
#include "heap.h"
#include "util.h"
#include "strings.h"

//#define MAX_HASH_TABLE_SIZE     (UINT_MAX / sizeof(TListEntry) / 2)

#define TABLE_SIZE  4096
#define TABLE_MASK  (TABLE_SIZE-1)

static TListEntry * pHashTable = NULL;

//--------------------------------------------------------------------------
// Name         Strings_Init
//
// 
//--------------------------------------------------------------------------
void Strings_Init ( void )
{
  int i;

  ASSERT( pHashTable == NULL );
  if (pHashTable) return;

  // Allocate the bucket array
  //
  pHashTable = GC_ARRAY_NEW( TListEntry, TABLE_SIZE );

  // Initialize the buckets
  //
  for ( i = 0; i < TABLE_SIZE; ++i )
    INITIALIZE_LIST_HEAD( &pHashTable[i] );
};

//--------------------------------------------------------------------------
// Name         Strings_Done
//
// Release control of all data so the GC can collect it
//--------------------------------------------------------------------------
void Strings_Done ( void )
{
  int i;

  if (pHashTable == NULL)
    return;

  for ( i = 0; i < TABLE_SIZE; ++i )
  {
    TListEntry * listHead = &pHashTable[i];
    TString * cur;

    for ( cur = (TString *)listHead->Flink; !END_OF_LIST( listHead, cur ); )
    {
      TString * toFree = cur;
      cur = (TString *)cur->link.Flink;

      // Remove all references
      CLEAR( &toFree->link );
    }

    CLEAR( listHead );
  }

  gcFree( pHashTable );
  pHashTable = NULL;
};

//--------------------------------------------------------------------------
// Name         Strings_Free
//
// Explicitly free all data structures
//--------------------------------------------------------------------------
void Strings_Free ( void )
{
  int i;

  if (pHashTable == NULL)
    return;

  for ( i = 0; i < TABLE_SIZE; ++i )
  {
    TListEntry * listHead = &pHashTable[i];
    TString * cur;

    for ( cur = (TString *)listHead->Flink; !END_OF_LIST( listHead, cur ); )
    {
      TString * toFree = cur;
      cur = (TString *)cur->link.Flink;
      gcFree( toFree );
    }
  }

  gcFree( pHashTable );
  pHashTable = NULL;
};


//--------------------------------------------------------------------------
// Name         NewStringLen
//
// If the string is already in the table, returns a pointer to it.
// Otherwise allocates a new entry and returns it.
//
// When hashing is no longer active (Strings_Free/Done() has been called),
// the strings are still allocated, but the search is not performed.
//
// The string doesn't have to be zero terminated
//--------------------------------------------------------------------------
STRING NewStringLen ( const char * str, size_t strLen )
{
  unsigned   hashVal;
  TString    * pSymbol, * pNew;
  TListEntry * listHead;

  ASSERT( pHashTable );

  hashVal  = CalcStringHash( str );

  if (pHashTable != NULL) // if hash is still active
  {
    listHead = &pHashTable[hashVal & TABLE_MASK];

    // We keep the actual hash values sorted. So when we reach a
    // greater one or the end of the list we can abort the search and
    // insert the new element there without violating the order.

    // find the one with the exact match
    //
    for ( pSymbol = (TString *)listHead->Flink;
          !END_OF_LIST( listHead, pSymbol )/* && pSymbol->nHash <= hashVal*/;
          pSymbol = (TString *)pSymbol->link.Flink )
    {
/*
      // Note that we don't compare the zeros because first we check
      // if the length matches
      if (pSymbol->nHash == hashVal && pSymbol->strLen == strLen &&
          memcmp( pSymbol->szString, str, strLen ) == 0)
      {
        return pSymbol;
      }

*/
      // Enforce a string ordering on: hashVal,strLen,string_itself.
      // On exit of the loop we must insert *before* pSymbol
      //
      if (pSymbol->nHash > hashVal) 
        break;
      if (pSymbol->nHash == hashVal)
      {
        if (pSymbol->strLen > strLen)
          break;
        if (pSymbol->strLen == strLen)
        {
          int cmpRes;
          if ((cmpRes = memcmp( pSymbol->szString, str, strLen )) > 0)
            break;
          if (cmpRes == 0)
            return;
        }
      }
    }
  }
  else
    pSymbol = NULL; // flag that we must not add the item to the hash table

  // ok, no match. Add a new entry

  // At this point pSymbol points to the item *after* the position where
  // the new element must be inserted. Note that pSymbol might
  // actually point to the list root (end of list), but this doesn't
  // change the algorithm.

  pNew = (TString *)gcAlloc( offsetof( TString, szString ) + strLen + 1 );

  pNew->nHash  = hashVal;
  pNew->strLen = strLen; 
  memcpy( pNew->szString, str, strLen );
  pNew->szString[strLen] = 0; // manually zero-terminate

  // Initialize the namesp data
  INITIALIZE_LIST_HEAD( &pNew->namesp.tags );
  INITIALIZE_LIST_HEAD( &pNew->namesp.symbols );

  if (pSymbol != NULL ) // if hashing is still active
    INSERT_TAIL_LIST( &pSymbol->link, &pNew->link ); // insert before pSymbol

  return pNew;
};

//--------------------------------------------------------------------------
// Name         NewString
//
// 
//--------------------------------------------------------------------------
STRING NewString ( const char * str )
{
  return NewStringLen( str, strlen( str ) );
};

/*
  I found that the hash function behaves better if it is always calculated
  on 16-bit values. However 32-bit compilers do not have _rotl for words,
  so this macro simulates a word _rotl.
*/
#if UINT_MAX > 0xFFFFu
#  if defined(_M_IX86)
      static _inline WORD WORD_ROTL ( DWORD val )
      {
        _asm mov ax, word ptr val
        _asm rol ax, 1
      };
#  else
#    define WORD_ROTL( x )  (((x & 0x8000u) >> 15) + ((x << 1) & 0xFFFFu))
#  endif
#else
#  define WORD_ROTL( x )  _rotl( x, 1 )
#endif

//--------------------------------------------------------------------------
// Name         CalcStringHash
//
// Calculates a hash value from a string.
//--------------------------------------------------------------------------
unsigned CalcStringHash ( const char * str )
{
  register unsigned value;
  register const UCHAR * s = (const UCHAR *)str;

  for( value = 0; *s; ++s )
  {
    value ^= *s;
    value = WORD_ROTL( value );
  }
  value += *(const UCHAR *)str;

  return value;
};


#ifdef _DEBUG

//--------------------------------------------------------------------------
// Name         StringData 
//
// This functions removes the "const" qualifier from the string. It is used
// only in _DEBUG builds because the compiler will warn us if the parameter
// passed is not of type STRING.
//--------------------------------------------------------------------------
TString * StringData ( STRING str )
{
  ASSERT( str );
  return (TString *)str;
};

#endif
