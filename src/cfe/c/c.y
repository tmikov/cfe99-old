%{
#include "precomp.h"
#include "cfe_priv.h"

#include "c.tab.inc" // automatically generated definitions

#define YYINITDEPTH 50

#define YYDEBUG 1

#if YYDEBUG
#  define YYERROR_VERBOSE 1
#endif

// In order to prevent usage of last_line & last_column, we declare this to be
// empty
#define YYLLOC_DEFAULT(Current, Rhs, N) 

void yyerror ( char * err );
int yylex ( void );

static void yy_mark ( unsigned typ, const YYSTYPE * pval );

// Use this to fix bison type errors when we *really* don't have a result
// from a production.
#define NO_RESULT  ((void)0)

static TDeclSpecs * DSpecs_Combine_Prim ( TDeclSpecs ** a, TDeclSpecs ** pb );
static TQualNode  * DSpecs_GenQual_Prim ( TDeclSpecs ** pds );

#define DSPECS_COMBINE( a, b ) DSpecs_Combine_Prim( &(a), &(b) )
#define DSPECS_GENQUAL( a )    DSpecs_GenQual_Prim( &(a) )

%}

%union 
{
  int               token;
  STRING            str;
  TSymbol           * sym;
  TQualNode         * m_typ; // modifiable TQualNode
  const TQualNode   * typ;
  const TTypeNode   * tnode;
  TEnumDesc         * edesc;
  TAst              * ast;
  TDeclSpecs        * dspecs;
  TScope            * scope;
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
translation_unit { Dump_Init(); Dump_TScope( CfeRoots.GlobalScope ); }
| 
;







garbage_collect: 
{
      // Is it time to collect ?
      // In debug builds we always perform a full collection to make sure
      // there isn't a marking bug.
      //
#ifdef _DEBUG
      if (GcCollectStart( true )) 
#else
      if (GcCollectStart( false )) 
#endif
      {
        YY_MARK_STACK();   // mark the contents of the parser stack
        Cfe_GcMarkRoots(); // mark all C front-end data

        GcCollectEnd();
      }
    }
;









ensure_file_scope: 
{
      ASSERT( CfeRoots.CurScope != NULL );
      while ( CfeRoots.CurScope->disposition != SCOPE_FILE)
      {
        TRACE0( "***Warning: popping extra scopes at file level!!\n" );
        SymTab_PopScope();
      }
    }
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
{ TODO_BAD("combine the strings") }
| string_literal LSTRING_CONST 
{ TODO_BAD("combine the strings") }
;




























identifier: 
IDENT 
;

any_identifier: 
TYPENAME { $$ = $1->id.strName }
| IDENT 
;







primary_expression: 
identifier {$$ = Expr_ident( &@1, $1 )}
| constant 
| string_literal {TODO_BAD("");}
| '(' expression ')' { $$ = $2; }

| '(' compound_statement ')' {TODO_BAD("");}
;


postfix_expression: 
primary_expression 
| postfix_expression '[' expression ']' {TODO_BAD("");}
| postfix_expression '(' argument_expression_list_opt ')' {TODO_BAD("");}
| postfix_expression '.' any_identifier {TODO_BAD("");}
| postfix_expression PTR any_identifier {TODO_BAD("");}
| postfix_expression INC {TODO_BAD("");}
| postfix_expression DEC {TODO_BAD("");}
| '(' type_name ')' '{' initializer_list comma_opt '}' {TODO_BAD("");}
;

argument_expression_list: 
assignment_expression {TODO_BAD("");}
| argument_expression_list ',' assignment_expression {TODO_BAD("");}
;

argument_expression_list_opt: 

| argument_expression_list 
;


unary_expression: 
postfix_expression 
| INC unary_expression {TODO_BAD("");}
| DEC unary_expression {TODO_BAD("");}
| '*' cast_expression {$$ = Expr_deref( &@1, $2 )}
| '&' cast_expression {$$ = Expr_addr( &@1, $2 )}
| unary_operator cast_expression {TODO_BAD("");}
| SIZEOF unary_expression {TODO_BAD("");}
| SIZEOF '(' type_name ')' {TODO_BAD("");}


| ALIGNOF unary_expression {TODO_BAD("");}
| ALIGNOF '(' type_name ')' {TODO_BAD("");}
| LOG_AND any_identifier {TODO_BAD("");}
;

unary_operator: 
'+' | '-' | '~' | '!' 
;



cast_expression: 
unary_expression 
| '(' type_name ')' cast_expression {TODO_BAD("");}
;


multiplicative_expression: 
cast_expression 
| multiplicative_expression '*' cast_expression {$$ = Expr_mul( &@2, $1, $3 )}
| multiplicative_expression '/' cast_expression {TODO_BAD("");}
| multiplicative_expression '%' cast_expression {TODO_BAD("");}
;


additive_expression: 
multiplicative_expression 
| additive_expression '+' multiplicative_expression {TODO_BAD("");}
| additive_expression '-' multiplicative_expression {TODO_BAD("");}
;


shift_expression: 
additive_expression 
| shift_expression SHIFT additive_expression {TODO_BAD("");}
;


relational_expression: 
shift_expression 
| relational_expression ARITHCOMPARE shift_expression {TODO_BAD("");}
;


equality_expression: 
relational_expression 
| equality_expression EQCOMPARE relational_expression {TODO_BAD("");}
;


and_expression: 
equality_expression 
| and_expression '&' equality_expression {$$ = Expr_binAnd( &@2, $1, $3 )}
;


exclusive_or_expression: 
and_expression 
| exclusive_or_expression '^' and_expression {$$ = Expr_binXor( &@2, $1, $3 )}
;


inclusive_or_expression: 
exclusive_or_expression 
| inclusive_or_expression '|' exclusive_or_expression {$$ = Expr_binOr( &@2, $1, $3 )}
;


logical_and_expression: 
inclusive_or_expression 
| logical_and_expression LOG_AND inclusive_or_expression {TODO_BAD("");}
;


logical_or_expression: 
logical_and_expression 
| logical_or_expression LOG_OR logical_and_expression {TODO_BAD("");}
;


conditional_expression: 
logical_or_expression 

| logical_or_expression '?' expression_opt ':' conditional_expression {TODO_BAD("");}
;


assignment_expression: 
conditional_expression 
| unary_expression assignment_operator assignment_expression 
{$$ = Expr_assign( &@2, $1, $3 )}
;

assignment_expression_opt: 
{ $$ = NULL }
| assignment_expression 
;

assignment_operator: 
ASSIGN | '=' 
;


expression1: 
assignment_expression 
| expression1 ',' assignment_expression {TODO_BAD("");}
;

expression: 
expression1 
;

expression_opt: 
{$$ = NULL}
| expression 
;


constant_expression: 
conditional_expression 
{
        TODO1( "fold" );
      }
;


































declaration: 
GenType_declaration_specifiers 
init_declarator_list_opt ';' 
{NO_RESULT}
;

GenType_declaration_specifiers: 
declaration_specifiers { $$ = DSpecs_GenType( $1 ) }
;














declaration_specifiers: 
storage_class_specifier declaration_specifiers_opt { $$ = DSPECS_COMBINE( $1, $2 ) }
| function_specifier declaration_specifiers_opt { $$ = DSPECS_COMBINE( $1,      $2 ) }
| type_qualifier declaration_specifiers_opt { $$ = DSPECS_COMBINE( $1,          $2 ) }

| type_specifier declaration_specifiers1 { $$ = DSPECS_COMBINE( $1,          $2 ) }
| typedef_name declaration_specifiers2 { $$ = DSPECS_COMBINE( $1,            $2 ) }
;

declaration_specifiers_opt: 
{ $$ = NULL }
| declaration_specifiers 
;

declaration_specifiers1: 
{ $$ = NULL }
| storage_class_specifier declaration_specifiers1 { $$ = DSPECS_COMBINE( $1, $2 ) }
| function_specifier declaration_specifiers1 { $$ = DSPECS_COMBINE( $1,      $2 ) }
| type_qualifier declaration_specifiers1 { $$ = DSPECS_COMBINE( $1,          $2 ) }
| type_specifier declaration_specifiers1 { $$ = DSPECS_COMBINE( $1,          $2 ) }
;

declaration_specifiers2: 
{ $$ = NULL }
| storage_class_specifier declaration_specifiers2 { $$ = DSPECS_COMBINE( $1, $2 ) }
| function_specifier declaration_specifiers2 { $$ = DSPECS_COMBINE( $1,      $2 ) }
| type_qualifier declaration_specifiers2 { $$ = DSPECS_COMBINE( $1,          $2 ) }
;

init_declarator_list: 
init_declarator 
| init_declarator_list ',' {$<dspecs>$=$<dspecs>0} init_declarator 
;

init_declarator_list_opt: 
GetDSpecsTyp 
{
        Declare( $<dspecs>0, New_TSymbol( NULL, $1 ), NULL, false );
      }
| init_declarator_list 
;

GetDSpecsTyp: 
{
        $$ = $<dspecs>0->type;
      }
;

init_declarator: 
GetDSpecsTyp declarator 
{
        Declare( $<dspecs>0, $2, NULL, false );
      }
| GetDSpecsTyp declarator '=' initializer 
{ 
        TODO1( "handle the initializer" );
        Declare( $<dspecs>0, $2, NULL, true );
      }
;


storage_class_specifier: 
sclass { $$ = New_DSpecs( $1 ) }

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
{
        $$ = New_DSpecs_ExtDeclSpec( ExtDeclSpec_Make( $3 ) );
      }
;














type_specifier: 
typespec { $$ = New_DSpecs( $1 ) }
| extended_int_specifier 
| extended_float_specifier 
| struct_or_union_specifier 
| enum_specifier 


| TYPEOF '(' expression ')' {TODO}
| TYPEOF '(' type_name ')' {TODO}
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
{
        $$ = New_DSpecs_ExtInt( $3 );
      }
;


extended_float_specifier: 
C_EXT_FLOAT '(' NUMBER ')' 
{
        $$ = New_DSpecs_ExtFloat( $3 );
      }
;



struct_or_union_specifier: 
struct_or_union any_identifier declare_tag 
struct_scope 
{
        const TTypeNode * tag = $3;
        if (tag->u.memberScope == NULL)
          MODIFY_TNODE(tag)->u.memberScope = $4;
        $$ = New_DSpecs_Tag( tag );
      }
| struct_or_union null_str declare_tag 
struct_scope 
{
        const TTypeNode * tag = $3;
        if (tag->u.memberScope == NULL)
          MODIFY_TNODE(tag)->u.memberScope = $4;
        $$ = New_DSpecs_Tag( tag );
      }
| struct_or_union any_identifier 
{
        $$ = New_DSpecs_Tag( DeclareTag( $1, $2, false ) );
      }
;

null_str: { $$ = NULL };

declare_tag: 
{
        $$ = DeclareTag( $<token>-1, $<str>0, true )
      }
;

struct_scope: 
'{' 
{ SymTab_PushNewScope( SCOPE_STRUCT ); }
struct_declaration_list 
'}' 
{ 
          // Error recovery is handled in struct_declaration_list
          $$ = SymTab_PopScope(); 
        }
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
{NO_RESULT}
;

GenType_specifier_qualifier_list: 
specifier_qualifier_list { $$ = DSpecs_GenType( $1 ) }
;








specifier_qualifier_list: 
type_qualifier specifier_qualifier_list { $$ = DSPECS_COMBINE( $1, $2 ) }
| type_qualifier 
| type_specifier specifier_qualifier_list1 { $$ = DSPECS_COMBINE( $1, $2 ) }
| typedef_name specifier_qualifier_list2 { $$ = DSPECS_COMBINE( $1,   $2 ) }
;

specifier_qualifier_list1: 
{ $$ = NULL }
| type_qualifier specifier_qualifier_list1 { $$ = DSPECS_COMBINE( $1, $2 ) }
| type_specifier specifier_qualifier_list1 { $$ = DSPECS_COMBINE( $1, $2 ) }
;

specifier_qualifier_list2: 
{ $$ = NULL }
| type_qualifier specifier_qualifier_list2 { $$ = DSPECS_COMBINE( $1, $2 ) }
;

struct_declarator_list: 
struct_declarator 
| struct_declarator_list ',' {$<dspecs>$=$<dspecs>0} struct_declarator 
;

struct_declarator: 
GetDSpecsTyp declarator 
{
        Declare( $<dspecs>0, $2, NULL, false );
      }
| GetDSpecsTyp declarator ':' constant_expression 
{
        Declare( $<dspecs>0, $2, $4, false );
      }
| GetDSpecsTyp ':' constant_expression 
{
        Declare( $<dspecs>0, New_TSymbol( NULL, $1 ), $3, false );
      }
;


enum_specifier: 
ENUM any_identifier declare_enum 
'{' {$<edesc>$=$<edesc>3} enumerator_list comma_opt '}' 
{
        $$ = New_DSpecs_Tag( DefineEnum( $3 ) );
      }
| ENUM null_str declare_enum 
'{' {$<edesc>$=$<edesc>3} enumerator_list comma_opt '}' 
{
        $$ = New_DSpecs_Tag( DefineEnum( $3 ) );
      }
| ENUM any_identifier 
{
        $$ = New_DSpecs_Tag( DeclareTag( $1, $2, false ) );
      }
;

declare_enum: 
declare_tag 
{
        $$ = New_TEnumDesc( $1 );
      }
;

enumerator_list: 
enumerator 
| enumerator_list ',' {$<edesc>$=$<edesc>0} enumerator 
| error 
;

enumerator: 
enumeration_constant 
{
        DeclareEnumerator( $<edesc>0, $1, NULL );
      }
| enumeration_constant '=' constant_expression 
{
        DeclareEnumerator( $<edesc>0, $1, $3 );
      }
;

enumeration_constant: 
any_identifier 
;


type_qualifier: 
qual { $$ = New_DSpecs( $1 ) }






| ATTR '(' any_identifier ')' 
{
        $$ = New_DSpecs_ExtQualAttr( ExtQualAttr_Make( $3 ) );
      }
;

qual: 
C_CONST 
| C_RESTRICT 
| C_VOLATILE 
;



function_specifier: 
C_INLINE { $$ = New_DSpecs( $1 ) }
;



declarator: 
pointer_opt direct_declarator 
{
        $$ = $2;
      }
;

direct_declarator_tail: 

{
        $$ = $<typ>0;
      }
| '[' type_qualifier_list_opt assignment_expression_opt ']' {$<typ>$=$<typ>0} direct_declarator_tail 
{
        $$ = Type_MakeArray( 
                $6,
                $2,
                $3,
                false
             );
      }
| '[' C_STATIC type_qualifier_list_opt assignment_expression ']' {$<typ>$=$<typ>0} direct_declarator_tail 
{
        $$ = Type_MakeArray( 
                $7,
                $3,
                $4,
                true
             );
      }
| '[' GenQual_type_qualifier_list C_STATIC assignment_expression ']' {$<typ>$=$<typ>0} direct_declarator_tail 
{
        $$ = Type_MakeArray( 
                $7,
                $2,
                $4,
                true
             );
      }
| '[' type_qualifier_list_opt '*' ']' {$<typ>$=$<typ>0} direct_declarator_tail 
{
        $$ = Type_MakeArray( 
                $6,
                $2,
                Ast( ast_unspecifiedVariableArraySize, &@3 ),
                false
             );
      }
| param_scope {$<typ>$=$<typ>0} direct_declarator_tail 
{
        $$ = Type_MakeFunction( $3, $1 );
      }
| '(' ')' {$<typ>$=$<typ>0} direct_declarator_tail 
{
        $$ = Type_MakeFunction( $4, NULL );
      }
| identifier_list_scope {$<typ>$=$<typ>0} direct_declarator_tail 
{
        $$ = Type_MakeFunction( $3, $1 );
      }
;

param_scope: 
'(' 
{
        SymTab_PushNewScope( SCOPE_PARAMS );
      }
parameter_type_list ')' 
{
        $$ = SymTab_PopScope(); // parameter_type_list takes care of error recovery
      }
;

identifier_list_scope: 
'(' 
{
        SymTab_PushNewScope( SCOPE_IDENT_LIST );
      }
identifier_list ')' 
{
        TODO1( "error recovery!" );
        $$ = SymTab_PopScope();
      }
;






















left_bra_NullTyp: 
'(' 
{ $$ = NULL }
;

direct_declarator: 
any_identifier {$<typ>$=$<typ>0} direct_declarator_tail 
{ 
        $$ = New_TSymbol( $1, $3 );
      }
| left_bra_NullTyp declarator ')' {$<typ>$=$<typ>0} direct_declarator_tail 
{
        $$ = Symbol_ChainType( $2, $5 );
      }
;







declarator1: 
pointer_opt direct_declarator1 
{
        $$ = $2;
      }
;

direct_declarator1: 
any_identifier {$<typ>$=$<typ>0} direct_declarator_tail 
{ 
        $$ = New_TSymbol( $1, $3 );
      }
| left_bra_NullTyp declarator2 ')' {$<typ>$=$<typ>0} direct_declarator_tail 
{
        $$ = Symbol_ChainType( $2, $5 );
      }
;

declarator2: 
pointer_opt direct_declarator2 
{
        $$ = $2;
      }
;

direct_declarator2: 
identifier {$<typ>$=$<typ>0} direct_declarator_tail 
{ 
        $$ = New_TSymbol( $1, $3 );
      }
| left_bra_NullTyp declarator2 ')' {$<typ>$=$<typ>0} direct_declarator_tail 
{
        $$ = Symbol_ChainType( $2, $5 );
      }
;







qual_pointer: 
'*' type_qualifier_list_opt 
{ 
        $$ = Type_MakePointer( $<typ>0, $2 );
      }
;

pointer: 
qual_pointer 
| pointer qual_pointer { $$ = $2 }
;

pointer_opt: 
{ $$ = $<typ>0 }
| pointer 
;

type_qualifier_list: 
type_qualifier 
| type_qualifier_list type_qualifier { $$ = DSPECS_COMBINE( $1, $2 ) }
;

GenQual_type_qualifier_list: 
type_qualifier_list { $$ = DSPECS_GENQUAL( $1 ) }
;


type_qualifier_list_opt: 
{ $$ = NULL }
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
{
        Declare( $1, $3, NULL, false );
      }
| GenType_declaration_specifiers 
GetDSpecsTyp 
abstract_declarator 
{
        Declare( $1, $3, NULL, false );
      }
| GenType_declaration_specifiers 
GetDSpecsTyp 
{
        Declare( $1, New_TSymbol( NULL, $2 ), NULL, false );
      }
| ELLIPSIS 
{
        DeclareEllipsis();
      }
;









identifier_list: 
identifier { DeclareParamIdent( $1 ); }
| identifier_list ',' any_identifier { DeclareParamIdent( $3 ); }
;


type_name: 
GenType_specifier_qualifier_list 
GetDSpecsTyp 
abstract_declarator 
{TODO}
| GenType_specifier_qualifier_list 
{TODO}
;

abstract_declarator: 
pointer 
{
        $$ = New_TSymbol( NULL, $1 );
      }
| pointer_opt direct_abstract_declarator 
{
        $$ = $2;
      }
;

direct_abstract_declarator_tail: 
'[' assignment_expression_opt ']' {$<typ>$=$<typ>0} direct_abstract_declarator_tail_opt 
{
        $$ = Type_MakeArray( 
                $5,
                NULL,
                $2,
                false
             );
      }
| '[' '*' ']' {$<typ>$=$<typ>0} direct_abstract_declarator_tail_opt 
{
        $$ = Type_MakeArray( 
                $5,
                NULL,
                Ast( ast_unspecifiedVariableArraySize, &@2 ),
                false
             );
      }
| '(' ')' {$<typ>$=$<typ>0} direct_abstract_declarator_tail_opt 
{
        $$ = Type_MakeFunction( $4, NULL );
      }
| param_scope {$<typ>$=$<typ>0} direct_abstract_declarator_tail_opt 
{
        $$ = Type_MakeFunction( $3, $1 );
      }
;

direct_abstract_declarator_tail_opt: 

{
        $$ = $<typ>0;
      }
| direct_abstract_declarator_tail 
;

direct_abstract_declarator: 
left_bra_NullTyp abstract_declarator ')' 
{$<typ>$=$<typ>0} direct_abstract_declarator_tail_opt 
{
        $$ = Symbol_ChainType( $2, $5 );
      }
| direct_abstract_declarator_tail 
{
        $$ = New_TSymbol( NULL, $1 );
      }
;


typedef_name: 
TYPENAME 
{
        $$ = New_DSpecs_Typename( $1->type );
      }
;


initializer: 
assignment_expression {TODO;}
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
any_identifier ':' statement {TODO}
| CASE constant_expression ':' statement 
| DEFAULT ':' statement 


| CASE constant_expression ELLIPSIS constant_expression ':' statement 
;


compound_statement: 
'{' 
{
        SymTab_PushNewScope( SCOPE_BLOCK );
      }
declare_labels_list_opt block_item_list_opt '}' 
{
        SymTab_PopScope();
      }
;





function_compound_statement: 
'{' 
declare_labels_list_opt block_item_list_opt '}' 
{
        SymTab_PopScope();
      }
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
expression_opt ';' {Stmt_Expression( $1 )}
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


| ';' { CompilerStrictError( "Empty statement at file scope" ); }
;


function_definition: 
GenType_declaration_specifiers 
GetDSpecsTyp 
declarator 
{
        DeclareFunctionDefinition( 
          $1, 
          $2,
          $3,
          NULL
        );
      }
function_compound_statement 
| GenType_declaration_specifiers 
GetDSpecsTyp 
declarator 
declaration_list_scope 
{
        DeclareFunctionDefinition( 
          $1, 
          $2,
          $3,
          $4
        );
      }
function_compound_statement 
;

declaration_list_scope: 
{
        SymTab_PushNewScope( SCOPE_PARAMS );
      }
declaration_list 
{
        // error recovery is handled in declaration_list
        $$ = SymTab_PopScope();  
      }
;

declaration_list: 
declaration 
| declaration_list declaration 
| error 
;
%%

// DSpecs manipulation routines, with explicitly freeing the no-longer used 
// argument to conserve memory. 
// Note: we need to zero the parser stack location that holds the 
// freed value.

//--------------------------------------------------------------------------
// Name         DSpecs_Combine_Prim
//
// 
//--------------------------------------------------------------------------
static TDeclSpecs * DSpecs_Combine_Prim ( TDeclSpecs ** pa, TDeclSpecs ** pb )
{
  TDeclSpecs * res = DSpecs_Add( *pa, *pb );

  if (res != *pa)
    GC_DELETE( *pa );
  if (res != *pb)
    GC_DELETE( *pb );

  return res;
};

//--------------------------------------------------------------------------
// Name         DSpecs_GenQual_Prim 
//
// 
//--------------------------------------------------------------------------
static TQualNode * DSpecs_GenQual_Prim ( TDeclSpecs ** pds )
{
  TQualNode * res = DSpecs_GenQual( *pds );
  GC_DELETE( *pds );
  return res;
}

//--------------------------------------------------------------------------
// Name         yy_mark 
//
// Mark the contents of a semantic stack entry (YYSTYPE).
//
// _typ_ identifies the type of the entry, corresponing to a member
// of the declared %union. The possible values of _typ_ (YYST_xxx) are 
// generated automatically by PREBIS and are defined in c.tab.inc
//--------------------------------------------------------------------------
static void yy_mark ( unsigned typ, const YYSTYPE * pval )
{
  MAKE_SURE( YYST__LAST == 11 );

  switch (typ)
  {
  case YYST_AST:
    GC_MARK( pval->ast ); break;

  case YYST_STR:
    GC_MARK( pval->str ); break;

  case YYST_TOKEN:
    break;

  case YYST_SYM:
    GC_MARK( pval->sym ); break;

  case YYST_SCOPE:
    GC_MARK( pval->scope ); break;

  case YYST_TYP:
    GC_MARK( pval->typ ); break;

  case YYST_DSPECS:
    GC_MARK( pval->dspecs ); break;

  case YYST_TNODE:
    GC_MARK( pval->tnode ); break;

  case YYST_EDESC:
    GC_MARK( pval->edesc ); break;

  case YYST_M_TYP:
    GC_MARK( pval->m_typ ); break;

  default:
    ASSERT( 0 );
  }
}


#ifndef TEST_YYPARSE

void yyerror ( char * err )
{
  CompilerError( "at '%s' : %s", yytext, err );
};

#else

void yyerror ( char * err )
{
  printf( "\n\n **** error: %s\n\n", err );
};

int yylex ( void )
{
  int res;
  char buf[32];
  for(;;)
  {
    printf( "Enter code:" );
    gets( buf );

    if (isdigit(buf[0]))
    {
      if (sscanf( buf, "%d", &res ) != 1)
        printf( "Error! Try again.\n" );
      else
        return res;
    }
    else
    if (isalpha( buf[0] ))
    {
      int i;
      for ( i = 0; yytname[i] != NULL; ++i )
      {
        if (_stricmp( buf, yytname[i] ) == 0)
          return i + 255;
      }
      printf( "Undefined token! Try again.\n" );
    }
    else
      return buf[0];
  }
};

int main ( int argc, char ** argv )
{
#if YYDEBUG
  yydebug = argc == 2;
#endif 
  yyparse();

  return 0;
};
#endif // TEST_YYPARSE

