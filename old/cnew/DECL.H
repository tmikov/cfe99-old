
TSymbol * Decl (
    TStorageClass * sClass,
    TSymbol * pSymbol,
    TYPE * typ
  );

TStructDef * DeclStruct (
    TYPE_NODE structType,
    TStructDef  * structDef,
    bool bCompleteDecl
  );

TStructDef * CompleteStructDecl (
    TStructDef * structDef,
    TScope     * structScope
  );

void DeclareEnumMember (
    TSymbol * pSym,
    TYPE * typ,
    TExprAst * pInit
  );
