
enum Enum2
{
  eb, ec
};

int eb; // @err: Redeclaration

int A[];
int A[5];
int A[10]; // @err: Redeclaration 

typedef int INT;
typedef int INT; // @warn: Redeclaration
typedef char INT; // @err: Redeclaration

int a, * b, c[10];
char c[10];       // @err: Redeclaration
int b;            // @err: Redeclaration
int * b;          
int a;

void func1 ( int a );

void func1 ()
{                 // @err: Redeclaration
}

void func2 ( a, b )
int * a, b[];
{
}

void func2 ( int a, int b ); // @err: Redeclaration

void func3 ( a, b )
int a;
int c; // @err: not in parameter list
{      // @err: 'b' not defined
}

void func4 ( int a )
char b;
{ // @err: mixing old-style and new-style
}

void func5
{            // @err: doesn't have function type
}

typedef void tf ( int a );

tf func6    // @err: can't use typedef to define function
{
}

void func7 ( int a, int b )
{
  int x;
  int b; // @err: redeclaration
  int y;
}

void func8 ( a, b )
char a;
int b;
{
  int a, b; //@err:  redeclaration
}  


