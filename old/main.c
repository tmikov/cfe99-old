#include "precomp.h"
#include "strings.h"
#include "tconf.h"
#include "scan.h"

extern void Prepr_Done ( void );

extern int yyparse ( void );
#if YYDEBUG
extern int yydebug;
#endif

void main ( int argc, char ** argv )
{
#if YYDEBUG
  yydebug = argc == 2;
#endif 

  Strings_Init();
  TConf_Init();
  Scanner_Init();

  yyparse();

  Prepr_Done();
  Strings_Done();
};
