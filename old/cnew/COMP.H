#ifndef COMP_H
#define COMP_H

#include "misc\xheap.h"
#include "symtab.h"

enum ERROR_SEVERITY
{
  ERR_FATAL,       // fatal error
  ERR_ERROR,       // error
  ERR_INFO,        // info
  ERR_WARN0,       // warning level 0
};

struct TCompileOptions
{
  int maxErrors;   // max number of errors before compilation is aborted
  int maxWarnings; // max number of warnings before compilation is aborted

  bool bSignedChars; // default chars are signed
  bool bANSI;        // attempt to achieve max. ANSI compliance
  bool bC9X;         // attempt to achieve max. C9X compliance
};

extern struct TCompileOptions * opt;
extern TArena GlobalArena;

void PrintErrorStr ( int severity, const char * msg, ... );
void Error ( const char * fmt, ... );
void Warning ( const char * fmt, ... );
void Fatal ( const char * fmt, ... );
void OutOfMemory ( void );

void Compile ( void );

#endif // COMP_H
