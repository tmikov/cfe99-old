#include "port\global.h"
#include "c-types.h"
#include "csyms.h"
#include "scope.h"
#include "misc\arena.h"

/////////////////////// static data /////////////////////////////

static bool IsBtIntegral[_BT_LAST] =
{
  false, // none
  false, // void
  true,  // bool
  true,  // char
  true,  // int
  false, // float
  false, // double
};

static bool IsBtFloat[_BT_LAST] =
{
  false, // none
  false, // void
  false, // bool
  false, // char
  false, // int
  true,  // float
  true,  // double
};

/////////////////////// public data ///////////////////////////////////

extern TArena GlobalArena;   // a global arena used for the strings as well

const char * NodeName[] =
{
  "",
  "basic",
  "enum",
  "struct",
  "union",
  "array",
  "pointer",
  "function",
};

const char * BtName[_BT_LAST] =
{
  "",
  "void",
  "bool",
  "char",
  "int",
  "float",
  "double",
};


/////////////////////// static functions //////////////////////////////

//--------------------------------------------------------------------------
// Name         SetTypeNodeFlags 
//
// Calculates and sets the isXXXX flags based on pNode->node and 
// pNode->basic.info
// It is called as soon as a node is allocated.
//--------------------------------------------------------------------------
static void SetTypeNodeFlags ( TTypeNode * pNode )
{
  enum TYPE_NODE node = pNode->node;  // get the node for convinience

  // set isClass
  //
  pNode->isClass = node == NODE_STRUCT || node == NODE_UNION;

  // set isIntegral
  //
  pNode->isIntegral = node == NODE_ENUM ||
                      node == NODE_BASIC && IsBtIntegral[pNode->info.basic];

  // set isFloat
  //
  pNode->isFloat = node == NODE_BASIC && IsBtFloat[pNode->info.basic];

  // set isArith
  //
  pNode->isArith = pNode->isIntegral | pNode->isFloat;

  // set isScalar
  //    
  pNode->isScalar = node == NODE_POINTER || pNode->isArith;

};

/////////////////////// public functions //////////////////////////////

//--------------------------------------------------------------------------
// Name         QualNode_New
//
// 
//--------------------------------------------------------------------------
TQualNode * QualNode_New ( void )
{
  TQualNode * res;
  ARENA_NEW0( &GlobalArena, res );
  return res;
};

//--------------------------------------------------------------------------
// Name         QualNode_Copy 
//
// 
//--------------------------------------------------------------------------
TQualNode * QualNode_Copy ( const TQualNode * q )
{
  TQualNode * res;

  ARENA_NEW( &GlobalArena, res );
  *res = *q;

  return res;
};

//--------------------------------------------------------------------------
// Name         QualNode_Free
//
// 
//--------------------------------------------------------------------------
void QualNode_Free ( TQualNode * p )
{
};

//--------------------------------------------------------------------------
// Name         TypeNode_New
//
// Allocates a non-NODE_BASIC node, calls SetTypeNodeFlags() to set the 
// isXXXX flags.
//--------------------------------------------------------------------------
TTypeNode * TypeNode_New ( TYPE_NODE node )
{
  TTypeNode * pNode;

  ARENA_NEW0( &GlobalArena, pNode );

  ASSERT( node != NODE_BASIC );

  pNode->node = node;

  // Since this is not a NODE_BASIC, we have enough info to set the flags
  SetTypeNodeFlags( pNode );

  return pNode;
};

//--------------------------------------------------------------------------
// Name         TypeNode_Copy
//
// 
//--------------------------------------------------------------------------
TTypeNode * TypeNode_Copy ( const TTypeNode * src )
{
  TTypeNode * pNode;

  ARENA_NEW( &GlobalArena, pNode );

  *pNode = *src;

  return pNode;
};

//--------------------------------------------------------------------------
// Name         TypeNode_NewBasic
//
// 
//--------------------------------------------------------------------------
TTypeNode * TypeNode_NewBasic ( BASIC_TYPE basic )
{
  TTypeNode * pNode;

  ARENA_NEW0( &GlobalArena, pNode );

  pNode->node = NODE_BASIC;

  // normalize implicit type to int
  pNode->info.basic = basic == BT_NONE ? BT_INT : basic;

  // set the flags
  SetTypeNodeFlags( pNode );

  return pNode;
};

//--------------------------------------------------------------------------
// Name         TypeNode_Free
//
// 
//--------------------------------------------------------------------------
void TypeNode_Free ( TTypeNode * p )
{
};

//--------------------------------------------------------------------------
// Name         BasicTypeWidth
//
// Returns the width in characters of the basic type specified in _type_.
// For invalid combinations (e.g. short char) returns 0.
//
// Note:
//   - this function is not suposed to be called with invalid combinations,
// so chaking the result against 0 is not a way to validate a type.
//   - Perhaps it will have to be enhaned for the "long long" type.
//--------------------------------------------------------------------------
HOST_UINT BasicTypeWidth ( const TYPE * type )
{
  TTypeNode * tnode = type->tnode;
                                     // basic,islong,isshort
  static const HOST_UINT typeSize[_BT_LAST][2][2] =
  {
    //{ { ---, short}, { long, long short} }

    // BT_NONE
    { { 0, 0 }, { 0, 0 } },

    // BT_VOID
    { { 0, 0 }, { 0, 0 } },

    // BT_BOOL
    { { TARGET_CHAR_SIZE, 0 }, { 0, 0 } },

    // BT_CHAR
    { { TARGET_CHAR_SIZE, 0 }, { 0, 0 } },

    // BT_INT
    { { TARGET_INT_SIZE, TARGET_SHORT_SIZE }, { TARGET_LONG_SIZE, 0 } },

    // BT_FLOAT
    { { TARGET_FLOAT_SIZE, 0 }, { 0, 0 } },

    // BT_DOUBLE
    { { TARGET_DOUBLE_SIZE, 0 }, { TARGET_DOUBLE_SIZE, 0 } },
  };

  ASSERT( tnode->node == NODE_BASIC );
  ASSERT( !tnode->isBitfield );
  return
    typeSize[tnode->info.basic][tnode->isLong][tnode->isShort];
};

//--------------------------------------------------------------------------
// Name         IsCompleteType
//
// Returns true if the type passed is complete. That means:
//   - it is not void
//   - it is not a forward use for class/struct/union/enum
//   - it is not an array without size
//--------------------------------------------------------------------------
bool IsCompleteType ( const TYPE * qnode )
{
  const TTypeNode * tnode;

  ASSERT( qnode );
  tnode = qnode->tnode;
  ASSERT( tnode );

  if (tnode->node == NODE_BASIC)
  {
    // void is never complete
    return tnode->info.basic != BT_VOID;
  }
  else
  if (tnode->isClass || tnode->node == NODE_ENUM)  // check for incomplete class or enum
  {
    ASSERT( tnode->info.structDef );
    return tnode->info.structDef->isComplete;
  }
  else
  if (tnode->node == NODE_ARRAY) // check for array without size
  {
    return TGT_INT_NE_0( tnode->info.arrayLen );
  }
  else
    return true; // all other cases are complete
};

//--------------------------------------------------------------------------
// Name         CopyTypeChain
//
// Creates a copy of a type chain.
// Note: this is not a "deep" copy, that is the structDef-s or function
// paramaters are not copied, they are just attached to the new type
// chain as well.
//--------------------------------------------------------------------------
TYPE * CopyTypeChain ( TYPE * src )
{
  TQualNode * q;
  TQualNode * result;  // this is the beginning of the result chain
  TTypeNode * t;

  ASSERT( src != NULL );
  if (src == NULL)
    return NULL;

  // allocate the first qnode, which we shall later return
  //
  result = q = QualNode_New(); 

  for(;;)
  {
    *q = *src;                    // copy the qnode

    ASSERT( src->tnode != NULL ); // make sure there is a tnode following

    t = TypeNode_Copy( src->tnode );  // allocate new and copy
    q->tnode = t;                      // and attach it to the qnode

    if ((src = t->of) == NULL);   // move on to the next node in the chain
      break;                      //  (if there isn't, exit the loop)

    q = QualNode_New();     // allocate a new qnode
    
    t->of = q;                    // attact the new qnode to the last tnode
  }

  return result;
};

#if 0
//--------------------------------------------------------------------------
// Name         FreeTypeChain
//
// Frees a type chain.
// Note: as well as CopyTypeChain(), this function is not "deep". It doesn't
// free any additional attributes associated to the chain (like structdefs
// of function parameters).
//--------------------------------------------------------------------------
void FreeTypeChain ( TYPE * src )
{
  while (src)
  {
    TTypeNode * tnode;

    tnode = src->tnode;          // preserve the tnode
    QualNode_Free( src );        // free the qnode
    src = tnode->of;             // get the next qnode
    TypeNode_Free( tnode );      // now we can free the tnode 
  }
};
#endif 

//--------------------------------------------------------------------------
// Name         CompareTypeChain
//
// Compares two type chains. Returns TRUE if they are exactly the same.
// Structs are compared by comparing the _structDef_ pointers.
//
// Functions are compared deeply - by comaring the number and types of all
// arguments and the return type.
//--------------------------------------------------------------------------
bool EqualTypeChains ( const TYPE * a, const TYPE * b )
{
  while (a != NULL)
  {
    TTypeNode * ta, * tb;

    if (b == NULL)
      return false; // how is this possible?

    if (a == b)   // type chains converge, so they are equal
      return true;

    // Check the qualifiers
    //
    if (a->isConst    != b->isConst ||
        a->isVolatile != b->isVolatile ||
        a->isRestrict != b->isRestrict ||
        a->attr       != b->attr)
      return false; // qualifier difference

    // Nove to the type node
    //
    ta = a->tnode;
    tb = b->tnode;
    ASSERT( ta != NULL && tb != NULL );

    if (ta == tb)   // type chains converge, so they are equal
      return true;

    if (ta->node != tb->node) // different nodes
      return false;

    // Make comparisons specific to the node type
    //
    switch (ta->node)
    {
      case NODE_BASIC:
        // Basic types must be the same and must have the same
        // specifiers
        //
        if (ta->info.basic  != tb->info.basic ||
            ta->isSigned    != tb->isSigned ||
            ta->isShort     != tb->isShort ||
            ta->isLong      != tb->isLong)
          return false;
        break;

      case NODE_ENUM:
      case NODE_STRUCT:
      case NODE_UNION:
        // Both must point to the same definition
        //
        if (ta->info.structDef != tb->info.structDef)
          return false;
        break;

      case NODE_ARRAY:
        // Array sizes must be either the same, or at least one of them
        // must be 0.
        if (TGT_INT_NE( ta->info.arrayLen, tb->info.arrayLen ) &&
            TGT_INT_NE_0( ta->info.arrayLen ) &&
            TGT_INT_NE_0( tb->info.arrayLen ) )
          return false;
        break;

      case NODE_POINTER:
        break;

      case NODE_FUNCTION:
        // If at least one is implicit, it is ok
        //
        if (ta->isImplicit || tb->isImplicit)
          break;

        // We must compare each parameter exactly
        //

        // explicit functions have at least a _void_ argument
        ASSERT( ta->info.funcParams && tb->info.funcParams );

        {
          TScope * pa = ta->info.funcParams;
          TScope * pb = tb->info.funcParams;
          TSymbol * symA, * symB;

          symA = ID_GET_SYMBOL( pa->locSymbols.first );
          symB = ID_GET_SYMBOL( pb->locSymbols.first );

          while (symA != NULL)
          {
            if (symB == NULL)
              return false; // different number of arguments

            if (!EqualTypeChains( symA->type, symB->type ))
              return false; // different type of a parameter

            symA = ID_GET_SYMBOL( symA->id.nextLocal );
            symB = ID_GET_SYMBOL( symB->id.nextLocal );
          }
          if (symB != NULL)
            return false; // different number of arguments
        }
        break;

      default:
        ASSERT( 0 );
    }

    // Move to the next qualifier
    //
    a = ta->of;
    b = tb->of;
  }

  ASSERT( a == NULL );
  if (b != NULL)
    return false;  // a ended before b (I wonder how this could be possible
                   // if they had the same type nodes)

  return true;
};

//--------------------------------------------------------------------------
// Name         PointerTo
//
// 
//--------------------------------------------------------------------------
TYPE * PointerTo ( TYPE * to, const TQualNode * qual /* = NULL */ )
{
  TQualNode * ptrQual;
  TTypeNode * ptrNode;

  ptrNode = TypeNode_New( NODE_POINTER );
  ptrNode->of = to;

  ptrQual = qual != NULL ? QualNode_Copy( qual ) : QualNode_New();
  ptrQual->tnode = ptrNode;

  return ptrQual;
};

//--------------------------------------------------------------------------
// Name         ArrayToPointer 
//
// Converts an array type to its pointer type
//--------------------------------------------------------------------------
TYPE * ArrayToPointer ( TYPE * arrayType )
{
  ASSERT( arrayType->tnode->node == NODE_ARRAY );

  return PointerTo( arrayType->tnode->of, arrayType );
};

//--------------------------------------------------------------------------
// Name         FuncToPointer 
//
// Converts an func type to its pointer type
//--------------------------------------------------------------------------
TYPE * FuncToPointer ( TYPE * funcType )
{
  ASSERT( funcType->tnode->node == NODE_FUNCTION );

  return PointerTo( funcType, NULL );
};


#if YYDEBUG
#include "symtab.h"

//--------------------------------------------------------------------------
// Name         PrintType
//
//
//--------------------------------------------------------------------------
void PrintType ( TYPE * type )
{
  TTypeNode * tnode;
  ASSERT( type );
  ASSERT( type->tnode );

  // Print the qualifier
  //
  if (type->isConst)
    printf( "const " );
  if (type->isVolatile)
    printf( "volatile " );
  if (type->isRestrict)
    printf( "__restrict " );
  if (type->attr)
    printf( "__attr(0x%x) ", type->attr );

  // Print the tnode
  //
  tnode = type->tnode;

  // print common flags
  //
  putchar( '<' );
  if (tnode->isIntegral)
    putchar( 'I' );
  if (tnode->isFloat)
    putchar( 'F' );
  if (tnode->isArith)
    putchar( 'A' );
  if (tnode->isScalar)
    putchar( 'S' );
  putchar( '>' );
  putchar( ' ' );

  switch (tnode->node)
  {
    case NODE_BASIC:
      if (tnode->isSigned)
         printf( "signed " );
      else
         printf( "unsigned " );
      if (tnode->isShort)
         printf( "short " );
      if (tnode->isLong)
         printf( "long " );
      printf( "%s", BtName[tnode->info.basic] );
      break;

    case NODE_POINTER:
      printf( "ptr to " );
      PrintType( tnode->of );
      break;

    case NODE_STRUCT:
    case NODE_UNION:
    case NODE_ENUM:
      printf( "%s %s", NodeName[tnode->node ], Struct_Name( tnode->info.structDef ) );
      break;

    case NODE_FUNCTION:
      printf( "Function ( " );

      // print the parameters
      ASSERT( tnode->info.funcParams );
      if (tnode->info.funcParams)
      {
        TSymbol * param;

        // Check and print the explicit (void) parameter
        if (tnode->info.funcParams->locSymbols.first == NULL &&
            !tnode->isImplicit)
          printf( "void" );
            

        // iterate through all parameters declared in the parameter scope
        //
        for ( param = ID_GET_SYMBOL( tnode->info.funcParams->locSymbols.first );
              param != NULL;
              param = ID_GET_SYMBOL( param->id.nextLocal ) )
        {
          // print the name (which is optional)
          if (param->id.strName != NULL)
            printf( "%s", C_STR(param->id.strName) );
          else
          if (param == EllipsisSym)
            printf( "..." );

          printf( ":" );

          if (param->type)
            PrintType( param->type );

          // if this is not the last symbol, print a comma
          if (param != ID_GET_SYMBOL(tnode->info.funcParams->locSymbols.last))
            printf( ", " );
        }
      }

      printf( " ) returning " );
      PrintType( tnode->of );
      break;

    case NODE_ARRAY:
      printf( "array[" );
      if (TGT_INT_NE_0( tnode->info.arrayLen ))
        printf( "%lu", (long unsigned)TGT_UINT_2_HOST_INT(tnode->info.arrayLen) );
      printf( "] of " );
      PrintType( tnode->of );
      break;

    default:
      ASSERT( 0 );
  }
};
#endif

