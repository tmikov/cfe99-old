struct s1 { int a; };
struct s1; // OK
struct s1 { int a; }; // @err: tag already defined

union s1; // @err: invalid redeclaration of tag

union s2
{
  int a;
  int b;
};

void func1 ( void )
{
  union s1;             // @err: invalid redeclaration of tag
  union s2 { char a; }; // OK
}

struct s3 
{
  struct s1 a; // OK
  union s1     // @err: tag already defined
  {
    int a;
    int b;
  } c;
};

signed struct s1 v1;       // @err: 'signed'/'unsigned' only apply to 'int' and 'char'
static struct s1 long v2;  // @err: 'long' only applies to 'int' and 'double'         
int struct s1 v3;          // @err: More than one type in declaration                 

 
 
// bitfields

struct s4
{
  float m1 : 10;   // @err: bitfield requires integer type
//  int   m2 : 10/0; // @err: constant expression required
  int   m3 : 1.0;  // @err: constant integer expression required
//  int   m4 : -1;   // @err: negative bitfield width
  int   m5 : 1000; // @err: too large bitfield width
  char  m6 : 9;    // @err: too large bitfield width
  char  m8 : 7;    // OK
  char  m9 : 8;    // OK
  int   m10 : 0;    // @err: zero-width bitfield must be unnamed
  int       : 5;    // OK
  int       : 0;    // OK

//  int;             // @err: Declaration does not declare anything
};


struct s5;

void func2 ( void )
{
  struct s5 { int a; };    // OK - declares a new struct
  struct { struct s5 a; };
}

struct { struct s5 a; }; // @err: 'a': uses incomplete type