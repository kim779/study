#if !defined(AFX_AXMTABLEMERGECTRL_H__A01ADDC0_1DC8_4A58_BAF4_FCFD495BBE2C__INCLUDED_)
#define AFX_AXMTABLEMERGECTRL_H__A01ADDC0_1DC8_4A58_BAF4_FCFD495BBE2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxMTableMergeCtrl.h : header file
//

#include "AxMTableCtrl.h"
#include "AxMCellMergeCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CAxMTableMergeCtrl window

class AFX_EXT_CLASS CAxMTableMergeCtrl : public CWnd
{
// Construction
public:
	CAxMTableMergeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMTableMergeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMTableMergeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMTableMergeCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual void OnCancel();
	virtual void OnOK();
	virtual void OnInit();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CButton* m_btnInit, *m_btnOK, *m_btnCancel;
	CAxMCellMergeCtrl* m_cell;
	CFont		m_font;
	CWnd*		m_parent;

	void		CreateControls();
	void		CloseWindow();
	CString		m_mergeString;
	CAxMCellMerge	m_cellInfo;
public:
	void	SetRangeInformation(LPCTSTR rangeInformation);
	CString GetRangeInformation();
	BOOL	Create(CWnd* parent, int x, int y);

	CString		GetMergeString();
	void		SetMergeString(LPCTSTR lpszMergeString);

	void		SetSize(int cols, int rows);
	void		SetSize(LPRECT lpRect);
	void		SetSize(LPRECT lpRect, int cols, int rows);
	void		Initialize();

	BOOL		IsActive(int col, int row);
	CRect		GetRect(int col, int row);
	int			GetColCount();
	int			GetRowCount();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMTABLEMERGECTRL_H__A01ADDC0_1DC8_4A58_BAF4_FCFD495BBE2C__INCLUDED_)
