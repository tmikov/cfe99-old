#include "port\global.h"
#include "c-tree.h"

//////////////////////// Global data //////////////////////////

// TODO: choose a better value for NumInPage
IMPLEMENT_FIXED_ALLOCATOR( ValueHeap, TTreeValue, 1024 );

#define DEFINE_EXPR_CODE( code, str, numArgs, flags )\
  { #code, str, numArgs, flags },

TExprCode ExprCode[] =
{
  #include "c-tree.def"
};
#undef DEFINE_EXPR_CODE


////////////////////// Global functions ///////////////////////

//--------------------------------------------------------------------------
// Name         CTree_Init
//
//
//--------------------------------------------------------------------------
void CTree_Init ( void )
{
  ValueHeap_Init();
};

//--------------------------------------------------------------------------
// Name         CTree_Done
//
//
//--------------------------------------------------------------------------
void CTree_Done ( void )
{
  ValueHeap_Done();
};

//--------------------------------------------------------------------------
// Name         AllocTree3
//
//
//--------------------------------------------------------------------------
TTreeValue * AllocTree3 (
    enum EXPR_CODE ecode,
    TYPE * type,
    TTreeValue * a1,
    TTreeValue * a2,
    TTreeValue * a3
  )
{
  TTreeValue * pTree;

  pTree = ValueHeap_CAlloc();  // allocate with zeroes

  pTree->ecode = ecode;
  pTree->type = type;

  // We don't need to check which arguments to copy or not
  // this would take more time than copying the arguments themselves
  //
  pTree->data.op.a1 = a1;
  pTree->data.op.a2 = a2;
  pTree->data.op.a3 = a3;

#ifdef _DEBUG
  // make sure that there aren't any non-null arguments supplied
  // where there should be NULL ones
  //
  switch (EXPR_PARAM_COUNT( ecode ))
  {
    case 0: ASSERT( a1 == NOARG );
    case 1: ASSERT( a2 == NOARG );
    case 2: ASSERT( a3 == NOARG );
    case 3: break;
    default: ASSERT( 0 );
  }
#endif

  return pTree;
};

//--------------------------------------------------------------------------
// Name         AllocEList
//
// Allocates and initializes an e-list node containg the data in item.
//   a1 points to the item
//   a2 is null (it will point to the next list node)
//   a3 points to the new allocated node itself (it is the list tail)
//--------------------------------------------------------------------------
TTreeValue * AllocEList (
    TTreeValue * item
  )
{
  TTreeValue * list;

  ASSERT( item );

  list = AllocTree3( e_list, NOTYPE, item, NOARG, NOARG );
  list->data.op.a2 = list;  // tie the tail to the node itself

  return list;
};

//--------------------------------------------------------------------------
// Name         EListAppend
//
// Appends a node to the end of the list. Returns the head of the list.
// The item must be also an e-list node.
//--------------------------------------------------------------------------
TTreeValue * EListAppend (
    TTreeValue * head,
    TTreeValue * item
  )
{
  ASSERT( head->ecode == e_list );
  ASSERT( item->ecode == e_list );

  // op.a3 : tail
  // op.a2 : next
  // op.a1 : value
  //
  head->data.op.a3 = head->data.op.a3->data.op.a2 = item;

  return head;
};

