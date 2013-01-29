typedef union {
  TypePtr  type;
  NodePtr  node;
  char*    name;
  float    realval;
  int      intval;
  bool     boolval;
} YYSTYPE;
#define	IDENTIFIER	258
#define	TYPENAME	259
#define	FLOAT_CONSTANT	260
#define	INT_CONSTANT	261
#define	STRING_LITERAL	262
#define	TRUE	263
#define	FALSE	264
#define	SIZEOF	265
#define	THIS	266
#define	INC_OP	267
#define	DEC_OP	268
#define	LEFT_OP	269
#define	RIGHT_OP	270
#define	LE_OP	271
#define	GE_OP	272
#define	EQ_OP	273
#define	NE_OP	274
#define	AND_OP	275
#define	OR_OP	276
#define	MUL_ASSIGN	277
#define	DIV_ASSIGN	278
#define	MOD_ASSIGN	279
#define	ADD_ASSIGN	280
#define	SUB_ASSIGN	281
#define	LEFT_ASSIGN	282
#define	RIGHT_ASSIGN	283
#define	AND_ASSIGN	284
#define	XOR_ASSIGN	285
#define	OR_ASSIGN	286
#define	TYPEDEF	287
#define	STATIC	288
#define	VIRTUAL	289
#define	PUBLIC	290
#define	PRIVATE	291
#define	PROTECTED	292
#define	NATIVE	293
#define	CHAR	294
#define	SHORT	295
#define	INT	296
#define	LONG	297
#define	SIGNED	298
#define	UNSIGNED	299
#define	FLOAT	300
#define	CONST	301
#define	BOOL	302
#define	VOID	303
#define	STRING	304
#define	STRUCT	305
#define	CLASS	306
#define	UNION	307
#define	ENUM	308
#define	NEW	309
#define	CASE	310
#define	DEFAULT	311
#define	IF	312
#define	ELSE	313
#define	SWITCH	314
#define	WHILE	315
#define	DO	316
#define	FOR	317
#define	EACH	318
#define	IN	319
#define	CONTINUE	320
#define	BREAK	321
#define	RETURN	322


extern YYSTYPE yylval;
