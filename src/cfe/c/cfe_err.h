#ifndef CFE_ERR_H
#define CFE_ERR_H

void FatalError ( const char * msg, ... );

#define TST  ((void)0) /* Flag that a particular error message is covered by a test */

void CompilerError ( const char * msg, ... );
void CompilerStrictError ( const char * msg, ... );
void CompilerWarning ( const char * msg, ... );
void CompilerInfoLoc ( const YYLTYPE * loc, const char * msg, ... );
void CompilerErrorLoc ( const YYLTYPE * loc, const char * msg, ... );
void CompilerStrictErrorLoc ( const YYLTYPE * loc, const char * msg, ... );
void CompilerWarningLoc ( const YYLTYPE * loc, const char * msg, ... );
void AstError ( struct TAst *, const char * msg, ... );
void AstWarning ( struct TAst *, const char * msg, ... );

void ShowPrevDeclaration ( const TSymbol * sym );

#define ScannerError   CompilerError
#define ScannerWarning CompilerWarning

#endif // CFE_ERR_H