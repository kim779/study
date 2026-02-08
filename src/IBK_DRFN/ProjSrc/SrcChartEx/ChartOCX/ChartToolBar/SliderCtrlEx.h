#if !defined(AFX_SLIDERCTRLEX_H__FAB8BF09_1085_458F_8943_E0C88E507AFD__INCLUDED_)
#define AFX_SLIDERCTRLEX_H__FAB8BF09_1085_458F_8943_E0C88E507AFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SliderCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrlEx window

class CSliderCtrlEx : public CSliderCtrl
{
// Construction
public:
	CSliderCtrlEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSliderCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSliderCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSliderCtrlEx)
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// (2008/12/20 - Seung-Won, Bae) Draw with PolyPolyLine()
protected:
	POINT *	m_ptPoints;
	DWORD *	m_dwLines;
	int		m_nLines;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDERCTRLEX_H__FAB8BF09_1085_458F_8943_E0C88E507AFD__INCLUDED_)
