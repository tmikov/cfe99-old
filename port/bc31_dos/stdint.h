// ------------------------------------------------------------------
//
// C99 <stdint.h> for BC31/x86/DOS
//
// ------------------------------------------------------------------

#ifndef STDINT_H
#define STDINT_H

// ------------------------------------------------------------------
// 8
//
typedef signed  char     int8_t;
typedef unsigned char    uint8_t;

#define INT8_MIN         SCHAR_MIN
#define INT8_MAX         SCHAR_MAX
#define UINT8_MAX        UCHAR_MAX

#define INT8_C( x )      (int8_t)(x)
#define UINT8_C( x )     (uint8_t)(x)

// ------------------------------------------------------------------
// 16
//
typedef signed   int     int16_t;
typedef unsigned int     uint16_t;

#define INT16_MIN        INT_MIN
#define INT16_MAX        INT_MAX
#define UINT16_MAX       UINT_MAX

#define INT16_C( x )     x
#define UINT16_C( x )    x##u

// ------------------------------------------------------------------
// 32
//
typedef signed   long    int32_t;
typedef unsigned long    uint32_t;

#define INT32_MIN        LONG_MIN
#define INT32_MAX        LONG_MAX
#define UINT32_MAX       ULONG_MAX

#define INT32_C( x )     x##l
#define UINT32_C( x )    x##lu

// 8
//
typedef int8_t           int_least8_t;
typedef uint8_t          uint_least8_t;
typedef int16_t          int_fast8_t;
typedef uint16_t         uint_fast8_t;

#define INT_LEAST8_MIN   INT8_MIN
#define INT_LEAST8_MAX   INT8_MAX
#define UINT_LEAST8_MAX  UINT8_MAX
#define INT_FAST8_MIN    INT16_MIN
#define INT_FAST8_MAX    INT16_MAX
#define UINT_FAST8_MAX   UINT16_MAX

// 16
//
typedef int16_t          int_least16_t;
typedef uint16_t         uint_least16_t;
typedef int16_t          int_fast16_t;
typedef uint16_t         uint_fast16_t;

#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST16_MAX  INT16_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define INT_FAST16_MIN   INT16_MIN
#define INT_FAST16_MAX   INT16_MAX
#define UINT_FAST16_MAX  UINT16_MAX

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

// intptr_t
//
#if defined(__SMALL__) || defined(__TINY__)

typedef int16_t          intptr_t;
typedef uint16_t         uintptr_t;

#define INTPTR_MIN       INT16_MIN
#define INTPTR_MAX       INT16_MAX
#define UINTPTR_MAX      UINT16_MAX

#else

typedef int32_t          intptr_t;
typedef uint32_t         uintptr_t;

#define INTPTR_MIN       INT32_MIN
#define INTPTR_MAX       INT32_MAX
#define UINTPTR_MAX      UINT32_MAX

#endif

// intmax_t
//
typedef int32_t          intmax_t;
typedef uint32_t         uintmax_t;

#define INTMAX_MIN       INT32_MIN
#define INTMAX_MAX       INT32_MAX
#define UINTMAX_MAX      UINT32_MAX

#define INTMAX_C( x )    INT32_C( x )
#define UINTMAX_C( x )   UINT32_C( x )

#endif // STDINT_H

