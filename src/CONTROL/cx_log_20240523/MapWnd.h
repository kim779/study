#pragma once
// MapWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window


#include "../../h/axisgenv.h"
#include "../../h/axisfire.h"

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(_param* pParam, CWnd* pWizard );

	CWnd*	m_pWizard;
	int	m_sParamKey;
	CString m_sParamName;

// Attributes
public:
	UINT AFXAPI HashDataAXIS(LPCSTR key);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMapWnd)
	afx_msg void axlog(LPCTSTR stag, LPCTSTR slog);
	afx_msg BSTR IDtoSha(LPCTSTR sId);
	afx_msg BSTR getroot();
	afx_msg void openPDF(LPCTSTR strPDF);
	afx_msg void Writelog(LPCTSTR spath, LPCTSTR slog);
	afx_msg BSTR HTSEncode(LPCTSTR sID, LPCTSTR sKey);
	afx_msg void ExcuteFile(LPCTSTR fullpath);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	void OpenChrome(BSTR strUrl);

	enum
	{
		dispidOpenChrome = 8L
	};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
