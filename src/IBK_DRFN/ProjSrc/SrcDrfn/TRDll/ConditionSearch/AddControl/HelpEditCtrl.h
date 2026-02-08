#if !defined(AFX_HELPEDITCTRL_H__17889DDA_0AE5_4784_8012_AE035CB90D93__INCLUDED_)
#define AFX_HELPEDITCTRL_H__17889DDA_0AE5_4784_8012_AE035CB90D93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHelpEditCtrl window

class CHelpEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CHelpEditCtrl();

// Attributes
public:
	CMenu		m_Menu;
	HCURSOR		m_DefaultCursor;

// Operations
public:
	BOOL CopyTextToClipboard();

	BOOL CopyText(LPSTR lpszBuffer, HWND hWnd = NULL);
	void CopyTextToClipboard(CString strData);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHelpEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHelpEditCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPEDITCTRL_H__17889DDA_0AE5_4784_8012_AE035CB90D93__INCLUDED_)
