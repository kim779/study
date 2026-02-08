#if !defined(AFX_CHROMEWND_H__E2F3A978_89C7_4C45_94E4_76468AD67F24__INCLUDED_)
#define AFX_CHROMEWND_H__E2F3A978_89C7_4C45_94E4_76468AD67F24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChromeWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChromeWnd window

class CChromeWnd : public CWnd
{
// Construction
public:
	CChromeWnd();
	CWnd* m_pParent{};
	CParam m_Param;
	CString m_strurl;
	CString _sURL;
	bool	_bScroll{};

	void	SetParam(struct _param* pParam);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChromeWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChromeWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChromeWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CChromeWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHROMEWND_H__E2F3A978_89C7_4C45_94E4_76468AD67F24__INCLUDED_)
