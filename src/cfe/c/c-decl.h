#ifndef C_DECL_H
#define C_DECL_H

TSymbol * Declare ( 
    const TDeclSpecs  * dspecs, 
    TSymbol           * sym, 
    const TAst        * bitfield,
    bool              bInit
  );

const TTypeNode * DeclareTag ( 
    int    token,    // STRUCT/UNION/ENUM
    STRING name,
    bool   bDefining // the struct is being defined
  );

void DeclareEnumerator ( 
    TEnumDesc   * edesc,
    STRING      name,
    const TAst  * value
  );

const TTypeNode * DefineEnum ( TEnumDesc * edesc );
void DeclareParamIdent ( STRING name );

void DeclareEllipsis ( void );

void DeclareFunctionDefinition ( 
    const TDeclSpecs * dspecs, 
    const TQualNode  * dspecsType,
    TSymbol          * sym,
    TScope           * identDeclList
  );

#endif // C_DECL_H