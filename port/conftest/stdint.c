#include <stdint.h>
int main ( void )
{
  int_least8_t  il8      = INT8_C( 0 );
  uint_least8_t uil8     = UINT8_C( 0 );
  int_least8_t  il8_min  = INT_LEAST8_MIN;
  int_least8_t  il8_max  = INT_LEAST8_MAX;
  uint_least8_t uil8_max = UINT_LEAST8_MAX;
  int_fast8_t   if8_min  = INT_FAST8_MIN;
  int_fast8_t   if8_max  = INT_FAST8_MAX;
  uint_fast8_t  uif8_max = UINT_FAST8_MAX;

  int_least16_t  il16      = INT16_C( 0 );
  uint_least16_t uil16     = UINT16_C( 0 );
  int_least16_t  il16_min  = INT_LEAST16_MIN;
  int_least16_t  il16_max  = INT_LEAST16_MAX;
  uint_least16_t uil16_max = UINT_LEAST16_MAX;
  int_fast16_t   if16_min  = INT_FAST16_MIN;
  int_fast16_t   if16_max  = INT_FAST16_MAX;
  uint_fast16_t  uif16_max = UINT_FAST16_MAX;

  int_least32_t  il32      = INT32_C( 0 );
  uint_least32_t uil32     = UINT32_C( 0 );
  int_least32_t  il32_min  = INT_LEAST32_MIN;
  int_least32_t  il32_max  = INT_LEAST32_MAX;
  uint_least32_t uil32_max = UINT_LEAST32_MAX;
  int_fast32_t   if32_min  = INT_FAST32_MIN;
  int_fast32_t   if32_max  = INT_FAST32_MAX;
  uint_fast32_t  uif32_max = UINT_FAST32_MAX;

  return 0;
};
