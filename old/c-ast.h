#ifndef C_AST_H
#define C_AST_H

enum AstCode_t
{
  AST_EMPTY = 0,

// primary-expression
//
  AST_INT_CONST,
  AST_FLOAT_CONST,
  AST_DOUBLE_CONST,
  AST_LONGDOUBLE_CONST,

  AST_STRING_LITERAL,    // const_len, data+

  AST_ENUM_CONST,       
  AST_IDENT,             // [symbol]

  AST_EXPR_STATEMENT,    // stmt

// postfix-expression
//
  AST_ARRAY_SUBSCRIPT,   // expr,expr

  AST_FUNCTION_CALL,     // func, param*

  AST_MEMBER_SELECT,     // struct, memb
  AST_PTR_MEMBER_SELECT, // struct, memb

  AST_POST_INC,          // expr
  AST_POST_DEC,          // expr

  AST_COMPOUND_LITERAL,  // const_len, data+

  AST_FETCH,             // expr

// unary-expression
//
  AST_PRE_INC,           // expr
  AST_PRE_DEC,           // expr

  AST_ADDR,              // expr
  AST_DEREF,             // expr
  AST_UNARY_PLUS,        // expr
  AST_UNARY_MINUS,       // expr
  AST_BIN_NOT,           // expr
  AST_LOG_NOT,           // expr

  AST_SIZEOF_EXPR,       // expr
  AST_SIZEOF_TYPE,       // [type]

  AST_ALIGNOF_EXPR,      // expr
  AST_ALIGNOF_TYPE,      // [type]

  AST_LABEL_ADDR,        // [label]

// cast-expression
//
  AST_CAST,              // [type] expr

// multiplicative-expression
//
  AST_MUL,               // expr, expr
  AST_DIV,               // expr, expr
  AST_REM,               // expr, expr

// additive-expression
// 
  AST_ADD,               // expr, expr
  AST_SUB,               // expr, expr

// shift-expression
//
  AST_SHL,               // expr, expr
  AST_SHR,               // expr, expr

// relational-expression
//
  AST_LESS_THAN,         // expr, expr
  AST_GREATER_THAN,      // expr, expr
  AST_LESS_EQ,           // expr, expr
  AST_GREATER_EQ,        // expr, expr

// equality-expression
//
  AST_EQUAL,             // expr, expr
  AST_NOT_EQUAL,         // expr, expr

// and-expression
//
  AST_BIN_AND,           // expr, expr

// xor-expression
//
  AST_BIN_XOR,           // expr, expr

// or-expression
//
  AST_BIN_OR,            // expr, expr

// logical-and-expression
//
  AST_LOG_AND,           // expr, expr

// logical-or-expression
//
  AST_LOG_OR,            // expr, expr

// conditional-expression
//
  AST_CONDITIONAL,       // expr, expr?, expr

// assignment-expression
//
  AST_ASSIGN,            // expr, expr
  AST_ASSIGN_MUL,        // expr, expr
  AST_ASSIGN_DIV,        // expr, expr
  AST_ASSIGN_REM,        // expr, expr
  AST_ASSIGN_ADD,        // expr, expr
  AST_ASSIGN_SUB,        // expr, expr
  AST_ASSIGN_SHL,        // expr, expr
  AST_ASSIGN_SHR,        // expr, expr
  AST_ASSIGN_BIN_AND,    // expr, expr
  AST_ASSIGN_BIN_OR,     // expr, expr
  AST_ASSIGN_BIN_XOR,    // expr, expr

// expression
//
  AST_COMMA,             // expr, expr


// declarations
//
  AST_DECL,              // [symbol] init_expr?


// statements
//
  AST_COMPOUND,          // stmt+

  AST_VOID_EXPR,         // expr

  AST_IF                 // stmt, stmt+

  AST_SWITCH,            // [case_list] expr, stmt

  AST_WHILE,             // expr, stmt
  AST_DO,                // stmt, expr
  AST_FOR,               // expr, expr, expr, stmt

  AST_GOTO,              // [label]
  AST_INDIRECT_GOTO,     // expr

  AST_CONTINUE,          // [loop_stmt]
  AST_BREAK,             // [loop_switch_stmt]

  AST_RETURN,            // expr? 

  AST_LABEL,             // [label_name]
  AST_CASE,              // [switch_stmt] const_expr, const_expr
  AST_DEFAULT,           // [switch_stmt] 
};

typedef struct TAstNode TAstNode, * AST;

struct TAstNode
{
  AstCode_t code;
  TAstNode * child, * sibling;

  yyltype loc;

  TYPE exprType; // type of the expression

  bool isConstant : 1;

  union
  {
    TARGET_LARGEST_INT_TYPE intConst;
    TARGET_FLOAT            fltConst;
    TARGET_DOUBLE           dblConst;
    TARGET_LONG_DOUBLE      ldblConst;

    struct TSymbol * symbol;
    TYPE type;
    AST ast; 
  } u;
};

struct TEmptyAstNode
{
  AstCode_t _empty; 
  TAstNode * _null, * sibling;
};

#define KID1( node )  ((node)->child)
#define KID2( node )  (KID1(node)->sibling)
#define KID3( node )  (KID2(node)->sibling)
#define KID4( node )  (KID3(node)->sibling)

#define IS_AST_EMPTY( node )   ((node)->code == AST_EMPY)

typedef TAstNode * KID_POS;

#define FIRST_KID_POS( node )   ((node)->child)
#define NEXT_KID_POS( pos )     ((pos)->sibling)
#define END_KIDS( pos )         ((pos) == NULL)
#define GET_KID( pos )          (pos)

/*
  AST a;

  for ( KID_POS pos = FIRST_KID_POS( a ); !END_KIDS( pos ); pos = NEXT_KID_POS( pos ) )
  {
    AST kid = GET_KID( pos );
  }
*/


AST New_Ast ( AstCode_t code, const yyltype * loc );

#define New_EmptyAst()  New_Ast( AST_EMPTY, NULL )


/*
  int i = 0;
  i = i + 1;

  DECL[i]( INT_CONST[0] );
  VOID_EXPR( 
    ASSIGN(
      IDENT[i],
      ADD( 
        FETCH( IDENT[i] ),
        INT_CONST[1]
      )
    )
  );
*/


#endif // C_AST_H
