// Note: the order is important and is used in c-types.c
//
typedef enum INFO_TOKEN INFO_TOKEN;
enum INFO_TOKEN
{
  TOK_NONE = 0,

  TOK_SHORT,   
  TOK_LONG,    
  TOK_SIGNED,  
  TOK_UNSIGNED,
  TOK_VOID,
  TOK_BOOL,
  TOK_CHAR,    
  TOK_INT,     
  TOK_FLOAT,   
  TOK_DOUBLE,  

  TOK_CONST,    
  TOK_VOLATILE,
  TOK_RESTRICT,

  TOK_EXTERN,   
  TOK_AUTO,     
  TOK_STATIC,   
  TOK_REGISTER, 
  TOK_TYPEDEF,  
};
#define INFO_TOKEN_BITS 6

// Convert a token code to basic type code (BT_xxx)
//
#define TOK_TO_BT( itok ) \
  ((itok) < TOK_VOID ? BT_NONE : (itok) - TOK_VOID + BT_VOID)




