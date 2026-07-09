// DownLoad.h: interface for the CDownLoad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOAD_H__A3F2DFD3_67A6_428D_A34C_62F7F7296222__INCLUDED_)
#define AFX_DOWNLOAD_H__A3F2DFD3_67A6_428D_A34C_62F7F7296222__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxmt.h>
class AFX_EXT_CLASS CDownLoad : public CWnd  
{
public:
	CDownLoad();
// Construction

// Attributes
public:
	struct	_rsmH*	m_SRH;
	struct	_rsmH*	m_RRH;
	class	CwSock*	m_wsock;

protected:
	CString		m_svrPath, m_localPath;
	CFile		m_file;
	bool		m_fDone, m_bResult;
	int		m_kind, m_nPercentage;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownLoad)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetProgress(int nPercentage, int nType = 1);
	void finish();
	void sendData();
	bool initialize();
	bool downLoad(CString svrPath, CString localPath, int kind = 0, bool bEnable = true);
	virtual ~CDownLoad();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDownLoad)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LONG OnReadData(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnOpenedSock(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_DOWNLOAD_H__A3F2DFD3_67A6_428D_A34C_62F7F7296222__INCLUDED_)
