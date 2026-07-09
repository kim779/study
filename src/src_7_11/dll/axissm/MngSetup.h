#if !defined(AFX_MNGSETUP_H__F9434804_9CC8_4862_B832_1303010D6244__INCLUDED_)
#define AFX_MNGSETUP_H__F9434804_9CC8_4862_B832_1303010D6244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MngSetup.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CMngSetup dialog

class AFX_EXT_CLASS CMngSetup : public CDialog
{
// Construction
public:
	CMngSetup(CString home, CWnd* pParent = NULL);   // standard constructor


protected:
	CString		m_home;
	CString		m_user;

	bool		m_bApply;

	int		m_pos;		// 0:좌상단, 1:우상단, 2:좌하단, 3:우하단
	int		m_popup;
	int		m_sound;
	CMapStringToString	m_mapMng;

	void		loadInfo();
	void		saveInfo();
	void		setControlValue(CString keys, CString value);

public:
	void		ApplySetup();
	void		DefaultSetup();

// Dialog Data
	//{{AFX_DATA(CMngSetup)
	enum { IDD = IDD_MNGSETUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMngSetup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMngSetup)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MNGSETUP_H__F9434804_9CC8_4862_B832_1303010D6244__INCLUDED_)
