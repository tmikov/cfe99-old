// declarations storage classes and scopes

// SCOPE_FILE -----------------------------------------------------

int; // @warning: doesn't declare anything

// SC_AUTO
auto     int g1;   // @err: invalid storage class
// SC_REGISTER
register int g2;   // @err: invalid storage class

// SC_NONE
typedef  int g3;
         int g3;   // @err: already declared

static   int g4;
         int g4;   // @err: storage class conflict

// SC_EXTERN
typedef  int g5;
extern   int g5;   // @err: already declared

typedef int f1;
void f1 ( void );  // @err: already declared

// SC_STATIC
typedef  int g6;
static   int g6;   // @err: already declared

         int g7;
static   int g7;   // @err: storage class conflict

extern   int g8;
static   int g8;   // @err: storage class conflict

void f2 ( void );
static void f2 ( void );  // @err: storage class conflict

extern void f3 ( void );
static void f3 ( void );  // @err: storage class conflict

// SC_TYPEDEF
typedef  int g9;
typedef  int g9;   // @warn: already declared

         int g10;
typedef  int g10;  // @err: already declared

extern   int g11;
typedef  int g11;  // @err: already declared

typedef  int f2;   // @err: already declared

static   int g12;
typedef  int g12;  // @err: already declared


// SCOPE_BLOCK -----------------------------------------------------

void func1 ( void )
{
           void f3 ( void ); // OK
  static   int f4 ( void ); // @err: invalid storage
  auto     int f5 ( void ); // @err: invalid storage
  register int f6 ( void ); // @err: invalid storage

  extern   int l1;  
           int l1; // @err: already declared
  auto     int l1; // @err: already declared
  static   int l1; // @err: already declared
  typedef  int l1; // @err: already declared

           int l2;
  extern   int l2; // @err: already declared 

  extern   int l4; 
  extern   int l4; // OK

  typedef  int l5;
  extern   int l5; // @err: already declared 

  int; // @warning: doesn't declare anything
}

// SCOPE_PARAMS -----------------------------------------------------

void func2 (
  extern   int p1, // @err: invalid storage
  static   int p2, // @err: invalid storage
  auto     int p3, // @err: invalid storage
  typedef  int p4, // @err: invalid storage

           int p5, // OK
  register int p6, // OK     

           int p6, // @err: already declared 
  register int p5, // @err: already declared 

  int              // OK
);


// SCOPE_STRUCT -----------------------------------------------------

struct struct1
{
  void func3 ( void ); // @err: not allowed

  extern   int m1; // @err: invalid storage
  static   int m2; // @err: invalid storage
  auto     int m3; // @err: invalid storage
  register int m4; // @err: invalid storage
  typedef  int m5; // @err: invalid storage

           int m6; // OK
           int m6; // @err: already declared 

//           int;    // err: empty not allowed

           int : 3; // OK
};


// -------------------------------------------------------------------

int g13;
int g14;

void func3 ( void )
{
  extern  int g13; // OK
  typedef int g13; // @err: already declared
  typedef int g14; // OK

  extern  int g15; // OK
  extern  int g16; // OK
  extern  int g16; // OK
  extern  int g15; // OK
}

int g15; // OK
static int g16; // @err: storage class conflict

typedef int g16; // OK - the extern declaration is not visible
typedef int g17;

static int g18;

void func4 ( void )
{
  extern int g16; // OK
  extern int g17; // OK - this overrides the typedef

  extern int g18; // OK
}


void func5 ( void pfunc ( void ) ); // OK - pfunc must not become extern
