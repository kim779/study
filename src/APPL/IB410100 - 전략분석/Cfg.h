
#include "cxcfg.h"

#pragma once
// Cfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCfg window

class CCfg : public CWnd
{
// Construction
public:
	CCfg();

// Attributes
public:
	CWnd*		m_parent{};
	CString		m_root;

	HINSTANCE	m_hInst;
	CWnd		*m_WCfg;
	std::unique_ptr <ICfgWnd>m_ICfg{};

	CWnd*		(APIENTRY* axCreate)(CWnd*, void*) {};

// Operations
public:
	BOOL	CreateCfg(CWnd* parent, CString root);

	BOOL	SetMap(LPCTSTR PrivateCfgName, LPCTSTR PublicCfgName, LPCTSTR CfgMapName);
	BOOL	ShowDlg();
	CString ReadCfg(LPCTSTR Ident);
	void	LoadDefaultCfg();
	void	SaveCfg();
	void	WriteCfg(LPCTSTR Ident, LPCTSTR Value);
	void	LoadCfg();
	void	SetCfgMap(LPCTSTR CfgMapName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCfg)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCfg();

	// Generated message map functions
protected:
	afx_msg long OnSend(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CCfg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

