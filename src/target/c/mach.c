#include "precomp.h"
#include "mach_priv.h"

TARGET_IMAX_T Const0;
TARGET_IMAX_T Const1;

//--------------------------------------------------------------------------
// Name         Mach_Init 
//
// 
//--------------------------------------------------------------------------
void Mach_Init ( void )
{
  TARGET_ASSIGN_SMALL( &Const0, 0 );
  TARGET_ASSIGN_SMALL( &Const1, 1 );

  MachConf_Init();
};