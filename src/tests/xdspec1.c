__declspec(wrong) int v1; // @err: unrecognized __declspec

extern __declspec(dllimport) int v2;
__declspec(dllexport) int v3;


extern __declspec(dllimport) __declspec(dllexport) int v4; // @err: More than one __declspec(dllimport/dllexport) in declaration
extern __declspec(dllimport) __declspec(dllimport) int v5; // @err: More than one __declspec(dllimport/dllexport) in declaration
extern __declspec(dllexport) __declspec(dllexport) int v6; // @err: More than one __declspec(dllimport/dllexport) in declaration

__declspec(noreturn) void func1 ( void );
__declspec(noreturn)  __declspec(noreturn) void func2 ( void ); // @err: More than one __declspec(noreturn) in declaration
__declspec(dllimport) __declspec(noreturn) void func3 ( void );
__declspec(dllexport) __declspec(noreturn) void func4 ( void );

__declspec(noreturn) int v7; // @err: __declspec(noreturn) only applies to functions

static __declspec(dllexport) int v8; //@err: __declspec(dllimport/dllexport) only applies to public symbols

__declspec(dllimport) int v9; //@err: __declspec(dllimport) must be used on external definitions

void func5 ( void )
{
  static __declspec(dllexport) int l1; //@err: __declspec(dllimport/dllexport) only applies to public symbols
  static __declspec(dllimport) int l2; //@err: __declspec(dllimport) must be used on external definitions
  __declspec(dllexport) int l3; //@err: __declspec(dllimport/dllexport) only applies to public symbols
  __declspec(dllimport) int l4; //@err: __declspec(dllimport/dllexport) only applies to public symbols
  extern __declspec(dllexport) int l5; 
  extern __declspec(dllimport) int l6; 
}

struct s1
{
  __declspec(dllexport) int m1; //@err: __declspec(dllimport/dllexport) only applies to public symbols
  __declspec(dllimport) int m2; //@err: __declspec(dllimport) must be used on external definitions
};

__declspec(dllimport) void func6 ( void ) 
{ //@err: __declspec(dllimport) used with a function definition
}

__declspec(noreturn) void func7 ( void );
void func7 ( void );
void func8 ( void );
__declspec(noreturn) void func8 ( void );


/*
  dllexport/dllimport valid cases:

  prev       sym
  ----------------
  nothing    nothing
  dllexport  dllexport
  dllimport  dllimport
  dllexport  nothing
*/
int v8_1;
int v8_1;
__declspec(dllexport) int v9_1;
__declspec(dllexport) int v9_1;
extern __declspec(dllimport) int v10;
extern __declspec(dllimport) int v10;

__declspec(dllexport) int v11;
int v11;

/*
  dllexport/dllimport bad cases:

  prev       sym
  ----------------
  nothing    dllexport
  nothing    dllimport
  dllexport  dllimport
  dllimport  dllexport
  dllimport  nothing

*/
int v12;
__declspec(dllexport) int v12;        //@err:__declspec(dllimport/dllexport) conflict with previous declaration

int v13;
extern __declspec(dllimport) int v13; //@err:__declspec(dllimport/dllexport) conflict with previous declaration

__declspec(dllexport) int v14;
extern __declspec(dllimport) int v14; //@err:__declspec(dllimport/dllexport) conflict with previous declaration

extern __declspec(dllimport) int v15;
__declspec(dllexport) int v15;        //@err:__declspec(dllimport/dllexport) conflict with previous declaration

extern __declspec(dllimport) int v16;
int v16;                              //@err:__declspec(dllimport/dllexport) conflict with previous declaration