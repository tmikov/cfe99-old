#include <stdarg.h>

void test ( int arg, ... )
{
  va_list ap, ap1;
  va_start( ap, arg );
  va_copy( ap1, ap );
  va_end( ap );
  va_end( ap1 );
};

int main ( void )
{
  test( 1, 2 );
  return 0;
};
