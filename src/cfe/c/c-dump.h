#ifndef C_DUMP_H
#define C_DUMP_H

void Dump_Init ( void );
void Dump_TQualNode ( const TQualNode * tag );
void Dump_TScope ( const TScope * tag );
void DumpAst ( TAst * ast );

#endif // C_DUMP_H
