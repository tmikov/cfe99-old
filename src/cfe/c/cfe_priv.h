#ifndef CFE_PRIV_H
#define CFE_PRIV_H

#define TODO_SAFE(x)      ((void)0)
#define TODO_BAD(x)       ASSERT(0)

#if 1
#define TODO          ((void)0)
#define TODO1(x)      ((void)0)
#else
#define TODO          ASSERT(0)
#define TODO1(x)      ASSERT(0)
#endif

#include "common\h\common.h"  // Common headers

// yacc location type
//

typedef struct TLocType
{
  STRING fileName;
  unsigned short line, col;    
} TLocType;

#define YYLTYPE TLocType

#define TLocType_GcMark( loc )  GC_MARK( (loc)->fileName )
#define YYLTYPE_GcMark    TLocType_GcMark

// The entire C front-end is intertwingled. It isn't really possible to 
// separate it into a hierarchical structure with clear dependancies, because
// the data structures all reference each other.
// I have separated the logically independant part sinto separate files (.h+.c),
// but in reality they all depend on each other. This should all have been one big 
// .h file, but it would to be too difficult to navigate.

// Forward type declarations
//
typedef struct TIdent           TIdent;
typedef struct TSymbol          TSymbol;
typedef struct TAst             TAst;
typedef struct TDeclSpecs       TDeclSpecs;

#include "target\h\mach.h"    // front-end-neutral target-specific defintions
#include "target\h\cfetarg.h" // C front end target-specific definitions

#include "cfe_err.h"
#include "idtab.h"
#include "c-types.h"

// scan.c
//
extern YYLTYPE yylloc;
extern char * yytext;

void Scanner_SetLine ( int line, STRING filename );
void Scanner_Init ( void );
void ScannerPrepr ( CORD cord );

STRING StringFromCord ( CORD cord );

// cfe_opts.c
//
extern bool bCharIsSigned;
extern bool bCharGoesLeft;
    // Whether subsequent characters in a character constant are shifted
    // left or right

extern bool bStrictC99; 

// cfe.c
//
#define Target_UChar_Max    IntegerTypesDesc[it_UChar].maxValue
#define Target_WChar_Max    IntegerTypesDesc[it_WChar].maxValue
#define Target_UIntMax_Max  IntegerTypesDesc[it_UIntMax].maxValue

// This structure contains all global data shared between the
// front end modules. This allows to easiy identify global data
// dependencies and easies the task of marking the CFE data roots
// for garbage collection.
//
extern struct TCfeRoots
{
  const TQualNode * IntegerTypes[it_Last];
  const TQualNode * FloatingTypes[TYPE__BT_LAST][rt_Last];  // real,imaginary,complex * float,double,ldouble
  const TQualNode * VoidType;
  const TQualNode * ErrorType;
  const TQualNode * EllipsisType;
  const TQualNode * CharType;       // signed or unsigned character type
  const TQualNode * ConstCharType;  // const char
  const TQualNode * ConstWCharType; // const wchar_t

  STRING      ErrorName;     // error variables have this name
  STRING      EllipsisName;

  TScope * CurScope;
  TScope * GlobalScope;
} CfeRoots;

#define SChar_Type       IntegerTypes[it_SChar]
#define UChar_Type       IntegerTypes[it_UChar]
#define WChar_Type       IntegerTypes[it_WChar]
#define Int_Type         IntegerTypes[it_Int]
#define UInt_Type        IntegerTypes[it_UInt]

#define RealTypes        FloatingTypes[TYPE_REAL]

#define Float_Type       RealTypes[rt_Float]
#define Double_Type      RealTypes[rt_Double]
#define LongDouble_Type  RealTypes[rt_LongDouble]

void Cfe_GcMarkRoots ( void );

extern struct TCfeGlob
{
  // Dump support
  //
  FILE * DumpFile;
  unsigned DumpIndent;
  unsigned DumpIndentStep;
} CfeGlob;

#include "c-ast.h"
#include "c-symtab.h"
#include "c-dspecs.h"
#include "c.tab.h"	          // yacc value type and token codes
#include "c-decl.h"
#include "c-expr.h"
#include "c-stmt.h"
#include "c-dump.h"


#endif // CFE_PRIV_H