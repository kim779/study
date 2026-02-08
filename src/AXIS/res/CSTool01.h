#if !defined(AFX_CSTOOL01_H__861170F8_8220_4A3D_8EB0_9328F0053ABD__INCLUDED_)
#define AFX_CSTOOL01_H__861170F8_8220_4A3D_8EB0_9328F0053ABD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSTool01.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCSTool01 window

class CCSTool01 : public CDialogBar
{
// Construction
public:
	CCSTool01(CWnd* parent, CString home);

// Attributes
public:
	CWnd*		m_parent;
	CWnd*		m_tool;
	CString		m_home;
	int		m_height;
	HINSTANCE	m_instance;
// Operations
public:
	void	loadCStool();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSTool01)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCSTool01();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	// Generated message map functions
protected:
	//{{AFX_MSG(CCSTool01)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTOOL01_H__861170F8_8220_4A3D_8EB0_9328F0053ABD__INCLUDED_)
