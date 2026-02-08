#pragma once
  
#include <afxtempl.h>

// swap 사용 가능..

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define COR_BLUE		RGB(0, 0, 255)
#define COR_RED			RGB(255, 0, 0)
#define COR_BLACK		RGB(0, 0, 0)
#define COR_WHITE		RGB(255, 255, 255)
#define USER_BACKCOLOR		RGB(222,219,222)
#define USER_BACKCOLOR2		RGB(208,208,208)
#define USER_BT_NORMALCOR	RGB(210, 210, 210)
#define USER_FOCUS_COR		RGB(199, 203, 221)
#define COR_BACK		RGB(247,247,255)
#define COR_3DTOP		RGB(128,128,128)
#define COR_3DBOTTOM		RGB(221,221,209)


//
//	user/userName폴더
//	file name : portfolio.i00 ~ portfolio.i99
//
// char gName[20];		// 그룹이름

struct _inters
{
	char	gubn;		// 종목구분
				// 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12];	// 종목코드
	char	name[30];	// 종목명
	int	possNum;	// 보유수량
	double	possPrice;	// 보유단가
	char	memo[30];	// 메모
	char	o_check;	// 주문 실행여부
				// 0:none, 1:checked
	char	o_kind;		// 주문구분
				// 1:매도, 2:매수
	char	o_mmgb;		// 매매구분
				// 1:보통, 2:시장가
	int	o_num;		// 주문수량
	double	o_price;	// 주문단가
	char	a_kind;		// 알람 설정
				// 1:그림만, 2:그림과 소리
	int	a_num;		// 조건 항목 갯수
	
	char	filler[100];	// Reserved
};

struct _alarms
{
	char	a_kind;		// 감시항목
				// 0:none, 1:현재가, 2:매도호가, 3:매수호가, 4:등락률, 5:손익평가, 6:수익률
	double	a_value;	// 감시 비교값
	char	a_condition;	// 감시 조건
				// 1:>=, 2:>, 3:=, 4:<, 5:<=
	char	filler[40];	// Reserved
};

typedef struct _listItem { 

	char*	Code[10];
	char*	Name[20];

}LISTITEM;

typedef struct PCode
{
	CString kind;
	CString name;
	CString code;
	CString hnam;
	CString month;
	CString price;
	BOOL	atm;
	char	mchk;
}PCODE;

typedef struct FCode
{
	CString code;
	CString name;
	char	mchk;
}FCODE;
