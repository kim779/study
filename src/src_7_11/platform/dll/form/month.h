#if !defined(AFX_MONTH_H__B82E9375_E53E_4A70_AB5B_3D88F046F251__INCLUDED_)
#define AFX_MONTH_H__B82E9375_E53E_4A70_AB5B_3D88F046F251__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// month.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Cmonth window

class Cmonth : public CMonthCalCtrl
{
// Construction
public:
	Cmonth();

// Attributes
protected:
	class	CfmEdit* m_edit;
	CString	m_date;

// Operations
public:
	BOOL	Create(CWnd* parent, CRect rect, CfmEdit* edit);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cmonth)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~Cmonth();

	// Generated message map functions
protected:
	void	setDate(bool toME, bool only = false);

	//{{AFX_MSG(Cmonth)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnReleasedcapture(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelect(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnMcnGetdaystate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONTH_H__B82E9375_E53E_4A70_AB5B_3D88F046F251__INCLUDED_)
