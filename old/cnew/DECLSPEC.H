#ifndef DECLSPEC_H
#define DECLSPEC_H

#include "strings.h"

void DeclSpec_Init ( void );
void DeclSpec_Done ( void );

unsigned FindDeclSpec ( STRING strName ); // 0 on error
unsigned FindAttr ( STRING strName ); // 0 on error


#endif // DECLSPEC_H
