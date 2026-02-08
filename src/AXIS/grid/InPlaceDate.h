#if !defined(AFX_INPLACEDATE_H__AF148CE3_46A9_4F45_9097_2C02C922B0B7__INCLUDED_)
#define AFX_INPLACEDATE_H__AF148CE3_46A9_4F45_9097_2C02C922B0B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InPlaceDT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInPlaceDate window
#include "fxGrid.h"
class CInPlaceDate : public CWnd
{
// Construction
public:
	CInPlaceDate(CWnd* parent, CRect& rect, UINT nid, CString date = _T(""), int row = -1, int col = -1, CString format = _T("yyyy/MM/dd"));

// Attributes
public:
	CWnd*	m_parent;
	CRect	m_rect;
	int	m_row, m_col;
	UINT	m_nid{};

// Operations
public:
	void	EndDate();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceDate)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	afx_msg void OnDateChange(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnUserString(NMHDR* pNotifyStruct, LRESULT* result);
// Implementation
public:
	virtual ~CInPlaceDate();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceDate)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CInPlaceTime window

class CInPlaceTime : public CWnd
{
// Construction
public:
	CInPlaceTime(CWnd* parent, CRect& rect, UINT nid, CString date = _T(""), int row = -1, int col = -1, CString format = _T("HH:m:s"));
// Attributes
public:
	CWnd*	m_parent;
	CRect	m_rect;
	int	m_row, m_col;
	UINT	m_nid{};

// Operations
public:
	void	EndTime();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceTime)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
//	afx_msg void OnDateChange(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnUserString(NMHDR* pNotifyStruct, LRESULT* result);

// Implementation
public:
	virtual ~CInPlaceTime();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceTime)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACEDATE_H__AF148CE3_46A9_4F45_9097_2C02C922B0B7__INCLUDED_)
