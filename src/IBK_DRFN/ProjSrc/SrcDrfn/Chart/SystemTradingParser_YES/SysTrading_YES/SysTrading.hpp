/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
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

#ifndef YY_SYSTRADING_SYSTRADING_HPP_INCLUDED
# define YY_SYSTRADING_SYSTRADING_HPP_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int SysTradingdebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAME = 258,
     BOOL_TYPE = 259,
     INT_NUMBER = 260,
     DBL_NUMBER = 261,
     RESERVED_VAR = 262,
     RESERVED_COND = 263,
     RESERVED_TEXT = 264,
     STRING = 265,
     ARRAY = 266,
     INPUT = 267,
     VARIABLE = 268,
     DEF_STRING = 269,
     DEF_STRING_SIMPLE = 270,
     DEF_STRING_ARRAY = 271,
     DEF_NUMERIC = 272,
     DEF_NUMERIC_SIMPLE = 273,
     DEF_NUMERIC_ARRAY = 274,
     DEF_BOOL = 275,
     DEF_BOOL_SIMPLE = 276,
     DEF_BOOL_ARRAY = 277,
     JMCODE = 278,
     CLOSE = 279,
     OPEN = 280,
     HIGH = 281,
     LOW = 282,
     VOLUME = 283,
     AMOUNT = 284,
     DATE = 285,
     TIME = 286,
     DATACNT = 287,
     CURRENTDATE = 288,
     CURRENTTIME = 289,
     CURRENTBAR = 290,
     STMT_SUFFIX = 291,
     COMMA = 292,
     EQ = 293,
     NE = 294,
     LT = 295,
     LE = 296,
     GT = 297,
     GE = 298,
     PLUS = 299,
     MINUS = 300,
     MULT = 301,
     DIVIDE = 302,
     LPAREN = 303,
     RPAREN = 304,
     LBRAC = 305,
     RBRAC = 306,
     COLON = 307,
     SEMICOLON = 308,
     STAR = 309,
     STARSTAR = 310,
     UPARROW = 311,
     AND = 312,
     BBEGIN = 313,
     BREAK = 314,
     DO = 315,
     DOWN = 316,
     DOWNTO = 317,
     ELSE = 318,
     ELSEIF = 319,
     BEND_SEMICOLON = 320,
     BEND = 321,
     FOR = 322,
     FUNCTION = 323,
     IF = 324,
     OR = 325,
     MOD = 326,
     NOT = 327,
     THEN = 328,
     TO = 329,
     WHILE = 330,
     CROSS = 331,
     ABOVE = 332,
     BELOW = 333
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 13 "SysTrading.y"

			char	name[NAME_SZ];
			int		ival;
			double	dval;
			EXPR	expr;
		

/* Line 2058 of yacc.c  */
#line 143 "SysTrading.hpp"
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
