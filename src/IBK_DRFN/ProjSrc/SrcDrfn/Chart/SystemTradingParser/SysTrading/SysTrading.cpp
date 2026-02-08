#include "StdAfx.h"
/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         SysTradingparse
#define yylex           SysTradinglex
#define yyerror         SysTradingerror
#define yylval          SysTradinglval
#define yychar          SysTradingchar
#define yydebug         SysTradingdebug
#define yynerrs         SysTradingnerrs

#define NAME  258
#define BOOL_TYPE  259
#define INT_NUMBER  260
#define DBL_NUMBER  261
#define RESERVED_VAR  262
#define RESERVED_COND  263
#define RESERVED_TEXT  264
#define STRING  265
#define ARRAY  266
#define INPUT  267
#define VARIABLE  268
#define DEF_STRING  269
#define DEF_STRING_SIMPLE  270
#define DEF_STRING_ARRAY  271
#define DEF_NUMERIC  272
#define DEF_NUMERIC_SIMPLE  273
#define DEF_NUMERIC_ARRAY  274
#define DEF_BOOL  275
#define DEF_BOOL_SIMPLE  276
#define DEF_BOOL_ARRAY  277
#define JMCODE  278
#define CLOSE  279
#define OPEN  280
#define HIGH  281
#define LOW  282
#define VOLUME  283
#define AMOUNT  284
#define DATE  285
#define TIME  286
#define DATACNT  287
#define CURRENTDATE  288
#define CURRENTTIME  289
#define CURRENTBAR  290
#define STMT_SUFFIX  291
#define COMMA  292
#define EQ  293
#define NE  294
#define LT  295
#define LE  296
#define GT  297
#define GE  298
#define PLUS  299
#define MINUS  300
#define MULT  301
#define DIVIDE  302
#define LPAREN  303
#define RPAREN  304
#define LBRAC  305
#define RBRAC  306
#define COLON  307
#define SEMICOLON  308
#define STAR  309
#define STARSTAR  310
#define UPARROW  311
#define AND  312
#define BBEGIN  313
#define BREAK  314
#define DO  315
#define DOWN  316
#define DOWNTO  317
#define ELSE  318
#define ELSEIF  319
#define BEND  320
#define FOR  321
#define FUNCTION  322
#define IF  323
#define OR  324
#define MOD  325
#define NOT  326
#define THEN  327
#define TO  328
#define WHILE  329
#define CROSS  330
#define ABOVE  331
#define BELOW  332

/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "SysTrading.y"

//#include <iostream.h>
#include "DeclareDefine.h"
#include "SysTradingParser.h"

extern int SysTradingerror(const char* s);
extern int SysTradinglex();

SysTradingParser* g_pSysTradingParser;


/* Line 371 of yacc.c  */
#line 87 "SysTrading.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "SysTrading.hpp".  */
#ifndef YY_SYSTRADING_SYSTRADING_HPP_INCLUDED
# define YY_SYSTRADING_SYSTRADING_HPP_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int SysTradingdebug;
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 13 "SysTrading.y"

			char	name[NAME_SZ];
			int		ival;
			double	dval;
			EXPR	expr;
		

/* Line 387 of yacc.c  */
#line 215 "SysTrading.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE SysTradinglval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int SysTradingparse (void *YYPARSE_PARAM);
#else
int SysTradingparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int SysTradingparse (void);
#else
int SysTradingparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_SYSTRADING_SYSTRADING_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 243 "SysTrading.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  108
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1034

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  221
/* YYNRULES -- Number of states.  */
#define YYNSTATES  386

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   332

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    11,    14,    17,    20,
      23,    26,    29,    32,    35,    38,    41,    44,    47,    48,
      50,    55,    57,    61,    63,    66,    68,    70,    73,    77,
      80,    84,    88,    93,    98,   103,   105,   108,   112,   117,
     119,   123,   127,   129,   132,   136,   140,   144,   148,   152,
     156,   158,   161,   163,   165,   167,   171,   175,   179,   183,
     187,   191,   193,   197,   199,   201,   203,   205,   207,   209,
     211,   213,   215,   218,   221,   225,   227,   229,   231,   233,
     238,   243,   245,   250,   252,   257,   262,   264,   268,   271,
     273,   278,   283,   288,   290,   292,   294,   296,   298,   300,
     302,   307,   309,   314,   316,   321,   323,   328,   330,   335,
     337,   342,   344,   349,   351,   356,   358,   360,   362,   364,
     368,   372,   376,   380,   384,   388,   392,   396,   400,   404,
     408,   412,   416,   420,   424,   428,   432,   436,   440,   444,
     449,   451,   455,   457,   465,   467,   471,   473,   475,   478,
     482,   485,   489,   493,   498,   504,   510,   517,   523,   530,
     532,   534,   536,   540,   544,   548,   552,   556,   560,   562,
     565,   568,   571,   574,   576,   580,   582,   586,   593,   600,
     604,   608,   611,   612,   624,   625,   627,   631,   632,   634,
     638,   639,   641,   645,   650,   652,   654,   656,   658,   660,
     662,   664,   666,   668,   675,   683,   684,   692,   699,   707,
     708,   716,   719,   721,   724,   726,   730,   732,   736,   741,
     746,   751
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      79,     0,    -1,    80,    -1,    79,    80,    -1,    -1,    81,
      -1,    82,    81,    -1,    97,    81,    -1,   103,    81,    -1,
      89,    81,    -1,   111,    81,    -1,   127,    81,    -1,   124,
      81,    -1,   128,    81,    -1,   129,    81,    -1,   130,    81,
      -1,   131,    81,    -1,   132,    81,    -1,    -1,    36,    -1,
      12,    52,    83,    53,    -1,    85,    -1,    83,    37,    85,
      -1,   121,    -1,    83,    85,    -1,    84,    -1,     3,    -1,
       3,    48,    -1,     3,    48,   100,    -1,     3,    49,    -1,
       3,   100,    49,    -1,     3,    48,    49,    -1,     3,    48,
     100,    49,    -1,     3,    48,    89,    49,    -1,     3,    48,
     102,    49,    -1,    87,    -1,    87,    53,    -1,    86,    69,
      87,    -1,    86,    69,    87,    53,    -1,    88,    -1,    87,
      57,    88,    -1,    48,    86,    49,    -1,    90,    -1,    71,
      90,    -1,    90,    38,    90,    -1,    90,    39,    90,    -1,
      90,    40,    90,    -1,    90,    41,    90,    -1,    90,    42,
      90,    -1,    90,    43,    90,    -1,    90,    -1,    90,    53,
      -1,   133,    -1,   134,    -1,    91,    -1,    48,    90,    49,
      -1,    90,    44,    91,    -1,    90,    45,    91,    -1,    90,
      70,    91,    -1,    90,    46,    91,    -1,    90,    47,    91,
      -1,    92,    -1,    48,    91,    49,    -1,   110,    -1,    10,
      -1,    32,    -1,     4,    -1,    93,    -1,    94,    -1,    95,
      -1,    96,    -1,   115,    -1,    44,    89,    -1,    45,    89,
      -1,    48,    89,    49,    -1,   133,    -1,   134,    -1,   101,
      -1,     3,    -1,     3,    50,   100,    51,    -1,     3,    50,
     113,    51,    -1,     7,    -1,     7,    50,    89,    51,    -1,
       8,    -1,     8,    50,    89,    51,    -1,    13,    52,    98,
      53,    -1,    99,    -1,    98,    37,    99,    -1,    98,    99,
      -1,    84,    -1,     3,    48,   100,    49,    -1,     3,    48,
      89,    49,    -1,     3,    48,    93,    49,    -1,     5,    -1,
       6,    -1,    10,    -1,     4,    -1,   101,    -1,   102,    -1,
      25,    -1,    25,    50,    89,    51,    -1,    26,    -1,    26,
      50,    89,    51,    -1,    27,    -1,    27,    50,    89,    51,
      -1,    24,    -1,    24,    50,    89,    51,    -1,    28,    -1,
      28,    50,    89,    51,    -1,    29,    -1,    29,    50,    89,
      51,    -1,    30,    -1,    30,    50,    89,    51,    -1,    31,
      -1,    31,    50,    89,    51,    -1,    23,    -1,    33,    -1,
      34,    -1,    35,    -1,   100,    44,   100,    -1,   100,    45,
     100,    -1,   100,    46,   100,    -1,   100,    47,   100,    -1,
     100,    70,   100,    -1,   101,    44,   101,    -1,   101,    45,
     101,    -1,   101,    46,   101,    -1,   101,    47,   101,    -1,
     101,    70,   101,    -1,   101,    44,   100,    -1,   101,    45,
     100,    -1,   101,    46,   100,    -1,   101,    47,   100,    -1,
     101,    70,   100,    -1,   100,    44,   101,    -1,   100,    45,
     101,    -1,   100,    46,   101,    -1,   100,    47,   101,    -1,
     100,    70,   101,    -1,    11,    52,   104,    53,    -1,   105,
      -1,   105,    37,   105,    -1,   108,    -1,     3,    50,   106,
      51,    48,   100,    49,    -1,   107,    -1,   106,    37,   107,
      -1,     5,    -1,    84,    -1,     3,    50,    -1,     3,    50,
     106,    -1,     3,    51,    -1,     3,   106,    51,    -1,     3,
      51,    51,    -1,     3,    50,   106,    51,    -1,     3,    50,
     106,    51,    48,    -1,     3,    50,   106,    51,   100,    -1,
       3,    50,   106,    51,   100,    49,    -1,     3,    50,   106,
      51,    49,    -1,     3,    50,   106,    51,    48,    49,    -1,
       5,    -1,     6,    -1,   109,    -1,    93,    38,    89,    -1,
      93,    38,    86,    -1,    94,    38,    89,    -1,    94,    38,
      86,    -1,    95,    38,    89,    -1,    96,    38,    86,    -1,
     112,    -1,    93,    38,    -1,    94,    38,    -1,    38,    89,
      -1,    38,    86,    -1,   114,    -1,   113,    37,   114,    -1,
      89,    -1,   116,   115,    49,    -1,   116,   119,    49,    50,
     100,    51,    -1,   116,   119,    49,    50,   113,    51,    -1,
     116,   119,    49,    -1,   117,   119,    49,    -1,     3,    48,
      -1,    -1,     3,    50,   100,    51,    48,   118,     3,    50,
     113,    51,    48,    -1,    -1,    86,    -1,   120,    37,    86,
      -1,    -1,    86,    -1,   120,    37,    86,    -1,    -1,   122,
      -1,   121,    37,   122,    -1,     3,    48,   123,    49,    -1,
      14,    -1,    15,    -1,    16,    -1,    17,    -1,    18,    -1,
      19,    -1,    20,    -1,    21,    -1,    22,    -1,    66,    93,
      38,    89,    62,    89,    -1,    66,    93,    38,    89,    61,
      73,    89,    -1,    -1,    66,    93,    38,    89,    73,    89,
     125,    -1,    66,    94,    38,    89,    62,    89,    -1,    66,
      94,    38,    89,    61,    73,    89,    -1,    -1,    66,    94,
      38,    89,    73,    89,   126,    -1,    59,    53,    -1,    58,
      -1,    65,    53,    -1,    65,    -1,    68,    86,    72,    -1,
      63,    -1,    64,    86,    72,    -1,    93,    75,    76,    89,
      -1,    94,    75,    76,    89,    -1,    93,    75,    77,    89,
      -1,    94,    75,    77,    89,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   123,   123,   127,   129,   130,   131,   132,   133,   134,
     145,   156,   157,   161,   165,   172,   177,   182,   188,   189,
     192,   195,   196,   197,   198,   204,   207,   213,   219,   225,
     231,   237,   245,   252,   260,   270,   278,   288,   296,   308,
     312,   322,   330,   334,   342,   350,   358,   366,   374,   382,
     392,   399,   406,   407,   411,   415,   421,   434,   447,   460,
     473,   488,   492,   499,   503,   507,   511,   515,   523,   531,
     535,   539,   543,   547,   555,   563,   567,   571,   577,   585,
     589,   761,   765,   772,   776,   793,   796,   797,   798,   804,
     807,   814,   822,   833,   838,   843,   848,   853,   854,   862,
     871,   880,   889,   898,   907,   916,   925,   934,   943,   952,
     961,   970,   979,   988,   997,  1006,  1015,  1024,  1033,  1045,
    1055,  1065,  1075,  1085,  1095,  1104,  1113,  1122,  1131,  1140,
    1152,  1164,  1176,  1188,  1200,  1209,  1218,  1227,  1236,  1247,
    1250,  1251,  1252,  1256,  1266,  1270,  1276,  1282,  1283,  1289,
    1295,  1301,  1307,  1313,  1319,  1325,  1331,  1337,  1343,  1351,
    1355,  1361,  1367,  1379,  1392,  1399,  1406,  1410,  1414,  1417,
    1423,  1429,  1435,  1444,  1449,  1456,  1462,  1469,  1478,  1487,
    1494,  1503,  1515,  1514,  1532,  1537,  1545,  1555,  1560,  1568,
    1579,  1582,  1586,  1596,  1606,  1611,  1617,  1623,  1628,  1634,
    1640,  1645,  1651,  1660,  1661,  1662,  1662,  1675,  1676,  1677,
    1677,  1688,  1692,  1700,  1707,  1717,  1729,  1740,  1747,  1760,
    1770,  1783
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "BOOL_TYPE", "INT_NUMBER",
  "DBL_NUMBER", "RESERVED_VAR", "RESERVED_COND", "RESERVED_TEXT", "STRING",
  "ARRAY", "INPUT", "VARIABLE", "DEF_STRING", "DEF_STRING_SIMPLE",
  "DEF_STRING_ARRAY", "DEF_NUMERIC", "DEF_NUMERIC_SIMPLE",
  "DEF_NUMERIC_ARRAY", "DEF_BOOL", "DEF_BOOL_SIMPLE", "DEF_BOOL_ARRAY",
  "JMCODE", "CLOSE", "OPEN", "HIGH", "LOW", "VOLUME", "AMOUNT", "DATE",
  "TIME", "DATACNT", "CURRENTDATE", "CURRENTTIME", "CURRENTBAR",
  "STMT_SUFFIX", "COMMA", "EQ", "NE", "LT", "LE", "GT", "GE", "PLUS",
  "MINUS", "MULT", "DIVIDE", "LPAREN", "RPAREN", "LBRAC", "RBRAC", "COLON",
  "SEMICOLON", "STAR", "STARSTAR", "UPARROW", "AND", "BBEGIN", "BREAK",
  "DO", "DOWN", "DOWNTO", "ELSE", "ELSEIF", "BEND", "FOR", "FUNCTION",
  "IF", "OR", "MOD", "NOT", "THEN", "TO", "WHILE", "CROSS", "ABOVE",
  "BELOW", "$accept", "stmt_list", "stmt", "stmt_suffix", "input_stmt",
  "input_list", "Declare_Error", "input_list2", "cond_expr", "cond_expr2",
  "cond_expr3", "expr", "expr2", "expr3", "expr4", "name_expr",
  "array_expr", "reserved_arr", "reserved_cond", "variable_stmt",
  "variable_list", "variable_list2", "data_list", "reserved_list",
  "expr_list", "array_stmt", "array_list", "array_list2", "array_list3",
  "array_list4", "Array_Error", "num_list", "num_expr", "assign_expr",
  "Assign_Error", "array_idx_list", "array_idx_list2", "function",
  "function_name", "function_name2", "@1", "func_arg_list",
  "func_arg_list2", "def_arg_list", "def_arg_list2", "data_type_list",
  "for_start", "$@2", "$@3", "loop_break", "block_begin", "block_end",
  "if_start", "if_else", "if_else_if", "cross_above", "cross_below", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    78,    79,    79,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    81,    81,
      82,    83,    83,    83,    83,    83,    84,    84,    84,    84,
      84,    84,    85,    85,    85,    86,    86,    86,    86,    87,
      87,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      89,    89,    89,    89,    90,    90,    90,    90,    90,    90,
      90,    91,    91,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    93,    94,
      94,    95,    95,    96,    96,    97,    98,    98,    98,    98,
      99,    99,    99,   100,   100,   100,   100,   100,   100,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   103,
     104,   104,   104,   105,   106,   106,   107,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   109,
     109,   110,   111,   111,   111,   111,   111,   111,   111,   112,
     112,   112,   112,   113,   113,   114,   115,   115,   115,   115,
     115,   116,   118,   117,   119,   119,   119,   120,   120,   120,
     121,   121,   121,   122,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   124,   124,   125,   124,   124,   124,   126,
     124,   127,   128,   129,   129,   130,   131,   132,   133,   133,
     134,   134
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       4,     1,     3,     1,     2,     1,     1,     2,     3,     2,
       3,     3,     4,     4,     4,     1,     2,     3,     4,     1,
       3,     3,     1,     2,     3,     3,     3,     3,     3,     3,
       1,     2,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     1,     1,     1,     1,     4,
       4,     1,     4,     1,     4,     4,     1,     3,     2,     1,
       4,     4,     4,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     4,     1,     4,     1,     4,     1,     4,     1,
       4,     1,     4,     1,     4,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       1,     3,     1,     7,     1,     3,     1,     1,     2,     3,
       2,     3,     3,     4,     5,     5,     6,     5,     6,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     1,     2,
       2,     2,     2,     1,     3,     1,     3,     6,     6,     3,
       3,     2,     0,    11,     0,     1,     3,     0,     1,     3,
       0,     1,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     7,     0,     7,     6,     7,     0,
       7,     2,     1,     2,     1,     3,     1,     3,     4,     4,
       4,     4
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,    78,    66,   159,   160,    81,    83,    64,     0,     0,
       0,   115,   105,    99,   101,   103,   107,   109,   111,   113,
      65,   116,   117,   118,    19,     0,     0,     0,     0,   212,
       0,   216,     0,   214,     0,     0,     0,     2,     5,    18,
      18,    50,    54,    61,    67,    68,    69,    70,    18,    77,
      18,   161,    63,    18,   168,    71,   184,   184,    18,    18,
      18,    18,    18,    18,    18,    52,    53,   181,     0,     0,
       0,     0,   190,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   172,    35,    39,   171,    42,    67,
      68,    69,    70,    72,    73,     0,     0,    54,   211,     0,
      42,    75,    76,   213,    78,     0,     0,     0,     1,     3,
       6,     9,     0,     0,     0,     0,    51,     0,   169,     0,
     170,     0,     0,     0,     7,     8,    10,   185,    71,     0,
       0,     0,    12,    11,    13,    14,    15,    16,    17,    66,
      93,    94,    64,   175,     0,    77,    98,     0,   173,     0,
       0,    26,   147,     0,   140,   142,    26,     0,    25,    21,
      23,   191,    26,    89,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    42,    43,     0,    36,     0,
       0,     0,     0,     0,     0,     0,    74,    55,    62,   217,
       0,     0,     0,   215,     0,    56,    57,    59,    60,    58,
     163,   162,     0,     0,   165,   164,     0,     0,   166,   167,
     176,   179,     0,   180,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,    80,    82,    84,    96,
      93,    94,    95,    27,    29,   148,   150,     0,    97,     0,
     144,   139,     0,    93,    27,     0,     0,    20,    24,     0,
      27,     0,     0,    85,    88,   106,   100,   102,   104,   108,
     110,   112,   114,    41,    37,    40,    44,    45,    46,    47,
      48,    49,     0,     0,     0,   218,   220,   219,   221,     0,
     186,   119,    97,   120,    97,   121,    97,   122,    97,   182,
     123,    97,   129,    97,   130,    97,   131,    97,   132,    97,
     133,    97,   174,    31,    28,   146,   149,   152,    30,     0,
     151,     0,   141,   194,   195,   196,   197,   198,   199,   200,
     201,   202,     0,    28,    98,     0,     0,    22,     0,   192,
       0,    67,    28,     0,    87,    38,    79,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   153,   145,     0,    33,
      32,    34,   193,     0,     0,    91,    92,    90,     0,     0,
     203,   205,     0,   207,   209,   177,   178,     0,   154,   157,
     155,     0,   204,   206,   208,   210,     0,   158,     0,   156,
       0,     0,   143,     0,     0,   183
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    36,    37,    38,    39,   157,   152,   159,   127,    85,
      86,   143,    41,    42,    43,    89,    90,    91,    92,    48,
     164,   165,   237,    49,   146,    50,   153,   154,   239,   240,
     155,    51,    52,    53,    54,   147,   148,    55,    56,    57,
     345,   129,   130,   160,   161,   325,    58,   373,   375,    59,
      60,    61,    62,    63,    64,    65,    66
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -273
static const yytype_int16 yypact[] =
{
     599,    69,  -273,  -273,  -273,   -16,    -8,  -273,   -31,    -2,
      12,  -273,    39,    60,    64,    73,    97,   113,   140,   149,
    -273,  -273,  -273,  -273,  -273,   550,   731,   731,   731,  -273,
     -30,  -273,   550,    92,    70,   550,   455,  -273,  -273,   168,
     168,   260,  -273,  -273,   -28,   -24,   171,   172,   168,  -273,
     168,  -273,  -273,   168,  -273,  -273,   501,   501,   168,   168,
     168,   168,   168,   168,   168,  -273,  -273,  -273,   764,   731,
     731,   208,   211,   212,   731,   731,   731,   731,   731,   731,
     731,   731,   550,   731,   147,   -14,  -273,  -273,   130,   146,
     153,  -273,  -273,  -273,  -273,   182,   188,   189,  -273,    23,
     231,  -273,  -273,  -273,   190,   201,   204,    43,  -273,  -273,
    -273,  -273,   797,   797,   797,   797,  -273,   797,   550,    67,
     550,   105,   731,   550,  -273,  -273,  -273,   -20,   194,   196,
     209,   199,  -273,  -273,  -273,  -273,  -273,  -273,  -273,   246,
    -273,  -273,   249,  -273,    95,    13,  -273,   -13,  -273,   203,
     206,   390,  -273,   226,   222,  -273,   829,     8,  -273,  -273,
     224,  -273,   861,  -273,     9,  -273,   230,   232,   234,   235,
     237,   239,   241,   243,   -40,   923,   142,   550,  -273,   550,
     731,   731,   731,   731,   731,   731,  -273,  -273,  -273,  -273,
     764,   731,   731,  -273,   731,  -273,  -273,  -273,  -273,  -273,
     147,  -273,   731,   731,   147,  -273,   731,   731,  -273,   147,
    -273,   245,   550,  -273,   973,   973,   973,   973,   214,   973,
     973,   973,   973,   973,   973,   731,  -273,  -273,  -273,  -273,
      17,  -273,  -273,   103,  -273,   293,   248,   115,   523,    35,
    -273,  -273,   307,  -273,   665,   255,   308,  -273,  -273,   309,
     698,   266,   312,  -273,  -273,  -273,  -273,  -273,  -273,  -273,
    -273,  -273,  -273,  -273,    31,  -273,   142,   142,   142,   142,
     142,   142,   300,   -29,   -21,  -273,  -273,  -273,  -273,   764,
      -6,   544,   523,   544,   523,   544,   523,   544,   523,  -273,
     544,   523,   544,   523,   544,   523,   544,   523,   544,   523,
     544,   523,  -273,  -273,   544,  -273,    54,  -273,  -273,   293,
    -273,   268,  -273,  -273,  -273,  -273,  -273,  -273,  -273,  -273,
    -273,  -273,   267,   219,   270,   271,   764,  -273,   275,  -273,
     276,   -27,   357,   764,  -273,  -273,  -273,   251,   731,   731,
     254,   731,   731,   425,    74,   325,   893,  -273,   293,  -273,
    -273,  -273,  -273,   219,   634,  -273,  -273,  -273,   357,   731,
    -273,  -273,   731,  -273,  -273,  -273,  -273,   281,   925,  -273,
     495,    84,  -273,  -273,  -273,  -273,   731,  -273,   517,  -273,
     284,   129,  -273,   973,   289,  -273
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -273,  -273,   304,   970,  -273,  -273,   124,  -153,   -17,   166,
     181,     0,   173,   -15,  -273,     1,    19,    20,    30,  -273,
    -273,  -158,     3,   -66,  -239,  -273,  -273,   119,  -232,    59,
    -273,  -273,  -273,  -273,  -273,  -272,   138,   316,  -273,  -273,
    -273,   317,  -273,  -273,   127,  -273,  -273,  -273,  -273,  -273,
    -273,  -273,  -273,  -273,  -273,   252,   265
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -190
static const yytype_int16 yytable[] =
{
      40,    44,   145,   306,   248,   324,   254,   344,    84,   263,
     118,   245,   251,    97,   120,    99,   -97,  -188,   107,    45,
      46,    71,   356,    98,   225,    87,    93,    94,    95,   177,
      47,  -189,   337,   338,    69,   105,    40,    44,   226,   178,
     340,   341,    70,   179,   339,   246,   252,   119,   119,   177,
      72,   121,   342,   106,  -146,    45,    46,   220,   221,   222,
     223,   247,   253,   177,    73,   174,    47,    97,  -146,   149,
     150,   144,   309,   104,   166,   167,   168,   169,   170,   171,
     172,   173,    95,   224,   335,   238,   310,   324,   179,    74,
     238,   309,   177,   327,   334,   189,   238,   195,   196,   197,
     198,   200,   199,   204,   381,   346,   209,   229,   243,   231,
      75,   225,   177,   232,    76,   193,   371,    67,   201,    68,
     205,   309,   208,    77,   145,   366,    11,    12,    13,    14,
      15,    16,    17,    18,    19,   380,    21,    22,    23,   214,
     215,   216,   217,   202,   203,   103,   218,    78,   282,   284,
     286,   288,   303,   291,   293,   295,   297,   299,   301,   214,
     215,   216,   217,    79,   308,   219,   225,   238,   180,   181,
     182,   183,   184,   185,   112,   113,   114,   115,   145,    97,
     384,   206,   207,   116,   145,   219,   112,   113,   114,   115,
      80,   273,   274,   272,    95,   280,   158,   163,    88,    81,
     117,    96,   275,   276,    24,   100,   277,   278,   100,   122,
     123,   151,   117,   145,   156,   162,   177,   281,   283,   285,
     287,   119,   290,   292,   294,   296,   298,   300,   121,   100,
     100,   186,   112,   113,   114,   115,   304,   187,   188,   191,
     190,   116,   192,   210,   322,   211,   212,   323,   213,   -96,
     330,   331,   -95,   332,   227,   175,   176,   228,   117,   242,
     145,   249,   289,   214,   215,   216,   217,   145,   350,   180,
     181,   182,   183,   184,   185,   112,   113,   114,   115,   241,
     238,   255,   343,   256,   101,   257,   258,   101,   259,   219,
     260,    88,   261,    88,   262,   279,   100,   102,   305,   307,
     102,   117,   238,   326,   112,   113,   114,   115,   101,   101,
     311,   245,   328,   116,   333,   251,   349,   238,   348,   351,
     352,   102,   102,   354,   359,   355,   322,   362,   367,   353,
     117,   376,   383,   330,   331,   101,   358,   385,   360,   361,
     109,   363,   364,   264,   214,   215,   216,   217,   102,   370,
     100,   336,   100,   266,   267,   268,   269,   270,   271,   372,
     265,   312,   374,   302,   101,   101,   101,   101,   347,   101,
     219,   378,   128,     0,   131,   101,   329,   102,   102,   102,
     102,     0,   102,     0,     0,   100,   378,     0,   102,     0,
       0,     0,     0,     0,   229,   230,   231,     0,     0,     0,
     232,   214,   215,   216,   217,     0,   357,     0,     0,     0,
       0,     0,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,     0,    21,    22,    23,     0,   219,     0,   101,
       0,   101,   101,   101,   101,   101,   101,   101,   233,   234,
     235,   236,   102,     0,   102,   102,   102,   102,   102,   102,
     102,     0,     0,     0,     0,   108,     0,     0,     1,     2,
       3,     4,     5,     6,   101,     7,     8,     9,    10,   214,
     215,   216,   217,     0,     0,     0,   365,   102,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,     0,   219,     0,     0,     0,    26,
      27,     0,     0,    28,     1,     2,     3,     4,     5,     6,
       0,     7,     0,    29,    30,     0,     0,     0,    31,    32,
      33,    34,     0,    35,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,  -187,   214,
     215,   216,   217,     0,   379,    26,    27,     0,     0,    82,
       0,     0,     0,     1,     2,     3,     4,     5,     6,     0,
       7,   214,   215,   216,   217,   219,   382,   220,   221,   222,
     223,     0,    83,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,     0,   219,   214,   215,
     216,   217,     0,   224,    26,    27,     0,     0,    82,     0,
       0,     0,     1,     2,     3,     4,     5,     6,     0,     7,
       8,     9,    10,     0,   219,     0,     0,     0,     0,     0,
       0,    83,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,     0,     0,
       0,     0,     0,    26,    27,     0,     0,    28,   313,   314,
     315,   316,   317,   318,   319,   320,   321,    29,    30,     0,
       0,     0,    31,    32,    33,    34,     0,    35,     1,   139,
     140,   141,     5,     6,     0,   142,     0,     0,     0,   313,
     314,   315,   316,   317,   318,   319,   320,   321,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     1,   139,   140,   141,     5,     6,     0,   142,    26,
      27,     0,     0,    28,   303,     0,     0,     0,     0,     0,
       0,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     1,     2,     3,     4,     5,     6,
       0,     7,    26,    27,     0,     0,    28,   303,     0,     0,
       0,     0,     0,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     1,   139,   140,
     141,     5,     6,     0,   142,    26,    27,     0,     0,    28,
       0,     0,     0,     0,     0,     0,     0,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       1,     2,     3,     4,     5,     6,     0,     7,    26,    27,
       0,     0,    28,     0,     0,     0,     0,     0,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,   229,   243,   231,     0,     0,     0,   232,
       0,    26,    27,     0,     0,   194,     0,     0,     0,     0,
       0,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,     0,    21,    22,    23,   229,   243,   231,     0,     0,
       0,   232,     0,     0,     0,     0,     0,   244,   234,     0,
       0,     0,     0,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,     0,    21,    22,    23,   229,   243,   231,
       0,     0,     0,   232,     0,     0,     0,     0,     0,   250,
     234,     0,     0,     0,     0,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,     0,    21,    22,    23,   229,
     243,   231,     0,     0,     0,   232,     0,     0,     0,     0,
       0,   368,   369,     0,     0,     0,     0,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,     0,    21,    22,
      23,   180,   181,   182,   183,   184,   185,   112,   113,   114,
     115,     0,   187,     0,   377,     0,   116,   229,   243,   231,
       0,     0,     0,   232,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   117,     0,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,     0,    21,    22,    23,   110,
     111,     0,     0,     0,     0,     0,     0,     0,   124,     0,
     125,     0,     0,   126,     0,     0,     0,     0,   132,   133,
     134,   135,   136,   137,   138
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-273)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       0,     0,    68,   235,   157,   244,   164,   279,    25,    49,
      38,     3,     3,    28,    38,    32,     3,    37,    35,     0,
       0,    52,    49,    53,    37,    25,    26,    27,    28,    69,
       0,    37,    61,    62,    50,    34,    36,    36,    51,    53,
      61,    62,    50,    57,    73,    37,    37,    75,    75,    69,
      52,    75,    73,    34,    37,    36,    36,    44,    45,    46,
      47,    53,    53,    69,    52,    82,    36,    82,    51,    69,
      70,    68,    37,     3,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    70,    53,   151,    51,   326,    57,    50,
     156,    37,    69,   246,   252,    72,   162,   112,   113,   114,
     115,   118,   117,   120,   376,    51,   123,     4,     5,     6,
      50,    37,    69,    10,    50,    72,   348,    48,   118,    50,
     120,    37,   122,    50,   190,    51,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    51,    33,    34,    35,    44,
      45,    46,    47,    76,    77,    53,    51,    50,   214,   215,
     216,   217,    49,   219,   220,   221,   222,   223,   224,    44,
      45,    46,    47,    50,    49,    70,    37,   233,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,   244,   194,
      51,    76,    77,    53,   250,    70,    44,    45,    46,    47,
      50,   191,   192,   190,   194,   212,    72,    73,    25,    50,
      70,    28,   202,   203,    36,    32,   206,   207,    35,    38,
      38,     3,    70,   279,     3,     3,    69,   214,   215,   216,
     217,    75,   219,   220,   221,   222,   223,   224,    75,    56,
      57,    49,    44,    45,    46,    47,   233,    49,    49,    38,
      50,    53,    38,    49,   244,    49,    37,   244,    49,     3,
     250,   250,     3,   250,    51,    82,    83,    51,    70,    37,
     326,    37,    48,    44,    45,    46,    47,   333,    49,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    53,
     346,    51,   279,    51,    32,    51,    51,    35,    51,    70,
      51,   118,    51,   120,    51,    50,   123,    32,     5,    51,
      35,    70,   368,    48,    44,    45,    46,    47,    56,    57,
       3,     3,     3,    53,    48,     3,    49,   383,    50,    49,
      49,    56,    57,    48,    73,    49,   326,    73,     3,   326,
      70,    50,    48,   333,   333,    83,   333,    48,   338,   339,
      36,   341,   342,   177,    44,    45,    46,    47,    83,   346,
     177,    51,   179,   180,   181,   182,   183,   184,   185,   359,
     179,   242,   362,   225,   112,   113,   114,   115,   309,   117,
      70,   368,    56,    -1,    57,   123,   249,   112,   113,   114,
     115,    -1,   117,    -1,    -1,   212,   383,    -1,   123,    -1,
      -1,    -1,    -1,    -1,     4,     5,     6,    -1,    -1,    -1,
      10,    44,    45,    46,    47,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    35,    -1,    70,    -1,   177,
      -1,   179,   180,   181,   182,   183,   184,   185,    48,    49,
      50,    51,   177,    -1,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,     0,    -1,    -1,     3,     4,
       5,     6,     7,     8,   212,    10,    11,    12,    13,    44,
      45,    46,    47,    -1,    -1,    -1,    51,   212,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    -1,    70,    -1,    -1,    -1,    44,
      45,    -1,    -1,    48,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    58,    59,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    68,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    37,    44,
      45,    46,    47,    -1,    49,    44,    45,    -1,    -1,    48,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      10,    44,    45,    46,    47,    70,    49,    44,    45,    46,
      47,    -1,    71,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    70,    44,    45,
      46,    47,    -1,    70,    44,    45,    -1,    -1,    48,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    10,
      11,    12,    13,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    48,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    58,    59,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    68,     3,     4,
       5,     6,     7,     8,    -1,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,     3,     4,     5,     6,     7,     8,    -1,    10,    44,
      45,    -1,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,     3,     4,     5,     6,     7,     8,
      -1,    10,    44,    45,    -1,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,     3,     4,     5,
       6,     7,     8,    -1,    10,    44,    45,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
       3,     4,     5,     6,     7,     8,    -1,    10,    44,    45,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,     4,     5,     6,    -1,    -1,    -1,    10,
      -1,    44,    45,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    35,     4,     5,     6,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    35,     4,     5,     6,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    35,     4,
       5,     6,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      35,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    -1,    49,    -1,    53,     4,     5,     6,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    35,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    58,    59,
      60,    61,    62,    63,    64
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    10,    11,    12,
      13,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    38,    44,    45,    48,    58,
      59,    63,    64,    65,    66,    68,    79,    80,    81,    82,
      89,    90,    91,    92,    93,    94,    95,    96,    97,   101,
     103,   109,   110,   111,   112,   115,   116,   117,   124,   127,
     128,   129,   130,   131,   132,   133,   134,    48,    50,    50,
      50,    52,    52,    52,    50,    50,    50,    50,    50,    50,
      50,    50,    48,    71,    86,    87,    88,    89,    90,    93,
      94,    95,    96,    89,    89,    89,    90,    91,    53,    86,
      90,   133,   134,    53,     3,    93,    94,    86,     0,    80,
      81,    81,    44,    45,    46,    47,    53,    70,    38,    75,
      38,    75,    38,    38,    81,    81,    81,    86,   115,   119,
     120,   119,    81,    81,    81,    81,    81,    81,    81,     4,
       5,     6,    10,    89,   100,   101,   102,   113,   114,    89,
      89,     3,    84,   104,   105,   108,     3,    83,    84,    85,
     121,   122,     3,    84,    98,    99,    89,    89,    89,    89,
      89,    89,    89,    89,    86,    90,    90,    69,    53,    57,
      38,    39,    40,    41,    42,    43,    49,    49,    49,    72,
      50,    38,    38,    72,    48,    91,    91,    91,    91,    91,
      86,    89,    76,    77,    86,    89,    76,    77,    89,    86,
      49,    49,    37,    49,    44,    45,    46,    47,    51,    70,
      44,    45,    46,    47,    70,    37,    51,    51,    51,     4,
       5,     6,    10,    48,    49,    50,    51,   100,   101,   106,
     107,    53,    37,     5,    48,     3,    37,    53,    85,    37,
      48,     3,    37,    53,    99,    51,    51,    51,    51,    51,
      51,    51,    51,    49,    87,    88,    90,    90,    90,    90,
      90,    90,   100,    89,    89,    89,    89,    89,    89,    50,
      86,   100,   101,   100,   101,   100,   101,   100,   101,    48,
     100,   101,   100,   101,   100,   101,   100,   101,   100,   101,
     100,   101,   114,    49,   100,     5,   106,    51,    49,    37,
      51,     3,   105,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    89,   100,   102,   123,    48,    85,     3,   122,
      89,    93,   100,    48,    99,    53,    51,    61,    62,    73,
      61,    62,    73,   100,   113,   118,    51,   107,    50,    49,
      49,    49,    49,   100,    48,    49,    49,    49,   100,    73,
      89,    89,    73,    89,    89,    51,    51,     3,    48,    49,
     100,   106,    89,   125,    89,   126,    50,    49,   100,    49,
      51,   113,    49,    48,    51,    48
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 124 "SysTrading.y"
    {
//		g_pSysTradingParser->WriteOneStatementEnd($1.sStr);
	}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 135 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->IsEndSentence(SysTradinglval.name);
		if(nError)
			return nError;

		// Write the general statement.
		nError = g_pSysTradingParser->WriteGeneralSentence((yyvsp[(1) - (2)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 146 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->IsEndSentence(SysTradinglval.name);
		if(nError)
			return nError;

		// Write the assign statement.
		nError = g_pSysTradingParser->WriteAssignSentence((yyvsp[(1) - (2)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 156 "SysTrading.y"
    {}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 158 "SysTrading.y"
    {
		g_pSysTradingParser->AddStack(STACK_FOR);
	}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 162 "SysTrading.y"
    {
		g_pSysTradingParser->ChangeBlockStack();
	}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 166 "SysTrading.y"
    {
//		g_pSysTradingParser->IsEndBlock(TRUE);
//		int nError = g_pSysTradingParser->EndBlock($1.sStr);
//		if(nError)
//			return nError;
	}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 173 "SysTrading.y"
    {
//		g_pSysTradingParser->IsEndBlock(TRUE);
		g_pSysTradingParser->AddStack(STACK_IF);
	}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 178 "SysTrading.y"
    {
//		g_pSysTradingParser->DelEndBlock();
		g_pSysTradingParser->AddStack(STACK_ELSE);
	}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 183 "SysTrading.y"
    {
//		g_pSysTradingParser->DelEndBlock();
		g_pSysTradingParser->AddStack(STACK_ELSEIF);
	}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 192 "SysTrading.y"
    {}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 195 "SysTrading.y"
    {}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 196 "SysTrading.y"
    {}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 197 "SysTrading.y"
    {}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 199 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(2);
		if(nError)
			return nError;
	}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 204 "SysTrading.y"
    {}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 208 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (1)].name));
		if(nError)
			return nError;
	}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 214 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (2)].name));
		if(nError)
			return nError;
	}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 220 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 226 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (2)].name));
		if(nError)
			return nError;
	}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 232 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 238 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 246 "SysTrading.y"
    {
		// Write the declare sentence.
		int nError = g_pSysTradingParser->WriteDeclareSentence((yyvsp[(3) - (4)].expr).cExprKind, (yyvsp[(1) - (4)].name), &(yyvsp[(3) - (4)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 253 "SysTrading.y"
    {
		(yyvsp[(3) - (4)].expr).cExprKind = DTTYPE_INT;
		// Write the declare sentence.
		int nError = g_pSysTradingParser->WriteDeclareSentence((yyvsp[(3) - (4)].expr).cExprKind, (yyvsp[(1) - (4)].name), &(yyvsp[(3) - (4)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 261 "SysTrading.y"
    {
		(yyvsp[(3) - (4)].expr).cExprKind = DTTYPE_INT;
		// Write the declare sentence.
		int nError = g_pSysTradingParser->WriteDeclareSentence((yyvsp[(3) - (4)].expr).cExprKind, (yyvsp[(1) - (4)].name), &(yyvsp[(3) - (4)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 271 "SysTrading.y"
    {
		// Setting Sentence End Flag
		g_pSysTradingParser->SetEndSentence(FALSE);

		// Parse the condition sentence (Normal)
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 279 "SysTrading.y"
    { 
		// Setting Sentence End Flag
		g_pSysTradingParser->SetEndSentence(TRUE);

		// Parse the condition sentence (Semicolon)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (2)].expr).sStr, NULL, (yyval.expr).sStr, 1);
		if(nError)
			return nError;
	}
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 289 "SysTrading.y"
    { 
		// Parse the condition sentence (Or)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 51);
		if(nError)
			return nError;
//		or( &$$, &$1, &$3); 
	}
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 297 "SysTrading.y"
    { 
		// Parse the condition sentence (Or)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (4)].expr).sStr, (yyvsp[(3) - (4)].expr).sStr, (yyval.expr).sStr, 51);
		if(nError)
			return nError;

		::lstrcat((yyval.expr).sStr, ";");
//		or( &$$, &$1, &$3); strcat($$.sStr, ";");
	}
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 309 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 313 "SysTrading.y"
    { 
		// Parse the condition sentence (And)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 52);
		if(nError)
			return nError;
//		and( &$$, &$1, &$3); 
	}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 323 "SysTrading.y"
    { 
		// Parse the condition sentence (Parenthesis)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(2) - (3)].expr).sStr, NULL, (yyval.expr).sStr, 2);
		(yyval.expr).cExprKind = EXP_COMPLEX;
		if(nError)
			return nError;
	}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 331 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 335 "SysTrading.y"
    { 
		// Parse the condition sentence (Not)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(2) - (2)].expr).sStr, NULL, (yyval.expr).sStr, 50);
		if(nError)
			return nError;
//		not(&$$, &$2); 
	}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 343 "SysTrading.y"
    { 
		// Parse the condition sentence (Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 3);
		if(nError)
			return nError;
//		eq( &$$, &$1, &$3); 
	}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 351 "SysTrading.y"
    { 
		// Parse the condition sentence (Not Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 8);
		if(nError)
			return nError;
//		ne( &$$, &$1, &$3); 
	}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 359 "SysTrading.y"
    { 
		// Parse the condition sentence (Less Then)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 4);
		if(nError)
			return nError;
//		lt( &$$, &$1, &$3); 
	}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 367 "SysTrading.y"
    { 
		// Parse the condition sentence (Less Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 5);
		if(nError)
			return nError;
//		le( &$$, &$1, &$3); 
	}
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 375 "SysTrading.y"
    { 
		// Parse the condition sentence (Greater Then)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 6);
		if(nError)
			return nError;
//		gt( &$$, &$1, &$3); 
	}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 383 "SysTrading.y"
    { 
		// Parse the condition sentence (Greater Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 7);
		if(nError)
			return nError;
//		ge( &$$, &$1, &$3); 
	}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 393 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
		
		// Setting Sentence End Flag
		g_pSysTradingParser->SetEndSentence(FALSE);
	}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 400 "SysTrading.y"
    { 
//		sprintf($$.sStr, "%s;", $1.sStr); 

		// Setting Sentence End Flag
		g_pSysTradingParser->SetEndSentence(TRUE);
	}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 406 "SysTrading.y"
    {}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 407 "SysTrading.y"
    {}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 412 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 416 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(2) - (3)].expr); 
		::wsprintf((yyval.expr).sStr, _T("(%s)"), (yyvsp[(2) - (3)].expr).sStr);
		::wsprintf((yyval.expr).sStrOrg, _T("(%s)"), (yyvsp[(2) - (3)].expr).sStrOrg);
	}
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 422 "SysTrading.y"
    { 
		CString strExpr2 = ::lstrlen((yyvsp[(1) - (3)].expr).sStrOrg) ? (yyvsp[(1) - (3)].expr).sStrOrg : (yyvsp[(1) - (3)].expr).sStr;
		CString strExpr3 = ::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg) ? (yyvsp[(3) - (3)].expr).sStrOrg : (yyvsp[(3) - (3)].expr).sStr;

		::wsprintf((yyval.expr).sStrOrg, _T("%s + %s"), strExpr2, strExpr3);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 1);
		if(nError)
			return nError;
//		plus( &$$, &$1, &$3); 
	}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 435 "SysTrading.y"
    { 
		CString strExpr2 = ::lstrlen((yyvsp[(1) - (3)].expr).sStrOrg) ? (yyvsp[(1) - (3)].expr).sStrOrg : (yyvsp[(1) - (3)].expr).sStr;
		CString strExpr3 = ::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg) ? (yyvsp[(3) - (3)].expr).sStrOrg : (yyvsp[(3) - (3)].expr).sStr;

		::wsprintf((yyval.expr).sStrOrg, _T("%s - %s"), strExpr2, strExpr3);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 2);
		if(nError)
			return nError;
//		minus( &$$, &$1, &$3); 
	}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 448 "SysTrading.y"
    { 
		CString strExpr2 = ::lstrlen((yyvsp[(1) - (3)].expr).sStrOrg) ? (yyvsp[(1) - (3)].expr).sStrOrg : (yyvsp[(1) - (3)].expr).sStr;
		CString strExpr3 = ::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg) ? (yyvsp[(3) - (3)].expr).sStrOrg : (yyvsp[(3) - (3)].expr).sStr;

		::wsprintf((yyval.expr).sStrOrg, _T("%s % %s"), strExpr2, strExpr3);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 3);
		if(nError)
			return nError;
//		mod( &$$, &$1, &$3); 
	}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 461 "SysTrading.y"
    { 
		CString strExpr2 = ::lstrlen((yyvsp[(1) - (3)].expr).sStrOrg) ? (yyvsp[(1) - (3)].expr).sStrOrg : (yyvsp[(1) - (3)].expr).sStr;
		CString strExpr3 = ::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg) ? (yyvsp[(3) - (3)].expr).sStrOrg : (yyvsp[(3) - (3)].expr).sStr;

		::wsprintf((yyval.expr).sStrOrg, _T("%s * %s"), strExpr2, strExpr3);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 4);
		if(nError)
			return nError;
//		multi( &$$, &$1, &$3); 
	}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 474 "SysTrading.y"
    { 
		CString strExpr2 = ::lstrlen((yyvsp[(1) - (3)].expr).sStrOrg) ? (yyvsp[(1) - (3)].expr).sStrOrg : (yyvsp[(1) - (3)].expr).sStr;
		CString strExpr3 = ::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg) ? (yyvsp[(3) - (3)].expr).sStrOrg : (yyvsp[(3) - (3)].expr).sStr;

		::wsprintf((yyval.expr).sStrOrg, _T("%s / %s"), strExpr2, strExpr3);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 5);
		if(nError)
			return nError;
//		divide( &$$, &$1, &$3); 
	}
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 489 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 493 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(2) - (3)].expr); 
		::wsprintf((yyval.expr).sStr, _T("(%s)"), (yyvsp[(2) - (3)].expr).sStr);
	}
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 500 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 504 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].expr).sStr);
	}
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 508 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStr, "giDataCnt");
	}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 512 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 516 "SysTrading.y"
    { 
//		::lstrcpy($1.sArg, _T("0"));
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&(yyvsp[(1) - (1)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 524 "SysTrading.y"
    { 
//		$$ = $1; 
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&(yyvsp[(1) - (1)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 532 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 536 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 540 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 544 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(2) - (2)].expr); 
	}
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 548 "SysTrading.y"
    { 
		// Parse the minus value sentence.
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(2) - (2)].expr).sStr, NULL, (yyval.expr).sStr, 6);
		(yyval.expr).cExprKind = EXP_COMPLEX;
		if(nError)
			return nError;
	}
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 556 "SysTrading.y"
    { 
		// Parse the parenthesis value sentence.
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(2) - (3)].expr).sStr, NULL, (yyval.expr).sStr, 7);
		(yyval.expr).cExprKind = EXP_COMPLEX;
		if(nError)
			return nError;
	}
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 564 "SysTrading.y"
    {
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 568 "SysTrading.y"
    {
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 572 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 578 "SysTrading.y"
    { 		
		::lstrcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
		::lstrcpy((yyval.expr).sArg, _T("0"));
//		if (var(&$$, $1) < 0) return 0; 
	}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 586 "SysTrading.y"
    {
		::lstrcpy((yyval.expr).sArg, (yyvsp[(3) - (4)].expr).sStr);
	}
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 590 "SysTrading.y"
    {
		// Parse the array sentence.
		::lstrcpy((yyval.expr).sArg, (yyvsp[(3) - (4)].expr).sStr);
//		g_pSysTradingParser->ParseArraySentence($1, $3.sStr, $$.sStr);
//		if (var_array(&$$, $1, &$3) < 0) return 0;
//		ArrayDimDel();
	}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 762 "SysTrading.y"
    { 
//		reserved_var(&$$, "VALUE", atoi($1+5), &gtZeroExpr); 
	}
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 766 "SysTrading.y"
    { 
//		reserved_var(&$$, "VALUE", atoi($1+5), &$3); 
	}
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 773 "SysTrading.y"
    { 
//		reserved_cond(&$$, "COND", atoi($1 + 4), &gtZeroExpr); 
	}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 777 "SysTrading.y"
    { 
//		reserved_cond(&$$, "COND", atoi($1+4), &$3); 
	}
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 793 "SysTrading.y"
    {}
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 796 "SysTrading.y"
    {}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 797 "SysTrading.y"
    {}
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 799 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(2);
		if(nError)
			return nError;
	}
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 804 "SysTrading.y"
    {}
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 808 "SysTrading.y"
    {
		// Write the variable sentence.
		int nError = g_pSysTradingParser->WriteVariableSentence((yyvsp[(3) - (4)].expr).cExprKind, (yyvsp[(1) - (4)].name), &(yyvsp[(3) - (4)].expr), (yyval.name));
		if(nError)
			return nError;
	}
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 815 "SysTrading.y"
    {
		(yyvsp[(3) - (4)].expr).cExprKind = DTTYPE_INT;
		// Write the variable sentence.
		int nError = g_pSysTradingParser->WriteVariableSentence((yyvsp[(3) - (4)].expr).cExprKind, (yyvsp[(1) - (4)].name), &(yyvsp[(3) - (4)].expr), (yyval.name));
		if(nError)
			return nError;
	}
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 823 "SysTrading.y"
    {
		// Parse the naming sentence.
//		::lstrcpy($3.sArg, _T("0"));
		int nError = g_pSysTradingParser->WriteNameVariableSentence((yyvsp[(1) - (4)].name), &(yyvsp[(3) - (4)].expr), (yyval.name));
		if(nError)
			return nError;
	}
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 834 "SysTrading.y"
    {
		(yyval.expr).cExprKind = DTTYPE_INT;
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 839 "SysTrading.y"
    {
		(yyval.expr).cExprKind = DTTYPE_DOUBLE;
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 844 "SysTrading.y"
    {
		(yyval.expr).cExprKind = DTTYPE_STRING;
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].expr).sStr);
	}
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 849 "SysTrading.y"
    {
		(yyval.expr).cExprKind = DTTYPE_BOOL;
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 855 "SysTrading.y"
    {
		(yyval.expr).cExprKind = DTTYPE_INT;
		::lstrcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].expr).sStr);
	}
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 863 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("OPEN"));

		// Parse the reserved sentence. (OPEN)
		int nError = g_pSysTradingParser->ParseReservedSentence2(1, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 872 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("OPEN"));

		// Parse the reserved sentence. (OPEN, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(2, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 881 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("HIGH"));

		// Parse the reserved sentence. (HIGH)
		int nError = g_pSysTradingParser->ParseReservedSentence2(3, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 890 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("HIGH"));

		// Parse the reserved sentence. (HIGH, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(4, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 899 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("LOW"));

		// Parse the reserved sentence. (LOW)
		int nError = g_pSysTradingParser->ParseReservedSentence2(5, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 908 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("LOW"));

		// Parse the reserved sentence. (LOW, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(6, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 917 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("CLOSE"));

		// Parse the reserved sentence. (CLOSE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(7, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 926 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("CLOSE"));

		// Parse the reserved sentence. (CLOSE, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(8, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 935 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("VOLUME"));

		// Parse the reserved sentence. (VOLUME)
		int nError = g_pSysTradingParser->ParseReservedSentence2(9, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 944 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("VOLUME"));

		// Parse the reserved sentence. (VOLUME, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(10, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 953 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("AMOUNT"));

		// Parse the reserved sentence. (AMOUNT)
		int nError = g_pSysTradingParser->ParseReservedSentence2(11, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 962 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("AMOUNT"));

		// Parse the reserved sentence. (AMOUNT, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(12, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 971 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("DATE"));

		// Parse the reserved sentence. (DATE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(13, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 980 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("DATE"));

		// Parse the reserved sentence. (DATE, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(14, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 989 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("TIME"));

		// Parse the reserved sentence. (TIME)
		int nError = g_pSysTradingParser->ParseReservedSentence2(15, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 998 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("TIME"));

		// Parse the reserved sentence. (TIME, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(16, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 1007 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("JMCODE"));

		// Parse the reserved sentence. (JMCODE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(17, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 1016 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("CURRENTDATE"));

		// Parse the reserved sentence. (CURRENTDATE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(18, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 1025 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("CURRENTTIME"));

		// Parse the reserved sentence. (CURRENTTIME)
		int nError = g_pSysTradingParser->ParseReservedSentence2(19, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 1034 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("CURRENTBAR"));

		// Parse the reserved sentence. (CURRENTBAR)
		int nError = g_pSysTradingParser->ParseReservedSentence2(20, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 1046 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s + %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 1);
		if(nError)
			return nError;
	}
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 1056 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s - %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 2);
		if(nError)
			return nError;
	}
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 1066 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s * %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 4);
		if(nError)
			return nError;
	}
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 1076 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s / %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 5);
		if(nError)
			return nError;
	}
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 1086 "SysTrading.y"
    { 
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s % %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 3);
		if(nError)
			return nError;
	}
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 1096 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s + %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 1);
		if(nError)
			return nError;
	}
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 1105 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s - %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 2);
		if(nError)
			return nError;
	}
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 1114 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s * %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 4);
		if(nError)
			return nError;
	}
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 1123 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s / %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 5);
		if(nError)
			return nError;
	}
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 1132 "SysTrading.y"
    { 
		::wsprintf((yyval.expr).sStrOrg, _T("%s %% %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 3);
		if(nError)
			return nError;
	}
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 1141 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s + %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);
		else
			::wsprintf((yyval.expr).sStrOrg, _T("%s + %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStr);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 1);
		if(nError)
			return nError;
	}
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 1153 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s - %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);
		else
			::wsprintf((yyval.expr).sStrOrg, _T("%s - %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStr);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 2);
		if(nError)
			return nError;
	}
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 1165 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s * %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);
		else
			::wsprintf((yyval.expr).sStrOrg, _T("%s * %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStr);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 4);
		if(nError)
			return nError;
	}
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 1177 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s / %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);
		else
			::wsprintf((yyval.expr).sStrOrg, _T("%s / %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStr);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 5);
		if(nError)
			return nError;
	}
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 1189 "SysTrading.y"
    { 
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s % %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);
		else
			::wsprintf((yyval.expr).sStrOrg, _T("%s % %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStr);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 3);
		if(nError)
			return nError;
	}
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 1201 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s + %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 1);
		if(nError)
			return nError;
	}
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 1210 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s - %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 2);
		if(nError)
			return nError;
	}
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 1219 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s * %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 4);
		if(nError)
			return nError;
	}
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 1228 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s / %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 5);
		if(nError)
			return nError;
	}
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 1237 "SysTrading.y"
    { 
		::wsprintf((yyval.expr).sStrOrg, _T("%s %% %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 3);
		if(nError)
			return nError;
	}
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 1247 "SysTrading.y"
    {}
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 1250 "SysTrading.y"
    {}
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 1251 "SysTrading.y"
    {}
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 1252 "SysTrading.y"
    {}
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 1257 "SysTrading.y"
    {
		// Write the variable sentence.
		int nError = g_pSysTradingParser->WriteArraySentence((yyvsp[(6) - (7)].expr).cExprKind, (yyvsp[(1) - (7)].name), (yyvsp[(3) - (7)].expr).sStr, &(yyvsp[(6) - (7)].expr), (yyval.name));
		if(nError)
			return nError;
//		g_pSysTradingParser->TempFunc();
	}
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 1267 "SysTrading.y"
    {
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 1271 "SysTrading.y"
    {
		sprintf((yyval.expr).sStr, "%s,%s", (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].name));
	}
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 1277 "SysTrading.y"
    { 
		strcpy((yyval.name), (yyvsp[(1) - (1)].name)); 
	}
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 1282 "SysTrading.y"
    {}
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 1284 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (2)].name));
		if(nError)
			return nError;
	}
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 1290 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 1296 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (2)].name));
		if(nError)
			return nError;
	}
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 1302 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 1308 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 1314 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (4)].name));
		if(nError)
			return nError;
	}
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 1320 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (5)].name));
		if(nError)
			return nError;
	}
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 1326 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (5)].name));
		if(nError)
			return nError;
	}
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 1332 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (6)].name));
		if(nError)
			return nError;
	}
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 1338 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (5)].name));
		if(nError)
			return nError;
	}
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 1344 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (6)].name));
		if(nError)
			return nError;
	}
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 1352 "SysTrading.y"
    { 
		strcpy((yyval.name), (yyvsp[(1) - (1)].name)); 
	}
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 1356 "SysTrading.y"
    { 
		strcpy((yyval.name), (yyvsp[(1) - (1)].name)); 
	}
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 1362 "SysTrading.y"
    { 
//		if (num(&$$, $1) < 0) return 0; 
	}
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 1368 "SysTrading.y"
    {
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&(yyvsp[(1) - (3)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;

		// Parse the assign sentence.
		nError = g_pSysTradingParser->ParseAssignSentence(1, &(yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 1380 "SysTrading.y"
    { 
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&(yyvsp[(1) - (3)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;

		// Parse the assign sentence.
		nError = g_pSysTradingParser->ParseAssignSentence(1, &(yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
//		assign(&$$, &$1, &$3); 
	}
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 1393 "SysTrading.y"
    { 
		// Parse the assign sentence.
		int nError = g_pSysTradingParser->ParseAssignSentence(1, &(yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 1400 "SysTrading.y"
    { 
		// Parse the assign sentence.
		int nError = g_pSysTradingParser->ParseAssignSentence(1, &(yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 1407 "SysTrading.y"
    { 
//		assign(&$$, &$1, &$3); 
	}
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 1411 "SysTrading.y"
    { 
//		assign(&$$, &$1, &$3); 
	}
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 1414 "SysTrading.y"
    {}
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 1418 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 1424 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 1430 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 1436 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 1445 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
//		if (ArrayDimNew($$.sStr) < 0) return 0;
	}
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 1450 "SysTrading.y"
    {
		sprintf((yyval.expr).sStr, "%s][%s", (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr);
//		if (ArrayDimInsert($3.sStr) < 0) return 0;
	}
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 1457 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 1463 "SysTrading.y"
    {
		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&(yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 1470 "SysTrading.y"
    {
		::lstrcpy((yyvsp[(1) - (6)].expr).sArg, (yyvsp[(5) - (6)].expr).sStr);

		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&(yyvsp[(1) - (6)].expr), (yyvsp[(2) - (6)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 1479 "SysTrading.y"
    {
		::lstrcpy((yyvsp[(1) - (6)].expr).sArg, (yyvsp[(5) - (6)].expr).sStr);

		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&(yyvsp[(1) - (6)].expr), (yyvsp[(2) - (6)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 1488 "SysTrading.y"
    {
		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&(yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 1495 "SysTrading.y"
    {
		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&(yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 1504 "SysTrading.y"
    {
		// Parse the function name sentence.
		int nError = g_pSysTradingParser->ParseFunctionNameSentence((yyvsp[(1) - (2)].name), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = 0;
		::lstrcpy((yyval.expr).sArg, _T("0"));
	}
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 1515 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->ParseFunctionNameSentence((yyvsp[(1) - (5)].name), (yyval.expr.sStr));
		if(nError)
			return nError;		
		(yyval.expr.cExprKind)  = (yyvsp[(3) - (5)].expr.cExprKind);
		::lstrcpy((yyval.expr.sArg) , (yyvsp[(3) - (5)].expr.sStr));
	}
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 1523 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->ParseFunctionNameSentence((yyvsp[(1) - (11)].name), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = (yyvsp[(3) - (11)].expr).cExprKind;
		::lstrcpy((yyval.expr).sArg, (yyvsp[(3) - (11)].expr).sStr);
	}
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 1532 "SysTrading.y"
    { 
		(yyval.expr).sStr[0] = '\0'; (yyval.expr).cExprKind = EXP_CONSTANT; 
//		::fwrite(")\n", 1, 2, fpOutput);
//		if (FuncArgsNew($$.sStr) < 0) return 0;
	}
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1538 "SysTrading.y"
    { 
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(1, NULL, &(yyvsp[(1) - (1)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = EXP_CONSTANT; 
	}
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1546 "SysTrading.y"
    {
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(2, (yyvsp[(1) - (3)].expr).sStr, &(yyvsp[(3) - (3)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = EXP_CONSTANT;
	}
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 1555 "SysTrading.y"
    { 
		(yyval.expr).sStr[0] = '\0'; (yyval.expr).cExprKind = EXP_CONSTANT; 
//		::fwrite(")\n", 1, 2, fpOutput);
//		if (FuncArgsNew($$.sStr) < 0) return 0;
	}
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1561 "SysTrading.y"
    { 
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(3, NULL, &(yyvsp[(1) - (1)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = EXP_CONSTANT; 
	}
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1569 "SysTrading.y"
    {
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(4, (yyvsp[(1) - (3)].expr).sStr, &(yyvsp[(3) - (3)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = EXP_CONSTANT;
	}
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1579 "SysTrading.y"
    {
		(yyval.expr).sStr[0] = '\0';
	}
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1583 "SysTrading.y"
    {
		(yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1587 "SysTrading.y"
    {
		// Parse the default argument list sentence.
		int nError = g_pSysTradingParser->ParseDefaultArgumentListSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1597 "SysTrading.y"
    {
		// Write the default argument list sentence.
		int nError = g_pSysTradingParser->WriteDefaultArgumentListSentence((yyvsp[(3) - (4)].expr).cExprKind, (yyvsp[(1) - (4)].name), (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1607 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_STRING((yyval.expr).cExprKind);
	}
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1612 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_STRING((yyval.expr).cExprKind);
		SET_SIMPLE((yyval.expr).cExprKind);
	}
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1618 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_STRING((yyval.expr).cExprKind);
		SET_ARRAY((yyval.expr).cExprKind);
	}
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1624 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_DOUBLE((yyval.expr).cExprKind);
	}
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1629 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_DOUBLE((yyval.expr).cExprKind);
		SET_SIMPLE((yyval.expr).cExprKind);
	}
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1635 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_DOUBLE((yyval.expr).cExprKind);
		SET_ARRAY((yyval.expr).cExprKind);
	}
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1641 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_BOOL((yyval.expr).cExprKind);
	}
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1646 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_BOOL((yyval.expr).cExprKind);
		SET_SIMPLE((yyval.expr).cExprKind);
	}
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1652 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_BOOL((yyval.expr).cExprKind);
		SET_ARRAY((yyval.expr).cExprKind);
	}
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1660 "SysTrading.y"
    {}
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1661 "SysTrading.y"
    {}
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1662 "SysTrading.y"
    {}
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1663 "SysTrading.y"
    {
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&(yyvsp[(2) - (7)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;

		g_pSysTradingParser->IsEndBlock(TRUE);
		// Write the for_start sentence.
		nError = g_pSysTradingParser->WriteForStartSentence(&(yyvsp[(2) - (7)].expr), (yyvsp[(4) - (7)].expr).sStr, (yyvsp[(6) - (7)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1675 "SysTrading.y"
    {}
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1676 "SysTrading.y"
    {}
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1677 "SysTrading.y"
    {}
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1678 "SysTrading.y"
    {
		g_pSysTradingParser->IsEndBlock(TRUE);
		// Write the for_start sentence.
		int nError = g_pSysTradingParser->WriteForStartSentence(&(yyvsp[(2) - (7)].expr), (yyvsp[(4) - (7)].expr).sStr, (yyvsp[(6) - (7)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1688 "SysTrading.y"
    {}
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1693 "SysTrading.y"
    { 
//		strcpy($$.sStr, "{\npSTActor->m_STDataManager.BracketIn();\n");
//		::fwrite($$.sStr, 1, ::strlen($$.sStr), fpOutput);
	}
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1701 "SysTrading.y"
    { 
		g_pSysTradingParser->IsEndBlock(TRUE);
		int nError = g_pSysTradingParser->EndBlock(NULL);
		if(nError)
			return nError;
	}
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1708 "SysTrading.y"
    { 
		g_pSysTradingParser->IsEndBlock(FALSE);
		int nError = g_pSysTradingParser->ProcessEndStatment();
		if(nError)
			return nError;
	}
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1718 "SysTrading.y"
    {
		g_pSysTradingParser->IsEndBlock(TRUE);
		g_pSysTradingParser->IsEndOneBlock();
		// Write the if_start sentence.
		int nError = g_pSysTradingParser->WriteIfStartSentence(1, (yyvsp[(2) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1730 "SysTrading.y"
    {
		g_pSysTradingParser->DelEndBlock();
		// Write the else sentence.
		int nError = g_pSysTradingParser->WriteIfStartSentence(2, NULL, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1741 "SysTrading.y"
    {
		g_pSysTradingParser->DelEndBlock();
	}
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1748 "SysTrading.y"
    {
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&(yyvsp[(1) - (4)].expr), (yyval.name));
		if(nError)
			return nError;

		// Parse the member function sentence.	(CROSS ABOVE)
		// g_pSysTradingParser->ParseMemberFunctionSentence(1, &$1, $4.sStr, $$);
		nError = g_pSysTradingParser->ParseInnerFunctionSentence(1, &(yyvsp[(1) - (4)].expr), (yyvsp[(4) - (4)].expr).sStr, (yyval.name));
		if(nError)
			return nError;
	}
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1761 "SysTrading.y"
    {
		// Parse the member function sentence.	(CROSS ABOVE)
		int nError = g_pSysTradingParser->ParseInnerFunctionSentence(1, &(yyvsp[(1) - (4)].expr), (yyvsp[(4) - (4)].expr).sStr, (yyval.name));
		if(nError)
			return nError;
	}
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1771 "SysTrading.y"
    {
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&(yyvsp[(1) - (4)].expr), (yyval.name));
		if(nError)
			return nError;

		// Parse the member function sentence. (CROSS BELOW)
		// g_pSysTradingParser->ParseMemberFunctionSentence(2, &$1, $4.sStr, $$);
		nError = g_pSysTradingParser->ParseInnerFunctionSentence(2, &(yyvsp[(1) - (4)].expr), (yyvsp[(4) - (4)].expr).sStr, (yyval.name));
		if(nError)
			return nError;
	}
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1784 "SysTrading.y"
    {
		// Parse the member function sentence. (CROSS BELOW)
		int nError = g_pSysTradingParser->ParseInnerFunctionSentence(2, &(yyvsp[(1) - (4)].expr), (yyvsp[(4) - (4)].expr).sStr, (yyval.name));
		if(nError)
			return nError;
	}
    break;


/* Line 1792 of yacc.c  */
#line 4186 "SysTrading.cpp"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 1793 "SysTrading.y"


/* programs */

/*
 * ???
 */
extern "C"
{
	int	SysTradingwrap()
	{
		return 1;
	}
}

/*
 * Report an error situation discovered in a production
 *
 * This does not do anything since we report all error situations through
 * idl_global->err() operations
 */
int SysTradingerror(const char *s)
{
//    cerr << "Syntax error: " << s << endl;
	return g_pSysTradingParser->GetParseErrorIndex(0);
}

void YACC_SetSysTradingParser(SysTradingParser* p_pParser)
{
	g_pSysTradingParser = p_pParser;
}