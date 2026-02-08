%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <string.h>
#include "DeclareDefine.h"

FILE* fpOutput;

extern int SysTradingerror(const char* s);
extern int SysTradinglex();

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

%type		<name>	inputname
%type		<name>	num_list
%type		<name>	array_list4

%type 		<expr>	num_expr name_expr expr expr2 expr3 expr4 assign_expr
%type 		<expr>	reserved_arr reserved_sise reserved_cond reserved_text
%type 		<expr>	for_start loop_break
%type 		<expr>	while_start
%type 		<expr>	if_start if_else if_else_if
%type 		<expr>	block_begin block_end
%type 		<expr>	cond_expr cond_expr2 cond_expr3
%type 		<expr>	def_arg_list def_arg_list2 data_type_list
%type 		<expr>	func_arg_list func_arg_list2
%type 		<expr>	user_func_def
%type 		<expr>	function
%type 		<expr>	data_list
%type		<expr>	array_list3
%type		<expr>	array_expr array_idx_list array_idx_list2

%type		<ival>	user_func_ret 

%%

inputname:
	INPUT COLON input_list SEMICOLON { strcpy($$, $1); }
;

stmt_list:
	stmt
|	stmt_list stmt

stmt:
	stmt_suffix
|   array_stmt stmt_suffix
|   input_stmt stmt_suffix
|   variable_stmt stmt_suffix
|	expr stmt_suffix			{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
|	assign_expr stmt_suffix		{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
|	cond_expr stmt_suffix		{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
|	loop_break stmt_suffix		{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
|   for_start stmt_suffix		{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
|   while_start stmt_suffix		{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
|   if_start stmt_suffix		{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
|   if_else stmt_suffix			{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
|   if_else_if stmt_suffix		{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
|   block_begin stmt_suffix		{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
|   block_end stmt_suffix		{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
|	user_func_proto stmt_suffix { ; }
|   user_func_def stmt_suffix	{::fwrite($1.sStr, 1, ::strlen($1.sStr), fpOutput);}
;

stmt_suffix:

| STMT_SUFFIX
;

array_stmt:
	ARRAY COLON array_list SEMICOLON {
	{ /* 선언부분과 다음 소스를 구분하기위해서 라인추가 */
		::fwrite(_T("\n\n"), 1, 2, fpOutput);
	}
}
;

array_list:
	array_list2 {
}	
| array_list2 COMMA array_list2 {
}
;

array_list2:
	NAME LBRAC array_list3 RBRAC LPAREN data_list RPAREN {;}
;

array_list3:
	array_list4 {
		strcpy($$.sStr, $1);
}
|	array_list3 COMMA array_list4 {
		sprintf($$.sStr, "%s,%s", $1.sStr, $3);
}
;

array_list4:
	INT_NUMBER	{ strcpy($$, $1); }
;

num_list:
	INT_NUMBER	{ strcpy($$, $1); }
|	DBL_NUMBER	{ strcpy($$, $1); }
;

input_stmt:
	INPUT COLON input_list SEMICOLON {}
;

input_list:
	input_list2 {
}
| input_list COMMA input_list2 {

}

input_list2:
	NAME LPAREN cond_expr RPAREN {;}

variable_stmt:
	VARIABLE COLON variable_list SEMICOLON {
	{ /* 선언부분과 다음 소스를 구분하기위해서 라인추가 */
		::fwrite(_T("\n\n"), 1, 2, fpOutput);
    }
}
;

variable_list:
	variable_list2 {
}
| variable_list COMMA variable_list2 {
}
;

variable_list2:
	NAME LPAREN data_list RPAREN {;}
;

data_list:
	INT_NUMBER {
		$$.cExprKind = DTTYPE_INT;
		strcpy($$.sStr, $1);
}
|	DBL_NUMBER {
		$$.cExprKind = DTTYPE_DOUBLE;
		strcpy($$.sStr, $1);
}
|	STRING {
		$$.cExprKind = DTTYPE_STRING;
		strcpy($$.sStr, $1.sStr);
}
|	BOOL_TYPE {
		$$.cExprKind = DTTYPE_BOOL;
		strcpy($$.sStr, $1);
}
;

for_start:
    FOR name_expr EQ expr DOWNTO expr
    {; }
|    FOR name_expr EQ expr DOWN TO expr
    {; }
|	FOR name_expr EQ expr TO expr
    {; }
;

while_start:
	WHILE cond_expr {; }
;

block_begin:
	BBEGIN	{; }
;

block_end:
	BEND SEMICOLON	{; }
|	BEND 			{; }
;

loop_break:
	BREAK SEMICOLON	{; }
;

if_start:
	IF cond_expr THEN {; }
;

if_else:
	ELSE	{; }
;

if_else_if:
	ELSEIF cond_expr THEN {; }
|	ELSE IF cond_expr THEN {; }
;

assign_expr:
	name_expr EQ expr						{; }
|	name_expr EQ cond_expr					{; }
|	array_expr EQ expr						{; }
|	array_expr EQ cond_expr					{; }
|	reserved_arr EQ expr					{; }
|	reserved_cond EQ cond_expr				{; }
;

expr:
  expr2     { $$ = $1; }
| expr2 SEMICOLON { sprintf($$.sStr, "%s;", $1.sStr); }
;

cond_expr:
  cond_expr2				{ $$ = $1; }
| cond_expr2 SEMICOLON { sprintf($$.sStr, "%s;", $1.sStr); }
| cond_expr OR cond_expr2 {; }
| cond_expr OR cond_expr2 SEMICOLON {strcat($$.sStr, ";");}
;

cond_expr2:
  cond_expr3				{ $$ = $1; }
| cond_expr2 AND cond_expr3 {; }
;

cond_expr3:
LPAREN cond_expr RPAREN  { 
					sprintf($$.sStr, "(%s)", $2.sStr);
					$$.cExprKind = EXP_COMPLEX;
				}
| expr2          { $$ = $1; }
| NOT expr2 {; }
| expr2 EQ expr2 {; }
| expr2 NE expr2 {; }
| expr2 LT expr2 {; }
| expr2 LE expr2 {; }
| expr2 GT expr2 {; }
| expr2 GE expr2 {; }
;

expr2:
  expr3				{ $$ = $1; }
| expr2 PLUS expr3 {; }
| expr2 MINUS expr3 {; }
| expr2 MOD expr3 {; }
;

expr3:
  expr4				{ $$ = $1; }
| expr3 MULT expr4 {; }
| expr3 DIVIDE expr4 {; }
;

expr4:
 num_expr			 { $$ = $1; }
| STRING			 { sprintf($$.sStr, "%s", $1.sStr); }
| DATACNT			 { sprintf($$.sStr, "giDataCnt"); } 
| BOOL_TYPE			 { sprintf($$.sStr, "%s", $1); }
| name_expr			 { $$ = $1; }
| array_expr		 { $$ = $1; }
| reserved_sise		 { $$ = $1; }
| reserved_arr		 { $$ = $1; }
| reserved_cond		 { $$ = $1; }
| function			 { $$ = $1; }
| PLUS expr	         { $$ = $2; }
| MINUS expr   { 
					sprintf($$.sStr, "-%s", $2.sStr);
					$$.cExprKind = EXP_COMPLEX;
				}
| LPAREN expr RPAREN  { 
					sprintf($$.sStr, "(%s)", $2.sStr);
					$$.cExprKind = EXP_COMPLEX;
				}
;

name_expr:
	NAME {; }
;

array_expr:
	NAME LBRAC array_idx_list RBRAC {;}
;

array_idx_list:
	array_idx_list2 {;}
|	array_idx_list COMMA array_idx_list2 {;}
;

array_idx_list2:
	expr { $$ = $1; }
;

num_expr:
	num_list {; }
;

reserved_arr:
  RESERVED_VAR {; }
| RESERVED_VAR LBRAC expr RBRAC {; }
;

reserved_sise:
  OPEN {; }
| OPEN LBRAC expr RBRAC {; }
| HIGH {; }
| HIGH LBRAC expr RBRAC {; }
| LOW {; }
| LOW LBRAC expr RBRAC {; }
| CLOSE {; }
| CLOSE LBRAC expr RBRAC {; }
| VOLUME {; }
| VOLUME LBRAC expr RBRAC {; }
| AMOUNT {; }
| AMOUNT LBRAC expr RBRAC {; }
| DATE {; }
| DATE LBRAC expr RBRAC {; }
| TIME {; }
| TIME LBRAC expr RBRAC {; }
| JMCODE { strcpy($$.sStr, "JMCODE"); }
| CURRENTDATE { strcpy($$.sStr, "giCurrentDate"); }
| CURRENTTIME { strcpy($$.sStr, "giCurrentTime"); }
;

reserved_cond:
  RESERVED_COND {; }
| RESERVED_COND LBRAC expr RBRAC {; }
;
reserved_text:
  RESERVED_TEXT {; }
| RESERVED_TEXT LBRAC expr RBRAC {; }
;

func_arg_list:
	{ 
		$$.sStr[0] = '\0'; $$.cExprKind = EXP_CONSTANT; 
}
| cond_expr { 
		sprintf($$.sStr, "%s", $1.sStr); 
		$$.cExprKind = EXP_CONSTANT; 
}
| func_arg_list2 COMMA cond_expr {
		sprintf($$.sStr, "%s, %s", $1.sStr, $3.sStr); 
		$$.cExprKind = EXP_CONSTANT;
}

func_arg_list2:
  cond_expr { 
		sprintf($$.sStr, "%s", $1.sStr); $$.cExprKind = EXP_CONSTANT; 
}
| func_arg_list2 COMMA cond_expr {
		sprintf($$.sStr, "%s, %s", $1.sStr, $3.sStr); 
		$$.cExprKind = EXP_CONSTANT;
}
;

def_arg_list:
	{
	$$.sStr[0] = '\0';
}
|	def_arg_list2 {
	$$ = $1;
}
| def_arg_list COMMA def_arg_list2 {
	sprintf($$.sStr, "%s, %s", $1.sStr, $3.sStr);	
}
;

def_arg_list2:
	NAME LPAREN data_type_list RPAREN {;}
;

data_type_list:
	DEF_STRING {
	$$.cExprKind = 0;
}	
|	DEF_STRING_SIMPLE {
	$$.cExprKind = 0;
}	
|	DEF_STRING_ARRAY {
	$$.cExprKind = 0;
}	
|	DEF_NUMERIC {
	$$.cExprKind = 0;
}
|	DEF_NUMERIC_SIMPLE {
	$$.cExprKind = 0;
}
|	DEF_NUMERIC_ARRAY {
	$$.cExprKind = 0;
}
|	DEF_BOOL {
	$$.cExprKind = 0;
}
|	DEF_BOOL_SIMPLE {
	$$.cExprKind = 0;
}
|	DEF_BOOL_ARRAY {
	$$.cExprKind = 0;
}
;

user_func_ret:
	DEF_NUMERIC	{ $$ = DTTYPE_DOUBLE; }
|	DEF_BOOL	{ $$ = DTTYPE_BOOL; }
|	DEF_STRING	{ $$ = DTTYPE_STRING; }
;

user_func_proto:
	FUNCTION user_func_ret NAME LPAREN def_arg_list RPAREN SEMICOLON {}
;

user_func_def:
	FUNCTION user_func_ret NAME LPAREN def_arg_list RPAREN 
	BBEGIN	{;}
;

function:
	NAME LPAREN func_arg_list RPAREN	{;}
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
    cerr << "Syntax error: " << s << endl;
	return 0;
}

void SetOutputFile(FILE* fpFile)
{
	fpOutput = fpFile;
}
