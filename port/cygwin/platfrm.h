/*
  Platform-specific include files for 
  GCC 2.95, x86, CygWin
*/
#ifndef PLATFRM_H
#define PLATFRM_H

// ------------------------------------------------------------------
//
// Extensions
//
//
#define INTEGRAL_MAX_BITS 64

#define LITTLE_ENDIAN     1
#define BIG_ENDIAN        0

// ------------------------------------------------------------------
//
// C99 
//

#ifdef NO_STDINT
  #include "stdint.h"
#endif

// ------------------------------------------------------------------
//
// errno.h
//
// ------------------------------------------------------------------

#ifndef EILSEQ
  #define EILSEQ EINVAL
#endif

// ------------------------------------------------------------------
//
// limits.h
//
// ------------------------------------------------------------------

#ifndef LLONG_MIN
  #define LLONG_MIN         LONG_LONG_MIN
  #define LLONG_MAX         LONG_LONG_MAX
  #define ULLONG_MAX        ULONG_LONG_MAX
#endif

// ------------------------------------------------------------------
//
// stdlib.h
//
// ------------------------------------------------------------------

#ifdef NO_UNDER_EXIT
  #define _Exit( status )   _exit( status )
  #undef NO_UNDER_EXIT
#endif

#endif // PLATFRM_H
