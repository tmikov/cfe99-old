#ifndef COMPERR_H
#define COMPERR_H

void FatalError ( const char * msg, ... );

void CompilerError (  const char * msg, ... );
void CompilerWarning (  const char * msg, ... );

#define ScannerError   CompilerError
#define ScannerWarning CompilerWarning

#endif // COMPERR_H