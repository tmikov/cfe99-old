/*
  Platform-specific include files for 
  Visual C 6.0, x86, Win32 
*/
#ifndef PLATFRM_H
#define PLATFRM_H

// ------------------------------------------------------------------
//
// Extensions
//
//

#define INTEGRAL_MAX_BITS  _INTEGRAL_MAX_BITS

#if _MSC_VER >= 1200
#  define FORCEINLINE     __forceinline
#endif

#define LITTLE_ENDIAN     1
#define BIG_ENDIAN        0

#if _MSC_VER >= 1200
#  define ASSUME( x )     __assume( x )
#endif


// ------------------------------------------------------------------
//
// C99 
//

// ------------------------------------------------------------------
//
// stdint.h
//
// ------------------------------------------------------------------

#ifdef NO_STDINT
  #include "stdint.h"
#endif


#ifdef NO_LONGLONG
  typedef int64_t  longlong_t;
  typedef uint64_t ulonglong_t;
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
  // VC doesn't support long double
  #define strtold( nptr, endptr )  strtod( nptr, endptr )
  #undef NO_STRTOLD
#endif

#ifdef NO_UNDER_EXIT
  #define _Exit( status )   _exit( status )
  #undef NO_UNDER_EXIT
#endif


#endif // PLATFRM_H
