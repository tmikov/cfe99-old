/*
  hashtab.c: A hash table for identifiers

Notes:
  - A hash table doesn't own its items. It is just a way to find an
  element fast.
*/
#include "port\global.h"
#include "misc\misc.h"
#include "hashtab.h"

////////////////////////////////////////////////////////////////////////
// Global functions

// Hash support ////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
// Name         HashTable_Init
//
// Prepare the hash table for use.
// The size of the table is rounded to the nearest smaller power of 2
// in order to speed access.
//--------------------------------------------------------------------------
THashTable * HashInit ( unsigned size )
{
  THashTable * pTab;
  unsigned i;

  if (size > MAX_HASH_TABLE_SIZE) // check for max size of the table
    size = MAX_HASH_TABLE_SIZE;

  // now find the nearest power of 2, smaller than size
  //
  for ( i = -1; size != 0; ++i, size >>= 1 );
  ASSERT( i >= 0 );
  size = 1 << i;

  // Consider that THashTable already has space for one item
  //
  pTab = (THashTable *)xmalloc( sizeof(THashTable) + (size - 1)*sizeof(TListEntry) );
  pTab->mask = size - 1;

  // Reset all hash chains
  //
  for ( i = 0; i < size; ++i )
    INITIALIZE_LIST_HEAD( pTab->HashTable + i );

  return pTab;
};

//--------------------------------------------------------------------------
// Name         HashTable_Done
//
// Just deallocates the hash table itself. Doesn't free the hash items
// (because hopefully they are all allocated in an arena and will be
// all freed simultanously)
//--------------------------------------------------------------------------
void HashDone ( THashTable * pTab )
{
  xfree( pTab );
};

//--------------------------------------------------------------------------
// Name         HashFind
//
// Description  Finds the identifier with this name and hash value
//
// Notes:
//  - There may be other ident-s with the same name and hash linked to
//    it. Must use FindNext to traverse them.
//  - We cannot be sure that the last inserted ident is of the inner-most
//    scope (I don't remember why but I must trust myself here...)
//--------------------------------------------------------------------------
THashEntry * HashFind ( THashTable * pTab, STRING strName )
{
  THashEntry * pSymbol;
  TListEntry * listHead;

  listHead = &pTab->HashTable[STRING_HASH( strName ) & pTab->mask];

  // find the one with the exact match
  //
  for ( pSymbol = (THashEntry *)listHead->Flink;
        !END_OF_LIST( listHead, pSymbol );
        pSymbol = (THashEntry *)pSymbol->link.Flink )
  {
    if (pSymbol->strName == strName)
      return pSymbol;
  }
  return NULL;
};

//--------------------------------------------------------------------------
// Name         FindNext
//
// Description  Traverses and returns the next ident with the same hash value
//              and name linked to pLast. It is not necessary linked directly
//              to pLast, so we have to walk the list. Returns NULL if no
//              more are found.
//--------------------------------------------------------------------------
THashEntry * HashFindNext ( THashTable * pTab, THashEntry * pLast )
{
  THashEntry * pSymbol;
  TListEntry * listHead;
  STRING strName;

  ASSERT( pLast );

  strName = pLast->strName; // cache the value in a register

  listHead = &pTab->HashTable[STRING_HASH(strName) & pTab->mask];

  // Start from the next item and walk until we find a matching item
  // or we reach the end of the list
  //
  for ( pSymbol = (THashEntry *)pLast->link.Flink;
        !END_OF_LIST( listHead, pSymbol );
        pSymbol = (THashEntry *)pSymbol->link.Flink )
  {
    if (pSymbol->strName == strName)
      return pSymbol;
  }

  return NULL;
};

//--------------------------------------------------------------------------
// Name         HashAdd
//
// Description  Adds a THashEntry to the hash table. 
//
// Note: symbols with szName == NULL, are not inserted into the table
//--------------------------------------------------------------------------
void HashAdd ( THashTable * pTab, THashEntry * pSymbol )
{
  if (pSymbol->strName == NULL)  // empty strings are not hashed
    return;

  ASSERT( !IS_HASHED( pSymbol ) ); 

  INSERT_TAIL_LIST( &pTab->HashTable[STRING_HASH(pSymbol->strName) & pTab->mask],
                    &pSymbol->link );
};

//--------------------------------------------------------------------------
// Name         HashRemove
//
// Description  Removes an ident from the hash table (but does not free
//              the structure)
// Note: symbols with szName == NULL, are not inserted into the table,
// so such symbols are not removed from it either.
//--------------------------------------------------------------------------
THashEntry * HashRemove ( THashTable * pTab, THashEntry * pSymbol )
{
  if (pSymbol->strName == NULL)  // empty strings are not hashed
    return pSymbol;

  ASSERT( IS_HASHED( pSymbol ) );

  REMOVE_ENTRY_LIST( &pSymbol->link );

  pSymbol->link.Flink = NULL; // mark as not hashed

  return pSymbol;
};


