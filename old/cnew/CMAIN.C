#include "port\global.h"
#include "comp.h"

static struct TCompileOptions CompOptions =
{
  25,         // maxErrors
  100,        // maxWarnings

  false,      // bSignedChars
  false,      // bANSI
  true        // bC9X
};

int main ( int argc, char ** argv )
{
#if YYDEBUG
  extern int yydebug;
  yydebug = argc == 2;
#endif 
  opt = &CompOptions;

  Compile();

#ifdef _DEBUG
  _dbg_print_heap( "Heap at end:\n" );
#endif
  return 0;
};
