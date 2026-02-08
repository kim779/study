// WndObj.h: interface for the CWndObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDOBJ_H__C5CLIQDA_98CC_9723_A19C_2CF28FC0LEC9__INCLUDED_)
#define AFX_WNDOBJ_H__C5CLIQDA_98CC_9723_A19C_2CF28FC0LEC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "../../include/WndObj.h"

typedef struct 
{
	char *szKey;
	VARIANT vValue;
} STDATAITEM;
#define SZSTDATAITEM	sizeof(STDATAITEM)

DECLARE_INTERFACE_(IPlatformObj, IUnknown)
{
public:
	// *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * lplpObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

	// *** User methods ***
	STDMETHOD_(long, PFGetBase)(LPCTSTR szKey) PURE;
	STDMETHOD_(long, PFGetKey)(LPCTSTR szKey) PURE;		// szKey : "MainDll", "BaseDesk", "RscMng"
	STDMETHOD_(void, PFSetKey)(LPCTSTR szKey, long dwKey) PURE;
	STDMETHOD_(void, PFInitialize) (long dwMainDllObj, long dwBaseDesk, long dwRscMng, CStringList* pAllProperties) PURE;
	STDMETHOD_(void, PFSetProperty) (STDATAITEM *stData, int nDataCnt) PURE;
	STDMETHOD_(long, PFGetState) (int nStatus, long dwData1, long dwData2) PURE;
	STDMETHOD_(long, PFSetState) (int nStatus, long dwData1, long dwData2) PURE;
	STDMETHOD_(LRESULT, PFWindowProc) (UINT message, WPARAM wParam, LPARAM lParam, int &nContinue) PURE; 
};

/////////////////////////////////////////////////////////////////////////////
// CWndObj class

DECLARE_INTERFACE_(IAWndObj, IUnknown)
{
public: 
	// *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * lplpObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

	// *** User Property or Methods ***

//	BOOL	 m_bVisible;		// 표시 유/무
//	BOOL	 m_bEnable;			// Enable 유/무
//	BOOL	 m_bTabStop;		// Tab 사용 유/무
//	BOOL	 m_bTransParent;	// 투영
//	BOOL	 m_bParentFont;		// Parent Font
//	BOOL	 m_bToolTip;		// ToolTip
//	BOOL	 m_bSvrDisable;		// Disable 구분(서버)
//	BOOL	 m_bSvrError;		// Error & Disable 구분(서버)
//	BOOL	 m_bSvrColor;		// Color 구분(서버)
//
//	short	 m_nFontID;			// Font ID
//	short	 m_nTabIndex;		// Tab Index
//	short	 m_nObjID;			// Object ID
//	CRect	 m_rectObj;			// 객체 영역(가변)
//	CRect	 m_rectOrg;			// 객체 영역(Origin)
//
//	CString	 m_strName;			// 객체 이름
//	CString	 m_strText;			// 객체 Text
//	CString	 m_strToolTip;		// 툴팁 Text
//	COLORREF m_clrText;			// 전경색
//	COLORREF m_clrBack;			// 배경색
//	COLORREF m_clrLine;			// 라인색

	CWnd	*m_pOwnerWnd;		// Owner Window

// Operations
public:
//	virtual void Draw(CDC *pDC, CGdiMgr *pGdiMgr) = 0;
//	virtual void Print(CDC *pDC, CGdiMgr *pGdiMgr) = 0;
//	virtual void SerializeLoad(CArchivePro *ar);
//	virtual void SerializeSave(CArchivePro *ar);
//	virtual void GetData(CString &strData, BOOL bRTM = FALSE) {}
//	virtual BOOL SetData(CString strData, BOOL bRTM = FALSE) { return FALSE; }
//	virtual void GetOccursData(CString &strData, short &nCount, short nType = 1) {}
//	virtual BOOL SetOccursData(CString strData, short nCount, BOOL bRTM = FALSE) { return FALSE; }
//	virtual long *GetProperty(long nType);
//	virtual BOOL SetProperty(long nType, long *Val);
//	virtual void PropertySheet() {}
//	virtual void ScriptMethod(long nMsgID) {}
//	virtual void ScriptProperty(long nMsgID) {}
//	virtual void SetDifInfo(LPTSTR strInfo) {}

//	virtual void OnLButtonDown(UINT nFlags, CPoint point) {}
//	virtual void OnLButtonUp(UINT nFlags, CPoint point) {}
//	virtual void OnLButtonDblClk(UINT nFlags, CPoint point) {}
//	virtual BOOL OnMouseMove(UINT nFlags, CPoint point) { return FALSE; }
//	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {}
//	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {}
//	virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {}
	STDMETHOD_(void, OnSize) (UINT nType, int cx, int cy) PURE;
//	virtual void OnSetFocus() {}
//	virtual void OnKillFocus() {}
//	virtual BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {	return FALSE; }
//	virtual BOOL OnContextMenu(CWnd *pWnd, CPoint point, CPoint ptDoc = (0,0), long ptr = 0) { return FALSE; }
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) { return FALSE; }
//	virtual void OnDestroy() {}
//	virtual void OnTimer(UINT nIDEvent) {}
//	virtual BOOL WindowProc(UINT message, WPARAM wParam, LPARAM lParam) { return FALSE; }

//	virtual CCmdTargetPlus *GetDispatch() { return NULL; }
//	STDMETHOD_(void, OnDraw)(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)PURE;
//	STDMETHOD_(int, OnMouseActivate)(CWnd* pDesktopWnd, UINT nHitTest, UINT message)PURE;

protected:
//	void Initialize();
};
typedef       IAWndObj FAR * LPIAWNDOBJ;

#define AZMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;


#endif // !defined(AFX_WNDOBJ_H__C5CLIQDA_98CC_9723_A19C_2CF28FC0LEC9__INCLUDED_)
