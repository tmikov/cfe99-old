/*
  Platform-specific include files for 
  Borland C 5.5, x86, Win32 
*/
#ifndef PLATFRM_H
#define PLATFRM_H

// ------------------------------------------------------------------
//
// Extensions
//
//

#define INTEGRAL_MAX_BITS  _INTEGRAL_MAX_BITS

#define LITTLE_ENDIAN     1
#define BIG_ENDIAN        0

// ------------------------------------------------------------------
//
// C99 
//

#ifdef NO_STDINT
  #include "stdint.h"
#endif

#ifdef NO_LONGLONG
  typedef int64_t  longlong_t;
  typedef uint64_t ulonglong_t;
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
  #define LLONG_MIN         INT64_MIN
  #define LLONG_MAX         INT64_MAX
  #define ULLONG_MAX        UINT64_MAX
#endif

// ------------------------------------------------------------------
//
// stdlib.h
//
// ------------------------------------------------------------------

#ifdef NO_STRTOLD
  #define strtold( nptr, endptr )  _strtold( nptr, endptr )
  #undef NO_STRTOLD
#endif

#ifdef NO_UNDER_EXIT
  #define _Exit( status )   _exit( status )
  #undef NO_UNDER_EXIT
#endif

#endif // PLATFRM_H
