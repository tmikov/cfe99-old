typedef unsigned UINT;
typedef int INT;

int func1 ( int UINT )
{
  return UINT - 1;
};

struct S
{
  int UINT;
};

int func2 ( struct S * p )
{
  return p->UINT - 1;
};

int func3 ( void )
{
  int UINT = 0;
  return UINT - 1;
};

int func4 ( void )
{
label:
  {
    typedef int label;
    goto label;
  }
};

void func ( void )
{
  INT UINT;
};

