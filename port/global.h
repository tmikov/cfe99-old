#ifndef GLOBAL_H
#define GLOBAL_H

/*
  globconf.h macros
  ===================================================================

  standard headers
  ------------------------------------------------------

  NO_ISO646
    Doesn't have <iso646.h>

  NO_STDBOOL 
    Doesn't have <stdbool.h>

  NO_STDINT 
    Doesn't have <stdint.h>



  language features
  ------------------------------------------------------

  NO_RESTRICT 
    Doesn't support restrict

  NO_UNDER_RESTRICT 
    Doesn't support __restrict

  NO_INLINE 
    Doesn't support inline

  NO_UNDER_INLINE 
    Doesn't support __inline

  NO_LONGLONG 
    Doesn't support long long



  functions and types
  ------------------------------------------------------

  NO_ISBLANK
    Doesn't support <ctype.h>/isblank()

  NO_LLDIV 
    Doesn't have <stdlib.h>/lldiv_t/lldiv()

  NO_VA_COPY 
    Doesn't support <stdarg.h>/va_copy()
  
  NO_STRTOULL, NO_STRTOLL, NO_STRTOLD, NO_STRTOF, NO_ATOLL,
  NO_LLABS, NO_UNDER_EXIT (_Exit)
    The corresponding functions in stdlib.h are missing
*/
#include "globconf.h"

/*
  Config macros:

  _DEBUG
     Defined for debug builds.
*/

#if !defined(_DEBUG) && !defined(NDEBUG)
  #define NDEBUG
#endif

#if defined(_DEBUG) && defined(NDEBUG)
  #error Noth DEBUG and NDEBUG defined!
#endif



// ------------------------------------------------------------------
//
// Include portable header files which should be present on all
// platforms. As we encounter incompatibilities, more and more
// of these files will be moved to the platform-specific includes
//
// ------------------------------------------------------------------
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NO_ISO646
  #include <iso646.h>
#endif

#ifndef NO_STDBOOL
  #include <stdbool.h>
#endif

#ifndef NO_STDINT
  #include <stdint.h>
#endif

/*

  Declarations that must be provided in the platform-specific 
  includes.

  Macros
  ===================================================================

  INTERGRAL_MAX_BITS
    The bit width of the widest integral type declared in <stdint.h>

  SUPPORT_INLINE        macro optional
    If defined, the platform supports inline functions using the
    'inline' macro.
    
  FORCEINLINE           macro optional
    If SUPPORT_INLINE is defined, this macro is the keyword that should
    be used to force a function to be inlined. If the compiler doesn't
    support this, it acts as inline.

  LITTLE_ENDIAN         boolean macro required
    The system is little-endian

  BIG_ENDIAN            boolean macro required
    The system is big-endian

  NELEM( array )        macro optional

  ASSUME( cond )        macro optional
    Provides a hint to the optimizer by asserting that a condition is 
    true.

  ASSERT( cond )        macro optional
    assert( cond )

  VERIFY( cond )        macro provided
    Like ASSERT(), but evaluates the condition even in release builds.

  CHECK_ASSUME( cond )  macro provided
    ASSERT( cond ); ASSUME( cond )

  COMPILE_TIME_EXPR_ASSERT( cond ) macro optional
    An expression that causes the compilation to abort if _cond_ is false.
    Important: this must be an expression !

  COMPILE_TIME_ASSERT( cond ) macro optional
    A statement that casues the compilation to abort if _cond_ is false.
    Under some compilers this is stronger than COMPILE_TIME_EXPR_ASSERT()
    which only generates an warning.

  The COMPILE_TIME_xxx macroses could differ in behaviour between debug
  and release builds.


  Types
  ===================================================================
  schar_t, uchar_t   provided



  C99 
  ===================================================================

  restrict              macro  optional
    Define it to map to the compiler's restrict keyword
  longlong_t            
    Define it to represent long long
  ulonglong_t
    Define it to represent unsigned long long

  inline                macro optional
    If SUPPORT_INLINE is defined, this macro is the keyword that should
    be used for inline functions by putting it before the function
    declaration. Empty otherwise.

  C99 header files
  ===================================================================

  <assert.h>
      assert

  <ctype.h>
      isalnum, isalpha, isblank, iscntrl, isdigit, isgraph, islower, isprint
      ispunct, isspace, isupper, isxdigit, tolower, toupper

  <errno.h>
      EDOM, EILSEQ, ERANGE, errno

  <iso646.h>
      and, and_eq, bitand, bitor, compl, not, not_eq, or, or_eq, xor, xor_eq,      

  <limits.h>
      CHAR_BIT, SCHAR_MIN, SCHAR_MAX, UCHAR_MAX, CHAR_MIN, CHAR_MAX, MB_LEN_MAX
      SHRT_MIN, SHRT_MAX, USHRT_MAX, INT_MIN, INT_MAX, UINT_MAX, LONG_MIN, LONG_MAX
      ULONG_MAX, LLONG_MIN, LLONG_MAX, ULLONG_MAX

  <setjmp.h>
      jmp_buf, setjmp, longjmp

  <stdarg.h>
      va_list, va_arg, va_copy, va_end, va_start

  <stdbool.h>
      _Bool
      bool, true, false, __bool_true_false_are_defined

  <stddef.h>  
      ptrdiff_t, size_t, wchar_t, NULL, offsetof

  <stdint.h>
      intN_t, uintN_t, int_leastN_t, uint_leastN_t, int_fastN_t, uint_fastN_t
      intptr_t, uintptr_t, intmax_t, uintmax_t, INTN_MIN, INTN_MAX, UINTN_MAX
      INT_LEASTN_MIN, INT_LEASTN_MAX, UINT_LEASTN_MAX, INT_FASTN_MIN, INT_FASTN_MAX
      UINT_FASTN_MAX, INTPTR_MIN, INTPTR_MAX, UINTPTR_MAX, INTMAX_MIN, INTMAX_MAX
      UINTMAX_MAX, PTRDIFF_MIN, PTRDIFF_MAX, SIG_ATOMIC_MIN, SIG_ATOMIC_MAX
      SIZE_MAX, WCHAR_MIN, WCHAR_MAX, WINT_MIN, WINT_MAX, INTN_C(value), UINTN_C(value)
      INTMAX_C(value), UINTMAX_C(value)

  <stdio.h>
      size_t, FILE, fpos_t, NULL, _IOFBF, _IOLBF, _IONBF, BUFSIZ, EOF, FOPEN_MAX
      FILENAME_MAX, L_tmpnam, SEEK_CUR, SEEK_END, SEEK_SET, TMP_MAX
      stderr, stdin, stdout

      remove, rename, tmpfile, tmpnam, fclose, fflush, fopen, freopen, setbuf, setvbuf
      fprintf, fscanf, printf, scanf, snprintf, sprintf, sscanf, vfprintf, vfscanf, vprintf
      vscanf, vsnprintf, vsprintf, vsscanf, fgetc, fgets, fputc, fputs, getc, getchar, gets
      putc, putchar, puts, ungetc, fread, fwrite, fgetpos, fseek, fsetpos, ftell, rewind
      clearerr, feof, ferror, perror, remove, rename, tmpfile, tmpnam, fclose, fflush, fopen
      freopen, setbuf, setvbuf, fprintf, fscanf, printf, scanf, snprintf, sprintf, sscanf
      vfprintf, vfscanf, vprintf, vscanf, vsnprintf, vsprintf, vsscanf, fgetc, fgets, fputc
      fputs, getc, getchar, gets, putc, putchar, puts, ungetc, fread, fwrite, fgetpos, fseek
      fsetpos, ftell, rewind, clearerr, feof, ferror, perror,     

  <stdlib.h>
      size_t, wchar_t, div_t, ldiv_t, lldiv_t, NULL, EXIT_FAILURE, EXIT_SUCCESS, RAND_MAX
      MB_CUR_MAX

      atof, atoi, atol, atoll, strtod, strtof, strtold, strtol, strtoll, strtoul, strtoull
      rand, srand, calloc, free, malloc, realloc, abort, atexit, exit, _Exit, getenv, system
      bsearch, qsort, abs, labs, llabs, div, ldiv, lldiv, mblen, mbtowc, wctomb, mbstowcs
      wcstombs

  <string.h>
      size_t, NULL
      memcpy, memmove, strcpy, strncpy, 
      strcat, strncat, 
      memcmp, strcmp, strcoll, strncmp, strxfrm, 
      memchr, strchr, strcspn, strpbrk, strrchr, strspn, strstr, strtok, 
      memset, strerror, strlen
*/


// ------------------------------------------------------------------
//
//  Here we list all tested platform+compiler combinations and 
//  include their specific .h files.
//
// ------------------------------------------------------------------

#if (_MSC_VER >= 1200) && defined(_M_IX86) && defined(_WIN32)
//
// x86 Visual C 6.0 under Win32
//
  #include "vc6_x86_win32\platfrm.h"

#elif (__TURBOC__ >= 0x410) && defined(__MSDOS__)
//
// Borland C 3.1
//
  #include "bc31_dos\platfrm.h"

#elif (__TURBOC__ >= 0x550)
//
// Borland C 5.5
//
  #include "bc55_x86_win32\platfrm.h"

#elif defined(__CYGWIN__)
//
// Cygwin and GCC
//
  #include "cygwin\platfrm.h"

#else
#  error Probably unsupported compiler/host platform. 
#endif

#ifdef NO_ISO646
  #include "common\iso646.h"
#endif

#ifdef NO_STDBOOL
  #include "common\stdbool.h"
#endif


#ifdef NO_RESTRICT
  #ifdef NO_UNDER_RESTRICT
    #define restrict
  #else  
    #define restrict __restrict
  #endif
#endif

#ifdef NO_INLINE
  #ifdef NO_UNDER_INLINE
    #define inline
  #else
    #define inline  __inline
    #define SUPPORT_INLINE  1
  #endif
#else
  #define SUPPORT_INLINE  1
#endif

#ifndef NO_LONGLONG
typedef long long          longlong_t;
typedef unsigned long long ulonglong_t;
#endif


// ------------------------------------------------------------------
//
// ctype.h
//
// ------------------------------------------------------------------

#ifdef NO_ISBLANK
int isblank ( int c );
#endif


// ------------------------------------------------------------------
//
// stdarg.h
//
// ------------------------------------------------------------------

#ifdef NO_VA_COPY
  #define va_copy( dest, src )   ((void)((dest) = (src)))
#endif


// ------------------------------------------------------------------
//
// stdlib.h
//
// ------------------------------------------------------------------

#ifdef NO_LLDIV
typedef struct _lldiv_t
{
  longlong_t  quot;
  ulonglong_t rem;
} lldiv_t;

lldiv_t lldiv ( longlong_t numer, ulonglong_t denom );
#endif

#ifdef NO_STRTOULL
  ulonglong_t strtoull( const char * restrict nptr, char ** restrict endptr, int base );
#endif

#ifdef NO_STRTOLL
  longlong_t strtoll( const char * restrict nptr, char ** restrict endptr, int base );
#endif

#ifdef NO_STRTOLD
  long double strtold( const char * restrict nptr, char ** restrict endptr );
#endif

#ifdef NO_STRTOF
  float strtof( const char * restrict nptr, char ** restrict endptr );
#endif

#ifdef NO_ATOLL
  //  longlong_t atoll( const char * nptr );
  #define atoll( nptr )  strtoll( nptr, (char **)NULL, 10 )
#endif

#ifdef NO_LLABS
  longlong_t llabs( longlong_t j );
#endif

#ifdef NO_UNDER_EXIT
  void _Exit( int status );
#endif

// ------------------------------------------------------------------
//
// Provide optional macro definitions
//
// ------------------------------------------------------------------

#ifndef FORCEINLINE
#  define FORCEINLINE inline
#endif

#ifndef ASSUME
#  define ASSUME( cond )        ((void)(cond))
#endif

#ifndef ASSERT
#  define ASSERT( x )  assert( x )
#endif

#ifndef NELEM
#  define NELEM( x )            (sizeof( x ) / sizeof( (x)[0] ))
#endif

#ifndef COMPILE_TIME_EXPR_ASSERT
  #define COMPILE_TIME_EXPR_ASSERT( cond )  (void)(1/((cond) != 0))
#endif

#ifndef COMPILE_TIME_ASSERT
#  define COMPILE_TIME_ASSERT( cond )  COMPILE_TIME_EXPR_ASSERT( cond )
#endif

// ------------------------------------------------------------------
//
// Define portable macros
//
// ------------------------------------------------------------------

#define CHECK_ASSUME( cond )   ASSERT( cond ); ASSUME( cond )

#ifdef _DEBUG
#  define VERIFY( x )  ASSERT( x )
#else
#  define VERIFY( x )  (x)
#endif


typedef signed char   schar_t;
typedef unsigned char uchar_t;

#endif // GLOBAL_H
