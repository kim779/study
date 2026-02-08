#if !defined(AFX_INTERFACE_OF_CHARTUTIL_MANAGER_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_CHARTUTIL_MANAGER_BY_JUNOK_LEE__INCLUDED_

// #include "../../_include/IAUChartUtilMng.h"
// 

#include "IBaseDefine.h"
#include <AFXTEMPL.H>

#define		UUID_IAUChartUtilManager				44

// 사용예는 이 파일의 제일 하단 부분의 코멘트것 참조.
//
//
/* -------------------------------------------------------------------------
설명 : 차트관련 기능을 위한 기능 제공트리형태의 리스트뷰지원
만든이  : 이준옥 2007.12.10(월)
History : 2007.12.10 인터페이스 정의
----------------------------------------------------------------------------
int nID = UUID_IChartUtilManager;
IChartUtilManager* pManager = (IChartUtilManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
IChartUtilManager* pManager = (IChartUtilManager*)AfxGetPctrInterface(nID);

------------------------------------------------------------------------- */

#define		ID_IAUChartUtilManager_Max				3
#define		ID_IAUChartUtilManager_FloatingDlg		1

// 수치조회창 클라이언트 인터페이스
DECLARE_INTERFACE(IChartFloatClient)
{
	// 0:Hide, 1:Show
	STDMETHOD_(void, StateChanged)(UINT nState) PURE;
};

// 수치조회창 인터페이스
DECLARE_INTERFACE(IChartFloatDlg)
{
	STDMETHOD_(LONG, AdviseFloatDlg)(IChartFloatClient* pIClient) PURE;
	STDMETHOD_(void, UnAdviseFloatDlg)(IChartFloatClient* pIClient) PURE;

	// Floating시세창 Show/Hide
	// 기존에 로딩되어 있지 않으면 로드한다.
	// nShow 0: Hide
	//		 1 : Show
	//		 2 : Toggle
	STDMETHOD_(void, ShowFloatDlg)(IChartFloatClient* pIClient, int nShow=2) PURE;

	// 상태정보 얻기
	// 0:Hide, 1:Show
	STDMETHOD_(int, GetFloatSiseState)() PURE;

	// 마우스 이동시 시세데이터를 넘겨준다.
	STDMETHOD_(void, SetDataWindowData)(CList<CString, CString&>* pListData) PURE;
};

DECLARE_INTERFACE(IAUChartUtilManager)
{
	STDMETHOD_(IChartFloatDlg*, AdviseChartFloatDlg)() PURE;
	STDMETHOD_(void, UnAdviseChartFloatDlg)(IChartFloatDlg*) PURE;
};

#endif //AFX_INTERFACE_OF_CHARTUTIL_MANAGER_BY_JUNOK_LEE__INCLUDED_
