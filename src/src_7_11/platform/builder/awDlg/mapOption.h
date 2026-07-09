#if !defined(AFX_MAPOPTION_H__FACCAE21_5674_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_MAPOPTION_H__FACCAE21_5674_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mapOption.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMapOption dialog

class AFX_EXT_CLASS CMapOption : public CDialog
{
	DECLARE_DYNCREATE(CMapOption)

// Construction
public:
	CMapOption()	{}
	CMapOption(struct _mapH* mapH);
	~CMapOption();

// Dialog Data
	//{{AFX_DATA(CMapOption)
	enum { IDD = IDD_GOPTION };
	BOOL	m_opt01;
	BOOL	m_opt04;
	BOOL	m_opt06;
	BOOL	m_opt09;
	BOOL	m_opt11;
	BOOL	m_opt12;
	BOOL	m_opt15;
	BOOL	m_opt16;
	BOOL	m_opt17;
	BOOL	m_opt18;
	BOOL	m_opt19;
	BOOL	m_opt22;
	BOOL	m_opt23;
	BOOL	m_opt24;
	BOOL	m_opt25;
	BOOL	m_opt26;
	BOOL	m_opt27;
	//}}AFX_DATA

protected:
	struct	_mapH*	m_mapH;
	CButton*	m_typeB[8];

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMapOption)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMapOption)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnClick(UINT cmdID);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPOPTION_H__FACCAE21_5674_11D4_A024_00001CD7F9BE__INCLUDED_)
