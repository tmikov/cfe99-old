#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "8051.h"


#line 23 "8051.nona"

int usage = 0;
int label = 0;

void NeedAcc ( void )
{
  ++usage;
  if (usage > 1)
  {
    printf( "push ACC\n" );
    printf( "push _R1\n" );
  }  
};

void FreeAcc ( void )
{
  --usage;
};

void PopAcc ( int o )
{
  if (o == 0)
  {
    printf( "pop _R1\n" );
    printf( "pop ACC\n" );
  }
  else
  {
    printf( "pop _R3\n" );
    printf( "pop _R2\n" );
  }
  FreeAcc();
};



#line 45 "8051.c"
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
    CS_TYPE nonterminal_attribute [11];
    _CS_cost cost [11];
  } costs_or_attrs;
  char pass_flag [11];
  struct
  {
    unsigned int _CS_fetch_byte: 1;
    unsigned int _CS_fetch_word: 1;
    unsigned int _CS_fetch: 2;
    unsigned int _CS_add: 3;
    unsigned int _CS_shl: 2;
    unsigned int _CS_cvt_to_word: 1;
    unsigned int _CS_cvt_to_byte: 1;
    unsigned int _CS_cvt: 2;
    unsigned int _CS_const: 2;
    unsigned int _CS_expr: 3;
    unsigned int _CS_assign: 2;
  } rule;

};

static short _CS_fetch_byte_decode_rule_vector [] =
{
  0, 1
};

static short _CS_fetch_word_decode_rule_vector [] =
{
  0, 2
};

static short _CS_fetch_decode_rule_vector [] =
{
  0, 3, 4
};

static short _CS_add_decode_rule_vector [] =
{
  0, 5, 6, 7, 8
};

static short _CS_shl_decode_rule_vector [] =
{
  0, 9, 10
};

static short _CS_cvt_to_word_decode_rule_vector [] =
{
  0, 11
};

static short _CS_cvt_to_byte_decode_rule_vector [] =
{
  0, 12
};

static short _CS_cvt_decode_rule_vector [] =
{
  0, 13, 14
};

static short _CS_const_decode_rule_vector [] =
{
  0, 15, 16
};

static short _CS_expr_decode_rule_vector [] =
{
  0, 17, 18, 19, 20, 21
};

static short _CS_assign_decode_rule_vector [] =
{
  0, 22, 23
};

static void _CS_closure_fetch_byte
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_fetch_word
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_fetch
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_add
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_shl
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_cvt_to_word
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_cvt_to_byte
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_cvt
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_const
  (struct _CS_state *state, CS_node node, int cost);

static void _CS_closure_fetch_byte
  (struct _CS_state *state, CS_node node, int cost)
{
  int all_cost;

  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_fetch])
    {
      /* fetch : fetch_byte */
      state->costs_or_attrs.cost [CSNT_fetch] = all_cost;
      state->rule._CS_fetch = 2;
      _CS_closure_fetch (state, node, all_cost);
    }
}

static void _CS_closure_fetch_word
  (struct _CS_state *state, CS_node node, int cost)
{
  int all_cost;

  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_fetch])
    {
      /* fetch : fetch_word */
      state->costs_or_attrs.cost [CSNT_fetch] = all_cost;
      state->rule._CS_fetch = 1;
      _CS_closure_fetch (state, node, all_cost);
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

static void _CS_closure_cvt_to_word
  (struct _CS_state *state, CS_node node, int cost)
{
  int all_cost;

  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_cvt])
    {
      /* cvt : cvt_to_word */
      state->costs_or_attrs.cost [CSNT_cvt] = all_cost;
      state->rule._CS_cvt = 2;
      _CS_closure_cvt (state, node, all_cost);
    }
}

static void _CS_closure_cvt_to_byte
  (struct _CS_state *state, CS_node node, int cost)
{
  int all_cost;

  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_cvt])
    {
      /* cvt : cvt_to_byte */
      state->costs_or_attrs.cost [CSNT_cvt] = all_cost;
      state->rule._CS_cvt = 1;
      _CS_closure_cvt (state, node, all_cost);
    }
}

static void _CS_closure_cvt
  (struct _CS_state *state, CS_node node, int cost)
{
  int all_cost;

  all_cost = cost;
  if (all_cost < state->costs_or_attrs.cost [CSNT_expr])
    {
      /* expr : cvt */
      state->costs_or_attrs.cost [CSNT_expr] = all_cost;
      state->rule._CS_expr = 5;
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
  switch (CS_OPERATION (node))
    {
    case IR_NM_e_plus:
      if (_CS_STATE (CS_OPERAND_1_OF_2 (node))->rule._CS_expr
          && _CS_STATE (CS_OPERAND_2_OF_2 (node))->rule._CS_expr)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
              + _CS_STATE (CS_OPERAND_2_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 81 "8051.nona"
 + 4;
#line 369 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_add])
            {
              /* add : IR_NM_e_plus ( expr, expr) */
              state->costs_or_attrs.cost [CSNT_add] = cost;
              state->rule._CS_add = 1;
              _CS_closure_add (state, node, cost);
            }
        }
      if (_CS_STATE (CS_OPERAND_1_OF_2 (node))->rule._CS_expr
          && CS_OPERATION (CS_OPERAND_2_OF_2 (node)) == IR_NM_e_fetch
          && CS_OPERATION (CS_OPERAND_1_OF_1 (CS_OPERAND_2_OF_2 (node))) == IR_NM_e_sym)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 91 "8051.nona"
 + 4;
#line 387 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_add])
            {
              /* add : IR_NM_e_plus ( expr, IR_NM_e_fetch ( IR_NM_e_sym)) */
              state->costs_or_attrs.cost [CSNT_add] = cost;
              state->rule._CS_add = 2;
              _CS_closure_add (state, node, cost);
            }
        }
      if (_CS_STATE (CS_OPERAND_1_OF_2 (node))->rule._CS_expr
          && CS_OPERATION (CS_OPERAND_2_OF_2 (node)) == IR_NM_e_cvt
          && CS_OPERATION (CS_OPERAND_1_OF_1 (CS_OPERAND_2_OF_2 (node))) == IR_NM_e_fetch
          && CS_OPERATION (CS_OPERAND_1_OF_1 (CS_OPERAND_1_OF_1 (CS_OPERAND_2_OF_2 (node)))) == IR_NM_e_sym)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 100 "8051.nona"
 + 4;
#line 406 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_add])
            {
              /* add : IR_NM_e_plus ( expr, IR_NM_e_cvt ( IR_NM_e_fetch ( IR_NM_e_sym))) */
              state->costs_or_attrs.cost [CSNT_add] = cost;
              state->rule._CS_add = 3;
              _CS_closure_add (state, node, cost);
            }
        }
      if (_CS_STATE (CS_OPERAND_1_OF_2 (node))->rule._CS_expr
          && CS_OPERATION (CS_OPERAND_2_OF_2 (node)) == IR_NM_e_icon)
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 109 "8051.nona"
 + 4;
#line 423 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_add])
            {
              /* add : IR_NM_e_plus ( expr, IR_NM_e_icon) */
              state->costs_or_attrs.cost [CSNT_add] = cost;
              state->rule._CS_add = 4;
              _CS_closure_add (state, node, cost);
            }
        }
      break;
    case IR_NM_e_shl:
      if (_CS_STATE (CS_OPERAND_1_OF_2 (node))->rule._CS_expr
          && CS_OPERATION (CS_OPERAND_2_OF_2 (node)) == IR_NM_e_icon &&
#line 119 "8051.nona"
IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node))) == 1)
#line 438 "8051.c"
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 118 "8051.nona"
 + 4;
#line 445 "8051.c"
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
#line 128 "8051.nona"
 + 8;
#line 463 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_shl])
            {
              /* shl : IR_NM_e_shl ( expr, expr) */
              state->costs_or_attrs.cost [CSNT_shl] = cost;
              state->rule._CS_shl = 2;
              _CS_closure_shl (state, node, cost);
            }
        }
      break;
    case IR_NM_e_assign:
      if (CS_OPERATION (CS_OPERAND_1_OF_2 (node)) == IR_NM_e_sym
          && _CS_STATE (CS_OPERAND_2_OF_2 (node))->rule._CS_expr &&
#line 175 "8051.nona"
IR_len(CS_ATTRIBUTE (node)) == 2)
#line 478 "8051.c"
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_2_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 174 "8051.nona"
 + 2;
#line 485 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_assign])
            {
              /* assign : IR_NM_e_assign ( IR_NM_e_sym, expr) */
              state->costs_or_attrs.cost [CSNT_assign] = cost;
              state->rule._CS_assign = 1;
            }
        }
      if (CS_OPERATION (CS_OPERAND_1_OF_2 (node)) == IR_NM_e_sym
          && _CS_STATE (CS_OPERAND_2_OF_2 (node))->rule._CS_expr &&
#line 183 "8051.nona"
IR_len(CS_ATTRIBUTE (node)) == 1)
#line 497 "8051.c"
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_2_OF_2 (node))->costs_or_attrs.cost [CSNT_expr]
#line 182 "8051.nona"
 + 1;
#line 504 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_assign])
            {
              /* assign : IR_NM_e_assign ( IR_NM_e_sym, expr) */
              state->costs_or_attrs.cost [CSNT_assign] = cost;
              state->rule._CS_assign = 2;
            }
        }
      break;
    case IR_NM_e_fetch:
      if (CS_OPERATION (CS_OPERAND_1_OF_1 (node)) == IR_NM_e_sym &&
#line 62 "8051.nona"
IR_len(CS_ATTRIBUTE (node)) == 1)
#line 517 "8051.c"
        {
          cost
            = 0
#line 61 "8051.nona"
 + 1;
#line 523 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_fetch_byte])
            {
              /* fetch_byte : IR_NM_e_fetch ( IR_NM_e_sym) */
              state->costs_or_attrs.cost [CSNT_fetch_byte] = cost;
              state->rule._CS_fetch_byte = 1;
              _CS_closure_fetch_byte (state, node, cost);
            }
        }
      if (CS_OPERATION (CS_OPERAND_1_OF_1 (node)) == IR_NM_e_sym &&
#line 70 "8051.nona"
IR_len(CS_ATTRIBUTE (node)) == 2)
#line 535 "8051.c"
        {
          cost
            = 0
#line 69 "8051.nona"
 + 2;
#line 541 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_fetch_word])
            {
              /* fetch_word : IR_NM_e_fetch ( IR_NM_e_sym) */
              state->costs_or_attrs.cost [CSNT_fetch_word] = cost;
              state->rule._CS_fetch_word = 1;
              _CS_closure_fetch_word (state, node, cost);
            }
        }
      break;
    case IR_NM_e_cvt:
      if (_CS_STATE (CS_OPERAND_1_OF_1 (node))->rule._CS_expr &&
#line 136 "8051.nona"
IR_len(CS_ATTRIBUTE (node)) == 2)
#line 555 "8051.c"
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_1 (node))->costs_or_attrs.cost [CSNT_expr]
#line 135 "8051.nona"
 + 1;
#line 562 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_cvt_to_word])
            {
              /* cvt_to_word : IR_NM_e_cvt ( expr) */
              state->costs_or_attrs.cost [CSNT_cvt_to_word] = cost;
              state->rule._CS_cvt_to_word = 1;
              _CS_closure_cvt_to_word (state, node, cost);
            }
        }
      if (_CS_STATE (CS_OPERAND_1_OF_1 (node))->rule._CS_expr &&
#line 143 "8051.nona"
IR_len(CS_ATTRIBUTE (node)) == 1)
#line 574 "8051.c"
        {
          cost
            = 0
              + _CS_STATE (CS_OPERAND_1_OF_1 (node))->costs_or_attrs.cost [CSNT_expr]
#line 142 "8051.nona"
 + 0;
#line 581 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_cvt_to_byte])
            {
              /* cvt_to_byte : IR_NM_e_cvt ( expr) */
              state->costs_or_attrs.cost [CSNT_cvt_to_byte] = cost;
              state->rule._CS_cvt_to_byte = 1;
              _CS_closure_cvt_to_byte (state, node, cost);
            }
        }
      break;
    case IR_NM_e_sym:
      break;
    case IR_NM_e_icon:
      if (
#line 152 "8051.nona"
IR_len(CS_ATTRIBUTE (node)) == 1)
#line 597 "8051.c"
        {
          cost
            = 0
#line 151 "8051.nona"
 + 1;
#line 603 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_const])
            {
              /* const : IR_NM_e_icon */
              state->costs_or_attrs.cost [CSNT_const] = cost;
              state->rule._CS_const = 1;
              _CS_closure_const (state, node, cost);
            }
        }
      if (
#line 160 "8051.nona"
IR_len(CS_ATTRIBUTE (node)) == 2)
#line 615 "8051.c"
        {
          cost
            = 0
#line 159 "8051.nona"
 + 2;
#line 621 "8051.c"
          if (cost < state->costs_or_attrs.cost [CSNT_const])
            {
              /* const : IR_NM_e_icon */
              state->costs_or_attrs.cost [CSNT_const] = cost;
              state->rule._CS_const = 2;
              _CS_closure_const (state, node, cost);
            }
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
  switch (state->rule._CS_fetch_byte)
    {
    case 1:
      fprintf (stderr, "       rule `fetch_byte : IR_NM_e_fetch ( IR_NM_e_sym)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_fetch_byte]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_fetch_word)
    {
    case 1:
      fprintf (stderr, "       rule `fetch_word : IR_NM_e_fetch ( IR_NM_e_sym)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_fetch_word]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_fetch)
    {
    case 1:
      fprintf (stderr, "       rule `fetch : fetch_word'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_fetch]);
      break;
    case 2:
      fprintf (stderr, "       rule `fetch : fetch_byte'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_fetch]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_add)
    {
    case 1:
      fprintf (stderr, "       rule `add : IR_NM_e_plus ( expr, expr)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_add]);
      break;
    case 2:
      fprintf (stderr, "       rule `add : IR_NM_e_plus ( expr, IR_NM_e_fetch ( IR_NM_e_sym))'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_add]);
      break;
    case 3:
      fprintf (stderr, "       rule `add : IR_NM_e_plus ( expr, IR_NM_e_cvt ( IR_NM_e_fetch ( IR_NM_e_sym)))'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_add]);
      break;
    case 4:
      fprintf (stderr, "       rule `add : IR_NM_e_plus ( expr, IR_NM_e_icon)'");
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
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_cvt_to_word)
    {
    case 1:
      fprintf (stderr, "       rule `cvt_to_word : IR_NM_e_cvt ( expr)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_cvt_to_word]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_cvt_to_byte)
    {
    case 1:
      fprintf (stderr, "       rule `cvt_to_byte : IR_NM_e_cvt ( expr)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_cvt_to_byte]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_cvt)
    {
    case 1:
      fprintf (stderr, "       rule `cvt : cvt_to_byte'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_cvt]);
      break;
    case 2:
      fprintf (stderr, "       rule `cvt : cvt_to_word'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_cvt]);
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
    case 2:
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
    case 5:
      fprintf (stderr, "       rule `expr : cvt'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_expr]);
      break;
    default:
      /* Nothing */
      break;
    }
  switch (state->rule._CS_assign)
    {
    case 1:
      fprintf (stderr, "       rule `assign : IR_NM_e_assign ( IR_NM_e_sym, expr)'");
      fprintf (stderr, ", overall cost %d\n", state->costs_or_attrs.cost [CSNT_assign]);
      break;
    case 2:
      fprintf (stderr, "       rule `assign : IR_NM_e_assign ( IR_NM_e_sym, expr)'");
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
    case IR_NM_e_shl:
      _CS_find_cover_pass_1 (CS_OPERAND_1_OF_2 (node));
      _CS_find_cover_pass_1 (CS_OPERAND_2_OF_2 (node));
      break;
    case IR_NM_e_assign:
      _CS_find_cover_pass_1 (CS_OPERAND_1_OF_2 (node));
      _CS_find_cover_pass_1 (CS_OPERAND_2_OF_2 (node));
      break;
    case IR_NM_e_fetch:
      _CS_find_cover_pass_1 (CS_OPERAND_1_OF_1 (node));
      break;
    case IR_NM_e_cvt:
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
    case IR_NM_e_shl:
      _CS_find_cover_pass_2 (CS_OPERAND_1_OF_2 (node));
      _CS_find_cover_pass_2 (CS_OPERAND_2_OF_2 (node));
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_find_cover: terminal IR_NM_e_shl, state:\n");
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
    case IR_NM_e_cvt:
      _CS_find_cover_pass_2 (CS_OPERAND_1_OF_1 (node));
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_find_cover: terminal IR_NM_e_cvt, state:\n");
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
    case CSNT_fetch_byte:
      return _CS_fetch_byte_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_fetch_byte];
    case CSNT_fetch_word:
      return _CS_fetch_word_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_fetch_word];
    case CSNT_fetch:
      return _CS_fetch_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_fetch];
    case CSNT_add:
      return _CS_add_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_add];
    case CSNT_shl:
      return _CS_shl_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_shl];
    case CSNT_cvt_to_word:
      return _CS_cvt_to_word_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_cvt_to_word];
    case CSNT_cvt_to_byte:
      return _CS_cvt_to_byte_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_cvt_to_byte];
    case CSNT_cvt:
      return _CS_cvt_decode_rule_vector
               [((struct _CS_state *) cover)->rule._CS_cvt];
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
      /* fetch_byte : IR_NM_e_fetch ( IR_NM_e_sym). */
      break;
    case 2:
      /* fetch_word : IR_NM_e_fetch ( IR_NM_e_sym). */
      break;
    case 3:
      /* fetch : fetch_word. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_fetch_word);
      break;
    case 4:
      /* fetch : fetch_byte. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_fetch_byte);
      break;
    case 5:
      /* add : IR_NM_e_plus ( expr, expr). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
      break;
    case 6:
      /* add : IR_NM_e_plus ( expr, IR_NM_e_fetch ( IR_NM_e_sym)). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      break;
    case 7:
      /* add : IR_NM_e_plus ( expr, IR_NM_e_cvt ( IR_NM_e_fetch ( IR_NM_e_sym))). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      break;
    case 8:
      /* add : IR_NM_e_plus ( expr, IR_NM_e_icon). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      break;
    case 9:
      /* shl : IR_NM_e_shl ( expr, IR_NM_e_icon). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      break;
    case 10:
      /* shl : IR_NM_e_shl ( expr, expr). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
      break;
    case 11:
      /* cvt_to_word : IR_NM_e_cvt ( expr). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_1 (node)), CSNT_expr);
      break;
    case 12:
      /* cvt_to_byte : IR_NM_e_cvt ( expr). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_1_OF_1 (node)), CSNT_expr);
      break;
    case 13:
      /* cvt : cvt_to_byte. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_cvt_to_byte);
      break;
    case 14:
      /* cvt : cvt_to_word. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_cvt_to_word);
      break;
    case 15:
      /* const : IR_NM_e_icon. */
      break;
    case 16:
      /* const : IR_NM_e_icon. */
      break;
    case 17:
      /* expr : fetch. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_fetch);
      break;
    case 18:
      /* expr : add. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_add);
      break;
    case 19:
      /* expr : shl. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_shl);
      break;
    case 20:
      /* expr : const. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_const);
      break;
    case 21:
      /* expr : cvt. */
      _CS_traverse_cover_1
          (_CS_STATE (node), CSNT_cvt);
      break;
    case 22:
      /* assign : IR_NM_e_assign ( IR_NM_e_sym, expr). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
      break;
    case 23:
      /* assign : IR_NM_e_assign ( IR_NM_e_sym, expr). */
      _CS_traverse_cover_1
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
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
      fprintf (stderr, "CS_traverse_cover: action for `fetch_byte : IR_NM_e_fetch ( IR_NM_e_sym)'\n");
#endif /* __CS_DEBUG__ */

#line 63 "8051.nona"
{
    NeedAcc();
    printf( "mov A,%s\n", IR_name(CS_ATTRIBUTE (CS_OPERAND_1_OF_1 (node))) );
  }
#line 1125 "8051.c"
      break;
    case 2:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `fetch_word : IR_NM_e_fetch ( IR_NM_e_sym)'\n");
#endif /* __CS_DEBUG__ */

#line 71 "8051.nona"
{
    NeedAcc();
    printf( "mov A,%s\n", IR_name(CS_ATTRIBUTE (CS_OPERAND_1_OF_1 (node))) );
    printf( "mov R1,%s+1\n", IR_name(CS_ATTRIBUTE (CS_OPERAND_1_OF_1 (node))) );
  }
#line 1138 "8051.c"
      break;
    case 3:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_fetch_word);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `fetch : fetch_word'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 4:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_fetch_byte);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `fetch : fetch_byte'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 5:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      nonterm_attr_2
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `add : IR_NM_e_plus ( expr, expr)'\n");
#endif /* __CS_DEBUG__ */

#line 82 "8051.nona"
{
    PopAcc( 1 );
    printf( "add A,R2\n" );
    printf( "xch A,R1\n" );
    printf( "addc A,R3\n" );
    printf( "xch A,R1\n" );
  }
#line 1175 "8051.c"
      break;
    case 6:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `add : IR_NM_e_plus ( expr, IR_NM_e_fetch ( IR_NM_e_sym))'\n");
#endif /* __CS_DEBUG__ */

#line 92 "8051.nona"
{
    printf( "add A,%s\n", IR_name(CS_ATTRIBUTE (CS_OPERAND_1_OF_1 (CS_OPERAND_2_OF_2 (node)))) );
    printf( "xch A,R1\n" );
    printf( "addc A,%s+1\n", IR_name(CS_ATTRIBUTE (CS_OPERAND_1_OF_1 (CS_OPERAND_2_OF_2 (node)))) );
    printf( "xch A,R1\n" );
  }
#line 1192 "8051.c"
      break;
    case 7:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `add : IR_NM_e_plus ( expr, IR_NM_e_cvt ( IR_NM_e_fetch ( IR_NM_e_sym)))'\n");
#endif /* __CS_DEBUG__ */

#line 101 "8051.nona"
{
    printf( "add A,%s\n", IR_name(CS_ATTRIBUTE (CS_OPERAND_1_OF_1 (CS_OPERAND_1_OF_1 (CS_OPERAND_2_OF_2 (node))))) );
    printf( "xch A,R1\n" );
    printf( "addc A,#0\n" );
    printf( "xch A,R1\n" );
  }
#line 1209 "8051.c"
      break;
    case 8:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `add : IR_NM_e_plus ( expr, IR_NM_e_icon)'\n");
#endif /* __CS_DEBUG__ */

#line 110 "8051.nona"
{
    printf( "add A,#%d\n", IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node))) & 255 );
    printf( "xch A,R1\n" );
    printf( "addc A,#%d\n", IR_value(CS_ATTRIBUTE (CS_OPERAND_2_OF_2 (node))) >> 8 );
    printf( "xch A,R1\n" );
  }
#line 1226 "8051.c"
      break;
    case 9:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `shl : IR_NM_e_shl ( expr, IR_NM_e_icon)'\n");
#endif /* __CS_DEBUG__ */

#line 120 "8051.nona"
{
    printf( "add A,A\n" );
    printf( "xch A,R1\n" );
    printf( "addc A,A\n" );
    printf( "xch A,R1\n" );
  }
#line 1243 "8051.c"
      break;
    case 10:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_2 (node)), CSNT_expr);
      nonterm_attr_2
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `shl : IR_NM_e_shl ( expr, expr)'\n");
#endif /* __CS_DEBUG__ */

#line 129 "8051.nona"
{
    PopAcc( 1 );
    printf( "lcall shl_R3R2_by_R1A\n" );
  }
#line 1261 "8051.c"
      break;
    case 11:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_1 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `cvt_to_word : IR_NM_e_cvt ( expr)'\n");
#endif /* __CS_DEBUG__ */

#line 137 "8051.nona"
{
    printf( "mov R1,#0\n" );
  }
#line 1275 "8051.c"
      break;
    case 12:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_1_OF_1 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `cvt_to_byte : IR_NM_e_cvt ( expr)'\n");
#endif /* __CS_DEBUG__ */

#line 144 "8051.nona"
{
  }
#line 1288 "8051.c"
      break;
    case 13:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_cvt_to_byte);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `cvt : cvt_to_byte'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 14:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_cvt_to_word);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `cvt : cvt_to_word'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 15:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `const : IR_NM_e_icon'\n");
#endif /* __CS_DEBUG__ */

#line 153 "8051.nona"
{
    NeedAcc();
    printf( "mov A,#%d\n", IR_value(CS_ATTRIBUTE (node)) & 255 );
  }
#line 1316 "8051.c"
      break;
    case 16:
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `const : IR_NM_e_icon'\n");
#endif /* __CS_DEBUG__ */

#line 161 "8051.nona"
{
    NeedAcc();
    printf( "mov A,#%d\n", IR_value(CS_ATTRIBUTE (node)) & 255 );
    printf( "mov R1,#%d\n", IR_value(CS_ATTRIBUTE (node)) >> 8 );
  }
#line 1329 "8051.c"
      break;
    case 17:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_fetch);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `expr : fetch'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 18:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_add);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `expr : add'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 19:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_shl);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `expr : shl'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 20:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_const);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `expr : const'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 21:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (node), CSNT_cvt);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `expr : cvt'\n");
#endif /* __CS_DEBUG__ */
      break;
    case 22:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `assign : IR_NM_e_assign ( IR_NM_e_sym, expr)'\n");
#endif /* __CS_DEBUG__ */

#line 176 "8051.nona"
{
    printf( "mov %s,A\n", IR_name(CS_ATTRIBUTE (CS_OPERAND_1_OF_2 (node))) );
    printf( "mov %s+1,_R1\n", IR_name(CS_ATTRIBUTE (CS_OPERAND_1_OF_2 (node))) );
  }
#line 1384 "8051.c"
      break;
    case 23:
      nonterm_attr_1
        = _CS_traverse_cover_2
          (_CS_STATE (CS_OPERAND_2_OF_2 (node)), CSNT_expr);
#ifdef __CS_DEBUG__
      fprintf (stderr, "CS_traverse_cover: action for `assign : IR_NM_e_assign ( IR_NM_e_sym, expr)'\n");
#endif /* __CS_DEBUG__ */

#line 184 "8051.nona"
{
    printf( "mov %s,A\n", IR_name(CS_ATTRIBUTE (CS_OPERAND_1_OF_2 (node))) );
  }
#line 1398 "8051.c"
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
    case IR_NM_e_shl:
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_2_OF_2 (node)));
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_1_OF_2 (node)));
      break;
    case IR_NM_e_assign:
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_2_OF_2 (node)));
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_1_OF_2 (node)));
      break;
    case IR_NM_e_fetch:
      _CS_delete_cover ((CS_cover) CS_STATE (CS_OPERAND_1_OF_1 (node)));
      break;
    case IR_NM_e_cvt:
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



#line 191 "8051.nona"


void do_print ( IR_node_t node )
{
  IR_print_node( node, 0 );
};

void test ( void )
{ 
  IR_node_t x;
  CS_cover cover;

  // w = b + 1000 >> 1;

/*
  x =
    IR_new_e_assign( 2, 
      IR_new_e_sym( 2, "w" ),
      IR_new_e_shl( 2,
        IR_new_e_plus( 2,
          IR_new_e_cvt( 2,
            IR_new_e_fetch( 1, IR_new_e_sym(1,"b") ) ),
          IR_new_e_icon( 2, 1000 )
        ),
        IR_new_e_icon( 2, 1 )
      )
    );
*/
  x =
   IR_new_e_assign( 2,
      IR_new_e_sym( 2, "x" ),
      IR_new_e_plus( 2, 
        IR_new_e_plus( 2, 
          IR_new_e_cvt( 2, IR_new_e_fetch( 1, IR_new_e_sym( 1, "a" ) ) ),
          IR_new_e_cvt( 2, IR_new_e_fetch( 1, IR_new_e_sym( 1, "b" ) ) )
        ),
        IR_new_e_cvt( 2, IR_new_e_fetch( 1, IR_new_e_sym( 1, "c" ) ) )
      )
   );



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



#line 1555 "8051.c"
