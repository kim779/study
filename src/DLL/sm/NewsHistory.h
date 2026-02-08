#if !defined(AFX_NEWSHISTORY_H__9BF415DC_4223_49BC_AC0C_FBAAA597E80E__INCLUDED_)
#define AFX_NEWSHISTORY_H__9BF415DC_4223_49BC_AC0C_FBAAA597E80E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewsHistory.h : header file
//
#include "resource.h"
#include "NewsListBox.h"
/////////////////////////////////////////////////////////////////////////////
// CNewsHistory dialog

class CNewsHistory : public CDialog
{
// Construction
public:
	CNewsHistory(bool (*axiscall)(int, WPARAM, LPARAM), CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewsHistory)
	enum { IDD = IDD_NEWHISTORY };
	NewsListBox 	m_list{};
	//}}AFX_DATA


	void	SetData(const CStringArray& ary);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewsHistory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStringArray	m_ary;
	bool		(*m_axiscall)(int, WPARAM, LPARAM);

	void	Init();
	void	fit();
	void	parseDat(CString dat, CMapStringToString& ary);
	// Generated message map functions
	//{{AFX_MSG(CNewsHistory)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkList();
	afx_msg void OnKillfocusList();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetfocusList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSHISTORY_H__9BF415DC_4223_49BC_AC0C_FBAAA597E80E__INCLUDED_)
