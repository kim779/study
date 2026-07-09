#if !defined(AFX_MAPSHEET_H__8A8F6600_5E82_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_MAPSHEET_H__8A8F6600_5E82_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mapSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapSheet

class AFX_EXT_CLASS CMapSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMapSheet)

// Construction
public:
	CMapSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMapSheet(LPCTSTR pszCaption, struct _mapH* mapH, CString dir,
				CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

protected:
	class	CMapProp*	m_prop;
	class	CMapOption*	m_option;
	class	CMapScript*	m_script;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapSheet)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPSHEET_H__8A8F6600_5E82_11D4_A024_00001CD7F9BE__INCLUDED_)
