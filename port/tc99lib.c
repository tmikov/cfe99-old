#include "global.h"


void * use ( void * arg, ... )
{
  return arg;
};

/*
  restrict
  longlong_t
  ulonglong_t_t
*/
void test_restrict ( void )
{
  ulonglong_t * restrict a;
  longlong_t * restrict b;
  use( &a, &b );
};

/*
  <assert.h>
      assert
*/
void test_assert_h ( void )
{
  bool cond;
  use( &cond );
  assert( cond );
};

/*
  <ctype.h>
      isalnum, isalpha, isblank, iscntrl, isdigit, isgraph, islower, isprint
      ispunct, isspace, isupper, isxdigit, tolower, toupper
*/
void test_ctype_h ( void )
{
  int c;

  use( &c );
  use( NULL,
    isalnum( c ),
    isalnum( c ), 
    isalpha( c ), 
    isblank( c ), 
    iscntrl( c ), 
    isdigit( c ), 
    isgraph( c ), 
    islower( c ), 
    isprint( c ),
    ispunct( c ), 
    isspace( c ), 
    isupper( c ), 
    isxdigit( c ), 
    tolower( c ), 
    toupper( c )
  );
};

/*
  <errno.h>
      EDOM, EILSEQ, ERANGE, errno
*/
void test_errno_h ( void )
{
  use( NULL, EDOM, EILSEQ, ERANGE, errno );
};

/*
  <iso646.h>
      and, and_eq, bitand, bitor, compl, not, not_eq, or, or_eq, xor, xor_eq,      
*/
void test_iso646_h ( void )
{
  int a, b, c, d, e, res;

  use( &a, &b, &c, &d, &e );

  res = a and 10;
  res and_eq not compl(b bitand 20 bitor 30) not_eq (c or 10);
  res or_eq d xor 10;
  res xor_eq e;

  use( &res );
};

/*
  <limits.h>
      CHAR_BIT, SCHAR_MIN, SCHAR_MAX, UCHAR_MAX, CHAR_MIN, CHAR_MAX, MB_LEN_MAX
      SHRT_MIN, SHRT_MAX, USHRT_MAX, INT_MIN, INT_MAX, UINT_MAX, LONG_MIN, LONG_MAX
      ULONG_MAX, LLONG_MIN, LLONG_MAX, ULLONG_MAX
*/
void test_limits_h ( void )
{
  use( NULL,
    CHAR_BIT,
    SCHAR_MIN,
    SCHAR_MAX,
    UCHAR_MAX, 
    CHAR_MIN, 
    CHAR_MAX, 
    MB_LEN_MAX, 
    SHRT_MIN, 
    SHRT_MAX, 
    USHRT_MAX, 
    INT_MIN, 
    INT_MAX, 
    UINT_MAX, 
    LONG_MIN, 
    LONG_MAX, 
    ULONG_MAX, 
    LLONG_MIN, 
    LLONG_MAX, 
    ULLONG_MAX
  );
};

/*
  <setjmp.h>
      jmp_buf, setjmp, longjmp
*/
void test_setjmp_h ( void )
{
  jmp_buf jb;
  int res;

  if ((res = setjmp( jb )) == 0)
    longjmp( jb, res + 1 );

  use( &res );
};

/*
  <stdarg.h>
      va_list, va_arg, va_copy, va_end, va_start
*/
void test_stdarg_h ( int arg1, ... )
{
  va_list ap1, ap2;

  va_start( ap1, arg1 );
  va_copy( ap2, ap1 );

  use( NULL, va_arg( ap1, int ) );
  use( &ap2 );

  va_end( ap2 );
  va_end( ap1 );
};

/*
  <stdbool.h>
      _Bool
      bool, true, false, __bool_true_false_are_defined
*/
void test_stdbool_h ( void )
{
  _Bool b1;
  bool b2;
  use( &b1, &b2, true, false, __bool_true_false_are_defined );
};


/*
  <stddef.h>  
      ptrdiff_t, size_t, wchar_t, NULL, offsetof
*/
void test_stddef_h ( void )
{
  ptrdiff_t pd;
  size_t sz;
  wchar_t wc;
  struct Coord { int x, y; };

  use( &pd, &sz, &wc, NULL, offsetof( struct Coord, y ) );
};


/*
  <stdint.h>
      intN_t, uintN_t, int_leastN_t, uint_leastN_t, int_fastN_t, uint_fastN_t
      intptr_t, uintptr_t, intmax_t, uintmax_t, INTN_MIN, INTN_MAX, UINTN_MAX
      INT_LEASTN_MIN, INT_LEASTN_MAX, UINT_LEASTN_MAX, INT_FASTN_MIN, INT_FASTN_MAX
      UINT_FASTN_MAX, INTPTR_MIN, INTPTR_MAX, UINTPTR_MAX, INTMAX_MIN, INTMAX_MAX
      UINTMAX_MAX, PTRDIFF_MIN, PTRDIFF_MAX, SIG_ATOMIC_MIN, SIG_ATOMIC_MAX
      SIZE_MAX, WCHAR_MIN, WCHAR_MAX, WINT_MIN, WINT_MAX, INTN_C(value), UINTN_C(value)
      INTMAX_C(value), UINTMAX_C(value)
*/
#define TEST_TYP( typ, TYP, iv, uv )                         \
void test_##typ ( void )                                     \
{                                                            \
  typ##_t    i;                                              \
  u##typ##_t u;                                              \
  use( &i, &u, iv, uv, TYP##_MIN, TYP##_MAX, U##TYP##_MAX ); \
};

TEST_TYP( int_least8, INT_LEAST8, 10, 20 );
TEST_TYP( int_fast8, INT_FAST8, 10, 20 );
#ifdef INT8_MIN
TEST_TYP( int8, INT8, INT8_C(10), INT8_C(20) );
#  define TEST_INT8      test_int8(), test_int_fast8(), test_int_least8()
#else
#  define TEST_INT8      test_int_fast8(), test_int_least8()
#endif

TEST_TYP( int_least16, INT_LEAST16, 10, 20 );
TEST_TYP( int_fast16, INT_FAST16, 10, 20 );
#ifdef INT16_MIN
TEST_TYP( int16, INT16, INT16_C(10), INT16_C(20) );
#  define TEST_INT16      test_int16(), test_int_fast16(), test_int_least16()
#else
#  define TEST_INT16      test_int_fast16(), test_int_least16()
#endif

#if INTEGRAL_MAX_BITS >= 32
TEST_TYP( int_least32, INT_LEAST32, 10, 20 );
TEST_TYP( int_fast32, INT_FAST32, 10, 20 );
#ifdef INT32_MIN
TEST_TYP( int32, INT32, INT32_C(10), INT32_C(20) );
#  define TEST_INT32      test_int32(), test_int_fast32(), test_int_least32()
#else
#  define TEST_INT32      test_int_fast32(), test_int_least32()
#endif
#else // 32
#  define TEST_INT32      ((void)0)
#endif // 32

#if INTEGRAL_MAX_BITS >= 64
TEST_TYP( int_least64, INT_LEAST64, 10, 20 );
TEST_TYP( int_fast64, INT_FAST64, 10, 20 );
#ifdef INT64_MIN
TEST_TYP( int64, INT64, INT64_C(10), INT64_C(20) );
#  define TEST_INT64      test_int64(), test_int_fast64(), test_int_least64()
#else
#  define TEST_INT64      test_int_fast64(), test_int_least64()
#endif
#else // 64
#  define TEST_INT64      ((void)0)
#endif // 64


#if INTEGRAL_MAX_BITS >= 128
TEST_TYP( int_least128, INT_LEAST128, 10, 20 );
TEST_TYP( int_fast128, INT_FAST128, 10, 20 );
#ifdef INT128_MIN
TEST_TYP( int128, INT128, INT128_C(10), INT128_C(20) );
#  define TEST_INT128      test_int128(), test_int_fast128(), test_int_least128()
#else
#  define TEST_INT128      test_int_fast128(), test_int_least128()
#endif
#else  // 128
#  define TEST_INT128      ((void)0)
#endif // 128

TEST_TYP( intmax, INTMAX, INTMAX_C(10), UINTMAX_C(20) );
TEST_TYP( intptr, INTPTR, 10, 20 );

void test_stdint_h ( void )
{
  TEST_INT8;
  TEST_INT16;
  TEST_INT32;
  TEST_INT64;
  TEST_INT128;
  test_intmax();
  test_intptr();
};

/*
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
*/
void test_stdio_h ( void )
{
};

/*
  <stdlib.h>
      size_t, wchar_t, div_t, ldiv_t, lldiv_t, NULL, EXIT_FAILURE, EXIT_SUCCESS, RAND_MAX
      MB_CUR_MAX

      atof, atoi, atol, atoll, strtod, strtof, strtold, strtol, strtoll, strtoul, strtoull
      rand, srand, calloc, free, malloc, realloc, abort, atexit, exit, _Exit, getenv, system
      bsearch, qsort, abs, labs, llabs, div, ldiv, lldiv, mblen, mbtowc, wctomb, mbstowcs
      wcstombs
*/
void test_stdlib_h ( void )
{
  size_t sz;
  wchar_t wc;
  div_t dt   = div( 100, 3 );
  ldiv_t ldt = ldiv( 100, 3 );
  lldiv_t lldt = lldiv( 100, 3 );

  use( &sz, &wc, &dt, &ldt, &lldt, NULL, EXIT_FAILURE, EXIT_SUCCESS, RAND_MAX, MB_CUR_MAX );

  atof( "1.2" );
  atoi( "10" );
  atol( "10" );
  atoll( "10" );
  strtod( "1.2", NULL );
  strtof( "1.2", NULL );
  strtold( "1.2", NULL );
  strtol( "10", NULL, 10 );
  strtoll( "10", NULL, 10 );
  strtoul( "10", NULL, 10 );
  strtoull( "10", NULL, 10 );
  rand();
  srand( 100 );
  calloc( 10, 10 );
  free( NULL );
  malloc( 100 );
  realloc( NULL, 100 );
  abort();
  atexit( NULL );
  exit( 10 );
  _Exit( 10 );
  getenv( "INCLUDE" );
  system( "dir" );
  bsearch( NULL, NULL, 10, 10, NULL );
  qsort( NULL, 10, 10, NULL );
  abs( 10 );
  labs( 10 );
  llabs( 10 );
// mblen( "abab", 10 );
// mbtowc
// wctomb
// mbstowcs
// wcstombs
};

/*
  <string.h>
      size_t, NULL
      memcpy, memmove, strcpy, strncpy, 
      strcat, strncat, 
      memcmp, strcmp, strcoll, strncmp, strxfrm, 
      memchr, strchr, strcspn, strpbrk, strrchr, strspn, strstr, strtok, 
      memset, strerror, strlen
*/
void test_string_h ( void )
{
  size_t sz;
  char buf[128];

  use( buf, &sz, NULL );

  use( 
    memcpy( buf, "memcpy", 6 ),
    memmove( buf, buf, sz ),
    strcpy( buf, "strcpy" ),
    strncpy( buf, "strncpy", 7 ),
    strcat( buf, "strcat" ),
    strncat( buf, "strncat", 7 ),
    memcmp( buf, buf, sz ),
    strcmp( buf, buf ),
    strcoll( buf, buf ),
    strncmp( buf, buf, 3 ),
    strxfrm( buf, "abc", 3 ),
    memchr( buf, 'e', sz ),
    strchr( buf, 'e' ),
    strcspn( buf, "abc" ),
    strpbrk( buf, "abc" ),
    strrchr( buf, 'e' ),
    strspn( buf, "abc" ),
    strstr( buf, "aba" ),
    strtok( buf, " \t" ),
    memset( buf, 0, sz ),
    strerror( EDOM ),
    strlen( buf )
  );
};

inline int test_inline ( void ) { return 10; };
FORCEINLINE int test_forceinline ( void ) { return 10; };

void test_extensions ( void )
{
  int array[10];
  schar_t sc;
  uchar_t uc;

  use( array );

  use( NULL,
    INTEGRAL_MAX_BITS,
    test_inline(),
    test_forceinline(),
    LITTLE_ENDIAN,
    BIG_ENDIAN,
    NELEM( array ),
    (COMPILE_TIME_EXPR_ASSERT( sizeof(array) > 0 ), array[0]),
    &sc,
    &uc
  );

  ASSUME( array[0] > 0 );
  ASSERT( array[0] > 0 );
  VERIFY( array[0] > 0 );
  CHECK_ASSUME( array[0]  > 0 );

  COMPILE_TIME_EXPR_ASSERT( sizeof(array) > 0 );
};


int main ( void )
{
  test_restrict();
  test_assert_h();
  test_ctype_h();
  test_errno_h();
  test_iso646_h();
  test_limits_h();
  test_setjmp_h();
  test_stdarg_h( 1, 2 );
  test_stdbool_h();
  test_stddef_h();
  test_stdint_h();
  test_stdio_h();
  test_stdlib_h();
  test_string_h();
  test_extensions();

  return EXIT_SUCCESS;
};
