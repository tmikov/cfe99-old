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
#include "ulibpriv.h"
#include "strings.h"

#define TABLE_SIZE  4096
#define TABLE_MASK  (TABLE_SIZE-1)

unsigned Strings_Count = 0;

static TListEntry * pHashTable = NULL;
static bool bPinned = false;

//--------------------------------------------------------------------------
// Name         TString_Mark 
//
// 
//--------------------------------------------------------------------------
void TString_GcMark ( const TString * str )
{
  GC_MARK( str->data );
};

//--------------------------------------------------------------------------
// Name         Strings_Init
//
// 
//--------------------------------------------------------------------------
bool Strings_Init ( void )
{
  int i;

  ASSERT( pHashTable == NULL );
  if (pHashTable) 
    return true;

  Strings_Count = 0;

  // Allocate the bucket array
  //
  if ((pHashTable = GC_ARRAY_NEW( TListRoot, TABLE_SIZE )) == NULL)
    return false;

  // Initialize the buckets
  //
  for ( i = 0; i < TABLE_SIZE; ++i )
    INITIALIZE_LIST_HEAD( &pHashTable[i] );

  bPinned = false;

  return true;
};

//--------------------------------------------------------------------------
// Name         Strings_GcPin
//
// 
//--------------------------------------------------------------------------
void Strings_GcPin ( void )
{
  if (pHashTable != NULL && !bPinned)
  {
    GcPin( pHashTable );
    bPinned = true;
  }
};


//--------------------------------------------------------------------------
// Name         Strings_GcUnpin 
//
// 
//--------------------------------------------------------------------------
void Strings_GcUnpin ( void )
{
  if (pHashTable == NULL || !bPinned)
    return;

/*
   This is not necessary with our "smart" GC because it
   references the elements in a list only from the list root.
   So even though the strings physically point to each other, they don't
   leep each other alive.

  // Remove all inter-string references
  {
    int i;

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
  }
*/

  GcUnpin( pHashTable );
};

//--------------------------------------------------------------------------
// Name         Strings_GcMark 
//
// 
//--------------------------------------------------------------------------
void Strings_GcMark ( void )
{
  GC_MARK( pHashTable );
};


//--------------------------------------------------------------------------
// Name         Strings_Detach
//
// Release control of all data so the GC can collect it
//--------------------------------------------------------------------------
void Strings_Detach ( void )
{
  Strings_GcUnpin();
  pHashTable = NULL;
  Strings_Count = 0;
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

  Strings_GcUnpin();

  for ( i = 0; i < TABLE_SIZE; ++i )
  {
    TListEntry * listHead = &pHashTable[i];
    TString * cur;

    for ( cur = (TString *)listHead->Flink; !END_OF_LIST( listHead, cur ); )
    {
      TString * toFree = cur;
      cur = (TString *)cur->link.Flink;
      GC_DELETE( toFree );
    }
  }

  GC_DELETE( pHashTable );
  Strings_Count = 0;
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

  hashVal  = CalcStringLenHash( str, strLen );

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
      // Enforce a strict ordering on: hashVal,strLen,string_itself.
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
            return pSymbol;
        }
      }
    }
  }
  else
    pSymbol = NULL; // flag that we must not add the item to the hash table

  // At this point pSymbol points to the item *after* the position where
  // the new element must be inserted. Note that pSymbol might
  // actually point to the list root (end of list), but this doesn't
  // change the algorithm.

  pNew = GC_ALLOC( TString, offsetof( TString, szString ) + strLen + 1 );

  // Normally the allocator aborts if out of memory, but it is a good practice
  // to check this (since it is easy) and have it work in case the allocator
  // does return NULL.
  //
  if (pNew != NULL) 
  {
    // ok, no match. Add a new entry
    ++Strings_Count;

    pNew->nHash  = hashVal;
    pNew->strLen = strLen; 
    memcpy( pNew->szString, str, strLen );
    pNew->szString[strLen] = 0; // manually zero-terminate

  /*
    // Initialize the namesp data
    pNew->namesp.token = 0;
    INITIALIZE_LIST_HEAD( &pNew->namesp.tags );
    INITIALIZE_LIST_HEAD( &pNew->namesp.symbols );
    INITIALIZE_LIST_HEAD( &pNew->namesp.labels );
  */

    if (pSymbol != NULL ) // if hashing is still active
      INSERT_TAIL_LIST( &pSymbol->link, &pNew->link ); // insert before pSymbol
  }

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
#if defined(__TURBOC__) && defined(__MSDOS__) 

static uint16_t uint16_rotl ( uint32_t val )
{
  _asm mov ax, word ptr val
  _asm rol ax, 1
  return _AX;
};

#elif defined(_MSC_VER) && defined(_M_IX86)

static inline uint16_t uint16_rotl ( uint32_t val )
{
  _asm mov ax, word ptr val
  _asm rol ax, 1
};

#else
#  define uint16_rotl( x )  (((x & 0x8000u) >> 15) + ((x << 1) & 0xFFFFu))
#endif

//--------------------------------------------------------------------------
// Name         CalcStringLenHash
//
// Calculates a hash value from a string.
//--------------------------------------------------------------------------
unsigned CalcStringLenHash ( const char * str, size_t strLen )
{
  register uint_fast16_t value;
  register const uchar_t * s = (const uchar_t *)str;

  value = 0;
  for( ; strLen > 0; ++s, --strLen )
  {
    value ^= *s;
    value = uint16_rotl( value );
  }
  value += *(const uchar_t *)str;

  return value;
};


#ifdef _DEBUG

//--------------------------------------------------------------------------
// Name         String_Get
//
// This functions removes the "const" qualifier from the string. It is used
// only in _DEBUG builds because the compiler will warn us if the parameter
// passed is not of type STRING.
//--------------------------------------------------------------------------
TString * String_Get ( STRING str )
{
  ASSERT( str );
  return (TString *)str;
};

#endif
