%{
//#include <iostream.h>
#include "DeclareDefine.h"
#include "SysTradingParser.h"

extern int SysTradingerror(const char* s);
extern int SysTradinglex();

SysTradingParser* g_pSysTradingParser;

%}

%union  {
			char	name[NAME_SZ];
			int		ival;
			double	dval;
			EXPR	expr;
		}

%token		<name>	NAME
%token		<name>	BOOL_TYPE
%token		<name>	INT_NUMBER
%token		<name>	DBL_NUMBER
%token		<name>	RESERVED_VAR
%token		<name>	RESERVED_COND
%token		<name>	RESERVED_TEXT

%token		<expr>	STRING

%token ARRAY
%token INPUT
%token VARIABLE

%token DEF_STRING
%token DEF_STRING_SIMPLE
%token DEF_STRING_ARRAY
%token DEF_NUMERIC
%token DEF_NUMERIC_SIMPLE
%token DEF_NUMERIC_ARRAY
%token DEF_BOOL
%token DEF_BOOL_SIMPLE
%token DEF_BOOL_ARRAY

%token JMCODE
%token CLOSE
%token OPEN
%token HIGH
%token LOW
%token VOLUME
%token AMOUNT
%token DATE
%token TIME
%token DATACNT
%token CURRENTDATE
%token CURRENTTIME
%token CURRENTBAR

%token STMT_SUFFIX
%token COMMA
%token EQ
%token NE
%token LT
%token LE
%token GT
%token GE
%token PLUS
%token MINUS
%token MULT
%token DIVIDE
%token LPAREN
%token RPAREN
%token LBRAC
%token RBRAC
%token COLON
%token SEMICOLON
%token STAR
%token STARSTAR
%token UPARROW

%token AND
%token BBEGIN
%token BREAK
%token DO
%token DOWN
%token DOWNTO
%token ELSE
%token ELSEIF
%token BEND_SEMICOLON
%token BEND
%token FOR
%token FUNCTION
%token IF
%token OR
%token MOD
%token NOT
%token THEN
%token TO
%token WHILE
%token CROSS
%token ABOVE
%token BELOW

%type 		<expr>	data_list reserved_list expr_list
%type 		<expr>	cond_expr cond_expr2 cond_expr3
%type 		<expr>	num_expr name_expr expr expr2 expr3 expr4 assign_expr
%type 		<expr>	reserved_arr reserved_sise reserved_cond reserved_text
%type		<expr>	array_expr array_idx_list array_idx_list2
%type 		<expr>	def_arg_list def_arg_list2 data_type_list
%type		<name>	num_list
%type		<name>	array_list4
%type		<expr>	array_list3
%type 		<expr>	function 
%type		<expr>	input_list2 function_name function_name2
%type		<name>	variable_list2 array_list2
%type 		<expr>	func_arg_list func_arg_list2
%type 		<expr>	for_start loop_break
%type 		<expr>	block_begin block_end
%type 		<expr>	if_start if_else if_else_if
%type		<name>	cross_above cross_below

%%

stmt_list:
	stmt
	{
//		g_pSysTradingParser->WriteOneStatementEnd($1.sStr);
	}
|	stmt_list stmt

stmt:
|	stmt_suffix
|	input_stmt stmt_suffix
|	variable_stmt stmt_suffix
|	array_stmt stmt_suffix
|	expr stmt_suffix 
	{
		// Write the general statement.
		int nError = g_pSysTradingParser->WriteGeneralSentence($1.sStr);
		if(nError)
			return nError;
	}
|	assign_expr stmt_suffix
	{
		// Write the assign statement.
		int nError = g_pSysTradingParser->WriteAssignSentence($1.sStr);
		if(nError)
			return nError;
	}
|	loop_break stmt_suffix {}
|   for_start stmt_suffix 
	{
		g_pSysTradingParser->AddStack(STACK_FOR);
	}
|   block_begin stmt_suffix 
	{
		g_pSysTradingParser->ChangeBlockStack();
	}
|   block_end stmt_suffix 
	{
//		g_pSysTradingParser->IsEndBlock(TRUE);
//		int nError = g_pSysTradingParser->EndBlock($1.sStr);
//		if(nError)
//			return nError;
	}
|   if_start stmt_suffix 
	{
//		g_pSysTradingParser->IsEndBlock(TRUE);
		g_pSysTradingParser->AddStack(STACK_IF);
	}
|   if_else stmt_suffix
	{
//		g_pSysTradingParser->DelEndBlock();
		g_pSysTradingParser->AddStack(STACK_ELSE);
	}
|   if_else_if stmt_suffix
	{
//		g_pSysTradingParser->DelEndBlock();
		g_pSysTradingParser->AddStack(STACK_ELSEIF);
	}

stmt_suffix:
|	STMT_SUFFIX

input_stmt:
	INPUT COLON input_list SEMICOLON {}

input_list:
	input_list2 {}
|	input_list COMMA input_list2 {}
|	def_arg_list {}
|	input_list input_list2 
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(2);
		if(nError)
			return nError;
	}
|	Declare_Error {}

Declare_Error:
	NAME
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, $1);
		if(nError)
			return nError;
	}
|	NAME LPAREN
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, $1);
		if(nError)
			return nError;
	}
|	NAME LPAREN data_list
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, $1);
		if(nError)
			return nError;
	}
|	NAME RPAREN
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, $1);
		if(nError)
			return nError;
	}
|	NAME data_list RPAREN 
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, $1);
		if(nError)
			return nError;
	}
|	NAME LPAREN RPAREN
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(1, $1);
		if(nError)
			return nError;
	}

input_list2:
	NAME LPAREN data_list RPAREN 
	{
		// Write the declare sentence.
		int nError = g_pSysTradingParser->WriteDeclareSentence($3.cExprKind, $1, &$3, $$.sStr);
		if(nError)
			return nError;
	}
|	NAME LPAREN expr RPAREN 
	{
		$3.cExprKind = DTTYPE_INT;
		// Write the declare sentence.
		int nError = g_pSysTradingParser->WriteDeclareSentence($3.cExprKind, $1, &$3, $$.sStr);
		if(nError)
			return nError;
	}

cond_expr:
	cond_expr2
	{
		// Parse the condition sentence (Normal)
		$$ = $1; 
	}
|	cond_expr2 SEMICOLON 
	{ 
		// Parse the condition sentence (Semicolon)
		int nError = g_pSysTradingParser->ParseConditionSentence($1.sStr, NULL, $$.sStr, 1);
		if(nError)
			return nError;
	}
|	cond_expr OR cond_expr2 
	{ 
		// Parse the condition sentence (Or)
		int nError = g_pSysTradingParser->ParseConditionSentence($1.sStr, $3.sStr, $$.sStr, 51);
		if(nError)
			return nError;
//		or( &$$, &$1, &$3); 
	}
|	cond_expr OR cond_expr2 SEMICOLON 
	{ 
		// Parse the condition sentence (Or)
		int nError = g_pSysTradingParser->ParseConditionSentence($1.sStr, $3.sStr, $$.sStr, 51);
		if(nError)
			return nError;

		::lstrcat($$.sStr, ";");
//		or( &$$, &$1, &$3); strcat($$.sStr, ";");
	}

cond_expr2:
	cond_expr3				
	{ 
		$$ = $1; 
	}
|	cond_expr2 AND cond_expr3 
	{ 
		// Parse the condition sentence (And)
		int nError = g_pSysTradingParser->ParseConditionSentence($1.sStr, $3.sStr, $$.sStr, 52);
		if(nError)
			return nError;
//		and( &$$, &$1, &$3); 
	}

cond_expr3:
	LPAREN cond_expr RPAREN  
	{ 
		// Parse the condition sentence (Parenthesis)
		int nError = g_pSysTradingParser->ParseConditionSentence($2.sStr, NULL, $$.sStr, 2);
		$$.cExprKind = EXP_COMPLEX;
		if(nError)
			return nError;
	}
|	expr2          
	{ 
		$$ = $1; 
	}
|	NOT expr2 
	{ 
		// Parse the condition sentence (Not)
		int nError = g_pSysTradingParser->ParseConditionSentence($2.sStr, NULL, $$.sStr, 50);
		if(nError)
			return nError;
//		not(&$$, &$2); 
	}
|	expr2 EQ expr2 
	{ 
		// Parse the condition sentence (Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence($1.sStr, $3.sStr, $$.sStr, 3);
		if(nError)
			return nError;
//		eq( &$$, &$1, &$3); 
	}
|	expr2 NE expr2 
	{ 
		// Parse the condition sentence (Not Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence($1.sStr, $3.sStr, $$.sStr, 8);
		if(nError)
			return nError;
//		ne( &$$, &$1, &$3); 
	}
|	expr2 LT expr2 
	{ 
		// Parse the condition sentence (Less Then)
		int nError = g_pSysTradingParser->ParseConditionSentence($1.sStr, $3.sStr, $$.sStr, 4);
		if(nError)
			return nError;
//		lt( &$$, &$1, &$3); 
	}
|	expr2 LE expr2 
	{ 
		// Parse the condition sentence (Less Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence($1.sStr, $3.sStr, $$.sStr, 5);
		if(nError)
			return nError;
//		le( &$$, &$1, &$3); 
	}
|	expr2 GT expr2 
	{ 
		// Parse the condition sentence (Greater Then)
		int nError = g_pSysTradingParser->ParseConditionSentence($1.sStr, $3.sStr, $$.sStr, 6);
		if(nError)
			return nError;
//		gt( &$$, &$1, &$3); 
	}
|	expr2 GE expr2 
	{ 
		// Parse the condition sentence (Greater Equal)
		int nError = g_pSysTradingParser->ParseConditionSentence($1.sStr, $3.sStr, $$.sStr, 7);
		if(nError)
			return nError;
//		ge( &$$, &$1, &$3); 
	}

expr:
	expr2     
	{ 
		$$ = $1; 
	}
|	expr2 SEMICOLON 
	{ 
//		sprintf($$.sStr, "%s;", $1.sStr); 
	}
|	cross_above {}
|	cross_below {}
;

expr2:
	expr3				
	{ 
		$$ = $1; 
	}
|	LPAREN expr2 RPAREN
	{ 
		$$ = $2; 
		::wsprintf($$.sStr, _T("(%s)"), $2.sStr);
	}
|	expr2 PLUS expr3 
	{ 
		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 1);
		if(nError)
			return nError;
//		plus( &$$, &$1, &$3); 
	}
|	expr2 MINUS expr3 
	{ 
		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 2);
		if(nError)
			return nError;
//		minus( &$$, &$1, &$3); 
	}
|	expr2 MOD expr3 
	{ 
		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 3);
		if(nError)
			return nError;
//		mod( &$$, &$1, &$3); 
	}
|	expr2 MULT expr3 
	{ 
		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 4);
		if(nError)
			return nError;
//		multi( &$$, &$1, &$3); 
	}
|	expr2 DIVIDE expr3 
	{ 
		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 5);
		if(nError)
			return nError;
//		divide( &$$, &$1, &$3); 
	}

expr3:
	expr4				
	{ 
		$$ = $1; 
	}
|	LPAREN expr3 RPAREN
	{ 
		$$ = $2; 
		::wsprintf($$.sStr, _T("(%s)"), $2.sStr);
	}

expr4:
	num_expr			 
	{ 
		$$ = $1; 
	}
|	STRING			 
	{ 
		::lstrcpy($$.sStr, $1.sStr);
	}
|	DATACNT			 
	{ 
		::lstrcpy($$.sStr, "giDataCnt");
	} 
|	BOOL_TYPE			 
	{ 
		::lstrcpy($$.sStr, $1);
	}
|	name_expr			 
	{ 
//		::lstrcpy($1.sArg, _T("0"));
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&$1, $$.sStr);
		if(nError)
			return nError;
	}
|	array_expr		 
	{ 
//		$$ = $1; 
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&$1, $$.sStr);
		if(nError)
			return nError;
	}
|	reserved_sise		 
	{ 
		$$ = $1; 
	}
|	reserved_arr		 
	{ 
		$$ = $1; 
	}
|	reserved_cond		 
	{ 
		$$ = $1; 
	}
|	function			 
	{ 
		$$ = $1; 
	}
|	PLUS expr	         
	{ 
		$$ = $2; 
	}
|	MINUS expr   
	{ 
		// Parse the minus value sentence.
		int nError = g_pSysTradingParser->ParseOperatorSentence($2.sStr, NULL, $$.sStr, 6);
		$$.cExprKind = EXP_COMPLEX;
		if(nError)
			return nError;
	}
|	LPAREN expr RPAREN  
	{ 
		// Parse the parenthesis value sentence.
		int nError = g_pSysTradingParser->ParseOperatorSentence($2.sStr, NULL, $$.sStr, 7);
		$$.cExprKind = EXP_COMPLEX;
		if(nError)
			return nError;
	}
|	cross_above
	{
		strcpy($$.sStr, $1);
	}
|	cross_below
	{
		strcpy($$.sStr, $1);
	}

name_expr:
	NAME 
	{ 		
		::lstrcpy($$.sStr, $1);
		::lstrcpy($$.sArg, _T("0"));
//		if (var(&$$, $1) < 0) return 0; 
	}

array_expr:
	NAME LBRAC data_list RBRAC 
	{
		::lstrcpy($$.sArg, $3.sStr);
	}
|	NAME LBRAC array_idx_list RBRAC 
	{
		// Parse the array sentence.
		::lstrcpy($$.sArg, $3.sStr);
//		g_pSysTradingParser->ParseArraySentence($1, $3.sStr, $$.sStr);
//		if (var_array(&$$, $1, &$3) < 0) return 0;
//		ArrayDimDel();
	}

reserved_sise:
	OPEN 
	{ 
		// Parse the reserved sentence. (OPEN)
		int nError = g_pSysTradingParser->ParseReservedSentence(1, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("OPEN"));
		if(nError)
			return nError;
	}
|	OPEN LBRAC expr RBRAC 
	{ 
		// Parse the reserved sentence. (OPEN, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(2, $3.sStr, $$.sStr);
		::wsprintf($$.sStrOrg, _T("OPEN(%s)"), $3.sStr);
		if(nError)
			return nError;
	}
|	HIGH 
	{ 
		// Parse the reserved sentence. (HIGH)
		int nError = g_pSysTradingParser->ParseReservedSentence(3, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("HIGH"));
		if(nError)
			return nError;
	}
|	HIGH LBRAC expr RBRAC 
	{ 
		// Parse the reserved sentence. (HIGH, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(4, $3.sStr, $$.sStr);
		::wsprintf($$.sStrOrg, _T("HIGH(%s)"), $3.sStr);
		if(nError)
			return nError;
	}
|	LOW 
	{ 
		// Parse the reserved sentence. (LOW)
		int nError = g_pSysTradingParser->ParseReservedSentence(5, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("LOW"));
		if(nError)
			return nError;
	}
|	LOW LBRAC expr RBRAC 
	{ 
		// Parse the reserved sentence. (LOW, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(6, $3.sStr, $$.sStr);
		::wsprintf($$.sStrOrg, _T("LOW(%s)"), $3.sStr);
		if(nError)
			return nError;
	}
|	CLOSE 
	{ 
		// Parse the reserved sentence. (CLOSE)
		int nError = g_pSysTradingParser->ParseReservedSentence(7, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("CLOSE"));
		if(nError)
			return nError;
	}
|	CLOSE LBRAC expr RBRAC 
	{ 
		// Parse the reserved sentence. (CLOSE, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(8, $3.sStr, $$.sStr);
		::wsprintf($$.sStrOrg, _T("CLOSE(%s)"), $3.sStr);
		if(nError)
			return nError;
	}
|	VOLUME 
	{ 
		// Parse the reserved sentence. (VOLUME)
		int nError = g_pSysTradingParser->ParseReservedSentence(9, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("VOLUME"));
		if(nError)
			return nError;
	}
|	VOLUME LBRAC expr RBRAC 
	{ 
		// Parse the reserved sentence. (VOLUME, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(10, $3.sStr, $$.sStr);
		::wsprintf($$.sStrOrg, _T("VOLUME(%s)"), $3.sStr);
		if(nError)
			return nError;
	}
|	AMOUNT 
	{ 
		// Parse the reserved sentence. (AMOUNT)
		int nError = g_pSysTradingParser->ParseReservedSentence(11, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("AMOUNT"));
		if(nError)
			return nError;
	}
|	AMOUNT LBRAC expr RBRAC 
	{ 
		// Parse the reserved sentence. (AMOUNT, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(12, $3.sStr, $$.sStr);
		::wsprintf($$.sStrOrg, _T("AMOUNT(%s)"), $3.sStr);
		if(nError)
			return nError;
	}
|	DATE 
	{ 
		// Parse the reserved sentence. (DATE)
		int nError = g_pSysTradingParser->ParseReservedSentence(13, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("DATE"));
		if(nError)
			return nError;
	}
|	DATE LBRAC expr RBRAC 
	{ 
		// Parse the reserved sentence. (DATE, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(14, $3.sStr, $$.sStr);
		::wsprintf($$.sStrOrg, _T("DATE(%s)"), $3.sStr);
		if(nError)
			return nError;
	}
|	TIME 
	{ 
		// Parse the reserved sentence. (TIME)
		int nError = g_pSysTradingParser->ParseReservedSentence(15, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("TIME"));
		if(nError)
			return nError;
	}
|	TIME LBRAC expr RBRAC 
	{ 
		// Parse the reserved sentence. (TIME, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence(16, $3.sStr, $$.sStr);
		::wsprintf($$.sStrOrg, _T("TIME(%s)"), $3.sStr);
		if(nError)
			return nError;
	}
|	JMCODE 
	{ 
		// Parse the reserved sentence. (JMCODE)
		int nError = g_pSysTradingParser->ParseReservedSentence(17, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("JMCODE"));
		if(nError)
			return nError;
	}
|	CURRENTDATE 
	{ 
		// Parse the reserved sentence. (CURRENTDATE)
		int nError = g_pSysTradingParser->ParseReservedSentence(18, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("CURRENTDATE"));
		if(nError)
			return nError;
	}
|	CURRENTTIME 
	{ 
		// Parse the reserved sentence. (CURRENTTIME)
		int nError = g_pSysTradingParser->ParseReservedSentence(19, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("CURRENTTIME"));
		if(nError)
			return nError;
	}
|	CURRENTBAR 
	{ 
		// Parse the reserved sentence. (CURRENTBAR)
		int nError = g_pSysTradingParser->ParseReservedSentence(20, NULL, $$.sStr);
		::lstrcpy($$.sStrOrg, _T("CURRENTBAR"));
		if(nError)
			return nError;
	}

reserved_arr:
	RESERVED_VAR 
	{ 
//		reserved_var(&$$, "VALUE", atoi($1+5), &gtZeroExpr); 
	}
|	RESERVED_VAR LBRAC expr RBRAC 
	{ 
//		reserved_var(&$$, "VALUE", atoi($1+5), &$3); 
	}
;

reserved_cond:
	RESERVED_COND 
	{ 
//		reserved_cond(&$$, "COND", atoi($1 + 4), &gtZeroExpr); 
	}
|	RESERVED_COND LBRAC expr RBRAC 
	{ 
//		reserved_cond(&$$, "COND", atoi($1+4), &$3); 
	}

reserved_text:
	RESERVED_TEXT 
	{ 
//		reserved_var(&$$, "TEXT", atoi($1+4), &gtZeroExpr); 
	}
|	RESERVED_TEXT LBRAC expr RBRAC 
	{ 
//		reserved_var(&$$, "TEXT", atoi($1+4), &$3); 
	}
;

variable_stmt:
	VARIABLE COLON variable_list SEMICOLON {}

variable_list:
	variable_list2 {}
|	variable_list COMMA variable_list2 {}
|	variable_list variable_list2 
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(2);
		if(nError)
			return nError;
	}
|	Declare_Error {}

variable_list2:
	NAME LPAREN data_list RPAREN 
	{
		// Write the variable sentence.
		int nError = g_pSysTradingParser->WriteVariableSentence($3.cExprKind, $1, &$3, $$);
		if(nError)
			return nError;
	}
|	NAME LPAREN expr RPAREN 
	{
		$3.cExprKind = DTTYPE_INT;
		// Write the variable sentence.
		int nError = g_pSysTradingParser->WriteVariableSentence($3.cExprKind, $1, &$3, $$);
		if(nError)
			return nError;
	}
|	NAME LPAREN name_expr RPAREN 
	{
		// Parse the naming sentence.
//		::lstrcpy($3.sArg, _T("0"));
		int nError = g_pSysTradingParser->WriteNameVariableSentence($1, &$3, $$);
		if(nError)
			return nError;
	}
;

data_list:
	INT_NUMBER 
	{
		$$.cExprKind = DTTYPE_INT;
		strcpy($$.sStr, $1);
	}
|	DBL_NUMBER 
	{
		$$.cExprKind = DTTYPE_DOUBLE;
		strcpy($$.sStr, $1);
	}
|	STRING 
	{
		$$.cExprKind = DTTYPE_STRING;
		strcpy($$.sStr, $1.sStr);
	}
|	BOOL_TYPE 
	{
		$$.cExprKind = DTTYPE_BOOL;
		strcpy($$.sStr, $1);
	}
|	reserved_list
|	expr_list
	{
		$$.cExprKind = DTTYPE_INT;
		::lstrcpy($$.sStr, $1.sStr);
	}
;

reserved_list:
	OPEN 
	{ 
		::lstrcpy($$.sStrOrg, _T("OPEN"));

		// Parse the reserved sentence. (OPEN)
		int nError = g_pSysTradingParser->ParseReservedSentence2(1, NULL, &$$);
		if(nError)
			return nError;
	}
|	OPEN LBRAC expr RBRAC 
	{ 
		::lstrcpy($$.sStrOrg, _T("OPEN"));

		// Parse the reserved sentence. (OPEN, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(2, $3.sStr, &$$);
		if(nError)
			return nError;
	}
|	HIGH 
	{ 
		::lstrcpy($$.sStrOrg, _T("HIGH"));

		// Parse the reserved sentence. (HIGH)
		int nError = g_pSysTradingParser->ParseReservedSentence2(3, NULL, &$$);
		if(nError)
			return nError;
	}
|	HIGH LBRAC expr RBRAC 
	{ 
		::lstrcpy($$.sStrOrg, _T("HIGH"));

		// Parse the reserved sentence. (HIGH, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(4, $3.sStr, &$$);
		if(nError)
			return nError;
	}
|	LOW 
	{ 
		::lstrcpy($$.sStrOrg, _T("LOW"));

		// Parse the reserved sentence. (LOW)
		int nError = g_pSysTradingParser->ParseReservedSentence2(5, NULL, &$$);
		if(nError)
			return nError;
	}
|	LOW LBRAC expr RBRAC 
	{ 
		::lstrcpy($$.sStrOrg, _T("LOW"));

		// Parse the reserved sentence. (LOW, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(6, $3.sStr, &$$);
		if(nError)
			return nError;
	}
|	CLOSE 
	{ 
		::lstrcpy($$.sStrOrg, _T("CLOSE"));

		// Parse the reserved sentence. (CLOSE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(7, NULL, &$$);
		if(nError)
			return nError;
	}
|	CLOSE LBRAC expr RBRAC 
	{ 
		::lstrcpy($$.sStrOrg, _T("CLOSE"));

		// Parse the reserved sentence. (CLOSE, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(8, $3.sStr, &$$);
		if(nError)
			return nError;
	}
|	VOLUME 
	{ 
		::lstrcpy($$.sStrOrg, _T("VOLUME"));

		// Parse the reserved sentence. (VOLUME)
		int nError = g_pSysTradingParser->ParseReservedSentence2(9, NULL, &$$);
		if(nError)
			return nError;
	}
|	VOLUME LBRAC expr RBRAC 
	{ 
		::lstrcpy($$.sStrOrg, _T("VOLUME"));

		// Parse the reserved sentence. (VOLUME, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(10, $3.sStr, &$$);
		if(nError)
			return nError;
	}
|	AMOUNT 
	{ 
		::lstrcpy($$.sStrOrg, _T("AMOUNT"));

		// Parse the reserved sentence. (AMOUNT)
		int nError = g_pSysTradingParser->ParseReservedSentence2(11, NULL, &$$);
		if(nError)
			return nError;
	}
|	AMOUNT LBRAC expr RBRAC 
	{ 
		::lstrcpy($$.sStrOrg, _T("AMOUNT"));

		// Parse the reserved sentence. (AMOUNT, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(12, $3.sStr, &$$);
		if(nError)
			return nError;
	}
|	DATE 
	{ 
		::lstrcpy($$.sStrOrg, _T("DATE"));

		// Parse the reserved sentence. (DATE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(13, NULL, &$$);
		if(nError)
			return nError;
	}
|	DATE LBRAC expr RBRAC 
	{ 
		::lstrcpy($$.sStrOrg, _T("DATE"));

		// Parse the reserved sentence. (DATE, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(14, $3.sStr, &$$);
		if(nError)
			return nError;
	}
|	TIME 
	{ 
		::lstrcpy($$.sStrOrg, _T("TIME"));

		// Parse the reserved sentence. (TIME)
		int nError = g_pSysTradingParser->ParseReservedSentence2(15, NULL, &$$);
		if(nError)
			return nError;
	}
|	TIME LBRAC expr RBRAC 
	{ 
		::lstrcpy($$.sStrOrg, _T("TIME"));

		// Parse the reserved sentence. (TIME, Braket)
		int nError = g_pSysTradingParser->ParseReservedSentence2(16, $3.sStr, &$$);
		if(nError)
			return nError;
	}
|	JMCODE 
	{ 
		::lstrcpy($$.sStrOrg, _T("JMCODE"));

		// Parse the reserved sentence. (JMCODE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(17, NULL, &$$);
		if(nError)
			return nError;
	}
|	CURRENTDATE 
	{ 
		::lstrcpy($$.sStrOrg, _T("CURRENTDATE"));

		// Parse the reserved sentence. (CURRENTDATE)
		int nError = g_pSysTradingParser->ParseReservedSentence2(18, NULL, &$$);
		if(nError)
			return nError;
	}
|	CURRENTTIME 
	{ 
		::lstrcpy($$.sStrOrg, _T("CURRENTTIME"));

		// Parse the reserved sentence. (CURRENTTIME)
		int nError = g_pSysTradingParser->ParseReservedSentence2(19, NULL, &$$);
		if(nError)
			return nError;
	}
|	CURRENTBAR 
	{ 
		::lstrcpy($$.sStrOrg, _T("CURRENTBAR"));

		// Parse the reserved sentence. (CURRENTBAR)
		int nError = g_pSysTradingParser->ParseReservedSentence2(20, NULL, &$$);
		if(nError)
			return nError;
	}
;

expr_list:
	data_list PLUS data_list
	{
		if(::lstrlen($3.sStrOrg))
			::wsprintf($$.sStrOrg, _T("%s + %s"), $1.sStr, $3.sStrOrg);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 1);
		if(nError)
			return nError;
	}
|	data_list MINUS data_list
	{
		if(::lstrlen($3.sStrOrg))
			::wsprintf($$.sStrOrg, _T("%s - %s"), $1.sStr, $3.sStrOrg);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 2);
		if(nError)
			return nError;
	}
|	data_list MULT data_list
	{
		if(::lstrlen($3.sStrOrg))
			::wsprintf($$.sStrOrg, _T("%s * %s"), $1.sStr, $3.sStrOrg);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 4);
		if(nError)
			return nError;
	}
|	data_list DIVIDE data_list
	{
		if(::lstrlen($3.sStrOrg))
			::wsprintf($$.sStrOrg, _T("%s / %s"), $1.sStr, $3.sStrOrg);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 5);
		if(nError)
			return nError;
	}
|	data_list MOD data_list 
	{ 
		if(::lstrlen($3.sStrOrg))
			::wsprintf($$.sStrOrg, _T("%s % %s"), $1.sStr, $3.sStrOrg);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 3);
		if(nError)
			return nError;
	}
|	reserved_list PLUS reserved_list
	{
		::wsprintf($$.sStrOrg, _T("%s + %s"), $1.sStrOrg, $3.sStrOrg);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 1);
		if(nError)
			return nError;
	}
|	reserved_list MINUS reserved_list
	{
		::wsprintf($$.sStrOrg, _T("%s - %s"), $1.sStrOrg, $3.sStrOrg);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 2);
		if(nError)
			return nError;
	}
|	reserved_list MULT reserved_list
	{
		::wsprintf($$.sStrOrg, _T("%s * %s"), $1.sStrOrg, $3.sStrOrg);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 4);
		if(nError)
			return nError;
	}
|	reserved_list DIVIDE reserved_list
	{
		::wsprintf($$.sStrOrg, _T("%s / %s"), $1.sStrOrg, $3.sStrOrg);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 5);
		if(nError)
			return nError;
	}
|	reserved_list MOD reserved_list 
	{ 
		::wsprintf($$.sStrOrg, _T("%s %% %s"), $1.sStrOrg, $3.sStrOrg);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 3);
		if(nError)
			return nError;
	}
|	reserved_list PLUS data_list
	{
		if(::lstrlen($3.sStrOrg))
			::wsprintf($$.sStrOrg, _T("%s + %s"), $1.sStrOrg, $3.sStrOrg);
		else
			::wsprintf($$.sStrOrg, _T("%s + %s"), $1.sStrOrg, $3.sStr);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 1);
		if(nError)
			return nError;
	}
|	reserved_list MINUS data_list
	{
		if(::lstrlen($3.sStrOrg))
			::wsprintf($$.sStrOrg, _T("%s - %s"), $1.sStrOrg, $3.sStrOrg);
		else
			::wsprintf($$.sStrOrg, _T("%s - %s"), $1.sStrOrg, $3.sStr);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 2);
		if(nError)
			return nError;
	}
|	reserved_list MULT data_list
	{
		if(::lstrlen($3.sStrOrg))
			::wsprintf($$.sStrOrg, _T("%s * %s"), $1.sStrOrg, $3.sStrOrg);
		else
			::wsprintf($$.sStrOrg, _T("%s * %s"), $1.sStrOrg, $3.sStr);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 4);
		if(nError)
			return nError;
	}
|	reserved_list DIVIDE data_list
	{
		if(::lstrlen($3.sStrOrg))
			::wsprintf($$.sStrOrg, _T("%s / %s"), $1.sStrOrg, $3.sStrOrg);
		else
			::wsprintf($$.sStrOrg, _T("%s / %s"), $1.sStrOrg, $3.sStr);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 5);
		if(nError)
			return nError;
	}
|	reserved_list MOD data_list 
	{ 
		if(::lstrlen($3.sStrOrg))
			::wsprintf($$.sStrOrg, _T("%s % %s"), $1.sStrOrg, $3.sStrOrg);
		else
			::wsprintf($$.sStrOrg, _T("%s % %s"), $1.sStrOrg, $3.sStr);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 3);
		if(nError)
			return nError;
	}
|	data_list PLUS reserved_list
	{
		::wsprintf($$.sStrOrg, _T("%s + %s"), $1.sStr, $3.sStrOrg);

		// Parse the operator sentence (Plus)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 1);
		if(nError)
			return nError;
	}
|	data_list MINUS reserved_list
	{
		::wsprintf($$.sStrOrg, _T("%s - %s"), $1.sStr, $3.sStrOrg);

		// Parse the operator sentence (Minus)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 2);
		if(nError)
			return nError;
	}
|	data_list MULT reserved_list
	{
		::wsprintf($$.sStrOrg, _T("%s * %s"), $1.sStr, $3.sStrOrg);

		// Parse the operator sentence (Multiple)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 4);
		if(nError)
			return nError;
	}
|	data_list DIVIDE reserved_list
	{
		::wsprintf($$.sStrOrg, _T("%s / %s"), $1.sStr, $3.sStrOrg);

		// Parse the operator sentence (Divide)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 5);
		if(nError)
			return nError;
	}
|	data_list MOD reserved_list 
	{ 
		::wsprintf($$.sStrOrg, _T("%s %% %s"), $1.sStr, $3.sStrOrg);

		// Parse the operator sentence (Mod)
		int nError = g_pSysTradingParser->ParseOperatorSentence($1.sStr, $3.sStr, $$.sStr, 3);
		if(nError)
			return nError;
	}

array_stmt:
	ARRAY COLON array_list SEMICOLON {}

array_list:
	array_list2 {}	
|	array_list2 COMMA array_list2 {}
|	Array_Error {}
;

array_list2:
	NAME LBRAC array_list3 RBRAC LPAREN data_list RPAREN 
	{
		// Write the variable sentence.
		int nError = g_pSysTradingParser->WriteArraySentence($6.cExprKind, $1, $3.sStr, &$6, $$);
		if(nError)
			return nError;
//		g_pSysTradingParser->TempFunc();
	}

array_list3:
	array_list4 
	{
		strcpy($$.sStr, $1);
	}
|	array_list3 COMMA array_list4 
	{
		sprintf($$.sStr, "%s,%s", $1.sStr, $3);
	}

array_list4:
	INT_NUMBER	
	{ 
		strcpy($$, $1); 
	}

Array_Error:
	Declare_Error {}
|	NAME LBRAC
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, $1);
		if(nError)
			return nError;
	}
|	NAME LBRAC array_list3
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, $1);
		if(nError)
			return nError;
	}
|	NAME RBRAC
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, $1);
		if(nError)
			return nError;
	}
|	NAME array_list3 RBRAC 
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, $1);
		if(nError)
			return nError;
	}
|	NAME RBRAC RBRAC
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, $1);
		if(nError)
			return nError;
	}
|	NAME LBRAC array_list3 RBRAC
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, $1);
		if(nError)
			return nError;
	}
|	NAME LBRAC array_list3 RBRAC LPAREN
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, $1);
		if(nError)
			return nError;
	}
|	NAME LBRAC array_list3 RBRAC data_list
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, $1);
		if(nError)
			return nError;
	}
|	NAME LBRAC array_list3 RBRAC data_list RPAREN
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, $1);
		if(nError)
			return nError;
	}
|	NAME LBRAC array_list3 RBRAC RPAREN
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, $1);
		if(nError)
			return nError;
	}
|	NAME LBRAC array_list3 RBRAC LPAREN RPAREN
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(5, $1);
		if(nError)
			return nError;
	}

num_list:
	INT_NUMBER	
	{ 
		strcpy($$, $1); 
	}
|	DBL_NUMBER	
	{ 
		strcpy($$, $1); 
	}

num_expr:
	num_list 
	{ 
//		if (num(&$$, $1) < 0) return 0; 
	}

assign_expr:
	name_expr EQ expr						
	{
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&$1, $$.sStr);
		if(nError)
			return nError;

		// Parse the assign sentence.
		nError = g_pSysTradingParser->ParseAssignSentence(1, &$1, $3.sStr, $$.sStr);
		if(nError)
			return nError;
	}
|	name_expr EQ cond_expr					
	{ 
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&$1, $$.sStr);
		if(nError)
			return nError;

		// Parse the assign sentence.
		nError = g_pSysTradingParser->ParseAssignSentence(1, &$1, $3.sStr, $$.sStr);
		if(nError)
			return nError;
//		assign(&$$, &$1, &$3); 
	}
|	array_expr EQ expr
	{ 
		// Parse the assign sentence.
		int nError = g_pSysTradingParser->ParseAssignSentence(1, &$1, $3.sStr, $$.sStr);
		if(nError)
			return nError;
	}
|	array_expr EQ cond_expr					
	{ 
		// Parse the assign sentence.
		int nError = g_pSysTradingParser->ParseAssignSentence(1, &$1, $3.sStr, $$.sStr);
		if(nError)
			return nError;
	}
|	reserved_arr EQ expr					
	{ 
//		assign(&$$, &$1, &$3); 
	}
|	reserved_cond EQ cond_expr				
	{ 
//		assign(&$$, &$1, &$3); 
	}
|	Assign_Error {}

Assign_Error:
	name_expr EQ
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}
|	array_expr EQ
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}
|	EQ expr
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}
|	EQ cond_expr
	{
		int nError = g_pSysTradingParser->GetParseErrorIndex(6, yyval.expr.sStr);
		if(nError)
			return nError;
	}


array_idx_list:
	array_idx_list2 
	{ 
		$$ = $1; 
//		if (ArrayDimNew($$.sStr) < 0) return 0;
	}
|	array_idx_list COMMA array_idx_list2 
	{
		sprintf($$.sStr, "%s][%s", $1.sStr, $3.sStr);
//		if (ArrayDimInsert($3.sStr) < 0) return 0;
	}

array_idx_list2:
	expr 
	{ 
		$$ = $1; 
	}

function:
	function_name function RPAREN	
	{
		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&$1, $2.sStr, $$.sStr);
		if(nError)
			return nError;
	}
|	function_name func_arg_list RPAREN LBRAC data_list RBRAC
	{
		::lstrcpy($1.sArg, $5.sStr);

		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&$1, $2.sStr, $$.sStr);
		if(nError)
			return nError;
	}
|	function_name func_arg_list RPAREN LBRAC array_idx_list RBRAC
	{
		::lstrcpy($1.sArg, $5.sStr);

		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&$1, $2.sStr, $$.sStr);
		if(nError)
			return nError;
	}
|	function_name func_arg_list RPAREN	
	{
		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&$1, $2.sStr, $$.sStr);
		if(nError)
			return nError;
	}
|	function_name2 func_arg_list RPAREN	
	{
		// Parse the function sentence.
		int nError = g_pSysTradingParser->ParseFunctionSentence(&$1, $2.sStr, $$.sStr);
		if(nError)
			return nError;
	}

function_name:
	NAME LPAREN
	{
		// Parse the function name sentence.
		int nError = g_pSysTradingParser->ParseFunctionNameSentence($1, $$.sStr);
		if(nError)
			return nError;
		$$.cExprKind = 0;
		::lstrcpy($$.sArg, _T("0"));
	}

function_name2:
	NAME LBRAC data_list RBRAC LPAREN
	{
		int nError = g_pSysTradingParser->ParseFunctionNameSentence($1, $<expr.sStr>$);
		if(nError)
			return nError;		
		$<expr.cExprKind>$  = $<expr.cExprKind>3;
		::lstrcpy($<expr.sArg>$ , $<expr.sStr>3);
	}
	NAME LBRAC array_idx_list RBRAC LPAREN
	{
		int nError = g_pSysTradingParser->ParseFunctionNameSentence($1, $$.sStr);
		if(nError)
			return nError;
		$$.cExprKind = $3.cExprKind;
		::lstrcpy($$.sArg, $3.sStr);
	}

func_arg_list:
	{ 
		$$.sStr[0] = '\0'; $$.cExprKind = EXP_CONSTANT; 
//		::fwrite(")\n", 1, 2, fpOutput);
//		if (FuncArgsNew($$.sStr) < 0) return 0;
	}
|	cond_expr 
	{ 
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(1, NULL, &$1, $$.sStr);
		if(nError)
			return nError;
		$$.cExprKind = EXP_CONSTANT; 
	}
|	func_arg_list2 COMMA cond_expr 
	{
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(2, $1.sStr, &$3, $$.sStr);
		if(nError)
			return nError;
		$$.cExprKind = EXP_CONSTANT;
	}

func_arg_list2:
	{ 
		$$.sStr[0] = '\0'; $$.cExprKind = EXP_CONSTANT; 
//		::fwrite(")\n", 1, 2, fpOutput);
//		if (FuncArgsNew($$.sStr) < 0) return 0;
	}
|	cond_expr
	{ 
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(3, NULL, &$1, $$.sStr);
		if(nError)
			return nError;
		$$.cExprKind = EXP_CONSTANT; 
	}
|	func_arg_list2 COMMA cond_expr 
	{
		// Parse the function argument sentence.
		int nError = g_pSysTradingParser->ParseFunctionArgumentSentence(4, $1.sStr, &$3, $$.sStr);
		if(nError)
			return nError;
		$$.cExprKind = EXP_CONSTANT;
	}
;

def_arg_list:
	{
		$$.sStr[0] = '\0';
	}
|	def_arg_list2 
	{
		$$ = $1;
	}
|	def_arg_list COMMA def_arg_list2 
	{
		// Parse the default argument list sentence.
		int nError = g_pSysTradingParser->ParseDefaultArgumentListSentence($1.sStr, $3.sStr, $$.sStr);
		if(nError)
			return nError;
	}
;

def_arg_list2:
	NAME LPAREN data_type_list RPAREN 
	{
		// Write the default argument list sentence.
		int nError = g_pSysTradingParser->WriteDefaultArgumentListSentence($3.cExprKind, $1, $$.sStr);
		if(nError)
			return nError;
	}
;

data_type_list:
	DEF_STRING 
	{
		$$.cExprKind = 0;
		SET_STRING($$.cExprKind);
	}	
|	DEF_STRING_SIMPLE 
	{
		$$.cExprKind = 0;
		SET_STRING($$.cExprKind);
		SET_SIMPLE($$.cExprKind);
	}	
|	DEF_STRING_ARRAY 
	{
		$$.cExprKind = 0;
		SET_STRING($$.cExprKind);
		SET_ARRAY($$.cExprKind);
	}	
|	DEF_NUMERIC 
	{
		$$.cExprKind = 0;
		SET_DOUBLE($$.cExprKind);
	}
|	DEF_NUMERIC_SIMPLE 
	{
		$$.cExprKind = 0;
		SET_DOUBLE($$.cExprKind);
		SET_SIMPLE($$.cExprKind);
	}
|	DEF_NUMERIC_ARRAY 
	{
		$$.cExprKind = 0;
		SET_DOUBLE($$.cExprKind);
		SET_ARRAY($$.cExprKind);
	}
|	DEF_BOOL 
	{
		$$.cExprKind = 0;
		SET_BOOL($$.cExprKind);
	}
|	DEF_BOOL_SIMPLE 
	{
		$$.cExprKind = 0;
		SET_BOOL($$.cExprKind);
		SET_SIMPLE($$.cExprKind);
	}
|	DEF_BOOL_ARRAY 
	{
		$$.cExprKind = 0;
		SET_BOOL($$.cExprKind);
		SET_ARRAY($$.cExprKind);
	}
;

for_start:
	FOR name_expr EQ expr DOWNTO expr {}
|	FOR name_expr EQ expr DOWN TO expr {}
|	FOR name_expr EQ expr TO expr {}
	{
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&$2, $$.sStr);
		if(nError)
			return nError;

		g_pSysTradingParser->IsEndBlock(TRUE);
		// Write the for_start sentence.
		nError = g_pSysTradingParser->WriteForStartSentence(&$2, $4.sStr, $6.sStr, $$.sStr);
		if(nError)
			return nError;
	}
|	FOR array_expr EQ expr DOWNTO expr {}
|	FOR array_expr EQ expr DOWN TO expr {}
|	FOR array_expr EQ expr TO expr {}
	{
		g_pSysTradingParser->IsEndBlock(TRUE);
		// Write the for_start sentence.
		int nError = g_pSysTradingParser->WriteForStartSentence(&$2, $4.sStr, $6.sStr, $$.sStr);
		if(nError)
			return nError;
	}
;

loop_break:
	BREAK SEMICOLON	{}
;

block_begin:
	BBEGIN
	{ 
//		strcpy($$.sStr, "{\npSTActor->m_STDataManager.BracketIn();\n");
//		::fwrite($$.sStr, 1, ::strlen($$.sStr), fpOutput);
	}
;

block_end:
	BEND_SEMICOLON
	{ 
		g_pSysTradingParser->IsEndBlock(TRUE);
		int nError = g_pSysTradingParser->EndBlock(NULL);
		if(nError)
			return nError;
	}
|	BEND SEMICOLON
	{ 
		g_pSysTradingParser->IsEndBlock(TRUE);
		int nError = g_pSysTradingParser->EndBlock(NULL);
		if(nError)
			return nError;
	}
|	BEND
	{ 
		g_pSysTradingParser->IsEndBlock(FALSE);
		int nError = g_pSysTradingParser->ProcessEndStatment();
		if(nError)
			return nError;
	}
;

if_start:
	IF cond_expr THEN 
	{
		g_pSysTradingParser->IsEndBlock(TRUE);
		g_pSysTradingParser->IsEndOneBlock();
		// Write the if_start sentence.
		int nError = g_pSysTradingParser->WriteIfStartSentence(1, $2.sStr, $$.sStr);
		if(nError)
			return nError;
	}
;

if_else:
	ELSE
	{
		g_pSysTradingParser->DelEndBlock();
		// Write the else sentence.
		int nError = g_pSysTradingParser->WriteIfStartSentence(2, NULL, $$.sStr);
		if(nError)
			return nError;
	}
;

if_else_if:
	ELSEIF cond_expr THEN 
	{
		g_pSysTradingParser->DelEndBlock();
	}
;

cross_above:
	name_expr CROSS ABOVE expr
	{
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&$1, $$);
		if(nError)
			return nError;

		// Parse the member function sentence.	(CROSS ABOVE)
		// g_pSysTradingParser->ParseMemberFunctionSentence(1, &$1, $4.sStr, $$);
		nError = g_pSysTradingParser->ParseInnerFunctionSentence(1, &$1, $4.sStr, $$);
		if(nError)
			return nError;
	}
|	array_expr CROSS ABOVE expr
	{
		// Parse the member function sentence.	(CROSS ABOVE)
		int nError = g_pSysTradingParser->ParseInnerFunctionSentence(1, &$1, $4.sStr, $$);
		if(nError)
			return nError;
	}

;
cross_below:
	name_expr CROSS BELOW expr
	{
		// Parse the naming sentence.
		int nError = g_pSysTradingParser->ParseNameSentence(&$1, $$);
		if(nError)
			return nError;

		// Parse the member function sentence. (CROSS BELOW)
		// g_pSysTradingParser->ParseMemberFunctionSentence(2, &$1, $4.sStr, $$);
		nError = g_pSysTradingParser->ParseInnerFunctionSentence(2, &$1, $4.sStr, $$);
		if(nError)
			return nError;
	}
|	array_expr CROSS BELOW expr
	{
		// Parse the member function sentence. (CROSS BELOW)
		int nError = g_pSysTradingParser->ParseInnerFunctionSentence(2, &$1, $4.sStr, $$);
		if(nError)
			return nError;
	}
;

	
%%

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