/*
  machconf.c
    Implementation for machine types and operations.
    Compiled in the context of target\c.
*/
#include "precomp.h"
#include "mach_priv.h"


///////////////////////////////////////////////////////////////////////////////
//  TMachIntTypeDesc MachIntTypesDesc[MachIntType_t::mt_last]; 
//      Define the properties of all target integer types
//      The types must be sorted according to their integer conversion rank
//
TMachIntTypeDesc MachIntTypesDesc[mt_Last] = 
{      	
  // precision, size
  { mt_I8,   8, 1 },
  { mt_I16, 16, 2 },
  { mt_I32, 32, 4 },
  { mt_I48, 48, 6 },
  { mt_I64, 64, 8 },
};


//--------------------------------------------------------------------------
// Name         TargetGenBitMask 
//
// Generate _bitCount_ lo bits set to 1 and set the rest to 0.
//--------------------------------------------------------------------------
static void TargetGenBitMask ( uchar * value, unsigned valueSize, unsigned bitCount )
{
  ASSERT( bitCount <= valueSize * CHAR_BIT );

  value += valueSize - 1; // point to the end of the number

  while (bitCount >= CHAR_BIT)
  {
    *value-- = UCHAR_MAX;
    bitCount -= CHAR_BIT;
  }

  if (bitCount > 0)
    *value = (1 << bitCount) - 1;
};

//--------------------------------------------------------------------------
// Name         priv_TargetAssignUInt16
//
// Assign a host unsigned integer to a target unsigned integer.
// Sets overflow_bit if it can't fit.
//--------------------------------------------------------------------------
void priv_TargetAssignUInt16 ( uchar * dest, unsigned destSize, uint_fast16_t uvalue )
{
  ASSERT( destSize );
  do
  {
    dest[--destSize] = (uchar)uvalue;
    uvalue >>= CHAR_BIT;
  }
  while (destSize > 0);

  overflow_bit = uvalue != 0;
};

//--------------------------------------------------------------------------
// Name         priv_UInt16FromTarget 
//
// 
//--------------------------------------------------------------------------
uint_fast16_t priv_UInt16FromTarget ( const uchar * src, unsigned srcSize )
{
  uint_fast16_t res;
  TARGET_IMAX_T FFFF;

  ASSERT( srcSize > 0 );

  // Initialize FFFF with 0xFFFFu
  priv_TargetAssignUInt16( FFFF.b, srcSize, 0xFFFFu );

  // if src > FFFF, overflow
  overflow_bit = gt_unsigned_integer( srcSize, src, FFFF.b );

  res = 0;
  do
    res = (res << CHAR_BIT) | *src++;
  while (--srcSize > 0);

  return res;
};

//--------------------------------------------------------------------------
// Name         InitTypes
//
// Complete the type descriptions by filling the fields that can easily
// be calculated at runtime. 
//--------------------------------------------------------------------------
static void InitTypes ( void )
{
  MachIntType_t i;

  MAKE_SURE( NELEM( MachIntTypesDesc ) == mt_Last );

  for ( i = 0; i < NELEM( MachIntTypesDesc ); ++i )
  {
    ASSERT( MachIntTypesDesc[i].mtId == i );
    ASSERT( (MachIntTypesDesc[i].precision + TARGET_CHAR_BIT - 1) / TARGET_CHAR_BIT == MachIntTypesDesc[i].targetSize );

    // Generate the mask
    //
    TargetGenBitMask( MachIntTypesDesc[i].mask.b, 
                      sizeof( MachIntTypesDesc[i].mask.b ),
                      MachIntTypesDesc[i].precision );

    // The max value of the signed is all bit the highest bits set
    TARGET_SHR_UNSIGNED( &MachIntTypesDesc[i].maxValue, &MachIntTypesDesc[i].mask, 1 );
    // The min signed value is (unsigned)MaxValue + 1
    TARGET_ADD_UNSIGNED( &MachIntTypesDesc[i].minValue, &MachIntTypesDesc[i].maxValue, &Const1 );
  }
};

//--------------------------------------------------------------------------
// Name         MachConf_Init 
//
// 
//--------------------------------------------------------------------------
void MachConf_Init ( void )
{
  InitTypes();
};

