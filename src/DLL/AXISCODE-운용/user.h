#pragma once

// swap 사용 가능..

#define COR_BLUE		RGB(0, 0, 255)
#define COR_RED			RGB(255, 0, 0)
#define COR_BLACK		RGB(0, 0, 0)
#define COR_WHITE		RGB(255, 255, 255)
#define USER_BACKCOLOR		GetSysColor(COLOR_ACTIVEBORDER)//RGB(192, 192, 192)
#define USER_BACKCOLOR2		RGB(208,208,208)
#define USER_BT_NORMALCOR	RGB(210, 210, 210)
#define USER_FOCUS_COR		RGB(199, 203, 221)
#define COR_BACK		RGB(247,247,255)
#define COR_3DTOP		RGB(128,128,128)
#define COR_3DBOTTOM		RGB(221,221,209)
#define USER_BT_NORMALCOR1	RGB(170, 170, 170)

typedef struct _listItem { 

	char*	Code[12];
	char*	Name[32];
	char*	Date[8];

} LISTITEM;

typedef struct PCode
{
	CString kind;
	CString name;
	CString code;
	CString hnam;
	CString month;
	CString price;
	CString	gcod;
	BOOL	atm{};
	char	mchk{};
} PCODE;

typedef struct FCode
{
	CString code;
	CString name;
	char	mchk{};
} FCODE;

typedef struct SFCode
{
	CString codx;
	CString hnam;
	CString	enam;
	char	mchk{};
	CString	tjgb;
	CString	gcod;
	CString	gnam;
	CString mgub;		//2016.03.18 KSJ 마켓구분 코스피[K], 코스닥[Q]
} SFCODE;

typedef struct SFCodeInfo
{
	CString info[17];		//2015.12.01 KSJ 13 -> 17
	CString codx[17];		//2015.12.01 KSJ 13 -> 17
	CString nam[17];		//2015.12.01 KSJ 13 -> 17
}SFCODEINFO;

typedef struct SFCodeBase
{
	CString basecodx;
	CString basenam;
}SFCODEBASE;


