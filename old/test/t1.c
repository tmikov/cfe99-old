// test scopes and namespaces

typedef double IDENT;

struct IDENT { IDENT IDENT; };

struct IDENT id;

void func10 (i,j) {};

void func9 (i,IDENT) {};

//void func8 (IDENT,i) {};

void func7 ( IDENT IDENT )
{
};

void func6 ( IDENT x, IDENT IDENT )
{
};

void func5 ( void )
{
  IDENT x;
  IDENT IDENT;
  IDENT = 0;
};

void func4 ( void )
{
  typedef long IDENT;
  IDENT l;
};

void func3 ( void )
{
  enum 
  {
    IDENT = 0, 
  };
};

/*
void func2 ( void )
{
goto IDENT;
  IDENT:; // is valid
};
*/

void * func1 ( void )
{
  return "aaaaa" L"bbbbb";
};

