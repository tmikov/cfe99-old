#ifndef SCAN_X_H
#define SCAN_X_H

// Defines extended operator codes used by the scanner
//

#include "comp.h"
#include "ParseTyp.h"

#define ScannerError Error
#define SCANNER_RET_E( e_code, lex_code )  yylval.ecode = (e_code); return lex_code

#endif // SCAN_X_H
