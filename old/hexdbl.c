#include "precomp.h"
#include "tconf.h"

// Size of the fractional part big enough to hold 54 bits

#define OUR_FRACT_SIZE  ((54 + CHAR_BIT - 1) / CHAR_BIT)

//--------------------------------------------------------------------------
// Name         HexStringToDouble 
//
// {HEXFRACT_OR_INT}[pP][+-]?[0-9]+
//--------------------------------------------------------------------------
const char * HexStringToDouble ( const char * text, IEEE_double_t * res )
{
  int binExp;   // exponent in powers of 2
  UCHAR fract[OUR_FRACT_SIZE];
  UCHAR t[OUR_FRACT_SIZE];
  bool bAfterThePoint; // after the decimal point

  // fract = 0
  memcpy( fract, zero_constant, sizeof(fract) );

  // skip leading zeroes to improve speed
  while (*text == '0')
    ++text;

  overflow_bit = false;

  binExp = 0;
  bAfterThePoint = false;

  for (;; ++text )
  {
    if (*text == '.')
      bAfterThePoint = true;
    else
    if (!isxdigit( *text ))
      break;
    else
    {
      UCHAR digit;

      // After the decimal point we update the exponent to compensate for with each 
      // additional digit we add to the fraction
      //
      if (bAfterThePoint)
        binExp -= 4;

      // fract *= 16
      unsigned_integer_shift_left( sizeof(fract), fract, 4, fract );
      if (overflow_bit)
        return NULL;

      digit = tolower(*text);
      if (digit >= 'a')
        digit -= 'a' - 10;
      else
        digit -= '0';

      // t = digit
      change_unsigned_integer_size( sizeof(digit), &digit, sizeof(t), t );

      // fract += digit
      add_unsigned_integer( sizeof(fract), fract, t, fract );
      if (overflow_bit)
        return NULL;
    }
  }

  // Now the exponent
  ASSERT( *text == 'p' || *text == 'P' );
  ++text;

  // Convert it to int and add it to our updated exponent
  // text will point to the first character after the exponent
  binExp += strtol( text, &text, 10 ); 

  // Now in fract[] we have the mantissa and in binExp we have the 
  // binary exponent. 

  *res = IEEE_double_from_unsigned_integer( sizeof(fract), fract );

  // Now for a very, very stupid approach, but at least something
  // that should generate at least vaguely correct result
  //
  {
    UCHAR t = 2;
    IEEE_double_t two = IEEE_double_from_unsigned_integer( sizeof(t), &t );

    if (binExp < 0)
    {
      do
        *res = IEEE_divide_double( *res, two );
      while (++binExp < 0);
    }
    else
    {
      while (binExp-- > 0)
        *res = IEEE_multiply_double( *res, two );
    }
  }

  return text;
};
