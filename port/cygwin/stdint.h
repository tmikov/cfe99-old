// ------------------------------------------------------------------
//
// C99 <stdint.h> for GCC/x86/Cygwin
//
// ------------------------------------------------------------------

#ifndef STDINT_H
#define STDINT_H

// ------------------------------------------------------------------
// 8
//
#ifndef __BIT_TYPES_DEFINED__
typedef signed   char    int8_t;
#endif
typedef unsigned char    uint8_t;

#define INT8_MIN         SCHAR_MIN
#define INT8_MAX         SCHAR_MAX
#define UINT8_MAX        UCHAR_MAX

#define INT8_C( x )      (int8_t)(x)
#define UINT8_C( x )     (uint8_t)(x)

// ------------------------------------------------------------------
// 16
//
#ifndef __BIT_TYPES_DEFINED__
typedef signed   short   int16_t;
#endif
typedef unsigned short   uint16_t;

#define INT16_MIN        SHRT_MIN
#define INT16_MAX        SHRT_MAX
#define UINT16_MAX       USHRT_MAX

#define INT16_C( x )     (int16_t)(x)
#define UINT16_C( x )    (uint16_t)(x)

// ------------------------------------------------------------------
// 32
//
#ifndef __BIT_TYPES_DEFINED__
typedef signed   int     int32_t;
#endif
typedef unsigned int     uint32_t;

#define INT32_MIN        INT_MIN
#define INT32_MAX        INT_MAX
#define UINT32_MAX       UINT_MAX

#define INT32_C( x )     (int32_t)(x)
#define UINT32_C( x )    (uint32_t)(x)

// ------------------------------------------------------------------
// 64
//
#ifndef __BIT_TYPES_DEFINED__
typedef signed long long   int64_t;
#endif
typedef unsigned long long uint64_t;

#define INT64_MIN        LONG_LONG_MIN
#define INT64_MAX        LONG_LONG_MAX
#define UINT64_MAX       ULONG_LONG_MAX

#define INT64_C( x )     x##LL
#define UINT64_C( x )    x##ULL


// 8
//
typedef int8_t           int_least8_t;
typedef uint8_t          uint_least8_t;
typedef int32_t          int_fast8_t;
typedef uint32_t         uint_fast8_t;

#define INT_LEAST8_MIN   INT8_MIN
#define INT_LEAST8_MAX   INT8_MAX
#define UINT_LEAST8_MAX  UINT8_MAX
#define INT_FAST8_MIN    INT32_MIN
#define INT_FAST8_MAX    INT32_MAX
#define UINT_FAST8_MAX   UINT32_MAX

// 16
//
typedef int16_t          int_least16_t;
typedef uint16_t         uint_least16_t;
typedef int32_t          int_fast16_t;
typedef uint32_t         uint_fast16_t;

#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST16_MAX  INT16_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define INT_FAST16_MIN   INT32_MIN
#define INT_FAST16_MAX   INT32_MAX
#define UINT_FAST16_MAX  UINT32_MAX

// 32
//
typedef int32_t          int_least32_t;
typedef uint32_t         uint_least32_t;
typedef int32_t          int_fast32_t;
typedef uint32_t         uint_fast32_t;

#define INT_LEAST32_MIN  INT32_MIN
#define INT_LEAST32_MAX  INT32_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define INT_FAST32_MIN   INT32_MIN
#define INT_FAST32_MAX   INT32_MAX
#define UINT_FAST32_MAX  UINT32_MAX

// 64
//
typedef int64_t          int_least64_t;
typedef uint64_t         uint_least64_t;
typedef int64_t          int_fast64_t;
typedef uint64_t         uint_fast64_t;

#define INT_LEAST64_MIN  INT64_MIN
#define INT_LEAST64_MAX  INT64_MAX
#define UINT_LEAST64_MAX UINT64_MAX
#define INT_FAST64_MIN   INT64_MIN
#define INT_FAST64_MAX   INT64_MAX
#define UINT_FAST64_MAX  UINT64_MAX

// intptr_t
//
typedef int32_t          intptr_t;
typedef uint32_t         uintptr_t;

#define INTPTR_MIN       INT32_MIN
#define INTPTR_MAX       INT32_MAX
#define UINTPTR_MAX      UINT32_MAX

// intmax_t
//
typedef int64_t          intmax_t;
typedef uint64_t         uintmax_t;

#define INTMAX_MIN       INT64_MIN
#define INTMAX_MAX       INT64_MAX
#define UINTMAX_MAX      UINT64_MAX

#define INTMAX_C( x )    INT64_C( x )
#define UINTMAX_C( x )   UINT64_C( x )

#endif // STDINT_H
