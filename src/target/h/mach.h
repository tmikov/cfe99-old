/*
  mach.h:  Wraps together all machine-dependant but front-end-neutral
    defintions.
*/
#ifndef MACH_H
#define MACH_H

/*
  machconf.h must define:
  =======================

  MachIntType_t 
      Enumeration defining all separate integer types supported by the target
  MachIntType_t::mt_Last
      The last value of MachIntType_t.

  TARGET_CHAR_BIT
      Number of bits in a target _char_

  TARGET_Ixx_T
      Opaque types holding a value of a target type on the host

  TARGET_IMAX_T
      A synonim for the largest integer type of TARGET_Ixx_T, capable
      of holding values of all other integer types.

  TARGET_FLOAT_T
  TARGET_DOUBLE_T
  TARGET_LONG_DOUBLE_T
      Opaque types holding a value of a target IEEE floating point type on the host.

  Operations on TARGET_Ixx_T values
  ---------------------------------

  TARGET_ASSIGN_HOST_CHAR
  TARGET_ASSIGN_SMALL
  SMALL_FROM_TARGET
  TARGET_CLEAR_INT
  TARGET_ASSIGN_UNSIGNED
  UINT16_FROM_TARGET
  TARGET_ASSIGN_UINT16

  TARGET_AND
  TARGET_OR
  TARGET_ADD_SIGNED
  TARGET_ADD_UNSIGNED
  TARGET_MUL_UNSIGNED
  TARGET_SHL_UNSIGNED
  TARGET_SHR_UNSIGNED

  TARGET_EQ
  TARGET_LE_UNSIGNED
  TARGET_GT_UNSIGNED
  TARGET_LT_SIGNED
  TARGET_GT_SIGNED
*/
#include "machconf.h"       // target-specific machine type definitions

typedef struct
{
  MachIntType_t  mtId;
  uchar precision;   // number of bits
  uchar targetSize;  // number of target characters

  TARGET_IMAX_T  mask;
  TARGET_IMAX_T  minValue, maxValue;  // min & max signed value
} TMachIntTypeDesc;

extern TARGET_IMAX_T Const0;
extern TARGET_IMAX_T Const1; 

void Mach_Init ( void );

//===============================
//
// Public imports from machconf.c
//

///////////////////////////////////////////////////////////////////////////////
//      Define the properties of all target integer types
//      The types must be sorted according to their integer conversion rank
//
extern TMachIntTypeDesc MachIntTypesDesc[mt_Last]; 

#endif // MACH_H