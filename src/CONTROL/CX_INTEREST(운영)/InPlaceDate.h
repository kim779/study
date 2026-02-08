#pragma once
// InPlaceDT.h : header file
//
#include "datetime.h"
/////////////////////////////////////////////////////////////////////////////
// CInPlaceDate window

class CInPlaceDate : public CWnd
{
// Construction
public:
	CInPlaceDate(CWnd* parent, CRect& rect, UINT nid, CString date = _T(""), int row = -1, int col = -1, CString format = _T("yyyy/MM/dd"));

// Attributes
public:
	CWnd* m_parent{};
	CRect	m_rect;
	int	m_row{}, m_col{};
	UINT	m_nid{};

	std::unique_ptr<CDateTime> m_date;

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
	CWnd* m_parent{};
	CRect	m_rect;
	int	m_row{}, m_col{};
	UINT	m_nid{};

	std::unique_ptr<CDateTime> m_time;

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