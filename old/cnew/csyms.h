#ifndef CSYMS_H
#define CSYMS_H

#include "misc\clist.h" // generic list manipulation
#include "misc\fixalloc.h"
#include "c-types.h"    // C types description
#include "tconf.h"      // target type configuration
#include "strings.h"    // string table


// STORAGE_CLASS and OUTPUT_CLASS together describe a C storage
// class.
//

typedef enum STORAGE_CLASS STORAGE_CLASS;
enum STORAGE_CLASS
{
  SC_NONE = 0,
  SC_AUTO,
  SC_REGISTER,
  SC_FIXED,        // it is at a fixed address
  SC_TYPEDEF,
  SC_CONSTANT,     // this is a constant value, not stored in memory
};
#define STORAGE_CLASS_BITS 4  // Num bits necessary to hold a value

#define IS_STORAGE_CLASS_LVALUE( sc )  ((sc) >= SC_AUTO && (sc) <= SC_FIXED)

typedef enum OUTPUT_CLASS OUTPUT_CLASS;
enum OUTPUT_CLASS
{
  OC_NONE = 0,
  OC_PUBLIC,
  OC_PRIVATE,
  OC_EXTERN,
};
#define OUTPUT_CLASS_BITS 3  // Num bits necessary to hold a value

// This is the width of the declspec field, and so
// the maxmimum number of declspec-s that are supported
//
#define DECLSPEC_BITS    16

// This describes a storage class. It applies to a symbol
//
typedef struct TStorageClass TStorageClass;
struct TStorageClass
{
  STORAGE_CLASS storClass : STORAGE_CLASS_BITS;
  OUTPUT_CLASS  outClass  : OUTPUT_CLASS_BITS;
  unsigned      declSpec  : DECLSPEC_BITS;     // Each declspec is a bit
};


typedef struct TIdent TIdent;
struct TIdent
{
  TListEntry     nameLink;      // link in the list of symbols with same name
  STRING         strName;       // the name itself
  TIdent         * nextLocal;   // next symbol in the same scope
  struct TScope  * parentScope; // the scope itself
};

#define IS_IDENT_HASHED( pIdent )  ((pIdent)->nameLink.Flink != NULL)

// This describes a struct/union/enum
//
typedef struct TStructDef TStructDef;
struct TStructDef
{
  TIdent    id;
  TYPE_NODE structType : TYPE_NODE_BITS; // Defines what this is: struct,union or enum
  unsigned  isComplete : 1;   // true if the structure is fully defined

  // true if the structure/union has at least one const member, or
  // if a member is a class itself, its _hasConstMembers_ flag is set.
  // In that case the class can't be assigned to.
  //
  unsigned        hasConstMembers : 1;

  union
  {
    struct TScope  * structScope; // This is the scope that holds all struct's members.
    TARGET_INT       enumCount;   // the largest value of enum item (so far)
  } info;
};

// The basic structure describing a symbol
//
typedef struct TSymbol TSymbol;
struct TSymbol
{
  TIdent          id;
  TQualNode     * type;    // the type of the symbol
  TStorageClass * storage; // storage class and declspec-s

  union
  {
    struct TExprAst  * init; // init expression
    struct TAst      * code; // code
  } data;
};

#ifndef __DEBUG
#  define ID_GET_SYMBOL( id )       ((TSymbol *)(id))
#  define ID_GET_STRUCT( id )       ((TStructDef *)(id))
#else

TSymbol * IdGetSymbol ( TIdent * id );
TStructDef * IdGetStruct ( TIdent * id );

#  define ID_GET_SYMBOL( id ) IdGetSymbol( id )
#  define ID_GET_STRUCT( id ) IdGetStruct( id )      
#endif

TStorageClass * StorageClass_New ( void );
void            StorageClass_Free ( TStorageClass * );

#define CombineDeclSpec( a, b )  ((a) | (b))

const char * Ident_Name ( const TIdent * id );

#define Symbol_Name( sym )    Ident_Name( &(sym)->id )
#define Struct_Name( sym )    Ident_Name( &(sym)->id )

TSymbol * Symbol_New ( STRING strName );
void      Symbol_Free ( TSymbol * );

TStructDef * Struct_New ( STRING strName );
void         Struct_Free ( TStructDef * );


#endif // CSYMS_H
