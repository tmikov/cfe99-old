/*
  c-dump.c

  Code for dumping front-end structures into XML format.
*/
#include "precomp.h"
#include "cfe_priv.h"
#include "c-dump.h"

#define DUMP_FILE   CfeGlob.DumpFile
#define IND         Indent()
#define NL          fprintf( DUMP_FILE, "\n" )
#define INCIND      CfeGlob.DumpIndent += CfeGlob.DumpIndentStep
#define DECIND      CfeGlob.DumpIndent -= CfeGlob.DumpIndentStep

// Verbose commands
//
#define TAG( tag )      fprintf( DUMP_FILE, "<" #tag ">\n" ), INCIND;
#define END_TAG( tag )  DECIND, IND, fprintf( DUMP_FILE, "</" #tag ">\n" )

#define D_ENUM( val, ename ) IND, fprintf( DUMP_FILE, "<" #val " v=%s/>\n", ename[tag->val] )
#define D_BOOL( val )        if (tag->val) IND, fprintf( DUMP_FILE, "<" #val "/>\n" )
#define D_LONG( val )        IND, fprintf( DUMP_FILE, "<" #val " v=%ld/>\n", (long)tag->val )
#define D_XLONG( val )       IND, fprintf( DUMP_FILE, "<" #val " v=0x%08lX/>\n", (long)tag->val )

#define SD_BOOL( val )        if (tag->val) fprintf( DUMP_FILE, " " #val )
#define SD_ENUM( val, ename ) fprintf( DUMP_FILE, " %s", ename[tag->val] )
#define SD_LONG( val )        fprintf( DUMP_FILE, " %ld", (long)tag->val )
#define SD_XLONG( val )       fprintf( DUMP_FILE, " 0x%08lX", (long)tag->val )

static bool bVerbType = false;

//--------------------------------------------------------------------------
// Name         Indent 
//
// 
//--------------------------------------------------------------------------
static void Indent ( void )
{
  fprintf( DUMP_FILE, "%*s", CfeGlob.DumpIndent, "" );
};

//--------------------------------------------------------------------------
// Name         Dump_TTypeNode 
//
// 
//--------------------------------------------------------------------------
static void Dump_TTypeNode ( const TTypeNode * tag, bool bDumpStruct )
{
  ASSERT( tag != NULL );

  SD_ENUM( typeKind, TypeKind_Name );

  if (tag->id.strName != NULL)
    fprintf( DUMP_FILE, " %s", Ident_Name( &tag->id ) );
  else
    fprintf( DUMP_FILE, " 0x%08x", (uint32_t)tag );

  SD_BOOL( staticArraySize );
  SD_BOOL( bDefined );

    switch (tag->typeKind)
    {
    case TYPE_BOOL:
    case TYPE_INTEGER:
    case TYPE_ENUM:
      fprintf( DUMP_FILE, " %s", tag->u.pIntDesc->name );
      break;

    case TYPE_REAL: 
    case TYPE_IMAGINARY:
    case TYPE_COMPLEX:
      fprintf( DUMP_FILE, " %s", RealType_Name[tag->u.realType] );
      break;
    }

  if (tag->of != NULL)
    Dump_TQualNode( tag->of );
}

//--------------------------------------------------------------------------
// Name         Dump_TQualNode 
//
// 
//--------------------------------------------------------------------------
void Dump_TQualNode ( const TQualNode * tag )
{
  SD_BOOL( isConst );
  SD_BOOL( isVolatile );
  SD_BOOL( isRestrict );

  if (tag->extQualAttr != 0)
    SD_XLONG( extQualAttr );

  Dump_TTypeNode( tag->tnode, false );
}

//--------------------------------------------------------------------------
// Name         Dump_TSymbol 
//
// 
//--------------------------------------------------------------------------
static void Dump_TSymbol ( const TSymbol * tag )
{
  ASSERT( tag != NULL );
  fprintf( DUMP_FILE, "<TSymbol name=%s>\n", Symbol_Name( tag ) );
  INCIND;
  {
    if (tag->identNode != NULL)
      IND, fprintf( DUMP_FILE, "<identNode/>\n" );

    IND, fprintf( DUMP_FILE, "<type>" );
    Dump_TQualNode( tag->type );
    fprintf( DUMP_FILE, "</type>\n" );

    if ( tag->defType != NULL)
    {
      IND, fprintf( DUMP_FILE, "<defType>" );
      Dump_TQualNode( tag->defType );
      fprintf( DUMP_FILE, "</defType>\n" );
    }

    if (tag->declaresGlobal != NULL)
      IND, fprintf( DUMP_FILE, "<declaresGlobal name=%s/>\n", Symbol_Name( tag->declaresGlobal ) );

    D_ENUM( storageClass, StorageClass_Name );
    D_ENUM( storageDuration, StorageDuration_Name );
    D_BOOL( bBitfield );
    D_BOOL( bIdentDefined );
    if (tag->bBitfield)
      D_LONG( bitfieldWidth );
  }
  END_TAG( TSymbol );
}

//--------------------------------------------------------------------------
// Name         Dump_TScope 
//
// 
//--------------------------------------------------------------------------
void Dump_TScope ( const TScope * tag )
{
  TAG( scope )
  {
    D_ENUM( disposition, ScopeDisposition_Name );
    D_BOOL( bHaveEllipsis );

    IND,TAG( Symbols )
    {
      const TSymbol * sym;
      for ( sym = tag->pFirstSymbol; sym != NULL; sym = (const TSymbol *)sym->id.pNext )
        IND, Dump_TSymbol( sym );
    }
    END_TAG( Symbols );
  }
  END_TAG( scope );
}

//--------------------------------------------------------------------------
// Name         DumpIntConst 
//
// 
//--------------------------------------------------------------------------
static void DumpIntConst ( TAst * ast )
{
  int i, len;

  len = ast->exprType->tnode->u.pIntDesc->pMachType->precision / CHAR_BIT;

  fprintf( DUMP_FILE, "<imax_t v=\"0x" );

  for ( i = sizeof(TARGET_IMAX_T) - len; i < sizeof(TARGET_IMAX_T); ++i )
    fprintf( DUMP_FILE, "%0*x", (CHAR_BIT+3)/4, ast->u.intConst.b[i] );

  fprintf( DUMP_FILE, "\"/>" );
};


//--------------------------------------------------------------------------
// Name         DumpSymbol 
//
// 
//--------------------------------------------------------------------------
static void DumpSymbol ( const TAst * ast )
{
  fprintf( DUMP_FILE, "<id>%s</id>", Symbol_Name( ast->u.sym ) );
}

//--------------------------------------------------------------------------
// Name         AstDump 
//
// 
//--------------------------------------------------------------------------
static void AstDump ( TAst * ast )
{
  if (ast == NULL)
    return;

  IND,fprintf( DUMP_FILE, "<%s>\n", Ast_Name[ast->code] ); 
  INCIND;
  if (ast->exprType)
  {
    IND, fprintf( DUMP_FILE, "<exprType>" );
    Dump_TQualNode( ast->exprType );
    fprintf( DUMP_FILE, "</exprType>\n" );
  }
  {
    KID_POS kidPos;

    switch (ast->code)
    {
    case ast_intConst:
      IND; DumpIntConst( ast ); NL;
      break;

    case ast_symbol:
      IND; DumpSymbol( ast ); NL;
      break;
    }

    for ( kidPos = FIRST_KID_POS( ast ); !END_KIDS( ast, kidPos ); kidPos = NEXT_KID_POS( ast, kidPos ))
      AstDump( GET_KID( kidPos ) );
  }
  DECIND; IND; fprintf( DUMP_FILE, "</%s>\n", Ast_Name[ast->code] ); 
};


//--------------------------------------------------------------------------
// Name         Dump_Init 
//
// 
//--------------------------------------------------------------------------
void Dump_Init ( void )
{
  CfeGlob.DumpFile = stdout;
  CfeGlob.DumpIndent = 0;
  CfeGlob.DumpIndentStep = 4;
}

//--------------------------------------------------------------------------
// Name         DumpAst 
//
// 
//--------------------------------------------------------------------------
void DumpAst ( TAst * ast )
{
  Dump_Init(); // PATCH
  AstDump( ast );
};
