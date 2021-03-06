typedef union
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
} YYSTYPE;
#define	SWITCH	258
#define	STRUCT	259
#define	CONTINUE	260
#define	CASE	261
#define	SIZEOF	262
#define	DEFAULT	263
#define	ELSE	264
#define	WHILE	265
#define	DO	266
#define	ENUM	267
#define	GOTO	268
#define	BREAK	269
#define	IF	270
#define	FOR	271
#define	UNION	272
#define	RETURN	273
#define	ELLIPSIS	274
#define	IDENT	275
#define	TYPENAME	276
#define	NUMBER	277
#define	F_NUMBER	278
#define	STRING_CONST	279
#define	ASSIGN	280
#define	EQCOMPARE	281
#define	ARITHCOMPARE	282
#define	SHIFT	283
#define	SCLASS	284
#define	DECLSPEC	285
#define	ATTR	286
#define	TYPEQUAL	287
#define	TYPESPEC	288
#define	LOG_OR	289
#define	LOG_AND	290
#define	UNARY	291
#define	INC	292
#define	DEC	293
#define	HYPERUNARY	294
#define	PTR	295


extern YYSTYPE yylval;
