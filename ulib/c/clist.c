#ifdef USE_GC

#include "ulibpriv.h"
#include "clist.h"

//--------------------------------------------------------------------------
// Name         TListRoot_GcMark 
//
// 
//--------------------------------------------------------------------------
void TListRoot_GcMark ( const TListRoot * head )
{
  const TListEntry * entry;
  for ( entry = head->Flink; !END_OF_LIST(head,entry); entry = entry->Flink )
    GC_MARK( entry );
};

IMPLEMENT_ARRAY_GC_MARK( TListRoot );

#endif // USE_GC
