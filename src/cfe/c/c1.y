%{#include <stdio.h>%}


































%union
{
  int ast;
  int str;
  int token;
  int sym;
  int scope;
  int typ;
  int dspecs;
  int tnode;
  int edesc;
  int m_typ;
}



%token <token> 
C_VOID 
C_CHAR 
C_SHORT 
C_INT 
C_LONG 
C_SIGNED 
C_UNSIGNED 
C_FLOAT 
C_DOUBLE 
C__BOOL 
C__COMPLEX 
C__IMAGINARY 


%token C_EXT_INT 
%token C_EXT_FLOAT 



%token <token> 
C_CONST 
C_VOLATILE 
C_RESTRICT 



%token <token> 
C_EXTERN 
C_AUTO 
C_STATIC 
C_REGISTER 
C_TYPEDEF 
C_INLINE 



%token <token> 
STRUCT 
UNION 
ENUM 

%token 
SWITCH 
CONTINUE 
CASE 
SIZEOF 
DEFAULT 
ELSE 
WHILE 
DO 
GOTO 
BREAK 
IF 
FOR 
RETURN 

%token <str> IDENT 
%token <sym> TYPENAME 

%token <ast> 
NUMBER 
F_NUMBER 
STRING_CONST 
LSTRING_CONST 



%token 
LOG_OR 
LOG_AND 
INC 
DEC 
PTR 
ELLIPSIS 



%token 
ASSIGN 
EQCOMPARE 
ARITHCOMPARE 
SHIFT 


%token DECLSPEC 


%token ATTR 



%token 
TYPEOF 
LABEL 
ALIGNOF 
GCC_ATTRIBUTE 


%left IF 
%left ELSE 





%expect 3 

%type<ast> constant string_literal %type<str> identifier any_identifier %type<ast> primary_expression expression postfix_expression assignment_expression unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression expression_opt assignment_expression_opt expression1 constant_expression %type<dspecs> GenType_declaration_specifiers declaration_specifiers storage_class_specifier declaration_specifiers_opt function_specifier type_qualifier type_specifier declaration_specifiers1 typedef_name declaration_specifiers2 %type<typ> GetDSpecsTyp %type<sym> declarator %type<token> sclass %type<dspecs> declspec %type<token> typespec %type<dspecs> extended_int_specifier extended_float_specifier struct_or_union_specifier enum_specifier %type<token> struct_or_union %type<tnode> declare_tag %type<scope> struct_scope %type<str> null_str %type<dspecs> GenType_specifier_qualifier_list specifier_qualifier_list specifier_qualifier_list1 specifier_qualifier_list2 %type<edesc> declare_enum %type<str> enumeration_constant %type<token> qual %type<typ> pointer_opt %type<sym> direct_declarator %type<typ> direct_declarator_tail %type<m_typ> type_qualifier_list_opt GenQual_type_qualifier_list %type<scope> param_scope identifier_list_scope %type<typ> left_bra_NullTyp %type<sym> declarator1 direct_declarator1 declarator2 direct_declarator2 %type<typ> qual_pointer pointer %type<dspecs> type_qualifier_list %type<sym> abstract_declarator direct_abstract_declarator %type<typ> direct_abstract_declarator_tail direct_abstract_declarator_tail_opt %type<scope> declaration_list_scope %% 

start: 
translation_unit {} 
| 
;







garbage_collect: 
{} 
;









ensure_file_scope: 
{} 
;









comma_opt: 

| ',' 
;








constant: 
NUMBER 
| F_NUMBER 



;

string_literal: 
STRING_CONST 
| LSTRING_CONST 
| string_literal STRING_CONST 
{} 
| string_literal LSTRING_CONST 
{} 
;




























identifier: 
IDENT 
;

any_identifier: 
TYPENAME {} 
| IDENT 
;







primary_expression: 
identifier {} 
| constant 
| string_literal {} 
| '(' expression ')' {} 

| '(' compound_statement ')' {} 
;


postfix_expression: 
primary_expression 
| postfix_expression '[' expression ']' {} 
| postfix_expression '(' argument_expression_list_opt ')' {} 
| postfix_expression '.' any_identifier {} 
| postfix_expression PTR any_identifier {} 
| postfix_expression INC {} 
| postfix_expression DEC {} 
| '(' type_name ')' '{' initializer_list comma_opt '}' {} 
;

argument_expression_list: 
assignment_expression {} 
| argument_expression_list ',' assignment_expression {} 
;

argument_expression_list_opt: 

| argument_expression_list 
;


unary_expression: 
postfix_expression 
| INC unary_expression {} 
| DEC unary_expression {} 
| '*' cast_expression {} 
| '&' cast_expression {} 
| unary_operator cast_expression {} 
| SIZEOF unary_expression {} 
| SIZEOF '(' type_name ')' {} 


| ALIGNOF unary_expression {} 
| ALIGNOF '(' type_name ')' {} 
| LOG_AND any_identifier {} 
;

unary_operator: 
'+' | '-' | '~' | '!' 
;



cast_expression: 
unary_expression 
| '(' type_name ')' cast_expression {} 
;


multiplicative_expression: 
cast_expression 
| multiplicative_expression '*' cast_expression {} 
| multiplicative_expression '/' cast_expression {} 
| multiplicative_expression '%' cast_expression {} 
;


additive_expression: 
multiplicative_expression 
| additive_expression '+' multiplicative_expression {} 
| additive_expression '-' multiplicative_expression {} 
;


shift_expression: 
additive_expression 
| shift_expression SHIFT additive_expression {} 
;


relational_expression: 
shift_expression 
| relational_expression ARITHCOMPARE shift_expression {} 
;


equality_expression: 
relational_expression 
| equality_expression EQCOMPARE relational_expression {} 
;


and_expression: 
equality_expression 
| and_expression '&' equality_expression {} 
;


exclusive_or_expression: 
and_expression 
| exclusive_or_expression '^' and_expression {} 
;


inclusive_or_expression: 
exclusive_or_expression 
| inclusive_or_expression '|' exclusive_or_expression {} 
;


logical_and_expression: 
inclusive_or_expression 
| logical_and_expression LOG_AND inclusive_or_expression {} 
;


logical_or_expression: 
logical_and_expression 
| logical_or_expression LOG_OR logical_and_expression {} 
;


conditional_expression: 
logical_or_expression 

| logical_or_expression '?' expression_opt ':' conditional_expression {} 
;


assignment_expression: 
conditional_expression 
| unary_expression assignment_operator assignment_expression 
{} 
;

assignment_expression_opt: 
{} 
| assignment_expression 
;

assignment_operator: 
ASSIGN | '=' 
;


expression1: 
assignment_expression 
| expression1 ',' assignment_expression {} 
;

expression: 
expression1 
;

expression_opt: 
{} 
| expression 
;


constant_expression: 
conditional_expression 
{} 
;


































declaration: 
GenType_declaration_specifiers 
init_declarator_list_opt ';' 
{} 
;

GenType_declaration_specifiers: 
declaration_specifiers {} 
;














declaration_specifiers: 
storage_class_specifier declaration_specifiers_opt {} 
| function_specifier declaration_specifiers_opt {} 
| type_qualifier declaration_specifiers_opt {} 

| type_specifier declaration_specifiers1 {} 
| typedef_name declaration_specifiers2 {} 
;

declaration_specifiers_opt: 
{} 
| declaration_specifiers 
;

declaration_specifiers1: 
{} 
| storage_class_specifier declaration_specifiers1 {} 
| function_specifier declaration_specifiers1 {} 
| type_qualifier declaration_specifiers1 {} 
| type_specifier declaration_specifiers1 {} 
;

declaration_specifiers2: 
{} 
| storage_class_specifier declaration_specifiers2 {} 
| function_specifier declaration_specifiers2 {} 
| type_qualifier declaration_specifiers2 {} 
;

init_declarator_list: 
init_declarator 
| init_declarator_list ',' { return "YYST_DSPECS"; } init_declarator 
;

init_declarator_list_opt: 
GetDSpecsTyp 
{} 
| init_declarator_list 
;

GetDSpecsTyp: 
{} 
;

init_declarator: 
GetDSpecsTyp declarator 
{} 
| GetDSpecsTyp declarator '=' initializer 
{} 
;


storage_class_specifier: 
sclass {} 

| declspec 
;

sclass: 

C_TYPEDEF 
| C_EXTERN 
| C_STATIC 
| C_AUTO 
| C_REGISTER 
;







declspec: 
DECLSPEC '(' any_identifier ')' 
{} 
;














type_specifier: 
typespec {} 
| extended_int_specifier 
| extended_float_specifier 
| struct_or_union_specifier 
| enum_specifier 


| TYPEOF '(' expression ')' {} 
| TYPEOF '(' type_name ')' {} 
;

typespec: 
C_VOID 
| C_CHAR 
| C_SHORT 
| C_INT 
| C_LONG 
| C_SIGNED 
| C_UNSIGNED 
| C_FLOAT 
| C_DOUBLE 
| C__BOOL 
| C__COMPLEX 
| C__IMAGINARY 
;


extended_int_specifier: 
C_EXT_INT '(' NUMBER ')' 
{} 
;


extended_float_specifier: 
C_EXT_FLOAT '(' NUMBER ')' 
{} 
;



struct_or_union_specifier: 
struct_or_union any_identifier declare_tag 
struct_scope 
{} 
| struct_or_union null_str declare_tag 
struct_scope 
{} 
| struct_or_union any_identifier 
{} 
;

null_str: {} ;

declare_tag: 
{} 
;

struct_scope: 
'{' 
{} 
struct_declaration_list 
'}' 
{} 
;

struct_or_union: 
STRUCT 
| UNION 
;

struct_declaration_list: 
struct_declaration 
| struct_declaration_list struct_declaration 
| error 
;

struct_declaration: 









GenType_declaration_specifiers 
struct_declarator_list ';' 
{} 
;

GenType_specifier_qualifier_list: 
specifier_qualifier_list {} 
;








specifier_qualifier_list: 
type_qualifier specifier_qualifier_list {} 
| type_qualifier 
| type_specifier specifier_qualifier_list1 {} 
| typedef_name specifier_qualifier_list2 {} 
;

specifier_qualifier_list1: 
{} 
| type_qualifier specifier_qualifier_list1 {} 
| type_specifier specifier_qualifier_list1 {} 
;

specifier_qualifier_list2: 
{} 
| type_qualifier specifier_qualifier_list2 {} 
;

struct_declarator_list: 
struct_declarator 
| struct_declarator_list ',' { return "YYST_DSPECS"; } struct_declarator 
;

struct_declarator: 
GetDSpecsTyp declarator 
{} 
| GetDSpecsTyp declarator ':' constant_expression 
{} 
| GetDSpecsTyp ':' constant_expression 
{} 
;


enum_specifier: 
ENUM any_identifier declare_enum 
'{' { return "YYST_EDESC"; } enumerator_list comma_opt '}' 
{} 
| ENUM null_str declare_enum 
'{' { return "YYST_EDESC"; } enumerator_list comma_opt '}' 
{} 
| ENUM any_identifier 
{} 
;

declare_enum: 
declare_tag 
{} 
;

enumerator_list: 
enumerator 
| enumerator_list ',' { return "YYST_EDESC"; } enumerator 
| error 
;

enumerator: 
enumeration_constant 
{} 
| enumeration_constant '=' constant_expression 
{} 
;

enumeration_constant: 
any_identifier 
;


type_qualifier: 
qual {} 






| ATTR '(' any_identifier ')' 
{} 
;

qual: 
C_CONST 
| C_RESTRICT 
| C_VOLATILE 
;



function_specifier: 
C_INLINE {} 
;



declarator: 
pointer_opt direct_declarator 
{} 
;

direct_declarator_tail: 

{} 
| '[' type_qualifier_list_opt assignment_expression_opt ']' { return "YYST_TYP"; } direct_declarator_tail 
{} 
| '[' C_STATIC type_qualifier_list_opt assignment_expression ']' { return "YYST_TYP"; } direct_declarator_tail 
{} 
| '[' GenQual_type_qualifier_list C_STATIC assignment_expression ']' { return "YYST_TYP"; } direct_declarator_tail 
{} 
| '[' type_qualifier_list_opt '*' ']' { return "YYST_TYP"; } direct_declarator_tail 
{} 
| param_scope { return "YYST_TYP"; } direct_declarator_tail 
{} 
| '(' ')' { return "YYST_TYP"; } direct_declarator_tail 
{} 
| identifier_list_scope { return "YYST_TYP"; } direct_declarator_tail 
{} 
;

param_scope: 
'(' 
{} 
parameter_type_list ')' 
{} 
;

identifier_list_scope: 
'(' 
{} 
identifier_list ')' 
{} 
;






















left_bra_NullTyp: 
'(' 
{} 
;

direct_declarator: 
any_identifier { return "YYST_TYP"; } direct_declarator_tail 
{} 
| left_bra_NullTyp declarator ')' { return "YYST_TYP"; } direct_declarator_tail 
{} 
;







declarator1: 
pointer_opt direct_declarator1 
{} 
;

direct_declarator1: 
any_identifier { return "YYST_TYP"; } direct_declarator_tail 
{} 
| left_bra_NullTyp declarator2 ')' { return "YYST_TYP"; } direct_declarator_tail 
{} 
;

declarator2: 
pointer_opt direct_declarator2 
{} 
;

direct_declarator2: 
identifier { return "YYST_TYP"; } direct_declarator_tail 
{} 
| left_bra_NullTyp declarator2 ')' { return "YYST_TYP"; } direct_declarator_tail 
{} 
;







qual_pointer: 
'*' type_qualifier_list_opt 
{} 
;

pointer: 
qual_pointer 
| pointer qual_pointer {} 
;

pointer_opt: 
{} 
| pointer 
;

type_qualifier_list: 
type_qualifier 
| type_qualifier_list type_qualifier {} 
;

GenQual_type_qualifier_list: 
type_qualifier_list {} 
;


type_qualifier_list_opt: 
{} 
| GenQual_type_qualifier_list 
;




forward_parameter_decl: 

;









parameter_type_list: 
forward_parameter_decl parameter_list 

| error 
;










parameter_list: 
parameter_declaration 
| parameter_list ',' parameter_declaration 
;

parameter_declaration: 
GenType_declaration_specifiers 
GetDSpecsTyp 
declarator1 
{} 
| GenType_declaration_specifiers 
GetDSpecsTyp 
abstract_declarator 
{} 
| GenType_declaration_specifiers 
GetDSpecsTyp 
{} 
| ELLIPSIS 
{} 
;









identifier_list: 
identifier {} 
| identifier_list ',' any_identifier {} 
;


type_name: 
GenType_specifier_qualifier_list 
GetDSpecsTyp 
abstract_declarator 
{} 
| GenType_specifier_qualifier_list 
{} 
;

abstract_declarator: 
pointer 
{} 
| pointer_opt direct_abstract_declarator 
{} 
;

direct_abstract_declarator_tail: 
'[' assignment_expression_opt ']' { return "YYST_TYP"; } direct_abstract_declarator_tail_opt 
{} 
| '[' '*' ']' { return "YYST_TYP"; } direct_abstract_declarator_tail_opt 
{} 
| '(' ')' { return "YYST_TYP"; } direct_abstract_declarator_tail_opt 
{} 
| param_scope { return "YYST_TYP"; } direct_abstract_declarator_tail_opt 
{} 
;

direct_abstract_declarator_tail_opt: 

{} 
| direct_abstract_declarator_tail 
;

direct_abstract_declarator: 
left_bra_NullTyp abstract_declarator ')' 
{ return "YYST_TYP"; } direct_abstract_declarator_tail_opt 
{} 
| direct_abstract_declarator_tail 
{} 
;


typedef_name: 
TYPENAME 
{} 
;


initializer: 
assignment_expression {} 
| '{' initializer_list comma_opt '}' 
;

initializer_list: 
designation_opt initializer 
| initializer_list ',' designation_opt initializer 
;

designation: 
designator_list '=' 
;

designation_opt: 

| designation 
;

designator_list: 
designator 
| designator_list designator 
;

designator: 
'[' constant_expression ']' 
| '.' any_identifier 


| '[' constant_expression ELLIPSIS constant_expression ']' 

;








statement: 
labeled_statement 
| compound_statement 
| expression_statement 
| selection_statement 
| iteration_statement 
| jump_statement 
;


labeled_statement: 
any_identifier ':' statement {} 
| CASE constant_expression ':' statement 
| DEFAULT ':' statement 


| CASE constant_expression ELLIPSIS constant_expression ':' statement 
;


compound_statement: 
'{' 
{} 
declare_labels_list_opt block_item_list_opt '}' 
{} 
;





function_compound_statement: 
'{' 
declare_labels_list_opt block_item_list_opt '}' 
{} 
;



declare_labels_list: 
declare_labels ';' 
| declare_labels_list declare_labels ';' 
;

declare_labels_list_opt: 

| declare_labels_list 
;

declare_labels: 
LABEL any_identifier 
| declare_labels ',' any_identifier 
;

block_item_list: 
block_item 
| block_item_list block_item 
;

block_item_list_opt: 

| block_item_list 
;

block_item: 
declaration 
| statement 
;


expression_statement: 
expression_opt ';' {} 
;


selection_statement: 
IF '(' expression ')' statement %prec IF 
| IF '(' expression ')' statement ELSE statement %prec ELSE 
| SWITCH '(' expression ')' statement 
;


iteration_statement: 
WHILE '(' expression ')' statement 
| DO statement WHILE '(' expression ')' ';' 
| FOR '(' expression_opt ';' expression_opt ';' expression_opt ')' statement 
| FOR '(' declaration expression_opt ';' expression_opt ')' statement 
;


jump_statement: 
GOTO any_identifier ';' 

| GOTO '*' cast_expression ';' 

| CONTINUE ';' 
| BREAK ';' 
| RETURN expression_opt ';' 
;








translation_unit: 
external_declaration 
ensure_file_scope 
garbage_collect 
| translation_unit external_declaration 
ensure_file_scope 
garbage_collect 
;

external_declaration: 
function_definition 
| declaration 
| error 


| ';' {} 
;


function_definition: 
GenType_declaration_specifiers 
GetDSpecsTyp 
declarator 
{} 
function_compound_statement 
| GenType_declaration_specifiers 
GetDSpecsTyp 
declarator 
declaration_list_scope 
{} 
function_compound_statement 
;

declaration_list_scope: 
{} 
declaration_list 
{} 
;

declaration_list: 
declaration 
| declaration_list declaration 
| error 
;

%%

static const char * yytypstrs[] =
{
  "YYST_AST",
  "YYST_STR",
  "YYST_TOKEN",
  "YYST_SYM",
  "YYST_SCOPE",
  "YYST_TYP",
  "YYST_DSPECS",
  "YYST_TNODE",
  "YYST_EDESC",
  "YYST_M_TYP",
  0
};

static const char * yytoktyp_str[][3] =
{
  { "C_VOID", "void", "YYST_TOKEN" },
  { "C_CHAR", "char", "YYST_TOKEN" },
  { "C_SHORT", "short", "YYST_TOKEN" },
  { "C_INT", "int", "YYST_TOKEN" },
  { "C_LONG", "long", "YYST_TOKEN" },
  { "C_SIGNED", "signed", "YYST_TOKEN" },
  { "C_UNSIGNED", "unsigned", "YYST_TOKEN" },
  { "C_FLOAT", "float", "YYST_TOKEN" },
  { "C_DOUBLE", "double", "YYST_TOKEN" },
  { "C__BOOL", "_Bool", "YYST_TOKEN" },
  { "C__COMPLEX", "_Complex", "YYST_TOKEN" },
  { "C__IMAGINARY", "_Imaginary", "YYST_TOKEN" },
  { "C_EXT_INT", 0, 0 },
  { "C_EXT_FLOAT", 0, 0 },
  { "C_CONST", "const", "YYST_TOKEN" },
  { "C_VOLATILE", "volatile", "YYST_TOKEN" },
  { "C_RESTRICT", "restrict", "YYST_TOKEN" },
  { "C_EXTERN", "extern", "YYST_TOKEN" },
  { "C_AUTO", "auto", "YYST_TOKEN" },
  { "C_STATIC", "static", "YYST_TOKEN" },
  { "C_REGISTER", "register", "YYST_TOKEN" },
  { "C_TYPEDEF", "typedef", "YYST_TOKEN" },
  { "C_INLINE", "inline", "YYST_TOKEN" },
  { "STRUCT", "struct", "YYST_TOKEN" },
  { "UNION", "union", "YYST_TOKEN" },
  { "ENUM", "enum", "YYST_TOKEN" },
  { "SWITCH", "switch", 0 },
  { "CONTINUE", "continue", 0 },
  { "CASE", "case", 0 },
  { "SIZEOF", "sizeof", 0 },
  { "DEFAULT", "default", 0 },
  { "ELSE", "else", 0 },
  { "WHILE", "while", 0 },
  { "DO", "do", 0 },
  { "GOTO", "goto", 0 },
  { "BREAK", "break", 0 },
  { "IF", "if", 0 },
  { "FOR", "for", 0 },
  { "RETURN", "return", 0 },
  { "IDENT", "identifier", "YYST_STR" },
  { "TYPENAME", "type name", "YYST_SYM" },
  { "NUMBER", "integer number", "YYST_AST" },
  { "F_NUMBER", "real number", "YYST_AST" },
  { "STRING_CONST", "string constant", "YYST_AST" },
  { "LSTRING_CONST", "wide string constant", "YYST_AST" },
  { "LOG_OR", "||", 0 },
  { "LOG_AND", "&&", 0 },
  { "INC", "++", 0 },
  { "DEC", "--", 0 },
  { "PTR", "->", 0 },
  { "ELLIPSIS", "...", 0 },
  { "ASSIGN", "'*=', '/=', '%=', '+=', '-=', '&=', '|=', '^=', '<<=' or '>>='", 0 },
  { "EQCOMPARE", "'==' or '!='", 0 },
  { "ARITHCOMPARE", "'<=', '>=', '<' or '>'", 0 },
  { "SHIFT", "'<<' or '>>'", 0 },
  { "DECLSPEC", "__declspec", 0 },
  { "ATTR", "__attr", 0 },
  { "TYPEOF", "__typeof__", 0 },
  { "LABEL", "__label__", 0 },
  { "ALIGNOF", "__alignof__", 0 },
  { "GCC_ATTRIBUTE", "__attribute__", 0 },
  { "start", 0, 0 },
  { "translation_unit", 0, 0 },
  { "garbage_collect", 0, 0 },
  { "ensure_file_scope", 0, 0 },
  { "comma_opt", 0, 0 },
  { "','", 0, 0 },
  { "constant", 0, "YYST_AST" },
  { "string_literal", 0, "YYST_AST" },
  { "identifier", 0, "YYST_STR" },
  { "any_identifier", 0, "YYST_STR" },
  { "primary_expression", 0, "YYST_AST" },
  { "'('", 0, 0 },
  { "expression", 0, "YYST_AST" },
  { "')'", 0, 0 },
  { "compound_statement", 0, 0 },
  { "postfix_expression", 0, "YYST_AST" },
  { "'['", 0, 0 },
  { "']'", 0, 0 },
  { "argument_expression_list_opt", 0, 0 },
  { "'.'", 0, 0 },
  { "type_name", 0, 0 },
  { "'{'", 0, 0 },
  { "initializer_list", 0, 0 },
  { "'}'", 0, 0 },
  { "argument_expression_list", 0, 0 },
  { "assignment_expression", 0, "YYST_AST" },
  { "unary_expression", 0, "YYST_AST" },
  { "'*'", 0, 0 },
  { "cast_expression", 0, "YYST_AST" },
  { "'&'", 0, 0 },
  { "unary_operator", 0, 0 },
  { "'+'", 0, 0 },
  { "'-'", 0, 0 },
  { "'~'", 0, 0 },
  { "'!'", 0, 0 },
  { "multiplicative_expression", 0, "YYST_AST" },
  { "'/'", 0, 0 },
  { "'%'", 0, 0 },
  { "additive_expression", 0, "YYST_AST" },
  { "shift_expression", 0, "YYST_AST" },
  { "relational_expression", 0, "YYST_AST" },
  { "equality_expression", 0, "YYST_AST" },
  { "and_expression", 0, "YYST_AST" },
  { "exclusive_or_expression", 0, "YYST_AST" },
  { "'^'", 0, 0 },
  { "inclusive_or_expression", 0, "YYST_AST" },
  { "'|'", 0, 0 },
  { "logical_and_expression", 0, "YYST_AST" },
  { "logical_or_expression", 0, "YYST_AST" },
  { "conditional_expression", 0, "YYST_AST" },
  { "'?'", 0, 0 },
  { "expression_opt", 0, "YYST_AST" },
  { "':'", 0, 0 },
  { "assignment_operator", 0, 0 },
  { "assignment_expression_opt", 0, "YYST_AST" },
  { "'='", 0, 0 },
  { "expression1", 0, "YYST_AST" },
  { "constant_expression", 0, "YYST_AST" },
  { "declaration", 0, 0 },
  { "GenType_declaration_specifiers", 0, "YYST_DSPECS" },
  { "init_declarator_list_opt", 0, 0 },
  { "';'", 0, 0 },
  { "declaration_specifiers", 0, "YYST_DSPECS" },
  { "storage_class_specifier", 0, "YYST_DSPECS" },
  { "declaration_specifiers_opt", 0, "YYST_DSPECS" },
  { "function_specifier", 0, "YYST_DSPECS" },
  { "type_qualifier", 0, "YYST_DSPECS" },
  { "type_specifier", 0, "YYST_DSPECS" },
  { "declaration_specifiers1", 0, "YYST_DSPECS" },
  { "typedef_name", 0, "YYST_DSPECS" },
  { "declaration_specifiers2", 0, "YYST_DSPECS" },
  { "init_declarator_list", 0, 0 },
  { "init_declarator", 0, 0 },
  { "GetDSpecsTyp", 0, "YYST_TYP" },
  { "declarator", 0, "YYST_SYM" },
  { "initializer", 0, 0 },
  { "sclass", 0, "YYST_TOKEN" },
  { "declspec", 0, "YYST_DSPECS" },
  { "typespec", 0, "YYST_TOKEN" },
  { "extended_int_specifier", 0, "YYST_DSPECS" },
  { "extended_float_specifier", 0, "YYST_DSPECS" },
  { "struct_or_union_specifier", 0, "YYST_DSPECS" },
  { "enum_specifier", 0, "YYST_DSPECS" },
  { "struct_or_union", 0, "YYST_TOKEN" },
  { "declare_tag", 0, "YYST_TNODE" },
  { "struct_scope", 0, "YYST_SCOPE" },
  { "null_str", 0, "YYST_STR" },
  { "struct_declaration_list", 0, 0 },
  { "struct_declaration", 0, 0 },
  { "error", 0, 0 },
  { "struct_declarator_list", 0, 0 },
  { "GenType_specifier_qualifier_list", 0, "YYST_DSPECS" },
  { "specifier_qualifier_list", 0, "YYST_DSPECS" },
  { "specifier_qualifier_list1", 0, "YYST_DSPECS" },
  { "specifier_qualifier_list2", 0, "YYST_DSPECS" },
  { "struct_declarator", 0, 0 },
  { "declare_enum", 0, "YYST_EDESC" },
  { "enumerator_list", 0, 0 },
  { "enumerator", 0, 0 },
  { "enumeration_constant", 0, "YYST_STR" },
  { "qual", 0, "YYST_TOKEN" },
  { "pointer_opt", 0, "YYST_TYP" },
  { "direct_declarator", 0, "YYST_SYM" },
  { "direct_declarator_tail", 0, "YYST_TYP" },
  { "type_qualifier_list_opt", 0, "YYST_M_TYP" },
  { "GenQual_type_qualifier_list", 0, "YYST_M_TYP" },
  { "param_scope", 0, "YYST_SCOPE" },
  { "identifier_list_scope", 0, "YYST_SCOPE" },
  { "parameter_type_list", 0, 0 },
  { "identifier_list", 0, 0 },
  { "left_bra_NullTyp", 0, "YYST_TYP" },
  { "declarator1", 0, "YYST_SYM" },
  { "direct_declarator1", 0, "YYST_SYM" },
  { "declarator2", 0, "YYST_SYM" },
  { "direct_declarator2", 0, "YYST_SYM" },
  { "qual_pointer", 0, "YYST_TYP" },
  { "pointer", 0, "YYST_TYP" },
  { "type_qualifier_list", 0, "YYST_DSPECS" },
  { "forward_parameter_decl", 0, 0 },
  { "parameter_list", 0, 0 },
  { "parameter_declaration", 0, 0 },
  { "abstract_declarator", 0, "YYST_SYM" },
  { "direct_abstract_declarator", 0, "YYST_SYM" },
  { "direct_abstract_declarator_tail", 0, "YYST_TYP" },
  { "direct_abstract_declarator_tail_opt", 0, "YYST_TYP" },
  { "designation_opt", 0, 0 },
  { "designation", 0, 0 },
  { "designator_list", 0, 0 },
  { "designator", 0, 0 },
  { "statement", 0, 0 },
  { "labeled_statement", 0, 0 },
  { "expression_statement", 0, 0 },
  { "selection_statement", 0, 0 },
  { "iteration_statement", 0, 0 },
  { "jump_statement", 0, 0 },
  { "declare_labels_list_opt", 0, 0 },
  { "block_item_list_opt", 0, 0 },
  { "function_compound_statement", 0, 0 },
  { "declare_labels_list", 0, 0 },
  { "declare_labels", 0, 0 },
  { "block_item_list", 0, 0 },
  { "block_item", 0, 0 },
  { "external_declaration", 0, 0 },
  { "function_definition", 0, 0 },
  { "declaration_list_scope", 0, "YYST_SCOPE" },
  { "declaration_list", 0, 0 },
  { 0, 0 }
};
#include "prebis_1.inc"

