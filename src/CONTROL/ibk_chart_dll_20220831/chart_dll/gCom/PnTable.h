#if !defined(AFX_PNTABLE_H__A6711A61_C432_4CA8_AAD0_03ED7DF10E31__INCLUDED_)
#define AFX_PNTABLE_H__A6711A61_C432_4CA8_AAD0_03ED7DF10E31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnTable window
#include "PnWndGrid.h"

class CPnTable : public CWnd
{
// Construction
public:
	CPnTable(CWnd *pView, CWnd* pParent, CFont* pFont, int ctrlID, char* info);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnTable)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnTable();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnTable)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT	OnTableEvent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void	setItem(char *pData);
	char*	setRowCol(char *pData);
	void	setTable(char *pData);
	void	setReal(bool bShift, CString realS);
	void	insertColumn(bool bInsert, CString insertS);
	void	selectIndex(int index);
	void	setItemData(GV_ITEM item, int row, int col, CString text);

private:
	CAxisGComApp	*m_pApp;
	CWnd		*m_pView;
	CWnd		*m_pParent;
	CFont		*m_pFont;
	class CPnWndGrid	*m_pGrid;

	int		m_row;	// 가로줄수(수평개수)
	int		m_col;	// 세로줄수(수직개수)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNTABLE_H__A6711A61_C432_4CA8_AAD0_03ED7DF10E31__INCLUDED_)
