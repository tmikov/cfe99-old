#include "precomp.h"
#include "cfe_priv.h"
#include "idtab.h"
#include "ulib\avl-0.4\avl.h"

struct TIdentTab
{
  avl_tree tree;
};

struct TIdentNode
{
  avl      Avl;
  TIdent * id;
} TIdentAVL;

//--------------------------------------------------------------------------
// Name         TIdentNode_GcMark 
//
// 
//--------------------------------------------------------------------------
static void TIdentNode_GcMark ( const TIdentNode * node )
{
  GC_MARK( node->Avl.left );
  GC_MARK( node->Avl.right );
  GC_MARK( node->id );
};

//--------------------------------------------------------------------------
// Name         TIdentTab_GcMark 
//
// 
//--------------------------------------------------------------------------
static void TIdentTab_GcMark ( const TIdentTab * tab )
{
  GC_MARK( tab->tree.root );
};


//--------------------------------------------------------------------------
// Name         CompareIdents
//
// 
//--------------------------------------------------------------------------
static int CompareIdents( void* a, void* b )
{
  TIdentNode * ida = (TIdentNode *)a;
  TIdentNode * idb = (TIdentNode *)b;

  if (ida->id->strName == idb->id->strName)
    return 0;
  else
  if ((uintptr_t)ida->id->strName < (uintptr_t)idb->id->strName)
    return -1;
  else
    return 1;
};


//--------------------------------------------------------------------------
// Name         New_IdentTab 
//
// 
//--------------------------------------------------------------------------
TIdentTab * New_IdentTab ( void )
{
  TIdentTab * res = GC_NEW( TIdentTab );
  res->tree.compar = CompareIdents;
  return res;
};

//--------------------------------------------------------------------------
// Name         IdentTab_Done 
//
// 
//--------------------------------------------------------------------------
void IdentTab_Done ( TIdentTab * tab )
{
};

//--------------------------------------------------------------------------
// Name         IdentTab_Insert 
//
// 
//--------------------------------------------------------------------------
void IdentTab_Insert ( TIdentTab * tab, TIdent * id, TIdentNode ** ppNode )
{
  TIdentNode * pNode = GC_NEW( TIdentNode );
  pNode->id = id;

  ASSERT( tab != NULL );

  if (ppNode != NULL)
    *ppNode = pNode;

  avl_insert( &tab->tree, &pNode->Avl );
};

//--------------------------------------------------------------------------
// Name         IdentTab_Find 
//
// 
//--------------------------------------------------------------------------
TIdent * IdentTab_Find ( TIdentTab * tab, STRING strName, TIdentNode ** ppNode )
{
  TIdentNode * node;

  if (tab != NULL && strName != NULL)
  {
    node = (TIdentNode *)tab->tree.root;
    while (node != NULL)
    {
      if (strName == node->id->strName)
        break;

      if ((uintptr_t)strName < (uintptr_t)node->id->strName)
        node = (TIdentNode *)node->Avl.left;
      else
        node = (TIdentNode *)node->Avl.right;
    }
  }
  else
    node = NULL;

  if (ppNode != NULL)
    *ppNode = node;

  return node ? node->id : NULL;
};

//--------------------------------------------------------------------------
// Name         IdentTab_Remove 
//
// 
//--------------------------------------------------------------------------
void IdentTab_Remove ( TIdentTab * tab, TIdentNode * node )
{
  ASSERT( tab != NULL );
  ASSERT( node != NULL );

  avl_remove( &tab->tree, &node->Avl );

  GC_DELETE( node );
};

/*
//--------------------------------------------------------------------------
// Name         ForEach 
//
// 
//--------------------------------------------------------------------------
static bool ForEach ( TIdentNode * node, void * ctx, bool (*func)(void * ctx, TIdent * id) )
{
  ASSERT( node != NULL );

  if (node->Avl.left != NULL)
    if (!ForEach( (TIdentNode *)node->Avl.left, ctx, func ))
      return false;

  if (!func( ctx, node->id ))
    return false;

  if (node->Avl.right != NULL)
    if (!ForEach( (TIdentNode *)node->Avl.right, ctx, func ))
      return false;

  return true;
};

bool IdentTab_ForEach ( TIdentTab * tab, void * ctx, bool (*func)(void * ctx, TIdent * id) )
{
  if (tab != NULL && tab->tree.root != NULL)
    return ForEach( (TIdentNode *)tab->tree.root, ctx, func );
  else
    return true;
};
*/

//--------------------------------------------------------------------------
// Name         IdentTab_ForEach 
//
// We use preorder traversal because it is the easiest to implement
// without recursion and this function doesn't guarantee any order.
//
// Why don't we want to use recursion ?
// Because we will have to push the same parameters _ctx_ and _func_
// every time. Also I like the idea that the non-recursive algorithm
// is obvioulsy bound by the explicit stack.
//--------------------------------------------------------------------------
bool IdentTab_ForEach ( TIdentTab * tab, void * ctx, bool (*func)(void * ctx, TIdent * id) )
{
/*
  The depth _d_ of a balanced binary tree with n items is 
    d = ceil( log2( n + 1 ) );
  The depth _d_ of an AVL tree is:
    ceil( log2( n + 1 ) ) <= d < 2*ceil( log2( n + 1 ) )

  We use the maximum number of objects that can be accessed by a pointer
  as a conservative estimation for the max number of nodes in our tree.
  It _p_ is the numbert of bits in a pointer, the max number of nodes is
  2**p.

  So we have:
    d < 2*ceil( log2(2**p + 1) )

  Foo every p > 0 we have:
    2**p + 1 <= 1.5 * 2**p
  So, 
    log2(2**p + 1) <= log2(1.5 * 2**p)
    log2(2**p + 1) <= 0.6 + p

    ceil(0.6+p) = p+1

  => 
    d < 2p+2
*/
  TIdentNode * node;
  TIdentNode * stack[2 * sizeof(void*) * CHAR_BIT + 2]; 
  TIdentNode ** sp = stack;

  if (tab == NULL || tab->tree.root == NULL)
    return true;

  node = (TIdentNode *)tab->tree.root;

  // Push an ending 0
  *sp++ = NULL;

  do
  {
    do
    {
      if (!func( ctx, node->id ))
        return false;

      if (node->Avl.right)
        *sp++ = (TIdentNode *)node->Avl.right;
    }
    while (node = (TIdentNode *)node->Avl.left);
  }
  while (node = *--sp);

  return true;
};

