/*
  In this file GCC means GCC on C mode.
  VC means VC,BCC32,IAR in C mode and VC,BCC32,IAR,GCC in C++ mode.

  I shall safely assume that what is marked as VC *is* the desired
  behaviour.
  This means that uncomplete type are assumed to have global scope.
*/

                            // GCC:assumes a local struct
void test ( struct C * p ); // VC:assumes a global struct


void func ( void )
{
                 // GCC:assumes a local struct
  struct C * p;  // VC:assumes a global struct

                 // GCC: this is a local struct. The above matches it,
  struct C       // VC:this is a local struct, different from the above
  {
    int a, b, c;
  } x;

                 // GCC: ok
  p = &x;        // VC:incompatible types

  test( p );     // GCC:incompatible types
}

struct C
{
  int x, y;
};

void test ( struct C * p )  // GCC:incorrect redeclaration
{
  p->x = 0;
}


