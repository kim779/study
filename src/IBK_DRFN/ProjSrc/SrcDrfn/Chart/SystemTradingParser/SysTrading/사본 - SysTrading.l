
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DeclareDefine.h"
#include "SysTrading.cpp.h"

#define TOKEN_NAME_SZ	64
#define NULL_TOKEN		0

static int giLineNo;

static char *gsBuf = NULL;
static int   giLen = 0;
static int   giPos = 0;
static int	 giTokenListCnt = 0;		

typedef struct _LEX_TOKEN
{
	char	sName[NAME_SZ + 1];
	int		iTokenValue;
} LEX_TK_INFO;

LEX_TK_INFO gtpTkList[] = {
		"A", NULL_TOKEN,
		"AN", NULL_TOKEN,
		"AT", NULL_TOKEN,
		"BY", NULL_TOKEN,
		"DOES", NULL_TOKEN,
		"IS", NULL_TOKEN,
		"OF", NULL_TOKEN,
		"ON", NULL_TOKEN,
		"THAN", NULL_TOKEN,
		"THE", NULL_TOKEN,
		"WAS", NULL_TOKEN, 
		"ARRAY", ARRAY,
		"INPUT", INPUT,
		"INPUTS", INPUT,
		"VAR", VARIABLE,
		"VARIABLE", VARIABLE,
		"VARIABLES", VARIABLE,
		"STRING", DEF_STRING,
		"STRINGREF", DEF_STRING,
		"STRINGSERIES", DEF_STRING,
		"STRINGSIMPLE", DEF_STRING_SIMPLE,
		"STRINGARRAY", DEF_STRING_ARRAY,
		"STRINGARRAYREF", DEF_STRING_ARRAY,
		"NUMERIC", DEF_NUMERIC,
		"NUMERICREF", DEF_NUMERIC,
		"NUMERICSERIES", DEF_NUMERIC,
		"NUMERICSIMPLE", DEF_NUMERIC_SIMPLE,
		"NUMERICARRAY", DEF_NUMERIC_ARRAY,
		"NUMERICARRAYREF", DEF_NUMERIC_ARRAY,
		"TRUEFALSE", DEF_BOOL,
		"TRUEFALSEREF", DEF_BOOL,
		"TRUEFALSESERIES", DEF_BOOL,
		"TRUEFALSESIMPLE", DEF_BOOL_SIMPLE,
		"TRUEFALSEARRAY", DEF_BOOL_ARRAY,
		"TRUEFALSEARRAYREF", DEF_BOOL_ARRAY,
		NULL, NULL_TOKEN };

int CountTokenList()
{
	int i = 0;

	while (1)
	{
		if (gtpTkList[i].sName[0] == '\0')	
		{
			return i + 1;
		}

		i++;
	}

	return 0;
}

int TokenSortComp(const void *A, const void *B)
{
	LEX_TK_INFO *a = (LEX_TK_INFO *)A;
	LEX_TK_INFO *b = (LEX_TK_INFO *)B;

	return strcmp(a->sName, b->sName);
}

int TokenFindComp(const void *A, const void *B)
{
	char *a = (char *)A;
	LEX_TK_INFO *b = (LEX_TK_INFO *)B;

	return strcmp(a, b->sName);	
}

int LEX_CheckEOF()
{
	if (getchar() == EOF) return 1;

	return -1;
}

int LEX_Init(char *psBuf, int piLen)
{
	static int liInitFlag = 0;

	BEGIN (INITIAL);

//	if (psBuf == NULL) return -1;
//	if (piLen <= 0) return -2;

	if (!liInitFlag)
	{
		giTokenListCnt = CountTokenList();

		qsort(gtpTkList, giTokenListCnt, sizeof(LEX_TK_INFO), TokenSortComp);

		liInitFlag = 1;
	}

//	gsBuf = psBuf;
//	giLen = piLen;
	giPos = 0;

	return 1;
}

int LEX_Close()
{
	int c;

	if (LEX_CheckEOF() < 1)
	{
		/* 버퍼에 데이터가 남아 있는경우 버퍼를 비운다. */
		while ((c = getchar()));
	}

	return 1;
}

%}

%a 30000
%o 60000
%e 2000
%p 6000
%n 1000

A       [aA]
B       [bB]
C       [cC]
D       [dD]
E       [eE]
F       [fF]
G       [gG]
H       [hH]
I       [iI]
J       [jJ]
K       [kK]
L       [lL]
M       [mM]
N       [nN]
O       [oO]
P       [pP]
Q       [qQ]
R       [rR]
S       [sS]
T       [tT]
U       [uU]
V       [vV]
W       [wW]
X       [xX]
Y       [yY]
Z       [zZ] 

%%
","						{ return COMMA; }
"="						{ return EQ; }
"=="					{ return EQ; }
"<>"					{ return NE; }
"!="					{ return NE; }
"<"						{ return LT; }
"<="					{ return LE; }
">"						{ return GT; }
">="					{ return GE; }
"+"						{ return PLUS; }
"-"						{ return MINUS; }
"*"						{ return MULT; }
"/"						{ return DIVIDE; }
"%"						{ return MOD; }
"("						{ return LPAREN; }
")"						{ return RPAREN; }
"["						{ return LBRAC; }
"]"						{ return RBRAC; }
":"						{ return COLON; }
";"						{ return SEMICOLON; }
"&&"					{ return AND; }
"||"					{ return OR; }
"!"						{ return NOT; }

{A}{N}{D}				{ return AND; }
{B}{E}{G}{I}{N}			{ return BBEGIN; }
{B}{R}{E}{A}{K}			{ return BREAK; }
{D}{I}{V}				{ return MOD; }
{D}{O}					{ return DO; }
{D}{O}{W}{N}			{ return DOWN; }
{D}{O}{W}{N}{T}{O}		{ return DOWNTO; }
{E}{L}{S}{E}			{ return ELSE; }
{E}{L}{S}{E}{I}{F}		{ return ELSEIF; }
{E}{N}{D}				{ return BEND; }
{F}{O}{R}				{ return FOR; }
{F}{U}{N}{C}{T}{I}{O}{N}	{return FUNCTION; }
{I}{F}					{ return IF; }
{N}{O}{T}				{ return NOT; }
{O}{R}					{ return OR; }
{T}{H}{E}{N}			{ return THEN; }
{T}{O}					{ return TO; }
{W}{H}{I}{L}{E}			{ return WHILE; }

\"[^\"\n]*\"			{ 
	if (yyleng > 1024 - 1) 
	{
		strcpy(SysTradinglval.expr.sStr, "\"String error\"");
//		ErrInsert(CNVT_ERROR, ERR_STR, giLineNo, NULL, NULL);	
	}
	else
	{
		strncpy(SysTradinglval.name, yytext, min(yyleng, NAME_SZ - 1));
	}
	return STRING; }

\"[^\"\n]*$			{
	strcpy(SysTradinglval.expr.sStr, "\"String error\"");
//	ErrInsert(CNVT_ERROR, ERR_STR, giLineNo, NULL, NULL);	
	return STRING; }

{J}{M}{C}{O}{D}{E}		{ return JMCODE;  }

{C} |
{C}{L}{O}{S}{E}			{ return CLOSE;  }
{O} |
{O}{P}{E}{N}			{ return OPEN;   }
{H} |
{H}{I}{G}{H}			{ return HIGH;   }
{L} |
{L}{O}{W}				{ return LOW;    }
{V} |
{V}{O}{L}{U}{M}{E}		{ return VOLUME; }
{A}{M}{O}{U}{N}{T}		{ return AMOUNT; }

{D} |
{D}{A}{T}{E}			{ return DATE; }
{T} |
{T}{I}{M}{E}			{ return TIME; }
{D}{A}{T}{A}{C}{N}{T}	{ return DATACNT; }

{C}{U}{R}{R}{E}{N}{T}{D}{A}{T}{E}		{ return CURRENTDATE; }
{C}{U}{R}{R}{E}{N}{T}{T}{I}{M}{E}		{ return CURRENTTIME; }

[V][A][L][U][E][1-9][0-9]? {
	strncpy(SysTradinglval.name, yytext, min(yyleng, NAME_SZ - 1));
	return  RESERVED_VAR;
}

[C][O][N][D][1-9][0-9]? {
	strncpy(SysTradinglval.name, yytext, min(yyleng, NAME_SZ - 1));
	return  RESERVED_COND;
}

[T][E][X][T][1-9][0-9]? {
	strncpy(SysTradinglval.name, yytext, min(yyleng, NAME_SZ - 1));
	return  RESERVED_TEXT;
}

{T}{R}{U}{E} { strcpy(SysTradinglval.name, "TRUE"); 
	return BOOL_TYPE; 
}

{F}{A}{L}{S}{E} {
	strcpy(SysTradinglval.name, "FALSE");
	return BOOL_TYPE;
}

([0-9]+\.) |
(([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) { 
	strncpy(SysTradinglval.name, yytext, min(yyleng, NAME_SZ - 1));
	return DBL_NUMBER; 
}

[0-9]+ { 
	strncpy(SysTradinglval.name, yytext, min(yyleng, NAME_SZ - 1));
	return INT_NUMBER; 
}

([_a-zA-Z])([_a-zA-Z0-9])* {
	int liInitFlag = 0;

	if (giTokenListCnt > 0 && yyleng <= TOKEN_NAME_SZ)
	{
		LEX_TK_INFO *ltpToken;

		ltpToken = (LEX_TK_INFO *)bsearch(yytext, gtpTkList, giTokenListCnt, 
					sizeof(LEX_TK_INFO), TokenSortComp);

		if (ltpToken != NULL)
		{
			liInitFlag = 1;
			if (ltpToken->iTokenValue != NULL_TOKEN)
			{
				/*STRCPY(SysTradinglval.name, sizeof(SysTradinglval.name)-1, yytext, yyleng);*/
				return ltpToken->iTokenValue;
			}
		}
	}
	
	if (!liInitFlag)
	{
		strncpy(SysTradinglval.name, yytext, min(yyleng, NAME_SZ - 1));
		return NAME;
	}
}

[\t ]		{ /* white char */ ; }
[\r ]		{ /* carriage return */	;		}
\n			{ giLineNo++; }
[/][/].*\n	{ giLineNo++; }	
#.*\n		{ giLineNo++; }	
.			{ /*SysTradingerror("illegal token"); */}

"(*" 	{
			register int c;
			int liOldLineNo = giLineNo;

			while ((c = yyinput()))
			{
				if (c == '*')
				{
					if ((c = yyinput()) == ')')
						break;
					else
						unput(c);
				}          
				else if (c == '\n')
					giLineNo++;
			}

			if (c == 0)
			{
				/* comment error! */
//				ErrInsert(CNVT_ERROR, ERR_CMMT, liOldLineNo, NULL, NULL);	
				return 0;
			}
		}

"{"     { 	
			register int c;
			int liOldLineNo = giLineNo;

			while ((c = yyinput()))
			{
           		if (c == '}')
					break;
				else if (c == '\n')
					giLineNo++;
			}

			if (c == 0)
			{
				/* comment error! */
//				ErrInsert(CNVT_ERROR, ERR_CMMT, liOldLineNo, NULL, NULL);	
				return 0;
			}
		}


"/*"	{	
			register int c;
			int liOldLineNo = giLineNo;

			while ((c = yyinput()))
			{
				if (c == '*')
				{
					if ((c = yyinput()) == '/')
						break;
					else
						unput(c);
				}          
				else if (c == '\n')
					giLineNo++;
			}

			if (c == 0)
			{
				/* comment error! */
//				ErrInsert(CNVT_ERROR, ERR_CMMT, liOldLineNo, NULL, NULL);	
				return 0;
			}
		} 
%%

#ifndef yywrap
yywrap() { return 1; }
#endif