/*
  The AST represents the semantics explicitly. The idea is that we don't have to track
  l-value/r-value with flags.
  Special operators:

    deref -> converts an address to an l-value
             deref( addr( op ) ) = op
    fetch -> reads the value of an l-value (converts an l-value to an r-value)
             fetch( deref( op ) ) = fetch( op )
    addr  -> convert an lvalue to an address
             addr( deref( op ) ) = op

    assign -> left operand is an l-value, right is an r-value

  Examples:
    
    a = b
      "b"       symbol( b )
      "a"       symbol( a )
      "a = b"     fetch( symbol( b ) )
                assign( symbol( a ), fetch( symbol( b ) ) )

    a = *p
      "p"       symbol( p )
      "*p"        fetch( symbol( p ) )
                deref( fetch( symbol( p ) ) )
      "a"       symbol( a )
      "a = *p"    fetch( fetch( symbol( p ) ) )
                assign( symbol( a ), fetch( fetch( symbol( p ) ) ) )

    *p = a
      "a"       symbol( a )
      "p"       symbol( p )
      "*p"        fetch( symbol( p ) )
                deref( fetch( symbol( p ) ) )
      "*p = a"    fetch( symbol( a ) )
                assign( deref( fetch( symbol( p ) ) ), fetch( symbol( a ) ) )


    &a
                addr( symbol( a ) )
    *&a         
                deref( addr( symbol( a ) )) => symbol( a )
*/
  AST_OP( error )
  AST_OP( fetch )
  AST_OP( deref )             // expr
  AST_OP( addr )              // expr

// used for variable length array type of unspecified size
// ("[ type_qualifier_list_opt * ]")
//
  AST_OP( unspecifiedVariableArraySize )

// primary-expression
//
  AST_OP( intConst )
  AST_OP( floatConst )
  AST_OP( doubleConst )
  AST_OP( longDoubleConst )

  AST_OP( stringLiteral )    // const_len, data+

  AST_OP( enumConst )       
  AST_OP( symbol )           // [symbol]

  AST_OP( exprStatement )    // stmt

// postfix-expression
//
  AST_OP( arraySubscript )   // expr,expr

  AST_OP( functionCall )     // func, param*

  AST_OP( memberSelect )     // struct, memb
  AST_OP( ptrMemberSelect ) // struct, memb

  AST_OP( postInc )          // expr
  AST_OP( postDec )          // expr

  AST_OP( compoundLiteral )  // const_len, data+

// unary-expression
//
  AST_OP( preInc )           // expr
  AST_OP( preDec )           // expr

  AST_OP( unaryPlus )        // expr
  AST_OP( unaryMinus )       // expr
  AST_OP( binNot )           // expr
  AST_OP( logNot )           // expr

  AST_OP( sizeofExpr )       // expr
  AST_OP( sizeofType )       // [type]

  AST_OP( alignofExpr )      // expr
  AST_OP( alignofType )      // [type]

  AST_OP( labelAddr )        // [label]

// cast-expression
//
  AST_OP( cast )              // expr

// multiplicative-expression
//
  AST_OP( mul )               // expr, expr
  AST_OP( div )               // expr, expr
  AST_OP( rem )               // expr, expr

// additive-expression
// 
  AST_OP( add )               // expr, expr
  AST_OP( sub )               // expr, expr

// shift-expression
//
  AST_OP( shl )               // expr, expr
  AST_OP( shr )               // expr, expr

// relational-expression
//
  AST_OP( lessThan )         // expr, expr
  AST_OP( greaterThan )      // expr, expr
  AST_OP( lessEq )           // expr, expr
  AST_OP( greaterEq )        // expr, expr

// equality-expression
//
  AST_OP( equal )             // expr, expr
  AST_OP( notEqual )         // expr, expr

// and-expression
//
  AST_OP( binAnd )           // expr, expr

// xor-expression
//
  AST_OP( binXor )           // expr, expr

// or-expression
//
  AST_OP( binOr )            // expr, expr

// logical-and-expression
//
  AST_OP( logAnd )           // expr, expr

// logical-or-expression
//
  AST_OP( logOr )            // expr, expr

// conditional-expression
//
  AST_OP( conditional )       // expr, expr?, expr

// assignment-expression
//
  AST_OP( assign )            // expr, expr
  AST_OP( assignMul )        // expr, expr
  AST_OP( assignDiv )        // expr, expr
  AST_OP( assignRem )        // expr, expr
  AST_OP( assignAdd )        // expr, expr
  AST_OP( assignSub )        // expr, expr
  AST_OP( assignShl )        // expr, expr
  AST_OP( assignShr )        // expr, expr
  AST_OP( assignBinAnd )    // expr, expr
  AST_OP( assignBinOr )     // expr, expr
  AST_OP( assignBinXor )    // expr, expr

// expression
//
  AST_OP( comma )             // expr, expr


// declarations
//
  AST_OP( decl )              // [symbol] init_expr?


// statements
//
  AST_OP( compound )          // stmt+

  AST_OP( voidExpr )         // expr

  AST_OP( if )                // stmt, stmt+

  AST_OP( switch )            // [case_list] expr , stmt

  AST_OP( while )             // expr, stmt
  AST_OP( do )                // stmt, expr
  AST_OP( for )               // expr, expr, expr, stmt

  AST_OP( goto )              // [label]
  AST_OP( indirectGoto )     // expr

  AST_OP( continue )          // [loop_stmt]
  AST_OP( break )             // [loop_switch_stmt]

  AST_OP( return )            // expr? 

  AST_OP( label )             // [label_name]
  AST_OP( case )              // [switch_stmt] const_expr, const_expr
  AST_OP( default )           // [switch_stmt] 