#ifndef PP_PARSE_H
#define PP_PARSE_H

#undef YYSTYPE
#define yystype ppstype

#define yyparse pp_parse
#define yylex pp_lex
#define yyerror pp_error
#define yylval pp_lval
#define yychar pp_char
#define yydebug pp_debug
#define yynerrs pp_nerrs

#endif // PP_PARSE_H
