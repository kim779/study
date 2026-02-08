#pragma once

#define COLOR_LINE		93
#define COLOR_TABLE		75
#define COLOR_BK		64
#define COLOR_GRIDHEAD		74
#define COLOR_GUIDEBACK		213
#define COLOR_PANELBACK		66
#define COLOR_BODYBACK		181
#define COLOR_GRIDHEADTXT	76
#define COLOR_DATA		99
#define COLOR_TEXT		69
#define COLOR_TABLEHEAD		96
#define	COLOR_MDB		171		// 매도 back
#define	COLOR_MSB		170		// 매수 back
#define COLOR_PLUSTXT		94
#define COLOR_MINUSTXT		95
#define COLOR_FOCUS		78

// for speed parsing
class StringProxy : public pair<LPCSTR,LPCSTR>
{
public:
	StringProxy() : pair<LPCSTR,LPCSTR>() {}
	StringProxy(LPCSTR st, LPCSTR ed) : pair<LPCSTR, LPCSTR>(st, ed) {}

	string ToStr()		{ return string(first, second); }
	CString ToCStr()	{ return CString(first, second-first); }
};

class Jango
{
public:
	Jango() { ZeroMemory(this, sizeof(Jango)); }
	void operator=(const Jango &rhs) { CopyMemory(this, &rhs, sizeof(Jango)); }

	char	flag;		// for Window Message
	char	acno[12];	// 계좌번호
	char	cod2[12];	// 종목코드
	char	hnam[40];	// 종목명
	char	jggb[2];	// 잔고코드
	int	jqty;		// 잔고수량
	int	xqty;		// 매도가능
	double	pamt;		// 매입평균가
	double	mamt;		// 매입금액

	int	curr;		// 현재가     (*)
	double	camt;		// 평가금액
	double	tamt;		// 평가손익   (*)
	double	srat;		// 평가수익률 (*)
	char	sycd[2];	// 신용코드
	char	sydt[8];	// 대출일
	char	samt[15];	// 신용금액
};