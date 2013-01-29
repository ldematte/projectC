
/*  A Bison parser, made from minicpp2.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

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




#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <new>

#include <common.h>
#include "type.h"
#include "symbol.h"
#include "SymbolTable.h"
#include "SyntaxTree.h"
#include "ScopeManager.h"
#include "OutputManager.h"
#include "ErrorManager.h"

#include "lexer.h"

#define scope_stack  ScopeManager::GetSingleton()
#define type_tree    TypeRoot::GetSingleton()
#define symbol_table SymbolTable::GetSingleton()
#define parse_tree   SyntaxTree::GetSingleton()
#define output       OutputManager::GetSingleton()

#define YYDEBUG 1
#define YYERROR_VERBOSE


typedef union {
  TypePtr  type;
  NodePtr  node;
  char*    name;
  float    realval;
  int      intval;
  bool     boolval;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		294
#define	YYFLAG		-32768
#define	YYNTBASE	91

#define YYTRANSLATE(x) ((unsigned)(x) <= 322 ? yytranslate[x] : 147)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    82,     2,     2,     2,    85,    72,     2,    77,
    78,    83,    80,    73,    81,    79,    84,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    71,    70,    86,
    76,    87,    90,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    74,     2,    75,    88,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    68,    89,    69,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     7,     9,    11,    12,    15,    18,    19,
    27,    28,    38,    39,    47,    48,    56,    57,    64,    69,
    72,    74,    76,    78,    80,    82,    84,    86,    88,    90,
    92,    94,    96,    98,   100,   102,   103,   105,   106,   108,
   112,   118,   124,   132,   137,   141,   143,   147,   149,   152,
   155,   157,   162,   166,   168,   169,   175,   177,   179,   181,
   183,   185,   187,   189,   192,   193,   198,   200,   203,   208,
   212,   214,   217,   223,   231,   237,   240,   246,   254,   260,
   267,   269,   271,   274,   277,   280,   284,   286,   288,   290,
   292,   294,   296,   298,   302,   304,   309,   313,   318,   322,
   325,   328,   330,   334,   336,   339,   342,   345,   350,   352,
   354,   356,   358,   360,   365,   371,   374,   380,   382,   386,
   390,   394,   396,   400,   404,   406,   410,   414,   416,   420,
   424,   428,   432,   434,   438,   442,   444,   448,   450,   454,
   456,   460,   462,   466,   468,   472,   474,   480,   482,   486,
   488,   490,   492,   494,   496,   498,   500,   502,   504,   506,
   508,   510,   514
};

static const short yyrhs[] = {    92,
     0,    91,    92,     0,    97,     0,   107,     0,    94,     0,
     0,   103,    93,     0,    92,    93,     0,     0,    51,     3,
    68,    95,    93,    69,    70,     0,     0,    51,     3,    71,
     4,    68,    96,    93,    69,    70,     0,     0,    33,   101,
   112,    68,    98,   117,    69,     0,     0,    34,   101,   112,
    68,    99,   117,    69,     0,     0,   101,   112,    68,   100,
   117,    69,     0,    38,   101,   111,    70,     0,    46,   102,
     0,   102,     0,    48,     0,    39,     0,    40,     0,    41,
     0,    42,     0,    45,     0,    43,     0,    44,     0,    49,
     0,    47,     0,     4,     0,    35,     0,    36,     0,    37,
     0,     0,    72,     0,     0,   106,     0,   101,   104,     3,
     0,   101,   104,     3,    73,   106,     0,   101,   104,     3,
    74,    75,     0,   101,   104,     3,    74,    75,    73,   106,
     0,    33,   101,   108,    70,     0,   101,   108,    70,     0,
   109,     0,   108,    73,   109,     0,   111,     0,   111,   110,
     0,    76,   143,     0,     3,     0,   111,    74,     6,    75,
     0,   111,    74,    75,     0,   112,     0,     0,   111,   113,
    77,   105,    78,     0,   118,     0,   115,     0,   119,     0,
   120,     0,   122,     0,   124,     0,   107,     0,    68,    69,
     0,     0,    68,   116,   117,    69,     0,   114,     0,   117,
   114,     0,    55,   146,    71,   114,     0,    56,    71,   114,
     0,    70,     0,   145,    70,     0,    57,    77,   145,    78,
   114,     0,    57,    77,   145,    78,   114,    58,   114,     0,
    59,    77,   145,    78,   114,     0,    62,    77,     0,    60,
    77,   145,    78,   114,     0,    61,   114,    60,    77,   145,
    78,    70,     0,   121,   123,   119,    78,   114,     0,   121,
   123,   119,   145,    78,   114,     0,   119,     0,   107,     0,
    65,    70,     0,    66,    70,     0,    67,    70,     0,    67,
   145,    70,     0,     3,     0,    11,     0,     6,     0,     5,
     0,     7,     0,     8,     0,     9,     0,    77,   145,    78,
     0,   125,     0,   126,    74,   145,    75,     0,   126,    77,
    78,     0,   126,    77,   127,    78,     0,   126,    79,     3,
     0,   126,    12,     0,   126,    13,     0,   143,     0,   127,
    73,   143,     0,   126,     0,    12,   128,     0,    13,   128,
     0,   129,   130,     0,    10,    77,   102,    78,     0,   131,
     0,    80,     0,    81,     0,    82,     0,   128,     0,    77,
   102,    78,   130,     0,    54,   102,    77,   127,    78,     0,
    54,   102,     0,    54,   102,    74,   145,    75,     0,   130,
     0,   132,    83,   130,     0,   132,    84,   130,     0,   132,
    85,   130,     0,   132,     0,   133,    80,   132,     0,   133,
    81,   132,     0,   133,     0,   134,    14,   133,     0,   134,
    15,   133,     0,   134,     0,   135,    86,   134,     0,   135,
    87,   134,     0,   135,    16,   134,     0,   135,    17,   134,
     0,   135,     0,   136,    18,   135,     0,   136,    19,   135,
     0,   136,     0,   137,    72,   136,     0,   137,     0,   138,
    88,   137,     0,   138,     0,   139,    89,   138,     0,   139,
     0,   140,    20,   139,     0,   140,     0,   141,    21,   140,
     0,   141,     0,   141,    90,   145,    71,   142,     0,   142,
     0,   141,   144,   143,     0,    76,     0,    22,     0,    23,
     0,    24,     0,    25,     0,    26,     0,    27,     0,    28,
     0,    29,     0,    30,     0,    31,     0,   143,     0,   145,
    73,   143,     0,   142,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    97,    98,   104,   105,   106,   111,   112,   117,   126,   132,
   140,   147,   159,   169,   174,   187,   192,   201,   207,   220,
   221,   226,   227,   228,   229,   230,   231,   232,   233,   234,
   235,   236,   248,   249,   250,   254,   255,   259,   260,   264,
   273,   281,   290,   301,   308,   318,   319,   323,   329,   338,
   343,   353,   359,   366,   371,   376,   386,   387,   388,   389,
   390,   391,   392,   397,   398,   404,   413,   417,   427,   431,
   438,   442,   449,   453,   457,   464,   472,   476,   480,   485,
   494,   495,   499,   500,   501,   502,   508,   512,   513,   514,
   515,   516,   517,   518,   522,   523,   524,   525,   529,   534,
   535,   539,   540,   544,   545,   546,   547,   548,   559,   563,
   564,   565,   569,   570,   574,   575,   576,   580,   581,   582,
   583,   587,   588,   589,   593,   594,   595,   599,   600,   601,
   602,   603,   607,   608,   609,   613,   614,   618,   619,   623,
   624,   628,   629,   633,   634,   638,   639,   643,   644,   648,
   649,   650,   651,   652,   653,   654,   655,   656,   657,   658,
   662,   663,   667
};

static const char * const yytname[] = {   "$","error","$undefined.","IDENTIFIER",
"TYPENAME","FLOAT_CONSTANT","INT_CONSTANT","STRING_LITERAL","TRUE","FALSE","SIZEOF",
"THIS","INC_OP","DEC_OP","LEFT_OP","RIGHT_OP","LE_OP","GE_OP","EQ_OP","NE_OP",
"AND_OP","OR_OP","MUL_ASSIGN","DIV_ASSIGN","MOD_ASSIGN","ADD_ASSIGN","SUB_ASSIGN",
"LEFT_ASSIGN","RIGHT_ASSIGN","AND_ASSIGN","XOR_ASSIGN","OR_ASSIGN","TYPEDEF",
"STATIC","VIRTUAL","PUBLIC","PRIVATE","PROTECTED","NATIVE","CHAR","SHORT","INT",
"LONG","SIGNED","UNSIGNED","FLOAT","CONST","BOOL","VOID","STRING","STRUCT","CLASS",
"UNION","ENUM","NEW","CASE","DEFAULT","IF","ELSE","SWITCH","WHILE","DO","FOR",
"EACH","IN","CONTINUE","BREAK","RETURN","'{'","'}'","';'","':'","'&'","','",
"'['","']'","'='","'('","')'","'.'","'+'","'-'","'!'","'*'","'/'","'%'","'<'",
"'>'","'^'","'|'","'?'","translation_unit","declaration_block","class_body",
"class_declaration","@1","@2","function_definition","@3","@4","@5","type_decl",
"type_specifier","access_modifier","arg_modifier","parameter_decl_clause","parameter_type_list",
"declaration","identifier_list","init_declarator","initializer","declarator",
"func_declarator","@6","statement","compound_statement","@7","statement_list",
"labeled_statement","expression_statement","selection_statement","for_init",
"iteration_statement","for_init_statement","jump_statement","primary_expression",
"postfix_expression","argument_expression_list","unary_expression","unary_operator",
"cast_expression","new_expression","multiplicative_expression","additive_expression",
"shift_expression","relational_expression","equality_expression","and_expression",
"exclusive_or_expression","inclusive_or_expression","logical_and_expression",
"logical_or_expression","conditional_expression","assignment_expression","assignment_operator",
"expression","constant_expression",""
};
#endif

static const short yyr1[] = {     0,
    91,    91,    92,    92,    92,    93,    93,    93,    95,    94,
    96,    94,    98,    97,    99,    97,   100,    97,    97,   101,
   101,   102,   102,   102,   102,   102,   102,   102,   102,   102,
   102,   102,   103,   103,   103,   104,   104,   105,   105,   106,
   106,   106,   106,   107,   107,   108,   108,   109,   109,   110,
   111,   111,   111,   111,   113,   112,   114,   114,   114,   114,
   114,   114,   114,   115,   116,   115,   117,   117,   118,   118,
   119,   119,   120,   120,   120,   121,   122,   122,   122,   122,
   123,   123,   124,   124,   124,   124,   125,   125,   125,   125,
   125,   125,   125,   125,   126,   126,   126,   126,   126,   126,
   126,   127,   127,   128,   128,   128,   128,   128,   128,   129,
   129,   129,   130,   130,   131,   131,   131,   132,   132,   132,
   132,   133,   133,   133,   134,   134,   134,   135,   135,   135,
   135,   135,   136,   136,   136,   137,   137,   138,   138,   139,
   139,   140,   140,   141,   141,   142,   142,   143,   143,   144,
   144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
   145,   145,   146
};

static const short yyr2[] = {     0,
     1,     2,     1,     1,     1,     0,     2,     2,     0,     7,
     0,     9,     0,     7,     0,     7,     0,     6,     4,     2,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     0,     1,     0,     1,     3,
     5,     5,     7,     4,     3,     1,     3,     1,     2,     2,
     1,     4,     3,     1,     0,     5,     1,     1,     1,     1,
     1,     1,     1,     2,     0,     4,     1,     2,     4,     3,
     1,     2,     5,     7,     5,     2,     5,     7,     5,     6,
     1,     1,     2,     2,     2,     3,     1,     1,     1,     1,
     1,     1,     1,     3,     1,     4,     3,     4,     3,     2,
     2,     1,     3,     1,     2,     2,     2,     4,     1,     1,
     1,     1,     1,     4,     5,     2,     5,     1,     3,     3,
     3,     1,     3,     3,     1,     3,     3,     1,     3,     3,
     3,     3,     1,     3,     3,     1,     3,     1,     3,     1,
     3,     1,     3,     1,     3,     1,     5,     1,     3,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     3,     1
};

static const short yydefact[] = {     0,
    32,     0,     0,     0,    23,    24,    25,    26,    28,    29,
    27,     0,    31,    22,    30,     0,     0,     1,     5,     3,
     0,    21,     4,     0,     0,     0,    20,     0,     2,    51,
     0,    46,    48,    54,     0,    54,    55,    54,    55,    54,
     9,     0,    45,     0,     0,     0,    49,     0,    17,    44,
    13,    15,    19,     6,     0,    47,     0,    53,    87,    90,
    89,    91,    92,    93,     0,    88,     0,     0,     0,     0,
   110,   111,   112,    95,   104,   113,     0,   118,   109,   122,
   125,   128,   133,   136,   138,   140,   142,   144,   146,   148,
    50,    38,     0,     0,     0,    33,    34,    35,     6,     0,
     6,    11,    52,     0,     0,   105,   106,   116,     0,   161,
     0,   100,   101,     0,     0,     0,   107,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   151,   152,   153,   154,   155,
   156,   157,   158,   159,   160,   150,     0,     0,    36,     0,
    39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    65,    71,     0,    63,    67,    58,     0,    57,
    59,    60,     0,    61,    62,     0,     0,     0,     8,     0,
     7,     6,     0,     0,     0,     0,     0,    94,     0,    97,
     0,   102,    99,   119,   120,   121,   123,   124,   126,   127,
   131,   132,   129,   130,   134,   135,   137,   139,   141,   143,
   145,     0,   149,    37,     0,    56,     0,   146,   163,     0,
     0,     0,     0,     0,     0,    76,    83,    84,    85,     0,
    64,     0,    18,    68,    82,    81,     0,    72,    14,    16,
    10,     0,   108,     0,     0,   114,   162,    96,     0,    98,
     0,    40,     0,    70,     0,     0,     0,     0,    86,     0,
     0,     0,   117,   115,   103,   147,     0,     0,    69,     0,
     0,     0,     0,    66,     0,     0,    12,    41,    42,    73,
    75,    77,     0,    79,     0,     0,     0,     0,    80,    43,
    74,    78,     0,     0
};

static const short yydefgoto[] = {    17,
    99,   100,    19,    54,   182,    20,    94,    95,    93,   165,
    22,   101,   215,   150,   151,   166,    31,    32,    47,    33,
    40,    48,   167,   168,   232,   169,   170,   171,   172,   173,
   174,   237,   175,    74,    75,   191,    76,    77,    78,    79,
    80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
    90,   110,   148,   176,   220
};

static const short yypact[] = {   792,
-32768,   513,   513,   513,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   441,-32768,-32768,-32768,    56,   746,-32768,-32768,-32768,
    74,-32768,-32768,    74,    74,    74,-32768,     4,-32768,-32768,
     3,-32768,    21,    75,    32,    89,    38,   110,   -10,-32768,
-32768,    85,-32768,    74,     6,   313,-32768,    23,-32768,-32768,
-32768,-32768,-32768,   766,   111,-32768,    43,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,   104,-32768,   393,   393,   441,   631,
-32768,-32768,-32768,-32768,    17,-32768,   313,-32768,-32768,   -34,
   -49,   134,    -1,    64,    73,    95,    96,   164,   693,-32768,
-32768,   513,   534,   534,   534,-32768,-32768,-32768,   766,   117,
   766,-32768,-32768,   441,   313,-32768,-32768,    39,   109,-32768,
   -40,-32768,-32768,   313,   153,   185,-32768,   313,   313,   313,
   313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
   313,   313,   313,   313,   313,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   313,   313,   118,   115,
-32768,   513,   313,   123,   119,   120,   128,   534,   129,   125,
   130,   121,   140,-32768,    74,-32768,-32768,-32768,   214,-32768,
-32768,-32768,   614,-32768,-32768,    66,   294,   374,-32768,   141,
-32768,   766,   132,   313,   313,   313,   313,-32768,    15,-32768,
   -39,-32768,-32768,-32768,-32768,-32768,   -34,   -34,   -49,   -49,
   134,   134,   134,   134,    -1,    -1,    64,    73,    95,    96,
   164,    46,-32768,-32768,   209,-32768,    74,    -3,-32768,   143,
   534,   313,   313,   313,   155,-32768,-32768,-32768,-32768,    68,
-32768,   534,-32768,-32768,-32768,-32768,   694,-32768,-32768,-32768,
-32768,   144,-32768,    67,   -38,-32768,-32768,-32768,   313,-32768,
   313,    77,   534,-32768,   -36,   -32,   -26,   139,-32768,   454,
   233,   158,-32768,-32768,-32768,-32768,   513,   154,-32768,   534,
   534,   534,   313,-32768,   534,   -25,-32768,-32768,   159,   179,
-32768,-32768,   -15,-32768,   534,   513,   534,   178,-32768,-32768,
-32768,-32768,   249,-32768
};

static const short yypgoto[] = {-32768,
    26,   -78,-32768,-32768,-32768,-32768,-32768,-32768,-32768,     0,
    10,-32768,-32768,-32768,  -242,     7,   -18,   206,-32768,   142,
    41,-32768,  -150,-32768,-32768,   -85,-32768,  -153,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,    79,   102,-32768,   -63,-32768,
    50,    53,   -58,    44,   135,   133,   145,   138,   116,  -142,
  -140,   -41,-32768,   -69,-32768
};


#define	YYLAST		843


static const short yytable[] = {    21,
   111,    24,    25,    26,    91,    35,    23,   225,   177,   178,
   218,    57,   219,   117,   125,   126,    21,   135,   234,   236,
   179,    27,   181,    23,   278,    18,   234,   234,   112,   113,
   121,   122,   187,   249,   249,   111,   187,   188,   250,   264,
   187,   270,    29,   290,   189,   271,   187,   187,   118,   119,
   120,   272,   285,    21,   194,   195,   196,   187,    28,    53,
    23,    34,   288,    45,    36,    38,   201,   202,   203,   204,
   254,    41,    43,   192,    42,    44,    30,   212,   108,   109,
    58,   129,   130,   261,   127,   128,   147,   187,    55,   248,
   114,   149,   230,   115,    45,   116,    46,   -55,    21,    92,
    21,    50,   269,   242,    44,    23,   213,    23,   218,   234,
   266,    45,   184,   183,   244,   185,   251,   103,   187,   280,
   281,   282,   246,    59,   284,    60,    61,    62,    63,    64,
    65,    66,    67,    68,   289,   238,   291,   259,   187,   187,
   187,   263,    49,   192,   131,   247,   260,   123,   124,   267,
   268,   217,   255,   256,   257,    59,    51,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    37,    39,   106,   107,
   197,   198,   205,   206,    69,   199,   200,    52,   102,   235,
   104,    21,   132,   134,   133,   180,   186,   193,    23,   214,
   229,   276,   216,   221,   227,   222,   223,    70,    35,   228,
    71,    72,    73,   283,   224,   226,    69,   265,   231,   243,
   241,   252,   262,   253,   258,   273,    59,     1,    60,    61,
    62,    63,    64,    65,    66,    67,    68,   277,   279,    70,
   190,   286,    71,    72,    73,    59,   287,    60,    61,    62,
    63,    64,    65,    66,    67,    68,   152,   292,   294,    56,
   211,     0,     5,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,   245,   208,   207,   149,    69,   153,   154,
   155,   210,   156,   157,   158,   159,     0,   209,   160,   161,
   162,   163,   233,   164,     0,   149,    69,     0,     0,     0,
    70,     0,     0,    71,    72,    73,    59,     1,    60,    61,
    62,    63,    64,    65,    66,    67,    68,     0,     0,    70,
   275,     0,    71,    72,    73,    59,     0,    60,    61,    62,
    63,    64,    65,    66,    67,    68,   152,     0,     0,     0,
     0,     0,     5,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,     0,     0,     0,     0,    69,   153,   154,
   155,     0,   156,   157,   158,   159,     0,     0,   160,   161,
   162,   163,   239,   164,     0,     0,    69,     0,     0,     0,
    70,     0,     0,    71,    72,    73,    59,     1,    60,    61,
    62,    63,    64,    65,    66,    67,    68,     0,     0,    70,
     0,     0,    71,    72,    73,    59,     0,    60,    61,    62,
    63,    64,    65,    66,    67,    68,   152,     0,     0,     0,
     0,     0,     5,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,     0,     0,     0,     0,    69,   153,   154,
   155,     0,   156,   157,   158,   159,     0,     0,   160,   161,
   162,   163,   240,   164,     1,     0,    69,     0,     0,     0,
    70,     0,     0,    71,    72,    73,    59,     1,    60,    61,
    62,    63,    64,    65,    66,    67,    68,     0,     0,   105,
     0,     0,    71,    72,    73,     0,     0,     0,     0,     5,
     6,     7,     8,     9,    10,    11,   152,    13,    14,    15,
     0,     0,     5,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,     0,     0,     0,     0,    69,   153,   154,
   155,     0,   156,   157,   158,   159,     1,     0,   160,   161,
   162,   163,   274,   164,     0,     0,     0,     0,     0,     0,
    70,     0,     0,    71,    72,    73,    59,     1,    60,    61,
    62,    63,    64,    65,    66,    67,    68,     0,     0,     0,
     0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
    14,    15,     0,     0,     0,     0,   152,     0,     0,     0,
     0,     0,     5,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,     0,     0,     0,     0,    69,   153,   154,
   155,     0,   156,   157,   158,   159,     0,     0,   160,   161,
   162,   163,     0,   164,     0,     0,     0,     0,     0,     0,
    70,     0,     0,    71,    72,    73,    59,     1,    60,    61,
    62,    63,    64,    65,    66,    67,    68,     0,     0,     0,
     0,     0,     0,    59,     1,    60,    61,    62,    63,    64,
    65,    66,    67,    68,     0,     0,   152,     0,     0,     0,
     0,     0,     5,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,     0,     0,     0,     0,    69,     0,     5,
     6,     7,     8,     9,    10,    11,     0,    13,    14,    15,
     0,     0,     0,   164,    69,     0,     0,     0,     0,     0,
    70,     0,     0,    71,    72,    73,    59,     0,    60,    61,
    62,    63,    64,    65,    66,    67,    68,    70,     0,     0,
    71,    72,    73,   135,   136,   137,   138,   139,   140,   141,
   142,   143,   144,   145,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   293,     0,    69,     0,     1,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   164,     0,     0,     0,     0,   146,     1,
    70,     0,     0,    71,    72,    73,     0,     0,     2,     3,
     0,     0,   147,     4,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,    15,     1,    16,     0,     2,     3,
    96,    97,    98,     4,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,    15,     0,    16,     0,     0,     0,
     0,     0,     0,     0,     2,     3,     0,     0,     0,     4,
     5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
    15,     0,    16
};

static const short yycheck[] = {     0,
    70,     2,     3,     4,    46,    24,     0,   158,    94,    95,
   153,     6,   153,    77,    16,    17,    17,    21,   169,   173,
    99,    12,   101,    17,   267,     0,   177,   178,    12,    13,
    80,    81,    73,    73,    73,   105,    73,    78,    78,    78,
    73,    78,    17,   286,   114,    78,    73,    73,    83,    84,
    85,    78,    78,    54,   118,   119,   120,    73,     3,    70,
    54,    21,    78,    74,    24,    25,   125,   126,   127,   128,
   221,    68,    70,   115,    71,    73,     3,   147,    69,    70,
    75,    18,    19,   237,    86,    87,    90,    73,     4,    75,
    74,    92,   162,    77,    74,    79,    76,    77,    99,    77,
   101,    70,   253,   182,    73,    99,   148,   101,   251,   260,
   251,    74,    74,   104,   184,    77,    71,    75,    73,   270,
   271,   272,   186,     3,   275,     5,     6,     7,     8,     9,
    10,    11,    12,    13,   285,    70,   287,    70,    73,    73,
    73,    75,    68,   185,    72,   187,   232,    14,    15,    73,
    74,   152,   222,   223,   224,     3,    68,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    25,    26,    67,    68,
   121,   122,   129,   130,    54,   123,   124,    68,    68,   173,
    77,   182,    88,    20,    89,    69,    78,     3,   182,    72,
    70,   261,    78,    71,    70,    77,    77,    77,   217,    70,
    80,    81,    82,   273,    77,    77,    54,   249,    69,    78,
    70,     3,    69,    71,    60,    77,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    70,    75,    77,
    78,    73,    80,    81,    82,     3,    58,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    33,    70,     0,    44,
   135,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,   185,   132,   131,   267,    54,    55,    56,
    57,   134,    59,    60,    61,    62,    -1,   133,    65,    66,
    67,    68,    69,    70,    -1,   286,    54,    -1,    -1,    -1,
    77,    -1,    -1,    80,    81,    82,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    -1,    -1,    77,
    78,    -1,    80,    81,    82,     3,    -1,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    33,    -1,    -1,    -1,
    -1,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    -1,    -1,    -1,    -1,    54,    55,    56,
    57,    -1,    59,    60,    61,    62,    -1,    -1,    65,    66,
    67,    68,    69,    70,    -1,    -1,    54,    -1,    -1,    -1,
    77,    -1,    -1,    80,    81,    82,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    -1,    -1,    77,
    -1,    -1,    80,    81,    82,     3,    -1,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    33,    -1,    -1,    -1,
    -1,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    -1,    -1,    -1,    -1,    54,    55,    56,
    57,    -1,    59,    60,    61,    62,    -1,    -1,    65,    66,
    67,    68,    69,    70,     4,    -1,    54,    -1,    -1,    -1,
    77,    -1,    -1,    80,    81,    82,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    -1,    -1,    77,
    -1,    -1,    80,    81,    82,    -1,    -1,    -1,    -1,    39,
    40,    41,    42,    43,    44,    45,    33,    47,    48,    49,
    -1,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    -1,    -1,    -1,    -1,    54,    55,    56,
    57,    -1,    59,    60,    61,    62,     4,    -1,    65,    66,
    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
    77,    -1,    -1,    80,    81,    82,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    -1,    -1,    -1,
    -1,    39,    40,    41,    42,    43,    44,    45,    46,    47,
    48,    49,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
    -1,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    -1,    -1,    -1,    -1,    54,    55,    56,
    57,    -1,    59,    60,    61,    62,    -1,    -1,    65,    66,
    67,    68,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
    77,    -1,    -1,    80,    81,    82,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    -1,    -1,    -1,
    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    -1,    -1,    33,    -1,    -1,    -1,
    -1,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    -1,    -1,    -1,    -1,    54,    -1,    39,
    40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
    -1,    -1,    -1,    70,    54,    -1,    -1,    -1,    -1,    -1,
    77,    -1,    -1,    80,    81,    82,     3,    -1,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    77,    -1,    -1,
    80,    81,    82,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,     0,    -1,    54,    -1,     4,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    76,     4,
    77,    -1,    -1,    80,    81,    82,    -1,    -1,    33,    34,
    -1,    -1,    90,    38,    39,    40,    41,    42,    43,    44,
    45,    46,    47,    48,    49,     4,    51,    -1,    33,    34,
    35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    -1,    51
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */


/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
{ parse_tree.addSon(new DeclBlock(yyvsp[0].node)); ;
    break;}
case 2:
{ parse_tree.addSon(new DeclBlock(yyvsp[0].node)); ;
    break;}
case 3:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 4:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 5:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 6:
{ yyval.node = new EmptyNode(); ;
    break;}
case 7:
{ 
      scope_stack.getCurrentScope()->accessLevel = (ACCESS_LEVEL)yyvsp[-1].intval;
      yyval.node = yyvsp[0].node;
    ;
    break;}
case 8:
{
      yyval.node = new ClassBodyNode(yyvsp[-1].node, yyvsp[0].node);      
    ;
    break;}
case 9:
{ 
      SymPtr new_class = symbol_table.declareClass(symbol_table.getLexToken(yyvsp[-1].intval), 
                                                   scope_stack.getCurrentScope());
      scope_stack.pushScope(new_class); 
    ;
    break;}
case 10:
{
      //we have done with the current class
      SymPtr new_class = scope_stack.getCurrentScope();
      //new_class->updateType();
      yyval.node = new ClassNode(yyvsp[-2].node, new_class);           
      scope_stack.popScope(); 
    ;
    break;}
case 11:
{ 
      SymPtr new_class = symbol_table.declareClass(symbol_table.getLexToken(yyvsp[-3].intval), 
                                                   symbol_table.getLexToken(yyvsp[-1].intval), 
                                                   scope_stack.getCurrentScope());
      scope_stack.pushScope(new_class); 
    ;
    break;}
case 12:
{ 
      //we have done with the current class
      SymPtr new_class = scope_stack.getCurrentScope();
      //new_class->updateType(); 
      yyval.node = new ClassNode(yyvsp[-2].node, new_class);           
      scope_stack.popScope(); 
    ;
    break;}
case 13:
{
      SymPtr class_scope = scope_stack.getCurrentScope()->parent;       	  
 	  SymPtr new_func = symbol_table.declareFunction2(yyvsp[-1].node->getSymbol(), yyvsp[-2].type, 
 	                                                  class_scope->accessLevel);
	                                                 
	  new_func->isStatic = true;
	  new_func->isDefined = true;
   	  scope_stack.pushScope(new_func);	  
    ;
    break;}
case 14:
{
      yyval.node = new FunctionDefNode(yyvsp[-1].node, yyvsp[-4].node->getSymbol());            
      scope_stack.popScope();       
    ;
    break;}
case 15:
{ 
      SymPtr class_scope = scope_stack.getCurrentScope()->parent;
      if (class_scope->type->typeID != TYPE_CLASS)
      {
        //raise error
        ErrorManager::GetSingleton().logAndRaise("\"virtual\" allowed only in classes");
      }
	  
      SymPtr new_func = symbol_table.declareVirtualFunction(yyvsp[-1].node->getSymbol(), yyvsp[-2].type,
	                                                        class_scope->accessLevel);
   	  new_func->isDefined = true;
    ;
    break;}
case 16:
{ 
      yyval.node = new FunctionDefNode(yyvsp[-1].node, yyvsp[-4].node->getSymbol());
      scope_stack.popScope();      
    ;
    break;}
case 17:
{ 
      SymPtr class_scope = scope_stack.getCurrentScope()->parent;
      SymPtr new_func = symbol_table.declareFunction2(yyvsp[-1].node->getSymbol(), yyvsp[-2].type,
	                                                  class_scope->accessLevel);

  	  new_func->isDefined = true;
      new_func->isCtor = (new_func->name == class_scope->name);
    ;
    break;}
case 18:
{ 
      yyval.node = new FunctionDefNode(yyvsp[-1].node, yyvsp[-4].node->getSymbol());
      scope_stack.popScope();      
    ;
    break;}
case 19:
{
	    SymPtr class_scope = scope_stack.getCurrentScope()->parent;       	  
 	    /*SymPtr new_func = */ symbol_table.declareNativeFunction2(yyvsp[-1].node->getSymbol(), yyvsp[-2].type, 
 	                                                          class_scope->accessLevel);
 	                                                         
 	    yyval.node = new EmptyNode();
        scope_stack.popScope();     
 	  ;
    break;}
case 20:
{ yyval.type = TypeDef::createConst(yyvsp[0].type); ;
    break;}
case 21:
{ yyval.type = yyvsp[0].type;          ;
    break;}
case 22:
{ yyval.type = type_tree.getStdType(TYPE_VOID);   ;
    break;}
case 23:
{ yyval.type = type_tree.getStdType(TYPE_INT);    ;
    break;}
case 24:
{ yyval.type = type_tree.getStdType(TYPE_INT);    ;
    break;}
case 25:
{ yyval.type = type_tree.getStdType(TYPE_INT);    ;
    break;}
case 26:
{ yyval.type = type_tree.getStdType(TYPE_INT);    ;
    break;}
case 27:
{ yyval.type = type_tree.getStdType(TYPE_FLOAT);  ;
    break;}
case 28:
{ yyval.type = type_tree.getStdType(TYPE_INT);    ;
    break;}
case 29:
{ yyval.type = type_tree.getStdType(TYPE_INT);    ;
    break;}
case 30:
{ yyval.type = type_tree.getStdType(TYPE_STRING); ;
    break;}
case 31:
{ yyval.type = type_tree.getStdType(TYPE_BOOL);   ;
    break;}
case 32:
{ 
	    SymPtr searchScope = scope_stack.getCurrentScope();
	    if (searchScope->type == NULL)
	      searchScope = searchScope->parent;
	      
	    yyval.type = searchScope->type->resolveName(symbol_table.getLexToken(yyvsp[0].intval));	   
	  ;
    break;}
case 33:
{ yyval.intval = ACCESS_PUBLIC;    ;
    break;}
case 34:
{ yyval.intval = ACCESS_PRIVATE;   ;
    break;}
case 35:
{ yyval.intval = ACCESS_PROTECTED; ;
    break;}
case 36:
{ yyval.boolval = true;  ;
    break;}
case 37:
{ yyval.boolval = false; ;
    break;}
case 38:
{  yyval.type = TypeDef::createProduct(); ;
    break;}
case 39:
{  yyval.type = yyvsp[0].type;                       ;
    break;}
case 40:
{ 
	  SymPtr current_scope = scope_stack.getCurrentScope();
      TypePtr var = TypeDef::createParameter(yyvsp[-2].type, yyvsp[-1].boolval);
      symbol_table.declareParam(symbol_table.getLexToken(yyvsp[0].intval), var, current_scope);
      TypePtr param_list = TypeDef::createProduct();
      param_list->addParam(var);
	  yyval.type = param_list;
	;
    break;}
case 41:
{ 
      SymPtr current_scope = scope_stack.getCurrentScope();
      TypePtr var = TypeDef::createParameter(yyvsp[-4].type, yyvsp[-3].boolval);
      symbol_table.declareParam(symbol_table.getLexToken(yyvsp[-2].intval), var, current_scope);
	  yyvsp[0].type->addParam(var);
	  yyval.type = yyvsp[0].type;
    ;
    break;}
case 42:
{ 
	  SymPtr current_scope = scope_stack.getCurrentScope();
      TypePtr var = TypeDef::createParameter(yyvsp[-4].type, yyvsp[-3].boolval, true);
      symbol_table.declareParam(symbol_table.getLexToken(yyvsp[-2].intval), var, current_scope);
      TypePtr param_list = TypeDef::createProduct();
      param_list->addParam(var);
	  yyval.type = param_list;
	;
    break;}
case 43:
{ 
      SymPtr current_scope = scope_stack.getCurrentScope();
      TypePtr var = TypeDef::createParameter(yyvsp[-6].type, yyvsp[-5].boolval, true);
      symbol_table.declareParam(symbol_table.getLexToken(yyvsp[-4].intval), var, current_scope);
	  yyvsp[0].type->addParam(var);
	  yyval.type = yyvsp[0].type;
    ;
    break;}
case 44:
{
      //a tutti gli id in identifier_list applico le info sul tipo
      NodePtr decl = new DeclListNode(yyvsp[-2].type, yyvsp[-1].node, true); //type, id list, is static
      decl->propagateTypeInfo(decl->getType(), true);
      yyval.node = decl;
    ;
    break;}
case 45:
{
      //a tutti gli id in identifier_list applico le info sul tipo
      NodePtr decl = new DeclListNode(yyvsp[-2].type, yyvsp[-1].node, false); //type, id list, is static
      decl->propagateTypeInfo(decl->getType(), false);
      yyval.node = decl;
    ;
    break;}
case 46:
{ yyval.node = new DeclNode(yyvsp[0].node); ;
    break;}
case 47:
{ yyval.node = new DeclNode(yyvsp[0].node, yyvsp[-2].node); ;
    break;}
case 48:
{
      if (!(yyvsp[0].node->getSymbol()->isFullyDeclared))
        symbol_table.declareVar(yyvsp[0].node->getSymbol(), TR.getStdType(TYPE_VOID), scope_stack.getCurrentScope());  
      yyval.node = yyvsp[0].node;          
    ;
    break;}
case 49:
{ 
      if (!(yyvsp[-1].node->getSymbol()->isFullyDeclared))
        symbol_table.declareVar(yyvsp[-1].node->getSymbol(), TR.getStdType(TYPE_VOID), scope_stack.getCurrentScope()); 
      yyval.node = new InitDeclNode(yyvsp[-1].node, yyvsp[0].node); 
    ;
    break;}
case 50:
{ yyval.node = new AssignInitNode(yyvsp[0].node); ;
    break;}
case 51:
{
	    SymPtr current_scope = scope_stack.getCurrentScope();
	    
	    //we don't know if this is a function or a var, we need
	    //to disambiguate at an higher level
	    //per ora facciamo finta che sia una variabile  semplice
        SymPtr new_var = symbol_table.declareVar(symbol_table.getLexToken(yyvsp[0].intval), current_scope);      
        yyval.node = new VarDeclNode(new_var);
	  ;
    break;}
case 52:
{	    
	    TypePtr var_type = TypeDef::createEmptyArray(yyvsp[-1].intval);
	    SymPtr arraySymbol = symbol_table.declareVar(yyvsp[-3].node->getSymbol(), var_type, scope_stack.getCurrentScope());
	    yyval.node = new VarDeclArrayNode(yyvsp[-3].node, arraySymbol);
      ;
    break;}
case 53:
{
	    TypePtr var_type = TypeDef::createEmptyArray();
	    SymPtr arraySymbol = symbol_table.declareVar(yyvsp[-2].node->getSymbol(), var_type, scope_stack.getCurrentScope());
	    yyval.node = new VarDeclArrayNode(yyvsp[-2].node, arraySymbol);
      ;
    break;}
case 54:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 55:
{
        yyvsp[0].node->getSymbol()->symKind = SYM_FUNCTION;
	    scope_stack.pushScope(yyvsp[0].node->getSymbol());
	  ;
    break;}
case 56:
{	    
        SymPtr new_var = symbol_table.declareFunction1(yyvsp[-4].node->getSymbol(), yyvsp[-1].type);        
        yyval.node = new FunctionDeclNode(new_var);	    
	  ;
    break;}
case 64:
{ yyval.node = new EmptyNode(); ;
    break;}
case 65:
{ 
        SymPtr block = symbol_table.declareLclBlock(scope_stack.getCurrentScope());
        scope_stack.pushScope(block);
      ;
    break;}
case 66:
{
        yyval.node = new StatementListNode(yyvsp[-1].node);                
        //finally
        scope_stack.popScope();
      ;
    break;}
case 67:
{
	    yyval.node = yyvsp[0].node;
	  ;
    break;}
case 68:
{
	    yyval.node = new StatementListNode(yyvsp[-1].node, yyvsp[0].node);
	  ;
    break;}
case 69:
{
	    yyval.node = new CaseNode(yyvsp[-2].node, yyvsp[0].node);
	  ;
    break;}
case 70:
{
	    yyval.node = new CaseNode(yyvsp[0].node);
	  ;
    break;}
case 71:
{
	    yyval.node = new ExprNode();
	  ;
    break;}
case 72:
{
	    yyval.node = yyvsp[-1].node;
	  ;
    break;}
case 73:
{
	    yyval.node = new ITENode(yyvsp[-2].node, yyvsp[0].node);
	  ;
    break;}
case 74:
{
	    yyval.node = new ITENode(yyvsp[-4].node, yyvsp[-2].node, yyvsp[0].node);
	  ;
    break;}
case 75:
{
	    yyval.node = new SwitchNode(yyvsp[-2].node, yyvsp[0].node);
	  ;
    break;}
case 76:
{
	    SymPtr block = symbol_table.declareLclBlock(scope_stack.getCurrentScope());
        scope_stack.pushScope(block);
	  ;
    break;}
case 77:
{
	    yyval.node = new WhileNode(yyvsp[-2].node, yyvsp[0].node);
	  ;
    break;}
case 78:
{
	    yyval.node = new DoWhileNode(yyvsp[-2].node, yyvsp[-5].node);
	  ;
    break;}
case 79:
{
	    yyval.node = new ForNode(yyvsp[-3].node, yyvsp[-2].node, new ExprNode(), yyvsp[0].node);
	    scope_stack.popScope();
	  ;
    break;}
case 80:
{
	    yyval.node = new ForNode(yyvsp[-4].node, yyvsp[-3].node, yyvsp[-2].node, yyvsp[0].node);
	    scope_stack.popScope();
	  ;
    break;}
case 81:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 82:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 83:
{ yyval.node = new ContinueNode(); ;
    break;}
case 84:
{ yyval.node = new BreakNode(); ;
    break;}
case 85:
{ yyval.node = new ReturnNode(scope_stack.getCurrentScope()); ;
    break;}
case 86:
{ yyval.node = new ReturnNode(yyvsp[-1].node, scope_stack.getCurrentScope()); ;
    break;}
case 87:
{ 
	    yyval.node = new IdentUseNode(scope_stack.getCurrentScope(), symbol_table.getLexToken(yyvsp[0].intval)); 
	  ;
    break;}
case 88:
{ yyval.node = new ThisNode(scope_stack.getCurrentScope()); ;
    break;}
case 89:
{ yyval.node = new IntNode(yyvsp[0].intval); ;
    break;}
case 90:
{ yyval.node = new FloatNode(yyvsp[0].realval); ;
    break;}
case 91:
{ yyval.node = new StringNode(yyvsp[0].name); ;
    break;}
case 92:
{ yyval.node = new BoolNode(true); ;
    break;}
case 93:
{ yyval.node = new BoolNode(false); ;
    break;}
case 94:
{ yyval.node = new ExprNode(yyvsp[-1].node); ;
    break;}
case 95:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 96:
{ yyval.node = new ArrayUseNode(yyvsp[-3].node, yyvsp[-1].node); ;
    break;}
case 97:
{ yyval.node = new FunctionCallNode(yyvsp[-2].node); ;
    break;}
case 98:
{ 
      yyval.node = new FunctionCallNode(yyvsp[-3].node, yyvsp[-1].node); 
    ;
    break;}
case 99:
{
	    //HERE
  	  yyval.node = new IdentUseNode(scope_stack.getCurrentScope(), symbol_table.getLexToken(yyvsp[0].intval), yyvsp[-2].node);
    ;
    break;}
case 100:
{ yyval.node = new PostIncrNode(yyvsp[-1].node); ;
    break;}
case 101:
{ yyval.node = new PostDecrNode(yyvsp[-1].node); ;
    break;}
case 102:
{ yyval.node = new ArgListNode(yyvsp[0].node); ;
    break;}
case 103:
{ yyval.node = new ArgListNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 104:
{ yyval.node = yyvsp[0].node ;
    break;}
case 105:
{ yyval.node = new PreIncrNode(yyvsp[0].node); ;
    break;}
case 106:
{ yyval.node = new PreDecrNode(yyvsp[0].node); ;
    break;}
case 107:
{ yyval.node = new UnaryOpNode((UnaryOpNode::UN_OP_TYPE)yyvsp[-1].intval, yyvsp[0].node); ;
    break;}
case 108:
{ 
	    //TODO2: magari migliorarlo x gli array?
	    //o per le stringhe? in futuro, magari...
	    int dim = yyvsp[-1].type->getTypeSize();
	    if (dim == 0) 
	    {
	      //raise error
	    } 
	    yyval.node = new IntNode(dim); 
	  ;
    break;}
case 109:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 110:
{ yyval.intval = UnaryOpNode::UN_OP_PLUS;  ;
    break;}
case 111:
{ yyval.intval = UnaryOpNode::UN_OP_MINUS; ;
    break;}
case 112:
{ yyval.intval = UnaryOpNode::UN_OP_NOT;   ;
    break;}
case 113:
{ yyval.node = yyvsp[0].node;                   ;
    break;}
case 114:
{ yyval.node = new CastNode(yyvsp[-2].type, yyvsp[0].node); ;
    break;}
case 115:
{ yyval.node = new NewNode(yyvsp[-3].type, yyvsp[-1].node);      ;
    break;}
case 116:
{ yyval.node = new NewNode(yyvsp[0].type);          ;
    break;}
case 117:
{ yyval.node = new NewArrayNode(yyvsp[-3].type, yyvsp[-1].node); ;
    break;}
case 118:
{ yyval.node = yyvsp[0].node;                  ;
    break;}
case 119:
{ yyval.node = new MulNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 120:
{ yyval.node = new DivNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 121:
{ yyval.node = new ModNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 122:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 123:
{ yyval.node = new AddNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 124:
{ yyval.node = new SubNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 125:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 126:
{ yyval.node = new LshNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 127:
{ yyval.node = new RshNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 128:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 129:
{ yyval.node = new LessNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 130:
{ yyval.node = new GreaterNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 131:
{ yyval.node = new LENode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 132:
{ yyval.node = new GENode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 133:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 134:
{ yyval.node = new EqualNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 135:
{ yyval.node = new NEqualNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 136:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 137:
{ yyval.node = new BitAndNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 138:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 139:
{ yyval.node = new BitXorNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 140:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 141:
{ yyval.node = new BitOrNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 142:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 143:
{ yyval.node = new AndNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 144:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 145:
{ yyval.node = new OrNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 146:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 147:
{ yyval.node = new ThreeNode(yyvsp[-4].node, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 148:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 149:
{ yyval.node = new AssignementNode(yyvsp[-2].node, yyvsp[0].node, (AssignementNode::ASSIGN_TYPE)yyvsp[-1].intval); ;
    break;}
case 150:
{ yyval.intval = AssignementNode::ASSIGN; ;
    break;}
case 151:
{ yyval.intval = AssignementNode::ASSIGN_MUL; ;
    break;}
case 152:
{ yyval.intval = AssignementNode::ASSIGN_DIV; ;
    break;}
case 153:
{ yyval.intval = AssignementNode::ASSIGN_MOD; ;
    break;}
case 154:
{ yyval.intval = AssignementNode::ASSIGN_ADD; ;
    break;}
case 155:
{ yyval.intval = AssignementNode::ASSIGN_SUB; ;
    break;}
case 156:
{ yyval.intval = AssignementNode::ASSIGN_LEFT; ;
    break;}
case 157:
{ yyval.intval = AssignementNode::ASSIGN_RIGHT; ;
    break;}
case 158:
{ yyval.intval = AssignementNode::ASSIGN_AND; ;
    break;}
case 159:
{ yyval.intval = AssignementNode::ASSIGN_XOR; ;
    break;}
case 160:
{ yyval.intval = AssignementNode::ASSIGN_OR; ;
    break;}
case 161:
{ yyval.node = yyvsp[0].node; ;
    break;}
case 162:
{ yyval.node = new ExprNode(yyvsp[-2].node, yyvsp[0].node); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}

#include <stdio.h>

extern char yytext[];

void yyerror(char* s)
{
  ErrorManager::GetSingleton().logAndRaise(s);
}
