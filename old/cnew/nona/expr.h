#ifndef __IR_expr__
#define __IR_expr__

#ifdef offsetof
#define _IR_offsetof(type, field) offsetof (type, field)
#else
#define _IR_offsetof(type, field) ((char *)&((type *) 0)->field - (char *) 0)
#endif


#line 1 "expr.sprut"

typedef int integer_t;
typedef char bool_t;
typedef char * string_t;
typedef void * void_t;

#define IR_PRINT_integer_t( x )  printf( "%d", x )
#define IR_PRINT_string_t( x )   printf( "%s", x )
#define IR_PRINT_void_t( x )     printf( "%p", x )



#line 25 "expr.h"

typedef enum IR_node_mode_enum
{
  IR_NM_node,
  IR_NM_expr,
  IR_NM_binary,
  IR_NM_unary,
  IR_NM_e_plus,
  IR_NM_e_minus,
  IR_NM_e_shl,
  IR_NM_e_shr,
  IR_NM_e_assign,
  IR_NM_e_fetch,
  IR_NM_e_sym,
  IR_NM_e_icon,
  IR_NM_e_cvt,
  IR_NM__root,
  IR_NM__error
} IR_node_mode_t;

extern short _IR_node_level [];

extern unsigned char _IR_SF_node [];

extern unsigned char _IR_SF_expr [];

extern unsigned char _IR_SF_binary [];

extern unsigned char _IR_SF_unary [];

extern unsigned char _IR_SF_e_plus [];

extern unsigned char _IR_SF_e_minus [];

extern unsigned char _IR_SF_e_shl [];

extern unsigned char _IR_SF_e_shr [];

extern unsigned char _IR_SF_e_assign [];

extern unsigned char _IR_SF_e_fetch [];

extern unsigned char _IR_SF_e_sym [];

extern unsigned char _IR_SF_e_icon [];

extern unsigned char _IR_SF_e_cvt [];

extern unsigned char _IR_SF__root [];

extern unsigned char _IR_SF__error [];

extern unsigned char *_IR_is_type[];

extern void *_IR_class_structure_array [];

typedef struct IR_node *IR_node_t;

typedef struct _IR_double_link *IR_double_link_t;

struct _IR_double_link
{
  IR_node_t field_itself;
  IR_node_t link_owner;
  void (*set_function) (IR_node_t, IR_node_t);
  IR_double_link_t previous_link;
  IR_double_link_t next_link;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
} _IR_node;

struct _IR_S_expr
{
  void_t  state;
  integer_t  len;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_expr _IR_S_expr;
} _IR_expr;

struct _IR_S_binary
{
  struct _IR_S_expr _IR_M_expr;
  IR_node_t  left;
  IR_node_t  right;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_binary _IR_S_binary;
} _IR_binary;

struct _IR_S_unary
{
  struct _IR_S_expr _IR_M_expr;
  IR_node_t  op;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_unary _IR_S_unary;
} _IR_unary;

struct _IR_S_e_plus
{
  struct _IR_S_binary _IR_M_binary;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_e_plus _IR_S_e_plus;
} _IR_e_plus;

struct _IR_S_e_minus
{
  struct _IR_S_binary _IR_M_binary;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_e_minus _IR_S_e_minus;
} _IR_e_minus;

struct _IR_S_e_shl
{
  struct _IR_S_binary _IR_M_binary;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_e_shl _IR_S_e_shl;
} _IR_e_shl;

struct _IR_S_e_shr
{
  struct _IR_S_binary _IR_M_binary;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_e_shr _IR_S_e_shr;
} _IR_e_shr;

struct _IR_S_e_assign
{
  struct _IR_S_binary _IR_M_binary;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_e_assign _IR_S_e_assign;
} _IR_e_assign;

struct _IR_S_e_fetch
{
  struct _IR_S_unary _IR_M_unary;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_e_fetch _IR_S_e_fetch;
} _IR_e_fetch;

struct _IR_S_e_sym
{
  struct _IR_S_expr _IR_M_expr;
  string_t  name;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_e_sym _IR_S_e_sym;
} _IR_e_sym;

struct _IR_S_e_icon
{
  struct _IR_S_expr _IR_M_expr;
  integer_t  value;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_e_icon _IR_S_e_icon;
} _IR_e_icon;

struct _IR_S_e_cvt
{
  struct _IR_S_unary _IR_M_unary;
};

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
  struct _IR_S_e_cvt _IR_S_e_cvt;
} _IR_e_cvt;

typedef struct IR_node
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
} _IR__root;

typedef struct
{
  IR_node_mode_t _IR_node_mode;
  int _IR_last_graph_pass_number;
  IR_node_t _IR_temporary;
} _IR__error;

#define IR_NODE_MODE(t) ((t)->_IR_node_mode)

extern char *IR_node_name[];

extern unsigned char IR_node_size[];

#define IR_NODE_LEVEL(node) _IR_node_level [IR_NODE_MODE (node)]

#define IR_IS_TYPE(type, super) ((_IR_is_type [super] [type /8] >> (type % 8)) & 1)

#define IR_IS_OF_TYPE(node, super)IR_IS_TYPE (IR_NODE_MODE (node), super)

extern void _IR_set_double_field_value
  (IR_double_link_t link, IR_node_t value, int disp, int flag);

extern IR_double_link_t IR__first_double_link (IR_node_t node);

#define IR__next_double_link(prev_double_link) ((prev_double_link)->next_link)

extern IR_double_link_t IR_F__next_double_link (IR_double_link_t prev_double_link);

#define IR__previous_double_link(next_double_link) ((next_double_link)->previous_link)

extern IR_double_link_t IR_F__previous_double_link (IR_double_link_t next_double_link);

#define IR__owner(link) ((link)->link_owner)

extern IR_node_t IR_F__owner (IR_double_link_t link);

extern void_t IR_F_state (IR_node_t _node);

extern integer_t IR_F_len (IR_node_t _node);

extern IR_node_t IR_F_left (IR_node_t _node);

extern IR_node_t IR_F_right (IR_node_t _node);

extern IR_node_t IR_F_op (IR_node_t _node);

extern string_t IR_F_name (IR_node_t _node);

extern integer_t IR_F_value (IR_node_t _node);

#define IR_state(_node) (((_IR_expr *) (_node))->_IR_S_expr.state)

#define IR_len(_node) (((_IR_expr *) (_node))->_IR_S_expr.len)

#define IR_left(_node) (((_IR_binary *) (_node))->_IR_S_binary.left)

#define IR_right(_node) (((_IR_binary *) (_node))->_IR_S_binary.right)

#define IR_op(_node) (((_IR_unary *) (_node))->_IR_S_unary.op)

#define IR_name(_node) (((_IR_e_sym *) (_node))->_IR_S_e_sym.name)

#define IR_value(_node) (((_IR_e_icon *) (_node))->_IR_S_e_icon.value)

extern void IR__set_double_link (IR_double_link_t link, IR_node_t value);

extern void IR_F_set_state (IR_node_t _node, void_t _value);

extern void IR_F_set_len (IR_node_t _node, integer_t _value);

extern void IR_F_set_left (IR_node_t _node, IR_node_t _value);

extern void IR_F_set_right (IR_node_t _node, IR_node_t _value);

extern void IR_F_set_op (IR_node_t _node, IR_node_t _value);

extern void IR_F_set_name (IR_node_t _node, string_t _value);

extern void IR_F_set_value (IR_node_t _node, integer_t _value);

#define IR_set_state(_node, _value) ((((_IR_expr *) (_node))->_IR_S_expr.state) = (_value))

#define IR_set_len(_node, _value) ((((_IR_expr *) (_node))->_IR_S_expr.len) = (_value))

#define IR_set_left(_node, _value) ((((_IR_binary *) (_node))->_IR_S_binary.left) = (_value))

#define IR_set_right(_node, _value) ((((_IR_binary *) (_node))->_IR_S_binary.right) = (_value))

#define IR_set_op(_node, _value) ((((_IR_unary *) (_node))->_IR_S_unary.op) = (_value))

#define IR_set_name(_node, _value) ((((_IR_e_sym *) (_node))->_IR_S_e_sym.name) = (_value))

#define IR_set_value(_node, _value) ((((_IR_e_icon *) (_node))->_IR_S_e_icon.value) = (_value))

extern IR_node_t IR_create_node (IR_node_mode_t node_mode);

extern IR_node_t IR_new_e_plus
(integer_t len,
 IR_node_t left,
 IR_node_t right);

extern IR_node_t IR_new_e_minus
(integer_t len,
 IR_node_t left,
 IR_node_t right);

extern IR_node_t IR_new_e_shl
(integer_t len,
 IR_node_t left,
 IR_node_t right);

extern IR_node_t IR_new_e_shr
(integer_t len,
 IR_node_t left,
 IR_node_t right);

extern IR_node_t IR_new_e_assign
(integer_t len,
 IR_node_t left,
 IR_node_t right);

extern IR_node_t IR_new_e_fetch
(integer_t len,
 IR_node_t op);

extern IR_node_t IR_new_e_sym
(integer_t len,
 string_t name);

extern IR_node_t IR_new_e_icon
(integer_t len,
 integer_t value);

extern IR_node_t IR_new_e_cvt
(integer_t len,
 IR_node_t op);

extern void IR_free_node (IR_node_t node);

extern void IR_free_graph (IR_node_t graph);

extern void IR_conditional_free_graph (IR_node_t graph, int (* _IR_traverse_guard_function_parameter) (IR_node_t node));

extern IR_node_t IR_copy_node (IR_node_t node);

extern IR_node_t IR_copy_graph (IR_node_t graph);

extern IR_node_t IR_conditional_copy_graph (IR_node_t graph, int (* _IR_traverse_guard_function_parameter) (IR_node_t node));

extern int IR_is_equal_node (IR_node_t node_1, IR_node_t node_2);

extern int IR_is_equal_graph (IR_node_t graph_1, IR_node_t graph_2);

extern int IR_conditional_is_equal_graph (IR_node_t graph_1, IR_node_t graph_2, int (* _IR_traverse_guard_function_parameter) (IR_node_t node));

extern int IR_check_node (IR_node_t node, int class_field_flag);

extern int IR_check_graph (IR_node_t graph, int class_field_flag);

extern int IR_conditional_check_graph (IR_node_t graph, int class_field_flag, int (* _IR_traverse_guard_function_parameter) (IR_node_t node));

extern void IR_print_node (IR_node_t node, int class_field_flag);

extern int IR_output_node (FILE *output_file, IR_node_t node, int level);

extern IR_node_t IR_input_node (FILE *input_file, IR_node_t *original_address);

extern void IR_traverse_depth_first
  (IR_node_t graph, int class_field_flag, void (*function) (IR_node_t node));

extern void IR_traverse_reverse_depth_first
  (IR_node_t graph, int class_field_flag, int (*function) (IR_node_t node));

extern IR_node_t IR_transform_dag
(IR_node_t graph, int class_field_flag, int (*guard_function) (IR_node_t node),
 IR_node_t (*transformation_function) (IR_node_t node));

extern void IR_start (void);

extern void IR_stop (void);

#endif /* __IR_expr__ */
