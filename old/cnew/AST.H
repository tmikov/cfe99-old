#ifndef AST_H
#define AST_H

#include "exprast.h"

typedef enum AST_CODE AST_CODE;
enum AST_CODE
{
  if_node,
  while_node,
  do_node,
  for_node,
  expr_node,
};

typedef struct TAst TAst;
struct TAst
{
  AST_CODE opCode;     // operation code
  TExprAst * expr;     // control expression
  TAst     * next;     // next node
  TAst     * body[2];  // body of this node
};

#endif // AST_H
