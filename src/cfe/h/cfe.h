#ifndef CFE_H
#define CFE_H

void Cfe_Init ( void );

#if YYDEBUG
extern int yydebug;
#endif
int yyparse ( void );

#endif // CFE_H