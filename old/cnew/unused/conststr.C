#include "port\global.h"
#include "misc\xheap.h"
#include "misc\clist.h"
#include "conststr.h"


static TListRoot StrList = { NULL };

//--------------------------------------------------------------------------
// Name         ConstStr_Init
//
//
//--------------------------------------------------------------------------
void ConstStr_Init ( void )
{
  INITIALIZE_LIST_HEAD( &StrList );
};

//--------------------------------------------------------------------------
// Name         ConstStr_Done
//
// Frees all allocated strings.
//--------------------------------------------------------------------------
void ConstStr_Done ( void )
{
  if (StrList.Flink != NULL) // if it was initialized
  {
    // Remove all allocated items
    //
    while (!IS_LIST_EMPTY( &StrList ))
    {
      void * p = REMOVE_TAIL_LIST( &StrList );
      xfree( p );
    }
  }
};

//--------------------------------------------------------------------------
// Name         AllocConstStrLen
//
// Note: If str == NULL, returns NULL
//--------------------------------------------------------------------------
const char * AllocConstStrLen ( const char * str, unsigned strLen )
{
  if (str == NULL)
  {
    ASSERT( strLen == 0 );
    return str;
  }
  else
  {
    CONST_STR_HEAD * p = (CONST_STR_HEAD *)xmalloc( sizeof(CONST_STR_HEAD) + strLen + 1 );

    INSERT_TAIL_LIST( &StrList, &p->link );
    p->refCnt = 1;
    p->size   = strLen + 1;
    return memcpy( p+1, str, strLen + 1 ); // copy the string itself
  }
};

//--------------------------------------------------------------------------
// Name         AllocConstString
//
// Note: If str == NULL, returns NULL
//--------------------------------------------------------------------------
const char * AllocConstString ( const char * str )
{
  return str == NULL ? str : AllocConstStrLen( str, strlen( str ) );
};

//--------------------------------------------------------------------------
// Name         FreeConstString
//
//
//--------------------------------------------------------------------------
void FreeConstString ( const char * str )
{
  if (str != NULL)
  {
    CONST_STR_HEAD * p = (CONST_STR_HEAD *)str - 1;

    if (--p->refCnt == 0)
    {
      REMOVE_ENTRY_LIST( &p->link );
      xfree( p );
    }
  }
};

//--------------------------------------------------------------------------
// Name         CopyConstString
//
//
//--------------------------------------------------------------------------
const char * CopyConstString ( const char * str )
{
  if (str != NULL)  // allow passing of NULL pointers.
    ++((CONST_STR_HEAD *)str - 1)->refCnt;
  return str;
};



