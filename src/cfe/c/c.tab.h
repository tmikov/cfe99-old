#ifndef BISON_CFE_C_C_TAB_H
# define BISON_CFE_C_C_TAB_H

#ifndef YYSTYPE
typedef union 
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
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;

  int last_line;
  int last_column;
} yyltype;

# define YYLTYPE yyltype
# define YYLTYPE_IS_TRIVIAL 1
#endif

# define	C_VOID	257
# define	C_CHAR	258
# define	C_SHORT	259
# define	C_INT	260
# define	C_LONG	261
# define	C_SIGNED	262
# define	C_UNSIGNED	263
# define	C_FLOAT	264
# define	C_DOUBLE	265
# define	C__BOOL	266
# define	C__COMPLEX	267
# define	C__IMAGINARY	268
# define	C_EXT_INT	269
# define	C_EXT_FLOAT	270
# define	C_CONST	271
# define	C_VOLATILE	272
# define	C_RESTRICT	273
# define	C_EXTERN	274
# define	C_AUTO	275
# define	C_STATIC	276
# define	C_REGISTER	277
# define	C_TYPEDEF	278
# define	C_INLINE	279
# define	STRUCT	280
# define	UNION	281
# define	ENUM	282
# define	SWITCH	283
# define	CONTINUE	284
# define	CASE	285
# define	SIZEOF	286
# define	DEFAULT	287
# define	ELSE	288
# define	WHILE	289
# define	DO	290
# define	GOTO	291
# define	BREAK	292
# define	IF	293
# define	FOR	294
# define	RETURN	295
# define	IDENT	296
# define	TYPENAME	297
# define	NUMBER	298
# define	F_NUMBER	299
# define	STRING_CONST	300
# define	LSTRING_CONST	301
# define	LOG_OR	302
# define	LOG_AND	303
# define	INC	304
# define	DEC	305
# define	PTR	306
# define	ELLIPSIS	307
# define	ASSIGN	308
# define	EQCOMPARE	309
# define	ARITHCOMPARE	310
# define	SHIFT	311
# define	DECLSPEC	312
# define	ATTR	313
# define	TYPEOF	314
# define	LABEL	315
# define	ALIGNOF	316
# define	GCC_ATTRIBUTE	317


extern YYSTYPE yylval;

#endif /* not BISON_CFE_C_C_TAB_H */
