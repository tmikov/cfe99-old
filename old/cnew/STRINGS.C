/*
  strings.c: Keeps a global table of all strings. Each string is
  inserted only once. In this way in order to compare two strings for
  equality it is enough just to compare the pointers.

  In this implementation everything is inserted in an arena.
  Consequently strings can never be removed from the table. Everything
  is freed at once when the entire table is destroyed.

  If necessary the implementation can easily be changed in the following way:
  - pHashTable will be allocated in the C heap
  - TString structures will be allocated in a PagedHeap
  - the character strings will be allocated in the C heap.
  - TString will have a reference count member which is increased on
  NewString() and decreased on FreeString(). When the counter reaches 0,
  the character string is freed from the C heap, the TString structure
  is freed from the PagedHeap.
  - When the entire table is being destroyed, all strings in it
  are freed from the C heap and the PagedHeap.

  The interface of the module will remain the same and new functions 
  FreeString() and UseString() will have to be added.

18 Aug 1999:
  - The hash table itself is no longer in the arena. This is because
  at some point we may no longer need it, so we can free it.
*/
#include "port\global.h"
#include "misc\arena.h"
#include "misc\xheap.h"
#include "strings.h"

//#define MAX_HASH_TABLE_SIZE     (UINT_MAX / sizeof(TListEntry) / 2)

#define TABLE_SIZE  4096
#define TABLE_MASK  (TABLE_SIZE-1)

extern TArena GlobalArena;   // a global arena used for the strings as well
static TListEntry * pHashTable;

#define SA (&GlobalArena)

//--------------------------------------------------------------------------
// Name         Strings_Init
//
// 
//--------------------------------------------------------------------------
void Strings_Init ( void )
{
  int i;

  // Allocate the bucket array
  //
  pHashTable = (TListEntry *)xmalloc( TABLE_SIZE * sizeof(pHashTable[0]) );

  // Initialize the buckets
  //
  for ( i = 0; i < TABLE_SIZE; ++i )
    INITIALIZE_LIST_HEAD( &pHashTable[i] );
};

//--------------------------------------------------------------------------
// Name         Strings_FreeHash
//
// This function is called when the string table's hash capability
// will no longer be used, but the strings themselves will be.
//--------------------------------------------------------------------------
void Strings_FreeHash ( void )
{
  xfree( pHashTable );
  pHashTable = NULL;
};

//--------------------------------------------------------------------------
// Name         Strings_Done
//
// 
//--------------------------------------------------------------------------
void Strings_Done ( void )
{
  xfree( pHashTable );
};

//--------------------------------------------------------------------------
// Name         NewStringLen
//
// If the string is already in the table, returns a pointer to it.
// Otherwise allocates a new entry and returns it.
//
// When hashing is no longer active (Strings_FreeHash() has been called),
// the strings are still allocated, but the search is not performed.
//--------------------------------------------------------------------------
STRING NewStringLen ( const char * str, size_t strLen )
{
  unsigned   hashVal;
  TString    * pSymbol, * pNew;
  TListEntry * listHead;

  ++strLen; // include the zero
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
          !END_OF_LIST( listHead, pSymbol ) && pSymbol->nHash <= hashVal;
          pSymbol = (TString *)pSymbol->link.Flink )
    {
      if (pSymbol->nHash == hashVal &&
          memcmp( pSymbol->szString, str, strLen ) == 0)
      {
        return pSymbol;
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

  pNew = (TString *)Arena_CAlloc( SA, offsetof( TString, szString ) + strLen );

  pNew->nHash  = hashVal;
  pNew->strLen = strLen - 1; 
  memcpy( pNew->szString, str, strLen );

  // Initialize the cathegory lists
  //
  INITIALIZE_LIST_HEAD( &pNew->symbols );
  INITIALIZE_LIST_HEAD( &pNew->structs );

  if (pSymbol != NULL ) // if hashing is still active
    INSERT_TAIL_LIST( &pSymbol->link, &pNew->link );

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
