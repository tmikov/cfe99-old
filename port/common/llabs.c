#include "global.h"

#ifdef NO_LLABS

longlong_t llabs( longlong_t j )
{
  return j >= 0 ? j : -j;
};

#endif
