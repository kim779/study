#if !defined(AFX_IEWND_H__928980D6_5A89_4367_B8F3_24FC6B6AC566__INCLUDED_)
#define AFX_IEWND_H__928980D6_5A89_4367_B8F3_24FC6B6AC566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IEWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIEWnd window


class CIEWnd : public CWnd
{
// Construction
public:
	CIEWnd();
	CWnd* m_pParent{};
	CString m_strurl;
	

	void	SetParam(struct _param* pParam);
	BOOL	IsDev();
	class CAxBrowser* m_pHtml;
	CString Variant(int comm, CString data);
	struct _param m_Param;
	CFont* m_pNFont, * m_pBFont;
	CRect	m_Htmlrc;
	CString	m_sURL;
	CString m_sRoot{};
	bool	m_bScroll;
	int	m_scrGap;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIEWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIEWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIEWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CIEWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	void Navigate(BSTR sUrl);

	enum
	{
		dispidNavigate = 1L
	};
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEWND_H__928980D6_5A89_4367_B8F3_24FC6B6AC566__INCLUDED_)
