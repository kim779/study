#ifndef __common_h__
#define __common_h__

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

// define Color!
#define COLOR_LINE			93
#define COLOR_TABLE			75
#define COLOR_BK			64
#define COLOR_GRIDHEAD		74
#define COLOR_GUIDEBACK		213
#define COLOR_PANELBACK		66
#define COLOR_BODYBACK		181
#define COLOR_GRIDHEADTXT	76
#define COLOR_DATA			99
#define COLOR_TEXT			69
#define COLOR_TABLEHEAD		96
#define	COLOR_MDB			171		// 매도 back
#define	COLOR_MSB			170		// 매수 back
#define COLOR_PLUSTXT		94
#define COLOR_MINUSTXT		95
#define COLOR_FOCUS			78
#define COLOR_HEADER		66
#define COLOR_ODD_ROW		68
#define COLOR_EVEN_ROW		77
#define COLOR_SELECT_ROW	78

#define PADDING_VALUE	(6)

// 그리드 셀 높이
#define GRID_CELL_HEIGHT	(18)

// Static 높이
#define DEF_STATIC_HEIGHT	(20)

// TR key
typedef enum {
	TK_SDLDQ003_GROUP	= 1,
	TK_SDLDQ003_ACCOUNT	= 2,
	TK_SDLDT003			= 3,
} TRKEY;

class StringProxy : public pair<LPCSTR,LPCSTR> {
public:
	StringProxy() : pair<LPCSTR,LPCSTR>() {}
	StringProxy(LPCSTR st, LPCSTR ed) : pair<LPCSTR, LPCSTR>(st, ed) {}
	string ToStr() { return string(first, second); }
	CString ToCStr() { return CString(first, second-first); }
};

typedef struct _accn_info
{
	struct _accn_info() {
		ZeroMemory(this, sizeof(struct _accn_info));
	}
	char acno[21];
	char acno2[21];	//작대기(-) 넣은 계좌번호
	char acnm[41];
	char acpw[45];	//2013.02.18 KSJ 암호화때문에 44byte로 바뀜
	int  mult;
	double rate;
	BOOL used;
} accn_info;

typedef struct _op_eq_acno
{
	LPCSTR m_acno;
	struct _op_eq_acno(LPCSTR acno)
	{
		m_acno = acno;
	}
	BOOL operator()(accn_info &rsh)
	{
		return strcmp(m_acno, rsh.acno)==0;
	}
} op_eq_acno;

typedef struct _op_le_acno
{
	LPCSTR m_acno;
	struct _op_le_acno(LPCSTR acno)
	{
		m_acno = acno;
	}
	BOOL operator()(accn_info &rsh)
	{
		return strcmp(m_acno, rsh.acno);
	}
} op_le_acno;

typedef struct _grop_info
{
	struct _grop_info() { 
		ZeroMemory(this, sizeof(struct _grop_info));
	}
	char gpno[21];
	char gpnm[41];
	BOOL load;
} grop_info;

typedef map<CString, vector<accn_info> > accn_data;
typedef map<CString, vector<accn_info> >::iterator p_accn_data;
typedef map<CString, grop_info> grop_data;
typedef map<CString, grop_info>::iterator p_grop_data;

typedef struct _sdldq003_mid
{
	char irec[5];
	char gubn[1];		// 1:그룹조회 2:계좌조회
	char type[2];		// 10:주식, 11:선물, 12:혼합
	char gpno[20];
	char gpnm[40];
} sdldq003_mid;

typedef struct _sdldq003_mod
{
	sdldq003_mid mid;
	char orec[5];
	struct _grid 
	{
		char gpno[20];
		char gpnm[40];
		char acno[20];
		char acnm[40];
		char acpw[44];	//2013.02.18 KSJ 암호화때문에 44byte로 바뀜
		char rate[17];
		char mult[17];
		char nick[40];
	} grid[1];
} sdldq003_mod;

typedef struct _sdldt003_grid
{
	char acno[20];
	char acpw[44];
	char acnm[40];
	char rate[17];
	char mult[17];
	char nick[40];
} sdldt003_grid;

typedef struct _sdldt003_mid
{
	char irec[5];
	char gubn[1];		// 1:그룹조회 2:계좌조회
	char type[2];		// 10:주식, 11:선물, 12:혼합
	char gpno[20];
	char gpnm[40];
	char orec[5];
	//sdldt003_grid grid[1];
} sdldt003_mid, sdldt003_mod;

#endif