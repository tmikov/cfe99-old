_Pragma   (  "test1" )  _Pragma("test1")  // OK

_Pragma ;            // @error
_Pragma( ;           // @error
_Pragma( "test2" ;   // @error




int v1 = 09; // @err: bad octal number
int v2 = 0x; // @err: invalid hex number

float v3 = 10.1e; // @err: number expected after exponent
float v4 = 10e1;  // OK
float v5 = 0xAB.Cp;   // @err: bad floating point constant
float v6 = 0xAB.C;    // @err: bad floating point constant
float v7 = 0x.;       // @err: bad floating point constant 
float v8 = 0xAB.Cp3;  // OK

void func1 ( int a, .. ); // @err: ".." is invalid

@ // @err: bad character


// @err: Unterminated character
int v9 = 'a
; 

int v10 = '\g'; // @err: bad escape character
int v11 = L'\u123'; // @err: bad universal character

// @err: unterminated character constant
int v12 = '







