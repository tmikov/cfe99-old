#ifndef __CS_386__
#define __CS_386__


#line 1 "386.nona"

#include "expr.h"

#define CS_NODE IR_node_t

#ifndef CS_OPERAND_1_OF_2
#define CS_OPERAND_1_OF_2(node)  IR_left (node)
#endif

#ifndef CS_OPERAND_2_OF_2
#define CS_OPERAND_2_OF_2(node)  IR_right (node)
#endif

#ifndef CS_OPERAND_1_OF_1
#define CS_OPERAND_1_OF_1(node)  IR_op (node)
#endif


#line 25 "386.h"
#ifndef CS_NODE
#define CS_NODE  struct IR_node_struct *
#endif

#ifndef CS_TYPE
#define CS_TYPE  CS_node
#endif

#define CSNT_symbol 1
#define CSNT_lvalue 2
#define CSNT_reg 3
#define CSNT_fetch_reg 4
#define CSNT_fetch 5
#define CSNT_direct 6
#define CSNT_reg_const 7
#define CSNT_add 8
#define CSNT_shl 9
#define CSNT_const 10
#define CSNT_expr 11
#define CSNT_assign 12

typedef CS_NODE CS_node;

typedef struct _CS_state *CS_cover;

extern CS_cover CS_find_cover (CS_node node);

extern int CS_it_is_axiom (CS_cover cover, int nonterminal);

extern CS_TYPE CS_traverse_cover (CS_cover cover, int nonterminal);

extern void CS_delete_cover (CS_cover cover);

extern void CS_start (void);

extern void CS_finish (void);

#endif /* __CS_386__ */
