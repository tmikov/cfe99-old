%union 
{
  STRING str;
  CORD   cord;
  ScanNumber_t scanNum;
  ScanFNumber_t scanFNum;
}

// type specifiers
//
// TODO: good candidates for merging into one token
//
%token C_SIGNED C_UNSIGNED
%token C_SHORT C_LONG
%token C_INT C_CHAR
%token C_FLOAT C_DOUBLE
%token C_VOID
%token C__BOOL C__COMPLEX C__IMAGINARY

// type qualifiers
//
// TODO: good candidates for merging into one token
//
%token C_CONST C_VOLATILE C_RESTRICT

// Storage classes
//
%token C_EXTERN C_AUTO C_STATIC C_REGISTER C_TYPEDEF

%token C_INLINE

// C reserved words
//
%token SWITCH
%token STRUCT
%token CONTINUE
%token CASE
%token SIZEOF
%token DEFAULT
%token ELSE
%token WHILE
%token DO
%token ENUM
%token GOTO
%token BREAK
%token IF
%token FOR
%token UNION
%token RETURN

%token  IDENT     // All identifiers that are not reserved words
%token  TYPENAME  // All identifiers that are typedefs in the current block

%token  NUMBER          // A numeric constant
%token  F_NUMBER        // A floating point constant
%token  CHAR_CONST
%token  LCHAR_CONST
%token  STRING_CONST     // A string constant
%token  LSTRING_CONST    

// Punctuators longer than a character

%token LOG_OR LOG_AND INC DEC PTR ELLIPSIS 

// Identifiers that group similar tokens to simplify the grammar

%token  ASSIGN        // *= /= %= += -= &= |= ^= <<= >>=
%token  EQCOMPARE     // == !=
%token  ARITHCOMPARE  // <= >= < >
%token  SHIFT         // << >>

// __declspec
%token  DECLSPEC 

// a non-ansi attribute like __cdecl, __pascal, __far, __near
%token  ATTR


// Set precedences to avoid IF-ELSE shift/reduce conflict
%left   IF
%left   ELSE

%%

start 
  : translation_unit 
  ;


/////////////////////////////////////////////////////////////////
//
// Little helpers added by me
//


//  An optional ',' at the end of initializer_list/enumerator_list
comma_opt
  : //empty
  | ','
  ;


/////////////////////////////////////////////////////////////////
//
//  A.1 Lexical elements
//


constant
  : NUMBER
  | F_NUMBER
/*
  | enumeration_constant
*/
  | character_constant
  ;

character_constant
  : CHAR_CONST
  | LCHAR_CONST
  ;

string_literal
  : STRING_CONST
  | LSTRING_CONST
  | string_literal STRING_CONST
  | string_literal LSTRING_CONST
  ;

/*
  6.2.3  Name spaces of identifiers

  [#1] If more than one declaration of a particular identifier
  is visible at any point in a translation unit, the syntactic
  context disambiguates uses that refer to different entities.
  Thus,  there are separate name spaces for various categories
  of identifiers, as follows:

    -- label names (disambiguated by the syntax of  the  label
       declaration and use);

    -- the   tags  of  structures,  unions,  and  enumerations
       (disambiguated  by  following  any22)  of  the keywords
       struct, union, or enum);

    -- the members of structures or unions; each structure  or
       union  has  a  separate  name  space  for  its  members
       (disambiguated by the type of the  expression  used  to
       access the member via the . or -> operator);

    -- all  other  identifiers,  called  ordinary  identifiers
       (declared in ordinary  declarators  or  as  enumeration
       constants).
*/

// regular identifiers 
identifier
  : IDENT  
  ;

any_identifier
  : TYPENAME
  | IDENT
  ;

/////////////////////////////////////////////////////////////////
//
//  A.2.1 Expressions
//

// 6.5.1
primary_expression
  : identifier
  | constant
  | string_literal
  | '(' expression ')'
  ;

// 6.5.2
postfix_expression
  : primary_expression
  | postfix_expression '[' expression ']'
  | postfix_expression '(' argument_expression_list_opt ')'
  | postfix_expression '.' any_identifier
  | postfix_expression PTR any_identifier
  | postfix_expression INC
  | postfix_expression DEC
  | '(' type_name ')' '{' initializer_list comma_opt '}'
  ;

argument_expression_list
  : assignment_expression
  | argument_expression_list ',' assignment_expression
  ;

argument_expression_list_opt
  : //empty
  | argument_expression_list
  ;

// 6.5.3
unary_expression
  : postfix_expression
  | INC unary_expression
  | DEC unary_expression
  | unary_operator cast_expression
  | SIZEOF unary_expression
  | SIZEOF '(' type_name ')'
  ;

unary_operator
  : '&' | '*' | '+' | '-' | '~' | '!'
  ;


// 6.5.4
cast_expression
  : unary_expression
  | '(' type_name ')' cast_expression
  ;

// 6.5.5
multiplicative_expression
  : cast_expression
  | multiplicative_expression '*' cast_expression
  | multiplicative_expression '/' cast_expression
  | multiplicative_expression '%' cast_expression
  ;

// 6.5.6
additive_expression
  : multiplicative_expression
  | additive_expression '+' multiplicative_expression
  | additive_expression '-' multiplicative_expression
  ;

// 6.5.7
shift_expression
  : additive_expression
  | shift_expression SHIFT additive_expression
  ;

// 6.5.8
relational_expression
  : shift_expression 
  | relational_expression ARITHCOMPARE shift_expression
  ;

// 6.5.9
equality_expression
  : relational_expression
  | equality_expression EQCOMPARE relational_expression
  ;

// 6.5.10
and_expression
  : equality_expression
  | and_expression '&' equality_expression
  ;

// 6.5.11
exclusive_or_expression
  : and_expression 
  | exclusive_or_expression '^' and_expression
  ;

// 6.5.12
inclusive_or_expression
  : exclusive_or_expression
  | inclusive_or_expression '|' exclusive_or_expression
  ;

// 6.5.13
logical_and_expression
  : inclusive_or_expression
  | logical_and_expression LOG_AND inclusive_or_expression
  ;

// 6.5.14
logical_or_expression
  : logical_and_expression 
  | logical_or_expression LOG_OR logical_and_expression
  ;

// 6.5.15
conditional_expression
  : logical_or_expression
  | logical_or_expression '?' expression ':' conditional_expression
  ;

// 6.5.16
assignment_expression
  : conditional_expression
  | unary_expression assignment_operator assignment_expression
  ;

assignment_expression_opt
  : //empty
  | assignment_expression
  ;

assignment_operator
  : ASSIGN | '='
  ;

// 6.5.17
expression
  : assignment_expression
  | expression ',' assignment_expression
  ;

expression_opt
  : //empty
  | expression
  ;

// 6.6
constant_expression
  : conditional_expression
  ;


/////////////////////////////////////////////////////////////////
//
//  A.2.2 Declarations
//


// 6.7
declaration
  : declaration_specifiers init_declarator_list_opt ';'
  ;

declaration_specifiers
  : storage_class_specifier declaration_specifiers_opt
  | type_specifier declaration_specifiers_opt
  | type_qualifier declaration_specifiers_opt
  | function_specifier declaration_specifiers_opt
  ;

declaration_specifiers_opt
  : //empty
  | declaration_specifiers
  ;

init_declarator_list
  : init_declarator
  | init_declarator_list ',' init_declarator
  ;

init_declarator_list_opt
  : //empty
  | init_declarator_list
  ;

init_declarator
  : declarator
  | declarator '=' initializer
  ;

// 6.7.1
storage_class_specifier
  : C_TYPEDEF
  | C_EXTERN
  | C_STATIC
  | C_AUTO
  | C_REGISTER
  ;

// 6.7.2

/*
  This is declaration is too loose.
  A typedef_name can't be combined with any other specifier in the same
  specifier list. The same applies for struct/union and enum.
  So, we can put a few additional rules to prevent that.
*/
type_specifier
  : C_VOID
  | C_CHAR
  | C_SHORT
  | C_INT
  | C_LONG
  | C_FLOAT
  | C_DOUBLE
  | C_SIGNED
  | C_UNSIGNED
  | C__BOOL
  | C__COMPLEX 
  | C__IMAGINARY
  | struct_or_union_specifier
  | enum_specifier
  | typedef_name
  ; 

// 6.7.2.1
struct_or_union_specifier
  : struct_or_union any_identifier '{' struct_declaration_list '}'
  | struct_or_union                '{' struct_declaration_list '}'
  | struct_or_union any_identifier
  ;

struct_or_union
  : STRUCT 
  | UNION
  ;

struct_declaration_list
  : struct_declaration
  | struct_declaration_list struct_declaration
  ;

struct_declaration
  : specifier_qualifier_list struct_declarator_list ';'
  ;

specifier_qualifier_list
  : type_specifier specifier_qualifier_list_opt
  | type_qualifier specifier_qualifier_list_opt
  ;

specifier_qualifier_list_opt
  : //empty
  | specifier_qualifier_list
  ;

struct_declarator_list
  : struct_declarator
  | struct_declarator_list ',' struct_declarator
  ;

struct_declarator
  : declarator
  | declarator_opt ':' constant_expression
  ;

// 6.7.2.2
enum_specifier
  : ENUM any_identifier '{' enumerator_list comma_opt '}'
  | ENUM                '{' enumerator_list comma_opt '}'
  | ENUM any_identifier
  ;

enumerator_list
  : enumerator
  | enumerator_list ',' enumerator
  ;

enumerator
  : enumeration_constant
  | enumeration_constant '=' constant_expression
  ;

enumeration_constant
  : any_identifier
  ;

// 6.7.3
type_qualifier
  : C_CONST
  | C_RESTRICT
  | C_VOLATILE
  ;

// 6.7.4
function_specifier
  : C_INLINE
  ;

// 6.7.5
declarator
  : pointer direct_declarator
  | direct_declarator
  ;

declarator_opt
  : //empty
  | declarator
  ;

direct_declarator
  : identifier
  | '(' declarator ')'
  | direct_declarator '[' type_qualifier_list_opt assignment_expression_opt ']'
  | direct_declarator '[' C_STATIC type_qualifier_list_opt assignment_expression ']'
  | direct_declarator '[' type_qualifier_list C_STATIC assignment_expression ']'
  | direct_declarator '[' type_qualifier_list_opt '*' ']'
  | direct_declarator '(' parameter_type_list ')'
  | direct_declarator '(' identifier_list_opt ')'
  ;

pointer
  : '*' type_qualifier_list_opt
  | '*' type_qualifier_list_opt pointer
  ;

type_qualifier_list
  : type_qualifier
  | type_qualifier_list type_qualifier
  ;

type_qualifier_list_opt
  : //empty
  | type_qualifier_list
  ;

parameter_type_list
  : parameter_list
  | parameter_list ',' ELLIPSIS
  ;

parameter_type_list_opt
  : //empty
  | parameter_type_list
  ;

parameter_list
  : parameter_declaration
  | parameter_list ',' parameter_declaration
  ;

parameter_declaration
  : declaration_specifiers declarator
  | declaration_specifiers abstract_declarator
  | declaration_specifiers 
  ;

/*
  In a identifier list (old-style parameter list)
  all but the first identifier can redefine a typedef. 
  (If teh first one was a typedef then we would assume that this
  is a new style declaration).

  Both VC and BCC behave in this way. Theoretically there might be a way
  to fix this, perhaps with LR(2).
*/
identifier_list
  : identifier
  | identifier_list ',' any_identifier
  ;

identifier_list_opt
  : //empty
  | identifier_list
  ;

// 6.7.6
type_name
  : specifier_qualifier_list abstract_declarator
  | specifier_qualifier_list 
  ;

abstract_declarator
  : pointer
  | pointer direct_abstract_declarator
  | direct_abstract_declarator
  ;

direct_abstract_declarator
  : '(' abstract_declarator ')'
  | direct_abstract_declarator '[' assignment_expression_opt ']'
  |                            '[' assignment_expression_opt ']'
  | direct_abstract_declarator '[' '*' ']'
  |                            '[' '*' ']'
  | direct_abstract_declarator '(' parameter_type_list_opt ')'
  |                            '(' parameter_type_list_opt ')'
  ;

// 6.7.7
typedef_name
  : TYPENAME
  ;

// 6.7.8
initializer
  : assignment_expression
  | '{' initializer_list comma_opt '}'
  ;

initializer_list
  : designation_opt initializer
  | initializer_list ',' designation_opt initializer
  ;

designation
  : designator_list '='
  ;

designation_opt
  : //empty
  | designation
  ;

designator_list
  : designator
  | designator_list designator
  ;

designator
  : '[' constant_expression ']'
  | '.' any_identifier
  ;

/////////////////////////////////////////////////////////////////
//
//  A.2.3 Statements
//


// 6.8
statement
  : labeled_statement
  | compound_statement
  | expression_statement
  | selection_statement
  | iteration_statement
  | jump_statement
  ;

// 6.8.1
labeled_statement
  : any_identifier ':' statement
  | CASE constant_expression ':' statement
  | DEFAULT ':' statement
  ;

// 6.8.2
compound_statement
  : '{' block_item_list_opt '}'
  ;

block_item_list
  : block_item
  | block_item_list block_item
  ;

block_item_list_opt
  : //empty
  | block_item_list
  ;

block_item
  : declaration
  | statement
  ;

// 6.8.3
expression_statement
  : expression_opt ';'
  ;

// 6.8.4
selection_statement
  : IF '(' expression ')' statement                  %prec IF
  | IF '(' expression ')' statement ELSE statement   %prec ELSE
  | SWITCH '(' expression ')' statement
  ;

// 6.8.5
iteration_statement
  : WHILE '(' expression ')' statement
  | DO statement WHILE '(' expression ')' ';'
  | FOR '(' expression_opt ';' expression_opt ';' expression_opt ')' statement
  | FOR '(' declaration expression_opt ';' expression_opt ')' statement
  ;

// 6.8.6
jump_statement
  : GOTO any_identifier ';'
  | CONTINUE ';'
  | BREAK ';'
  | RETURN expression_opt ';'
  ;


/////////////////////////////////////////////////////////////////
//
//  A.2.4 External definitions
//

// 6.9
translation_unit
  : external_declaration
  | translation_unit external_declaration
  ;

external_declaration
  : function_definition
  | declaration
  ;

// 6.9.1
function_definition
  : declaration_specifiers declarator declaration_list_opt compound_statement
  ;

declaration_list
  : declaration
  | declaration_list declaration
  ;

declaration_list_opt
  : //empty
  | declaration_list
  ;

%%

