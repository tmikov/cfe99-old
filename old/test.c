// GNU C extensions

int func1 ( int j )
{
  return ( { int i = 0; ++i; i; } );
}

void func2 ( void )
{
  ({
    __label__ l1;
    l1: goto l1;
  });
  ({
    __label__ l1;
    l1: goto l1;
  });
}

void func3 ( void )
{
  int * p = (int *)&&lab1 + 1;
lab1:
  goto *(p+1);
}

void func4 ( int i )
{
  typeof( i = 0, 1 ) j;
  typeof( int * ) pi;
}

int func5 ( int x, int y )
{
  return x ? : y;
}

void func6 ( int y )
{
  typedef ty = y + 10;
};

void func7 ( void )
{
  __label__ k;
  __label__ l;
  0;
};


void func8 ( void )
{
  int array[] = { [0 ... 5] = 1, [6 ... 10] = 10 };
};

/*
void func9 ( void )
{
  struct C { int x; int y; } c = { x: 1, y: 2 };
};
*/

int func10 ( int i )
{
  switch (i)
  {
    case 1 ... 5: return 1;
    default: return 0;
  }
};

;;

