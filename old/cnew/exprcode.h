/*
  exprcode.h: defines the codes used in expression trees.
*/
#ifndef EXPRCODE_H
#define EXPRCODE_H

// declare the enum 
#define DEFINE_EXPR_CODE( code, str, numArgs, flags )   code,
typedef enum EXPR_CODE EXPR_CODE;
enum EXPR_CODE
{
  #include "exprast.def"
};
#undef DEFINE_EXPR_CODE


#endif // EXPRCODE_H
