#include "global.h"

#ifdef NO_LLDIV
lldiv_t lldiv ( longlong_t numer, ulonglong_t denom )
{
  lldiv_t res;

  res.quot = numer / denom;
  res.rem  = numer % denom;

  return res;
};
#endif

