
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DeclareDefine.h"
#include "../../include/DeclareDefine.h"
#include "SysTrading_YESParser.h"
#include "SysTrading_YES.cpp.h"

SysTrading_YESParser* g_pLEX_SysTradingParser;

#define YY_INPUT(buf, result, max_size) { result = my_yyinput(buf, max_size); }

int my_yyinput(char* buf, int max_size)
{
    int nSize = __min(max_size, g_pLEX_SysTradingParser->myinputlim - g_pLEX_SysTradingParser->myinputptr);
	if(nSize > 0)
	{
	    memcpy(buf, g_pLEX_SysTradingParser->myinputptr, nSize);
		g_pLEX_SysTradingParser->myinputptr += nSize;
	}

	return nSize;
}

#define TOKEN_NAME_SZ	64
#define NULL_TOKEN		0

static int giLineNo;

static char *gsBuf = NULL;
static int   giLen = 0;
static int   giPos = 0;
static int	 giTokenListCnt = 0;		

static int liInitFlag = 0;

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
		"NUMERIC", DEF_NUMERIC,
		"NUMERICREF", DEF_NUMERIC,
		"NUMERICSERIES", DEF_NUMERIC,
		"TRUEFALSE", DEF_BOOL,
		"TRUEFALSEREF", DEF_BOOL,
		"TRUEFALSESERIES", DEF_BOOL,
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

	char* szA = ::strdup(a->sName);
	::strupr(szA);

	int nCmp = ::lstrcmp(szA, b->sName);

	::free(szA);
	return nCmp;
}

int TokenFindComp(const void *A, const void *B)
{
	char *a = (char *)A;
	LEX_TK_INFO *b = (LEX_TK_INFO *)B;

	return strcmp(a, b->sName);	
}

int LEX_CheckEOF()
{
//	char a;

//	if (getc(a) == EOF) return 1;

	return -1;
}

void LEX_DeleteCurBuffer()
{
	if(yy_current_buffer)
		yy_delete_buffer(yy_current_buffer);
}

int LEX_Init(char *psBuf, int piLen)
{
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
//	int c;

//	if (LEX_CheckEOF() < 1)
//	{
		/* 버퍼에 데이터가 남아 있는경우 버퍼를 비운다. */
//		while ((c = yyinput()));
//	}

	LEX_DeleteCurBuffer();

	liInitFlag = 0;
	giLineNo = 0;

	gsBuf = NULL;
	giLen = 0;
	giPos = 0;
	giTokenListCnt = 0;	
		
	return 1;
}

int LEX_GetLineNumber()
{
	return giLineNo;
}

void LEX_InitLineNumber()
{
	giLineNo = 0;
}

void LEX_SetSysTradingParser(SysTrading_YESParser* p_pParser)
{
	g_pLEX_SysTradingParser = p_pParser;
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
"("						{ 
							g_pLEX_SysTradingParser->IncreaseParenthesis();
							return LPAREN; 
						}
")"						{ 
							g_pLEX_SysTradingParser->DecreaseParenthesis();
							return RPAREN; 
						}
"["						{ return LBRAC; }
"]"						{ return RBRAC; }
":"						{ return COLON; }
";"						{ return SEMICOLON; }
"&&"					{ return AND; }
"||"					{ return OR; }
"!"						{ return NOT; }

{A}{N}{D}				{ return AND; }
"{" |
{B}{E}{G}{I}{N}			{ return BBEGIN; }
{D}{O}{W}{N}{T}{O}		{ return DOWNTO; }
{E}{L}{S}{E}			{ return ELSE; }
"}" |
{E}{N}{D}				{ return BEND; }
{F}{O}{R}				{ return FOR; }
{I}{F}					{ return IF; }
{O}{R}					{ return OR; }
{T}{H}{E}{N}			{ return THEN; }
{T}{O}					{ return TO; }
{W}{H}{I}{L}{E}			{ return WHILE; }

\"[^\"\n]*\"			{ 
	if (yyleng > 1024 - 1) 
	{
		g_pLEX_SysTradingParser->GetParseErrorIndex(3, yytext);
		return 0;
//		strcpy(SysTrading_YESlval.expr.sStr, "\"String error\"");
//		ErrInsert(CNVT_ERROR, ERR_STR, giLineNo, NULL, NULL);	
	}
	else
	{
		::memset(SysTrading_YESlval.name, 0x00, NAME_SZ);
		::strncpy(SysTrading_YESlval.name, yytext, min(yyleng, NAME_SZ - 1));
	}
	return STRING; }

\"[^\"\n]*$			{
	g_pLEX_SysTradingParser->GetParseErrorIndex(3, yytext);
//	strcpy(SysTrading_YESlval.expr.sStr, "\"String error\"");
//	ErrInsert(CNVT_ERROR, ERR_STR, giLineNo, NULL, NULL);	
	return 0;}

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
{I} |
{I}{N}{D}{E}{X}			{ return INDEX; }
{M} |
{M}{O}{N}{E}{Y}			{ return MONEY; }
{B}{I}{D}{S}			{ return BIDS; }
{A}{S}{K}{S}			{ return ASKS; }
{T}{I}{C}{K}{S}			{ return TICKS; }
{U}{P}{T}{I}{C}{K}{S}	{ return UPTICKS; }
{D}{O}{W}{N}{T}{I}{C}{K}{S}	{ return DOWNTICKS; }
{U}{P}{V}{O}{L}			{ return UPVOL; }
{D}{O}{W}{N}{V}{O}{L}	{ return DOWNVOL; }

{D}{A}{T}{E}			{ return DATE; }
{S}{D}{A}{T}{E}			{ return SDATE; }
{T}{I}{M}{E}			{ return TIME; }
{S}{T}{I}{M}{E}			{ return STIME; }

{O}{I} |
{O}{P}{E}{N}{I}{N}{T}{E}{R}{E}{S}{T}	{ return OPENINTEREST; }
{B}{P} |
{B}{A}{S}{E}{P}{R}{I}{C}{E}				{ return BASEPRICE; }

[V][A][R][1-9]? {
	::memset(SysTrading_YESlval.name, 0x00, NAME_SZ);
	::strncpy(SysTrading_YESlval.name, yytext, min(yyleng, NAME_SZ - 1));
	return  RESERVED_VAR;
}

{T}{R}{U}{E} { 
	strcpy(SysTrading_YESlval.name, "TRUE"); 
	return BOOL_TYPE; 
}

{F}{A}{L}{S}{E} {
	strcpy(SysTrading_YESlval.name, "FALSE");
	return BOOL_TYPE;
}

([0-9]+\.) |
(([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) { 
	::memset(SysTrading_YESlval.name, 0x00, NAME_SZ);
	::strncpy(SysTrading_YESlval.name, yytext, min(yyleng, NAME_SZ - 1));
	return DBL_NUMBER; 
}

[0-9]+ { 
	::memset(SysTrading_YESlval.name, 0x00, NAME_SZ);
	::strncpy(SysTrading_YESlval.name, yytext, min(yyleng, NAME_SZ - 1));
	return INT_NUMBER; 
}

([_a-zA-Z])([_a-zA-Z0-9])* {
	liInitFlag = 0;

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
				/*STRCPY(SysTrading_YESlval.name, sizeof(SysTrading_YESlval.name)-1, yytext, yyleng);*/
				return ltpToken->iTokenValue;
			}
		}
	}
	
	if (!liInitFlag)
	{
		::memset(SysTrading_YESlval.name, 0x00, NAME_SZ);
		::strncpy(SysTrading_YESlval.name, yytext, min(yyleng, NAME_SZ - 1));
		return NAME;
	}
}

[\t ]		{ /* white char */ ; }
[\r ]		{ /* carriage return */	;		}
\n			{ giLineNo++; }
[/][/].*\n	{ giLineNo++; }	
#.*\n		{ giLineNo++; }	
.			{ }

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
				else if (c == -1)
					break;
			}

			if (c == -1)
			{
				/* comment error! */
				giLineNo = liOldLineNo;
				g_pLEX_SysTradingParser->GetParseErrorIndex(4);
//				ErrInsert(CNVT_ERROR, ERR_CMMT, liOldLineNo, NULL, NULL);	
				return 1;
			}
		} 
%%

#ifndef yywrap
yywrap() { return 1; }
#endif