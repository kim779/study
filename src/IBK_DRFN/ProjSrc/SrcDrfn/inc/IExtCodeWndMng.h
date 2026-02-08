#if !defined(AFX_INTERFACE_OF_EXTCODEWINDOW_MANAGER_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_EXTCODEWINDOW_MANAGER_BY_JUNOK_LEE__INCLUDED_

// #include "../../_include/IExtCodeWndMng.h"
// 

#include "IBaseDefine.h"

/* -------------------------------------------------------------------------
설명 : 기존의 code.ocx에서 제공하는 코드조회창을 대체하기위한 DLL 주식코드입력기
만든이  : 이준옥 2006.01.06(금)
History : 2006.01.06 인터페이스 정의
----------------------------------------------------------------------------
int nID = 27;
IExtCodeWndManager* pManager = (IExtCodeWndManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
IExtCodeWndManager* pManager = (IExtCodeWndManager*)AfxGetPctrInterface(nID);

------------------------------------------------------------------------- */
DECLARE_INTERFACE(IExtCodeWndManager)
{
	/* --------------------------------------------------- 
	-. 설명[ShowCodeTable]
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(long, ShowCodeTable)(LPCTSTR szCodeKey, const VARIANT& hWnd) PURE;

	/* --------------------------------------------------- 
	-. 설명[ShowModeCodeTable]내부적으로도 하는 기능 없음.
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(long, ShowModeCodeTable)(LPCTSTR szCodeKey, const VARIANT& hWnd) PURE;

	/* --------------------------------------------------- 
	-. 설명
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(void, SetPosition)(LPCTSTR szKey, short nX, short nY) PURE;

	/* --------------------------------------------------- 
	-. 설명
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(long, ShowCodeTableWithPosition)(LPCTSTR szCodeKey, const VARIANT& hWnd, short nX, short nY) PURE;

	/**********************************************************************************
	-. 설 명 : App 에 있는 m_HistoryList 에 데이터를 추가한다.
	-. 변 수 
	short nMaxCount  : History 창에 보여줄 List 의 갯수
	long nType       : 28byte(CodeInput 의 CodeInput 의 m_szCodeChoice 의 long 값)
	LPCTSTR Code     : Code (예 : 00660 )
	LPCTSTR CodeName : Code Name ( 예 : 하이닉스 )
	-. 리턴값 : 동일한 history 가 있으면 FALSE, 아니면 TRUE 를 리턴한다.
	***********************************************************************************/
	STDMETHOD_(BOOL, InsertHistory)(short nMaxCount, long nType, LPCTSTR Code, LPCTSTR CodeName) PURE;
	
	/**********************************************************************************
	-. 설 명 : nType 에 맞는 History 데이터를 리턴한다.
	-. 변 수 
	   short nMaxCount  : History 창에 보여줄 List 의 갯수
	   long nType       : 28byte(CodeInput 의 CodeInput 의 m_szCodeChoice 의 long 값)
	-. 리턴값  : 타입에 맞는 코드/코드명을 리스터를 리턴한다.
		양식: "Code0=CodeName0;Code1=CodeName1;" 로 리턴한다.
	***********************************************************************************/
	STDMETHOD_(CString, GetHistoryList)(short nMaxCount, long nType) PURE;

	STDMETHOD_(void, CleanCodeTable)() PURE;

};

#endif //AFX_INTERFACE_OF_EXTCODEWINDOW_MANAGER_BY_JUNOK_LEE__INCLUDED_
