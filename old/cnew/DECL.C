#include "port\global.h"
#include "symtab.h"
#include "ast.h"
#include "decl.h"
#include "comp.h"

//--------------------------------------------------------------------------
// Name         NonStructScope
//
// Find the first non-struct parent scope.
//
// Note: For C++ this is the current scope.
//--------------------------------------------------------------------------
TScope * NonStructScope ( void )
{
  TScope * pScope;

  pScope = CurScope;
  while (!LOCAL_STRUCTS( pScope->scopeType ))
    pScope = pScope->parentScope;

  return pScope;
};

//--------------------------------------------------------------------------
// Name         ValidateDeclType
//
// Returns true if typ is a valid type.
// Checks for errors like arrays of void, functions returning arrays, etc.
//--------------------------------------------------------------------------
static bool ValidateDeclType ( const TYPE * typ )
{
  TTypeNode * tnode;

  ASSERT( typ );
  do
  {
    tnode = typ->tnode;

    // array's type can only be: SCALAR, CLASS or ARRAY[with known size]
    //
    if (tnode->node == NODE_ARRAY)
    {
      ASSERT( tnode->of );

      typ = tnode->of;
      tnode = typ->tnode;

      if (!tnode->isScalar && !tnode->isClass &&
          !(tnode->node == NODE_ARRAY && TGT_INT_NE_0(tnode->info.arrayLen) ))
      {
        Error( "Invalid array type" );
        return false;
      }
    }
    else
    // - functions can only return VOID, SCALAR or CLASS.
    //
    if (tnode->node == NODE_FUNCTION)
    {
      ASSERT( tnode->of );

      typ = tnode->of;
      tnode = typ->tnode;

      if (!tnode->isScalar && !tnode->isClass && !IS_VOID(tnode))
      {
        Error( "Invalid function return type" );
        return false;
      }
    }
    else
      typ = tnode->of;  // move on to the next item in the chain
  }
  while (typ != NULL);

  return true;
};

//--------------------------------------------------------------------------
// Name         Decl
//
// Declares a symbol in the current scope.
// 
// pSymbol is "fresh"-allocated. It is not inserted anywhere, yet.
//
// The initializator is still not applied.
// The bitfield attribute is not set, yet.
//
// Returns a pointer to the symbol. Note: this may be a different symbol than
// the one that was passed, in case this is a redeclaration.
// For now in case of error returns NULL. Later might return something
// different.
//--------------------------------------------------------------------------
TSymbol * Decl (
    TStorageClass * sClass,
    TSymbol * pSymbol,
    TYPE * typ
  )
{
  TSymbol * origSymbol;      // points to the symbol with the same name
  TTypeNode * tnode;         // quick access to the type-node of the symbol
  bool        bIsDefinition;

  tnode = typ->tnode;

  // See if there is another symbol with the same name in the
  // current scope
  //
  origSymbol = FindScopeSymbol( pSymbol->id.strName, CurScope );

  // Change the type of function parameters: arrays and functions
  // must be converted to pointers.
  //
  if (CurScope->scopeType == FUNC_PARAM_SCOPE)
  {
    if (tnode->node == NODE_ARRAY)
    {
      typ = ArrayToPointer( typ );
      tnode = typ->tnode;
    }
    else
    if (tnode->node == NODE_FUNCTION)
    {
      typ = FuncToPointer( typ );
      tnode = typ->tnode;
    }
  }


  // Perform basic validations over the type
  //
  if (!ValidateDeclType( typ ))
    goto errorExit;

  // - functions can only be SC_FIXED or SC_TYPEDEF
  //
  if (tnode->node == NODE_FUNCTION &&
      sClass->storClass != SC_FIXED && sClass->storClass != SC_TYPEDEF)
  {
    Error( "Invalid storage class for function" );
    goto errorExit;
  }

  switch (CurScope->scopeType)
  {
// =========================================== this is a scope inside a struct
    case STRUCT_SCOPE:
      // - No storage specifiers are allowed
      //
      if (sClass->storClass || sClass->outClass)
      {
        Error( "No storage class is allowed for struct members" );
        goto errorExit;
      }
      // - Functions are not allowed
      //
      if (tnode->node == NODE_FUNCTION)
      {
        Error( "Functions are not allowed" );
        goto errorExit;
      }
      break;

// ================================================== this is the global scope
    case FILE_SCOPE:
      // - only SC_TYPEDEF and SC_FIXED is allowed
      //
      if (sClass->storClass != SC_TYPEDEF  && sClass->storClass != SC_FIXED)
      {
        Error( "Invalid storage class for file scope" );
        goto errorExit;
      }
      break;

// ========================================= this is a scope inside a function
    case FUNCTION_SCOPE:
      // - static functions are not allowed
      //
      if (tnode->node == NODE_FUNCTION && sClass->outClass == OC_PRIVATE)
      {
        Error( "Local declaration of static functions is illegal" );
        goto errorExit;
      }
      break;

// ====================== this is a scope iside function parameter declaration
    case FUNC_PARAM_SCOPE:
      // - only SC_AUTO and SC_REGISTER is allowed
      //
      if (sClass->storClass != SC_AUTO && sClass->storClass != SC_REGISTER)
      {
        Error( "Invalid storage class for function parameter" );
        goto errorExit;
      }
      break;
  }

  // Check for duplicate declarations in all but global scope
  //
  if (CurScope->scopeType != FILE_SCOPE && origSymbol != NULL)
  {
    Error( "Redeclaration of '%s'", Symbol_Name(pSymbol) );
    goto errorExit;
  }

/*
  Determine whether this is a definition or a declaration

  Any declaration is a definition unless any of these is true:
  - declares a function without function's body (doesn't apply now)
  - contains an extern specifier or ??linkage specification? *and*
  neither an initializer nor a function body (these two do not apply now)!
  - it is a typedef declaration
  - it is a class name declaration: "struct X;" (doesn't apply now)
*/
  bIsDefinition = sClass->outClass != OC_EXTERN &&
                  sClass->storClass != SC_TYPEDEF;

  // A definition can't give an object an _incomplete type_
  //
  if (bIsDefinition && !IsCompleteType( typ ))
  {
    Error( "Invalid declaration (incomplete type)" );
    goto errorExit;
  }

  // handle duplicate declarations in global scope
  //
  if (origSymbol != NULL && CurScope->scopeType == FILE_SCOPE)
  {
    // First make sure the types are the same
    //
    if (!EqualTypeChains( typ, origSymbol->type ))
    {
      Error( "Type mismatch in redeclaration of '%s'", Symbol_Name(pSymbol) );
      goto errorExit;
    }

    //  - the storage classes (enum STORAGE_CLASS) must be the same
    //  - It seems that the ouptut classes must be either the same, or
    //  at least one of them must be none. (None means OC_PUBLIC).    
    //
    if (sClass->storClass != origSymbol->storage->storClass ||
        sClass->outClass != origSymbol->storage->outClass &&
        sClass->outClass != OC_PUBLIC &&
        origSymbol->storage->outClass != OC_PUBLIC)
    {
      Error( "Invalid redeclaration of '%s': different storage class", Symbol_Name(pSymbol) );
      goto errorExit;
    }

    // Combine the output class: Any class overrides OC_PUBLIC
    //
    if (origSymbol->storage->outClass == OC_PUBLIC)
      origSymbol->storage->outClass = sClass->outClass;

    // Combine declspec-s
    // TODO: check for conflicts
    //
    origSymbol->storage->declSpec =
      CombineDeclSpec( origSymbol->storage->declSpec, sClass->declSpec );

    // In case of explicit function declaration overriding an implicit one, we must
    // also override the type.
    //
    if (typ->tnode->node == NODE_FUNCTION && !typ->tnode->isImplicit &&
        origSymbol->type->tnode->isImplicit)
    {
      origSymbol->type->tnode = typ->tnode;
    }

    StorageClass_Free( sClass );
    Symbol_Free( pSymbol );

    return origSymbol;
  }
  else
    ASSERT( origSymbol == NULL );

  PRINT_TYPE( "", typ, "\n" );

  // Initialize the symbol and add it to the scope and the hash table
  //
  pSymbol->storage = sClass;
  pSymbol->type = typ;
  Scope_AddSymbol( CurScope, pSymbol );
  ActivateSymbol( pSymbol );
  return pSymbol;

errorExit:
  StorageClass_Free( sClass );
  Symbol_Free( pSymbol );
  // we could free the type chain as well, but it is risky and not
  // necessary. If it points to a typedef, we must be careful not
  // to free the typedef, etc.

  return NULL;
};

//--------------------------------------------------------------------------
// Name         DeclStruct
//
//
//--------------------------------------------------------------------------
TStructDef * DeclStruct (
    TYPE_NODE structType,
    TStructDef     * structDef,
    bool bCompleteDecl
  )
{
  TScope * pScope;

  // find the potential parent scope for this struct.
  //
  if (bCompleteDecl)
  {
    // If it is a completed declaration, the potential scope is
    // the first suitable (non-struct) scope.
    //
    pScope = NonStructScope();
  }
  else
  {
    // If this is a non-complete declaration, the potential scope
    // is either the main scope (fwd struct declaration) or
    // the scope of the original struct.
    //
    if (structDef->structType != 0)  // is there an original struct?
      pScope = structDef->id.parentScope;
    else
      pScope = MainScope;
  }

  ASSERT( pScope );

  // Check if this is a redeclaration. If structDef is "fresh-allocated",
  // its structType will be 0.
  //
  if (structDef->structType != 0)
  {
    // Check if the "original" struct was declared in the same scope
    //
    if (structDef->id.parentScope == pScope) // this is a redeclaration
    {
      // is it the same structType ?
      //
      if (structDef->structType != structType)
      {
        Error( "Invalid redeclaration of '%s %s'",
               NodeName[structDef->structType],
               Struct_Name( structDef )
             );

        // allocate a new anonymous StructDef for this redeclaration
        //
        structDef = Struct_New( NULL );
      }

      // Are we trying to complete the same struct twice ?
      //
      if (bCompleteDecl && structDef->isComplete)
      {
        Error( "Redeclaration of '%s %s'",
               NodeName[structDef->structType],
               Struct_Name( structDef )
             );

        // allocate a new anonymous StructDef for this redeclaration
        //
        structDef = Struct_New( NULL );
      }
      else
        return structDef; 
    }
    else // this is not a re-declaration, so we must allocate a new structDef
      structDef = Struct_New( structDef->id.strName );
  }

  // Add the struct to the scope it belngs to and to the hash table
  // (note: anon structs will not be added to the table: HashAdd() ignores
  // them.
  //
  Scope_AddStruct( pScope, structDef );
  ActivateStruct( structDef );
  structDef->structType = structType;

  return structDef;
};

//--------------------------------------------------------------------------
// Name         CompeteStructDecl
//
//
//--------------------------------------------------------------------------
TStructDef * CompleteStructDecl (
    TStructDef * structDef,
    TScope     * structScope
  )
{
  TSymbol * sym;

  structDef->isComplete = true;

  // check if has at least one const member
  //
  for ( sym = ID_GET_SYMBOL( structScope->locSymbols.first );
        sym != NULL;
        sym = ID_GET_SYMBOL( sym->id.nextLocal ) )
  {
    TYPE * qnode = sym->type;

    // check for const attr or a struct with const members
    //
    if (qnode->isConst ||  
        qnode->tnode->isClass && qnode->tnode->info.structDef->hasConstMembers)
    {
      structDef->hasConstMembers = true;
      break;
    }
  }

  structDef->info.structScope = structScope;

#if YYDEBUG
  printf( "%s %s ", NodeName[structDef->structType], Struct_Name( structDef ) );
  printf( "{\n" );
  for ( sym = ID_GET_SYMBOL(structScope->locSymbols.first);
        sym != NULL;
        sym = ID_GET_SYMBOL(sym->id.nextLocal) )
  {
    printf( "  %s:", Symbol_Name(sym) );
    PRINT_TYPE( "  ", sym->type, "\n"  );
  };
  printf( "}\n" );
#endif

  return structDef;
};

//--------------------------------------------------------------------------
// Name         DeclareEnumMember
//
// Called by the parser to declare a new enum member.
// Checks for redeclaration in the apropriate scope.
//--------------------------------------------------------------------------
void DeclareEnumMember (
    TSymbol * pSymbol,       // The fresh-allocated ident
    TYPE * typ,           // type pointing to the enumDef
    TExprAst * pInit    // initialization value
  )
{
  TScope * pScope;

  // Find the scope where the enum member is to be declared
  pScope = NonStructScope();

  // Check for redeclarations
  if (FindScopeSymbol( pSymbol->id.strName, pScope ))
  {
    Error( "Multiple declaration of '%s'", Symbol_Name(pSymbol) );
    goto errorExit;
  }                        

  // Process the value of the ident
  //
  if (pInit != NULL)
  {
    // TODO: support this
    Error( "Initialized enum members not suported" );
    pInit = NULL;
  }
  else
  {
    // TODO: check for overflow?
    // TODO: generate a value in pInit
    TGT_INT_ADD( typ->tnode->info.structDef->info.enumCount, 
                 typ->tnode->info.structDef->info.enumCount,
                 Int1 );
  }

  // Initialize the symbol and add it to the scope and the hash table
  //
  Scope_AddSymbol( pScope, pSymbol );
  ActivateSymbol( pSymbol );
  pSymbol->storage = EnumSClass;
  pSymbol->type = typ;
  pSymbol->data.init = pInit;
  return;

errorExit:
  Symbol_Free( pSymbol );
};

