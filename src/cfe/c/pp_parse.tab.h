#ifndef BISON_CFE_C_PP_PARSE_TAB_H
# define BISON_CFE_C_PP_PARSE_TAB_H

#ifndef YYSTYPE
typedef union 
{
  unsigned long num;
  STRING        str;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	PREPR_LINE	257
# define	PREPR_PRAGMA	258
# define	PREPR_NUMBER	259
# define	PREPR_ERROR	260
# define	PREPR_STRING	261
# define	PREPR_IDENT	262


extern YYSTYPE yylval;

#endif /* not BISON_CFE_C_PP_PARSE_TAB_H */
