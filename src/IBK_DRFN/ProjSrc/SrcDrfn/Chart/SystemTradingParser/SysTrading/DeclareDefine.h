#ifndef __DECLAREDEFINE_PARSER__
#define __DECLAREDEFINE_PARSER__

#define NAME_SZ 64
#define PATH_SZ 256
#define EXPR_SZ 1024

#define NULL_TOKEN		0
#define ETC_MODE		0
#define COMPILE_MODE	1
#define STRIP_MODE		2
#define BINARY_MODE		3

#define MAX_VAR 10
#define MAX_RESERVE 51

#define MAX_ARR_DIM		10		/* 최대 배열차원 */
#define MAX_ARR_SIZE	5000	/* 최대 배열데이터수 */

#define MAX_CNVT_BUF	20

#define MAX_STACK_SIZE	20

#define DEFAULT_VAR_MAX 20
#define DEFAULT_VAR_INC 10

#define MAX_FUNC_CNT		500	
#define DEFAULT_FUNC_MAX	100
#define DEFAULT_FUNC_INC	5
#define DEFAULT_UFUNC_MAX	10
#define DEFAULT_UFUNC_INC	5


#define USER_VAR_NAME   "UserVar"
#define USER_FUNC_NAME	"UFunc"
#define	UFUNC_RET_NAME	"Ret"

#define	F_UPTO		0
#define	F_DOWNTO	1

#define MAX_BLOCK   100
#define MAX_ARG_CNT	30

#define DTTYPE_VOID		0x0001
#define DTTYPE_CHAR		0x0002
#define DTTYPE_STRING	0x0004
#define DTTYPE_INT		0x0008
#define DTTYPE_DOUBLE	0x0010
#define DTTYPE_BOOL		0x0020
#define DTTYPE_ARRAY	0x0040
#define DTTYPE_SIMPLE	0x0080

#define IS_VOID(a)		((a) & DTTYPE_VOID)
#define IS_CHAR(a)		((a) & DTTYPE_CHAR)
#define IS_STRING(a)	((a) & DTTYPE_STRING)
#define IS_INT(a)		((a) & DTTYPE_INT)
#define IS_DOUBLE(a)	((a) & DTTYPE_DOUBLE)
#define IS_BOOL(a)		((a) & DTTYPE_BOOL)
#define IS_ARRAY(a)		((a) & DTTYPE_ARRAY)
#define IS_SIMPLE(a)	((a) & DTTYPE_SIMPLE)

#define SET_VOID(a)		((a) |= DTTYPE_VOID)
#define SET_CHAR(a)		((a) |= DTTYPE_CHAR)
#define SET_STRING(a)	((a) |= DTTYPE_STRING)
#define SET_INT(a)		((a) |= DTTYPE_INT)
#define SET_DOUBLE(a)	((a) |= DTTYPE_DOUBLE)
#define SET_BOOL(a)		((a) |= DTTYPE_BOOL)
#define SET_ARRAY(a)	((a) |= DTTYPE_ARRAY)
#define SET_SIMPLE(a)	((a) |= DTTYPE_SIMPLE)

#define CLR_VOID(a)		((a) &= ~DTTYPE_VOID)
#define CLR_CHAR(a)		((a) &= ~DTTYPE_CHAR)
#define CLR_STRING(a)	((a) &= ~DTTYPE_STRING)
#define CLR_INT(a)		((a) &= ~DTTYPE_INT)
#define CLR_DOUBLE(a)	((a) &= ~DTTYPE_DOUBLE)
#define CLR_BOOL(a)		((a) &= ~DTTYPE_BOOL)
#define CLR_ARRAY(a)	((a) &= ~DTTYPE_ARRAY)
#define CLR_SIMPLE(a)	((a) &= ~DTTYPE_SIMPLE)

#define MAIN_FUNC_TYPE		1
#define INTER_FUNC_TYPE		2
#define USER_FUNC_TYPE		3

#define NORMAL_DEF			1
#define	FUNC_ARG_DEF		2
#define	INPUT_DEF			3

#define B_MAIN		0
#define B_FOR		1
#define B_IF		2
#define B_ELSE		3
#define B_WHILE		4
#define B_FUNC		5
#define B_BEGIN		6


typedef struct _CVNT_INFO
{
	char	sIDX		[NAME_SZ];
	char	sUserInfo	[NAME_SZ];
	char	sHome		[PATH_SZ];
	char	sStripPath	[PATH_SZ];
	char	sCCPath		[PATH_SZ];
	char	sCFlags		[NAME_SZ];
	char	sSrcPath	[PATH_SZ];
	char	sBinPath	[PATH_SZ];
	char	sIncPath	[PATH_SZ];
	char	sLibPath	[PATH_SZ];
	char	sLibs		[NAME_SZ];
	char	sFileName	[NAME_SZ];
	char	sTemplate	[PATH_SZ];

	int		iUsrProcTimeOut;

	char	sSrcBFileName[PATH_SZ];
	char	sSrcCFileName[PATH_SZ];
	char	sSrcHFileName[PATH_SZ];
	char	sSrcFFileName[PATH_SZ];

	int		iFileDelMethod;

	char	sLogF[PATH_SZ];
	
} CNVT_INFO;

typedef struct _VAR_INFO
{
	char	sOrgName[NAME_SZ];
	char	sCnvtName[NAME_SZ];
	int		iDefFlag;
	int		iDefUsed;
	int		iType;
	int		iArrDim;
	int		ipArrSize[MAX_ARR_DIM];
	int		iTotArrSize;
	char	sInitValue[128];
} VAR_INFO;	

typedef struct _VAR_TABLE
{
	int		iCnt;
	int		iMax;
	int		iInc;
	VAR_INFO *tpItem;
} VAR_TABLE;

typedef struct _ARRAY_DIM
{
	int		iCnt;
	char	*spDim[MAX_ARR_DIM];	
} ARRAY_DIM;

typedef struct _FUNC_INFO
{
	char	sName[NAME_SZ];
	char	sCnvtName[NAME_SZ];
	int		iIdx;
	int		iType;
	int		iArgCnt;
	int		iArgFixed;
	int		ipArgType[MAX_ARG_CNT];
	int		iRetType;
	char	sProtoType[256];
} FUNC_INFO;

typedef struct _FUNC_TABLE
{
	int		iCnt;
	int		iMax;
	int		iInc;
	FUNC_INFO *tpItem;
} FUNC_TABLE;

typedef struct _FUNC_ARGS
{
	int		iArgc;
	char	*spArgs[MAX_ARG_CNT];	
} FUNC_ARGS;

typedef struct _STACK_TABLE
{
	int		iPos;
	void	*vpData[MAX_STACK_SIZE];
} STACK_TABLE;

#define EXP_CONSTANT	((char)1)
#define EXP_VARIABLE	((char)2)
#define EXP_TARRAY		((char)3)
#define EXP_NARRAY		((char)4)
#define EXP_COMPLEX		((char)5)
#define EXP_COND		((char)6)
#define EXP_CONTROL		((char)7)
#define EXP_FUNCION		((char)8)

#define IS_TARRAY(a) (a == EXP_TARRAY)

typedef struct _EXPR
{
	char sStr[EXPR_SZ];
	char sStrOrg[EXPR_SZ];
	unsigned char cExprKind;
	char sArg[EXPR_SZ];
} EXPR;

extern int giBlockDepth;
extern char gsIdentSpace[];
extern int giBlockInfo;
extern int giUFuncArgCnt;
extern int gipUFuncArgType[];

char *StrCat(int piCnt, ...);

#define STRCPY(o_buf, o_len, i_buf, i_len) \
{ int __l = (i_len > o_len) ? o_len: i_len; memcpy(o_buf, i_buf, __l); o_buf[__l] = 0; }


#define		FUNCTION_DEFAULT	0x00000001
#define		FUNCTION_USER		0x00000002
#define		FUNCTION_MEMBER		0x00000004

#define SYSVECTOR	vector<LPSTR>
#define	STACKVECTOR	vector<UINT>

#define		STACK_FOR			1
#define		STACK_FOR_BLOCK		2
#define		STACK_IF			3
#define		STACK_IF_BLOCK		4
#define		STACK_ELSEIF		5
#define		STACK_ELSEIF_BLOCK	6
#define		STACK_ELSE			7
#define		STACK_ELSE_BLOCK	8
#define		STACK_WHILE			9
#define		STACK_WHILE_BLOCK	10
#define		STACK_FOR_BLOCKEND	200
#define		STACK_IF_END		300
#define		STACK_IF_BLOCKEND	400
#define		STACK_ELSEIF_BLOCKEND	600
#define		STACK_ELSE_BLOCKEND	800
#define		STACK_WHILE_BLOCKEND	1000
#define		STACK_STATEMENT		9000

#define		PARSE_CPLUSPLUS		1
#define		PARSE_VISUALBASIC	2

#define		NumericSeries	0
#define		NumericSimple	1
#define		TRUEFALSE		2

#define		MEMVAR_OPEN		0x0001
#define		MEMVAR_HIGH		0x0002
#define		MEMVAR_LOW		0x0004
#define		MEMVAR_CLOSE	0x0008
#define		MEMVAR_VOLUME	0x0010
#define		MEMVAR_AMOUNT	0x0020

#endif
