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
#define BEND_SEMICOLON  320
#define BEND  321
#define FOR  322
#define FUNCTION  323
#define IF  324
#define OR  325
#define MOD  326
#define NOT  327
#define THEN  328
#define TO  329
#define WHILE  330
#define CROSS  331
#define ABOVE  332
#define BELOW  333
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
#line 216 "SysTrading.cpp"
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
#line 244 "SysTrading.cpp"

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
#define YYFINAL  109
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1022

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  79
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  58
/* YYNRULES -- Number of rules.  */
#define YYNRULES  241
/* YYNRULES -- Number of states.  */
#define YYNSTATES  456

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   333

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
      75,    76,    77,    78
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    11,    14,    17,    20,
      23,    26,    29,    32,    35,    38,    41,    44,    47,    48,
      50,    55,    57,    61,    63,    66,    68,    70,    73,    77,
      80,    84,    88,    93,    98,   100,   103,   107,   112,   114,
     118,   122,   124,   127,   131,   135,   139,   143,   147,   151,
     153,   156,   158,   160,   162,   166,   170,   174,   178,   182,
     186,   188,   192,   194,   196,   198,   200,   202,   204,   206,
     208,   210,   212,   215,   218,   222,   224,   226,   228,   233,
     238,   240,   245,   247,   252,   254,   259,   261,   266,   268,
     273,   275,   280,   282,   287,   289,   294,   296,   298,   300,
     302,   304,   309,   311,   316,   321,   323,   327,   330,   332,
     337,   342,   347,   349,   351,   353,   355,   357,   359,   361,
     366,   368,   373,   375,   380,   382,   387,   389,   394,   396,
     401,   403,   408,   410,   415,   417,   419,   421,   423,   427,
     431,   435,   439,   443,   447,   451,   455,   459,   463,   467,
     471,   475,   479,   483,   487,   491,   495,   499,   503,   508,
     510,   514,   516,   524,   526,   530,   532,   534,   537,   541,
     544,   548,   552,   557,   563,   569,   576,   582,   589,   591,
     593,   595,   599,   603,   607,   611,   615,   619,   621,   624,
     627,   630,   633,   635,   639,   641,   645,   652,   659,   663,
     667,   670,   671,   683,   684,   686,   690,   691,   693,   697,
     698,   700,   704,   709,   711,   713,   715,   717,   719,   721,
     723,   725,   727,   734,   742,   743,   751,   758,   766,   767,
     775,   778,   780,   782,   785,   787,   791,   793,   797,   802,
     807,   812
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      80,     0,    -1,    81,    -1,    80,    81,    -1,    -1,    82,
      -1,    83,    82,    -1,    99,    82,    -1,   105,    82,    -1,
      90,    82,    -1,   113,    82,    -1,   129,    82,    -1,   126,
      82,    -1,   130,    82,    -1,   131,    82,    -1,   132,    82,
      -1,   133,    82,    -1,   134,    82,    -1,    -1,    36,    -1,
      12,    52,    84,    53,    -1,    86,    -1,    84,    37,    86,
      -1,   123,    -1,    84,    86,    -1,    85,    -1,     3,    -1,
       3,    48,    -1,     3,    48,   102,    -1,     3,    49,    -1,
       3,   102,    49,    -1,     3,    48,    49,    -1,     3,    48,
     102,    49,    -1,     3,    48,    90,    49,    -1,    88,    -1,
      88,    53,    -1,    87,    70,    88,    -1,    87,    70,    88,
      53,    -1,    89,    -1,    88,    57,    89,    -1,    48,    87,
      49,    -1,    91,    -1,    72,    91,    -1,    91,    38,    91,
      -1,    91,    39,    91,    -1,    91,    40,    91,    -1,    91,
      41,    91,    -1,    91,    42,    91,    -1,    91,    43,    91,
      -1,    91,    -1,    91,    53,    -1,   135,    -1,   136,    -1,
      92,    -1,    48,    91,    49,    -1,    91,    44,    92,    -1,
      91,    45,    92,    -1,    91,    71,    92,    -1,    91,    46,
      92,    -1,    91,    47,    92,    -1,    93,    -1,    48,    92,
      49,    -1,   112,    -1,    10,    -1,    32,    -1,     4,    -1,
      94,    -1,    95,    -1,    96,    -1,    97,    -1,    98,    -1,
     117,    -1,    44,    90,    -1,    45,    90,    -1,    48,    90,
      49,    -1,   135,    -1,   136,    -1,     3,    -1,     3,    50,
     102,    51,    -1,     3,    50,   115,    51,    -1,    25,    -1,
      25,    50,    90,    51,    -1,    26,    -1,    26,    50,    90,
      51,    -1,    27,    -1,    27,    50,    90,    51,    -1,    24,
      -1,    24,    50,    90,    51,    -1,    28,    -1,    28,    50,
      90,    51,    -1,    29,    -1,    29,    50,    90,    51,    -1,
      30,    -1,    30,    50,    90,    51,    -1,    31,    -1,    31,
      50,    90,    51,    -1,    23,    -1,    33,    -1,    34,    -1,
      35,    -1,     7,    -1,     7,    50,    90,    51,    -1,     8,
      -1,     8,    50,    90,    51,    -1,    13,    52,   100,    53,
      -1,   101,    -1,   100,    37,   101,    -1,   100,   101,    -1,
      85,    -1,     3,    48,   102,    49,    -1,     3,    48,    90,
      49,    -1,     3,    48,    94,    49,    -1,     5,    -1,     6,
      -1,    10,    -1,     4,    -1,   103,    -1,   104,    -1,    25,
      -1,    25,    50,    90,    51,    -1,    26,    -1,    26,    50,
      90,    51,    -1,    27,    -1,    27,    50,    90,    51,    -1,
      24,    -1,    24,    50,    90,    51,    -1,    28,    -1,    28,
      50,    90,    51,    -1,    29,    -1,    29,    50,    90,    51,
      -1,    30,    -1,    30,    50,    90,    51,    -1,    31,    -1,
      31,    50,    90,    51,    -1,    23,    -1,    33,    -1,    34,
      -1,    35,    -1,   102,    44,   102,    -1,   102,    45,   102,
      -1,   102,    46,   102,    -1,   102,    47,   102,    -1,   102,
      71,   102,    -1,   103,    44,   103,    -1,   103,    45,   103,
      -1,   103,    46,   103,    -1,   103,    47,   103,    -1,   103,
      71,   103,    -1,   103,    44,   102,    -1,   103,    45,   102,
      -1,   103,    46,   102,    -1,   103,    47,   102,    -1,   103,
      71,   102,    -1,   102,    44,   103,    -1,   102,    45,   103,
      -1,   102,    46,   103,    -1,   102,    47,   103,    -1,   102,
      71,   103,    -1,    11,    52,   106,    53,    -1,   107,    -1,
     107,    37,   107,    -1,   110,    -1,     3,    50,   108,    51,
      48,   102,    49,    -1,   109,    -1,   108,    37,   109,    -1,
       5,    -1,    85,    -1,     3,    50,    -1,     3,    50,   108,
      -1,     3,    51,    -1,     3,   108,    51,    -1,     3,    51,
      51,    -1,     3,    50,   108,    51,    -1,     3,    50,   108,
      51,    48,    -1,     3,    50,   108,    51,   102,    -1,     3,
      50,   108,    51,   102,    49,    -1,     3,    50,   108,    51,
      49,    -1,     3,    50,   108,    51,    48,    49,    -1,     5,
      -1,     6,    -1,   111,    -1,    94,    38,    90,    -1,    94,
      38,    87,    -1,    95,    38,    90,    -1,    95,    38,    87,
      -1,    97,    38,    90,    -1,    98,    38,    87,    -1,   114,
      -1,    94,    38,    -1,    95,    38,    -1,    38,    90,    -1,
      38,    87,    -1,   116,    -1,   115,    37,   116,    -1,    90,
      -1,   118,   117,    49,    -1,   118,   121,    49,    50,   102,
      51,    -1,   118,   121,    49,    50,   115,    51,    -1,   118,
     121,    49,    -1,   119,   121,    49,    -1,     3,    48,    -1,
      -1,     3,    50,   102,    51,    48,   120,     3,    50,   115,
      51,    48,    -1,    -1,    87,    -1,   122,    37,    87,    -1,
      -1,    87,    -1,   122,    37,    87,    -1,    -1,   124,    -1,
     123,    37,   124,    -1,     3,    48,   125,    49,    -1,    14,
      -1,    15,    -1,    16,    -1,    17,    -1,    18,    -1,    19,
      -1,    20,    -1,    21,    -1,    22,    -1,    67,    94,    38,
      90,    62,    90,    -1,    67,    94,    38,    90,    61,    74,
      90,    -1,    -1,    67,    94,    38,    90,    74,    90,   127,
      -1,    67,    95,    38,    90,    62,    90,    -1,    67,    95,
      38,    90,    61,    74,    90,    -1,    -1,    67,    95,    38,
      90,    74,    90,   128,    -1,    59,    53,    -1,    58,    -1,
      65,    -1,    66,    53,    -1,    66,    -1,    69,    87,    73,
      -1,    63,    -1,    64,    87,    73,    -1,    94,    76,    77,
      90,    -1,    95,    76,    77,    90,    -1,    94,    76,    78,
      90,    -1,    95,    76,    78,    90,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   124,   124,   128,   130,   131,   132,   133,   134,   135,
     142,   149,   150,   154,   158,   165,   170,   175,   181,   182,
     185,   188,   189,   190,   191,   197,   200,   206,   212,   218,
     224,   230,   238,   245,   255,   260,   267,   275,   287,   291,
     301,   309,   313,   321,   329,   337,   345,   353,   361,   371,
     375,   379,   380,   384,   388,   393,   401,   409,   417,   425,
     435,   439,   446,   450,   454,   458,   462,   470,   478,   482,
     486,   490,   494,   498,   506,   514,   518,   524,   532,   536,
     546,   554,   562,   570,   578,   586,   594,   602,   610,   618,
     626,   634,   642,   650,   658,   666,   674,   682,   690,   698,
     708,   712,   719,   723,   740,   743,   744,   745,   751,   754,
     761,   769,   780,   785,   790,   795,   800,   801,   809,   818,
     827,   836,   845,   854,   863,   872,   881,   890,   899,   908,
     917,   926,   935,   944,   953,   962,   971,   980,   992,  1002,
    1012,  1022,  1032,  1042,  1051,  1060,  1069,  1078,  1087,  1099,
    1111,  1123,  1135,  1147,  1156,  1165,  1174,  1183,  1194,  1197,
    1198,  1199,  1203,  1213,  1217,  1223,  1229,  1230,  1236,  1242,
    1248,  1254,  1260,  1266,  1272,  1278,  1284,  1290,  1298,  1302,
    1308,  1314,  1326,  1339,  1346,  1353,  1357,  1361,  1364,  1370,
    1376,  1382,  1391,  1396,  1403,  1409,  1416,  1425,  1434,  1441,
    1450,  1462,  1461,  1479,  1484,  1492,  1502,  1507,  1515,  1526,
    1529,  1533,  1543,  1553,  1558,  1564,  1570,  1575,  1581,  1587,
    1592,  1598,  1607,  1608,  1609,  1609,  1622,  1623,  1624,  1624,
    1635,  1639,  1647,  1654,  1661,  1671,  1683,  1694,  1701,  1714,
    1724,  1737
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
  "DO", "DOWN", "DOWNTO", "ELSE", "ELSEIF", "BEND_SEMICOLON", "BEND",
  "FOR", "FUNCTION", "IF", "OR", "MOD", "NOT", "THEN", "TO", "WHILE",
  "CROSS", "ABOVE", "BELOW", "$accept", "stmt_list", "stmt", "stmt_suffix",
  "input_stmt", "input_list", "Declare_Error", "input_list2", "cond_expr",
  "cond_expr2", "cond_expr3", "expr", "expr2", "expr3", "expr4",
  "name_expr", "array_expr", "reserved_sise", "reserved_arr",
  "reserved_cond", "variable_stmt", "variable_list", "variable_list2",
  "data_list", "reserved_list", "expr_list", "array_stmt", "array_list",
  "array_list2", "array_list3", "array_list4", "Array_Error", "num_list",
  "num_expr", "assign_expr", "Assign_Error", "array_idx_list",
  "array_idx_list2", "function", "function_name", "function_name2", "@1",
  "func_arg_list", "func_arg_list2", "def_arg_list", "def_arg_list2",
  "data_type_list", "for_start", "$@2", "$@3", "loop_break", "block_begin",
  "block_end", "if_start", "if_else", "if_else_if", "cross_above",
  "cross_below", YY_NULL
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
     325,   326,   327,   328,   329,   330,   331,   332,   333
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    79,    80,    80,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    82,    82,
      83,    84,    84,    84,    84,    84,    85,    85,    85,    85,
      85,    85,    86,    86,    87,    87,    87,    87,    88,    88,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    90,
      90,    90,    90,    91,    91,    91,    91,    91,    91,    91,
      92,    92,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    94,    95,    95,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      97,    97,    98,    98,    99,   100,   100,   100,   100,   101,
     101,   101,   102,   102,   102,   102,   102,   102,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   105,   106,
     106,   106,   107,   108,   108,   109,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   111,   111,
     112,   113,   113,   113,   113,   113,   113,   113,   114,   114,
     114,   114,   115,   115,   116,   117,   117,   117,   117,   117,
     118,   120,   119,   121,   121,   121,   122,   122,   122,   123,
     123,   123,   124,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   126,   126,   127,   126,   126,   126,   128,   126,
     129,   130,   131,   131,   131,   132,   133,   134,   135,   135,
     136,   136
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       4,     1,     3,     1,     2,     1,     1,     2,     3,     2,
       3,     3,     4,     4,     1,     2,     3,     4,     1,     3,
       3,     1,     2,     3,     3,     3,     3,     3,     3,     1,
       2,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     1,     1,     1,     4,     4,
       1,     4,     1,     4,     1,     4,     1,     4,     1,     4,
       1,     4,     1,     4,     1,     4,     1,     1,     1,     1,
       1,     4,     1,     4,     4,     1,     3,     2,     1,     4,
       4,     4,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     4,     1,     4,     1,     4,     1,     4,     1,     4,
       1,     4,     1,     4,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     1,
       3,     1,     7,     1,     3,     1,     1,     2,     3,     2,
       3,     3,     4,     5,     5,     6,     5,     6,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     1,     2,     2,
       2,     2,     1,     3,     1,     3,     6,     6,     3,     3,
       2,     0,    11,     0,     1,     3,     0,     1,     3,     0,
       1,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     7,     0,     7,     6,     7,     0,     7,
       2,     1,     1,     2,     1,     3,     1,     3,     4,     4,
       4,     4
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,    77,    65,   178,   179,   100,   102,    63,     0,     0,
       0,    96,    86,    80,    82,    84,    88,    90,    92,    94,
      64,    97,    98,    99,    19,     0,     0,     0,     0,   231,
       0,   236,     0,   232,   234,     0,     0,     0,     2,     5,
      18,    18,    49,    53,    60,    66,    67,    68,    69,    70,
      18,    18,   180,    62,    18,   187,    71,   203,   203,    18,
      18,    18,    18,    18,    18,    18,    51,    52,   200,     0,
       0,     0,     0,   209,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   191,    34,    38,   190,    41,
      66,    67,    69,    70,    72,    73,     0,     0,    53,   230,
       0,    41,    75,    76,   233,    77,     0,     0,     0,     1,
       3,     6,     9,     0,     0,     0,     0,    50,     0,   188,
       0,   189,     0,     0,     0,     7,     8,    10,   204,    71,
       0,     0,     0,    12,    11,    13,    14,    15,    16,    17,
      65,   112,   113,    63,    96,    86,    80,    82,    84,    88,
      90,    92,    94,    97,    98,    99,   194,     0,   116,   117,
       0,   192,     0,     0,    26,   166,     0,   159,   161,    26,
       0,    25,    21,    23,   210,    26,   108,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    41,    42,
       0,    35,     0,     0,     0,     0,     0,     0,     0,    74,
      54,    61,   237,     0,     0,     0,   235,     0,    55,    56,
      58,    59,    57,   182,   181,     0,     0,   184,   183,     0,
       0,   185,   186,   195,   198,     0,   199,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,    79,   101,   103,
     115,   112,   113,   114,   134,   124,   118,   120,   122,   126,
     128,   130,   132,   135,   136,   137,    27,    29,   167,   169,
       0,     0,   163,   158,     0,   112,    27,     0,     0,    20,
      24,     0,    27,     0,     0,   104,   107,    87,    81,    83,
      85,    89,    91,    93,    95,    40,    36,    39,    43,    44,
      45,    46,    47,    48,     0,     0,     0,   238,   240,   239,
     241,     0,   205,     0,     0,     0,     0,     0,     0,     0,
       0,   138,   116,   139,   116,   140,   116,   141,   116,   201,
     142,   116,   148,   116,   149,   116,   150,   116,   151,   116,
     152,   116,   193,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    28,   165,   168,   171,    30,     0,   170,     0,
     160,   213,   214,   215,   216,   217,   218,   219,   220,   221,
       0,    28,     0,     0,    22,     0,   211,     0,    66,    28,
       0,   106,    37,    78,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    81,    83,    85,    89,    91,    93,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   172,
     164,     0,    33,    32,   212,     0,     0,   110,   111,   109,
       0,     0,   222,   224,     0,   226,   228,   196,   197,     0,
     125,   119,   121,   123,   127,   129,   131,   133,   173,   176,
     174,     0,   223,   225,   227,   229,     0,   177,     0,   175,
       0,     0,   162,     0,     0,   202
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    37,    38,    39,    40,   170,   165,   172,   128,    86,
      87,   156,    42,    43,    44,    90,    91,    47,    92,    93,
      50,   177,   178,   270,   158,   159,    51,   166,   167,   271,
     272,   168,    52,    53,    54,    55,   160,   161,    56,    57,
      58,   400,   130,   131,   173,   174,   372,    59,   443,   445,
      60,    61,    62,    63,    64,    65,    66,    67
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -308
static const yytype_int16 yypact[] =
{
     589,   140,  -308,  -308,  -308,   -19,    40,  -308,   -10,     2,
      33,  -308,    50,    55,    88,    95,   112,   116,   134,   137,
    -308,  -308,  -308,  -308,  -308,   539,   751,   751,   751,  -308,
     -20,  -308,   539,  -308,   150,   210,   539,   442,  -308,  -308,
     181,   181,    97,  -308,  -308,    12,    35,  -308,   183,   184,
     181,   181,  -308,  -308,   181,  -308,  -308,   489,   489,   181,
     181,   181,   181,   181,   181,   181,  -308,  -308,  -308,   784,
     751,   751,   220,   221,   223,   751,   751,   751,   751,   751,
     751,   751,   751,   539,   751,   167,    -4,  -308,  -308,   911,
     159,   166,  -308,  -308,  -308,  -308,   200,   165,   201,  -308,
      59,   154,  -308,  -308,  -308,   205,   218,   228,    79,  -308,
    -308,  -308,  -308,   817,   817,   817,   817,  -308,   817,   539,
      32,   539,    53,   751,   539,  -308,  -308,  -308,   -14,   219,
     222,   230,   224,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
     269,  -308,  -308,   272,   274,     8,     9,    13,    15,    18,
      19,    36,    37,   277,   278,   281,  -308,   207,   313,  -308,
      -8,  -308,   239,   243,   636,  -308,   240,   264,  -308,   130,
      11,  -308,  -308,   265,  -308,   849,  -308,    14,  -308,   252,
     254,   255,   256,   257,   258,   266,   267,   -25,   325,   506,
     539,  -308,   539,   751,   751,   751,   751,   751,   751,  -308,
    -308,  -308,  -308,   784,   751,   751,  -308,   751,  -308,  -308,
    -308,  -308,  -308,   167,  -308,   751,   751,   167,  -308,   751,
     751,  -308,   167,  -308,   270,   539,  -308,   751,   751,   751,
     751,   751,   751,   751,   751,   961,   961,   961,   961,   271,
     961,   961,   961,   961,   961,   961,   751,  -308,  -308,  -308,
    -308,    -7,  -308,  -308,  -308,   273,   275,   276,   288,   292,
     311,   327,   329,  -308,  -308,  -308,   306,  -308,   310,   324,
     242,    -5,  -308,  -308,   318,  -308,   685,   279,   379,  -308,
    -308,   384,   718,   280,   391,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,    80,  -308,   506,   506,
     506,   506,   506,   506,   253,    54,   115,  -308,  -308,  -308,
    -308,   784,     4,   351,   353,   355,   356,   357,   358,   359,
     360,   511,   313,   511,   313,   511,   313,   511,   313,  -308,
     511,   313,   511,   313,   511,   313,   511,   313,   511,   313,
     511,   313,  -308,   751,   751,   751,   751,   751,   751,   751,
     751,  -308,   511,  -308,    69,  -308,  -308,   310,  -308,   347,
    -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
     370,   346,   373,   784,  -308,   375,  -308,   377,    42,   354,
     784,  -308,  -308,  -308,   331,   751,   751,   344,   751,   751,
     369,    71,   424,   426,   434,   435,   436,   440,   441,   448,
     458,   428,   430,   431,   432,   438,   447,   451,   452,   881,
    -308,   310,  -308,  -308,  -308,   346,   958,  -308,  -308,  -308,
     354,   751,  -308,  -308,   751,  -308,  -308,  -308,  -308,   414,
    -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,   913,  -308,
     413,    75,  -308,  -308,  -308,  -308,   751,  -308,   483,  -308,
     443,    76,  -308,   961,   456,  -308
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -308,  -308,   473,   957,  -308,  -308,    74,  -164,   -17,   295,
     296,     0,   -23,   -21,  -308,     1,    20,  -308,    47,    52,
    -308,  -308,  -167,     3,   437,  -308,  -308,  -308,   251,  -265,
     174,  -308,  -308,  -308,  -308,  -308,  -307,   289,   479,  -308,
    -308,  -308,   480,  -308,  -308,   259,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,    67,   238
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -209
static const yytype_int16 yytable[] =
{
      41,    45,    89,   354,   391,    97,   280,    98,    85,   101,
     286,  -124,  -118,   101,   277,   100,  -120,   283,  -122,   108,
      46,  -126,  -128,  -207,   295,    88,    94,    95,    96,   246,
    -165,    70,   357,    99,   101,   101,   106,    41,    45,  -130,
    -132,  -208,    72,   247,  -165,   190,   358,    48,   278,   191,
     119,   284,    49,   192,    73,   107,   190,    46,   227,   228,
     188,   189,    98,   229,   279,   230,   187,   285,   231,   232,
     162,   163,   157,   121,   190,   179,   180,   181,   182,   183,
     184,   185,   186,    96,    48,    74,   233,   234,   120,    49,
      71,   418,   208,   209,   210,   211,    89,   212,    89,   102,
      75,   101,   213,   102,   217,    76,   357,   222,   246,   215,
     216,   122,   357,   246,   374,   384,   385,   381,   120,   214,
     409,   218,   428,   221,   102,   102,   450,   454,   386,   190,
     219,   220,   202,   382,   250,   275,   252,   192,    77,   451,
     253,   113,   114,   115,   116,    78,   441,   171,   176,   190,
     117,   102,   206,   254,   255,   256,   257,   258,   259,   260,
     261,   262,    79,   263,   264,   265,    80,   101,   118,   101,
     298,   299,   300,   301,   302,   303,   387,   388,   276,   267,
     102,   102,   102,   102,    81,   102,    98,    82,    68,   389,
      69,   102,   193,   194,   195,   196,   197,   198,   113,   114,
     115,   116,   101,   104,   305,   306,   304,    96,   312,   113,
     114,   115,   116,   105,   200,   307,   308,    24,   117,   309,
     310,   123,   124,   164,   169,   118,   175,   313,   314,   315,
     316,   317,   318,   319,   320,   120,   118,   190,   321,   323,
     325,   327,   122,   330,   332,   334,   336,   338,   340,   199,
     201,   235,   236,   237,   238,   203,   204,   102,   239,   102,
     102,   102,   102,   102,   102,   102,   205,   225,   223,   352,
     103,   224,  -115,   226,   103,  -114,   370,  -134,   240,   371,
    -135,  -136,   377,   378,  -137,   379,   235,   236,   237,   238,
     248,   356,   102,   273,   249,   103,   103,   235,   236,   237,
     238,   274,   281,   287,   383,   288,   289,   290,   291,   292,
     250,   275,   252,   240,   390,   353,   253,   293,   294,   329,
     311,   359,   103,   343,   240,   344,   345,   373,   380,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   346,   263,
     264,   265,   347,   401,   402,   403,   404,   405,   406,   407,
     408,   103,   103,   103,   103,   351,   103,   241,   242,   243,
     244,   348,   103,   193,   194,   195,   196,   197,   198,   113,
     114,   115,   116,   370,   200,   355,   415,   349,   117,   350,
     377,   378,   277,   420,   245,   422,   423,   375,   425,   426,
     235,   236,   237,   238,   283,   413,   118,   411,   235,   236,
     237,   238,   392,   419,   393,   421,   394,   395,   396,   397,
     398,   399,   440,   235,   236,   237,   238,   240,   424,   412,
     427,   442,   414,   416,   444,   240,   417,  -125,   103,  -119,
     103,   103,   103,   103,   103,   103,   103,  -121,  -123,  -127,
     240,   448,   109,  -129,  -131,     1,     2,     3,     4,     5,
       6,  -133,     7,     8,     9,    10,   448,   235,   236,   237,
     238,   429,   449,   103,   446,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,   430,
      25,   431,   432,   433,   240,   296,    26,    27,   297,   434,
      28,   453,     1,     2,     3,     4,     5,     6,   435,     7,
      29,    30,   436,   437,   455,    31,    32,    33,    34,    35,
     110,    36,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,   360,  -206,   235,   236,   237,
     238,   410,   452,    26,    27,   342,   129,    83,   132,     0,
     376,     0,     1,     2,     3,     4,     5,     6,     0,     7,
     113,   114,   115,   116,   240,   235,   236,   237,   238,     0,
       0,    84,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     0,     0,   118,     0,     0,
       0,     0,   240,    26,    27,     0,     0,    83,     0,     0,
       0,     0,     1,     2,     3,     4,     5,     6,     0,     7,
       8,     9,    10,     0,     0,     0,     0,     0,     0,     0,
       0,    84,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,     0,     0,
       0,     0,     0,    26,    27,     0,     0,    28,     0,     0,
     250,   251,   252,     0,     0,     0,   253,    29,    30,     0,
       0,     0,    31,    32,    33,    34,    35,     0,    36,   254,
     255,   256,   257,   258,   259,   260,   261,   262,     0,   263,
     264,   265,   322,   324,   326,   328,     0,   331,   333,   335,
     337,   339,   341,     0,   266,   267,   268,   269,     1,   140,
     141,   142,     5,     6,     0,   143,     0,     0,     0,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    20,   153,   154,
     155,     1,   140,   141,   142,     5,     6,     0,   143,    26,
      27,     0,     0,    28,   351,     0,     0,     0,     0,     0,
       0,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      20,   153,   154,   155,     1,     2,     3,     4,     5,     6,
       0,     7,    26,    27,     0,     0,    28,   351,     0,     0,
       0,     0,     0,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     1,   140,   141,
     142,     5,     6,     0,   143,    26,    27,     0,     0,    28,
       0,     0,     0,     0,     0,     0,     0,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    20,   153,   154,   155,
       1,     2,     3,     4,     5,     6,     0,     7,    26,    27,
       0,     0,    28,     0,     0,     0,     0,     0,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,   250,   275,   252,     0,     0,     0,   253,
       0,    26,    27,     0,     0,   207,     0,     0,     0,     0,
       0,     0,   254,   255,   256,   257,   258,   259,   260,   261,
     262,     0,   263,   264,   265,   250,   275,   252,     0,     0,
       0,   253,     0,     0,     0,     0,     0,   282,   267,     0,
       0,     0,     0,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,     0,   263,   264,   265,   250,   275,   252,
       0,     0,     0,   253,     0,     0,     0,     0,     0,   438,
     439,     0,     0,     0,     0,     0,   254,   255,   256,   257,
     258,   259,   260,   261,   262,     0,   263,   264,   265,   193,
     194,   195,   196,   197,   198,   113,   114,   115,   116,     0,
       0,     0,   447,     0,   117,   250,   275,   252,     0,     0,
       0,   253,   361,   362,   363,   364,   365,   366,   367,   368,
     369,     0,   118,     0,   254,   255,   256,   257,   258,   259,
     260,   261,   262,     0,   263,   264,   265,   111,   112,     0,
       0,     0,     0,     0,     0,     0,     0,   125,   126,     0,
       0,   127,     0,     0,     0,     0,   133,   134,   135,   136,
     137,   138,   139
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-308)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       0,     0,    25,   268,   311,    28,   170,    28,    25,    32,
     177,     3,     3,    36,     3,    32,     3,     3,     3,    36,
       0,     3,     3,    37,    49,    25,    26,    27,    28,    37,
      37,    50,    37,    53,    57,    58,    35,    37,    37,     3,
       3,    37,    52,    51,    51,    70,    51,     0,    37,    53,
      38,    37,     0,    57,    52,    35,    70,    37,    50,    50,
      83,    84,    83,    50,    53,    50,    83,    53,    50,    50,
      70,    71,    69,    38,    70,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    37,    52,    50,    50,    76,    37,
      50,    49,   113,   114,   115,   116,   119,   118,   121,    32,
      50,   124,   119,    36,   121,    50,    37,   124,    37,    77,
      78,    76,    37,    37,   278,    61,    62,   284,    76,   119,
      51,   121,    51,   123,    57,    58,    51,    51,    74,    70,
      77,    78,    73,    53,     4,     5,     6,    57,    50,   446,
      10,    44,    45,    46,    47,    50,   411,    73,    74,    70,
      53,    84,    73,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    50,    33,    34,    35,    50,   190,    71,   192,
     193,   194,   195,   196,   197,   198,    61,    62,    48,    49,
     113,   114,   115,   116,    50,   118,   207,    50,    48,    74,
      50,   124,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,   225,    53,   204,   205,   203,   207,   225,    44,
      45,    46,    47,     3,    49,   215,   216,    36,    53,   219,
     220,    38,    38,     3,     3,    71,     3,   227,   228,   229,
     230,   231,   232,   233,   234,    76,    71,    70,   235,   236,
     237,   238,    76,   240,   241,   242,   243,   244,   245,    49,
      49,    44,    45,    46,    47,    50,    38,   190,    51,   192,
     193,   194,   195,   196,   197,   198,    38,    37,    49,   266,
      32,    49,     3,    49,    36,     3,   276,     3,    71,   276,
       3,     3,   282,   282,     3,   282,    44,    45,    46,    47,
      51,    49,   225,    53,    51,    57,    58,    44,    45,    46,
      47,    37,    37,    51,    51,    51,    51,    51,    51,    51,
       4,     5,     6,    71,   311,     5,    10,    51,    51,    48,
      50,     3,    84,    50,    71,    50,    50,    48,    48,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    50,    33,
      34,    35,    50,   343,   344,   345,   346,   347,   348,   349,
     350,   113,   114,   115,   116,    49,   118,    44,    45,    46,
      47,    50,   124,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,   373,    49,    51,   373,    50,    53,    50,
     380,   380,     3,   380,    71,   385,   386,     3,   388,   389,
      44,    45,    46,    47,     3,    49,    71,    50,    44,    45,
      46,    47,    51,    49,    51,    74,    51,    51,    51,    51,
      51,    51,   409,    44,    45,    46,    47,    71,    74,    49,
      51,   421,    49,    48,   424,    71,    49,     3,   190,     3,
     192,   193,   194,   195,   196,   197,   198,     3,     3,     3,
      71,   438,     0,     3,     3,     3,     4,     5,     6,     7,
       8,     3,    10,    11,    12,    13,   453,    44,    45,    46,
      47,     3,    49,   225,    50,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    51,
      38,    51,    51,    51,    71,   190,    44,    45,   192,    51,
      48,    48,     3,     4,     5,     6,     7,     8,    51,    10,
      58,    59,    51,    51,    48,    63,    64,    65,    66,    67,
      37,    69,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,   274,    37,    44,    45,    46,
      47,   357,    49,    44,    45,   246,    57,    48,    58,    -1,
     281,    -1,     3,     4,     5,     6,     7,     8,    -1,    10,
      44,    45,    46,    47,    71,    44,    45,    46,    47,    -1,
      -1,    72,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    71,    44,    45,    -1,    -1,    48,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    10,
      11,    12,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    48,    -1,    -1,
       4,     5,     6,    -1,    -1,    -1,    10,    58,    59,    -1,
      -1,    -1,    63,    64,    65,    66,    67,    -1,    69,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    35,   235,   236,   237,   238,    -1,   240,   241,   242,
     243,   244,   245,    -1,    48,    49,    50,    51,     3,     4,
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
      27,    28,    29,    30,    31,    -1,    33,    34,    35,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      -1,    -1,    49,    -1,    53,     4,     5,     6,    -1,    -1,
      -1,    10,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    71,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    35,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    59,    60,    61,    62,
      63,    64,    65
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    10,    11,    12,
      13,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    38,    44,    45,    48,    58,
      59,    63,    64,    65,    66,    67,    69,    80,    81,    82,
      83,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   105,   111,   112,   113,   114,   117,   118,   119,   126,
     129,   130,   131,   132,   133,   134,   135,   136,    48,    50,
      50,    50,    52,    52,    52,    50,    50,    50,    50,    50,
      50,    50,    50,    48,    72,    87,    88,    89,    90,    91,
      94,    95,    97,    98,    90,    90,    90,    91,    92,    53,
      87,    91,   135,   136,    53,     3,    94,    95,    87,     0,
      81,    82,    82,    44,    45,    46,    47,    53,    71,    38,
      76,    38,    76,    38,    38,    82,    82,    82,    87,   117,
     121,   122,   121,    82,    82,    82,    82,    82,    82,    82,
       4,     5,     6,    10,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    33,    34,    35,    90,   102,   103,   104,
     115,   116,    90,    90,     3,    85,   106,   107,   110,     3,
      84,    85,    86,   123,   124,     3,    85,   100,   101,    90,
      90,    90,    90,    90,    90,    90,    90,    87,    91,    91,
      70,    53,    57,    38,    39,    40,    41,    42,    43,    49,
      49,    49,    73,    50,    38,    38,    73,    48,    92,    92,
      92,    92,    92,    87,    90,    77,    78,    87,    90,    77,
      78,    90,    87,    49,    49,    37,    49,    50,    50,    50,
      50,    50,    50,    50,    50,    44,    45,    46,    47,    51,
      71,    44,    45,    46,    47,    71,    37,    51,    51,    51,
       4,     5,     6,    10,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    33,    34,    35,    48,    49,    50,    51,
     102,   108,   109,    53,    37,     5,    48,     3,    37,    53,
      86,    37,    48,     3,    37,    53,   101,    51,    51,    51,
      51,    51,    51,    51,    51,    49,    88,    89,    91,    91,
      91,    91,    91,    91,   102,    90,    90,    90,    90,    90,
      90,    50,    87,    90,    90,    90,    90,    90,    90,    90,
      90,   102,   103,   102,   103,   102,   103,   102,   103,    48,
     102,   103,   102,   103,   102,   103,   102,   103,   102,   103,
     102,   103,   116,    50,    50,    50,    50,    50,    50,    50,
      50,    49,   102,     5,   108,    51,    49,    37,    51,     3,
     107,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      90,   102,   125,    48,    86,     3,   124,    90,    94,   102,
      48,   101,    53,    51,    61,    62,    74,    61,    62,    74,
     102,   115,    51,    51,    51,    51,    51,    51,    51,    51,
     120,    90,    90,    90,    90,    90,    90,    90,    90,    51,
     109,    50,    49,    49,    49,   102,    48,    49,    49,    49,
     102,    74,    90,    90,    74,    90,    90,    51,    51,     3,
      51,    51,    51,    51,    51,    51,    51,    51,    48,    49,
     102,   108,    90,   127,    90,   128,    50,    49,   102,    49,
      51,   115,    49,    48,    51,    48
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
#line 125 "SysTrading.y"
    {
//		g_pSysTradingParser->WriteOneStatementEnd($1.sStr);
	}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 136 "SysTrading.y"
    {
		// Write the general statement.
		int nError = g_pSysTradingParser->WriteGeneralSentence((yyvsp[(1) - (2)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 143 "SysTrading.y"
    {
		// Write the assign statement.
		int nError = g_pSysTradingParser->WriteAssignSentence((yyvsp[(1) - (2)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 149 "SysTrading.y"
    {}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 151 "SysTrading.y"
    {
		g_pSysTradingParser->AddStack(STACK_FOR);
	}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 155 "SysTrading.y"
    {
		g_pSysTradingParser->ChangeBlockStack();
	}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 159 "SysTrading.y"
    {
//		g_pSysTradingParser->IsEndBlock(TRUE);
//		int nError = g_pSysTradingParser->EndBlock($1.sStr);
//		if(nError)
//			return nError;
	}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 166 "SysTrading.y"
    {
//		g_pSysTradingParser->IsEndBlock(TRUE);
		g_pSysTradingParser->AddStack(STACK_IF);
	}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 171 "SysTrading.y"
    {
//		g_pSysTradingParser->DelEndBlock();
		g_pSysTradingParser->AddStack(STACK_ELSE);
	}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 176 "SysTrading.y"
    {
//		g_pSysTradingParser->DelEndBlock();
		g_pSysTradingParser->AddStack(STACK_ELSEIF);
	}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 185 "SysTrading.y"
    {}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 188 "SysTrading.y"
    {}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 189 "SysTrading.y"
    {}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 190 "SysTrading.y"
    {}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 192 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(2);
		if(nError)
			return nError;
	}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 197 "SysTrading.y"
    {}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 201 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (1)].name));
		if(nError)
			return nError;
	}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 207 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (2)].name));
		if(nError)
			return nError;
	}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 213 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 219 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (2)].name));
		if(nError)
			return nError;
	}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 225 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 231 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 239 "SysTrading.y"
    {
		// Write the declare sentence.
		int nError = g_pSysTradingParser->WriteDeclareSentence((yyvsp[(3) - (4)].expr).cExprKind, (yyvsp[(1) - (4)].name), &(yyvsp[(3) - (4)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 246 "SysTrading.y"
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
#line 256 "SysTrading.y"
    {
		// Parse the condition sentence (Normal)
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 261 "SysTrading.y"
    { 
		// Parse the condition sentence (Semicolon)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (2)].expr).sStr, NULL, (yyval.expr).sStr, 1);
		if(nError)
			return nError;
	}
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 268 "SysTrading.y"
    { 
		// Parse the condition sentence (Or)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 51);
		if(nError)
			return nError;
//		or( &$$, &$1, &$3); 
	}
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 276 "SysTrading.y"
    { 
		// Parse the condition sentence (Or)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (4)].expr).sStr, (yyvsp[(3) - (4)].expr).sStr, (yyval.expr).sStr, 51);
		if(nError)
			return nError;

		::lstrcat((yyval.expr).sStr, ";");
//		or( &$$, &$1, &$3); strcat($$.sStr, ";");
	}
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 288 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 292 "SysTrading.y"
    { 
		// Parse the condition sentence (And)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 52);
		if(nError)
			return nError;
//		and( &$$, &$1, &$3); 
	}
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 302 "SysTrading.y"
    { 
		// Parse the condition sentence (Parenthesis)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(2) - (3)].expr).sStr, NULL, (yyval.expr).sStr, 2);
		(yyval.expr).cExprKind = EXP_COMPLEX;
		if(nError)
			return nError;
	}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 310 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 314 "SysTrading.y"
    { 
		// Parse the condition sentence (Not)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(2) - (2)].expr).sStr, NULL, (yyval.expr).sStr, 50);
		if(nError)
			return nError;
//		not(&$$, &$2); 
	}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 322 "SysTrading.y"
    { 
		// Parse the condition sentence (Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 3);
		if(nError)
			return nError;
//		eq( &$$, &$1, &$3); 
	}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 330 "SysTrading.y"
    { 
		// Parse the condition sentence (Not Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 8);
		if(nError)
			return nError;
//		ne( &$$, &$1, &$3); 
	}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 338 "SysTrading.y"
    { 
		// Parse the condition sentence (Less Then)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 4);
		if(nError)
			return nError;
//		lt( &$$, &$1, &$3); 
	}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 346 "SysTrading.y"
    { 
		// Parse the condition sentence (Less Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 5);
		if(nError)
			return nError;
//		le( &$$, &$1, &$3); 
	}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 354 "SysTrading.y"
    { 
		// Parse the condition sentence (Greater Then)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 6);
		if(nError)
			return nError;
//		gt( &$$, &$1, &$3); 
	}
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 362 "SysTrading.y"
    { 
		// Parse the condition sentence (Greater Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 7);
		if(nError)
			return nError;
//		ge( &$$, &$1, &$3); 
	}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 372 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 376 "SysTrading.y"
    { 
//		sprintf($$.sStr, "%s;", $1.sStr); 
	}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 379 "SysTrading.y"
    {}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 380 "SysTrading.y"
    {}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 385 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 389 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(2) - (3)].expr); 
		::wsprintf((yyval.expr).sStr, _T("(%s)"), (yyvsp[(2) - (3)].expr).sStr);
	}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 394 "SysTrading.y"
    { 
		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 1);
		if(nError)
			return nError;
//		plus( &$$, &$1, &$3); 
	}
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 402 "SysTrading.y"
    { 
		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 2);
		if(nError)
			return nError;
//		minus( &$$, &$1, &$3); 
	}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 410 "SysTrading.y"
    { 
		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 3);
		if(nError)
			return nError;
//		mod( &$$, &$1, &$3); 
	}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 418 "SysTrading.y"
    { 
		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 4);
		if(nError)
			return nError;
//		multi( &$$, &$1, &$3); 
	}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 426 "SysTrading.y"
    { 
		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 5);
		if(nError)
			return nError;
//		divide( &$$, &$1, &$3); 
	}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 436 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 440 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(2) - (3)].expr); 
		::wsprintf((yyval.expr).sStr, _T("(%s)"), (yyvsp[(2) - (3)].expr).sStr);
	}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 447 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 451 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].expr).sStr);
	}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 455 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStr, "giDataCnt");
	}
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 459 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 463 "SysTrading.y"
    { 
//		::lstrcpy($1.sArg, _T("0"));
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&(yyvsp[(1) - (1)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 471 "SysTrading.y"
    { 
//		$$ = $1; 
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&(yyvsp[(1) - (1)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 479 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 483 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 487 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 491 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 495 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(2) - (2)].expr); 
	}
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 499 "SysTrading.y"
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
#line 507 "SysTrading.y"
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
#line 515 "SysTrading.y"
    {
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 519 "SysTrading.y"
    {
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 525 "SysTrading.y"
    { 		
		::lstrcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
		::lstrcpy((yyval.expr).sArg, _T("0"));
//		if (var(&$$, $1) < 0) return 0; 
	}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 533 "SysTrading.y"
    {
		::lstrcpy((yyval.expr).sArg, (yyvsp[(3) - (4)].expr).sStr);
	}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 537 "SysTrading.y"
    {
		// Parse the array sentence.
		::lstrcpy((yyval.expr).sArg, (yyvsp[(3) - (4)].expr).sStr);
//		g_pSysTradingParser->ParseArraySentence($1, $3.sStr, $$.sStr);
//		if (var_array(&$$, $1, &$3) < 0) return 0;
//		ArrayDimDel();
	}
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 547 "SysTrading.y"
    { 
		// Parse the reserved sentence. (OPEN)
		int nError = g_pSysTradingParser->ParseReservedSentence(1, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("OPEN"));
		if(nError)
			return nError;
	}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 555 "SysTrading.y"
    { 
		// Parse the reserved sentence. (OPEN, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(2, (yyvsp[(3) - (4)].expr).sStr, (yyval.expr).sStr);
		::wsprintf((yyval.expr).sStrOrg, _T("OPEN(%s)"), (yyvsp[(3) - (4)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 563 "SysTrading.y"
    { 
		// Parse the reserved sentence. (HIGH)
		int nError = g_pSysTradingParser->ParseReservedSentence(3, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("HIGH"));
		if(nError)
			return nError;
	}
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 571 "SysTrading.y"
    { 
		// Parse the reserved sentence. (HIGH, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(4, (yyvsp[(3) - (4)].expr).sStr, (yyval.expr).sStr);
		::wsprintf((yyval.expr).sStrOrg, _T("HIGH(%s)"), (yyvsp[(3) - (4)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 579 "SysTrading.y"
    { 
		// Parse the reserved sentence. (LOW)
		int nError = g_pSysTradingParser->ParseReservedSentence(5, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("LOW"));
		if(nError)
			return nError;
	}
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 587 "SysTrading.y"
    { 
		// Parse the reserved sentence. (LOW, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(6, (yyvsp[(3) - (4)].expr).sStr, (yyval.expr).sStr);
		::wsprintf((yyval.expr).sStrOrg, _T("LOW(%s)"), (yyvsp[(3) - (4)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 595 "SysTrading.y"
    { 
		// Parse the reserved sentence. (CLOSE)
		int nError = g_pSysTradingParser->ParseReservedSentence(7, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("CLOSE"));
		if(nError)
			return nError;
	}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 603 "SysTrading.y"
    { 
		// Parse the reserved sentence. (CLOSE, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(8, (yyvsp[(3) - (4)].expr).sStr, (yyval.expr).sStr);
		::wsprintf((yyval.expr).sStrOrg, _T("CLOSE(%s)"), (yyvsp[(3) - (4)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 611 "SysTrading.y"
    { 
		// Parse the reserved sentence. (VOLUME)
		int nError = g_pSysTradingParser->ParseReservedSentence(9, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("VOLUME"));
		if(nError)
			return nError;
	}
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 619 "SysTrading.y"
    { 
		// Parse the reserved sentence. (VOLUME, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(10, (yyvsp[(3) - (4)].expr).sStr, (yyval.expr).sStr);
		::wsprintf((yyval.expr).sStrOrg, _T("VOLUME(%s)"), (yyvsp[(3) - (4)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 627 "SysTrading.y"
    { 
		// Parse the reserved sentence. (AMOUNT)
		int nError = g_pSysTradingParser->ParseReservedSentence(11, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("AMOUNT"));
		if(nError)
			return nError;
	}
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 635 "SysTrading.y"
    { 
		// Parse the reserved sentence. (AMOUNT, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(12, (yyvsp[(3) - (4)].expr).sStr, (yyval.expr).sStr);
		::wsprintf((yyval.expr).sStrOrg, _T("AMOUNT(%s)"), (yyvsp[(3) - (4)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 643 "SysTrading.y"
    { 
		// Parse the reserved sentence. (DATE)
		int nError = g_pSysTradingParser->ParseReservedSentence(13, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("DATE"));
		if(nError)
			return nError;
	}
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 651 "SysTrading.y"
    { 
		// Parse the reserved sentence. (DATE, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(14, (yyvsp[(3) - (4)].expr).sStr, (yyval.expr).sStr);
		::wsprintf((yyval.expr).sStrOrg, _T("DATE(%s)"), (yyvsp[(3) - (4)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 659 "SysTrading.y"
    { 
		// Parse the reserved sentence. (TIME)
		int nError = g_pSysTradingParser->ParseReservedSentence(15, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("TIME"));
		if(nError)
			return nError;
	}
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 667 "SysTrading.y"
    { 
		// Parse the reserved sentence. (TIME, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(16, (yyvsp[(3) - (4)].expr).sStr, (yyval.expr).sStr);
		::wsprintf((yyval.expr).sStrOrg, _T("TIME(%s)"), (yyvsp[(3) - (4)].expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 675 "SysTrading.y"
    { 
		// Parse the reserved sentence. (JMCODE)
		int nError = g_pSysTradingParser->ParseReservedSentence(17, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("JMCODE"));
		if(nError)
			return nError;
	}
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 683 "SysTrading.y"
    { 
		// Parse the reserved sentence. (CURRENTDATE)
		int nError = g_pSysTradingParser->ParseReservedSentence(18, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("CURRENTDATE"));
		if(nError)
			return nError;
	}
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 691 "SysTrading.y"
    { 
		// Parse the reserved sentence. (CURRENTTIME)
		int nError = g_pSysTradingParser->ParseReservedSentence(19, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("CURRENTTIME"));
		if(nError)
			return nError;
	}
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 699 "SysTrading.y"
    { 
		// Parse the reserved sentence. (CURRENTBAR)
		int nError = g_pSysTradingParser->ParseReservedSentence(20, NULL, (yyval.expr).sStr);
		::lstrcpy((yyval.expr).sStrOrg, _T("CURRENTBAR"));
		if(nError)
			return nError;
	}
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 709 "SysTrading.y"
    { 
//		reserved_var(&$$, "VALUE", atoi($1+5), &gtZeroExpr); 
	}
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 713 "SysTrading.y"
    { 
//		reserved_var(&$$, "VALUE", atoi($1+5), &$3); 
	}
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 720 "SysTrading.y"
    { 
//		reserved_cond(&$$, "COND", atoi($1 + 4), &gtZeroExpr); 
	}
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 724 "SysTrading.y"
    { 
//		reserved_cond(&$$, "COND", atoi($1+4), &$3); 
	}
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 740 "SysTrading.y"
    {}
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 743 "SysTrading.y"
    {}
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 744 "SysTrading.y"
    {}
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 746 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(2);
		if(nError)
			return nError;
	}
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 751 "SysTrading.y"
    {}
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 755 "SysTrading.y"
    {
		// Write the variable sentence.
		int nError = g_pSysTradingParser->WriteVariableSentence((yyvsp[(3) - (4)].expr).cExprKind, (yyvsp[(1) - (4)].name), &(yyvsp[(3) - (4)].expr), (yyval.name));
		if(nError)
			return nError;
	}
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 762 "SysTrading.y"
    {
		(yyvsp[(3) - (4)].expr).cExprKind = DTTYPE_INT;
		// Write the variable sentence.
		int nError = g_pSysTradingParser->WriteVariableSentence((yyvsp[(3) - (4)].expr).cExprKind, (yyvsp[(1) - (4)].name), &(yyvsp[(3) - (4)].expr), (yyval.name));
		if(nError)
			return nError;
	}
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 770 "SysTrading.y"
    {
		// Parse the naming sentence.
//		::lstrcpy($3.sArg, _T("0"));
		int nError = g_pSysTradingParser->WriteNameVariableSentence((yyvsp[(1) - (4)].name), &(yyvsp[(3) - (4)].expr), (yyval.name));
		if(nError)
			return nError;
	}
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 781 "SysTrading.y"
    {
		(yyval.expr).cExprKind = DTTYPE_INT;
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 786 "SysTrading.y"
    {
		(yyval.expr).cExprKind = DTTYPE_DOUBLE;
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 791 "SysTrading.y"
    {
		(yyval.expr).cExprKind = DTTYPE_STRING;
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].expr).sStr);
	}
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 796 "SysTrading.y"
    {
		(yyval.expr).cExprKind = DTTYPE_BOOL;
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 802 "SysTrading.y"
    {
		(yyval.expr).cExprKind = DTTYPE_INT;
		::lstrcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].expr).sStr);
	}
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 810 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("OPEN"));

		// Parse the reserved sentence. (OPEN)
		int nError = g_pSysTradingParser->ParseReservedSentence2(1, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 819 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("OPEN"));

		// Parse the reserved sentence. (OPEN, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(2, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 828 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("HIGH"));

		// Parse the reserved sentence. (HIGH)
		int nError = g_pSysTradingParser->ParseReservedSentence2(3, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 837 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("HIGH"));

		// Parse the reserved sentence. (HIGH, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(4, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 846 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("LOW"));

		// Parse the reserved sentence. (LOW)
		int nError = g_pSysTradingParser->ParseReservedSentence2(5, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 855 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("LOW"));

		// Parse the reserved sentence. (LOW, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(6, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 864 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("CLOSE"));

		// Parse the reserved sentence. (CLOSE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(7, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 873 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("CLOSE"));

		// Parse the reserved sentence. (CLOSE, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(8, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 882 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("VOLUME"));

		// Parse the reserved sentence. (VOLUME)
		int nError = g_pSysTradingParser->ParseReservedSentence2(9, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 891 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("VOLUME"));

		// Parse the reserved sentence. (VOLUME, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(10, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 900 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("AMOUNT"));

		// Parse the reserved sentence. (AMOUNT)
		int nError = g_pSysTradingParser->ParseReservedSentence2(11, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 909 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("AMOUNT"));

		// Parse the reserved sentence. (AMOUNT, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(12, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 918 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("DATE"));

		// Parse the reserved sentence. (DATE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(13, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 927 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("DATE"));

		// Parse the reserved sentence. (DATE, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(14, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 936 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("TIME"));

		// Parse the reserved sentence. (TIME)
		int nError = g_pSysTradingParser->ParseReservedSentence2(15, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 945 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("TIME"));

		// Parse the reserved sentence. (TIME, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(16, (yyvsp[(3) - (4)].expr).sStr, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 954 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("JMCODE"));

		// Parse the reserved sentence. (JMCODE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(17, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 963 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("CURRENTDATE"));

		// Parse the reserved sentence. (CURRENTDATE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(18, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 972 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("CURRENTTIME"));

		// Parse the reserved sentence. (CURRENTTIME)
		int nError = g_pSysTradingParser->ParseReservedSentence2(19, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 981 "SysTrading.y"
    { 
		::lstrcpy((yyval.expr).sStrOrg, _T("CURRENTBAR"));

		// Parse the reserved sentence. (CURRENTBAR)
		int nError = g_pSysTradingParser->ParseReservedSentence2(20, NULL, &(yyval.expr));
		if(nError)
			return nError;
	}
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 993 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s + %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 1);
		if(nError)
			return nError;
	}
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 1003 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s - %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 2);
		if(nError)
			return nError;
	}
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 1013 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s * %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 4);
		if(nError)
			return nError;
	}
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 1023 "SysTrading.y"
    {
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s / %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 5);
		if(nError)
			return nError;
	}
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 1033 "SysTrading.y"
    { 
		if(::lstrlen((yyvsp[(3) - (3)].expr).sStrOrg))
			::wsprintf((yyval.expr).sStrOrg, _T("%s % %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 3);
		if(nError)
			return nError;
	}
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 1043 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s + %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 1);
		if(nError)
			return nError;
	}
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 1052 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s - %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 2);
		if(nError)
			return nError;
	}
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 1061 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s * %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 4);
		if(nError)
			return nError;
	}
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 1070 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s / %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 5);
		if(nError)
			return nError;
	}
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 1079 "SysTrading.y"
    { 
		::wsprintf((yyval.expr).sStrOrg, _T("%s %% %s"), (yyvsp[(1) - (3)].expr).sStrOrg, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 3);
		if(nError)
			return nError;
	}
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 1088 "SysTrading.y"
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

  case 149:
/* Line 1792 of yacc.c  */
#line 1100 "SysTrading.y"
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

  case 150:
/* Line 1792 of yacc.c  */
#line 1112 "SysTrading.y"
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

  case 151:
/* Line 1792 of yacc.c  */
#line 1124 "SysTrading.y"
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

  case 152:
/* Line 1792 of yacc.c  */
#line 1136 "SysTrading.y"
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

  case 153:
/* Line 1792 of yacc.c  */
#line 1148 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s + %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 1);
		if(nError)
			return nError;
	}
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 1157 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s - %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 2);
		if(nError)
			return nError;
	}
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 1166 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s * %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 4);
		if(nError)
			return nError;
	}
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 1175 "SysTrading.y"
    {
		::wsprintf((yyval.expr).sStrOrg, _T("%s / %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 5);
		if(nError)
			return nError;
	}
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 1184 "SysTrading.y"
    { 
		::wsprintf((yyval.expr).sStrOrg, _T("%s %% %s"), (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStrOrg);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr, 3);
		if(nError)
			return nError;
	}
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 1194 "SysTrading.y"
    {}
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 1197 "SysTrading.y"
    {}
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 1198 "SysTrading.y"
    {}
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 1199 "SysTrading.y"
    {}
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 1204 "SysTrading.y"
    {
		// Write the variable sentence.
		int nError = g_pSysTradingParser->WriteArraySentence((yyvsp[(6) - (7)].expr).cExprKind, (yyvsp[(1) - (7)].name), (yyvsp[(3) - (7)].expr).sStr, &(yyvsp[(6) - (7)].expr), (yyval.name));
		if(nError)
			return nError;
//		g_pSysTradingParser->TempFunc();
	}
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 1214 "SysTrading.y"
    {
		strcpy((yyval.expr).sStr, (yyvsp[(1) - (1)].name));
	}
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 1218 "SysTrading.y"
    {
		sprintf((yyval.expr).sStr, "%s,%s", (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].name));
	}
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 1224 "SysTrading.y"
    { 
		strcpy((yyval.name), (yyvsp[(1) - (1)].name)); 
	}
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 1229 "SysTrading.y"
    {}
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 1231 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (2)].name));
		if(nError)
			return nError;
	}
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 1237 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 1243 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (2)].name));
		if(nError)
			return nError;
	}
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 1249 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 1255 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (3)].name));
		if(nError)
			return nError;
	}
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 1261 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (4)].name));
		if(nError)
			return nError;
	}
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 1267 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (5)].name));
		if(nError)
			return nError;
	}
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 1273 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (5)].name));
		if(nError)
			return nError;
	}
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 1279 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (6)].name));
		if(nError)
			return nError;
	}
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 1285 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (5)].name));
		if(nError)
			return nError;
	}
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 1291 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, (yyvsp[(1) - (6)].name));
		if(nError)
			return nError;
	}
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 1299 "SysTrading.y"
    { 
		strcpy((yyval.name), (yyvsp[(1) - (1)].name)); 
	}
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 1303 "SysTrading.y"
    { 
		strcpy((yyval.name), (yyvsp[(1) - (1)].name)); 
	}
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 1309 "SysTrading.y"
    { 
//		if (num(&$$, $1) < 0) return 0; 
	}
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 1315 "SysTrading.y"
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

  case 182:
/* Line 1792 of yacc.c  */
#line 1327 "SysTrading.y"
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

  case 183:
/* Line 1792 of yacc.c  */
#line 1340 "SysTrading.y"
    { 
		// Parse the assign sentence.
		int nError = g_pSysTradingParser->ParseAssignSentence(1, &(yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 1347 "SysTrading.y"
    { 
		// Parse the assign sentence.
		int nError = g_pSysTradingParser->ParseAssignSentence(1, &(yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1354 "SysTrading.y"
    { 
//		assign(&$$, &$1, &$3); 
	}
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1358 "SysTrading.y"
    { 
//		assign(&$$, &$1, &$3); 
	}
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 1361 "SysTrading.y"
    {}
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1365 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1371 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1377 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}
    break;

  case 191:
/* Line 1792 of yacc.c  */
#line 1383 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1392 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
//		if (ArrayDimNew($$.sStr) < 0) return 0;
	}
    break;

  case 193:
/* Line 1792 of yacc.c  */
#line 1397 "SysTrading.y"
    {
		sprintf((yyval.expr).sStr, "%s][%s", (yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr);
//		if (ArrayDimInsert($3.sStr) < 0) return 0;
	}
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1404 "SysTrading.y"
    { 
		(yyval.expr) = (yyvsp[(1) - (1)].expr); 
	}
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1410 "SysTrading.y"
    {
		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&(yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1417 "SysTrading.y"
    {
		::lstrcpy((yyvsp[(1) - (6)].expr).sArg, (yyvsp[(5) - (6)].expr).sStr);

		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&(yyvsp[(1) - (6)].expr), (yyvsp[(2) - (6)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1426 "SysTrading.y"
    {
		::lstrcpy((yyvsp[(1) - (6)].expr).sArg, (yyvsp[(5) - (6)].expr).sStr);

		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&(yyvsp[(1) - (6)].expr), (yyvsp[(2) - (6)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1435 "SysTrading.y"
    {
		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&(yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1442 "SysTrading.y"
    {
		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&(yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1451 "SysTrading.y"
    {
		// Parse the function name sentence.
		int nError = g_pSysTradingParser->ParseFunctionNameSentence((yyvsp[(1) - (2)].name), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = 0;
		::lstrcpy((yyval.expr).sArg, _T("0"));
	}
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1462 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->ParseFunctionNameSentence((yyvsp[(1) - (5)].name), (yyval.expr.sStr));
		if(nError)
			return nError;		
		(yyval.expr.cExprKind)  = (yyvsp[(3) - (5)].expr.cExprKind);
		::lstrcpy((yyval.expr.sArg) , (yyvsp[(3) - (5)].expr.sStr));
	}
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1470 "SysTrading.y"
    {
		int nError = g_pSysTradingParser->ParseFunctionNameSentence((yyvsp[(1) - (11)].name), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = (yyvsp[(3) - (11)].expr).cExprKind;
		::lstrcpy((yyval.expr).sArg, (yyvsp[(3) - (11)].expr).sStr);
	}
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1479 "SysTrading.y"
    { 
		(yyval.expr).sStr[0] = '\0'; (yyval.expr).cExprKind = EXP_CONSTANT; 
//		::fwrite(")\n", 1, 2, fpOutput);
//		if (FuncArgsNew($$.sStr) < 0) return 0;
	}
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1485 "SysTrading.y"
    { 
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(1, NULL, &(yyvsp[(1) - (1)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = EXP_CONSTANT; 
	}
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1493 "SysTrading.y"
    {
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(2, (yyvsp[(1) - (3)].expr).sStr, &(yyvsp[(3) - (3)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = EXP_CONSTANT;
	}
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1502 "SysTrading.y"
    { 
		(yyval.expr).sStr[0] = '\0'; (yyval.expr).cExprKind = EXP_CONSTANT; 
//		::fwrite(")\n", 1, 2, fpOutput);
//		if (FuncArgsNew($$.sStr) < 0) return 0;
	}
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1508 "SysTrading.y"
    { 
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(3, NULL, &(yyvsp[(1) - (1)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = EXP_CONSTANT; 
	}
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1516 "SysTrading.y"
    {
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(4, (yyvsp[(1) - (3)].expr).sStr, &(yyvsp[(3) - (3)].expr), (yyval.expr).sStr);
		if(nError)
			return nError;
		(yyval.expr).cExprKind = EXP_CONSTANT;
	}
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1526 "SysTrading.y"
    {
		(yyval.expr).sStr[0] = '\0';
	}
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1530 "SysTrading.y"
    {
		(yyval.expr) = (yyvsp[(1) - (1)].expr);
	}
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1534 "SysTrading.y"
    {
		// Parse the default argument list sentence.
		int nError = g_pSysTradingParser->ParseDefaultArgumentListSentence((yyvsp[(1) - (3)].expr).sStr, (yyvsp[(3) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1544 "SysTrading.y"
    {
		// Write the default argument list sentence.
		int nError = g_pSysTradingParser->WriteDefaultArgumentListSentence((yyvsp[(3) - (4)].expr).cExprKind, (yyvsp[(1) - (4)].name), (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1554 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_STRING((yyval.expr).cExprKind);
	}
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1559 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_STRING((yyval.expr).cExprKind);
		SET_SIMPLE((yyval.expr).cExprKind);
	}
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1565 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_STRING((yyval.expr).cExprKind);
		SET_ARRAY((yyval.expr).cExprKind);
	}
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1571 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_DOUBLE((yyval.expr).cExprKind);
	}
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1576 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_DOUBLE((yyval.expr).cExprKind);
		SET_SIMPLE((yyval.expr).cExprKind);
	}
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1582 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_DOUBLE((yyval.expr).cExprKind);
		SET_ARRAY((yyval.expr).cExprKind);
	}
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1588 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_BOOL((yyval.expr).cExprKind);
	}
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1593 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_BOOL((yyval.expr).cExprKind);
		SET_SIMPLE((yyval.expr).cExprKind);
	}
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1599 "SysTrading.y"
    {
		(yyval.expr).cExprKind = 0;
		SET_BOOL((yyval.expr).cExprKind);
		SET_ARRAY((yyval.expr).cExprKind);
	}
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1607 "SysTrading.y"
    {}
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1608 "SysTrading.y"
    {}
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1609 "SysTrading.y"
    {}
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1610 "SysTrading.y"
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

  case 226:
/* Line 1792 of yacc.c  */
#line 1622 "SysTrading.y"
    {}
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1623 "SysTrading.y"
    {}
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1624 "SysTrading.y"
    {}
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1625 "SysTrading.y"
    {
		g_pSysTradingParser->IsEndBlock(TRUE);
		// Write the for_start sentence.
		int nError = g_pSysTradingParser->WriteForStartSentence(&(yyvsp[(2) - (7)].expr), (yyvsp[(4) - (7)].expr).sStr, (yyvsp[(6) - (7)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1635 "SysTrading.y"
    {}
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1640 "SysTrading.y"
    { 
//		strcpy($$.sStr, "{\npSTActor->m_STDataManager.BracketIn();\n");
//		::fwrite($$.sStr, 1, ::strlen($$.sStr), fpOutput);
	}
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1648 "SysTrading.y"
    { 
		g_pSysTradingParser->IsEndBlock(TRUE);
		int nError = g_pSysTradingParser->EndBlock(NULL);
		if(nError)
			return nError;
	}
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1655 "SysTrading.y"
    { 
		g_pSysTradingParser->IsEndBlock(TRUE);
		int nError = g_pSysTradingParser->EndBlock(NULL);
		if(nError)
			return nError;
	}
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1662 "SysTrading.y"
    { 
		g_pSysTradingParser->IsEndBlock(FALSE);
		int nError = g_pSysTradingParser->ProcessEndStatment();
		if(nError)
			return nError;
	}
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1672 "SysTrading.y"
    {
		g_pSysTradingParser->IsEndBlock(TRUE);
		g_pSysTradingParser->IsEndOneBlock();
		// Write the if_start sentence.
		int nError = g_pSysTradingParser->WriteIfStartSentence(1, (yyvsp[(2) - (3)].expr).sStr, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1684 "SysTrading.y"
    {
		g_pSysTradingParser->DelEndBlock();
		// Write the else sentence.
		int nError = g_pSysTradingParser->WriteIfStartSentence(2, NULL, (yyval.expr).sStr);
		if(nError)
			return nError;
	}
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1695 "SysTrading.y"
    {
		g_pSysTradingParser->DelEndBlock();
	}
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1702 "SysTrading.y"
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

  case 239:
/* Line 1792 of yacc.c  */
#line 1715 "SysTrading.y"
    {
		// Parse the member function sentence.	(CROSS ABOVE)
		int nError = g_pSysTradingParser->ParseInnerFunctionSentence(1, &(yyvsp[(1) - (4)].expr), (yyvsp[(4) - (4)].expr).sStr, (yyval.name));
		if(nError)
			return nError;
	}
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1725 "SysTrading.y"
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

  case 241:
/* Line 1792 of yacc.c  */
#line 1738 "SysTrading.y"
    {
		// Parse the member function sentence. (CROSS BELOW)
		int nError = g_pSysTradingParser->ParseInnerFunctionSentence(2, &(yyvsp[(1) - (4)].expr), (yyvsp[(4) - (4)].expr).sStr, (yyval.name));
		if(nError)
			return nError;
	}
    break;


/* Line 1792 of yacc.c  */
#line 4414 "SysTrading.cpp"
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
#line 1747 "SysTrading.y"


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