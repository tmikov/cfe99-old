/*
  This is my version of C51 grammar desgined for BISON.
  It is influenced by GNU CC's C Grammar at some places.

Notes:
  1) Some reserved words and symbols are grouped in yylex() to simplify
the grammar. Normally this could be done by more rules.
  2) I avoided the IF-ELSE shift/reduce conflict by setting ELSE higher
precedence that IF

*/

%{
#include "port\global.h"
#include "comp.h"
#include "declspec.h"
#include "parsetyp.h"
#include "decl.h"

#define YYINITDEPTH 50

#define YYDEBUG 1

#if YYDEBUG
#  define YYERROR_VERBOSE 1
#endif
#define YYSTDERR          stdout

extern char * yytext;
extern int yyleng;

void yyerror ( char * err );
int yylex ( void );

%}

%union
{
  enum EXPR_CODE    ecode;
  enum INFO_TOKEN   itok;
  TExprAst          * evalue;
  TSymbol           * sym;
  TStructDef        * structDef;
  unsigned          u;
  TParseType        * parstyp;
  TDeclData         * decl;
  TYPE              * typ;
  STRING            str;
}

/*
// type_start specifiers
//
%token  C_SHORT   C_SIGNED  C_DOUBLE  C_VOID
%token  C_INT     C_CHAR    C_LONG    C_UNSIGNED  C_FLOAT

// type_start qualifiers
//
%token  C_CONST   C_VOLATILE

// Storage classes
//
%token  C_EXTERN  C_AUTO    C_STATIC  C_REGISTER   C_TYPEDEF
*/

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
%token ELLIPSIS   // "..."

%token<str>  IDENT     // All identifiers that are not reserved words
%token<sym>  TYPENAME  // All identifiers that are typedefs in the current block

%token<evalue>  NUMBER          // A numeric constant
%token<evalue>  F_NUMBER        // A floating point constant
%token<evalue>  STRING_CONST    // A string constant

// Identifiers that group similar tokens to simplify the grammar
//

%token<ecode>  ASSIGN        // *= /= %= += -= &= |= ^= <<= >>=
%token<ecode>  EQCOMPARE     // == !=
%token<ecode>  ARITHCOMPARE  // <= >= < >
%token<ecode>  SHIFT         // << >>

// a storage class
%token<itok>  SCLASS   // EXTERN STATIC AUTO REGISTER TYPEDEF

// __declspec
%token  DECLSPEC 

// a non-ansi attribute like __cdecl, __pascal, __far, __near
%token  ATTR

// a type_start qualifier
%token<itok>  TYPEQUAL // CONST  VOLATILE

// reserved words that specify type_start specificators
%token<itok>  TYPESPEC // LONG   SHORT  SIGNED  UNSIGNED CHAR   INT    VOID

// Set precedences to avoid IF-ELSE shift/reduce conflict
%left   IF
%left   ELSE

// define precedence and associativity of operators
%left   ','
%right  ASSIGN  '='
%right  '?'     ':'
%left   LOG_OR
%left   LOG_AND
%left   '|'
%left   '^'
%left   '&' 
%left   EQCOMPARE
%left   ARITHCOMPARE
%left   SHIFT
%left   '+'     '-'
%left   '*'     '/'     '%'
%right  UNARY   INC     DEC     '!'     '~'
%left   HYPERUNARY
%left   PTR     '.'     '['     '('

%expect 1

/*
State 18 contains 1 shift/reduce conflict. (Two ways to parse type_start)
*/


/*
 Declare the types of some symbols.
 Because the grammar not ready yet, these caused lots of type errors,
 so I had to put empty {} statements at those plases.
*/
%type<evalue> e e_comma const_expr expr cast_expr unary_expr primary
%type<evalue> string

%type<ecode>  unop

%type<str>    identifier
%type<sym>    enum_ident

%type<u>      declspec_ident struct_union

%type<parstyp> empty_type_start
%type<parstyp> type_start typespec after_type_spec  after_typedef_spec
%type<parstyp> decl_modif declspec typedef_spec

%type<structDef> enum_def struct_def _struct_id struct_id_or_anon

/* this is to avoid error messages */
%type<typ>     function_definition global_declaration local_declaration

%type<u>       attribute attribute_ident
%type<typ>     type_quals

%type<decl>    decl decl_notype decl_with_type decl_or_parm decl_parm
%type<decl>    typecast

%%

/////////////////////////////////////////////////////////////////////////////

program
  : global_declaration_seq
  ;

global_declaration_seq
  : global_declaration_seq global_declaration 
        { 
          EndScopesTo( MainScope ); // in case any dangling scopes left after errors
        }
      
  | /* empty */
  ;

local_declaration_seq
  : local_declaration_seq local_declaration ';'
  | /* empty */
  ;

function_definition
  : type_start _decl local_declaration_seq compound_stmt
        { FreeParseType( $1 ) }
  | empty_type_start _decl_notype local_declaration_seq compound_stmt
        { FreeParseType( $1 ) }
  ;

// a declaration on file scope. In C implicit type is allowed
// (for example "x;" is valid), so the rule is a little bit more
// complicated.
//
global_declaration
  : type_start optional_decl_list ';'
        { FreeParseType( $1 ) }             
  | empty_type_start decl_notype_init ';'                // implicit type
        { FreeParseType( $1 ) }
  | empty_type_start decl_notype_init ',' {$<typ>$ = $<typ>1} decl_list ';'
        { FreeParseType( $1 ) }
  | function_definition
  | ';'  { Warning( "Empty statement in global scope" ) }
  | error ';'        { $$ = NULL }
  ;

/*
  The productions below need to have access to the type specifier
  returned by type_start and empty_type_start. It is accessed
  through $0, because they are always called in the "proper" context.
  I wonder if this is a good technique, but anyway it is much better
  than using global variables.
*/

// a declaration in local scope (in a function or struct). It must
// begin with an explicit type specificator; functions are
// not allowed.
//
local_declaration
  : type_start decl_list
        { FreeParseType( $1 ) }
  ;

optional_decl_list // $0 = type_start
  : /* empty */       { Warning( "Empty declaration list" ) }
  | decl_list
  ;

/*
  Below we need an addition rule {$$ = $0} because
  decl_maybe_init requites type_start to be in its $0.
*/
decl_list // $0 = type_start
  : decl_list ',' {$<typ>$ = $<typ>0} decl_maybe_init
  | decl_maybe_init
  ;

decl_notype_init  // $0 = type_start
  : _decl_notype             
  | _decl_notype '=' init    { Error("Initialization not supported") }
  ;

decl_maybe_init  // $0 = type_start
  : _decl                    
  | _decl '=' init           { Error("Initialization not supported") }
  | _decl ':' const_expr     { Error("Bitfield not supported") }
  ;

init
  : expr                                {}
  | '{' init_list optional_comma '}'    {}
  | '{' error '}'                       {}
  ;

init_list
  : init_list ',' init
  | init
  ;

optional_comma
  : ','
  | /*empty*/
  ;

/////////////////////////////////////////////////////////////////////////

/* Return an implicit, default type */
empty_type_start
  : /* empty */        { $$ = NULL }
  ;

// a type_start specification e.g. "extern unsigned long" 
type_start
  : typespec      after_type_spec     { $$ = CombineParseType( $1, $2 ) }
  | typedef_spec  after_typedef_spec  { $$ = CombineParseType( $1, $2 ) }
  | decl_modif    type_start          { $$ = CombineParseType( $1, $2 ) }
  | decl_modif
  ;

typespec
  : TYPESPEC  { $$ = NewTokenParseType( $1 ) }
  ;

// This comes after an explicit TYPESPEC (int,char,short,etc).
// Only more TYPESPEC-s and decl_modif-s are allowed
//
after_type_spec
  : after_type_spec typespec    { $$ = CombineParseType( $1, $2 ) }
  | after_type_spec decl_modif  { $$ = CombineParseType( $1, $2 ) }
  | /*empty*/                   { $$ = NULL } 
  ;

// This comes after an explicit predefined type (a typedef
// or structure/union/enum). Only decl_modif-s are allowed.
//
after_typedef_spec
  : after_typedef_spec decl_modif  { $$ = CombineParseType( $1, $2 ) }
  | /*empty*/                      { $$ = NULL }
  ;

// These are declaration modificators. They modify the storage
// class, const/volatile attributes, also the __declspec() extended
// attributes. They can be anywhere in a type specificator.
//
decl_modif
  : SCLASS    { $$ = NewTokenParseType( $1 ) }
  | declspec
  | TYPEQUAL  { $$ = NewTokenParseType( $1 ) }
  ;

declspec
  : DECLSPEC '(' declspec_ident ')'
        {
          // if the indentifier was recognized, allocate a parse type for it
          $$ = $3 ? NewDeclspecParseType( $3 ) : NULL;
        }
  ;

// returns 0 if the identifier wasn't recognized as a declspec
declspec_ident
  : identifier
        {
          if (($$ = FindDeclSpec( $1 )) == 0) // if not found
            Warning( "Ignoring unknown declspec(%s)", C_STR($1) );
        }
  ;

// This is a "predefined" type (typedef) definition or usage.
// For parsing purposes TYPENAME-s, structs and enums behave in
// the same way.
//
typedef_spec
  : TYPENAME     { $$ = NewTypenameParseType( $1 ) }
  | struct_def   { $$ = NewStructParseType( $1 ) }
  | enum_def     { $$ = NewStructParseType( $1 ) }
  ;

struct_union
  : STRUCT    { $$ = NODE_STRUCT }
  | UNION     { $$ = NODE_UNION }
  ;

_struct_id
  : identifier
        {
          if (($$ = FindActiveStruct( $1 )) == NULL)
            $$ = Struct_New( $1 );
        }
  ;

struct_id_or_anon
  : _struct_id
  | /* empty */
       {
         $$ = Struct_New( NULL )
       }
  ;

struct_def
  : struct_union _struct_id
        {
          $$ = DeclStruct( $1, $2, false );
        }

  | struct_union struct_id_or_anon '{'
        {
          $2 = DeclStruct( $1, $2, true );
          NewActiveScope( STRUCT_SCOPE );
        }
    local_declaration_seq '}'
        {
          $$ = CompleteStructDecl( $2, EndCurScope() );
        }

  | struct_union struct_id_or_anon '{' {} error '}'
       {
         // In case of error, end the scope that was created in the prev rule
         EndCurScope(); 
         $$ = NULL;
       }
  ;

enum_def
  : ENUM _struct_id                        // enum ident;
        {
          $$ = DeclStruct( NODE_ENUM, $2, false );
        }

  | ENUM struct_id_or_anon '{'     // enum ident { .. };
        {
          TYPE * qnode;

          // Allocate the TYPE that will be used by the members of
          // the enum
          //
          qnode = QualNode_New();
          qnode->isConst = true;

          qnode->tnode = TypeNode_New( NODE_ENUM );

          qnode->tnode->info.structDef = DeclStruct( NODE_ENUM, $2, true );

          $<typ>$ = qnode; // save the type
        }
    enum_inside '}'
        {
          $$ = $<typ>4->tnode->info.structDef; // get the structDef
          $$->isComplete = true;
        }

  | ENUM struct_id_or_anon '{' {} error '}'
        {
          $$ = NULL;
        } 
  ;

// $0 = TYPE(NODE_ENUM)
enum_inside
  : enum_list optional_comma
  | /*empty*/
  ;

// $0 = TYPE(NODE_ENUM)
enum_list
  : enum_list ',' { $<typ>$ = $<typ>0 } enum_elem
  | enum_elem
  ;

enum_ident
  : identifier
        {
          $$ = Symbol_New( $1 ); // allocate the ident
        }

// $0 = TYPE(NODE_ENUM)
enum_elem
  : enum_ident
        {
          DeclareEnumMember(
              $1,             // the ident
              $<typ>0,        // the type
              NULL            // init value
            );
        }

  | enum_ident '=' const_expr
        {
          DeclareEnumMember( $1, $<typ>0, $3 );
        }
  ;
           
/////////////////////////////////////////////////////////////////////////

// attribute like _far, _near, _cdecl, _pascal, or whatever the
// compiler/platform supports are specified this way.
// Note: these are part of the type declarator, not the type
// specifier (unlike DECLSPEC).
//
attribute
  : ATTR '(' attribute_ident ')'  { $$ = $3 }
  ;

// returns 0 if the identifier wasn't recognized as an attribute
attribute_ident
  : identifier
        {
          if (($$ = FindAttr( $1 )) == 0) // if not found
            Warning( "Ignoring unknown attr(%s)", C_STR($1) );
        }
  ;

// const/volatile qualifiers
//
type_quals
  : /* empty */         { $$ = QualNode_New() }
  | type_quals TYPEQUAL { CombineQualToken( $1, $2 ); $$ = $1; }
  ;

/////////////////////////////////////////////////////////////////////////

_decl         // $0 = type_start
  : decl                    { ParsedDecl( $<parstyp>0, $1 ) }
  ;

_decl_notype  // $0 = type_start
  : decl_notype             { ParsedDecl( $<parstyp>0, $1 ) }
  ;

_decl_or_parm  // $0 = type_start
  : decl_or_parm            { ParsedDecl( $<parstyp>0, $1 ) }
  ;

_typecast      // $0 = type_start
  : typecast                { ParsedDecl( $<parstyp>0, $1 ) }
  ;

// Any declarator (except in parameter list)
//
decl
  : decl_notype
  | decl_with_type
  ;

// declarator in a parameter list.
//
decl_or_parm
  : decl_notype
  | decl_parm
  ;

// This is a declarator allowed whether or not there is an explicit
// typespec before it. It can't redeclare a typedef-name
//
decl_notype
  : attribute decl_notype    %prec UNARY            // $$ = $2 attr($1)
        { $$ = DeclAddAttribute( $1, $2 ); }

  | '*' type_quals decl_notype                      // $$ = $3 qual($2) pointer to
        { $$ = DeclAddPointer( $2, $3 ); }

  | decl_notype '[' const_expr ']'      %prec '['   // $$ = $1 array [$3] of
        { $$ = DeclAddArray( $1, $3 ); }

  | decl_notype '[' ']'                 %prec '['   // $$ = $1 array [] of
        { $$ = DeclAddArray( $1, NULL ); }

  | decl_notype '('                                 // $$ = $1 is function of
        { 
          NewActiveScope( FUNC_PARAM_SCOPE );
          $$ = DeclAddFunction( $1 ); 
        }
    any_params ')'      %prec '('
        {
          $$ = $<decl>3;
          EndCurScope(); 
        }

/*
  // The previous rule requires special error recovery because it has
  // created a new scope which must in all cases be closed
  //
  | decl_notype '(' {} error ')' %prec '('
        {
          // in some cases the scope may be still haven't been created.
          // For example in the case "int func ( - );"
          // For that purpose I have added the {} after the '('
          //

          // In case of error, end the scope that was created in the prev rule
          EndCurScope();

          $$ = $1;
          $$->etype->isImplicit = true; // Make the function implicit
        }
*/

  | '(' decl_notype ')'                             // $$ = $2
        { $$ = $2 }

  | IDENT                                           // $$ = $1 is
        { $$ = DeclAddIdent( $1 ); }
  ;

// This is a declarator allowed only after an explicit
// typespec. So it can redeclare a typedef-name
//
decl_with_type
  : attribute decl_with_type          %prec UNARY // $$ = $2 attr($1)
        { $$ = DeclAddAttribute( $1, $2 ); }

  | '*' type_quals decl_with_type                 // $$ = $3 qual($2) pointer to
        { $$ = DeclAddPointer( $2, $3 ); }

  | decl_with_type '[' const_expr ']' %prec '['   // $$ = $1 array [$3] of
        { $$ = DeclAddArray( $1, $3 ); }

  | decl_with_type '[' ']'            %prec '['   // $$ = $1 array [] of
        { $$ = DeclAddArray( $1, NULL ); }

  | decl_with_type '('
        { 
          NewActiveScope( FUNC_PARAM_SCOPE );
          $$ = DeclAddFunction( $1 ); 
        }
    any_params ')'      %prec '('
        {
          $$ = $<decl>3;
          EndCurScope(); 
        }

  | '(' decl_with_type ')'                        // $$ = $2
        { $$ = $2 }

  | TYPENAME                                      // $$ = $1 is
        { $$ = DeclAddIdent( $1->id.strName ); }
  ;

// This is a declarator that can appear in parameter list in addition
// to decl_notype. It is like decl_with_type, but it does not allow
// a typedef name in paretheses as an identifier.
//
decl_parm
  : attribute decl_parm             %prec UNARY // $$ = $2 attr($1)
        { $$ = DeclAddAttribute( $1, $2 ); }

  | '*' type_quals decl_parm                    // $$ = $3 qual($2) pointer to
        { $$ = DeclAddPointer( $2, $3 ); }

  | decl_parm '[' const_expr ']'    %prec '['   // $$ = $1 array [$3] of
        { $$ = DeclAddArray( $1, $3 ); }

  | decl_parm '[' ']'               %prec '['   // $$ = $1 array [] of
        { $$ = DeclAddArray( $1, NULL ); }

  | decl_parm '(' 
        { 
          NewActiveScope( FUNC_PARAM_SCOPE );
          $$ = DeclAddFunction( $1 ); 
        }
    any_params ')'      %prec '('
        {
          $$ = $<decl>3;
          EndCurScope(); 
        }

  | TYPENAME                                    // $$ = $1 is
        { $$ = DeclAddIdent( $1->id.strName ); }
  ;

////////////// typecast: same as decl_xxx but no ident at the end
// (That's why some of the rules look weird)

typecast
  : attribute typecast              %prec UNARY  // $$ = $2 attr($1)
        { $$ = DeclAddAttribute( $1, $2 ); }

  | '*' type_quals typecast                 // $$ = $3 qual($2) pointer to
        { $$ = DeclAddPointer( $2, $3 ); }

  | '*' type_quals
        { $$ = DeclAddPointer( $2, NULL ); }

  | typecast '[' const_expr ']'      %prec '['  // $$ = $1 array [$3] of
        { $$ = DeclAddArray( $1, $3 ); }

  | typecast '[' ']'                 %prec '['  // $$ = $1 array [] of
        { $$ = DeclAddArray( $1, NULL ); }

  | typecast '('  
        { 
          NewActiveScope( FUNC_PARAM_SCOPE );
          $$ = DeclAddFunction( $1 ); 
        }
    optional_decl_params ')'      %prec '('
        {
          $$ = $<decl>3;
          EndCurScope(); 
        }

  | '[' const_expr ']'               %prec '['
        { $$ = DeclAddArray( NULL, $2 ); }

  | '[' ']'                          %prec '['
        { $$ = DeclAddArray( NULL, NULL ); }

  | '(' 
        { 
          NewActiveScope( FUNC_PARAM_SCOPE );
          $$ = DeclAddFunction( NULL ); 
        }
    optional_decl_params ')'      %prec '('
        {
          $$ = $<decl>2;
          EndCurScope(); 
        }

  | '(' typecast ')'
        { $$ = $2 }
  ;


// a list of formal parameters of a function. Old or new style.
//
any_params
  : decl_params
        {
          // Mark that this is a new-style function header
          CurScope->info.funcNode->tnode->isImplicit = false;
        }
  | old_style_params    {yyerror("Unsupported")}
  ;

// A list of new-style parameters, or empty. This is allowed only
// in typecasts. The "empty" is in fact an old-style function signature.
//
optional_decl_params
  : decl_params
  | /*empty*/    // old-style
  ;

// old style parameters
//
old_style_params
  : old_param_list
  | old_param_list optional_comma ELLIPSIS
  | /*empty*/      // this way "int func()" is in fact old-style
  ;

// a list of old-style parameter declarations.
// Note that the first parameter must be non-typedef, but
// the following can redeclare typedef-s.
//
old_param_list
  : old_param_list ',' identifier
  | IDENT                             {}
  ;

// new style parameters
//
decl_params
  : decl_param_list
  | decl_param_list optional_comma ELLIPSIS
        {
          Scope_AddSymbol( CurScope, EllipsisSym );
        }

/*
 This is a special production which allows us to handle
 the "(void)" parameter list. void is allowed only if it is the
 first and the only parameter (and it is anonymous of course).

 That's why we must process the case when we have *exactly one*
 anonymous type_start parameter, separately.
*/
  | type_start
        {
          if ($1->basic != BT_VOID) // check for the "(void)" case
          {
            // Create a declaration with an anonymous ident
            ParsedDecl( $1, NewDeclData() );  
          }

          FreeParseType( $1 );
        }

  | decl_type_start_param ',' decl_param_list1
  ;

// A list of new-style parameter declarations. (See the note for type_start
// above). It can't start with a type_start - only anonymous parameter
// (which has already been handled in the parent rule (decl_params)
//
decl_param_list
  : decl_param_list ',' decl_param
  | decl_param_list ',' decl_type_start_param
  | decl_param
  ;

decl_param_list1
  : decl_param_list ',' decl_param
  | decl_param_list ',' decl_type_start_param
  | decl_param
  | decl_type_start_param
  ;

// Parses a single anonymous type_start-only parameter. This is just
// a macro.
decl_type_start_param
  : type_start
        {
          // Create a declaration with an anonymous ident
          ParsedDecl( $1, NewDeclData() );  
          FreeParseType( $1 );
        }
  ;

// a single new-style definition in the parameter list.
// It requires an explicit type start
//
decl_param
  : type_start _decl_or_parm   { FreeParseType( $1 ) }
  | type_start _typecast       { FreeParseType( $1 ) }
  ;

////////////////////////////////////////////////////////////////////////
// statements

compound_stmt
  : '{' /*local_declaration_seq*/ stmt_list '}'
  | '{' error '}'                
  ;

stmt_list
  : /* empty */          
  | stmt_list stmt       
  ;

stmt
  : IDENT ':' stmt               {}
  | CASE const_expr ':' stmt
  | DEFAULT ':' stmt
  | compound_stmt
  | if_stmt
  | while_stmt
  | do_stmt
  | for_stmt
  | switch_stmt
  | break_stmt
  | continue_stmt
  | goto_stmt
  | return_stmt

  | local_declaration ';' {} // allow for C9X (aka C++) declarations.

  | e ';'        {}
  | ';' 
  ;

// A C++ style expression inside of an operator. It can declare
// variables
//
cpp_expr
  : e                    {}
  | local_declaration    {}
  ;


if1
  : IF '(' cpp_expr ')' stmt
  ;

if_stmt
  : if1            %prec IF
  | if1 ELSE stmt  %prec ELSE
  ;

while_stmt
  : WHILE '(' cpp_expr ')' stmt
  ;

do_stmt
  : DO stmt WHILE '(' e ')' ';'
  ;

for_stmt
  : FOR '(' cpp_expr_empty ';' cpp_expr_empty ';' expr_empty ')' stmt
  ;

cpp_expr_empty
  : /* empty */   
  | cpp_expr             
  ;

expr_empty
  : /* empty */   {}
  | e             {}
  ;

switch_stmt
  : SWITCH '(' cpp_expr ')' stmt
  ;

break_stmt
  : BREAK ';'
  ;

continue_stmt
  : CONTINUE ';' 
  ;

goto_stmt
  : GOTO identifier ';'
  ;

return_stmt
  : RETURN ';'
  | RETURN e ';'
  ;

e
  : e_comma     
  ;

// an expression with comma (note: comma can't be in a constant expression)
e_comma
  : e_comma ',' expr
  | expr
  ;

// a semantic alias for an expression whose value must be known at compile time
// The type_start must be integral
const_expr
  : expr
  ;

// an expression without comma
expr
  : cast_expr          
  | expr ASSIGN expr        
  | expr '=' expr           

  | expr '?' expr ':' expr  

  | expr LOG_OR  expr   
  | expr LOG_AND expr   

  | expr '|'     expr       
  | expr '^'     expr       
  | expr '&'     expr       

  | expr EQCOMPARE  expr    
  | expr ARITHCOMPARE expr  

  | expr SHIFT expr       

  | expr '+'     expr       
  | expr '-'     expr       

  | expr '*'     expr       
  | expr '/'     expr       
  | expr '%'     expr       
  ;

cast_expr
  : unary_expr   
  | '(' typename ')' cast_expr  %prec UNARY    {}
  ;

unary_expr
  : primary   
  | '*' cast_expr                %prec UNARY       {} 
  | unop cast_expr               %prec UNARY       {}   
  | SIZEOF unary_expr            %prec UNARY       {}
  | SIZEOF '(' typename ')'      %prec HYPERUNARY  {}
  ;

unop
  : '&'    {$$ = e_ref}   
  | '-'    {$$ = e_uminus}   
  | '+'    {$$ = e_uplus}   
  | INC    {$$ = e_pre_inc}
  | DEC    {$$ = e_pre_dec}
  | '~'    {$$ = e_bin_not}   
  | '!'    {$$ = e_log_not}   
  ;

/*
  Here I use "%prec UNARY" to avoid shift/reduce conflict. That is: make
  the prec of the rule lower than of '('. In that case BISON shifts.
*/
primary
  : NUMBER
  | F_NUMBER
  | IDENT           %prec UNARY    {}
  | string                      
  | '(' e_comma ')'                {}
  | '(' error ')'                  {}

  | IDENT '(' arg_list ')'    %prec UNARY  {}

  | primary '(' arg_list ')'    
  | primary '[' e_comma ']'
  | primary '.' identifier   // $$ is lvalue if $1 is lvalue

  | primary PTR identifier

  | primary INC 
  | primary DEC 
  ;

identifier
  : IDENT
  | TYPENAME  { $$ = $1->id.strName }
  ;

// combine strings
string
  : STRING_CONST
  | string STRING_CONST
  ;

typename
  : type_start _typecast    { FreeParseType( $1 ) }
  | type_start             { FreeParseType( $1 ) }
  ;

// list of arguments to a functions
arg_list
  : /* empty */         
  | arguments
  ;

arguments
  : arguments ',' argument  {/* $$ = EListAppend( $1, $3 ) */} 
  | argument
  ;

argument
  : expr  {/* $$ = AllocEList( $1 ) */}
  ;

%%

//--------------------------------------------------------------------------
// Name         ScannerClassifyIdent
//
// This function is called from the scanner to classify the scanned
// identifiers. It determines whether those are typename-s and returns
// TYPENAME accordingly.
//--------------------------------------------------------------------------
int ScannerClassifyIdent ( void )
{
  STRING str;
  TSymbol * sym;

  // Allocate the string
  str = NewStringLen( yytext, yyleng );

  // Check if this is a typename
  //
  if ((sym = FindActiveSymbol( str )) != NULL &&
      sym->storage->storClass == SC_TYPEDEF)
  {
    yylval.sym = sym;
    return TYPENAME;
  }
  else
  {
    yylval.str = str;
    return IDENT;
  }
};

void yyerror ( char * err )
{
  Error( err );
};
/*
int main ( int argc, char ** argv )
{
#if YYDEBUG
  yydebug = argc == 2;
#endif 
  yyparse();

  return 0;
};
*/

