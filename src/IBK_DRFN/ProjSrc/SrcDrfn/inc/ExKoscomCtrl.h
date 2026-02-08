#if !defined(AFX_EXKOSCOMCTRL_H__3511D941_A193_4AAB_8EFF_82C0E8F6BEC3__INCLUDED_)
#define AFX_EXKOSCOMCTRL_H__3511D941_A193_4AAB_8EFF_82C0E8F6BEC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EXKOSCOMCTRL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEXKOSCOMCTRL window
class CLoaderKoscomCtrl;
class CExKoscomCtrl
{
public:
	CExKoscomCtrl(void);
	~CExKoscomCtrl(void);

// Attributes
protected:
	CLoaderKoscomCtrl*		m_pLoader;
	BOOL					m_bCreated;


	// Operations
public:

	BOOL	InitKoscomCtrl(CWnd *pParentWnd, UINT nCtrlType, UINT nCtrlID, long pMasterMgr);

	BOOL	InitCodeInput(CWnd *pParentWnd, UINT nCtrlID, UINT nProType);
	BOOL	InitButton(CWnd *pParentWnd, UINT nCtrlID);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/22
	// Return		: short 입력된 코드의 구분값
	// Paramter
	//		LPCTSTR szCode : 코드번호 문자열
	// Comments		: 종목코드, 업종코드등의 구분값을 구한다
	//-----------------------------------------------------------------------------
	short	GetGubunFromCode(LPCTSTR szCode);

	CString GetDataText();
	void	SetDataText(LPCTSTR szText);
	void	SetEnabled(BOOL bEnable);
	BOOL	GetEnabled();

	long	GetBase(LPCTSTR lpszKey);

	void	SetFocus();

	OLE_HANDLE	GetHWnd();		// OLE_HANDLE
	HWND	GetSafeHwnd();		// HWND

	void	MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void	MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
	BOOL	SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags);
	BOOL	ShowWindow(int nCmdShow);
	void	GetWindowRect(LPRECT lpRect);
	void	GetClientRect(LPRECT lpRect);
	void	SetMarketKind(UINT nMarketKind);
	UINT	GetMarketKind();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXKOSCOMCTRL_H__3511D941_A193_4AAB_8EFF_82C0E8F6BEC3__INCLUDED_)