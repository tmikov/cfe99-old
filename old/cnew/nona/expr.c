#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "expr.h"

#ifndef IR_START_ALLOC
#define IR_START_ALLOC()
#endif

#ifndef IR_STOP_ALLOC
#define IR_STOP_ALLOC()
#endif

#ifndef IR_ALLOC
#define IR_ALLOC(ptr, size, ptr_type) ((ptr) = (ptr_type) malloc (size))
#endif

#ifndef IR_FREE
#define IR_FREE(ptr, size) free (ptr)
#endif

#ifndef IR_BEGIN_IR_node_t
#define IR_BEGIN_IR_node_t(a) memset (&(a), 0, sizeof (a))
#endif

#ifndef IR_END_IR_node_t
#define IR_END_IR_node_t(a)
#endif

#ifndef IR_COPY_IR_node_t
#define IR_COPY_IR_node_t(a, b) ((a) = (b))
#endif

#ifndef IR_EQUAL_IR_node_t
#define IR_EQUAL_IR_node_t(a, b) ((a) == (b))
#endif

#ifndef IR_PRINT_IR_node_t
#define IR_PRINT_IR_node_t(a) printf ("%lx", (unsigned long) (a))
#endif

#ifndef IR_INPUT_IR_node_t
#define IR_INPUT_IR_node_t(file, field) (fread (&(field), sizeof (field), 1, (file)) != sizeof (field))
#endif

#ifndef IR_OUTPUT_IR_node_t
#define IR_OUTPUT_IR_node_t(file, field) (fwrite (&(field), sizeof (field), 1, (file)) != sizeof (field))
#endif

#ifndef IR_BEGIN_integer_t
#define IR_BEGIN_integer_t(a) memset (&(a), 0, sizeof (a))
#endif

#ifndef IR_END_integer_t
#define IR_END_integer_t(a)
#endif

#ifndef IR_COPY_integer_t
#define IR_COPY_integer_t(a, b) ((a) = (b))
#endif

#ifndef IR_EQUAL_integer_t
#define IR_EQUAL_integer_t(a, b) ((a) == (b))
#endif

#ifndef IR_PRINT_integer_t
#define IR_PRINT_integer_t(a)
#endif

#ifndef IR_INPUT_integer_t
#define IR_INPUT_integer_t(file, field) (fread (&(field), sizeof (field), 1, (file)) != sizeof (field))
#endif

#ifndef IR_OUTPUT_integer_t
#define IR_OUTPUT_integer_t(file, field) (fwrite (&(field), sizeof (field), 1, (file)) != sizeof (field))
#endif

#ifndef IR_BEGIN_string_t
#define IR_BEGIN_string_t(a) memset (&(a), 0, sizeof (a))
#endif

#ifndef IR_END_string_t
#define IR_END_string_t(a)
#endif

#ifndef IR_COPY_string_t
#define IR_COPY_string_t(a, b) ((a) = (b))
#endif

#ifndef IR_EQUAL_string_t
#define IR_EQUAL_string_t(a, b) ((a) == (b))
#endif

#ifndef IR_PRINT_string_t
#define IR_PRINT_string_t(a)
#endif

#ifndef IR_INPUT_string_t
#define IR_INPUT_string_t(file, field) (fread (&(field), sizeof (field), 1, (file)) != sizeof (field))
#endif

#ifndef IR_OUTPUT_string_t
#define IR_OUTPUT_string_t(file, field) (fwrite (&(field), sizeof (field), 1, (file)) != sizeof (field))
#endif

#ifndef IR_BEGIN_void_t
#define IR_BEGIN_void_t(a) memset (&(a), 0, sizeof (a))
#endif

#ifndef IR_END_void_t
#define IR_END_void_t(a)
#endif

#ifndef IR_COPY_void_t
#define IR_COPY_void_t(a, b) ((a) = (b))
#endif

#ifndef IR_EQUAL_void_t
#define IR_EQUAL_void_t(a, b) ((a) == (b))
#endif

#ifndef IR_PRINT_void_t
#define IR_PRINT_void_t(a)
#endif

#ifndef IR_INPUT_void_t
#define IR_INPUT_void_t(file, field) (fread (&(field), sizeof (field), 1, (file)) != sizeof (field))
#endif

#ifndef IR_OUTPUT_void_t
#define IR_OUTPUT_void_t(file, field) (fwrite (&(field), sizeof (field), 1, (file)) != sizeof (field))
#endif

#ifndef IR_BEGIN_bool_t
#define IR_BEGIN_bool_t(a) memset (&(a), 0, sizeof (a))
#endif

#ifndef IR_END_bool_t
#define IR_END_bool_t(a)
#endif

#ifndef IR_COPY_bool_t
#define IR_COPY_bool_t(a, b) ((a) = (b))
#endif

#ifndef IR_EQUAL_bool_t
#define IR_EQUAL_bool_t(a, b) ((a) == (b))
#endif

#ifndef IR_PRINT_bool_t
#define IR_PRINT_bool_t(a)
#endif

#ifndef IR_INPUT_bool_t
#define IR_INPUT_bool_t(file, field) (fread (&(field), sizeof (field), 1, (file)) != sizeof (field))
#endif

#ifndef IR_OUTPUT_bool_t
#define IR_OUTPUT_bool_t(file, field) (fwrite (&(field), sizeof (field), 1, (file)) != sizeof (field))
#endif

short _IR_node_level [] =
{
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

unsigned char _IR_SF_node [] =
{255, 31
};

unsigned char _IR_SF_expr [] =
{254, 31
};

unsigned char _IR_SF_binary [] =
{244, 1
};

unsigned char _IR_SF_unary [] =
{8, 18
};

unsigned char _IR_SF_e_plus [] =
{16, 0
};

unsigned char _IR_SF_e_minus [] =
{32, 0
};

unsigned char _IR_SF_e_shl [] =
{64, 0
};

unsigned char _IR_SF_e_shr [] =
{128, 0
};

unsigned char _IR_SF_e_assign [] =
{0, 1
};

unsigned char _IR_SF_e_fetch [] =
{0, 2
};

unsigned char _IR_SF_e_sym [] =
{0, 4
};

unsigned char _IR_SF_e_icon [] =
{0, 8
};

unsigned char _IR_SF_e_cvt [] =
{0, 16
};

unsigned char _IR_SF__root [] =
{255, 127
};

unsigned char _IR_SF__error [] =
{0, 64
};

unsigned char *_IR_is_type[] =
{
_IR_SF_node,
_IR_SF_expr,
_IR_SF_binary,
_IR_SF_unary,
_IR_SF_e_plus,
_IR_SF_e_minus,
_IR_SF_e_shl,
_IR_SF_e_shr,
_IR_SF_e_assign,
_IR_SF_e_fetch,
_IR_SF_e_sym,
_IR_SF_e_icon,
_IR_SF_e_cvt,
_IR_SF__root,
_IR_SF__error
};

void *_IR_class_structure_array [] =
{
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
};

char *IR_node_name[] =
{
  "node",
  "expr",
  "binary",
  "unary",
  "e_plus",
  "e_minus",
  "e_shl",
  "e_shr",
  "e_assign",
  "e_fetch",
  "e_sym",
  "e_icon",
  "e_cvt",
  "%root",
  "%error"
};

unsigned char IR_node_size[] =
{
  sizeof (_IR_node),
  sizeof (_IR_expr),
  sizeof (_IR_binary),
  sizeof (_IR_unary),
  sizeof (_IR_e_plus),
  sizeof (_IR_e_minus),
  sizeof (_IR_e_shl),
  sizeof (_IR_e_shr),
  sizeof (_IR_e_assign),
  sizeof (_IR_e_fetch),
  sizeof (_IR_e_sym),
  sizeof (_IR_e_icon),
  sizeof (_IR_e_cvt),
  sizeof (_IR__root),
  sizeof (_IR__error)
};

static int (* _IR_traverse_guard_function_variable) (IR_node_t node);

static int _IR_current_graph_pass_number;

void _IR_set_double_field_value
  (IR_double_link_t link, IR_node_t value, int disp, int flag)
{
}

IR_double_link_t IR__first_double_link (IR_node_t node)
{
  return NULL;
}

IR_double_link_t IR_F__next_double_link (IR_double_link_t prev_double_link)
{
#ifdef __IR_DEBUG__
  if (prev_double_link == NULL)
    abort ();
#endif /* __IR_DEBUG__ */
  return prev_double_link->next_link;
}

IR_double_link_t IR_F__previous_double_link (IR_double_link_t next_double_link)
{
#ifdef __IR_DEBUG__
  if (next_double_link == NULL)
    abort ();
#endif /* __IR_DEBUG__ */
  return next_double_link->previous_link;
}

IR_node_t IR_F__owner (IR_double_link_t link)
{
#ifdef __IR_DEBUG__
  if (link == NULL)
    abort ();
#endif /* __IR_DEBUG__ */
  return link->link_owner;
}

void_t IR_F_state (IR_node_t _node)
{
#ifdef __IR_DEBUG__
  if (_node == NULL || !((_IR_SF_expr [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
        abort ();
#endif /* __IR_DEBUG__ */
  return (((_IR_expr *) _node)->_IR_S_expr.state);
}

integer_t IR_F_len (IR_node_t _node)
{
#ifdef __IR_DEBUG__
  if (_node == NULL || !((_IR_SF_expr [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
        abort ();
#endif /* __IR_DEBUG__ */
  return (((_IR_expr *) _node)->_IR_S_expr.len);
}

IR_node_t IR_F_left (IR_node_t _node)
{
#ifdef __IR_DEBUG__
  if (_node == NULL || !((_IR_SF_binary [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
        abort ();
#endif /* __IR_DEBUG__ */
  return (((_IR_binary *) _node)->_IR_S_binary.left);
}

IR_node_t IR_F_right (IR_node_t _node)
{
#ifdef __IR_DEBUG__
  if (_node == NULL || !((_IR_SF_binary [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
        abort ();
#endif /* __IR_DEBUG__ */
  return (((_IR_binary *) _node)->_IR_S_binary.right);
}

IR_node_t IR_F_op (IR_node_t _node)
{
#ifdef __IR_DEBUG__
  if (_node == NULL || !((_IR_SF_unary [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
        abort ();
#endif /* __IR_DEBUG__ */
  return (((_IR_unary *) _node)->_IR_S_unary.op);
}

string_t IR_F_name (IR_node_t _node)
{
#ifdef __IR_DEBUG__
  if (_node == NULL || !((_IR_SF_e_sym [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
        abort ();
#endif /* __IR_DEBUG__ */
  return (((_IR_e_sym *) _node)->_IR_S_e_sym.name);
}

integer_t IR_F_value (IR_node_t _node)
{
#ifdef __IR_DEBUG__
  if (_node == NULL || !((_IR_SF_e_icon [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
        abort ();
#endif /* __IR_DEBUG__ */
  return (((_IR_e_icon *) _node)->_IR_S_e_icon.value);
}

void IR__set_double_link (IR_double_link_t link, IR_node_t value)
{
#ifdef __IR_DEBUG__
  if (link == NULL)
    abort ();
#endif /* __IR_DEBUG__ */
  (*link->set_function) (link->link_owner, value);
}

void IR_F_set_state (IR_node_t _node, void_t _value)
{
#ifdef __IR_DEBUG__
  if (_node == NULL
      || !((_IR_SF_expr [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
    abort ();
#endif /* __IR_DEBUG__ */
  (((_IR_expr *) _node)->_IR_S_expr.state) = _value;
}

void IR_F_set_len (IR_node_t _node, integer_t _value)
{
#ifdef __IR_DEBUG__
  if (_node == NULL
      || !((_IR_SF_expr [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
    abort ();
#endif /* __IR_DEBUG__ */
  (((_IR_expr *) _node)->_IR_S_expr.len) = _value;
}

void IR_F_set_left (IR_node_t _node, IR_node_t _value)
{
#ifdef __IR_DEBUG__
  if (_node == NULL
      || _value != NULL && !((_IR_SF_expr [IR_NODE_MODE (_value) / 8] >> (IR_NODE_MODE (_value) % 8)) & 1)
         && IR_NODE_MODE (_value) != IR_NM__error
      || !((_IR_SF_binary [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
    abort ();
#endif /* __IR_DEBUG__ */
  (((_IR_binary *) _node)->_IR_S_binary.left) = _value;
}

void IR_F_set_right (IR_node_t _node, IR_node_t _value)
{
#ifdef __IR_DEBUG__
  if (_node == NULL
      || _value != NULL && !((_IR_SF_expr [IR_NODE_MODE (_value) / 8] >> (IR_NODE_MODE (_value) % 8)) & 1)
         && IR_NODE_MODE (_value) != IR_NM__error
      || !((_IR_SF_binary [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
    abort ();
#endif /* __IR_DEBUG__ */
  (((_IR_binary *) _node)->_IR_S_binary.right) = _value;
}

void IR_F_set_op (IR_node_t _node, IR_node_t _value)
{
#ifdef __IR_DEBUG__
  if (_node == NULL
      || _value != NULL && !((_IR_SF_expr [IR_NODE_MODE (_value) / 8] >> (IR_NODE_MODE (_value) % 8)) & 1)
         && IR_NODE_MODE (_value) != IR_NM__error
      || !((_IR_SF_unary [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
    abort ();
#endif /* __IR_DEBUG__ */
  (((_IR_unary *) _node)->_IR_S_unary.op) = _value;
}

void IR_F_set_name (IR_node_t _node, string_t _value)
{
#ifdef __IR_DEBUG__
  if (_node == NULL
      || !((_IR_SF_e_sym [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
    abort ();
#endif /* __IR_DEBUG__ */
  (((_IR_e_sym *) _node)->_IR_S_e_sym.name) = _value;
}

void IR_F_set_value (IR_node_t _node, integer_t _value)
{
#ifdef __IR_DEBUG__
  if (_node == NULL
      || !((_IR_SF_e_icon [IR_NODE_MODE (_node) / 8] >> (IR_NODE_MODE (_node) % 8)) & 1))
    abort ();
#endif /* __IR_DEBUG__ */
  (((_IR_e_icon *) _node)->_IR_S_e_icon.value) = _value;
}

static void _IR_node_field_initiation (IR_node_mode_t node_mode, IR_node_t node)
{
  switch (node_mode)
    {
    case IR_NM_node:
      break;
    case IR_NM_expr:
      IR_BEGIN_void_t ((((_IR_expr *) node)->_IR_S_expr.state));
      IR_BEGIN_integer_t ((((_IR_expr *) node)->_IR_S_expr.len));
      break;
    case IR_NM_binary:
      _IR_node_field_initiation (IR_NM_expr, (IR_node_t) ((char *) &((_IR_binary *) node)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      IR_BEGIN_IR_node_t ((((_IR_binary *) node)->_IR_S_binary.left));
      IR_BEGIN_IR_node_t ((((_IR_binary *) node)->_IR_S_binary.right));
      break;
    case IR_NM_unary:
      _IR_node_field_initiation (IR_NM_expr, (IR_node_t) ((char *) &((_IR_unary *) node)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      IR_BEGIN_IR_node_t ((((_IR_unary *) node)->_IR_S_unary.op));
      break;
    case IR_NM_e_plus:
      _IR_node_field_initiation (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_plus *) node)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_minus:
      _IR_node_field_initiation (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_minus *) node)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_shl:
      _IR_node_field_initiation (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shl *) node)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_shr:
      _IR_node_field_initiation (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shr *) node)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_assign:
      _IR_node_field_initiation (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_assign *) node)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_fetch:
      _IR_node_field_initiation (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_fetch *) node)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)));
      break;
    case IR_NM_e_sym:
      _IR_node_field_initiation (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_sym *) node)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      IR_BEGIN_string_t ((((_IR_e_sym *) node)->_IR_S_e_sym.name));
      break;
    case IR_NM_e_icon:
      _IR_node_field_initiation (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_icon *) node)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      IR_BEGIN_integer_t ((((_IR_e_icon *) node)->_IR_S_e_icon.value));
      break;
    case IR_NM_e_cvt:
      _IR_node_field_initiation (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_cvt *) node)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)));
      break;
    case IR_NM__error:
      break;
    default:
      abort ();
      break;
    }
}

IR_node_t IR_create_node (IR_node_mode_t node_mode)
{
 IR_node_t result;

  IR_ALLOC (result, IR_node_size [node_mode], IR_node_t);
#ifdef __IR_DEBUG__
  if (result == NULL)
    abort ();
#endif /* __IR_DEBUG__ */
  IR_NODE_MODE (result) = node_mode;
  result->_IR_last_graph_pass_number = 0;
  _IR_node_field_initiation (node_mode, result);
  return result;
}

IR_node_t IR_new_e_plus
(integer_t len,
 IR_node_t left,
 IR_node_t right)
{
  IR_node_t result;

  result = IR_create_node (IR_NM_e_plus);
  (((_IR_e_plus *) result)->_IR_S_e_plus._IR_M_binary._IR_M_expr.len) = len;
  (((_IR_e_plus *) result)->_IR_S_e_plus._IR_M_binary.left) = left;
  (((_IR_e_plus *) result)->_IR_S_e_plus._IR_M_binary.right) = right;
  return result;
}

IR_node_t IR_new_e_minus
(integer_t len,
 IR_node_t left,
 IR_node_t right)
{
  IR_node_t result;

  result = IR_create_node (IR_NM_e_minus);
  (((_IR_e_minus *) result)->_IR_S_e_minus._IR_M_binary._IR_M_expr.len) = len;
  (((_IR_e_minus *) result)->_IR_S_e_minus._IR_M_binary.left) = left;
  (((_IR_e_minus *) result)->_IR_S_e_minus._IR_M_binary.right) = right;
  return result;
}

IR_node_t IR_new_e_shl
(integer_t len,
 IR_node_t left,
 IR_node_t right)
{
  IR_node_t result;

  result = IR_create_node (IR_NM_e_shl);
  (((_IR_e_shl *) result)->_IR_S_e_shl._IR_M_binary._IR_M_expr.len) = len;
  (((_IR_e_shl *) result)->_IR_S_e_shl._IR_M_binary.left) = left;
  (((_IR_e_shl *) result)->_IR_S_e_shl._IR_M_binary.right) = right;
  return result;
}

IR_node_t IR_new_e_shr
(integer_t len,
 IR_node_t left,
 IR_node_t right)
{
  IR_node_t result;

  result = IR_create_node (IR_NM_e_shr);
  (((_IR_e_shr *) result)->_IR_S_e_shr._IR_M_binary._IR_M_expr.len) = len;
  (((_IR_e_shr *) result)->_IR_S_e_shr._IR_M_binary.left) = left;
  (((_IR_e_shr *) result)->_IR_S_e_shr._IR_M_binary.right) = right;
  return result;
}

IR_node_t IR_new_e_assign
(integer_t len,
 IR_node_t left,
 IR_node_t right)
{
  IR_node_t result;

  result = IR_create_node (IR_NM_e_assign);
  (((_IR_e_assign *) result)->_IR_S_e_assign._IR_M_binary._IR_M_expr.len) = len;
  (((_IR_e_assign *) result)->_IR_S_e_assign._IR_M_binary.left) = left;
  (((_IR_e_assign *) result)->_IR_S_e_assign._IR_M_binary.right) = right;
  return result;
}

IR_node_t IR_new_e_fetch
(integer_t len,
 IR_node_t op)
{
  IR_node_t result;

  result = IR_create_node (IR_NM_e_fetch);
  (((_IR_e_fetch *) result)->_IR_S_e_fetch._IR_M_unary._IR_M_expr.len) = len;
  (((_IR_e_fetch *) result)->_IR_S_e_fetch._IR_M_unary.op) = op;
  return result;
}

IR_node_t IR_new_e_sym
(integer_t len,
 string_t name)
{
  IR_node_t result;

  result = IR_create_node (IR_NM_e_sym);
  (((_IR_e_sym *) result)->_IR_S_e_sym._IR_M_expr.len) = len;
  (((_IR_e_sym *) result)->_IR_S_e_sym.name) = name;
  return result;
}

IR_node_t IR_new_e_icon
(integer_t len,
 integer_t value)
{
  IR_node_t result;

  result = IR_create_node (IR_NM_e_icon);
  (((_IR_e_icon *) result)->_IR_S_e_icon._IR_M_expr.len) = len;
  (((_IR_e_icon *) result)->_IR_S_e_icon.value) = value;
  return result;
}

IR_node_t IR_new_e_cvt
(integer_t len,
 IR_node_t op)
{
  IR_node_t result;

  result = IR_create_node (IR_NM_e_cvt);
  (((_IR_e_cvt *) result)->_IR_S_e_cvt._IR_M_unary._IR_M_expr.len) = len;
  (((_IR_e_cvt *) result)->_IR_S_e_cvt._IR_M_unary.op) = op;
  return result;
}

static void _IR_free_node_fields (IR_node_mode_t node_mode, IR_node_t node)
{
  switch (node_mode)
    {
    case IR_NM_node:
      break;
    case IR_NM_expr:
      IR_END_integer_t ((((_IR_expr *) node)->_IR_S_expr.len));
      IR_END_void_t ((((_IR_expr *) node)->_IR_S_expr.state));
      break;
    case IR_NM_binary:
      IR_END_IR_node_t ((((_IR_binary *) node)->_IR_S_binary.right));
      IR_END_IR_node_t ((((_IR_binary *) node)->_IR_S_binary.left));
      _IR_free_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_binary *) node)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      break;
    case IR_NM_unary:
      IR_END_IR_node_t ((((_IR_unary *) node)->_IR_S_unary.op));
      _IR_free_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_unary *) node)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      break;
    case IR_NM_e_plus:
      _IR_free_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_plus *) node)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_minus:
      _IR_free_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_minus *) node)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_shl:
      _IR_free_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shl *) node)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_shr:
      _IR_free_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shr *) node)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_assign:
      _IR_free_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_assign *) node)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_fetch:
      _IR_free_node_fields (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_fetch *) node)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)));
      break;
    case IR_NM_e_sym:
      IR_END_string_t ((((_IR_e_sym *) node)->_IR_S_e_sym.name));
      _IR_free_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_sym *) node)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      break;
    case IR_NM_e_icon:
      IR_END_integer_t ((((_IR_e_icon *) node)->_IR_S_e_icon.value));
      _IR_free_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_icon *) node)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      break;
    case IR_NM_e_cvt:
      _IR_free_node_fields (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_cvt *) node)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)));
      break;
    default:
      abort ();
      break;
    }
}

void IR_free_node (IR_node_t node)
{
  if (node == NULL)
    return;
  _IR_free_node_fields (IR_NODE_MODE (node), node);
  IR_FREE (node, IR_node_size [IR_NODE_MODE (node)]);
}

static void _IR_free_graph (IR_node_t graph)
{
  if (graph == NULL
      || graph->_IR_last_graph_pass_number == _IR_current_graph_pass_number)
    return;
  graph->_IR_last_graph_pass_number = _IR_current_graph_pass_number;
  if (_IR_traverse_guard_function_variable == NULL
      || (*_IR_traverse_guard_function_variable) (graph))
  switch (IR_NODE_MODE (graph))
    {
    case IR_NM_e_plus:
      _IR_free_graph ((((_IR_e_plus *) graph)->_IR_S_e_plus._IR_M_binary.left));
      _IR_free_graph ((((_IR_e_plus *) graph)->_IR_S_e_plus._IR_M_binary.right));
      break;
    case IR_NM_e_minus:
      _IR_free_graph ((((_IR_e_minus *) graph)->_IR_S_e_minus._IR_M_binary.left));
      _IR_free_graph ((((_IR_e_minus *) graph)->_IR_S_e_minus._IR_M_binary.right));
      break;
    case IR_NM_e_shl:
      _IR_free_graph ((((_IR_e_shl *) graph)->_IR_S_e_shl._IR_M_binary.left));
      _IR_free_graph ((((_IR_e_shl *) graph)->_IR_S_e_shl._IR_M_binary.right));
      break;
    case IR_NM_e_shr:
      _IR_free_graph ((((_IR_e_shr *) graph)->_IR_S_e_shr._IR_M_binary.left));
      _IR_free_graph ((((_IR_e_shr *) graph)->_IR_S_e_shr._IR_M_binary.right));
      break;
    case IR_NM_e_assign:
      _IR_free_graph ((((_IR_e_assign *) graph)->_IR_S_e_assign._IR_M_binary.left));
      _IR_free_graph ((((_IR_e_assign *) graph)->_IR_S_e_assign._IR_M_binary.right));
      break;
    case IR_NM_e_fetch:
      _IR_free_graph ((((_IR_e_fetch *) graph)->_IR_S_e_fetch._IR_M_unary.op));
      break;
    case IR_NM_e_sym:
      break;
    case IR_NM_e_icon:
      break;
    case IR_NM_e_cvt:
      _IR_free_graph ((((_IR_e_cvt *) graph)->_IR_S_e_cvt._IR_M_unary.op));
      break;
    default:
      abort ();
      break;
    }
  IR_FREE (graph, IR_node_size [IR_NODE_MODE (graph)]);
}

void IR_free_graph (IR_node_t graph)
{
  _IR_traverse_guard_function_variable = NULL;
  _IR_current_graph_pass_number++;
  _IR_free_graph (graph);
}

void IR_conditional_free_graph (IR_node_t graph, int (* _IR_traverse_guard_function_parameter) (IR_node_t node))
{
  _IR_traverse_guard_function_variable = _IR_traverse_guard_function_parameter;
  _IR_current_graph_pass_number++;
  _IR_free_graph (graph);
}

static void _IR_copy_node_fields (IR_node_mode_t node_mode, IR_node_t to, IR_node_t from)
{
  switch (node_mode)
    {
    case IR_NM_node:
      break;
    case IR_NM_expr:
      IR_COPY_void_t ((((_IR_expr *) to)->_IR_S_expr.state), (((_IR_expr *) from)->_IR_S_expr.state));
      IR_COPY_integer_t ((((_IR_expr *) to)->_IR_S_expr.len), (((_IR_expr *) from)->_IR_S_expr.len));
      break;
    case IR_NM_binary:
      _IR_copy_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_binary *) to)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), (IR_node_t) ((char *) &((_IR_binary *) from)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      IR_COPY_IR_node_t ((((_IR_binary *) to)->_IR_S_binary.left), (((_IR_binary *) from)->_IR_S_binary.left));
      IR_COPY_IR_node_t ((((_IR_binary *) to)->_IR_S_binary.right), (((_IR_binary *) from)->_IR_S_binary.right));
      break;
    case IR_NM_unary:
      _IR_copy_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_unary *) to)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), (IR_node_t) ((char *) &((_IR_unary *) from)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      IR_COPY_IR_node_t ((((_IR_unary *) to)->_IR_S_unary.op), (((_IR_unary *) from)->_IR_S_unary.op));
      break;
    case IR_NM_e_plus:
      _IR_copy_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_plus *) to)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), (IR_node_t) ((char *) &((_IR_e_plus *) from)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_minus:
      _IR_copy_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_minus *) to)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), (IR_node_t) ((char *) &((_IR_e_minus *) from)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_shl:
      _IR_copy_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shl *) to)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), (IR_node_t) ((char *) &((_IR_e_shl *) from)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_shr:
      _IR_copy_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shr *) to)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), (IR_node_t) ((char *) &((_IR_e_shr *) from)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_assign:
      _IR_copy_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_assign *) to)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), (IR_node_t) ((char *) &((_IR_e_assign *) from)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)));
      break;
    case IR_NM_e_fetch:
      _IR_copy_node_fields (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_fetch *) to)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), (IR_node_t) ((char *) &((_IR_e_fetch *) from)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)));
      break;
    case IR_NM_e_sym:
      _IR_copy_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_sym *) to)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), (IR_node_t) ((char *) &((_IR_e_sym *) from)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      IR_COPY_string_t ((((_IR_e_sym *) to)->_IR_S_e_sym.name), (((_IR_e_sym *) from)->_IR_S_e_sym.name));
      break;
    case IR_NM_e_icon:
      _IR_copy_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_icon *) to)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), (IR_node_t) ((char *) &((_IR_e_icon *) from)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)));
      IR_COPY_integer_t ((((_IR_e_icon *) to)->_IR_S_e_icon.value), (((_IR_e_icon *) from)->_IR_S_e_icon.value));
      break;
    case IR_NM_e_cvt:
      _IR_copy_node_fields (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_cvt *) to)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), (IR_node_t) ((char *) &((_IR_e_cvt *) from)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)));
      break;
    default:
      abort ();
      break;
    }
}

IR_node_t IR_copy_node (IR_node_t node)
{
  IR_node_t result;

  if (node == NULL)
    return NULL;
  IR_ALLOC (result, IR_node_size [IR_NODE_MODE (node)], IR_node_t);
#ifdef __IR_DEBUG__
  if (result == NULL)
    abort ();
#endif /* __IR_DEBUG__ */
  IR_NODE_MODE (result) = IR_NODE_MODE (node);
  result->_IR_last_graph_pass_number = 0;
  _IR_copy_node_fields (IR_NODE_MODE (node), result, node);
  return result;
}

static IR_node_t _IR_copy_graph (IR_node_t graph)
{
  IR_node_t result;

  if (graph == NULL)
    return NULL;
  else if (graph->_IR_last_graph_pass_number == _IR_current_graph_pass_number)
    return graph->_IR_temporary;
  result = IR_copy_node (graph);
  graph->_IR_last_graph_pass_number = _IR_current_graph_pass_number;
  graph->_IR_temporary = result;
  if (_IR_traverse_guard_function_variable == NULL
      || (*_IR_traverse_guard_function_variable) (graph))
  switch (IR_NODE_MODE (graph))
    {
    case IR_NM_e_plus:
      (((_IR_e_plus *) result)->_IR_S_e_plus._IR_M_binary.left)
        = _IR_copy_graph ((((_IR_e_plus *) graph)->_IR_S_e_plus._IR_M_binary.left));
      (((_IR_e_plus *) result)->_IR_S_e_plus._IR_M_binary.right)
        = _IR_copy_graph ((((_IR_e_plus *) graph)->_IR_S_e_plus._IR_M_binary.right));
      break;
    case IR_NM_e_minus:
      (((_IR_e_minus *) result)->_IR_S_e_minus._IR_M_binary.left)
        = _IR_copy_graph ((((_IR_e_minus *) graph)->_IR_S_e_minus._IR_M_binary.left));
      (((_IR_e_minus *) result)->_IR_S_e_minus._IR_M_binary.right)
        = _IR_copy_graph ((((_IR_e_minus *) graph)->_IR_S_e_minus._IR_M_binary.right));
      break;
    case IR_NM_e_shl:
      (((_IR_e_shl *) result)->_IR_S_e_shl._IR_M_binary.left)
        = _IR_copy_graph ((((_IR_e_shl *) graph)->_IR_S_e_shl._IR_M_binary.left));
      (((_IR_e_shl *) result)->_IR_S_e_shl._IR_M_binary.right)
        = _IR_copy_graph ((((_IR_e_shl *) graph)->_IR_S_e_shl._IR_M_binary.right));
      break;
    case IR_NM_e_shr:
      (((_IR_e_shr *) result)->_IR_S_e_shr._IR_M_binary.left)
        = _IR_copy_graph ((((_IR_e_shr *) graph)->_IR_S_e_shr._IR_M_binary.left));
      (((_IR_e_shr *) result)->_IR_S_e_shr._IR_M_binary.right)
        = _IR_copy_graph ((((_IR_e_shr *) graph)->_IR_S_e_shr._IR_M_binary.right));
      break;
    case IR_NM_e_assign:
      (((_IR_e_assign *) result)->_IR_S_e_assign._IR_M_binary.left)
        = _IR_copy_graph ((((_IR_e_assign *) graph)->_IR_S_e_assign._IR_M_binary.left));
      (((_IR_e_assign *) result)->_IR_S_e_assign._IR_M_binary.right)
        = _IR_copy_graph ((((_IR_e_assign *) graph)->_IR_S_e_assign._IR_M_binary.right));
      break;
    case IR_NM_e_fetch:
      (((_IR_e_fetch *) result)->_IR_S_e_fetch._IR_M_unary.op)
        = _IR_copy_graph ((((_IR_e_fetch *) graph)->_IR_S_e_fetch._IR_M_unary.op));
      break;
    case IR_NM_e_sym:
      break;
    case IR_NM_e_icon:
      break;
    case IR_NM_e_cvt:
      (((_IR_e_cvt *) result)->_IR_S_e_cvt._IR_M_unary.op)
        = _IR_copy_graph ((((_IR_e_cvt *) graph)->_IR_S_e_cvt._IR_M_unary.op));
      break;
    default:
      abort ();
      break;
    }
  return result;
}

IR_node_t IR_copy_graph (IR_node_t graph)
{
  _IR_traverse_guard_function_variable = NULL;
  _IR_current_graph_pass_number++;
  return _IR_copy_graph (graph);
}

IR_node_t IR_conditional_copy_graph (IR_node_t graph, int (* _IR_traverse_guard_function_parameter) (IR_node_t node))
{
  _IR_traverse_guard_function_variable = _IR_traverse_guard_function_parameter;
  _IR_current_graph_pass_number++;
  return _IR_copy_graph (graph);
}

static int _IR_node_field_equality (IR_node_mode_t node_mode, IR_node_t node_1, IR_node_t node_2)
{
  switch (node_mode)
    {
    case IR_NM_node:
      return 1;
    case IR_NM_expr:
      if (!IR_EQUAL_void_t ((((_IR_expr *) node_1)->_IR_S_expr.state), (((_IR_expr *) node_2)->_IR_S_expr.state)))
        return 0;
      if (!IR_EQUAL_integer_t ((((_IR_expr *) node_1)->_IR_S_expr.len), (((_IR_expr *) node_2)->_IR_S_expr.len)))
        return 0;
      return 1;
    case IR_NM_binary:
      if (!_IR_node_field_equality (IR_NM_expr, (IR_node_t) ((char *) &((_IR_binary *) node_1)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), (IR_node_t) ((char *) &((_IR_binary *) node_2)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr))))
        return 0;
      if (!IR_EQUAL_IR_node_t ((((_IR_binary *) node_1)->_IR_S_binary.left), (((_IR_binary *) node_2)->_IR_S_binary.left)))
        return 0;
      if (!IR_EQUAL_IR_node_t ((((_IR_binary *) node_1)->_IR_S_binary.right), (((_IR_binary *) node_2)->_IR_S_binary.right)))
        return 0;
      return 1;
    case IR_NM_unary:
      if (!_IR_node_field_equality (IR_NM_expr, (IR_node_t) ((char *) &((_IR_unary *) node_1)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), (IR_node_t) ((char *) &((_IR_unary *) node_2)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr))))
        return 0;
      if (!IR_EQUAL_IR_node_t ((((_IR_unary *) node_1)->_IR_S_unary.op), (((_IR_unary *) node_2)->_IR_S_unary.op)))
        return 0;
      return 1;
    case IR_NM_e_plus:
      if (!_IR_node_field_equality (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_plus *) node_1)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), (IR_node_t) ((char *) &((_IR_e_plus *) node_2)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary))))
        return 0;
      return 1;
    case IR_NM_e_minus:
      if (!_IR_node_field_equality (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_minus *) node_1)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), (IR_node_t) ((char *) &((_IR_e_minus *) node_2)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary))))
        return 0;
      return 1;
    case IR_NM_e_shl:
      if (!_IR_node_field_equality (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shl *) node_1)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), (IR_node_t) ((char *) &((_IR_e_shl *) node_2)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary))))
        return 0;
      return 1;
    case IR_NM_e_shr:
      if (!_IR_node_field_equality (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shr *) node_1)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), (IR_node_t) ((char *) &((_IR_e_shr *) node_2)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary))))
        return 0;
      return 1;
    case IR_NM_e_assign:
      if (!_IR_node_field_equality (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_assign *) node_1)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), (IR_node_t) ((char *) &((_IR_e_assign *) node_2)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary))))
        return 0;
      return 1;
    case IR_NM_e_fetch:
      if (!_IR_node_field_equality (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_fetch *) node_1)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), (IR_node_t) ((char *) &((_IR_e_fetch *) node_2)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary))))
        return 0;
      return 1;
    case IR_NM_e_sym:
      if (!_IR_node_field_equality (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_sym *) node_1)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), (IR_node_t) ((char *) &((_IR_e_sym *) node_2)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr))))
        return 0;
      if (!IR_EQUAL_string_t ((((_IR_e_sym *) node_1)->_IR_S_e_sym.name), (((_IR_e_sym *) node_2)->_IR_S_e_sym.name)))
        return 0;
      return 1;
    case IR_NM_e_icon:
      if (!_IR_node_field_equality (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_icon *) node_1)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), (IR_node_t) ((char *) &((_IR_e_icon *) node_2)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr))))
        return 0;
      if (!IR_EQUAL_integer_t ((((_IR_e_icon *) node_1)->_IR_S_e_icon.value), (((_IR_e_icon *) node_2)->_IR_S_e_icon.value)))
        return 0;
      return 1;
    case IR_NM_e_cvt:
      if (!_IR_node_field_equality (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_cvt *) node_1)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), (IR_node_t) ((char *) &((_IR_e_cvt *) node_2)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary))))
        return 0;
      return 1;
    default:
      abort ();
      break;
    }
}

int IR_is_equal_node (IR_node_t node_1, IR_node_t node_2)
{
  if (node_1 == node_2)
    return 1;
  else if (node_1 == NULL || node_2 == NULL
           || IR_NODE_MODE (node_1) != IR_NODE_MODE (node_2))
    return node_1 == node_2;
  return _IR_node_field_equality (IR_NODE_MODE (node_1), node_2, node_1);
}

static int _IR_is_equal_graph (IR_node_t graph_1, IR_node_t graph_2)
{
  int result;

  if (graph_1 == graph_2)
    return 1;
  else if (graph_1 == NULL || graph_2 == NULL)
    return 0;
  else if (graph_1->_IR_last_graph_pass_number == _IR_current_graph_pass_number
           && graph_2->_IR_last_graph_pass_number == _IR_current_graph_pass_number)
    return 1;
  else if (graph_1->_IR_last_graph_pass_number == _IR_current_graph_pass_number
           || graph_2->_IR_last_graph_pass_number == _IR_current_graph_pass_number)
    return 0;
  result = IR_is_equal_node (graph_1, graph_2);
  graph_1->_IR_last_graph_pass_number = _IR_current_graph_pass_number;
  graph_2->_IR_last_graph_pass_number = _IR_current_graph_pass_number;
  if (result
      && (_IR_traverse_guard_function_variable == NULL
          || (*_IR_traverse_guard_function_variable) (graph_1)))
    switch (IR_NODE_MODE (graph_1))
      {
      case IR_NM_e_plus:
        result = _IR_is_equal_graph ((((_IR_e_plus *) graph_1)->_IR_S_e_plus._IR_M_binary.left), (((_IR_e_plus *) graph_2)->_IR_S_e_plus._IR_M_binary.left));
        if (!result)
          break;
        result = _IR_is_equal_graph ((((_IR_e_plus *) graph_1)->_IR_S_e_plus._IR_M_binary.right), (((_IR_e_plus *) graph_2)->_IR_S_e_plus._IR_M_binary.right));
        if (!result)
          break;
        break;
      case IR_NM_e_minus:
        result = _IR_is_equal_graph ((((_IR_e_minus *) graph_1)->_IR_S_e_minus._IR_M_binary.left), (((_IR_e_minus *) graph_2)->_IR_S_e_minus._IR_M_binary.left));
        if (!result)
          break;
        result = _IR_is_equal_graph ((((_IR_e_minus *) graph_1)->_IR_S_e_minus._IR_M_binary.right), (((_IR_e_minus *) graph_2)->_IR_S_e_minus._IR_M_binary.right));
        if (!result)
          break;
        break;
      case IR_NM_e_shl:
        result = _IR_is_equal_graph ((((_IR_e_shl *) graph_1)->_IR_S_e_shl._IR_M_binary.left), (((_IR_e_shl *) graph_2)->_IR_S_e_shl._IR_M_binary.left));
        if (!result)
          break;
        result = _IR_is_equal_graph ((((_IR_e_shl *) graph_1)->_IR_S_e_shl._IR_M_binary.right), (((_IR_e_shl *) graph_2)->_IR_S_e_shl._IR_M_binary.right));
        if (!result)
          break;
        break;
      case IR_NM_e_shr:
        result = _IR_is_equal_graph ((((_IR_e_shr *) graph_1)->_IR_S_e_shr._IR_M_binary.left), (((_IR_e_shr *) graph_2)->_IR_S_e_shr._IR_M_binary.left));
        if (!result)
          break;
        result = _IR_is_equal_graph ((((_IR_e_shr *) graph_1)->_IR_S_e_shr._IR_M_binary.right), (((_IR_e_shr *) graph_2)->_IR_S_e_shr._IR_M_binary.right));
        if (!result)
          break;
        break;
      case IR_NM_e_assign:
        result = _IR_is_equal_graph ((((_IR_e_assign *) graph_1)->_IR_S_e_assign._IR_M_binary.left), (((_IR_e_assign *) graph_2)->_IR_S_e_assign._IR_M_binary.left));
        if (!result)
          break;
        result = _IR_is_equal_graph ((((_IR_e_assign *) graph_1)->_IR_S_e_assign._IR_M_binary.right), (((_IR_e_assign *) graph_2)->_IR_S_e_assign._IR_M_binary.right));
        if (!result)
          break;
        break;
      case IR_NM_e_fetch:
        result = _IR_is_equal_graph ((((_IR_e_fetch *) graph_1)->_IR_S_e_fetch._IR_M_unary.op), (((_IR_e_fetch *) graph_2)->_IR_S_e_fetch._IR_M_unary.op));
        if (!result)
          break;
        break;
      case IR_NM_e_sym:
        break;
      case IR_NM_e_icon:
        break;
      case IR_NM_e_cvt:
        result = _IR_is_equal_graph ((((_IR_e_cvt *) graph_1)->_IR_S_e_cvt._IR_M_unary.op), (((_IR_e_cvt *) graph_2)->_IR_S_e_cvt._IR_M_unary.op));
        if (!result)
          break;
        break;
      default:
        abort ();
        break;
      }
  return result;
}

int IR_is_equal_graph (IR_node_t graph_1, IR_node_t graph_2)
{
  _IR_traverse_guard_function_variable = NULL;
  _IR_current_graph_pass_number++;
  return _IR_is_equal_graph (graph_1, graph_2);
}

int IR_conditional_is_equal_graph (IR_node_t graph_1, IR_node_t graph_2, int (* _IR_traverse_guard_function_parameter) (IR_node_t node))
{
  _IR_traverse_guard_function_variable = _IR_traverse_guard_function_parameter;
  _IR_current_graph_pass_number++;
  return _IR_is_equal_graph (graph_1, graph_2);
}

static int _IR_check_node_fields (IR_node_mode_t node_mode, IR_node_t node, int class_field_flag)
{
  switch (node_mode)
    {
    case IR_NM_node:
      break;
    case IR_NM_expr:
      break;
    case IR_NM_binary:
      if (_IR_check_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_binary *) node)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag))
        return 1;
      if ((((_IR_binary *) node)->_IR_S_binary.left) != NULL
          && !((_IR_SF_expr [IR_NODE_MODE ((((_IR_binary *) node)->_IR_S_binary.left)) / 8]
                >> (IR_NODE_MODE ((((_IR_binary *) node)->_IR_S_binary.left)) % 8)) & 1))
         return 1;
      if ((((_IR_binary *) node)->_IR_S_binary.right) != NULL
          && !((_IR_SF_expr [IR_NODE_MODE ((((_IR_binary *) node)->_IR_S_binary.right)) / 8]
                >> (IR_NODE_MODE ((((_IR_binary *) node)->_IR_S_binary.right)) % 8)) & 1))
         return 1;
      break;
    case IR_NM_unary:
      if (_IR_check_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_unary *) node)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag))
        return 1;
      if ((((_IR_unary *) node)->_IR_S_unary.op) != NULL
          && !((_IR_SF_expr [IR_NODE_MODE ((((_IR_unary *) node)->_IR_S_unary.op)) / 8]
                >> (IR_NODE_MODE ((((_IR_unary *) node)->_IR_S_unary.op)) % 8)) & 1))
         return 1;
      break;
    case IR_NM_e_plus:
      if (_IR_check_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_plus *) node)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag))
        return 1;
      break;
    case IR_NM_e_minus:
      if (_IR_check_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_minus *) node)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag))
        return 1;
      break;
    case IR_NM_e_shl:
      if (_IR_check_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shl *) node)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag))
        return 1;
      break;
    case IR_NM_e_shr:
      if (_IR_check_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shr *) node)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag))
        return 1;
      break;
    case IR_NM_e_assign:
      if (_IR_check_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_assign *) node)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag))
        return 1;
      break;
    case IR_NM_e_fetch:
      if (_IR_check_node_fields (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_fetch *) node)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag))
        return 1;
      break;
    case IR_NM_e_sym:
      if (_IR_check_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_sym *) node)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag))
        return 1;
      break;
    case IR_NM_e_icon:
      if (_IR_check_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_icon *) node)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag))
        return 1;
      break;
    case IR_NM_e_cvt:
      if (_IR_check_node_fields (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_cvt *) node)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag))
        return 1;
      break;
    default:
      abort ();
      break;
    }
  return 0;
}

static void _IR_print_node_fields_for_check (IR_node_mode_t node_mode, IR_node_t node, int class_field_flag)
{
  switch (node_mode)
    {
    case IR_NM_node:
      break;
    case IR_NM_expr:
      break;
    case IR_NM_binary:
      _IR_print_node_fields_for_check (IR_NM_expr, (IR_node_t) ((char *) &((_IR_binary *) node)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag);
      IR_print_node ((((_IR_binary *) node)->_IR_S_binary.left), class_field_flag);
      IR_print_node ((((_IR_binary *) node)->_IR_S_binary.right), class_field_flag);
      break;
    case IR_NM_unary:
      _IR_print_node_fields_for_check (IR_NM_expr, (IR_node_t) ((char *) &((_IR_unary *) node)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag);
      IR_print_node ((((_IR_unary *) node)->_IR_S_unary.op), class_field_flag);
      break;
    case IR_NM_e_plus:
      _IR_print_node_fields_for_check (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_plus *) node)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag);
      break;
    case IR_NM_e_minus:
      _IR_print_node_fields_for_check (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_minus *) node)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag);
      break;
    case IR_NM_e_shl:
      _IR_print_node_fields_for_check (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shl *) node)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag);
      break;
    case IR_NM_e_shr:
      _IR_print_node_fields_for_check (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shr *) node)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag);
      break;
    case IR_NM_e_assign:
      _IR_print_node_fields_for_check (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_assign *) node)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag);
      break;
    case IR_NM_e_fetch:
      _IR_print_node_fields_for_check (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_fetch *) node)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag);
      break;
    case IR_NM_e_sym:
      _IR_print_node_fields_for_check (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_sym *) node)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag);
      break;
    case IR_NM_e_icon:
      _IR_print_node_fields_for_check (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_icon *) node)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag);
      break;
    case IR_NM_e_cvt:
      _IR_print_node_fields_for_check (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_cvt *) node)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag);
      break;
    default:
      abort ();
      break;
    }
}

int IR_check_node (IR_node_t node, int class_field_flag)
{
  if (node == NULL)
    return 0;
  if (!_IR_check_node_fields (IR_NODE_MODE (node), node, class_field_flag))
    return 0;
  IR_print_node (node, class_field_flag);
  _IR_print_node_fields_for_check (IR_NODE_MODE (node), node, class_field_flag);
  return 1;
}

static int _IR_check_graph (IR_node_t graph, int class_field_flag)
{
  int result;

  if (graph == NULL
      || graph->_IR_last_graph_pass_number == _IR_current_graph_pass_number)
    return 0;
  result = IR_check_node (graph, class_field_flag);
  graph->_IR_last_graph_pass_number = _IR_current_graph_pass_number;
  if (_IR_traverse_guard_function_variable == NULL
      || (*_IR_traverse_guard_function_variable) (graph))
  switch (IR_NODE_MODE (graph))
    {
    case IR_NM_e_plus:
      result = _IR_check_graph ((((_IR_e_plus *) graph)->_IR_S_e_plus._IR_M_binary.left), class_field_flag) || result;
      result = _IR_check_graph ((((_IR_e_plus *) graph)->_IR_S_e_plus._IR_M_binary.right), class_field_flag) || result;
      break;
    case IR_NM_e_minus:
      result = _IR_check_graph ((((_IR_e_minus *) graph)->_IR_S_e_minus._IR_M_binary.left), class_field_flag) || result;
      result = _IR_check_graph ((((_IR_e_minus *) graph)->_IR_S_e_minus._IR_M_binary.right), class_field_flag) || result;
      break;
    case IR_NM_e_shl:
      result = _IR_check_graph ((((_IR_e_shl *) graph)->_IR_S_e_shl._IR_M_binary.left), class_field_flag) || result;
      result = _IR_check_graph ((((_IR_e_shl *) graph)->_IR_S_e_shl._IR_M_binary.right), class_field_flag) || result;
      break;
    case IR_NM_e_shr:
      result = _IR_check_graph ((((_IR_e_shr *) graph)->_IR_S_e_shr._IR_M_binary.left), class_field_flag) || result;
      result = _IR_check_graph ((((_IR_e_shr *) graph)->_IR_S_e_shr._IR_M_binary.right), class_field_flag) || result;
      break;
    case IR_NM_e_assign:
      result = _IR_check_graph ((((_IR_e_assign *) graph)->_IR_S_e_assign._IR_M_binary.left), class_field_flag) || result;
      result = _IR_check_graph ((((_IR_e_assign *) graph)->_IR_S_e_assign._IR_M_binary.right), class_field_flag) || result;
      break;
    case IR_NM_e_fetch:
      result = _IR_check_graph ((((_IR_e_fetch *) graph)->_IR_S_e_fetch._IR_M_unary.op), class_field_flag) || result;
      break;
    case IR_NM_e_sym:
      break;
    case IR_NM_e_icon:
      break;
    case IR_NM_e_cvt:
      result = _IR_check_graph ((((_IR_e_cvt *) graph)->_IR_S_e_cvt._IR_M_unary.op), class_field_flag) || result;
      break;
    default:
      abort ();
      break;
    }
  return result;
}

int IR_check_graph (IR_node_t graph, int class_field_flag)
{
  _IR_traverse_guard_function_variable = NULL;
  _IR_current_graph_pass_number++;
  return _IR_check_graph (graph, class_field_flag);
}

int IR_conditional_check_graph (IR_node_t graph, int class_field_flag, int (* _IR_traverse_guard_function_parameter) (IR_node_t node))
{
  _IR_traverse_guard_function_variable = _IR_traverse_guard_function_parameter;
  _IR_current_graph_pass_number++;
  return _IR_check_graph (graph, class_field_flag);
}

static void _IR_print_node_fields (IR_node_mode_t node_mode, IR_node_t node, int class_field_flag)
{
  switch (node_mode)
    {
    case IR_NM_node:
      break;
    case IR_NM_expr:
      printf ("      state: ");
      IR_PRINT_void_t ((((_IR_expr *) node)->_IR_S_expr.state));
      fputc ('\n', stdout);
      printf ("      len: ");
      IR_PRINT_integer_t ((((_IR_expr *) node)->_IR_S_expr.len));
      fputc ('\n', stdout);
      break;
    case IR_NM_binary:
      _IR_print_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_binary *) node)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag);
      printf ("      left: ");
      IR_PRINT_IR_node_t ((((_IR_binary *) node)->_IR_S_binary.left));
      fputc ('\n', stdout);
      printf ("      right: ");
      IR_PRINT_IR_node_t ((((_IR_binary *) node)->_IR_S_binary.right));
      fputc ('\n', stdout);
      break;
    case IR_NM_unary:
      _IR_print_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_unary *) node)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag);
      printf ("      op: ");
      IR_PRINT_IR_node_t ((((_IR_unary *) node)->_IR_S_unary.op));
      fputc ('\n', stdout);
      break;
    case IR_NM_e_plus:
      _IR_print_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_plus *) node)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag);
      break;
    case IR_NM_e_minus:
      _IR_print_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_minus *) node)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag);
      break;
    case IR_NM_e_shl:
      _IR_print_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shl *) node)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag);
      break;
    case IR_NM_e_shr:
      _IR_print_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shr *) node)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag);
      break;
    case IR_NM_e_assign:
      _IR_print_node_fields (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_assign *) node)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag);
      break;
    case IR_NM_e_fetch:
      _IR_print_node_fields (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_fetch *) node)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag);
      break;
    case IR_NM_e_sym:
      _IR_print_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_sym *) node)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag);
      printf ("      name: ");
      IR_PRINT_string_t ((((_IR_e_sym *) node)->_IR_S_e_sym.name));
      fputc ('\n', stdout);
      break;
    case IR_NM_e_icon:
      _IR_print_node_fields (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_icon *) node)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag);
      printf ("      value: ");
      IR_PRINT_integer_t ((((_IR_e_icon *) node)->_IR_S_e_icon.value));
      fputc ('\n', stdout);
      break;
    case IR_NM_e_cvt:
      _IR_print_node_fields (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_cvt *) node)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag);
      break;
    default:
      abort ();
      break;
    }
}

void IR_print_node (IR_node_t node, int class_field_flag)
{
  if (node == NULL)
    return;
  IR_PRINT_IR_node_t (node);
  printf (" %s :\n", IR_node_name [IR_NODE_MODE (node)]);
  _IR_print_node_fields (IR_NODE_MODE (node), node, class_field_flag);
}

static int _IR_output_node_fields (IR_node_mode_t node_mode, FILE *output_file, IR_node_t node, int level)
{
  int result;

  result = 0;
  switch (node_mode)
    {
    case IR_NM_node:
      break;
    case IR_NM_expr:
      if (level >= 0)
        {
          result = IR_OUTPUT_void_t (output_file, (((_IR_expr *) node)->_IR_S_expr.state)) || result;
          result = IR_OUTPUT_integer_t (output_file, (((_IR_expr *) node)->_IR_S_expr.len)) || result;
        }
      break;
    case IR_NM_binary:
          result = _IR_output_node_fields (IR_NM_expr, output_file, (IR_node_t) ((char *) &((_IR_binary *) node)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), level) || result;
      if (level >= 0)
        {
          result = IR_OUTPUT_IR_node_t (output_file, (((_IR_binary *) node)->_IR_S_binary.left)) || result;
          result = IR_OUTPUT_IR_node_t (output_file, (((_IR_binary *) node)->_IR_S_binary.right)) || result;
        }
      break;
    case IR_NM_unary:
          result = _IR_output_node_fields (IR_NM_expr, output_file, (IR_node_t) ((char *) &((_IR_unary *) node)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), level) || result;
      if (level >= 0)
        {
          result = IR_OUTPUT_IR_node_t (output_file, (((_IR_unary *) node)->_IR_S_unary.op)) || result;
        }
      break;
    case IR_NM_e_plus:
          result = _IR_output_node_fields (IR_NM_binary, output_file, (IR_node_t) ((char *) &((_IR_e_plus *) node)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), level) || result;
      break;
    case IR_NM_e_minus:
          result = _IR_output_node_fields (IR_NM_binary, output_file, (IR_node_t) ((char *) &((_IR_e_minus *) node)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), level) || result;
      break;
    case IR_NM_e_shl:
          result = _IR_output_node_fields (IR_NM_binary, output_file, (IR_node_t) ((char *) &((_IR_e_shl *) node)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), level) || result;
      break;
    case IR_NM_e_shr:
          result = _IR_output_node_fields (IR_NM_binary, output_file, (IR_node_t) ((char *) &((_IR_e_shr *) node)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), level) || result;
      break;
    case IR_NM_e_assign:
          result = _IR_output_node_fields (IR_NM_binary, output_file, (IR_node_t) ((char *) &((_IR_e_assign *) node)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), level) || result;
      break;
    case IR_NM_e_fetch:
          result = _IR_output_node_fields (IR_NM_unary, output_file, (IR_node_t) ((char *) &((_IR_e_fetch *) node)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), level) || result;
      break;
    case IR_NM_e_sym:
          result = _IR_output_node_fields (IR_NM_expr, output_file, (IR_node_t) ((char *) &((_IR_e_sym *) node)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), level) || result;
      if (level >= 0)
        {
          result = IR_OUTPUT_string_t (output_file, (((_IR_e_sym *) node)->_IR_S_e_sym.name)) || result;
        }
      break;
    case IR_NM_e_icon:
          result = _IR_output_node_fields (IR_NM_expr, output_file, (IR_node_t) ((char *) &((_IR_e_icon *) node)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), level) || result;
      if (level >= 0)
        {
          result = IR_OUTPUT_integer_t (output_file, (((_IR_e_icon *) node)->_IR_S_e_icon.value)) || result;
        }
      break;
    case IR_NM_e_cvt:
          result = _IR_output_node_fields (IR_NM_unary, output_file, (IR_node_t) ((char *) &((_IR_e_cvt *) node)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), level) || result;
      break;
    default:
      abort ();
      break;
    }
  return result;
}

int IR_output_node (FILE *output_file, IR_node_t node, int level)
{
  int result;

  if (node == NULL)
    return 0;
#ifdef __IR_DEBUG__
  if (IR_NODE_LEVEL (node) > level)
    abort ();
#endif /* __IR_DEBUG__ */
  result = fwrite (&level, sizeof (int), 1, output_file) != sizeof (int);
  result = IR_OUTPUT_IR_node_t (output_file, node) || result;
  result = fwrite (&IR_NODE_MODE (node), sizeof (IR_NODE_MODE (node)), 1, output_file) != sizeof (IR_NODE_MODE (node)) || result;
  result = _IR_output_node_fields (IR_NODE_MODE (node), output_file, node, level) || result;
 return result;
}

static int _IR_input_node_fields (IR_node_mode_t node_mode, FILE *input_file, IR_node_t result, int level)
{
  switch (node_mode)
    {
    case IR_NM_node:
      break;
    case IR_NM_expr:
      if (IR_INPUT_void_t (input_file, (((_IR_expr *) result)->_IR_S_expr.state)))
        return 1;
      if (IR_INPUT_integer_t (input_file, (((_IR_expr *) result)->_IR_S_expr.len)))
        return 1;
      break;
    case IR_NM_binary:
      if (_IR_input_node_fields (IR_NM_expr, input_file, (IR_node_t) ((char *) &((_IR_binary *) result)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), level))
        return 1;
      if (IR_INPUT_IR_node_t (input_file, (((_IR_binary *) result)->_IR_S_binary.left)))
        return 1;
      if (IR_INPUT_IR_node_t (input_file, (((_IR_binary *) result)->_IR_S_binary.right)))
        return 1;
      break;
    case IR_NM_unary:
      if (_IR_input_node_fields (IR_NM_expr, input_file, (IR_node_t) ((char *) &((_IR_unary *) result)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), level))
        return 1;
      if (IR_INPUT_IR_node_t (input_file, (((_IR_unary *) result)->_IR_S_unary.op)))
        return 1;
      break;
    case IR_NM_e_plus:
      if (_IR_input_node_fields (IR_NM_binary, input_file, (IR_node_t) ((char *) &((_IR_e_plus *) result)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), level))
        return 1;
      break;
    case IR_NM_e_minus:
      if (_IR_input_node_fields (IR_NM_binary, input_file, (IR_node_t) ((char *) &((_IR_e_minus *) result)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), level))
        return 1;
      break;
    case IR_NM_e_shl:
      if (_IR_input_node_fields (IR_NM_binary, input_file, (IR_node_t) ((char *) &((_IR_e_shl *) result)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), level))
        return 1;
      break;
    case IR_NM_e_shr:
      if (_IR_input_node_fields (IR_NM_binary, input_file, (IR_node_t) ((char *) &((_IR_e_shr *) result)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), level))
        return 1;
      break;
    case IR_NM_e_assign:
      if (_IR_input_node_fields (IR_NM_binary, input_file, (IR_node_t) ((char *) &((_IR_e_assign *) result)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), level))
        return 1;
      break;
    case IR_NM_e_fetch:
      if (_IR_input_node_fields (IR_NM_unary, input_file, (IR_node_t) ((char *) &((_IR_e_fetch *) result)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), level))
        return 1;
      break;
    case IR_NM_e_sym:
      if (_IR_input_node_fields (IR_NM_expr, input_file, (IR_node_t) ((char *) &((_IR_e_sym *) result)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), level))
        return 1;
      if (IR_INPUT_string_t (input_file, (((_IR_e_sym *) result)->_IR_S_e_sym.name)))
        return 1;
      break;
    case IR_NM_e_icon:
      if (_IR_input_node_fields (IR_NM_expr, input_file, (IR_node_t) ((char *) &((_IR_e_icon *) result)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), level))
        return 1;
      if (IR_INPUT_integer_t (input_file, (((_IR_e_icon *) result)->_IR_S_e_icon.value)))
        return 1;
      break;
    case IR_NM_e_cvt:
      if (_IR_input_node_fields (IR_NM_unary, input_file, (IR_node_t) ((char *) &((_IR_e_cvt *) result)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), level))
        return 1;
      break;
    default:
      abort ();
      break;
    }
  return 0;
}

IR_node_t IR_input_node (FILE *input_file, IR_node_t *original_address)
{
  IR_node_t result;
  IR_node_mode_t node_mode;
  int level;

  if (fread (&level, sizeof (int), 1, input_file) != sizeof (int))
    return NULL;
#ifdef __IR_DEBUG__
  if (0 < level)
      abort ();
#endif /* __IR_DEBUG__ */
  if (IR_INPUT_IR_node_t (input_file, *original_address))
    return NULL;
  if (fread (&node_mode, sizeof (node_mode), 1, input_file) != sizeof (node_mode))
    return NULL;
  result = IR_create_node (node_mode);
  if (result == NULL)
    return NULL;
  if (_IR_input_node_fields (node_mode, input_file, result, level))
    return NULL;
  return result;
}

static void _IR_traverse_depth_first
  (IR_node_t graph, int class_field_flag, void (*function) (IR_node_t node));

static void _IR_node_field_traverse_depth_first
  (IR_node_mode_t node_mode, IR_node_t graph, int class_field_flag, void (*function) (IR_node_t node))
{
  switch (node_mode)
    {
    case IR_NM_node:
      break;
    case IR_NM_expr:
      break;
    case IR_NM_binary:
      _IR_node_field_traverse_depth_first (IR_NM_expr, (IR_node_t) ((char *) &((_IR_binary *) graph)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, function);
      _IR_traverse_depth_first ((((_IR_binary *) graph)->_IR_S_binary.left), class_field_flag, function);
      _IR_traverse_depth_first ((((_IR_binary *) graph)->_IR_S_binary.right), class_field_flag, function);
      break;
    case IR_NM_unary:
      _IR_node_field_traverse_depth_first (IR_NM_expr, (IR_node_t) ((char *) &((_IR_unary *) graph)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, function);
      _IR_traverse_depth_first ((((_IR_unary *) graph)->_IR_S_unary.op), class_field_flag, function);
      break;
    case IR_NM_e_plus:
      _IR_node_field_traverse_depth_first (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_plus *) graph)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, function);
      break;
    case IR_NM_e_minus:
      _IR_node_field_traverse_depth_first (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_minus *) graph)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, function);
      break;
    case IR_NM_e_shl:
      _IR_node_field_traverse_depth_first (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shl *) graph)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, function);
      break;
    case IR_NM_e_shr:
      _IR_node_field_traverse_depth_first (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shr *) graph)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, function);
      break;
    case IR_NM_e_assign:
      _IR_node_field_traverse_depth_first (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_assign *) graph)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, function);
      break;
    case IR_NM_e_fetch:
      _IR_node_field_traverse_depth_first (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_fetch *) graph)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag, function);
      break;
    case IR_NM_e_sym:
      _IR_node_field_traverse_depth_first (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_sym *) graph)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, function);
      break;
    case IR_NM_e_icon:
      _IR_node_field_traverse_depth_first (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_icon *) graph)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, function);
      break;
    case IR_NM_e_cvt:
      _IR_node_field_traverse_depth_first (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_cvt *) graph)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag, function);
      break;
    default:
      abort ();
      break;
    }
}

static void _IR_traverse_depth_first
  (IR_node_t graph, int class_field_flag, void (*function) (IR_node_t node))
{
  if (graph == NULL
      || graph->_IR_last_graph_pass_number == _IR_current_graph_pass_number)
    return;
  graph->_IR_last_graph_pass_number = _IR_current_graph_pass_number;
  _IR_node_field_traverse_depth_first (IR_NODE_MODE (graph), graph, class_field_flag, function);
  (*function) (graph);
}

void IR_traverse_depth_first
  (IR_node_t graph, int class_field_flag, void (*function) (IR_node_t node))
{
  _IR_current_graph_pass_number++;
  _IR_traverse_depth_first (graph, class_field_flag, function);
}

static void _IR_traverse_reverse_depth_first
  (IR_node_t graph, int class_field_flag, int (*function) (IR_node_t node));

static void _IR_node_field_traverse_reverse_depth_first
  (IR_node_mode_t node_mode, IR_node_t graph, int class_field_flag, int (*function) (IR_node_t node))
{
  switch (node_mode)
    {
    case IR_NM_node:
      break;
    case IR_NM_expr:
      break;
    case IR_NM_binary:
      _IR_node_field_traverse_reverse_depth_first (IR_NM_expr, (IR_node_t) ((char *) &((_IR_binary *) graph)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, function);
      _IR_traverse_reverse_depth_first ((((_IR_binary *) graph)->_IR_S_binary.left), class_field_flag, function);
      _IR_traverse_reverse_depth_first ((((_IR_binary *) graph)->_IR_S_binary.right), class_field_flag, function);
      break;
    case IR_NM_unary:
      _IR_node_field_traverse_reverse_depth_first (IR_NM_expr, (IR_node_t) ((char *) &((_IR_unary *) graph)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, function);
      _IR_traverse_reverse_depth_first ((((_IR_unary *) graph)->_IR_S_unary.op), class_field_flag, function);
      break;
    case IR_NM_e_plus:
      _IR_node_field_traverse_reverse_depth_first (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_plus *) graph)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, function);
      break;
    case IR_NM_e_minus:
      _IR_node_field_traverse_reverse_depth_first (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_minus *) graph)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, function);
      break;
    case IR_NM_e_shl:
      _IR_node_field_traverse_reverse_depth_first (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shl *) graph)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, function);
      break;
    case IR_NM_e_shr:
      _IR_node_field_traverse_reverse_depth_first (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shr *) graph)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, function);
      break;
    case IR_NM_e_assign:
      _IR_node_field_traverse_reverse_depth_first (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_assign *) graph)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, function);
      break;
    case IR_NM_e_fetch:
      _IR_node_field_traverse_reverse_depth_first (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_fetch *) graph)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag, function);
      break;
    case IR_NM_e_sym:
      _IR_node_field_traverse_reverse_depth_first (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_sym *) graph)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, function);
      break;
    case IR_NM_e_icon:
      _IR_node_field_traverse_reverse_depth_first (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_icon *) graph)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, function);
      break;
    case IR_NM_e_cvt:
      _IR_node_field_traverse_reverse_depth_first (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_cvt *) graph)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag, function);
      break;
    default:
      abort ();
      break;
    }
}

static void _IR_traverse_reverse_depth_first
  (IR_node_t graph, int class_field_flag, int (*function) (IR_node_t node))
{
  if (graph == NULL
      || graph->_IR_last_graph_pass_number == _IR_current_graph_pass_number)
    return;
  graph->_IR_last_graph_pass_number = _IR_current_graph_pass_number;
  if ((*function) (graph))
  _IR_node_field_traverse_reverse_depth_first (IR_NODE_MODE (graph), graph, class_field_flag, function);
}

void IR_traverse_reverse_depth_first
  (IR_node_t graph, int class_field_flag, int (*function) (IR_node_t node))
{
  _IR_current_graph_pass_number++;
  _IR_traverse_reverse_depth_first (graph, class_field_flag, function);
}

static IR_node_t _IR_transform_dag
(IR_node_t graph, int class_field_flag, int (*guard_function) (IR_node_t node),
 IR_node_t (*transformation_function) (IR_node_t node));

static void _IR_node_field_transformation
(IR_node_mode_t node_mode, IR_node_t graph, int class_field_flag, int (*guard_function) (IR_node_t node),
 IR_node_t (*transformation_function) (IR_node_t node))
{
  switch (node_mode)
    {
    case IR_NM_node:
      break;
    case IR_NM_expr:
      break;
    case IR_NM_binary:
      _IR_node_field_transformation (IR_NM_expr, (IR_node_t) ((char *) &((_IR_binary *) graph)->_IR_S_binary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, guard_function, transformation_function);
      (((_IR_binary *) graph)->_IR_S_binary.left)
        = _IR_transform_dag ((((_IR_binary *) graph)->_IR_S_binary.left), class_field_flag, guard_function, transformation_function);
      (((_IR_binary *) graph)->_IR_S_binary.right)
        = _IR_transform_dag ((((_IR_binary *) graph)->_IR_S_binary.right), class_field_flag, guard_function, transformation_function);
      break;
    case IR_NM_unary:
      _IR_node_field_transformation (IR_NM_expr, (IR_node_t) ((char *) &((_IR_unary *) graph)->_IR_S_unary._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, guard_function, transformation_function);
      (((_IR_unary *) graph)->_IR_S_unary.op)
        = _IR_transform_dag ((((_IR_unary *) graph)->_IR_S_unary.op), class_field_flag, guard_function, transformation_function);
      break;
    case IR_NM_e_plus:
      _IR_node_field_transformation (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_plus *) graph)->_IR_S_e_plus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, guard_function, transformation_function);
      break;
    case IR_NM_e_minus:
      _IR_node_field_transformation (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_minus *) graph)->_IR_S_e_minus._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, guard_function, transformation_function);
      break;
    case IR_NM_e_shl:
      _IR_node_field_transformation (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shl *) graph)->_IR_S_e_shl._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, guard_function, transformation_function);
      break;
    case IR_NM_e_shr:
      _IR_node_field_transformation (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_shr *) graph)->_IR_S_e_shr._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, guard_function, transformation_function);
      break;
    case IR_NM_e_assign:
      _IR_node_field_transformation (IR_NM_binary, (IR_node_t) ((char *) &((_IR_e_assign *) graph)->_IR_S_e_assign._IR_M_binary - _IR_offsetof (_IR_binary, _IR_S_binary)), class_field_flag, guard_function, transformation_function);
      break;
    case IR_NM_e_fetch:
      _IR_node_field_transformation (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_fetch *) graph)->_IR_S_e_fetch._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag, guard_function, transformation_function);
      break;
    case IR_NM_e_sym:
      _IR_node_field_transformation (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_sym *) graph)->_IR_S_e_sym._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, guard_function, transformation_function);
      break;
    case IR_NM_e_icon:
      _IR_node_field_transformation (IR_NM_expr, (IR_node_t) ((char *) &((_IR_e_icon *) graph)->_IR_S_e_icon._IR_M_expr - _IR_offsetof (_IR_expr, _IR_S_expr)), class_field_flag, guard_function, transformation_function);
      break;
    case IR_NM_e_cvt:
      _IR_node_field_transformation (IR_NM_unary, (IR_node_t) ((char *) &((_IR_e_cvt *) graph)->_IR_S_e_cvt._IR_M_unary - _IR_offsetof (_IR_unary, _IR_S_unary)), class_field_flag, guard_function, transformation_function);
      break;
    default:
      abort ();
      break;
    }
}

static IR_node_t _IR_transform_dag
(IR_node_t graph, int class_field_flag, int (*guard_function) (IR_node_t node),
 IR_node_t (*transformation_function) (IR_node_t node))
{
  if (graph == NULL || !(*guard_function) (graph))
    return graph;
  else if (graph->_IR_last_graph_pass_number == _IR_current_graph_pass_number)
    {
      if (graph->_IR_temporary == NULL)
        /* Graph cycle is occured. */
        graph->_IR_temporary = graph;
      return graph->_IR_temporary;
    }
  graph->_IR_temporary = NULL;
  graph->_IR_last_graph_pass_number = _IR_current_graph_pass_number;
  _IR_node_field_transformation (IR_NODE_MODE (graph), graph, class_field_flag, guard_function, transformation_function);
  if (graph->_IR_temporary == NULL)
    {
      /* No cycle. */
      graph->_IR_temporary = (*transformation_function) (graph);
      graph = graph->_IR_temporary;
    }
  return graph;
}

IR_node_t IR_transform_dag
(IR_node_t graph, int class_field_flag, int (*guard_function) (IR_node_t node),
 IR_node_t (*transformation_function) (IR_node_t node))
{
  _IR_current_graph_pass_number++;
  return
    _IR_transform_dag
      (graph, class_field_flag, guard_function, transformation_function);
}

void IR_start (void)
{
#ifdef __IR_DEBUG__
  if (sizeof (_IR_node) > 255)
    {
      fprintf (stderr, "Very long node `node' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_expr) > 255)
    {
      fprintf (stderr, "Very long node `expr' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_binary) > 255)
    {
      fprintf (stderr, "Very long node `binary' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_unary) > 255)
    {
      fprintf (stderr, "Very long node `unary' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_e_plus) > 255)
    {
      fprintf (stderr, "Very long node `e_plus' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_e_minus) > 255)
    {
      fprintf (stderr, "Very long node `e_minus' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_e_shl) > 255)
    {
      fprintf (stderr, "Very long node `e_shl' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_e_shr) > 255)
    {
      fprintf (stderr, "Very long node `e_shr' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_e_assign) > 255)
    {
      fprintf (stderr, "Very long node `e_assign' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_e_fetch) > 255)
    {
      fprintf (stderr, "Very long node `e_fetch' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_e_sym) > 255)
    {
      fprintf (stderr, "Very long node `e_sym' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_e_icon) > 255)
    {
      fprintf (stderr, "Very long node `e_icon' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
#ifdef __IR_DEBUG__
  if (sizeof (_IR_e_cvt) > 255)
    {
      fprintf (stderr, "Very long node `e_cvt' use option `-long-node-size' or `-short-node-size'\n");
      exit (1);
    }
#endif /* __IR_DEBUG__ */
  _IR_current_graph_pass_number = 0;
  IR_START_ALLOC ();
}

void IR_stop (void)
{
  IR_STOP_ALLOC ();
}



#line 54 "expr.sprut"


/*
void do_print ( IR_node_t node )
{
  IR_print_node( node, 0 );
};

int main ( void )
{ 
  IR_node_t x;

  IR_start();

  // a = b + 2 >> 1;

  x =
    IR_new_e_assign(
      IR_new_e_sym( "a" ),
      IR_new_e_shl(
        IR_new_e_plus(
          IR_new_e_fetch( IR_new_e_sym("b") ),
          IR_new_e_icon( 2 )
        ),
        IR_new_e_icon( 1 )
      )
    );

  IR_traverse_depth_first( x, 1, do_print );

  IR_stop();
};
*/

