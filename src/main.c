#include "precomp.h"
#include "common\h\common.h"
#include "target\h\mach.h"
#include "cfe\h\cfe.h"

int main ( int argc, char ** argv )
{
  if (!GcInit())
    abort();

  Strings_Init();
  Mach_Init();
  Cfe_Init();

  yydebug = argc > 1;
  yyparse();

  GcDone();

  return EXIT_SUCCESS;
};
