#include "precomp.h"
#include "tconf.h"
#include "util.h"

TARGET_LARGEST_INT_TYPE Const1;

//--------------------------------------------------------------------------
// Name         ClearHiBits 
//
// Clear a number of hi bits in a value by shifting.
//
// OPTNOTE: could be done directly
//--------------------------------------------------------------------------
static void ClearHiBits ( UCHAR * value, unsigned valueSize, unsigned bitCount )
{
  unsigned_integer_shift_left( valueSize, value, bitCount, value );
  unsigned_integer_shift_right( valueSize, value, bitCount, value );
  overflow_bit = false;
};

//--------------------------------------------------------------------------
// Name         GenLoBits
//
// Return a value consisting of its lo bitCount bits set
//--------------------------------------------------------------------------
static void GenLoBits ( UCHAR * value, unsigned valueSize, unsigned bitCount )
{
  unsigned actualBitLen;

  unsigned_integer_maximum( valueSize, value );

  actualBitLen = valueSize * CHAR_BIT;

  ASSERT( bitCount <= actualBitLen );
  if (bitCount < actualBitLen)
    ClearHiBits( value, valueSize, actualBitLen - bitCount );

  overflow_bit = false;
};

//--------------------------------------------------------------------------
// Name         TargetAssignHostUInt 
//
// Assign a host unsigned integer to a target unsigned integer.
// Sets overflow_bit if it can't fit.
//--------------------------------------------------------------------------
void TargetAssignHostUInt ( UCHAR * dest, unsigned destSize, unsigned uvalue )
{
  ASSERT( destSize );
  do
  {
    dest[--destSize] = (UCHAR)uvalue;
    uvalue >>= CHAR_BIT;
  }
  while (destSize > 0);

  overflow_bit = uvalue != 0;
};

//--------------------------------------------------------------------------
// Name         UInt16FromTarget 
//
// 
//--------------------------------------------------------------------------
unsigned UInt16FromTarget ( const UCHAR * src, unsigned srcSize )
{
  unsigned res;
  UCHAR FFFF[TARGET_LARGEST_INT_SIZE_ON_HOST];

  ASSERT( srcSize > 0 && srcSize <= TARGET_LARGEST_INT_SIZE_ON_HOST);

  // Initialize FFFF with 0xFFFFu
  TargetAssignHostUInt( FFFF, srcSize, 0xFFFFu );

  // if src > FFFF, overflow
  overflow_bit = gt_unsigned_integer( srcSize, src, FFFF );

#if 1
  // Since we return a 16-bit value and a TARGET_CHAR_BIT is at least 8,
  // no need to process more than two characters
  //
  res = src[srcSize-1];
  if (srcSize > 1)
    res |= src[srcSize-2] << CHAR_BIT;
#else
  res = 0;
  do
    res = (res << CHAR_BIT) | *src++;
  while (--srcSize > 0);
#endif

  return res;
};


// The types are sorted according to their integer conversion rank
// in pairs signed-unsigned
const TIntegerTypeDesc IntegerTypesDesc[] = 
{
  // rank, precision

  // _Bool
  { 1, 1, 0, false }, { 0 },

  // char
  { 10, TARGET_CHAR_BIT, 0, false }, { 0 },

  // short
  { 20, TARGET_SHORT_BIT, 0, false }, { 0 },

  // int 
  { 40, TARGET_INT_BIT, 0, false }, { 0 },

  // extended integer type
  { 50, TARGET_EX_INT50_BIT, 0, true }, { 0 },

  // long
  { 80, TARGET_LONG_BIT, 1, false }, { 0 },

  // long long
  { 100, TARGET_LONGLONG_BIT, 2, false }, { 0 },

};

//--------------------------------------------------------------------------
// Name         InitTypes
//
// Complete the type descriptions by filling the fields that can easily
// be calculated at runtime. 
//--------------------------------------------------------------------------
static void InitTypes ( void )
{
  unsigned i;
  // remove the const
  TIntegerTypeDesc * itypes = (TIntegerTypeDesc *)IntegerTypesDesc;

  // Copy the unsigned description to their signed brother,
  // calculate the max values, initialize the brother pointers.
  //
  MAKE_SURE( _countof( itypes ) % 2 == 0 );
  for ( i = 0; i < _countof( itypes ); i += 2 )
  {
    // Generate the mask
    //
    GenLoBits( itypes[i].mask.b, 
               sizeof( itypes[i].mask.b ),
               itypes[i].precision );

    // Calculate the size in characters
    //
    itypes[i].size = 
      (itypes[i].precision + TARGET_CHAR_BIT - 1)/TARGET_CHAR_BIT;

    // Copy to the description so far
    //
    itypes[i+1] = itypes[i];

    // Set the signedness
    //
    itypes[i].isSigned = true;


    // The min value of the unsigned is 0. It already is 0
    // The max value of the unsigned is the mask
    itypes[i+1].maxValue = itypes[i+1].mask;  

    // The max value of the signed is all bit the highest bits set
    TARGET_SHR_UNSIGNED( &itypes[i].maxValue, &itypes[i].mask, 1 );
    // The min signed value is (unsigned)MaxValue + 1
    TARGET_ADD_UNSIGNED( &itypes[i].minValue, *itypes[i].maxValue, &Const1 );

    // Initialize the brother pointers
    //
    itypes[i].brother   = &itypes[i+1]; 
    itypes[i+1].brother = &itypes[i];
  }


#ifdef _DEBUG
  //
  // Make sure that ranks are set accordingly
  //
  for ( i = 0; i < _countof( itypes ); ++i )
  {
    unsigned j;
    for ( j = i+1; j < _countof( itypes ); ++j )
    {
      if (itypes[j].rank > itypes[i].rank)
      {
        ASSERT( itypes[j].precision >= itypes[i] );
      }
      else
      {
        ASSERT( (j >> 1) == (i >> 1) ); // they are brothers
        ASSERT( itypes[j].brother == &itypes[i] );
        ASSERT( itypes[i].brother == &itypes[j] );
      }
      else
        ASSERT( 0 ); // must be sorted
    }
  }

#endif
};

//--------------------------------------------------------------------------
// Name         TConf_Init 
//
// 
//--------------------------------------------------------------------------
void TConf_Init ()
{
  IEEE_reset();

  // Initialize constants
  //
  TARGET_ASSIGN_8BIT( &Const1, 1 );

  // Initialize the type descriptions
  //
  InitTypes();
};

