typedef int * IDENT;


int func1 ( void )
{
  auto IDENT x;
  return *x;
};

int func2 ( void )
{
  auto signed IDENT;
  return IDENT;
};

int func3 ( void )
{
  auto x;
  return x;
};



