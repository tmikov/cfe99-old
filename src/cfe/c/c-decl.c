/*
  c-decl.c

  Handle C declarations.
*/
#include "precomp.h"
#include "cfe_priv.h"

/*

6.2.2 Linkages of identifiers
=======================================================

*1 An identifier declared in different scopes or in the same scope more than once can be
made to refer to the same object or function by a process called linkage.21) There are
three kinds of linkage: external, internal, and none.

*2 In the set of translation units and libraries that constitutes an entire program, each
declaration of a particular identifier with external linkage denotes the same object or
function. Within one translation unit, each declaration of an identifier with internal
linkage denotes the same object or function. Each declaration of an identifier with no
linkage denotes a unique entity.

*3 If the declaration of a file scope identifier for an object or a function contains the storageclass
specifier static, the identifier has internal linkage.22)

*4 For an identifier declared with the storage-class specifier extern in a scope in which a
prior declaration of that identifier is visible,23) if the prior declaration specifies internal or
external linkage, the linkage of the identifier at the later declaration is the same as the
linkage specified at the prior declaration. If no prior declaration is visible, or if the prior
declaration specifies no linkage, then the identifier has external linkage.

*5 If the declaration of an identifier for a function has no storage-class specifier, its linkage
is determined exactly as if it were declared with the storage-class specifier extern. If
the declaration of an identifier for an object has file scope and no storage-class specifier,
its linkage is external.

*6 The following identifiers have no linkage: an identifier declared to be anything other than
an object or a function; an identifier declared to be a function parameter; a block scope
identifier for an object declared without the storage-class specifier extern.

*7 If, within a translation unit, the same identifier appears with both internal and external
linkage, the behavior is undefined.

*21) There is no linkage between different identifiers.
*22) A function declaration can contain the storage-class specifier static only if it is at file scope; see
    6.7.1.


6.2.5 Types
=======================================================

1 The meaning of a value stored in an object or returned by a function is determined by the
type of the expression used to access it. (An identifier declared to be an object is the
simplest such expression; the type is specified in the declaration of the identifier.) Types
are partitioned into object types (types that fully describe objects), function types (types
that describe functions), and incomplete types (types that describe objects but lack
information needed to determine their sizes).

*19 The void type comprises an empty set of values; it is an incomplete type that cannot be
completed.

22 An array type of unknown size is an incomplete type. It is completed, for an identifier of
that type, by specifying the size in a later declaration (with internal or external linkage).
A structure or union type of unknown content (as described in 6.7.2.3) is an incomplete
type. It is completed, for all declarations of that type, by declaring the same structure or
union tag with its defining content later in the same scope.

6.2.7 Compatible type and composite type
=======================================================

1 Two types have compatible type if their types are the same. Additional rules for
determining whether two types are compatible are described in 6.7.2 for type specifiers,
in 6.7.3 for type qualifiers, and in 6.7.5 for declarators.46) Moreover, two structure,
union, or enumerated types declared in separate translation units are compatible if their
tags and members satisfy the following requirements: If one is declared with a tag, the
other shall be declared with the same tag. If both are complete types, then the following
additional requirements apply: there shall be a one-to-one correspondence between their
members such that each pair of corresponding members are declared with compatible
types, and such that if one member of a corresponding pair is declared with a name, the
other member is declared with the same name. For two structures, corresponding
members shall be declared in the same order. For two structures or unions, corresponding
bit-fields shall have the same widths. For two enumerations, corresponding members
shall have the same values.

*2 All declarations that refer to the same object or function shall have compatible type;
otherwise, the behavior is undefined.

*3 A composite type can be constructed from two types that are compatible; it is a type that
is compatible with both of the two types and satisfies the following conditions:
— If one type is an array of known constant size, the composite type is an array of that
size; otherwise, if one type is a variable length array, the composite type is that type.
— If only one type is a function type with a parameter type list (a function prototype),
the composite type is a function prototype with the parameter type list.
— If both types are function types with parameter type lists, the type of each parameter
in the composite parameter type list is the composite type of the corresponding
parameters.
These rules apply recursively to the types from which the two types are derived.

*4 For an identifier with internal or external linkage declared in a scope in which a prior
declaration of that identifier is visible,47) if the prior declaration specifies internal or
external linkage, the type of the identifier at the later declaration becomes the composite
type.


6.7 Declarations
=======================================================

Constraints

2 A declaration shall declare at least a declarator (other than the parameters of a function or
the members of a structure or union), a tag, or the members of an enumeration.

*3 If an identifier has no linkage, there shall be no more than one declaration of the identifier
(in a declarator or type specifier) with the same scope and in the same name space, except
for tags as specified in 6.7.2.3.

*4 All declarations in the same scope that refer to the same object or function shall specify
compatible types.

Semantics

5 A declaration specifies the interpretation and attributes of a set of identifiers. A definition
of an identifier is a declaration for that identifier that:
— for an object, causes storage to be reserved for that object;
— for a function, includes the function body;98)
— for an enumeration constant or typedef name, is the (only) declaration of the
identifier.

*7 If an identifier for an object is declared with no linkage, the type for the object shall be
complete by the end of its declarator, or by the end of its init-declarator if it has an
initializer; in the case of function arguments (including in prototypes), it is the adjusted
type (see 6.7.5.3) that is required to be complete.

6.7.1 Storage-class specifiers
=======================================================

Constraints

*2 At most, one storage-class specifier may be given in the declaration specifiers in a
declaration.99)

4 A declaration of an identifier for an object with storage-class specifier register
suggests that access to the object be as fast as possible. The extent to which such
suggestions are effective is implementation-defined.100)
100) The implementation may treat any register declaration simply as an auto declaration. However,
whether or not addressable storage is actually used, the address of any part of an object declared with
storage-class specifier register cannot be computed, either explicitly (by use of the unary &
operator as discussed in 6.5.3.2) or implicitly (by converting an array name to a pointer as discussed in
6.3.2.1). Thus, the only operator that can be applied to an array declared with storage-class specifier
register is sizeof.

*5 The declaration of an identifier for a function that has block scope shall have no explicit
storage-class specifier other than extern.

6 If an aggregate or union object is declared with a storage-class specifier other than
typedef, the properties resulting from the storage-class specifier, except with respect to
linkage, also apply to the members of the object, and so on recursively for any aggregate
or union member objects.


6.7.2 Type specifiers
=======================================================

Semantics
4 Specifiers for structures, unions, and enumerations are discussed in 6.7.2.1 through
6.7.2.3. Declarations of typedef names are discussed in 6.7.7. The characteristics of the
other types are discussed in 6.2.5.

6.7.2.1 Structure and union specifiers
=======================================================

Constraints

2 A structure or union shall not contain a member with incomplete or function type (hence,
a structure shall not contain an instance of itself, but may contain a pointer to an instance
of itself), except that the last member of a structure with more than one named member
may have incomplete array type; such a structure (and any union containing, possibly
recursively, a member that is such a structure) shall not be a member of a structure or an
element of an array.

*3 The expression that specifies the width of a bit-field shall be an integer constant
expression that has nonnegative value that shall not exceed the number of bits in an object
of the type that is specified if the colon and expression are omitted. If the value is zero,
the declaration shall have no declarator.

*4 A bit-field shall have a type that is a qualified or unqualified version of _Bool, signed
int, unsigned int, or some other implementation-defined type.

*7 The presence of a struct-declaration-list in a struct-or-union-specifier declares a new type,
within a translation unit. The struct-declaration-list is a sequence of declarations for the
members of the structure or union. If the struct-declaration-list contains no named
members, the behavior is undefined. The type is incomplete until after the } that
terminates the list.

*8 A member of a structure or union may have any object type other than a variably
modified type.102) In addition, a member may be declared to consist of a specified
number of bits (including a sign bit, if any). Such a member is called a bit-field;103) its
width is preceded by a colon.

11 A bit-field declaration with no declarator, but only a colon and a width, indicates an
unnamed bit-field.105) As a special case, a bit-field structure member with a width of 0
indicates that no further bit-field is to be packed into the unit in which the previous bit-
field, if any, was placed.

6.7.2.3 Tags
=======================================================

Constraints

*1 A specific type shall have its content defined at most once.

*2 A type specifier of the form
enum identifier
without an enumerator list shall only appear after the type it specifies is complete.

Semantics

3 All declarations of structure, union, or enumerated types that have the same scope and
use the same tag declare the same type. The type is incomplete109) until the closing brace
of the list defining the content, and complete thereafter.

*4 Two declarations of structure, union, or enumerated types which are in different scopes or
use different tags declare distinct types. Each declaration of a structure, union, or
enumerated type which does not include a tag declares a distinct type.

*6 A declaration of the form
struct-or-union identifier ;
specifies a structure or union type and declares the identifier as a tag of that type.111)
111) A similar construction with enum does not exist. (!!!)

*7 If a type specifier of the form
struct-or-union identifier
occurs other than as part of one of the above forms, and no other declaration of the
identifier as a tag is visible, then it declares an incomplete structure or union type, and
declares the identifier as the tag of that type.111)

*8 If a type specifier of the form
struct-or-union identifier
or
enum identifier
occurs other than as part of one of the above forms, and a declaration of the identifier as a
tag is visible, then it specifies the same type as that other declaration, and does not
redeclare the tag.

6.7.3 Type qualifiers
=======================================================

Constraints
2 Types other than pointer types derived from object or incomplete types shall not be
restrict-qualified.

8 If the specification of an array type includes any type qualifiers, the element type is soqualified,
not the array type. If the specification of a function type includes any type
qualifiers, the behavior is undefined.116)
116) Both of these can occur through the use of typedefs.


6.7.4 Function specifiers
=======================================================

Constraints

2 Function specifiers shall be used only in the declaration of an identifier for a function.

3 An inline definition of a function with external linkage shall not contain a definition of a
modifiable object with static storage duration, and shall not contain a reference to an
identifier with internal linkage.

4 In a hosted environment, the inline function specifier shall not appear in a declaration
of main.

Semantics

5 A function declared with an inline function specifier is an inline function. The
function specifier may appear more than once; the behavior is the same as if it appeared
only once. Making a function an inline function suggests that calls to the function be as
fast as possible.118) The extent to which such suggestions are effective is
implementation-defined.119)

6 Any function with internal linkage can be an inline function. For a function with external
linkage, the following restrictions apply: If a function is declared with an inline
function specifier, then it shall also be defined in the same translation unit. If all of the
file scope declarations for a function in a translation unit include the inline function
specifier without extern, then the definition in that translation unit is an inline
definition. An inline definition does not provide an external definition for the function,
and does not forbid an external definition in another translation unit. An inline definition
provides an alternative to an external definition, which a translator may use to implement
any call to the function in the same translation unit. It is unspecified whether a call to the
function uses the inline definition or the external definition.120)


6.7.5.2 Array declarators
=======================================================

Constraints

*1 In addition to optional type qualifiers and the keyword static, the [ and ] may delimit
an expression or *. If they delimit an expression (which specifies the size of an array), the
expression shall have an integer type. If the expression is a constant expression, it shall
have a value greater than zero. The element type shall not be an incomplete or function
type. The optional type qualifiers and the keyword static shall appear only in a
declaration of a function parameter with an array type, and then only in the outermost
array type derivation.

*2 Only an ordinary identifier (as defined in 6.2.3) with both block scope or function
prototype scope and no linkage shall have a variably modified type. If an identifier is
declared to be an object with static storage duration, it shall not have a variable length
array type.

Semantics

*4 If the size is not present, the array type is an incomplete type. If the size is * instead of
being an expression, the array type is a variable length array type of unspecified size,
which can only be used in declarations with function prototype scope;122) such arrays are
nonetheless complete types. If the size is an integer constant expression and the element
type has a known constant size, the array type is not a variable length array type;
otherwise, the array type is a variable length array type.

5 If the size is an expression that is not an integer constant expression: if it occurs in a
declaration at function prototype scope, it is treated as if it were replaced by *; otherwise,
each time it is evaluated it shall have a value greater than zero. The size of each instance
of a variable length array type does not change during its lifetime. Where a size
expression is part of the operand of a sizeof operator and changing the value of the
size expression would not affect the result of the operator, it is unspecified whether or not
the size expression is evaluated.

*6 For two array types to be compatible, both shall have compatible element types, and if
both size specifiers are present, and are integer constant expressions, then both size
specifiers shall have the same constant value. If the two array types are used in a context
which requires them to be compatible, it is undefined behavior if the two size specifiers
evaluate to unequal values.

6.7.5.3 Function declarators (including prototypes)
=======================================================

Constraints

*1 A function declarator shall not specify a return type that is a function type or an array
type.

*2 The only storage-class specifier that shall occur in a parameter declaration is register.

*3 An identifier list in a function declarator that is not part of a definition of that function
shall be empty.

*4 After adjustment, the parameters in a parameter type list in a function declarator that is
part of a definition of that function shall not have incomplete type.

Semantics

*7 A declaration of a parameter as ‘‘array of type’’ shall be adjusted to ‘‘qualified pointer to
type’’, where the type qualifiers (if any) are those specified within the [ and ] of the
array type derivation. If the keyword static also appears within the [ and ] of the
array type derivation, then for each call to the function, the value of the corresponding
actual argument shall provide access to the first element of an array with at least as many
elements as specified by the size expression.

*8 A declaration of a parameter as ‘‘function returning type’’ shall be adjusted to ‘‘pointer to
function returning type’’, as in 6.3.2.1.

9 If the list terminates with an ellipsis (, ...), no information about the number or types
of the parameters after the comma is supplied.123)

*10 The special case of an unnamed parameter of type void as the only item in the list
specifies that the function has no parameters.

*12 If the function declarator is not part of a definition of that function, parameters may have
incomplete type and may use the [*] notation in their sequences of declarator specifiers
to specify variable length array types.

*13 The storage-class specifier in the declaration specifiers for a parameter declaration, if
present, is ignored unless the declared parameter is one of the members of the parameter
type list for a function definition.

*14 An identifier list declares only the identifiers of the parameters of the function. An empty
list in a function declarator that is part of a definition of that function specifies that the
function has no parameters. The empty list in a function declarator that is not part of a
definition of that function specifies that no information about the number or types of the
parameters is supplied.124)

*15 For two function types to be compatible, both shall specify compatible return types.125)
Moreover, the parameter type lists, if both are present, shall agree in the number of
parameters and in use of the ellipsis terminator; corresponding parameters shall have
compatible types. If one type has a parameter type list and the other type is specified by a
function declarator that is not part of a function definition and that contains an empty
identifier list, the parameter list shall not have an ellipsis terminator and the type of each
parameter shall be compatible with the type that results from the application of the
default argument promotions. If one type has a parameter type list and the other type is
specified by a function definition that contains a (possibly empty) identifier list, both shall
agree in the number of parameters, and the type of each prototype parameter shall be
compatible with the type that results from the application of the default argument
promotions to the type of the corresponding identifier. (In the determination of type
compatibility and of a composite type, each parameter declared with function or array
type is taken as having the adjusted type and each parameter declared with qualified type
is taken as having the unqualified version of its declared type.)

6.7.7 Type definitions
=======================================================

Constraints

*2 If a typedef name specifies a variably modified type then it shall have block scope.

Semantics

3 In a declaration whose storage-class specifier is typedef, each declarator defines an
identifier to be a typedef name that denotes the type specified for the identifier in the way
described in 6.7.5. Any array size expressions associated with variable length array
declarators are evaluated each time the declaration of the typedef name is reached in the
order of execution. A typedef declaration does not introduce a new type, only a
synonym for the type so specified.

*/

static bool AreCompatibleTypes ( 
    const TQualNode * a, 
    const TQualNode * b, 
    bool            bIgnoreQuals,
    bool            bInitB 
  );

//--------------------------------------------------------------------------
// Name         Type_IsDefaultArgumentCompatible 
//
// returnes _true_ if the type is compatible with the type that results from
// applying the default argument promotions:
//   - integer promotion
//   - float promoted to double
//--------------------------------------------------------------------------
static bool Type_IsDefaultArgumentCompatible ( const TQualNode * typ )
{
  ASSERT( typ != NULL );

  if (Type_IsInteger( typ ))
  { 
    // see: TYPE_IntegerPromotion()
    const TIntegerTypeDesc * typDesc = typ->tnode->u.pIntDesc;

    if (typDesc->rank < Int_Desc.rank)
    {
      if (typDesc->pMachType->precision < Int_Desc.pMachType->precision)
        return false;
    }
  }
  else // if is float
  if (typ->tnode->typeKind == TYPE_REAL && typ->tnode->u.realType == rt_Float)
  {
    return false;
  }

  return true;
};

//--------------------------------------------------------------------------
// Name         AreCompatibleParamLists
//
// Check if the parameters of two function types are compatible
//--------------------------------------------------------------------------
static bool AreCompatibleParamLists ( 
    const TTypeNode * ta, 
    const TTypeNode * tb,
    bool            bInitB
  )
{
  const TScope * sa, * sb;
  const TScope * idScope, * parScope;

/*
  - params && params
  - params && !definition empty_ident_list

*/
  sa = ta->u.memberScope;
  sb = tb->u.memberScope;

  // See if one of the scopes is old-style identifier list
  if (Scope_IsIdentList( sa ))
  {
    // if both are old-style ident lists, they are compatible
    //
    if (Scope_IsIdentList( sb ))
      return true;

    // We care about the contents of the identifier scope only if it is in 
    // a function definition. In this case it can't be, so we ignore it.
    // (it should be NULL anyway, since old-style ident lists are an error
    // in function declarations)
    //
    idScope = NULL;
    parScope = sb;
  }
  else
  if (Scope_IsIdentList( sb ))
  {
    // We care about the contents of the identifier scope only if it is in 
    // a function definition.
    idScope = bInitB ? sb : NULL;
    parScope = sa;
  }
  else
  {
    const TSymbol * syma, * symb;

    // both scopes are new-style parameter scopes
    syma = sa->pFirstSymbol;
    symb = sb->pFirstSymbol;

    for(;;)
    {
      // Check if one or both are NULL
      //
      if (syma == NULL)
        return symb == NULL;
      else
      if (symb == NULL)
        return false; 

      // If one is "...", both must be "..."
      //
      if (syma->id.strName == CfeRoots.EllipsisName)
      {
        ASSERT( syma->id.pNext == NULL ); // this must be the last param
        return symb->id.strName == CfeRoots.EllipsisName;
      }
      else
      if (symb->id.strName == CfeRoots.EllipsisName)
        return false;

      // The *unqualified* (!) types must be compatible
      //
      if (!AreCompatibleTypes( syma->type, symb->type, true, false ))
        return false;

      syma = (const TSymbol *)syma->id.pNext;
      symb = (const TSymbol *)symb->id.pNext;
    }

    ASSERT( 0 ); // never reach here
  }

  // one of the scopes is old-style identifier list
  //
  {
    const TSymbol * par, * id;

    // The parameter scope must not have "..."
    if (parScope->bHaveEllipsis)
      return false;

    // Make sure the number of parameters matches teh number of idents,
    // but only if we actually have an ident list
    //
    par = parScope->pFirstSymbol;
    id = idScope != NULL ? idScope->pFirstSymbol : NULL;

    for(;;)
    {
      // Check if one or both are NULL
      //
      if (par == NULL)
        return id == NULL;
      else
      if (id == NULL && idScope != NULL)
        return false;

      // All parameters must have types compatible with the result of default
      // argument promotions.
      //
      if (!Type_IsDefaultArgumentCompatible( par->type ))
        return false;

      par = (const TSymbol *)par->id.pNext;
      if (id != NULL)
        id = (const TSymbol *)id->id.pNext;
    }

    ASSERT( 0 ); // never reach here
  }
};

//--------------------------------------------------------------------------
// Name         AreCompatibleTypes
//
// Note: the types must have been validated.
//--------------------------------------------------------------------------
static bool AreCompatibleTypes ( 
    const TQualNode * a, 
    const TQualNode * b, 
    bool            bIgnoreQuals, 
        // Ignore the qualifiers of the first tnode. We need this when
        // checking function parameters, since the standard says that they
        // are compared unqualified
    bool            bInitB  // whether b is being initialized
  )
{
  const TTypeNode * ta, * tb;

  for(;;)
  {
    ASSERT( a != NULL && b != NULL );

    // if the types are the same, they are compatible
    if (a == b)
      return true;

    // Check the qualifiers
    if (!bIgnoreQuals)
    {
      if ((a->isConst     ^ b->isConst)    ||
          (a->isVolatile  ^ b->isVolatile) ||
          (a->isRestrict  ^ b->isRestrict) ||
          (a->extQualAttr ^ b->extQualAttr))
      {
        return false;
      }
    }

    ta = a->tnode;
    tb = b->tnode;

    // We already checked the qualifiers, so
    // if the type nodes are the same, they are compatible
    if (ta == tb)
      return true;

    // The error type is compatible with everything
    if (ta->typeKind == TYPE_ERROR || tb->typeKind == TYPE_ERROR)
      return true;

    // tnodes must be the same kind
    if (ta->typeKind != tb->typeKind)
      return false;

    switch (ta->typeKind)
    {
    case TYPE_VOID:
      return true;

    case TYPE_ENUM:
      return false; // compatible enums must be the same enum

    case TYPE_BOOL:
    case TYPE_INTEGER:
      return ta->u.pIntDesc == tb->u.pIntDesc;

    case TYPE_REAL:
    case TYPE_IMAGINARY:
    case TYPE_COMPLEX:
      return ta->u.realType == tb->u.realType;

    case TYPE_STRUCT:
    case TYPE_UNION:
      return false;  // compatible structs must be the same struct

    case TYPE_ARRAY:
      // IsConstantExpression assumes NULL expression to be non-constant
      if (IsConstantExpression( ta->u.arrayLen ) &&
          IsConstantExpression( tb->u.arrayLen ))
      {
        // The types must have been validated, so they must be integers
        ASSERT( Type_IsInteger( ta->u.arrayLen->exprType ) );
        ASSERT( Type_IsInteger( tb->u.arrayLen->exprType ) );

        if (!TARGET_EQ( &ta->u.arrayLen->u.intConst, &tb->u.arrayLen->u.intConst ))
          return false;
      }
      break; // check the element type

    case TYPE_POINTER:
      break; // just check what we point to

    case TYPE_FUNCTION:
      if (!AreCompatibleParamLists( ta, tb, bInitB ))
        return false;
      break; // check the result type

    default:
      CHECK_ASSUME( 0 );
      return false;
    }

    bIgnoreQuals = false;
    bInitB = false;
    a = ta->of;
    b = tb->of;
  }
};

static const TQualNode * CompositeType ( 
    const TQualNode * a, 
    const TQualNode * b, 
    bool      bInitB 
  );

static const TQualNode * CompositeType_Prim ( 
    const TQualNode * a, 
    const TQualNode * b, 
    bool      bInitB 
  );

//--------------------------------------------------------------------------
// Name         CompositeFunctionType_Optimistic 
//
// Work under the assumption that we can return NULL, _a_ or _b_
//--------------------------------------------------------------------------
static bool CompositeFunctionType_Optimistic ( 
    const TQualNode * a, 
    const TQualNode * b,
    const TQualNode * compOf,
    const TQualNode ** ppRes
  )
{
  bool bResult = false;
  const TSymbol * syma, * symb;
  bool ra, rb;

  // compOf must be <either> or it must be either a's or b's return
  // type
  if (compOf == NULL)
  {
    ra = rb = true;
  }
  else
  if (compOf == a->tnode->of)
  {
    ra = true;
    rb = false;
  }
  else
  if (compOf == b->tnode->of)
  {
    ra = false;
    rb = true;
  }
  else
    goto exitFunc; // neither

  syma = a->tnode->u.memberScope->pFirstSymbol;
  symb = b->tnode->u.memberScope->pFirstSymbol;

  while (syma != NULL)
  {
    const TQualNode * compType;

    ASSERT( symb != NULL );

    // We always copy the name of the _b_ (later) parameter.
    // If both parameter names are the same, we can use either, but if
    // not, we must use _b_

    if (syma->id.strName != symb->id.strName)
    {
      // we must use _b_
      if (!rb)
        goto exitFunc;
      ra = false;
    }

    compType = CompositeType_Prim( syma->type, symb->type, false );

    if (compType != NULL)
    {
      if (compType == syma->type)
      {
        if (!ra)
          goto exitFunc;
        rb = false;
      }
      else
      if (compType == symb->type)
      {
        if (!rb)
          goto exitFunc;
        ra = false;
      }
    }

    syma = (TSymbol *)syma->id.pNext;
    symb = (TSymbol *)symb->id.pNext;
  }
  ASSERT( symb == NULL );

  if (ra)
  {
    if (rb)
      *ppRes = NULL;
    else
      *ppRes = a;
  }
  else
    *ppRes = b;

  bResult = true;

exitFunc:
  return bResult;
};

//--------------------------------------------------------------------------
// Name         CompositeFunctionType_Full 
//
// 
//--------------------------------------------------------------------------
static const TQualNode * CompositeFunctionType_Full ( 
    const TQualNode * a, 
    const TQualNode * b,
    const TQualNode * compOf
  )
{
  TSymbol * syma, * symb;
  const TQualNode * compType;

  // If the type result is <either> use the type of _a_
  if (compOf == NULL)
    compOf = a->tnode->of;

  // Copy the function node and the return type
  //
  compType = QualNode_Clone( a, TypeNode_Clone( a->tnode, compOf ) );
  MODIFY_TNODE(compType->tnode)->u.memberScope = Scope_Clone( a->tnode->u.memberScope );

  // Copy the symbols
  //
  syma = a->tnode->u.memberScope->pFirstSymbol;
  symb = b->tnode->u.memberScope->pFirstSymbol;

  while (syma != NULL)
  {
    TSymbol * curParam;

    ASSERT( symb != NULL );

    // Copy the name of the _b_ (later) parameter

    curParam = Symbol_Clone( symb );
    curParam->type = CompositeType( syma->type, symb->type, false );

    Scope_AddSymbol( compType->tnode->u.memberScope, curParam );

    syma = (TSymbol *)syma->id.pNext;
    symb = (TSymbol *)symb->id.pNext;
  }
  ASSERT( symb == NULL );

  return compType;
};

//--------------------------------------------------------------------------
// Name         CompositeFunctionType
//
// 
//--------------------------------------------------------------------------
static const TQualNode * CompositeFunctionType ( 
    const TQualNode * a, 
    const TQualNode * b,
    const TQualNode * compOf
  )
{
  TQualNode * res;

  if (CompositeFunctionType_Optimistic( a, b, compOf, &res ))
    return res;

  return CompositeFunctionType_Full( a, b, compOf );
};

//--------------------------------------------------------------------------
// Name         CompositeType_Prim 
//
// A helper for CompositeType. 
// Since we keep the type and qual nodes immutable, in order to minimize 
// allocation, CompositeType_Prim returns NULL when any of the types can be
// used as a composite type.
//--------------------------------------------------------------------------
static const TQualNode * CompositeType_Prim ( 
    const TQualNode * a, 
    const TQualNode * b, 
    bool      bInitB 
  )
{
  const TTypeNode * ta, * tb;
  const TQualNode * compOf;

  ASSERT( AreCompatibleTypes( a, b, true, bInitB ) );
  ASSERT( a != NULL && b != NULL );

  if (a == b) // if same types, we are done
    return NULL;

  ta = a->tnode;
  tb = b->tnode;

  if (ta == tb)  // if same type nodes. we are done
    return NULL;

  // The composite of anything with an error type is the error type.
  // Check the case of _b_ being error-type here and check for _a_
  // later in the switch.
  //
  if (tb->typeKind == TYPE_ERROR)
  {
    if (ta->typeKind == TYPE_ERROR) // both are errors - return "either"
      return NULL;
    return b;  // only _b_ is error. 
  }

  switch (ta->typeKind)
  {
  case TYPE_ERROR:
    // We checked eralier that tb is not TYPE_ERROR
    ASSERT( tb->typeKind != TYPE_ERROR );
    return a;

  case TYPE_VOID:
  case TYPE_BOOL:
  case TYPE_INTEGER:
  case TYPE_ENUM:
  case TYPE_REAL:
  case TYPE_IMAGINARY:
  case TYPE_COMPLEX:
    return NULL;

  case TYPE_STRUCT:
  case TYPE_UNION:
    // two structs are compatible if they are the same struct, so we should
    // have not reached here
    CHECK_ASSUME( 0 );
    return NULL;

  case TYPE_ARRAY:
    {
      bool ra, rb;

      // Get the composite type of the element type
      compOf = CompositeType_Prim( ta->of, tb->of, false );

      // Play a little game to avoid allocation if we can
      //

      // Determine which type is suitable to be returned
      //
      // - prefer  [const] over [variable], [*] and []
      ra = IsConstantExpression( ta->u.arrayLen );
      rb = IsConstantExpression( tb->u.arrayLen );

      // - prefer [variable] over [*] and [] 
      if (!ra && !rb)
      {
        ra = ta->u.arrayLen != NULL && ta->u.arrayLen->code != ast_unspecifiedVariableArraySize;
        rb = tb->u.arrayLen != NULL && tb->u.arrayLen->code != ast_unspecifiedVariableArraySize;
      }

      // prefer [*] over []
      if (!ra && !rb)
      {
        ra = ta->u.arrayLen != NULL;
        rb = tb->u.arrayLen != NULL;
      }

      if (!ra && !rb)
      {
        ra = true;
        rb = true;
      }

      if (!ra)
        ta = NULL;
      if (!rb)
        tb = NULL;
    }
    break;

  case TYPE_POINTER:
    // Get the composite type of the element type
    compOf = CompositeType_Prim( ta->of, tb->of, false );

    // Preserve _staticArraySize_
    //
    if (ta->staticArraySize)
    {
      if (!tb->staticArraySize)
        tb = NULL;
    }
    else
    if (tb->staticArraySize)
      ta = NULL;

    break;

  case TYPE_FUNCTION:
    // Get the composite type of the element type
    compOf = CompositeType_Prim( ta->of, tb->of, false );

    if (ta->u.memberScope == NULL)   // ta is empty ident list?
    {
      if (tb->u.memberScope != NULL) // tb is NOT an empty ident list?
        ta = NULL;
      // else - both are empty ident lists and are equally suitable
    }
    else
    if (tb->u.memberScope == NULL)   // tb is empty ident list?
    {
      tb = NULL; // ta is OK
    }
    else
    if (Scope_IsIdentList( ta->u.memberScope ))    // ta is an ident list?
    {
      if (!Scope_IsIdentList( tb->u.memberScope )) // tb is NOT an ident list?
        ta = NULL;
      // else - both are non-empty ident lists and are equally suitable
    }
    else
    if (Scope_IsIdentList( tb->u.memberScope ))    // tb is an ident list?
    {
      tb = NULL; // ta is OK
    }
    else
    {
      // We have reached the problematic case. Both functions are new-style, so
      // the type of each parameter is the composite type of the corresponding
      // parameters
      //
      return CompositeFunctionType( a, b, compOf );
    }
    break;

  default:
    CHECK_ASSUME( 0 );
    return NULL;
  }

  // prepare a valid result in _a_
  if (ta == NULL)
    a = b;

  // If the composite type of the child is "either", 
  if (compOf == NULL)
  {
    if (ta != NULL && tb != NULL)
    {
      // and both altarnatives at this level are valid, return NULL, which
      // will indicate to our called that the composite type of the
      // child is "either"
      return NULL;
    }
    else
    {
      // Otherwise, return the valid result
      return a;
    }
  }

  if (ta != NULL && compOf == ta->of)  
    return a;
  if (tb != NULL && compOf == tb->of)  
    return b;

  return QualNode_Clone( a, TypeNode_Clone( a->tnode, compOf ) );
};

//--------------------------------------------------------------------------
// Name         CompositeType
//
// Note: _b_ is the later type.
//--------------------------------------------------------------------------
static const TQualNode * CompositeType ( const TQualNode * a, const TQualNode * b, bool bInitB )
{
  const TQualNode * res;

  if (NULL == (res = CompositeType_Prim( a, b, bInitB )) )
    // If it doesn't matter, return the first type
    res = a;

  return res;
};


//--------------------------------------------------------------------------
// Name         ValidateArrayType 
//
// 
//--------------------------------------------------------------------------
static TTypeNode * ValidateArrayType ( 
    const TTypeNode * tnode, 
    StorageClass_t  sclass,
    bool            bOuter // is this the outermost type derivation
  )
{
  TTypeNode * rnode = NULL;

  // Is this a variable expression ?
  //
  if (tnode->u.arrayLen != NULL &&             // ! []
      tnode->u.arrayLen->code != ast_error &&  // ! [error]
      tnode->u.arrayLen->code != ast_unspecifiedVariableArraySize && // ![*]
      !IsConstantExpression( tnode->u.arrayLen ) // ! [const]
     )
  {
    if (CfeRoots.CurScope->disposition != SCOPE_PARAMS &&
        CfeRoots.CurScope->disposition != SCOPE_BLOCK ||
        sclass == SC_EXTERN || sclass == SC_STATIC) 
    {
      CompilerError( "Variable length array is not allowed here" );

      CLONE_RNODE( rnode, tnode, tnode->of )->u.arrayLen = Ast_error( &tnode->u.arrayLen->loc );
      goto exitFunc;
    }
  }

exitFunc:
  return rnode != NULL ? rnode : tnode;
}

//--------------------------------------------------------------------------
// Name         CheckForUnspecifiedVariableArraySize 
//
// Checks a type for the presence of [*]
//--------------------------------------------------------------------------
static bool CheckForUnspecifiedVariableArraySize ( const TQualNode * qual )
{
  const TTypeNode * tnode;

  do
  {
    ASSERT( qual != NULL );

    tnode = qual->tnode;
    ASSERT( tnode != NULL );

    if (tnode->typeKind == TYPE_ARRAY && tnode->u.arrayLen != NULL &&
        tnode->u.arrayLen->code == ast_unspecifiedVariableArraySize)
    {
      return false;
    }
  }
  while (NULL != (qual = tnode->of) );

  return false;
};

//--------------------------------------------------------------------------
// Name         ReplaceVariableLen 
//
// Replace variable-sized arays with [*] and ignore _staticArraySize_
//--------------------------------------------------------------------------
static const TQualNode * ReplaceVariableLen ( const TQualNode * qual )
{
  TTypeNode       * rnode = NULL;
  const TTypeNode * tnode = qual->tnode;

  // Process the "derived from" type
  //
  if (tnode->of != NULL)
  {
    const TQualNode * rof;
    if ( (rof = ReplaceVariableLen( tnode->of )) != tnode->of)
      CLONE_RNODE( rnode, tnode, rof );
  }

  // Ignore _staticArraySize_
  //
  if (tnode->staticArraySize)
    CLONE_RNODE( rnode, tnode, tnode->of )->staticArraySize = false;

  if (tnode->typeKind == TYPE_ARRAY)
  {
    if (tnode->u.arrayLen != NULL && 
        tnode->u.arrayLen->code != ast_unspecifiedVariableArraySize &&
        !IsConstantExpression( tnode->u.arrayLen ))
    {
      CLONE_RNODE( rnode, tnode, tnode->of )->u.arrayLen = Ast( ast_unspecifiedVariableArraySize, &tnode->u.arrayLen->loc );
    }
  }

  return qual->tnode == tnode ? qual : QualNode_Clone( qual, tnode );
};

//--------------------------------------------------------------------------
// Name         ValidateParameterType 
//
// 
//--------------------------------------------------------------------------
static const TQualNode * ValidateParameterType (
    const TSymbol    * name,    // for error messages
    const TQualNode  * qual,
    bool             bDefinition
  )
{
  if (bDefinition)
  {
// 6.7.5.3-4 After adjustment, the parameters in a parameter type list in 
// a function declarator that is part of a definition of that function 
// shall not have incomplete type.
//
    if (!Type_IsComplete( qual ))
    {
      CompilerErrorLoc( &name->declLoc, "'%s': parameter must have a complete type in a function definition", Symbol_Name(name) );
      qual = CfeRoots.ErrorType;
    }
    else
// 6.7.5.3-12 If the function declarator is not part of a definition of 
// that function, parameters may have incomplete type and may use the [*] 
// notation in their sequences of declarator specifiers to specify 
// variable length array types.
//
    if (CheckForUnspecifiedVariableArraySize( qual ))
    {
      CompilerErrorLoc( &name->declLoc, "'%s': variable length array cannot use [*] in a function definition", Symbol_Name(name) );
      qual = CfeRoots.ErrorType;
    }
  }
  else
  {
    // Replace variable-sized arays with [*] and  ignore _staticArraySize_
    qual = ReplaceVariableLen( qual );
  }

  return qual;
};

//--------------------------------------------------------------------------
// Name         ValidateParameter
//
// Validates a parameter type within a function declaration:
//   - no unnamed parameters in function definition
//   - enforce complete types in function definition
//   - no [*] in function definition
//
//   - replace variable-size arrays with [*] in non-definitions
//   - remove storage class in non-definitions
//   - remove parameter name in non-definitions
//--------------------------------------------------------------------------
static TSymbol * ValidateParameter ( 
    const TSymbol * sym,
    bool    bDefinition
  )
{
  TSymbol * rsym = NULL;

  // Check for "..." and ignore it
  if (sym->id.strName == CfeRoots.EllipsisName)
    goto exitFunc;

  // Validate the type
  //
  {
    const TQualNode * rtype;

    if ( (rtype = ValidateParameterType( sym, sym->type, bDefinition )) != sym->type)
      CLONE_RSYM( rsym, sym )->type = rtype;
  }

  if (bDefinition)
  {
    if (sym->id.strName == NULL)
      CompilerStrictErrorLoc( &sym->declLoc, "Unnamed parameters not allowed in function definition" );
  }
  else
  {
// 6.7.5.3-13 The storage-class specifier in the declaration specifiers 
// for a parameter declaration, if present, is ignored unless the declared 
// parameter is one of the members of the parameter type list for a 
// function definition.
    if (sym->storageClass != SC_NONE)
      CLONE_RSYM( rsym, sym )->storageClass = SC_NONE;

    // Remove parameter names in non-definitions
    //
    if (sym->id.strName != NULL)
      Symbol_ChangeName( CLONE_RSYM( rsym, sym ), NULL );
  }

exitFunc:
  return rsym != NULL ? rsym : sym;
}

//--------------------------------------------------------------------------
// Name         ValidateFunctionParameters 
//
// 
//--------------------------------------------------------------------------
static TScope * ValidateFunctionParameters (
    const TScope * memberScope,
    bool           bDefinition
  )
{
  const TSymbol * sym;
  int     state;

  const TSymbol * origSym; // record the first changed symbol
  TSymbol * resSym;        // record ValidateParameter(origSym)
  TScope * resScope; // the new scope

  /*
    We go to great lengtsh to avoid copying the parameter scope if we don't
    have to. We begin traversing the scope under the assumption that we will
    not have to copy it (state 0). If we find out that we have to copy, we start 
    over copying the symbols which we already processed , until we reach the same
    spot (state 1). Then we continue processing the symbols, this time copying
    every one into the new scope.
  */

  state = 0;
  resScope = NULL;

restartLoop:
  for ( sym = memberScope->pFirstSymbol; sym != NULL; sym = (const TSymbol *)sym->id.pNext )
  {
    switch (state)
    {
      TSymbol * rsym;

    // state 0: in this state we operate under the assumption that no
    // symbols will be modified by ValidateParameter(), so basically
    // we just skip over the symbols until one is actually modified
    //
    case 0:
      // If the symbol was changed
      if ( (rsym = ValidateParameter( sym, bDefinition )) != sym)
      {
        // Move to state 1 where we will copy all the symbols that we skipped
        // in state 0
        state = 1;

        // Create a new scope for
        resScope = Scope_Clone( memberScope );

        // record where we reached
        origSym = sym;
        resSym = rsym;

        goto restartLoop;
      }
      break;

    // state 1: In this state we copy the symbols which we skipped over
    // in state 0.
    //
    case 1:
      if (sym != origSym) // still haven't reached the first changed symbol
      {
        // ValidateParemeter() should not change this one, since it didn't
        // in the previous state
        ASSERT( ValidateParameter( sym, bDefinition ) == sym );

        // Clone the symbol
        rsym = Symbol_Clone( sym );

        // Add it to the new scope
        Scope_AddSymbol( resScope, rsym );
      }
      else
      {
        // We reached the first changed symbol!
        // Add it to the new scope and move to state 2
        state = 2;

        rsym = resSym;
        Scope_AddSymbol( resScope, rsym );
      }
      break;

    // state 2: In this state we clone and add the symbols to the new scope
    //
    case 2:
      // If the symbol was not cloned by ValidateParameter(), we clone it
      if ((rsym = ValidateParameter( sym, bDefinition )) == sym)
        rsym = Symbol_Clone( sym );

      Scope_AddSymbol( resScope, rsym );
      break;

    default:
      CHECK_ASSUME( 0 );
    }
  }

  return resScope != NULL ? resScope : memberScope;
};

//--------------------------------------------------------------------------
// Name         ValidateFunctionType 
//
// Validates the type of a function:
//  - no function definition returning incomplete type except void
//  - old-style non-definitions must have empty ident lists 
//
//  - Validate each parameter
//--------------------------------------------------------------------------
static TTypeNode * ValidateFunctionType ( 
    const TTypeNode   * tnode,
    bool        bDefinition
  )
{
  TTypeNode * rnode = NULL;

  // a function in its definition cannot return an incomplete type,
  // except void
  if (bDefinition && 
      !Type_IsVoid( tnode->of ) && !Type_IsComplete( tnode->of ))
  {
    CompilerError( "Function in must return a complete type" );
    CLONE_RNODE( rnode, tnode, CfeRoots.ErrorType );
  }

  // 6.7.5.3-3 An identifier list in a function declarator that is not part 
  // of a definition of that function shall be empty.
  //
  if (!bDefinition && 
      Scope_IsIdentList( tnode->u.memberScope ) &&
      tnode->u.memberScope != NULL)
  {
    CompilerError( "Identifier list is only allowed in function declaration" );
    CLONE_RNODE( rnode, tnode, tnode->of )->u.memberScope = NULL;
  }

  // Check the parameters
  //
  if (tnode->u.memberScope != NULL && 
      tnode->u.memberScope->disposition == SCOPE_PARAMS)
  {
    TSymbol * sym = tnode->u.memberScope->pFirstSymbol;

    // A parameter scope cannot be empty (or it would be an ident scope)
    ASSERT( sym != NULL );

    // check for the special case of (void)
    if (Type_IsVoid(sym->type))      
    {
      // function without parameters
    }
    else
    {
      TScope * resScope = ValidateFunctionParameters( tnode->u.memberScope, bDefinition );

      if (resScope != tnode->u.memberScope)
        CLONE_RNODE( rnode, tnode, tnode->of )->u.memberScope = resScope;
    }
  }

  return rnode != NULL ? rnode : tnode;
};

//--------------------------------------------------------------------------
// Name         ValidateType_Prim
//
// Validate the type to remove:
//   - function returning array
//   - function returning function
//   - variable-length array with inapropriate storage class/duration 
//
// Note: We pay extra attention to clone types only when really necessary.
//--------------------------------------------------------------------------
static const TQualNode * ValidateType_Prim (
    const TQualNode   * qual, 
    StorageClass_t    sclass, 
    bool              bInit,
    bool              bOuter
  )
{
  const TTypeNode * tnode = qual->tnode;
  const TQualNode * rof;

  ASSERT( qual != NULL );
  
  switch (tnode->typeKind)
  {
  case TYPE_ARRAY:
    tnode = ValidateArrayType( tnode, sclass, bOuter );
    break;

  case TYPE_FUNCTION:
    tnode = ValidateFunctionType( tnode, bInit );
    break;
  }

  // Validate the type we are derived from
  //
  rof = tnode->of != NULL ? 
    ValidateType_Prim( tnode->of, sclass, false, false ) :
    NULL;

  // If the "derived from" type changed, we must update it 
  if (rof != tnode->of)
  {
    if (tnode == qual->tnode)               // if not already cloned, clone
      tnode = TypeNode_Clone( tnode, rof );
    else
      MODIFY_TNODE(tnode)->of = rof;        // we are already cloned => just update
  }

  // If the tnode has changed, clone the qual, otherwise just return it
  //
  return tnode == qual->tnode ? qual : QualNode_Clone( qual, tnode ); 
}

//--------------------------------------------------------------------------
// Name         ValidateType 
//
// 
//--------------------------------------------------------------------------
const TQualNode * ValidateType ( 
    const TQualNode   * qual, 
    StorageClass_t    sclass, 
    bool              bInit 
  )
{
  return ValidateType_Prim( qual, sclass, bInit, true );
};


/*
7 A declaration of a parameter as ‘‘array of type’’ shall be adjusted to ‘‘qualified pointer to
type’’, where the type qualifiers (if any) are those specified within the [ and ] of the
array type derivation. If the keyword static also appears within the [ and ] of the
array type derivation, then for each call to the function, the value of the corresponding
actual argument shall provide access to the first element of an array with at least as many
elements as specified by the size expression.

8 A declaration of a parameter as ‘‘function returning type’’ shall be adjusted to ‘‘pointer to
function returning type’’, as in 6.3.2.1.
*/
//--------------------------------------------------------------------------
// Name         AdjustParameterType
//
// Adjust parameter types by converting arrays and functions to pointers
//--------------------------------------------------------------------------
static const TQualNode * AdjustParameterType ( const TQualNode * qual )
{
  const TTypeNode * tnode = qual->tnode;
  
  if (tnode->typeKind == TYPE_ARRAY)
  {
    qual = Type_MakePointer( tnode->of, QualNode_Clone( qual, NULL ) );

    // Copy the static array size ( e.g. [static 10])
    //
    MODIFY_TNODE(qual->tnode)->staticArraySize = tnode->staticArraySize;
    MODIFY_TNODE(qual->tnode)->u.arrayLen      = tnode->u.arrayLen;
  }
  else
  if (tnode->typeKind == TYPE_FUNCTION)
    qual = Type_MakePointer( qual, NULL );

  return qual;
};

//--------------------------------------------------------------------------
// Name         SameScopeError 
//
// 
//--------------------------------------------------------------------------
static void SameScopeError ( TSymbol * visibleSymbol )
{
  TST,CompilerError( "'%s': already declared in this scope", Symbol_Name(visibleSymbol) );  
  ShowPrevDeclaration( visibleSymbol );
};

//--------------------------------------------------------------------------
// Name         SAME_SCOPE_CHECK
//
// Checks for redeclaration in the same scope, provided a condition
// (cond) is true. 
// Note:
//  - (cond) is evaluated ONLY if there is a visible symbol in the same scope.
//    which allows it to refer to visibleSymbol safely.
//  - that is why this must be a macro.
//--------------------------------------------------------------------------
#define SAME_SCOPE_CHECK( cond )                                        \
  do {                                                                  \
    if (visibleSymbol != NULL &&                                        \
        visibleSymbol->id.scope == CfeRoots.CurScope && (cond))         \
    {                                                                   \
      SameScopeError( visibleSymbol );                                  \
      goto failFunc;                                                    \
    }                                                                   \
  } while (0)


//--------------------------------------------------------------------------
// Name         IsDeclEmpty 
//
// Returns true if a declaration does not declare anything, e.g:
//    "static int;"
// so that we can generate a warning.
// struct/union/enum declarations are excluded because because we neen noot
// generate a warning for cases like:
//    "struct x;"
//    "union y { int a; char b; };"
//--------------------------------------------------------------------------
static bool IsDeclEmpty ( const TSymbol * sym )
{
  return
    sym->id.strName == NULL && 
    sym->type->tnode->typeKind != TYPE_STRUCT && 
    sym->type->tnode->typeKind != TYPE_UNION &&
    sym->type->tnode->typeKind != TYPE_ENUM;
}

//--------------------------------------------------------------------------
// Name         RedeclareGlobalSymbol
//
// sym is a redeclaration of globalSymbol and possibly augments its type and/or 
// declspecs. Update globalSymbol with the new type.
//
// Verify that the types and declspecs are compatible and update the 
// globalSymbol with the composite type+declspecs.
//
// returns false on error.
//--------------------------------------------------------------------------
static bool RedeclareGlobalSymbol ( TSymbol * globalSymbol, TSymbol * sym, bool bInit )
{
  bool bResult = false;

  if (!AreCompatibleTypes( globalSymbol->type, sym->type, false, bInit ))
  {
    CompilerError( "'%s': type is incompatible with previous declaration", Symbol_Name(sym) ); 
    ShowPrevDeclaration( globalSymbol );
    goto exitFunc;
  }

  // Combine the types
  globalSymbol->type = CompositeType( globalSymbol->type, sym->type, bInit );

  // Combine extDeclSpec and validate the result (to be safe)
  globalSymbol->extDeclSpec = CfeConf_CompositeExtDeclSpec( globalSymbol, sym->extDeclSpec );
  CfeConf_ValidateExtDeclSpec( globalSymbol, bInit );

  // Update the declaration location
  globalSymbol->declLoc = sym->declLoc;

  bResult = true;

exitFunc:
  return bResult;
}



//--------------------------------------------------------------------------
// Name         ValidateDeclaration 
//
// Makes sure that the object could be allocated. Prevents things like:
//   - void x;
//   - incomplete objects in block scope
//   - incomplete objects which are initialized
//--------------------------------------------------------------------------
static void ValidateDeclaration ( TSymbol * sym, bool bInit )
{
  StorageDuration_t sduration = sym->storageDuration;

  /*
  6.7-7 If an identifier for an object is declared with no linkage, the type for the object shall be
  complete by the end of its declarator, or by the end of its init-declarator if it has an
  initializer; in the case of function arguments (including in prototypes), it is the adjusted
  type (see 6.7.5.3) that is required to be complete.
  */

  // if not a typedef and not a function, we must make sure the object
  // could be allocated
  //
  if (sduration != STORAGE_DURATION_CODE && sduration != STORAGE_DURATION_NONE)
  {
    // Initialized and auto objects must have complete type
    //
    if ((bInit || sduration == STORAGE_DURATION_AUTO) && !Type_IsComplete( sym->type ))
    {
      CompilerError( "'%s': uses incomplete type", Symbol_Name(sym) );
      sym->type = CfeRoots.ErrorType;
    }
    else
    if (Type_IsVoid( sym->type )) // void can never be completed
    {
      CompilerError( "'%s': uses void type", Symbol_Name(sym) );
      sym->type = CfeRoots.ErrorType;
    }
  }
}


//--------------------------------------------------------------------------
// Name         DeclareFileScope 
//
// 
//--------------------------------------------------------------------------
static TSymbol * DeclareFileScope ( 
    const TDeclSpecs * dspecs, 
    TSymbol          * sym,
    bool             bInit
  )
{
  TSymbol * globalSymbol;
  TSymbol * visibleSymbol = SymTab_LookupSymbol( sym->id.strName );
  StorageClass_t  sclass  = dspecs->sclass;
  StorageDuration_t sduration = STORAGE_DURATION_STATIC;

  // Search the scope's ident table for this symbol.
  // It is not necessarily visible, since it may have been previously declared
  // with a block-scope extern
  //
  globalSymbol = Scope_LookupSymbol( CfeRoots.GlobalScope, sym->id.strName );

  if (sym->type->tnode->typeKind == TYPE_FUNCTION)
    sduration = STORAGE_DURATION_CODE;

  switch (sclass)
  {
  case SC_AUTO:
  case SC_REGISTER:
    TST,CompilerError( "Invalid storage class in file scope", Symbol_Name(sym) );
    // Fix the error as best as we can. auto and register 
    sclass = SC_NONE;
    fall;

  case SC_NONE:
    // SC_NONE functions default to SC_EXTERN
    if (sym->type->tnode->typeKind == TYPE_FUNCTION)
    {
      sclass = SC_EXTERN;
      goto case_SC_EXTERN;
    }

    TST;SAME_SCOPE_CHECK( visibleSymbol->storageClass == SC_TYPEDEF );

    if (globalSymbol != NULL)
    {
      if (globalSymbol->storageClass == SC_EXTERN || 
          globalSymbol->storageClass == SC_NONE)
      {
        sclass = SC_NONE;
      }
      else
      {
        // Redeclaration of this type:
        //   static int x;
        //   int x;
        TST,CompilerError( "'%s' already declared with a different storage class", Symbol_Name(sym) );
        ShowPrevDeclaration( globalSymbol );
        goto failFunc;
      }
    }
    break;

  case_SC_EXTERN:
  case SC_EXTERN:
    TST;SAME_SCOPE_CHECK( visibleSymbol->storageClass == SC_TYPEDEF );

    if (globalSymbol != NULL)
    {
      // It can't be any other storage class, since it isn't a typedef
      ASSERT( globalSymbol->storageClass == SC_NONE ||
              globalSymbol->storageClass == SC_EXTERN ||
              globalSymbol->storageClass == SC_STATIC );

      // copy the storage class of the previous declaration
      sclass = globalSymbol->storageClass;
    }
    break;

  case SC_STATIC:
    TST;SAME_SCOPE_CHECK( visibleSymbol->storageClass == SC_TYPEDEF );

    if (globalSymbol != NULL)
    {
      if (globalSymbol->storageClass != SC_STATIC)
      {
        // Redeclaration of this type:
        //   int x;
        //   static int x;
        TST,CompilerError( "'%s' already declared with a different storage class", Symbol_Name(sym) );
        ShowPrevDeclaration( globalSymbol );
        goto failFunc;
      }
    }
    break;

  case SC_TYPEDEF:
    // A typedef can only redefine a typedef (and only in non-strict mode)
    SAME_SCOPE_CHECK( visibleSymbol->storageClass != SC_TYPEDEF );

    // if we are redefining a typedef, issue a strict mode warning
    if (visibleSymbol != NULL && visibleSymbol->id.scope == CfeRoots.CurScope)
    {
      CompilerStrictError( "'%s': already declared in this scope", Symbol_Name(visibleSymbol) );  
      ShowPrevDeclaration( visibleSymbol );
    }

    // if this is a typedef redefinition
    // make sure the types are compatible and update the original
    // definition
    if (visibleSymbol != NULL)
    {
      if (!RedeclareGlobalSymbol( visibleSymbol, sym, bInit ))
        goto failFunc;

      // forget the new definition
      sym = visibleSymbol;
    }

    // Ignore global table. 
    globalSymbol = NULL;

    sduration = STORAGE_DURATION_NONE;
    // Note: typedefs do not get added to the global table, since they
    // can't be extern-ed
    break;

  default:
    CHECK_ASSUME( 0 );
  }

  // Check and fix things like functions returning functions or arrays
  sym->type = ValidateType( sym->type, sclass, bInit );

  sym->storageDuration = sduration;
  sym->storageClass    = sclass;
  sym->extDeclSpec     = dspecs->extDeclSpec;
  sym->declLoc         = yylloc;

  // Check and fix things like "void x;"
  ValidateDeclaration( sym, bInit );

  // Validate extDeclSpec
  CfeConf_ValidateExtDeclSpec( sym, bInit );

  if (IsDeclEmpty(sym))
  {
    TST,CompilerWarning( "Declaration does not declare anything" );
    goto failFunc;
  }

  if (globalSymbol != NULL)
  {
    // Update the declaration globalSymbol and forget _sym_
    if (!RedeclareGlobalSymbol( globalSymbol, sym, bInit ))
      goto failFunc;

    // Update the global symbol's storage class
    globalSymbol->storageClass = sclass;

    sym = globalSymbol; // forget _sym_
  }

  // Make it visible
  //
  if (!Symbol_IsVisible( sym )) // it could be visible if it is _globalSymbol_
    Scope_MakeSymbolVisible( CfeRoots.CurScope, sym );

  // All symbols except typedefs must be added to the global scope
  // ident table. We use it to keep track of global symbols when
  // externing them from a block scope.
  //
  if (sclass != SC_TYPEDEF && sym != globalSymbol)
    Scope_AddToIdentTable( CfeRoots.GlobalScope, sym );

  return sym;

failFunc:
  return NULL;
};

//--------------------------------------------------------------------------
// Name         DeclareBlockScope 
//
// 
//--------------------------------------------------------------------------
static TSymbol * DeclareBlockScope (
    const TDeclSpecs * dspecs, 
    TSymbol          * sym,
    bool             bInit
  )
{
  TSymbol * globalSymbol = NULL;     // global symbol which we are declaring with extern
  TSymbol * visibleSymbol = SymTab_LookupSymbol( sym->id.strName );
  StorageClass_t  sclass  = dspecs->sclass;
  StorageDuration_t sduration;

  switch (sclass)
  {
  case SC_NONE:
    // SC_NONE functions default to SC_EXTERN
    if (sym->type->tnode->typeKind == TYPE_FUNCTION)
    {
      sclass = SC_EXTERN;
      goto case_SC_EXTERN;
    }
    sclass = SC_AUTO;
    fall;

  case SC_STATIC:
  case SC_AUTO:
  case SC_REGISTER:
    if (sym->type->tnode->typeKind == TYPE_FUNCTION)
    {
      TST,CompilerError( "A block-scope function may only have extern storage" );
      // Fix the error
      sclass = SC_EXTERN;
      goto case_SC_EXTERN;
    }

    TST;SAME_SCOPE_CHECK( true );
    visibleSymbol = NULL;

    sduration = sclass == SC_STATIC ? STORAGE_DURATION_STATIC 
                                    : STORAGE_DURATION_AUTO;
    break;

  case_SC_EXTERN:
  case SC_EXTERN:
    TST;SAME_SCOPE_CHECK( visibleSymbol->storageClass != SC_EXTERN );

    // Keep track only of visible symbols in the current scope
    if (visibleSymbol != NULL && visibleSymbol->id.scope != CfeRoots.CurScope)
      visibleSymbol = NULL;

    if (sym->type->tnode->typeKind == TYPE_FUNCTION)
      sduration = STORAGE_DURATION_CODE;
    else
      sduration = STORAGE_DURATION_STATIC;

    // Locate a global symbol with this name
    globalSymbol = Scope_LookupSymbol( CfeRoots.GlobalScope, sym->id.strName );
    break;

  case SC_TYPEDEF:
    TST;SAME_SCOPE_CHECK( true );
    visibleSymbol = NULL;

    sduration = STORAGE_DURATION_NONE;
    break;

  default:
    CHECK_ASSUME( 0 );
  }

  // Check and fix things like functions returning functions or arrays
  sym->type = ValidateType( sym->type, sclass, bInit );

  sym->storageDuration = sduration;
  sym->storageClass    = sclass;
  sym->extDeclSpec     = dspecs->extDeclSpec;
  sym->declLoc         = yylloc;

  // Check and fix things like "void x;"
  ValidateDeclaration( sym, bInit );

  // Validate extDeclSpec
  CfeConf_ValidateExtDeclSpec( sym, bInit );

  if (IsDeclEmpty(sym))
  {
    TST,CompilerWarning( "Declaration does not declare anything" );
    goto failFunc;
  }

  if (visibleSymbol != NULL)
  {
    // This is a repeated extern for the same symbol in the same scope
    ASSERT( sclass == SC_EXTERN );

    if (!AreCompatibleTypes( visibleSymbol->type, sym->type, false, bInit ))
    {
      CompilerError( "'%s': type is incompatible with previous declaration", Symbol_Name(sym) ); 
      ShowPrevDeclaration( visibleSymbol );
      goto failFunc;
    }

    // sym is a redeclaration of visibleSymbol and possibly augments its type and/or 
    // declspecs. Update visibleSymbol with the new type.
    //

    // Combine the types
    visibleSymbol->type = CompositeType( visibleSymbol->type, sym->type, bInit );

    // Combine extDeclSpec and validate the result (to be safe)
    visibleSymbol->extDeclSpec = CfeConf_CompositeExtDeclSpec( visibleSymbol, sym->extDeclSpec );
    CfeConf_ValidateExtDeclSpec( visibleSymbol, bInit );

    // Update the declaration location
    visibleSymbol->declLoc = sym->declLoc;

    sym = visibleSymbol; // forget _sym_

    ASSERT( globalSymbol != NULL && globalSymbol == sym->declaresGlobal );
  }
  if (globalSymbol != NULL)
  {
    // sym is a redeclaration of globalSymbol and possibly augments its type and/or 
    // declspecs. Update globaleSymbol with the new type.
    //
    if (!RedeclareGlobalSymbol( globalSymbol, sym, bInit ))
      goto failFunc;

    // update _sym_ too with the updated type+declspec
    sym->type = globalSymbol->type;
    sym->extDeclSpec = globalSymbol->extDeclSpec;
  }

  // if we are declaring a global symbol, we must refer to it
  if (sclass == SC_EXTERN)
  {
    // create it if it doesn't exist
    if (globalSymbol == NULL)
    {
      globalSymbol = Symbol_Clone( sym );

      // and add it to the global ident table
      Scope_AddSymbol( CfeRoots.GlobalScope, globalSymbol );
      Scope_AddToIdentTable( CfeRoots.GlobalScope, globalSymbol );
    }

    ASSERT( sym->declaresGlobal == NULL || sym->declaresGlobal == globalSymbol );
    sym->declaresGlobal = globalSymbol;
  }
  else
    ASSERT( globalSymbol == NULL );

  if (!Symbol_IsVisible( sym ))
    Scope_MakeSymbolVisible( CfeRoots.CurScope, sym );
  else
  {
    // Only extern symbols can be redeclared
    ASSERT( sym->storageClass == SC_EXTERN );
  }

  return sym;

failFunc:
  return NULL;
};

//--------------------------------------------------------------------------
// Name         DeclareParamScope 
//
// 
//--------------------------------------------------------------------------
static TSymbol * DeclareParamScope (
    const TDeclSpecs * dspecs, 
    TSymbol          * sym,
    bool             bInit
  )
{
  TSymbol * visibleSymbol = SymTab_LookupSymbol( sym->id.strName );
  StorageClass_t  sclass  = dspecs->sclass;

  switch (sclass)
  {
  case SC_EXTERN:
  case SC_STATIC:
  case SC_AUTO:
  case SC_TYPEDEF:
    TST,CompilerError( "'%s': Invalid parameter storage class", Symbol_Name(sym) );

    // Fix the error only in cases which make sense (e.g. it doesn't make 
    // sense to ignore a "typedef", but it does for "auto")
    //
    if (sclass == SC_TYPEDEF || sclass == SC_EXTERN)
      goto failFunc;
    sclass = SC_NONE;
    fall;

  case SC_NONE:
  case SC_REGISTER:
    TST;SAME_SCOPE_CHECK( true );

    ASSERT( Scope_LookupSymbol( CfeRoots.CurScope, sym->id.strName ) == NULL );
    break;

  default:
    CHECK_ASSUME( 0 );
  }

  // Check and fix things like functions returning functions or arrays
  // We do it *before* adjustment of parameters!
  //
  sym->type = ValidateType( sym->type, sclass, bInit );

  sym->type = AdjustParameterType( sym->type );

  sym->storageDuration = STORAGE_DURATION_AUTO;
  sym->storageClass    = sclass;
  sym->extDeclSpec     = dspecs->extDeclSpec;
  sym->declLoc         = yylloc;

  // Validate extDeclSpec
  CfeConf_ValidateExtDeclSpec( sym, bInit );

  Scope_MakeSymbolVisible( CfeRoots.CurScope, sym );

  return sym;

failFunc:
  return NULL;
};

//--------------------------------------------------------------------------
// Name         DeclareStructScope 
//
// 
//--------------------------------------------------------------------------
static TSymbol * DeclareStructScope (
    const TDeclSpecs * dspecs, 
    TSymbol          * sym, 
    const TAst       * bitfield 
  )
{
  TSymbol * visibleSymbol = Scope_LookupSymbol( CfeRoots.CurScope, sym->id.strName );
  StorageClass_t  sclass  = dspecs->sclass;

  if (sym->type->tnode->typeKind == TYPE_FUNCTION)
  {
    TST,CompilerError( "'%s': function declaration not allowed in struct/union scope", Symbol_Name(sym) );
    goto failFunc;
  }

  switch (sclass)
  {
  case SC_EXTERN:
  case SC_STATIC:
  case SC_AUTO:
  case SC_REGISTER:
  case SC_TYPEDEF:
    TST,CompilerError( "'%s': Invalid storage class in struct/union scope", Symbol_Name(sym) );

    // Fix the error only in cases which make sense (e.g. it doesn't make 
    // sense to ignore a "typedef", but it does for "auto")
    //
    if (sclass == SC_TYPEDEF || sclass == SC_EXTERN)
      goto failFunc;

    sclass = SC_NONE;
    fall;

  case SC_NONE:
    if (visibleSymbol != NULL)
    {
      TST,SameScopeError( visibleSymbol );
      goto failFunc;
    }
    break;

  default:
    CHECK_ASSUME( 0 );
  }

  // Check and fix things like functions returning functions or arrays
  sym->type = ValidateType( sym->type, STORAGE_DURATION_MEMBER, false );

  // All types in a structure must be complete
  //
  if (!Type_IsComplete( sym->type ))
  {
    CompilerError( "'%s': uses incomplete type", Symbol_Name(sym) );
    sym->type = CfeRoots.ErrorType;
  }

  // Process bitfield
  //
  if (bitfield)
  {
    uint_fast16_t width = TARGET_INT_BIT; // default value in case of error
    bool bErrorType;

    bErrorType = Type_IsError( sym->type );

    if (!bErrorType && !Type_IsInteger(sym->type))
    {
      TST,CompilerError( "'%s': bitfield requires integer type", Symbol_Name(sym) );

      // Fix the error as best as we can
      sym->type = CfeRoots.Int_Type;
    }

    bitfield = ConstantIntegerExpression( &yylloc, bitfield, "bitfield" );

    if (!bErrorType && bitfield->code != ast_error)
    {
      TARGET_IMAX_T maxW;

      // Default width is the maximum width
      width = bitfield->exprType->tnode->u.pIntDesc->pMachType->precision;
      TARGET_ASSIGN_SMALL( &maxW, width );

      // Check for negative
      //
      if (bitfield->exprType->tnode->u.pIntDesc->isSigned &&
          TARGET_LT_SIGNED( &bitfield->u.intConst, &Const0 ))
      {
        TST,CompilerError( "'%s': negative bitfield width", Symbol_Name(sym) );
      }
      else  
      // Check for too large value
      //
      if (TARGET_GT_UNSIGNED( &bitfield->u.intConst, &maxW ))
      {
        TST,CompilerError( "'%s': too large bitfield width", Symbol_Name(sym) );
      }
      else
        width = SMALL_FROM_TARGET( &bitfield->u.intConst );

      // width 0 is allowed only on anonymous bitfield
      if (width == 0 && sym->id.strName != NULL)
      {
        CompilerError( "'%s': zero-width bitfield must be unnamed", Symbol_Name(sym) );
        // fix the error by making it unnamed
        sym->id.strName = NULL;
      }
    }

    sym->bBitfield = true;
    sym->bitfieldWidth = (uint_least16_t)width;
  }

  sym->storageDuration  = STORAGE_DURATION_MEMBER;
  sym->storageClass     = sclass;
  sym->extDeclSpec      = dspecs->extDeclSpec;
  sym->declLoc          = yylloc;

  // Validate extDeclSpec
  CfeConf_ValidateExtDeclSpec( sym, false );

  // except for bitfields, members must be named
  if (sym->id.strName == NULL && bitfield == NULL)
  {
    TST,CompilerError( "Declaration does not declare anything" );
    goto failFunc;
  }

  Scope_AddSymbol( CfeRoots.CurScope, sym );
  Scope_AddToIdentTable( CfeRoots.CurScope, sym );

  return sym;

failFunc:
  return NULL;
};


//--------------------------------------------------------------------------
// Name         Declare
//
// Returns the declared (which may be different from _sym_ since this might
// be a redeclaration), or NULL if there is an error that can't be fixed
// meaningfully.
//--------------------------------------------------------------------------
TSymbol * Declare ( 
    const TDeclSpecs  * dspecs, 
    TSymbol           * sym, 
    const TAst        * bitfield,
    bool              bInit
  )
{
  switch (CfeRoots.CurScope->disposition)
  {
  case SCOPE_FILE:
    sym = DeclareFileScope( dspecs, sym, bInit );
    break;

  case SCOPE_BLOCK: 
    sym = DeclareBlockScope( dspecs, sym, bInit );
    break;

  case SCOPE_PARAMS:
    sym = DeclareParamScope( dspecs, sym, bInit );
    break;

  case SCOPE_STRUCT:
    ASSERT( bInit == false );
    sym = DeclareStructScope( dspecs, sym, bitfield );
    break;                

  default:
    CHECK_ASSUME( 0 );
  }

  return sym;
};


//--------------------------------------------------------------------------
// Name         DeclareTag 
//
// 
//--------------------------------------------------------------------------
const TTypeNode * DeclareTag ( 
    int    token,    // STRUCT/UNION
    STRING name,
    bool   bDefining // the struct is being defined
  )
{
  const TTypeNode * tag;
  TypeKind_t  kind;
  TScope    * declScope; // scope where the declaration occurs

  // Deterime the tag kind
  //
  switch (token)
  {
  case STRUCT: kind = TYPE_STRUCT; break;
  case UNION:  kind = TYPE_UNION; break;
  case ENUM:   kind = TYPE_ENUM; break;
  default:
    CHECK_ASSUME( 0 );
    break;
  }

  // Find the first non-struct active scope in which we will declare the tag
  for ( declScope = CfeRoots.CurScope; declScope->disposition == SCOPE_STRUCT; 
        declScope = declScope->parentScope )
  {
    ASSERT( declScope != NULL );
  }

  // Lookup a visible tag with the same name
  tag = SymTab_LookupTag( name ); 

  if (bDefining)
  {
    if (declScope->disposition == SCOPE_PARAMS)
      CompilerWarning( "'%s': %s declaration in parameter scope", 
                       STRING_Name(name), TypeKind_Name[kind] );

    // When defining, ignore tags not in current scope
    if (tag != NULL && tag->id.scope != declScope)
      tag = NULL;

    // If we are defining a tag which has already been defined in the 
    // current scope
    if (tag != NULL && tag->bDefined)
    {
      TST,CompilerError( "'%s': %s is already defined", 
                         STRING_Name(name), TypeKind_Name[tag->typeKind] );

      // Fix the error by ignoring the previous declaration and creating 
      // a dummy unnamed tag
      //
      tag = NULL;
      name = NULL;
    }
  }

  // Verify the tag kind
  if (tag != NULL && tag->typeKind != kind)
  {
    TST,CompilerError( "'%s': invalid redeclaration of %s", 
                       STRING_Name(name), TypeKind_Name[tag->typeKind] );

    // Fix the error by ignoring the previous declaration and creating 
    // a dummy unnamed tag
    //
    tag = NULL;
    name = NULL;
  }

  // If it doesn't exist, create a tag
  if (tag == NULL)
  {
    // forward declaration of enum is not allowed
    if (token == ENUM && !bDefining)
    {
      CompilerError( "'%s': forward declarations of enum are not allowed", STRING_Name(name) );
      // Fix the error by simply using int
      tag = CfeRoots.Int_Type->tnode;
    }
    else
    {
      TTypeNode * rtag;

      if (declScope->disposition == SCOPE_PARAMS)
        CompilerWarning( "'%s': struct declaration in parameter scope", STRING_Name(name) );

      tag = rtag = New_Tag( kind, name );
      rtag->bDefined  = bDefining;

      Scope_MakeTagVisible( declScope, rtag );
    }
  }

  return tag;
};


//--------------------------------------------------------------------------
// Name         DeclareEnumerator 
//
// 
//--------------------------------------------------------------------------
void DeclareEnumerator ( 
    TEnumDesc    * edesc,
    STRING    name,
    const TAst   * value
  )
{
  // enumerators assume the scope of the tag
  TScope  * declScope     = edesc->tag->id.scope;
  TSymbol * sym;

  // Check for re-declaration
  {
    TSymbol * visibleSymbol = SymTab_LookupSymbol( name );
    if (visibleSymbol != NULL && visibleSymbol->id.scope == declScope)
    {
      SameScopeError( visibleSymbol );
      // Fix the error by creating an anonymous symbol
      name = NULL;
    }
  }

  sym = New_TSymbol( name, edesc->type );
  sym->storageDuration = STORAGE_DURATION_NONE;
  sym->storageClass    = SC_NONE;
  sym->declLoc         = yylloc;

  if (value != NULL)
  {
    value = ConstantIntegerExpression( &yylloc, value, "enumerator" );

    if (value->code == ast_error)
    {
      // In case of error use 0
      sym->initValue = Ast_intConst( NULL, sym->type, Const0 );
    }
    else
    {
      TODO1( "convert to int" );
      sym->initValue = Ast_intConst( &value->loc, sym->type, value->u.intConst );
    }
  }
  else
  {
    // if no value is provided, use the current value
    sym->initValue = Ast_intConst( &yylloc, sym->type, edesc->curVal );
  }

  // increment the enumeration value and keep it within integer range
  //
  TARGET_AND( 
    &edesc->curVal,
    TARGET_ADD_SIGNED( &edesc->curVal, &sym->initValue->u.intConst, &Const1 ),
    &Target_UIntMax_Max
  );

  Scope_MakeSymbolVisible( declScope, sym );

  // if declaring in file scope, we must add the enumerator to the global
  // table
  if (declScope->disposition == SCOPE_FILE)
    Scope_AddToIdentTable( declScope, sym );
};

//--------------------------------------------------------------------------
// Name         DefineEnum 
//
// Complete an enum definition.
//--------------------------------------------------------------------------
const TTypeNode * DefineEnum ( TEnumDesc * edesc )
{
  TODO_SAFE( "Define the integer properties of the type depending on the range" );

  // for now simply use int. Note it is safe to modify it, because it has been
  // allocated specifically for this purpose.
  //
  MODIFY_TNODE(edesc->tag)->u.pIntDesc = &Int_Desc;

  return edesc->tag;
};

//--------------------------------------------------------------------------
// Name         DeclareParamIdent 
//
// Declares a parameter in a identifier list (old style functions)
//--------------------------------------------------------------------------
void DeclareParamIdent ( STRING name )
{
  TSymbol * sym;
  ASSERT( CfeRoots.CurScope->disposition == SCOPE_IDENT_LIST );

  // 6.9.1-6 [...] An identifier declared as a typedef name shall not be redeclared 
  // as a parameter.
  if (NULL != (sym = SymTab_LookupSymbol( name )) && 
      sym->storageClass == SC_TYPEDEF)
  {
    CompilerError( "'%s': Old-style parameter cannot redeclare a typedef", STRING_Name(name) );
    goto exitFunc;
  }

  // Look for a duplicate
  if (Scope_LookupSymbol( CfeRoots.CurScope, name ) != NULL)
  {
    CompilerError( "'%s': repeated identifier", STRING_Name(name) );
    goto exitFunc;
  }

  // allocate a new symbol
  sym = New_TSymbol( name, NULL );
  sym->declLoc = yylloc;

  // add it to the scope
  Scope_AddSymbol( CfeRoots.CurScope, sym );
  Scope_AddToIdentTable( CfeRoots.CurScope, sym );

exitFunc:
  ;
};

//--------------------------------------------------------------------------
// Name         DeclareEllipsis 
//
// 
//--------------------------------------------------------------------------
void DeclareEllipsis ( void )
{
  TSymbol * sym;

  ASSERT( CfeRoots.CurScope->disposition == SCOPE_PARAMS );

  // Note: we want the "...' to have a non-NULL type, because we will never
  // have to check sym->type against NULL
  //
  sym = New_TSymbol( CfeRoots.EllipsisName, CfeRoots.EllipsisType );
  sym->declLoc = yylloc;
  sym->storageDuration = STORAGE_DURATION_AUTO;

  Scope_AddSymbol( CfeRoots.CurScope, sym );

  ASSERT( !CfeRoots.CurScope->bHaveEllipsis );
  CfeRoots.CurScope->bHaveEllipsis = true;
};


//--------------------------------------------------------------------------
// Name         DeclareIdentList 
//
// Validates old-style parameter definition and converts to a new style
// parameter scope.
//
//   paramDecl is a scope containing the parameter declarations
//   identList is the list of (typeless) identifiers in the function declaration
//
// On exit identList will be converted to a new-style parameter list.
// Returns the new identList scope.
//
// Note: we are free to modify identList in place, because we are sure that
// it can't belong to a typedef - it belongs to the current declaration.
//--------------------------------------------------------------------------
static TScope * DeclareIdentList ( 
    TScope * identList, // identifier list
    TScope * paramDecl  // parameter declaration
  )
{
  TSymbol * sym;

  if (paramDecl != NULL)
  {
    // Validate the parameters
    //
    for ( sym = paramDecl->pFirstSymbol; 
          sym != NULL; sym = (TSymbol *)sym->id.pNext )
    {
      ValidateParameter( sym, true );

      // Find the parameter in the identifier list. If the name is NULL,
      // the error has already been reported by ValidateParameter()
      //
      if (sym->id.strName != NULL)
      {
        TSymbol * idSym;

        if (identList == NULL ||
            NULL == (idSym = Scope_LookupSymbol( identList, sym->id.strName )) )
        {
          TST,CompilerErrorLoc( &sym->declLoc, "'%s': is not a parameter", Symbol_Name(sym) ); 
        }
        else
        {
          ASSERT( !idSym->bIdentDefined );

          // Convert the ident to a full-fledged parameter
          //
          Symbol_Copy( idSym, sym );

          idSym->bIdentDefined = true;
        }
      }
    }
  }

  // Allocate the ident list if missing
  //
  if (identList == NULL)
    identList = New_TScope( NULL, SCOPE_PARAMS );
  else
    identList->disposition = SCOPE_PARAMS; // Convert it to a parameter scope

  // if it is empty, add ( void ) signifying new-style 0-params
  if (identList->pFirstSymbol == NULL)
    Scope_AddSymbol( identList, New_TSymbol( NULL, CfeRoots.VoidType ) );
  else
  {
    // Make sure all the idents in the ident list have been declared
    //
    for ( sym = identList->pFirstSymbol; 
          sym != NULL; sym = (TSymbol *)sym->id.pNext )
    {
      if (!sym->bIdentDefined)
      {
        TST,CompilerErrorLoc( &sym->declLoc, "'%s': parameter was not defined", Symbol_Name(sym) );

        sym->type = CfeRoots.ErrorType;
        sym->storageClass = SC_AUTO;
        sym->storageDuration = STORAGE_DURATION_AUTO;
      }
    }
  }

  return identList;
};

//--------------------------------------------------------------------------
// Name         DefineFunctionParameters 
//
// Inserts all parameters in a function scope and makes them visible.
// 
// Q: Why is this function necessary ?
// A: The parameters are kept in a scope of their own in the function type. 
// However when compiling the function body, these parameters need to be
// accessible in a natural way.
//--------------------------------------------------------------------------
static void DefineFunctionParameters ( TScope * funcScope, TScope * params )
{
  TSymbol * sym;

  if (params == NULL) // ignore empty scopes
    return;

  sym = params->pFirstSymbol;

  // check for the special case of (void)
  if (sym->id.pNext == NULL &&     // the only symbol
      sym->id.strName == NULL &&   // unnamed
      Type_IsVoid(sym->type))      // of void type
  {
    // function without parameters
    return;
  }

  // Copy all parameters and insert them in the scope
  //
  for ( ; sym != NULL; sym = (TSymbol *)sym->id.pNext )
  {
    TSymbol * clone;

    if (sym->id.strName == NULL) // ignore unnamed parameters
      continue;

    clone = Symbol_Clone( sym );

    // Add the new symbol to the scope and make it visible at the same time
    Scope_MakeSymbolVisible( funcScope, clone );
  }
}

//--------------------------------------------------------------------------
// Name         DeclareFunctionDefinition 
//
// 
//--------------------------------------------------------------------------
void DeclareFunctionDefinition ( 
    const TDeclSpecs * dspecs, 
    const TQualNode  * dspecsType,
    TSymbol          * funcSym,
    TScope           * identDeclList
  )
{
  TScope * funcScope; // the function scope

  if (funcSym->type->tnode->typeKind != TYPE_FUNCTION)
  {
    TST,CompilerError( "'%s': does not have function type in function definition", Symbol_Name(funcSym) );

    // Recover from the error by declaring a dummy function
    funcSym = New_TSymbol( funcSym->id.strName, Type_MakeFunction( CfeRoots.ErrorType, NULL ) );
  }
  else
  if (funcSym->type == dspecsType)
  {
    TST,CompilerError( "'%s': function type in function definition cannot come from a typedef", Symbol_Name(funcSym) );

    // Recover from the error by declaring a dummy function
    funcSym = New_TSymbol( funcSym->id.strName, Type_MakeFunction( CfeRoots.ErrorType, NULL ) );
  }

  // Check for mixed old-style/new-style declaration
  //
  if (identDeclList != NULL && !Scope_IsIdentList( funcSym->type->tnode->u.memberScope ))
  {
    TST,CompilerError( "'%s': mixing old-style and new-style function declaration", Symbol_Name(funcSym) );
    identDeclList = NULL;
  }

  // Convert the old-style parameters to new style. If we have an ident declaration list,
  // or we have an ident list in the function type
  //
  if (identDeclList != NULL || Scope_IsIdentList( funcSym->type->tnode->u.memberScope ))
  {
    // We can afford to modyft the _tnode_ in place, because the function type
    // doesn't come from a typedef, so it isn't shared
    //
    MODIFY_TNODE(funcSym->type->tnode)->u.memberScope = 
      DeclareIdentList( funcSym->type->tnode->u.memberScope, identDeclList );
  }

  funcSym = Declare( dspecs, funcSym, NULL, true );

  if (funcSym == NULL) // Recover from possible error by declaring a dummy function
    funcSym = New_TSymbol( NULL, Type_MakeFunction( CfeRoots.ErrorType, NULL ) );

  // Save the type at the moment of definition
  funcSym->defType = funcSym->type;

  // Declare the function scope
  //
  funcScope = SymTab_PushNewScope( SCOPE_BLOCK );

  // Declare the parameters inside the function scope
  DefineFunctionParameters( funcScope, funcSym->type->tnode->u.memberScope );
}



