#if !defined(AFX_VBGUIDE_H__A5CE9F79_B17E_4414_8B92_67D2DB7BAD8B__INCLUDED_)
#define AFX_VBGUIDE_H__A5CE9F79_B17E_4414_8B92_67D2DB7BAD8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VbGuide.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CVbGuide thread

class CVbGuide : public CWinThread
{
	DECLARE_DYNCREATE(CVbGuide)

public:
	CVbGuide(CString title, CString topic);

protected:
	CVbGuide();           // protected constructor used by dynamic creation

// Attributes
public:

private:
	CString		m_title;
	CString		m_topic;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVbGuide)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CVbGuide();

	// Generated message map functions
	//{{AFX_MSG(CVbGuide)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VBGUIDE_H__A5CE9F79_B17E_4414_8B92_67D2DB7BAD8B__INCLUDED_)
