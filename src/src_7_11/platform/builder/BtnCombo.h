#if !defined(AFX_BTNCOMBO_H__08214C27_871C_4900_A97D_2BFAF5286187__INCLUDED_)
#define AFX_BTNCOMBO_H__08214C27_871C_4900_A97D_2BFAF5286187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BtnCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBtnCombo window

class CBtnCombo : public CWnd
{
// Construction
public:
	CBtnCombo();

// Attributes
public:
	CComboBox	*m_pCombo;
	CFont		*m_pFont;
	DWORD		m_dwState;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBtnCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void	setComboWidth();
	void	SetItemData(int idx, DWORD data);
	
	CString GetText();
	int	AddString(CString str);

	void	setFont(CFont *pFont);
	virtual ~CBtnCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBtnCombo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSelOk();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BTNCOMBO_H__08214C27_871C_4900_A97D_2BFAF5286187__INCLUDED_)
