#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "386.h"


#line 23 "386.nona"

int eax_used = 0;

void NeedEax ( void )
{
  if (eax_used != 0)
    printf( "push eax\n" );
  ++eax_used;
};

char OperBuf[80];

void GetDirect ( IR_node_t node )
{
  if (IR_NODE_MODE( node ) == IR_NM_e_icon)
    sprintf( OperBuf, "%d", IR_value( node ) );
  else
  if (IR_NODE_MODE( node ) == IR_NM_e_sym)
    sprintf( OperBuf, "%s", IR_name( node ) );
  else
    abort();
};



#line 34 "386.c"
#ifndef CS_OPERATION
#define CS_OPERATION(node)  IR_NODE_MODE (node)
#endif

#ifndef CS_OPERAND_1_OF_2
#define CS_OPERAND_1_OF_2(node)  IR_operand_1 (node)
#endif

#ifndef CS_OPERAND_2_OF_2
#define CS_OPERAND_2_OF_2(node)  IR_operand_2 (node)
#endif

#ifndef CS_OPERAND_1_OF_1
#define CS_OPERAND_1_OF_1(node)  IR_operand (node)
#endif


#ifndef CS_STATE
#define CS_STATE(node)  IR_state (node)
#endif

#ifndef CS_SET_STATE
#define CS_SET_STATE(node, state)  IR_set_state (node, state)
#endif

#ifndef CS_ATTRIBUTE
#define CS_ATTRIBUTE(node)  (node)
#endif

#ifndef CS_ERROR
#define CS_ERROR(str)  fprintf (stderr, "%s\n", str)
#endif

#ifndef CS_START_ALLOC
#define CS_START_ALLOC()
#endif

#ifndef CS_FINISH_ALLOC
#define CS_FINISH_ALLOC()
#endif

#ifndef CS_ALLOC
#define CS_ALLOC(ptr, size, ptr_type)  ((ptr) = (ptr_type) malloc (size))
#endif

#ifndef CS_FREE
#define CS_FREE(ptr, size)  free (ptr)
#endif


#define _CS_STATE(node)  ((struct _CS_state *) CS_STATE (node))

typedef int _CS_cost;

struct _CS_state
{
  CS_node node;
  union
  {
    CS_TYPE nonterminal_attribute [12];
    _CS_cost cost [12];
  } costs_or_attrs;
  char pass_flag [12];
  struct
  {
    unsigned int _CS_symbol: 1;
    unsigned int _CS_lvalue: 1;
    unsigned int _CS_reg: 1;
    unsigned int _CS_fetch_reg: 1;
    unsigned int _CS_fetch: 2;
    unsigned int _CS_direct: 2;
    unsigned int _CS_reg_const: 2;
    unsigned int _CS_add: 2;
    unsigned int _CS_shl: 3;
    unsigned int _CS_const: 1;
    unsigned int _CS_expr: 3;
    unsigned int _CS_assign: 2;
  } rule;

};

static short _CS_symbol_decode_rule_vector [] =
{
  0, 1
};

static short _CS_lvalue_decode_rule_vector [] =
{
  0, 2
};

static short _CS_reg_decode_rule_vector [] =
{
  0, 3
};

static short _CS_fetch_reg_decode_rule_vector [] =
{
  0, 4
};

static short _CS_fetch_decode_rule_vector [] =
{
  0, 5, 6, 7
};

static short _CS_direct_decode_rule_vector [] =
{
  0, 8, 9, 10
};

static short _CS_reg_const_decode_rule_vector [] =
{
  0, 11, 12
};

static short _CS_add_decode_rule_vector [] =
{
  0, 13, 14, 15
};

static short _CS_shl_decode_rule_vector [] =
{
  0, 16, 17, 18, 19
};

static short _CS_const_decode_rule_vector [] =
{
  0, 20
};

static short _CS_expr_decode_rule_vector [] =
{
  0, 21, 22, 23, 24
};

static short _CS_assign_decode_rule_vector [] =
{
  0, 25, 26, 27
};

static void _CS_closure_symbol
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_fetch_reg
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_fetch
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_add
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_shl
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_const
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_symbol
  (struct _CS_state *state, CS_node node, int cost)
{
  int all_cost;

  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_lvalue])
    {
      /* lvalue : symbol */
      state->costs_or_attrs.cost [CSNT_lvalue] = all_cost;
      state->rule._CS_lvalue = 1;
    }
}

static void _CS_closure_fetch_reg
  (struct _CS_state *state, CS_node node, int cost)
{
  int all_cost;

  all_cost = cost
#line 73 "386.nona"
 + 1;
#line 216 "386.c"
  if (all_cost < state->costs_or_attrs.cost [CSNT_fetch])
    {
      /* fetch : fetch_reg */
      state->costs_or_attrs.cost [CSNT_fetch] = all_cost;
      state->rule._CS_fetch = 1;
      _CS_closure_fetch (state, node, all_cost);
    }
  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_direct])
    {
      /* direct : fetch_reg */
      state->costs_or_attrs.cost [CSNT_direct] = all_cost;
      state->rule._CS_direct = 3;
    }
  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_reg_const])
    {
      /* reg_const : fetch_reg */
      state->costs_or_attrs.cost [CSNT_reg_const] = all_cost;
      state->rule._CS_reg_const = 1;
    }
}

static void _CS_closure_fetch
  (struct _CS_state *state, CS_node node, int cost)
{
  int all_cost;

  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_expr])
    {
      /* expr : fetch */
      state->costs_or_attrs.cost [CSNT_expr] = all_cost;
      state->rule._CS_expr = 1;
    }
}

static void _CS_closure_add
  (struct _CS_state *state, CS_node node, int cost)
{
  int all_cost;

  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_expr])
    {
      /* expr : add */
      state->costs_or_attrs.cost [CSNT_expr] = all_cost;
      state->rule._CS_expr = 2;
    }
}

static void _CS_closure_shl
  (struct _CS_state *state, CS_node node, int cost)
{
  int all_cost;

  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_expr])
    {
      /* expr : shl */
      state->costs_or_attrs.cost [CSNT_expr] = all_cost;
      state->rule._CS_expr = 3;
    }
}

static void _CS_closure_const
  (struct _CS_state *state, CS_node node, int cost)
{
  int all_cost;

  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_expr])
    {
      /* expr : const */
      state->costs_or_attrs.cost [CSNT_expr] = all_cost;
      state->rule._CS_expr = 4;
    }
}

static struct _CS_state *_CS_state_ (CS_node node)
{
  int cost;
  struct _CS_state *state;

  CS_ALLOC (state, sizeof (struct _CS_state), struct _CS_state *);
  state->node = node;
  memset ((char *) &state->rule, 0, sizeof (state->rule));
  state->costs_or_attrs.cost [1] = 2147483647;
  state->costs_or_attrs.cost [2] = 2147483647;
  state->costs_or_attrs.cost [3] = 2147483647;
  state->costs_or_attrs.cost [4] = 2147483647;
  state->costs_or_attrs.cost [5] = 2147483647;
  state->costs_or_attrs.cost [6] = 2147483647;
  state->costs_or_attrs.cost [7] = 2147483647;
  state->costs_or_attrs.cost [8] = 2147483647;
  state->costs_or_attrs.cost [9] = 2147483647;
  state->costs_or_attrs.cost [10] = 2147483647;
  state->costs_or_attrs.cost [11] = 2147483647;
  state->costs_or_attrs.cost [12] = 2147483647;
  switch (CS_OPERATION (node))
    {
    case IR_NM_e_plus:
      if (_CS_STATE (CS_OPERAND_1_OF_2 (node))->rule._CS_expr
          && _CS_STATE (CS_OPERAND_2_OF_2 (node))->rule._CS_direct)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
              + _CS_STATE (CS_OPERAND_2_OF_2 (node))->costs_or_attrs.cost [CSNT_direct]
#line 114 "386.nona"
 + 1;
#line 328 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_add])
            {
              /* add : IR_NM_e_plus ( expr, direct) */
              state->costs_or_attrs.cost [CSNT_add] = cost;
              state->rule._CS_add = 1;
              _CS_closure_add (state, node, cost);
            }
        }
      if (_CS_STATE (CS_OPERAND_1_OF_2 (node))->rule._CS_expr
          && _CS_STATE (CS_OPERAND_2_OF_2 (node))->rule._CS_expr)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
              + _CS_STATE (CS_OPERAND_2_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 121 "386.nona"
 + 2;
#line 346 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_add])
            {
              /* add : IR_NM_e_plus ( expr, expr) */
              state->costs_or_attrs.cost [CSNT_add] = cost;
              state->rule._CS_add = 2;
              _CS_closure_add (state, node, cost);
            }
        }
      if (_CS_STATE (CS_OPERAND_1_OF_2 (node))->rule._CS_fetch_reg
          && _CS_STATE (CS_OPERAND_2_OF_2 (node))->rule._CS_reg_const)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (node))->costs_or_attrs.cost [CSNT_fetch_reg]
              + _CS_STATE (CS_OPERAND_2_OF_2 (node))->costs_or_attrs.cost [CSNT_reg_const]
#line 129 "386.nona"
 + 1;
#line 364 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_add])
            {
              /* add : IR_NM_e_plus ( fetch_reg, reg_const) */
              state->costs_or_attrs.cost [CSNT_add] = cost;
              state->rule._CS_add = 3;
              _CS_closure_add (state, node, cost);
            }
        }
      break;
    case IR_NM_e_minus:
      break;
    case IR_NM_e_shl:
      if (_CS_STATE (CS_OPERAND_1_OF_2 (node))->rule._CS_expr
          && CS_OPERATION (CS_OPERAND_2_OF_2 (node)) == IR_NM_e_icon)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 137 "386.nona"
 + 1;
#line 385 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_shl])
            {
              /* shl : IR_NM_e_shl ( expr, IR_NM_e_icon) */
              state->costs_or_attrs.cost [CSNT_shl] = cost;
              state->rule._CS_shl = 1;
              _CS_closure_shl (state, node, cost);
            }
        }
      if (_CS_STATE (CS_OPERAND_1_OF_2 (node))->rule._CS_expr
          && _CS_STATE (CS_OPERAND_2_OF_2 (node))->rule._CS_expr)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
              + _CS_STATE (CS_OPERAND_2_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 144 "386.nona"
 + 2;
#line 403 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_shl])
            {
              /* shl : IR_NM_e_shl ( expr, expr) */
              state->costs_or_attrs.cost [CSNT_shl] = cost;
              state->rule._CS_shl = 2;
              _CS_closure_shl (state, node, cost);
            }
        }
      if (CS_OPERATION (CS_OPERAND_1_OF_2 (node)) == IR_NM_e_plus
          && _CS_STATE (CS_OPERAND_1_OF_2 (CS_OPERAND_1_OF_2 (node)))->rule._CS_expr
          && CS_OPERATION (CS_OPERAND_2_OF_2 (CS_OPERAND_1_OF_2 (node))) == IR_NM_e_icon
          && CS_OPERATION (CS_OPERAND_2_OF_2 (node)) == IR_NM_e_icon &&
#line 157 "386.nona"
IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node))) > 0 && IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node))) <= 3)
#line 418 "386.c"
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (CS_OPERAND_1_OF_2 (node)))->costs_or_attrs.cost [CSNT_expr]
#line 156 "386.nona"
 + 1;
#line 425 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_shl])
            {
              /* shl : IR_NM_e_shl ( IR_NM_e_plus ( expr, IR_NM_e_icon), IR_NM_e_icon) */
              state->costs_or_attrs.cost [CSNT_shl] = cost;
              state->rule._CS_shl = 3;
              _CS_closure_shl (state, node, cost);
            }
        }
      if (CS_OPERATION (CS_OPERAND_1_OF_2 (node)) == IR_NM_e_plus
          && _CS_STATE (CS_OPERAND_1_OF_2 (CS_OPERAND_1_OF_2 (node)))->rule._CS_fetch_reg
          && CS_OPERATION (CS_OPERAND_2_OF_2 (CS_OPERAND_1_OF_2 (node))) == IR_NM_e_icon
          && CS_OPERATION (CS_OPERAND_2_OF_2 (node)) == IR_NM_e_icon &&
#line 165 "386.nona"
IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node))) > 0 && IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node))) <= 3)
#line 440 "386.c"
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (CS_OPERAND_1_OF_2 (node)))->costs_or_attrs.cost [CSNT_fetch_reg]
#line 164 "386.nona"
 + 1;
#line 447 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_shl])
            {
              /* shl : IR_NM_e_shl ( IR_NM_e_plus ( fetch_reg, IR_NM_e_icon), IR_NM_e_icon) */
              state->costs_or_attrs.cost [CSNT_shl] = cost;
              state->rule._CS_shl = 4;
              _CS_closure_shl (state, node, cost);
            }
        }
      break;
    case IR_NM_e_shr:
      break;
    case IR_NM_e_assign:
      if (_CS_STATE (CS_OPERAND_1_OF_2 (node))->rule._CS_lvalue
          && _CS_STATE (CS_OPERAND_2_OF_2 (node))->rule._CS_expr)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (node))->costs_or_attrs.cost [CSNT_lvalue]
              + _CS_STATE (CS_OPERAND_2_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 185 "386.nona"
 + 2;
#line 469 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_assign])
            {
              /* assign : IR_NM_e_assign ( lvalue, expr) */
              state->costs_or_attrs.cost [CSNT_assign] = cost;
              state->rule._CS_assign = 1;
            }
        }
      if (CS_OPERATION (CS_OPERAND_1_OF_2 (node)) == IR_NM_e_sym
          && _CS_STATE (CS_OPERAND_2_OF_2 (node))->rule._CS_expr)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_2_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 193 "386.nona"
 + 1;
#line 485 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_assign])
            {
              /* assign : IR_NM_e_assign ( IR_NM_e_sym, expr) */
              state->costs_or_attrs.cost [CSNT_assign] = cost;
              state->rule._CS_assign = 2;
            }
        }
      if (CS_OPERATION (CS_OPERAND_1_OF_2 (node)) == IR_NM_e_sym
          && CS_OPERATION (CS_OPERAND_2_OF_2 (node)) == IR_NM_e_icon)
        {
          cost
            = 0
#line 199 "386.nona"
 + 1;
#line 500 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_assign])
            {
              /* assign : IR_NM_e_assign ( IR_NM_e_sym, IR_NM_e_icon) */
              state->costs_or_attrs.cost [CSNT_assign] = cost;
              state->rule._CS_assign = 3;
            }
        }
      break;
    case IR_NM_e_fetch:
      if (_CS_STATE (CS_OPERAND_1_OF_1 (node))->rule._CS_reg)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_1 (node))->costs_or_attrs.cost [CSNT_reg];
          if (cost < state->costs_or_attrs.cost [CSNT_fetch_reg])
            {
              /* fetch_reg : IR_NM_e_fetch ( reg) */
              state->costs_or_attrs.cost [CSNT_fetch_reg] = cost;
              state->rule._CS_fetch_reg = 1;
              _CS_closure_fetch_reg (state, node, cost);
            }
        }
      if (_CS_STATE (CS_OPERAND_1_OF_1 (node))->rule._CS_lvalue)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_1 (node))->costs_or_attrs.cost [CSNT_lvalue]
#line 80 "386.nona"
 + 1;
#line 530 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_fetch])
            {
              /* fetch : IR_NM_e_fetch ( lvalue) */
              state->costs_or_attrs.cost [CSNT_fetch] = cost;
              state->rule._CS_fetch = 2;
              _CS_closure_fetch (state, node, cost);
            }
        }
      if (CS_OPERATION (CS_OPERAND_1_OF_1 (node)) == IR_NM_e_sym)
        {
          cost
            = 0
#line 86 "386.nona"
 + 1;
#line 545 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_fetch])
            {
              /* fetch : IR_NM_e_fetch ( IR_NM_e_sym) */
              state->costs_or_attrs.cost [CSNT_fetch] = cost;
              state->rule._CS_fetch = 3;
              _CS_closure_fetch (state, node, cost);
            }
        }
      if (CS_OPERATION (CS_OPERAND_1_OF_1 (node)) == IR_NM_e_sym)
        {
          cost
            = 0;
          if (cost < state->costs_or_attrs.cost [CSNT_direct])
            {
              /* direct : IR_NM_e_fetch ( IR_NM_e_sym) */
              state->costs_or_attrs.cost [CSNT_direct] = cost;
              state->rule._CS_direct = 2;
            }
        }
      break;
    case IR_NM_e_sym:
      if (
#line 51 "386.nona"
IR_isReg(CS_ATTRIBUTE (node)) == 0)
#line 570 "386.c"
        {
          cost
            = 0
#line 50 "386.nona"
 + 1;
#line 576 "386.c"
          if (cost < state->costs_or_attrs.cost [CSNT_symbol])
            {
              /* symbol : IR_NM_e_sym */
              state->costs_or_attrs.cost [CSNT_symbol] = cost;
              state->rule._CS_symbol = 1;
              _CS_closure_symbol (state, node, cost);
            }
        }
      if (
#line 61 "386.nona"
IR_isReg(CS_ATTRIBUTE (node)) != 0)
#line 588 "386.c"
        {
          cost
            = 0;
          if (cost < state->costs_or_attrs.cost [CSNT_reg])
            {
              /* reg : IR_NM_e_sym */
              state->costs_or_attrs.cost [CSNT_reg] = cost;
              state->rule._CS_reg = 1;
            }
        }
      break;
    case IR_NM_e_icon:
      cost
        = 0;
      if (cost < state->costs_or_attrs.cost [CSNT_direct])
        {
          /* direct : IR_NM_e_icon */
          state->costs_or_attrs.cost [CSNT_direct] = cost;
          state->rule._CS_direct = 1;
        }
      cost
        = 0;
      if (cost < state->costs_or_attrs.cost [CSNT_reg_const])
        {
          /* reg_const : IR_NM_e_icon */
          state->costs_or_attrs.cost [CSNT_reg_const] = cost;
          state->rule._CS_reg_const = 2;
        }
      cost
        = 0
#line 173 "386.nona"
 + 1;
#line 621 "386.c"
      if (cost < state->costs_or_attrs.cost [CSNT_const])
        {
          /* const : IR_NM_e_icon */
          state->costs_or_attrs.cost [CSNT_const] = cost;
          state->rule._CS_const = 1;
          _CS_closure_const (state, node, cost);
        }
      break;
    default:
      abort ();
      break;
    }
  return state;
}

#ifdef __CS_DEBUG__
static void _CS_print_node_state (struct _CS_state *state)
{
  switch (state->rule._CS_symbol)
    {
    case 1:
      fprintf (stderr, "       rule `symbol : IR_NM_e_sym'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_symbol]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_lvalue)
    {
    case 1:
      fprintf (stderr, "       rule `lvalue : symbol'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_lvalue]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_reg)
    {
    case 1:
      fprintf (stderr, "       rule `reg : IR_NM_e_sym'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_reg]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_fetch_reg)
    {
    case 1:
      fprintf (stderr, "       rule `fetch_reg : IR_NM_e_fetch ( reg)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_fetch_reg]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_fetch)
    {
    case 1:
      fprintf (stderr, "       rule `fetch : fetch_reg'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_fetch]);
      break;
    case 2:
      fprintf (stderr, "       rule `fetch : IR_NM_e_fetch ( lvalue)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_fetch]);
      break;
    case 3:
      fprintf (stderr, "       rule `fetch : IR_NM_e_fetch ( IR_NM_e_sym)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_fetch]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_direct)
    {
    case 1:
      fprintf (stderr, "       rule `direct : IR_NM_e_icon'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_direct]);
      break;
    case 2:
      fprintf (stderr, "       rule `direct : IR_NM_e_fetch ( IR_NM_e_sym)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_direct]);
      break;
    case 3:
      fprintf (stderr, "       rule `direct : fetch_reg'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_direct]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_reg_const)
    {
    case 1:
      fprintf (stderr, "       rule `reg_const : fetch_reg'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_reg_const]);
      break;
    case 2:
      fprintf (stderr, "       rule `reg_const : IR_NM_e_icon'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_reg_const]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_add)
    {
    case 1:
      fprintf (stderr, "       rule `add : IR_NM_e_plus ( expr, direct)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_add]);
      break;
    case 2:
      fprintf (stderr, "       rule `add : IR_NM_e_plus ( expr, expr)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_add]);
      break;
    case 3:
      fprintf (stderr, "       rule `add : IR_NM_e_plus ( fetch_reg, reg_const)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_add]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_shl)
    {
    case 1:
      fprintf (stderr, "       rule `shl : IR_NM_e_shl ( expr, IR_NM_e_icon)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_shl]);
      break;
    case 2:
      fprintf (stderr, "       rule `shl : IR_NM_e_shl ( expr, expr)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_shl]);
      break;
    case 3:
      fprintf (stderr, "       rule `shl : IR_NM_e_shl ( IR_NM_e_plus ( expr, IR_NM_e_icon), IR_NM_e_icon)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_shl]);
      break;
    case 4:
      fprintf (stderr, "       rule `shl : IR_NM_e_shl ( IR_NM_e_plus ( fetch_reg, IR_NM_e_icon), IR_NM_e_icon)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_shl]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_const)
    {
    case 1:
      fprintf (stderr, "       rule `const : IR_NM_e_icon'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_const]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_expr)
    {
    case 1:
      fprintf (stderr, "       rule `expr : fetch'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_expr]);
      break;
    case 2:
      fprintf (stderr, "       rule `expr : add'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_expr]);
      break;
    case 3:
      fprintf (stderr, "       rule `expr : shl'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_expr]);
      break;
    case 4:
      fprintf (stderr, "       rule `expr : const'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_expr]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_assign)
    {
    case 1:
      fprintf (stderr, "       rule `assign : IR_NM_e_assign ( lvalue, expr)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_assign]);
      break;
    case 2:
      fprintf (stderr, "       rule `assign : IR_NM_e_assign ( IR_NM_e_sym, expr)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_assign]);
      break;
    case 3:
      fprintf (stderr, "       rule `assign : IR_NM_e_assign ( IR_NM_e_sym, IR_NM_e_icon)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_assign]);
      break;
    default:
      /* Nothing */
      break;
    }
}
#endif /* __CS_DEBUG__ */

static void _CS_find_cover_pass_1 (CS_node node)
{
  if (node == NULL)
    {
      CS_ERROR ("CS_find_cover: tree is not complete");
      abort ();
    }
  switch (CS_OPERATION (node))
    {
    case IR_NM_e_plus:
      _CS_find_cover_pass_1 (CS_OPERAND_1_OF_2 (node));
      _CS_find_cover_pass_1 (CS_OPERAND_2_OF_2 (node));
      break;
    case IR_NM_e_minus:
      break;
    case IR_NM_e_shl:
      _CS_find_cover_pass_1 (CS_OPERAND_1_OF_2 (node));
      _CS_find_cover_pass_1 (CS_OPERAND_2_OF_2 (node));
      break;
    case IR_NM_e_shr:
      break;
    case IR_NM_e_assign:
      _CS_find_cover_pass_1 (CS_OPERAND_1_OF_2 (node));
      _CS_find_cover_pass_1 (CS_OPERAND_2_OF_2 (node));
      break;
    case IR_NM_e_fetch:
      _CS_find_cover_pass_1 (CS_OPERAND_1_OF_1 (node));
      break;
    case IR_NM_e_sym:
      break;
    case IR_NM_e_icon:
      break;
    default:
      CS_ERROR ("CS_find_cover: unknown operation code");
      abort ();
      break;
    }
  CS_SET_STATE (node, NULL);
}

static CS_cover _CS_find_cover_pass_2 (CS_node node)
{
  if (node == NULL)
    {
      CS_ERROR ("CS_find_cover: tree is not complete");
      abort ();
    }
  if (CS_STATE (node) != NULL)
    return (CS_cover) CS_STATE (node);
  switch (CS_OPERATION (node))
    {
    case IR_NM_e_plus:
      _CS_find_cover_pass_2 (CS_OPERAND_1_OF_2 (node));
      _CS_find_cover_pass_2 (CS_OPERAND_2_OF_2 (node));
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_find_cover: terminal IR_NM_e_plus, state:\n");
#endif /* __CS_DEBUG__ */
      break;
    case IR_NM_e_minus:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_find_cover: terminal IR_NM_e_minus, state:\n");
#endif /* __CS_DEBUG__ */
      break;
    case IR_NM_e_shl:
      _CS_find_cover_pass_2 (CS_OPERAND_1_OF_2 (node));
      _CS_find_cover_pass_2 (CS_OPERAND_2_OF_2 (node));
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_find_cover: terminal IR_NM_e_shl, state:\n");
#endif /* __CS_DEBUG__ */
      break;
    case IR_NM_e_shr:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_find_cover: terminal IR_NM_e_shr, state:\n");
#endif /* __CS_DEBUG__ */
      break;
    case IR_NM_e_assign:
      _CS_find_cover_pass_2 (CS_OPERAND_1_OF_2 (node));
      _CS_find_cover_pass_2 (CS_OPERAND_2_OF_2 (node));
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_find_cover: terminal IR_NM_e_assign, state:\n");
#endif /* __CS_DEBUG__ */
      break;
    case IR_NM_e_fetch:
      _CS_find_cover_pass_2 (CS_OPERAND_1_OF_1 (node));
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_find_cover: terminal IR_NM_e_fetch, state:\n");
#endif /* __CS_DEBUG__ */
      break;
    case IR_NM_e_sym:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_find_cover: terminal IR_NM_e_sym, state:\n");
#endif /* __CS_DEBUG__ */
      break;
    case IR_NM_e_icon:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_find_cover: terminal IR_NM_e_icon, state:\n");
#endif /* __CS_DEBUG__ */
      break;
    default:
      CS_ERROR ("CS_find_cover: unknown operation code");
      abort ();
      break;
    }
  CS_SET_STATE (node, _CS_state_ (node));
#ifdef __CS_DEBUG__
  _CS_print_node_state (_CS_STATE (node));
#endif /* __CS_DEBUG__ */
  return (CS_cover) CS_STATE (node);
}

CS_cover CS_find_cover (CS_node node)
{
  _CS_find_cover_pass_1 (node);
  return _CS_find_cover_pass_2 (node);
}

static int _CS_nonterminal_rule (CS_cover cover, int nonterminal)
{
  if (cover == NULL)
    return 0;
  switch (nonterminal)
    {
    case CSNT_symbol:
      return _CS_symbol_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_symbol];
    case CSNT_lvalue:
      return _CS_lvalue_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_lvalue];
    case CSNT_reg:
      return _CS_reg_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_reg];
    case CSNT_fetch_reg:
      return _CS_fetch_reg_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_fetch_reg];
    case CSNT_fetch:
      return _CS_fetch_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_fetch];
    case CSNT_direct:
      return _CS_direct_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_direct];
    case CSNT_reg_const:
      return _CS_reg_const_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_reg_const];
    case CSNT_add:
      return _CS_add_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_add];
    case CSNT_shl:
      return _CS_shl_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_shl];
    case CSNT_const:
      return _CS_const_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_const];
    case CSNT_expr:
      return _CS_expr_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_expr];
    case CSNT_assign:
      return _CS_assign_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_assign];
    default:
      return 0;
    }
}

int CS_it_is_axiom (CS_cover cover, int nonterminal)
{
  if (cover == NULL)
    {
      CS_ERROR ("CS_it_is_axiom: incorrect cover");
      abort ();
    }
  return _CS_nonterminal_rule (cover, nonterminal) != 0;
}

static void _CS_traverse_cover_1 (CS_cover cover, int nonterminal)
{
  CS_node node;

  if (cover == NULL)
    {
      CS_ERROR ("CS_traverse_cover: incorrect cover");
      abort ();
    }
  cover->pass_flag [nonterminal] = 0;
  node = cover->node;
  switch (_CS_nonterminal_rule (cover, nonterminal))
    {
    case 1:
      /* symbol : IR_NM_e_sym. */
      break;
    case 2:
      /* lvalue : symbol. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_symbol);
      break;
    case 3:
      /* reg : IR_NM_e_sym. */
      break;
    case 4:
      /* fetch_reg : IR_NM_e_fetch ( reg). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_1 (node)), CSNT_reg);
      break;
    case 5:
      /* fetch : fetch_reg. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_fetch_reg);
      break;
    case 6:
      /* fetch : IR_NM_e_fetch ( lvalue). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_1 (node)), CSNT_lvalue);
      break;
    case 7:
      /* fetch : IR_NM_e_fetch ( IR_NM_e_sym). */
      break;
    case 8:
      /* direct : IR_NM_e_icon. */
      break;
    case 9:
      /* direct : IR_NM_e_fetch ( IR_NM_e_sym). */
      break;
    case 10:
      /* direct : fetch_reg. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_fetch_reg);
      break;
    case 11:
      /* reg_const : fetch_reg. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_fetch_reg);
      break;
    case 12:
      /* reg_const : IR_NM_e_icon. */
      break;
    case 13:
      /* add : IR_NM_e_plus ( expr, direct). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_direct);
      break;
    case 14:
      /* add : IR_NM_e_plus ( expr, expr). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
      break;
    case 15:
      /* add : IR_NM_e_plus ( fetch_reg, reg_const). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_fetch_reg);
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_reg_const);
      break;
    case 16:
      /* shl : IR_NM_e_shl ( expr, IR_NM_e_icon). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      break;
    case 17:
      /* shl : IR_NM_e_shl ( expr, expr). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
      break;
    case 18:
      /* shl : IR_NM_e_shl ( IR_NM_e_plus ( expr, IR_NM_e_icon), IR_NM_e_icon). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (CS_OPERAND_1_OF_2 (node))), CSNT_expr);
      break;
    case 19:
      /* shl : IR_NM_e_shl ( IR_NM_e_plus ( fetch_reg, IR_NM_e_icon), IR_NM_e_icon). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (CS_OPERAND_1_OF_2 (node))), CSNT_fetch_reg);
      break;
    case 20:
      /* const : IR_NM_e_icon. */
      break;
    case 21:
      /* expr : fetch. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_fetch);
      break;
    case 22:
      /* expr : add. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_add);
      break;
    case 23:
      /* expr : shl. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_shl);
      break;
    case 24:
      /* expr : const. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_const);
      break;
    case 25:
      /* assign : IR_NM_e_assign ( lvalue, expr). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_lvalue);
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
      break;
    case 26:
      /* assign : IR_NM_e_assign ( IR_NM_e_sym, expr). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
      break;
    case 27:
      /* assign : IR_NM_e_assign ( IR_NM_e_sym, IR_NM_e_icon). */
      break;
    default:
      CS_ERROR ("CS_traverse_cover: incorrect cover");
      abort ();
      break;
    }
}

static CS_TYPE _CS_traverse_cover_2 (CS_cover cover, int nonterminal)
{
  CS_node node;
  CS_TYPE result;
  CS_TYPE nonterm_attr_1;
  CS_TYPE nonterm_attr_2;

  if (cover == NULL)
    {
      CS_ERROR ("CS_traverse_cover: incorrect cover");
      abort ();
    }
  if (cover->pass_flag [nonterminal] != 0)
    return cover->costs_or_attrs.nonterminal_attribute [nonterminal];
  node = cover->node;
  switch (_CS_nonterminal_rule (cover, nonterminal))
    {
    case 1:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `symbol : IR_NM_e_sym'\n");
#endif /* __CS_DEBUG__ */

#line 52 "386.nona"
{
    NeedEax();
    printf( "lea eax, %s\n", IR_name( CS_ATTRIBUTE (node) ) );
  }
#line 1172 "386.c"
      break;
    case 2:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_symbol);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `lvalue : symbol'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 3:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `reg : IR_NM_e_sym'\n");
#endif /* __CS_DEBUG__ */

#line 62 "386.nona"
{
    result = CS_ATTRIBUTE (node);
  }
#line 1191 "386.c"
      break;
    case 4:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_1 (node)), CSNT_reg);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `fetch_reg : IR_NM_e_fetch ( reg)'\n");
#endif /* __CS_DEBUG__ */

#line 68 "386.nona"
{
    result = nonterm_attr_1;
  }
#line 1205 "386.c"
      break;
    case 5:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_fetch_reg);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `fetch : fetch_reg'\n");
#endif /* __CS_DEBUG__ */

#line 74 "386.nona"
{
    NeedEax();
    printf( "mov eax, %s\n", IR_name(nonterm_attr_1) );
  }
#line 1220 "386.c"
      break;
    case 6:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_1 (node)), CSNT_lvalue);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `fetch : IR_NM_e_fetch ( lvalue)'\n");
#endif /* __CS_DEBUG__ */

#line 81 "386.nona"
{
    printf( "mov eax, dword ptr [eax]\n" );
  }
#line 1234 "386.c"
      break;
    case 7:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `fetch : IR_NM_e_fetch ( IR_NM_e_sym)'\n");
#endif /* __CS_DEBUG__ */

#line 87 "386.nona"
{
    NeedEax();
    printf( "mov eax, %s\n", IR_name( CS_ATTRIBUTE (CS_OPERAND_1_OF_1 (node)) ) );
  }
#line 1246 "386.c"
      break;
    case 8:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `direct : IR_NM_e_icon'\n");
#endif /* __CS_DEBUG__ */

#line 94 "386.nona"
{
    result = CS_ATTRIBUTE (node);
  }
#line 1257 "386.c"
      break;
    case 9:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `direct : IR_NM_e_fetch ( IR_NM_e_sym)'\n");
#endif /* __CS_DEBUG__ */

#line 100 "386.nona"
{
    result = CS_ATTRIBUTE (CS_OPERAND_1_OF_1 (node));
  }
#line 1268 "386.c"
      break;
    case 10:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_fetch_reg);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `direct : fetch_reg'\n");
#endif /* __CS_DEBUG__ */

#line 106 "386.nona"
{
    result = nonterm_attr_1;
  }
#line 1282 "386.c"
      break;
    case 11:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_fetch_reg);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `reg_const : fetch_reg'\n");
#endif /* __CS_DEBUG__ */

#line 111 "386.nona"
{ result = nonterm_attr_1; }
#line 1294 "386.c"
      break;
    case 12:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `reg_const : IR_NM_e_icon'\n");
#endif /* __CS_DEBUG__ */

#line 112 "386.nona"
{ result = CS_ATTRIBUTE (node); }
#line 1303 "386.c"
      break;
    case 13:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      nonterm_attr_2
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_direct);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `add : IR_NM_e_plus ( expr, direct)'\n");
#endif /* __CS_DEBUG__ */

#line 115 "386.nona"
{
    GetDirect( nonterm_attr_2 );
    printf( "add eax, %s\n", OperBuf );
  }
#line 1321 "386.c"
      break;
    case 14:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      nonterm_attr_2
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `add : IR_NM_e_plus ( expr, expr)'\n");
#endif /* __CS_DEBUG__ */

#line 122 "386.nona"
{
    printf( "pop ebx\n" );
    --eax_used;
    printf( "add eax, ebx\n" );
  }
#line 1340 "386.c"
      break;
    case 15:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_fetch_reg);
      nonterm_attr_2
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_reg_const);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `add : IR_NM_e_plus ( fetch_reg, reg_const)'\n");
#endif /* __CS_DEBUG__ */

#line 130 "386.nona"
{
    NeedEax();
    GetDirect( nonterm_attr_2 );
    printf( "lea eax, [%s + %s]\n", IR_name(nonterm_attr_1), OperBuf );
  }
#line 1359 "386.c"
      break;
    case 16:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `shl : IR_NM_e_shl ( expr, IR_NM_e_icon)'\n");
#endif /* __CS_DEBUG__ */

#line 138 "386.nona"
{
    GetDirect( CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node)) );
    printf( "shl eax, %s\n", OperBuf );
  }
#line 1374 "386.c"
      break;
    case 17:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      nonterm_attr_2
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `shl : IR_NM_e_shl ( expr, expr)'\n");
#endif /* __CS_DEBUG__ */

#line 145 "386.nona"
{
    printf( "mov cl, al\n" );
    printf( "pop eax\n" );
    --eax_used;
    printf( "shl eax, cl\n" );
  }
#line 1394 "386.c"
      break;
    case 18:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (CS_OPERAND_1_OF_2 (node))), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `shl : IR_NM_e_shl ( IR_NM_e_plus ( expr, IR_NM_e_icon), IR_NM_e_icon)'\n");
#endif /* __CS_DEBUG__ */

#line 158 "386.nona"
{
    printf( "lea eax, [eax * %d + %d]\n",
        1 << IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node))), IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (CS_OPERAND_1_OF_2 (node)))) << IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node))) );
  }
#line 1409 "386.c"
      break;
    case 19:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (CS_OPERAND_1_OF_2 (node))), CSNT_fetch_reg);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `shl : IR_NM_e_shl ( IR_NM_e_plus ( fetch_reg, IR_NM_e_icon), IR_NM_e_icon)'\n");
#endif /* __CS_DEBUG__ */

#line 166 "386.nona"
{
    printf( "lea eax, [%s * %d + %d]\n",
        IR_name(nonterm_attr_1),
        1 << IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node))), IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (CS_OPERAND_1_OF_2 (node)))) << IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node))) );
  }
#line 1425 "386.c"
      break;
    case 20:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `const : IR_NM_e_icon'\n");
#endif /* __CS_DEBUG__ */

#line 174 "386.nona"
{
    NeedEax();
    printf( "mov eax, %d\n", IR_value( CS_ATTRIBUTE (node) ) );  
  }
#line 1437 "386.c"
      break;
    case 21:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_fetch);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `expr : fetch'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 22:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_add);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `expr : add'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 23:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_shl);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `expr : shl'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 24:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_const);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `expr : const'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 25:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_lvalue);
      nonterm_attr_2
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `assign : IR_NM_e_assign ( lvalue, expr)'\n");
#endif /* __CS_DEBUG__ */

#line 186 "386.nona"
{
    printf( "pop ebx\n" );
    --eax_used;
    printf( "mov [ebx], eax\n" );
  }
#line 1488 "386.c"
      break;
    case 26:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `assign : IR_NM_e_assign ( IR_NM_e_sym, expr)'\n");
#endif /* __CS_DEBUG__ */

#line 194 "386.nona"
{
    printf( "mov %s, eax\n", IR_name(CS_ATTRIBUTE (CS_OPERAND_1_OF_2 (node))) );
  }
#line 1502 "386.c"
      break;
    case 27:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `assign : IR_NM_e_assign ( IR_NM_e_sym, IR_NM_e_icon)'\n");
#endif /* __CS_DEBUG__ */

#line 200 "386.nona"
{
    GetDirect( CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node)) );
    printf( "mov %s, %s\n", IR_name(CS_ATTRIBUTE (CS_OPERAND_1_OF_2 (node))), OperBuf );
  }
#line 1514 "386.c"
      break;
    default:
      CS_ERROR ("CS_traverse_cover: incorrect cover");
      abort ();
      break;
    }
  cover->costs_or_attrs.nonterminal_attribute [nonterminal] = result;
  cover->pass_flag [nonterminal] = 1;
  return result;
}

CS_TYPE CS_traverse_cover (CS_cover cover, int nonterminal)
{
  if (cover == NULL)
    {
      CS_ERROR ("CS_traverse_cover: incorrect cover");
      abort ();
    }
  _CS_traverse_cover_1 (cover, nonterminal);
  return _CS_traverse_cover_2 (cover, nonterminal);
}

static void _CS_delete_cover (CS_cover cover)
{
  CS_node node;

  if (cover == NULL)
    /* Corresponding node has been processed. */
    return;
  if (cover->node == NULL)
    {
      CS_ERROR ("CS_delete_cover: incorrect cover");
      abort ();
    }
  node = cover->node;
  switch (CS_OPERATION (node))
    {
    case IR_NM_e_plus:
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_2_OF_2 (node)));
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_1_OF_2 (node)));
      break;
    case IR_NM_e_minus:
      break;
    case IR_NM_e_shl:
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_2_OF_2 (node)));
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_1_OF_2 (node)));
      break;
    case IR_NM_e_shr:
      break;
    case IR_NM_e_assign:
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_2_OF_2 (node)));
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_1_OF_2 (node)));
      break;
    case IR_NM_e_fetch:
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_1_OF_1 (node)));
      break;
    case IR_NM_e_sym:
      break;
    case IR_NM_e_icon:
      break;
    default:
      CS_ERROR ("CS_delete_cover: unknown operation code");
      abort ();
      break;
    }
  CS_FREE (cover, sizeof (struct _CS_state));
  CS_SET_STATE (node, NULL);
}

void CS_delete_cover (CS_cover cover)
{
  if (cover == NULL)
    {
      CS_ERROR ("CS_delete_cover: incorrect cover");
      abort ();
    }
  _CS_delete_cover (cover);
}

void CS_start (void)
{
  CS_START_ALLOC ();
}

void CS_finish (void)
{
  CS_FINISH_ALLOC ();
}



#line 207 "386.nona"


void do_print ( IR_node_t node )
{
  IR_print_node( node, 0 );
};

void test ( void )
{ 
  IR_node_t x;
  CS_cover cover;

  // a = b + 2 >> 1;

  x =
    IR_new_e_assign(
      IR_new_e_sym( "edi", 1 ),
      IR_new_e_shl(
        IR_new_e_plus(
          IR_new_e_fetch( IR_new_e_sym("esi", 1) ),
          IR_new_e_icon( 10 )
        ),
        IR_new_e_icon( 3 )
      )
    );

/*
  x =
    IR_new_e_assign(
      IR_new_e_sym( "a", 0 ),
      IR_new_e_plus(
        IR_new_e_fetch( IR_new_e_sym("esi",1) ),
        IR_new_e_icon( 10 )
      )
    );
*/
//  IR_traverse_depth_first( x, 1, do_print );

  cover = CS_find_cover( x );
  CS_traverse_cover( cover, CSNT_assign );
  CS_delete_cover( cover );

  IR_stop();
}

int main ( void )
{
  IR_start();
  CS_start();

  test();

  CS_finish();
  IR_stop();
  return 0;
};



#line 1666 "386.c"
