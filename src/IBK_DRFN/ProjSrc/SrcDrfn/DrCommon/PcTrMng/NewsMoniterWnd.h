#if !defined(AFX_NEWSMONITERWND_H__100DFDE4_8042_41A8_8220_DDDFB1A0C29D__INCLUDED_)
#define AFX_NEWSMONITERWND_H__100DFDE4_8042_41A8_8220_DDDFB1A0C29D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewsMoniterWnd.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CNewsMoniterWnd window

class CNewsMoniterWnd : public CWnd
{
// Construction
public:
	CNewsMoniterWnd();

// Attributes
public:

// Operations
public:
	int SetSingSiganl(BOOL bAdvise, CWnd* pwndMain, CString strType, CString strKey);
	void GetSignalData(CString strType, CMapStringToPtr &mapSignalSet);
	void SetSignalData(CString strType, CMapStringToPtr *pmapSignalSet);
	void GetFilterData(long &stFilterInfo);
	void SetFilterData(long *pstFilterInfo);
	int  GetSiganlTable(CString strType, CStringArray &saSignalTable);
	HWND GetDummyHwnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewsMoniterWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNewsMoniterWnd();
	HINSTANCE			m_hDll;
	CString		m_szRootPath;

	BOOL OpenWindow();

	// Generated message map functions
protected:


	//{{AFX_MSG(CNewsMoniterWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSMONITERWND_H__100DFDE4_8042_41A8_8220_DDDFB1A0C29D__INCLUDED_)
