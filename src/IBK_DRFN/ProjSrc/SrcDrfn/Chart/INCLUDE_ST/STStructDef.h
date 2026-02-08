#ifndef __STRATEGY__STRUCT__DEFINE__BYALZIO__060914__HEADER__
#define __STRATEGY__STRUCT__DEFINE__BYALZIO__060914__HEADER__

// #include "../Include_ST/STStructDef.h"
typedef struct 
{
	HWND	hSystemTabEx;	// TabSystemTradingEx의 윈도우 핸들
	HWND	hSTAddin;		// ChartSTAddin의 SiteCustumAddin의 윈도우핸들
} stRunWndInfo;


typedef struct
{
	CString szSTFile;
	long	pChartInterface;
	CString szCode;
	int		nCount;
	CString szEndData;
	CStringArray stArrayPath;
	CString szSTName;		//전략명 : KHD : 2006.11.16

	stRunWndInfo wndInfo;
	char szCodeName[30+1];
	int  nCodeType;			//CTabSystemTradingEx::CSTAnalysisInput::GetCodeType()에서 넘겨받는 값.
	// 0:주식, 1:선옵, 2:업종
} stAddinSTRun;


// 실행중인 전략의 타입
// 조합된 형태로 실행될 수는 있으나, 멀티차트의 시스템탭에 정의된 형태로 처리한다.
// 즉 (시스템탭의)제공지표 또는 사용자지표 (STRun_Type_Index)
//	  제공하는 강세약세 또는 사용자강세약세(STRun_Type_BullBear)
//	  그외 ST 전략
enum
{
	STRun_Type_None=0,	// None
	STRun_Type_Index=1,		// 지표
	STRun_Type_ST=2,		// 전략
	STRun_Type_BullBear=4,	// 강세,약세
};

#define	Version_stAddinSTRunInfo		070213
// ChartSTIndexAddin (11) 을 통해서 실행중인 전략을 확인하고자 할때.
// ChartSTIndexAddin를 통해서 접근할 경우에는 멀티차트에서 선택된 item에 해당하는 정보만 접근할 수 있음.
// 메인 실행중에 실행중인 전략을 확인하려면 PcTr를 통해서 접근해야함.
typedef struct 
{
	// Input
	LPCSTR szSTFile;		// 전략 FullPath명.

	// Output
	BOOL	bIsRun;			// 전략의 실행여부
	CString szRunSTFile;	// 실행중인 전략패스(SD)

	int		nSTType;
	CString szSTName;		// 전략명

	int		nRunSTType;		// ChartSTIndexAddin에서 결과로 넘겨주는 정보.

} stAddinSTRunInfo;

#endif //__STRATEGY__STRUCT__DEFINE__BYALZIO__060914__HEADER__