#include "strings.h"
#include "c-types.h"
#include "csyms.h"
#include "scope.h"
#include "actsym.h"
#include "ast.h"
#include "misc\arena.h"

extern TArena GlobalArena;

extern TScope * MainScope;
extern TSymbol * EllipsisSym;
extern TStorageClass StaticEnumSClass;

#define EnumSClass &StaticEnumSClass

void SymTab_Init ( void );
void SymTab_Done ( void );



