/*
  Platform-specific include files for 
  Borland C 3.1, x86, DOS
*/
#ifndef PLATFRM_H
#define PLATFRM_H

// ------------------------------------------------------------------
//
// Extensions
//
//
#define INTEGRAL_MAX_BITS  32

#define LITTLE_ENDIAN     1
#define BIG_ENDIAN        0

// ------------------------------------------------------------------
//
// C99 
//

#ifdef NO_STDINT
  #include "bc31_dos\stdint.h"
#endif

#ifdef NO_LONGLONG
  typedef int32_t  longlong_t;
  typedef uint32_t ulonglong_t;
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
  #define LLONG_MIN         INT32_MIN
  #define LLONG_MAX         INT32_MAX
  #define ULLONG_MAX        UINT32_MAX
#endif

// ------------------------------------------------------------------
//
// stdlib.h
//
// ------------------------------------------------------------------

#ifdef NO_LLDIV
typedef ldiv_t lldiv_t;

#define lldiv ldiv

#undef NO_LLDIV
#endif

#ifdef NO_STRTOLD
  #define strtold( nptr, endptr )  _strtold( nptr, endptr )
  #undef NO_STRTOLD
#endif

#ifdef NO_UNDER_EXIT
  #define _Exit( status )   _exit( status )
  #undef NO_UNDER_EXIT
#endif

#endif // PLATFRM_H
