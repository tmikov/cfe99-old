#ifndef HASHTAB_H
#define HASHTAB_H

#include "misc\clist.h"
#include "strings.h"

// This is the entry that goes into the hash table. Multiple
// entries with same hash module will be linked together in a doubly-linked
// list.
// 
typedef struct THashEntry
{
  TListEntry    link;
  STRING        strName;
} THashEntry;

#define IS_HASHED( entry )  ((entry)->link.Flink != NULL)

// This is the structiure describing the hash table itself
//
typedef struct THashTable
{
  unsigned     mask;          // = hash_table_size - 1 = 2**n - 1
  TListEntry   HashTable[1];
} THashTable;

// Max size of hash table that we can address and index
//
#define MAX_HASH_TABLE_SIZE     (UINT_MAX / sizeof(TListEntry) / 2)

THashTable * HashInit (
    unsigned size
  );

void HashDone (
    THashTable * pTab
  );

THashEntry * HashFind (
    THashTable * pTab,
    STRING strName
  );

THashEntry * HashFindNext (
    THashTable * pTab,
    THashEntry * pLast
  );

void HashAdd (
    THashTable * pTab,
    THashEntry * pSymbol
  );

THashEntry * HashRemove (
    THashTable * pTab,
    THashEntry * pSymbol
  );

#endif // HASHTAB_H
