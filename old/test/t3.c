
typedef float IDENT;

void func1 ( unsigned ((IDENT)) );

void func2 ( unsigned ((i)) );

void func3 ( unsigned ((int)) );

unsigned f1 ( IDENT );
unsigned f3 ( int );

void test ( void )
{
  func1( f1 );
  func2( 10 );
  func3( f3 );

  func1( f3 );  // bad
  func2( f1 );  // bad
  func3( 10 );  // bad
};

