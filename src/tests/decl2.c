// Declaration specifiers

__int(8) i8; // OK
__int(999999) ix; // @err: too large int width
__int(100) i100; // @err: invalid int width


__float(32) f32; // OK
__float(999999) fx; // @err: too large float width
__float(100) f100; // @err: invalid float width


int v1; // OK
int int v2;        // @err: more than one type in declaration
int char v3;       // @err: more than one type in declaration
int const int v4;  // @err: more than one type in declaration


_Complex v5;                              // @err: what ?
_Complex _Complex v6;                     // @err: complex/imaginary conflict
_Complex float _Complex v7;               // @err: complex/imaginary conflict
_Complex float _Complex double v8;        // @err: complex/imaginary conflict @err: more than one type in declaration
_Imaginary v9;                            // @err: what ?
_Imaginary _Imaginary v10;                // @err: what ?
_Imaginary float _Imaginary v11;          // @err: complex/imaginary conflict
double _Imaginary float _Imaginary v12;   // @err: complex/imaginary conflict @err: more than one type in declaration

_Imaginary _Complex v13;                  // @err: what ?
_Imaginary float _Complex v14;            // @err: complex/imaginary conflict
_Imaginary float _Complex double v15;     // @err: complex/imaginary conflict @err: more than one type in declaration
long _Imaginary float _Complex double v16;  // @err: complex/imaginary conflict @err: more than one type in declaration

signed signed v19;      // @err: signed/unsigned conflict
unsigned unsigned v20;  // @err: signed/unsigned conflict
signed unsigned v20;    // @err: signed/unsigned conflict
unsigned signed v21;    // @err: signed/unsigned conflict
signed int v22;         
unsigned int v23;
unsigned v24;
signed v25;

short v26;
long v27;
short int v28;
long int v29;
short short v30;      // @err: short/long conflict
long long v31;        // OK
short long v32;       // @err: short/long conflict
short long long v33;  // @err: short/long conflict
long short long v34;  // @err: ???



const int v35;
const const int v36;    // @err: more than one const
const int const v37;    // @err: more than one const

volatile int v42;
volatile volatile int v43;    // @err: more than one volatile
volatile int volatile v44;    // @err: more than one volatile

const volatile v49;           // @warn: no specifier
const volatile const int v53; // @err: more than one const
volatile const volatile int v54;      // @err: more than one volatile


static static int v55;     // @err: More than one storage class
static int extern int v56; // @err: More than one storage class

static v57; // @warn: no specifier

_Imaginary long v17; // @err: _Complex/_Imaginary only apply to fp
_Complex long v18;   // @err: _Complex/_Imaginary only apply to fp

signed float v57a;   // @err: signed/unsigned apply only to int/char
unsigned double v58; // @err: signed/unsigned apply only to int/char

short int v59; 
short char v60;   // @err: short applies only to int
short double v61; // @err: short applies only to int

long int v62; 
long char v63;   // @err: long applies only to int/double
long double v64; // OK

long long v65;
long long char v66; // @err: long long applies only to int