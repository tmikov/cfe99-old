#include "global.h"

#ifdef NO_ISBLANK
int isblank ( int c )
{
  return c == ' ' || c == '\t';
};
#endif

