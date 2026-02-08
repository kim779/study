// SfnCodeInput.h : main header file for the SFNCODEINPUT DLL
//

#if !defined(AFX_SFNCODEINPUT_H__815B455B_CCE6_4427_AFCA_A28FF8F07531__INCLUDED_)
#define AFX_SFNCODEINPUT_H__815B455B_CCE6_4427_AFCA_A28FF8F07531__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CSfnCodeInput
// See SfnCodeInput.cpp for the implementation of this class
//

// #include "../../../../../Interface/ICtlBase.h"
// #include "../../../../CtrlWrap/WrpItemCode.h"		// 종목컨트롤
class CSfnCodeInput
{
public:
	CSfnCodeInput();
	~CSfnCodeInput();
	short	GetGubunFromCode(LPCTSTR szCode){return 0;};

	void	SetDataText(LPCTSTR szText);
	void	SetDataText2(LPCTSTR szCode);
	CString	GetDataText();

	void	SetEnabled(BOOL propVal);
	BOOL	GetEnabled();
	
	long	GetBase(LPCTSTR lpszKey);
	
	void	SetFocus();
	
	void	SetCurSel(short nIndex);

	OLE_HANDLE	GetHWnd();
	HWND	GetSafeHwnd();
	
	void	MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint);
	void	MoveWindow(LPCRECT lpRect, BOOL bRepaint);
	BOOL	SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags);
	BOOL	ShowWindow(int nCmdShow);
	void	GetWindowRect(LPRECT lpRect);
	void	GetClientRect(LPRECT lpRect);

//void	SetMember(ICtlBase* pCtlItemCode, CWrpItemCode* pItemCode);
private:
//	CWrpItemCode*	m_pItemCode;
//	ICtlBase*		m_pCtlItemCode;
	HWND			m_hItemCode;
//@신영삭제	CWnd*			m_pControlWnd;
};


/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_SFNCODEINPUT_H__815B455B_CCE6_4427_AFCA_A28FF8F07531__INCLUDED_)
