#ifndef IDTAB_H
#define IDTAB_H

typedef struct TIdentNode TIdentNode;
typedef struct TIdentTab        TIdentTab;

TIdentTab * New_IdentTab ( void );
void IdentTab_Done ( TIdentTab * tab );

void     IdentTab_Insert ( TIdentTab * tab, TIdent * id, TIdentNode ** ppNode );
TIdent * IdentTab_Find ( TIdentTab * tab, STRING strName, TIdentNode ** ppNode );

void     IdentTab_Remove ( TIdentTab * tab, TIdentNode * node );

bool     IdentTab_ForEach ( TIdentTab * tab, void * ctx, bool (*func)(void * ctx, TIdent * id) );

#endif // IDTAB_H