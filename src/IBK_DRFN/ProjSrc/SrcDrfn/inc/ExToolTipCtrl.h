// ExToolTipCtrl.h: interface for the CExToolTipCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTOOLTIPCTRL_H__F3584501_8535_4BD9_91F7_BA501EFC7F71__INCLUDED_)
#define AFX_EXTOOLTIPCTRL_H__F3584501_8535_4BD9_91F7_BA501EFC7F71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoaderToolTipCtrl;
class CExToolTipCtrl  
{
public:
	CExToolTipCtrl();
	virtual ~CExToolTipCtrl();

	BOOL	AddTool(CWnd* pWnd, LPCTSTR lpszText = LPSTR_TEXTCALLBACK, LPCRECT lpRectTool = NULL, UINT nIDTool = 0);
	BOOL	AddTool(CWnd* pWnd, UINT nIDText, LPCRECT lpRectTool = NULL, UINT nIDTool = 0);
	void	Activate(BOOL bActivate);
	void	UpdateTipText(LPCTSTR lpszText, CWnd* pWnd, UINT nIDTool = 0);
	void	UpdateTipText(UINT nIDText, CWnd* pWnd, UINT nIDTool = 0);
	void	RelayEvent(LPMSG lpMsg);
	HWND	GetSafeHwnd();
	void	Update();
	void	DelTool(CWnd* pWnd, UINT nIDTool = 0);
	void	FillInToolInfo(TOOLINFO& ti, CWnd* pWnd, UINT nIDTool);
	int		GetDelayTime(DWORD dwDuration);
	void	GetMargin(LPRECT lprc);
	int		GetMaxTipWidth();
	void	GetText(CString& str, CWnd* pWnd, UINT nIDTool = 0);
	COLORREF	GetTipBkColor();
	COLORREF	GetTipTextColor();
	int		GetToolCount();
	BOOL	GetToolInfo(CToolInfo& ToolInfo, CWnd* pWnd, UINT nIDTool = 0);
	BOOL	HitTest(CWnd* pWnd, CPoint pt, LPTOOLINFO lpToolInfo);
	void	Pop();
	void	SetDelayTime(UINT nDelay);
	void	SetMargin(LPRECT lprc);
	int		SetMaxTipWidth(int iWidth);
	void	SetTipBkColor(COLORREF clr);
	void	SetTipTextColor(COLORREF clr);
	void	SetToolInfo(LPTOOLINFO lpToolInfo);
	void	SetToolRect(CWnd* pWnd, UINT nIDTool, LPCRECT lpRect);

protected:
	CLoaderToolTipCtrl*		m_pLoader;
};

#endif // !defined(AFX_EXTOOLTIPCTRL_H__F3584501_8535_4BD9_91F7_BA501EFC7F71__INCLUDED_)
