#if !defined(AFX_PNWNDCOMBO_H__F1C4B6F8_58AC_49EF_9CDB_313CC6FCC887__INCLUDED_)
#define AFX_PNWNDCOMBO_H__F1C4B6F8_58AC_49EF_9CDB_313CC6FCC887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnWndCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnWndSubEdit window

class CPnWndSubEdit : public CEdit
{
// Construction
public:
	CPnWndSubEdit(class CPnWndCombo *pCombo, CWnd* pParent, CWnd *pView, int type);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnWndSubEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnWndSubEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnWndSubEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void	SetUnit(int unit) { m_Unit = unit; }

private:

	class CPnWndCombo	*m_pCombo;
	CWnd		*m_pParent;
	CWnd		*m_pView;
	int		m_Type;

	int		m_Unit;
	bool		m_bKeyChange;
};

/////////////////////////////////////////////////////////////////////////////
// CPnWndCombo window

struct _combo
{
	int	val;
	char	name[40];
};

class CPnWndCombo : public CComboBox
{
// Construction
public:
	CPnWndCombo(CWnd* pParent, CWnd *pView, int type);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnWndCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnWndCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnWndCombo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSelchange();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void	SetData(struct _combo* data, int cnt);
	int	SelectCombo(int data);
	void	GetData(int& val);
	CString	GetEditData();
	void	SetEditData(CString sData);
	void	SetUnit(int unit);
	bool	IsFocusWnd();
	void	SetComboPalette(COLORREF bg, COLORREF fg) {m_cbRGB = bg; m_csRGB = fg;}
	void	DrawCombo(CDC *pDC);

private:
	void	CreateSubEdit();
	void	DeleteSubEdit();

	// Drawing Combo
	COLORREF	m_cbRGB;		//색상 저장변수
	COLORREF	m_csRGB;

public:
	class CPnWndSubEdit	*m_pSubEdit;
private:
	CWnd		*m_pParent;
	CWnd		*m_pView;
	int		m_Type;
	int		m_Unit;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNWNDCOMBO_H__F1C4B6F8_58AC_49EF_9CDB_313CC6FCC887__INCLUDED_)
