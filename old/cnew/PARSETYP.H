/*
  parsetyp.h: definitions used only while parsing the types and
  declarations.
*/
#ifndef PARSETYP_H
#define PARSETYP_H

#include "scantok.h"

typedef struct TParseType TParseType;
typedef struct TParseType
{
  // this is true, if the TParseType structure was not allocated
  // in the heap, and so it must not be freed or modified.
  unsigned         staticData : 1; 

  // TYPESPEC
  BASIC_TYPE       basic : BASIC_TYPE_BITS;
  unsigned         isSigned    : 1;
  unsigned         isUnsigned  : 1;
  unsigned         isShort     : 1;
  unsigned         isLong      : 1;

  //TYPEQUAL
  unsigned isConst    : 1;
  unsigned isVolatile : 1;
  unsigned isRestrict : 1;

  //SCLASS
  INFO_TOKEN sclass;

  //DECLSPEC
  unsigned       declspec    : DECLSPEC_BITS;

  // TYPENAME
  TSymbol * typename;

  // struct_spec
  TStructDef * structDef;
};

typedef struct TDeclData TDeclData;
struct TDeclData
{
  TSymbol * ident;    // the identifier being declared
  TQualNode * type;   // beginning of the type chain being described
  TTypeNode * etype;  // end of the type chain

  unsigned  attrs;  // attributes that must be applied to the next
                    // node in the type chain. (It will be processed
                    // later because declarations are parsed right to left).
                    // if there is no next node while parsing the declaration,
                    // the attributes are naturally assigned to the type specfier
};

DECLARE_FIXED_ALLOCATOR( ParseTypeHeap, TParseType );
DECLARE_FIXED_ALLOCATOR( DeclDataHeap,  TDeclData );

extern const char * TokName[];

void ParseType_Init ( void );
void ParseType_Done ( void );

// Functions that allocate different TParseType structures
//
TParseType * NewTokenParseType (
    INFO_TOKEN itok
  );
TParseType * NewTypenameParseType (
    TSymbol * typename
  );
TParseType * NewDeclspecParseType (
    unsigned declspec
  );
TParseType * NewStructParseType (
    TStructDef * structDef
  );


// if b is NULL, just returns a (and vica versa)
TParseType * CombineParseType (
    TParseType * a,
    TParseType * b
  );

void FreeParseType (
    TParseType * t
  );

// build a normal type from a parsetype
TYPE * ParseType (
    const TParseType * t
  );

TDeclData * NewDeclData ( void );

void FreeDeclData (
    TDeclData * p
  );

void CombineQualToken (
    TQualNode       * qnode,
    INFO_TOKEN itok
  );

TDeclData * AttachToDecl (
    TDeclData * decl,
    TYPE      * typ
  );

void ParsedDecl (
    const TParseType * typeStart,
    TDeclData  * decl
  );

void CombineQuals ( TQualNode * dest, const TQualNode * src );

TDeclData * DeclAddAttribute ( unsigned attr, TDeclData * decl );
TDeclData * DeclAddPointer   ( TQualNode * qual, TDeclData * decl );
TDeclData * DeclAddArray     ( TDeclData * decl, TExprAst * nelem );
TDeclData * DeclAddFunction  ( TDeclData * decl );
TDeclData * DeclAddIdent     ( STRING str );

#endif PARSETYP_H
