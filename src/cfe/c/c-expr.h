#ifndef C_EXPR_H
#define C_EXPR_H

TAst * Expr_GetRValue ( TAst * op );
TAst * Expr_ident ( const YYLTYPE * loc, STRING str );

TAst * Expr_deref ( const YYLTYPE * loc, TAst * op );
TAst * Expr_addr ( const YYLTYPE * loc, TAst * op );

TAst * Expr_binAnd ( const YYLTYPE * loc, TAst * l, TAst * r );
TAst * Expr_binOr ( const YYLTYPE * loc, TAst * l, TAst * r );
TAst * Expr_binXor ( const YYLTYPE * loc, TAst * l, TAst * r );

TAst * Expr_mul ( const YYLTYPE * loc, TAst * l, TAst * r );

TAst * Expr_assign ( const YYLTYPE * loc, TAst * l, TAst * r );

bool   IsConstantExpression ( const TAst * e );
const TAst * ConstantExpression ( const YYLTYPE * loc, const TAst * e, const char * context );
const TAst * ConstantIntegerExpression ( const YYLTYPE * loc, const TAst * e, const char * context );

#endif // C_EXPR_H
