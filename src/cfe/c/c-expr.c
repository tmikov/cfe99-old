#include "precomp.h"
#include "cfe_priv.h"
#include "c-expr.h"


/* 6.3.1.1 Boolean, characters, and integers

2 The following may be used in an expression wherever an int or unsigned int may
be used:

  — An object or expression with an integer type whose integer conversion rank is less
than the rank of int and unsigned int.

  — A bit-field of type _Bool, int, signed int, or unsigned int.
If an int can represent all values of the original type, the value is converted to an int;
otherwise, it is converted to an unsigned int. These are called the integer
promotions.48) All other types are unchanged by the integer promotions.

3 The integer promotions preserve value including sign. As discussed earlier, whether a
‘‘plain’’ char is treated as signed is implementation-defined.
*/
//--------------------------------------------------------------------------
// Name         TYPE_IntegerPromotion 
//
// 
//--------------------------------------------------------------------------
const TQualNode * TYPE_IntegerPromotion ( const TQualNode * typ )
{
  const TIntegerTypeDesc * typDesc = typ->tnode->u.pIntDesc;

  ASSERT( Type_IsInteger( typ ) );

  if (typDesc->rank < Int_Desc.rank)
  {
    // Check if it fits in Int; if not convert it to UInt
    // prec < IntPrec || prec == IntPrec && signed
    //
    typ = 
      typDesc->pMachType->precision < Int_Desc.pMachType->precision ||
        typDesc->pMachType->precision == Int_Desc.pMachType->precision && 
        typDesc->isSigned
        ? CfeRoots.Int_Type : CfeRoots.UInt_Type;
  }

  return typ;
};

//--------------------------------------------------------------------------
// Name         AST_IntegerPromotion 
//
// 
//--------------------------------------------------------------------------
TAst * AST_IntegerPromotion ( TAst * ast )
{
  const TQualNode * newTyp;

  ASSERT( Type_IsInteger( ast->exprType ) );

  // if the type has changed, convert
  if ( (newTyp = TYPE_IntegerPromotion( ast->exprType )) != ast->exprType)
    ast = Ast_cast( &ast->loc, newTyp, ast );

  return ast;
};


/* 6.3.1.8 Usual arithmetic conversions

1 Many operators that expect operands of arithmetic type cause conversions and yield result
types in a similar way. The purpose is to determine a common real type for the operands
and result. For the specified operands, each operand is converted, without change of type
domain, to a type whose corresponding real type is the common real type. Unless
explicitly stated otherwise, the common real type is also the corresponding real type of
the result, whose type domain is the type domain of the operands if they are the same,
and complex otherwise. This pattern is called the usual arithmetic conversions:

  First, if the corresponding real type of either operand is long double, the other
operand is converted, without change of type domain, to a type whose
corresponding real type is long double.

  Otherwise, if the corresponding real type of either operand is double, the other
operand is converted, without change of type domain, to a type whose
corresponding real type is double.

  Otherwise, if the corresponding real type of either operand is float, the other
operand is converted, without change of type domain, to a type whose
corresponding real type is float.51)

  Otherwise, the integer promotions are performed on both operands. Then the
following rules are applied to the promoted operands:
        If both operands have the same type, then no further conversion is needed.

        Otherwise, if both operands have signed integer types or both have unsigned
      integer types, the operand with the type of lesser integer conversion rank is
      converted to the type of the operand with greater rank.

        Otherwise, if the operand that has unsigned integer type has rank greater or
      equal to the rank of the type of the other operand, then the operand with
      signed integer type is converted to the type of the operand with unsigned
      integer type.

        Otherwise, if the type of the operand with signed integer type can represent
      all of the values of the type of the operand with unsigned integer type, then
      the operand with unsigned integer type is converted to the type of the
      operand with signed integer type.

        Otherwise, both operands are converted to the unsigned integer type
      corresponding to the type of the operand with signed integer type.

2 The values of floating operands and of the results of floating expressions may be
represented in greater precision and range than that required by the type; the types are not
changed thereby.52)
*/
//--------------------------------------------------------------------------
// Name         UsualArithmeticConversion 
//
// Note: this function doesn't check for or report any errors.
// The caller must have validated that the types of the operands are valid.
//--------------------------------------------------------------------------
void UsualArithmeticConversion ( TAst * * pOp1, TAst * * pOp2 )
{
  TAst * * tmp;
  bool bFloating = false;

  ASSERT( Type_IsArithmetic( (*pOp1)->exprType ) );
  ASSERT( Type_IsArithmetic( (*pOp2)->exprType ) );

  // Determine if the result will be a floating type and at the same
  // time put the "greater" floating operand first
  //
  if (Type_IsFloating( (*pOp2)->exprType))
  {
    bFloating = true;

    if (!Type_IsFloating( (*pOp1)->exprType ) ||
        (*pOp2)->exprType->tnode->u.realType > (*pOp1)->exprType->tnode->u.realType)
    {
      tmp = pOp1;
      pOp1 = pOp2;
      pOp2 = tmp;
    }
  }
  else
  if (Type_IsFloating( (*pOp1)->exprType ))
    bFloating = true;

  if (bFloating)
  {
    const TTypeNode * tn1, * tn2;

    tn1 = (*pOp1)->exprType->tnode;
    tn2 = (*pOp2)->exprType->tnode;

    if (tn1->u.realType != tn2->u.realType)
    {
      // Note: we cast the type (float, double, ldouble), but we preserve the
      // type domain ( real,imaginary,complex)
      //
      *pOp2 = Ast_cast( 
                &(*pOp2)->loc, CfeRoots.FloatingTypes[tn2->typeKind][tn1->u.realType], *pOp2
              );
    }
  }
  else
  {
    const TIntegerTypeDesc * d1, * d2;

    *pOp1 = AST_IntegerPromotion( *pOp1 );
    *pOp2 = AST_IntegerPromotion( *pOp2 );

    // Put the higher rank first
    if ((*pOp2)->exprType->tnode->u.pIntDesc->rank > (*pOp1)->exprType->tnode->u.pIntDesc->rank)
    {
      tmp = pOp1;
      pOp1 = pOp2;
      pOp2 = tmp;
    }

    d1 = (*pOp1)->exprType->tnode->u.pIntDesc;
    d2 = (*pOp2)->exprType->tnode->u.pIntDesc;
 
    /*
      The straight-foward encoding of the rules yields code like this:
        if (d1 == d2)
           do nothing no conversion if same type;
        else
        if (d1->isSigned == d2->isSigned)
          convert 2 to 1;
        else
        if (!d1->isSigned) // 1 is unsigned, 2 is signed
          convert 2 to 1;
        else // 1 is signed, 2 is unsigned
        if (d1->pMachType->precision > d2->pMachType->precision)
          convert 2 to 1;
        else // 1 is signed, 2 is unsigned, 1 doesn't have greater precision
        {
          convert 1 to (unsigned)1;
          convert 2 to (unsigned)1;
        }

      It is easy to see that if the types are different, the second is always
      converted to the first. Thus, we come with the optmization below,
      which actually makes sense when read on its own.
    */
    if (d1 != d2)
    {
      if (d1->isSigned && !d2->isSigned && 
          d1->pMachType->precision <= d2->pMachType->precision)
      {
        *pOp1 = Ast_cast( &(*pOp1)->loc, CfeRoots.IntegerTypes[d1->brother], *pOp1 );
      }

      // op2 might already have the type (e.g. 1 + 2u)
      if (d2 != (*pOp1)->exprType->tnode->u.pIntDesc)
        *pOp2 = Ast_cast( &(*pOp2)->loc, (*pOp1)->exprType, *pOp2 );
    }
  }
};

#define CHECK_ERROR( op )   if (Ast_isError( op )) return op;

/*
6.3.2.1 Lvalues, arrays, and function designators

2 Except when it is the operand of the sizeof operator, the unary & operator, the ++
operator, the -- operator, or the left operand of the . operator or an assignment operator,
an lvalue that does not have array type is converted to the value stored in the designated
object (and is no longer an lvalue). If the lvalue has qualified type, the value has the
unqualified version of the type of the lvalue; otherwise, the value has the type of the
lvalue. If the lvalue has an incomplete type and does not have array type, the behavior is
undefined.

3 Except when it is the operand of the sizeof operator or the unary & operator, or is a
string literal used to initialize an array, an expression that has type ‘‘array of type’’ is
converted to an expression with type ‘‘pointer to type’’ that points to the initial element of
the array object and is not an lvalue. If the array object has register storage class, the
behavior is undefined.
*/

//--------------------------------------------------------------------------
// Name         Expr_GetRValue 
//
// Removes qualifiers.
//
// symbol -> fetch symbol
// deref op -> fetch op
//--------------------------------------------------------------------------
TAst * Expr_GetRValue ( TAst * op )
{
  TAst * ast;

  CHECK_ERROR( op );

  TODO1( "convert functions to pointers" );

  switch (op->code)
  {
  case ast_symbol:  
    if (op->exprType->tnode->typeKind == TYPE_ARRAY) // Convert arrays to pointers
    {
      // Get the address of the array and typecast it to a pointer to an array element
      //
      ast = Ast_cast( 
        &op->loc, 
        Type_MakePointer( op->exprType->tnode->of, NULL ),
        Expr_addr( &op->loc, op )
      );
    }
    else
    if (op->exprType->tnode->typeKind == TYPE_FUNCTION) // Convert functions to pointers
    {
      ast = Expr_addr( &op->loc, op ); // addr op
    }
    else
    {
      // Convert a symbol to an r-value by fetching its value
      // symbol( x ) -> fetch( symbol( x ) )
      // The result type is unqualified
      //
      ast = Ast( ast_fetch, &op->loc );
      ast->child = op;
      ast->exprType = Type_Unqual( op->exprType );   
    }
    return ast;

  case ast_deref:  
    if (op->exprType->tnode->typeKind == TYPE_ARRAY) // Convert arrays to pointers
    {
      // Ignore the dereference and convert the pointer to a pointer to an array element
      //
      ast = Ast_cast(
        &op->loc, 
        Type_MakePointer( op->exprType->tnode->of, NULL ),
        op->child
      );
    }
    else
    {
      // Convert a dereferance to r-value by fetching the value of the operand.
      // deref( x ) -> fetch( x )
      // The result type is unqualified
      //
      ast = Ast( ast_fetch, &op->loc );
      ast->child = op->child;
      ast->exprType = Type_Unqual( op->exprType );   
    }
    return ast;
  }

  return op; 
};

/*
6.3.2.1 Lvalues, arrays, and function designators
1 An lvalue is an expression with an object type or an incomplete type other than void;53)
if an lvalue does not designate an object when it is evaluated, the behavior is undefined.
When an object is said to have a particular type, the type is specified by the lvalue used to
designate the object. A modifiable lvalue is an lvalue that does not have array type, does
not have an incomplete type, does not have a const-qualified type, and if it is a structure
or union, does not have any member (including, recursively, any member or element of
all contained aggregates or unions) with a const-qualified type.
*/
//--------------------------------------------------------------------------
// Name         Expr_IsLvalue 
//
// 
//--------------------------------------------------------------------------
static bool Expr_IsLvalue ( const TAst * ast )
{
  switch (ast->code)
  {
  case ast_deref:
    break;

  case ast_symbol:
    ASSERT( ast->u.sym->storageDuration != STORAGE_DURATION_NONE );
    break;

  default:
    return false;
  }

  return !Type_IsVoid( ast->exprType );
}

//--------------------------------------------------------------------------
// Name         Expr_IsModifiableLvalue 
//
// 
//--------------------------------------------------------------------------
static bool Expr_IsModifiableLvalue ( const TAst * ast )
{
  if (!Expr_IsLvalue( ast ))
    return false;

  TODO1( "check for const members of structs" );

  return
    ast->exprType->tnode->typeKind != TYPE_ARRAY &&
    !ast->exprType->isConst &&
    Type_IsComplete( ast->exprType );
}

/*
6.5.1.2 An identifier is a primary expression, provided it has been declared as designating an
object (in which case it is an lvalue) or a function (in which case it is a function
designator).76)

76) Thus, an undeclared identifier is a violation of the syntax.
*/

//--------------------------------------------------------------------------
// Name         Expr_ident 
//
// 
//--------------------------------------------------------------------------
TAst * Expr_ident ( const YYLTYPE * loc, STRING str )
{
  TSymbol * sym;
  TAst * ast;

  sym = SymTab_LookupSymbol( str );
  if (sym == NULL)
  {
    CompilerErrorLoc( loc, "'%s': undefined identifier", STRING_Name(str) );
    return Ast_error( loc );
  }
  else
  if (Type_IsError( sym->type ))
    return Ast_error( loc );

  // Is this an enumeration constant ?
  if (sym->storageDuration == STORAGE_DURATION_NONE)
  {
    TODO1( "A better way to detect enum constants" );
    ASSERT( sym->initValue != NULL );
    ast = Ast_intConst( loc, sym->type, sym->initValue->u.intConst );
  }
  else
  {
    ast = Ast( ast_symbol, loc );
    ast->u.sym = sym;
    ast->exprType = sym->type;
  }

  return ast;
}

/*
6.5.3.2 Address and indirection operators

Constraints
2 The operand of the unary * operator shall have pointer type.

Semantics
4 The unary * operator denotes indirection. If the operand points to a function, the result is
a function designator; if it points to an object, the result is an lvalue designating the
object. If the operand has type ‘‘pointer to type’’, the result has type ‘‘type’’. If an
invalid value has been assigned to the pointer, the behavior of the unary * operator is
undefined.83)
*/

//--------------------------------------------------------------------------
// Name         Expr_deref 
//
// 
//--------------------------------------------------------------------------
TAst * Expr_deref ( const YYLTYPE * loc, TAst * op )
{
  TAst * ast;

  op = Expr_GetRValue( op );
  CHECK_ERROR( op );

  if (op->exprType->tnode->typeKind != TYPE_POINTER)
  {
    CompilerErrorLoc( loc, "'*': operand doesn't have pointer type" );
    return Ast_error( loc );
  }

  // deref( addr( op ) ) => op
  //
  if (op->code == ast_addr)
  {
    ASSERT( op->exprType->tnode->of == op->child->exprType );
    return op->child;
  }

  ast = Ast( ast_deref, loc );
  ast->child = op;
  ast->exprType = op->exprType->tnode->of;
  
  return ast;
};

/*
6.5.3.2 Address and indirection operators

Constraints
1 The operand of the unary & operator shall be either a function designator, the result of a
[] or unary * operator, or an lvalue that designates an object that is not a bit-field and is
not declared with the register storage-class specifier.

Semantics
3 The unary & operator returns the address of its operand. If the operand has type ‘‘type’’,
the result has type ‘‘pointer to type’’. If the operand is the result of a unary * operator,
neither that operator nor the & operator is evaluated and the result is as if both were
omitted, except that the constraints on the operators still apply and the result is not an
lvalue. Similarly, if the operand is the result of a [] operator, neither the & operator nor
the unary * that is implied by the [] is evaluated and the result is as if the & operator
were removed and the [] operator were changed to a + operator. Otherwise, the result is
a pointer to the object or function designated by its operand.
*/

//--------------------------------------------------------------------------
// Name         Expr_addr 
//
// 
//--------------------------------------------------------------------------
TAst * Expr_addr ( const YYLTYPE * loc, TAst * op )
{
  TAst * ast;

  CHECK_ERROR( op );

  if (!Expr_IsLvalue( op ))
  {
    CompilerErrorLoc( loc, "'&': operand is not an l-value" );
    return Ast_error( loc );
  }

  if (op->code == ast_deref)  // "&*" ?
  {
    ASSERT( op->exprType == op->child->exprType->tnode->of );
    return op->child; // The result is as if "&*" was omitted.
  }

  if (op->code == ast_symbol)
  {
    // Must not be bit-field
    if (op->u.sym->bBitfield)
    {
      CompilerErrorLoc( loc, "'&': Can't take the address of a bitfield" );
      return Ast_error( loc );
    }
    // Must not be register storage class
    if (op->u.sym->storageClass == SC_REGISTER)
    {
      CompilerErrorLoc( loc, "'&': Can't take the address of a register variable" );
      return Ast_error( loc );
    }
  }

  TODO1( "int _far x;  &x -> int _far *" );

  ast = Ast( ast_addr, loc );
  ast->child = op;
  ast->exprType = Type_MakePointer( op->exprType, NULL );

  return ast;
}

//--------------------------------------------------------------------------
// Name         Expr_cast 
//
// 
//--------------------------------------------------------------------------
TAst * Expr_cast ( const YYLTYPE * loc, const TQualNode * toType, TAst * expr )
{
  TODO1( "Everything" );
  return Ast_cast( loc, toType, expr );
}

/*
6.5.10 Bitwise AND operator
Syntax
1 AND-expression:
     equality-expression
     AND-expression & equality-expression

Constraints
2 Each of the operands shall have integer type.

Semantics
3 The usual arithmetic conversions are performed on the operands.

4 The result of the binary & operator is the bitwise AND of the operands (that is, each bit in
the result is set if and only if each of the corresponding bits in the converted operands is
set).
*/
//--------------------------------------------------------------------------
// Name         Expr_intOp
//
// Preform an integer operation on two operands. 
// This is for operarions that conform to the of rules of "and" ("or" and "xor")
//--------------------------------------------------------------------------
static TAst * Expr_intOp ( 
    const YYLTYPE * loc, AstCode_t code, const char * szName, TAst * l, TAst * r  
    )
{
  ASSERT( l && r );

  l = Expr_GetRValue( l );
  r = Expr_GetRValue( r );

  CHECK_ERROR( l );
  CHECK_ERROR( r );

  // 2 Each of the operands shall have integer type.
  //
  if (!Type_IsInteger( l->exprType ) && !Type_IsInteger( r->exprType ))
  {
    CompilerErrorLoc( loc, "'%s' - operand is not integer", szName );
    return Ast_error( loc );
  }

  UsualArithmeticConversion( &l, &r );

  return Ast_binOp( code, loc, l->exprType, l, r );
};

//--------------------------------------------------------------------------
// Name         Expr_binAnd
//
// 
//--------------------------------------------------------------------------
TAst * Expr_binAnd ( const YYLTYPE * loc, TAst * l, TAst * r )
{
  return Expr_intOp( loc, ast_binAnd, "&", l, r );
};

//--------------------------------------------------------------------------
// Name         Expr_binOr
//
// 
//--------------------------------------------------------------------------
TAst * Expr_binOr ( const YYLTYPE * loc, TAst * l, TAst * r )
{
  return Expr_intOp( loc, ast_binOr, "|", l, r );
};

//--------------------------------------------------------------------------
// Name         Expr_binXor
//
// 
//--------------------------------------------------------------------------
TAst * Expr_binXor ( const YYLTYPE * loc, TAst * l, TAst * r )
{
  return Expr_intOp( loc, ast_binXor, "^", l, r );
};



/*
6.5.5 Multiplicative operators

Syntax
  1 multiplicative-expression:
        cast-expression
        multiplicative-expression * cast-expression
        multiplicative-expression / cast-expression
        multiplicative-expression % cast-expression

Constraints

2 Each of the operands shall have arithmetic type. The operands of the % operator shall
have integer type.

Semantics

3 The usual arithmetic conversions are performed on the operands.

4 The result of the binary * operator is the product of the operands.

5 The result of the / operator is the quotient from the division of the first operand by the
second; the result of the % operator is the remainder. In both operations, if the value of
the second operand is zero, the behavior is undefined.

6 When integers are divided, the result of the / operator is the algebraic quotient with any
fractional part discarded.87) If the quotient a/b is representable, the expression
(a/b)*b + a%b shall equal a.

G.5.1 Multiplicative operators

Semantics

1 If one operand has real type and the other operand has imaginary type, then the result has
imaginary type. If both operands have imaginary type, then the result has real type. (If
either operand has complex type, then the result has complex type.)

2 If the operands are not both complex, then the result and floating-point exception
behavior of the * operator is defined by the usual mathematical formula:

3 If the second operand is not complex, then the result and floating-point exception
behavior of the / operator is defined by the usual mathematical formula:

4 The * and / operators satisfy the following infinity properties for all real, imaginary, and
complex operands:315)

  — if one operand is an infinity and the other operand is a nonzero finite number or an
infinity, then the result of the * operator is an infinity;

  — if the first operand is an infinity and the second operand is a finite number, then the
result of the / operator is an infinity;

  — if the first operand is a finite number and the second operand is an infinity, then the
result of the / operator is a zero;

  — if the first operand is a nonzero finite number or an infinity and the second operand is
a zero, then the result of the / operator is an infinity.

5 If both operands of the * operator are complex or if the second operand of the / operator
is complex, the operator raises floating-point exceptions if appropriate for the calculation
of the parts of the result, and may raise spurious floating-point exceptions.

*/
//--------------------------------------------------------------------------
// Name         Expr_mul
//
// 
//--------------------------------------------------------------------------
TAst * Expr_mul ( 
    const YYLTYPE * loc, TAst * l, TAst * r  
    )
{
  const TQualNode * resType;

  ASSERT( l && r );

  l = Expr_GetRValue( l );
  r = Expr_GetRValue( r );

  CHECK_ERROR( l );
  CHECK_ERROR( r );

  if (!Type_IsArithmetic( l->exprType ) || !Type_IsArithmetic( r->exprType ))
  {
    CompilerErrorLoc( loc, "'%s' - operand is not an arithmetic type", "*" );

    return Ast_error( loc );
  }

  UsualArithmeticConversion( &l, &r );

  // Determine the result domain of floating types type (G.5.1.1)
  // e.g. multiplying real by imaginary produces imaginary, etc.
  // Naturally the type of the result can be derived from the usual formula:
  // (a + b.i)(c + d.i) = (a.c - b.d) + i(a.d + b.c)
  // It is easy determine the type of the result when a, b, c or d is 0. 
  //
  if (Type_IsFloating( l->exprType ))
  {
    const TTypeNode * tn1, * tn2;
    TypeKind_t resDomain;

    tn1 = l->exprType->tnode;
    tn2 = r->exprType->tnode;

    if (tn1->typeKind == TYPE_COMPLEX || tn2->typeKind == TYPE_COMPLEX)
      resDomain = TYPE_COMPLEX;
    else
    if (tn1->typeKind == tn2->typeKind) // r * r OR i * i
      resDomain = TYPE_REAL;
    else
      resDomain = TYPE_IMAGINARY;           // r * i OR i * r

    resType = CfeRoots.FloatingTypes[resDomain][tn1->u.realType];
  }
  else
    resType = l->exprType;

  return Ast_binOp( ast_mul, loc, resType, l, r );
};

/*
6.5.16 Assignment operators

Constraints

2 An assignment operator shall have a modifiable lvalue as its left operand.
Semantics

3 An assignment operator stores a value in the object designated by the left operand. An
assignment expression has the value of the left operand after the assignment, but is not an
lvalue. The type of an assignment expression is the type of the left operand unless the
left operand has qualified type, in which case it is the unqualified version of the type of
the left operand. The side effect of updating the stored value of the left operand shall
occur between the previous and the next sequence point.

4 The order of evaluation of the operands is unspecified. If an attempt is made to modify
the result of an assignment operator or to access it after the next sequence point, the
behavior is undefined.

6.5.16.1 Simple assignment

Constraints

1 One of the following shall hold:93)
— the left operand has qualified or unqualified arithmetic type and the right has
arithmetic type;
— the left operand has a qualified or unqualified version of a structure or union type
compatible with the type of the right;
— both operands are pointers to qualified or unqualified versions of compatible types,
and the type pointed to by the left has all the qualifiers of the type pointed to by the
right;
— one operand is a pointer to an object or incomplete type and the other is a pointer to a
qualified or unqualified version of void, and the type pointed to by the left has all
the qualifiers of the type pointed to by the right;
— the left operand is a pointer and the right is a null pointer constant; or
— the left operand has type _Bool and the right is a pointer.

Semantics

2 In simple assignment (=), the value of the right operand is converted to the type of the
assignment expression and replaces the value stored in the object designated by the left
operand.

3 If the value being stored in an object is read from another object that overlaps in any way
the storage of the first object, then the overlap shall be exact and the two objects shall
have qualified or unqualified versions of a compatible type; otherwise, the behavior is
undefined.

93) The asymmetric appearance of these constraints with respect to type qualifiers is due to the conversion
(specified in 6.3.2.1) that changes lvalues to ‘‘the value of the expression’’ which removes any type
qualifiers from the type category of the expression.
*/

//--------------------------------------------------------------------------
// Name         Expr_assign 
//
// 
//--------------------------------------------------------------------------
TAst * Expr_assign ( const YYLTYPE * loc, TAst * l, TAst * r )
{
  TAst * ast;
  const TQualNode * typ;

  r = Expr_GetRValue( r );

  CHECK_ERROR( l );
  CHECK_ERROR( r );

  if (!Expr_IsModifiableLvalue( l ))
  {
    CompilerErrorLoc( loc, "'=': left operand is not a modifiable l-value" );
    return Ast_error( loc );
  }

  typ = Type_Unqual( l->exprType );

  if (Type_IsArithmetic( l->exprType ))
  {
    // If left is arithmetic, right must be too
    //
    if (!Type_IsArithmetic( r->exprType ))
      goto badTypes;

    r = Expr_cast( loc, typ, r );
    CHECK_ERROR( r );
  }
  else
  if (l->exprType->tnode->typeKind == TYPE_STRUCT || l->exprType->tnode->typeKind == TYPE_UNION)
  {
    // If left is a struct, the right must be the same struct
    //
    if (l->exprType->tnode != r->exprType->tnode)
      goto badTypes;
  }
  else
    goto badTypes;

  TODO1( "All the cases" );

  ast = Ast_binOp( ast_assign, loc, typ, l, r );
  return ast;

badTypes:
  CompilerErrorLoc( loc, "'=': incompatible types" );
  return Ast_error( loc );
}

//--------------------------------------------------------------------------
// Name         IsConstantExpression 
//
// Note: NULL expression is allowed and is assumed not to be constant!
//--------------------------------------------------------------------------
bool IsConstantExpression ( const TAst * e )
{
  if (e == NULL)
    return false;

  switch (e->code)
  {
  case ast_error:
    break;
  case ast_intConst:
  case ast_floatConst:
  case ast_doubleConst:
  case ast_longDoubleConst:
    break;

  default:
    return false;
  }

  return true;
};


//--------------------------------------------------------------------------
// Name         ConstantExpression 
//
// 
//--------------------------------------------------------------------------
const TAst * ConstantExpression ( const YYLTYPE * loc, const TAst * e, const char * context )
{
  ASSERT( e );

  if (!IsConstantExpression( e ))
  {
    CompilerErrorLoc( loc, "%s: constant expression required", context );
    e = Ast_error( loc );
  }

  return e;
};

//--------------------------------------------------------------------------
// Name         ConstantIntegerExpression 
//
// 
//--------------------------------------------------------------------------
const TAst * ConstantIntegerExpression ( const YYLTYPE * loc, const TAst * e, const char * context )
{
  e = ConstantExpression( loc, e, context );

  if (e->code != ast_error)
  {
    if (!Type_IsInteger( e->exprType ))
    {
      CompilerErrorLoc( loc, "%s: constant integer expression required", context );
      e = Ast_error( loc );
    }
  }

  return e;
};