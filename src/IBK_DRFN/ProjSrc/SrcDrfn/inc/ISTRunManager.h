#if !defined(AFX_INTERFACE_OF_STRUNMANAGER_BY_JUNOKLEE_060721A__INCLUDED_)
#define AFX_INTERFACE_OF_STRUNMANAGER_BY_JUNOKLEE_060721A__INCLUDED_

// #include "../../inc/ISTRunManager.h" // 전략실행관련 인터페이스 제공.

interface ISTRunManager
{
	// Return Value : 0(Sucess), 0이외의값(Error Code)
	// pInData : STMNG_ITEMINFO*
	STDMETHOD_(int, RunST)(BOOL bImsiRun, CString &szRunFile, LONG pInData)  PURE;

	// 전략정보를 가져온다.
	// 리턴값은 STMNG_ITEMINFO* 형임.
	STDMETHOD_(LONG, GetSelectedSTInfo) () PURE;

	// View Handle을 가져옴
	STDMETHOD_(LONG, GetViewHandle) () PURE;

	// OutBar Color
	STDMETHOD_(LONG, GetOutBarColor) (COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine) PURE;
};

#endif // AFX_INTERFACE_OF_STRUNMANAGER_BY_JUNOKLEE_060721A__INCLUDED_