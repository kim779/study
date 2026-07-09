#if !defined(AFX_MAPSCRIPT_H__FACCAE22_5674_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_MAPSCRIPT_H__FACCAE22_5674_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mapScript.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMapScript dialog

class CMapScript : public CPropertyPage
{
	DECLARE_DYNCREATE(CMapScript)

// Construction
public:
	CMapScript()	{}
	CMapScript(struct _mapH* mapH, CString curdir);
	~CMapScript();

// Dialog Data
	//{{AFX_DATA(CMapScript)
	enum { IDD = IDD_GSCRIPT };
	CComboBox	m_event;
	CString		m_script;
	//}}AFX_DATA

protected:
	struct	_mapH*	m_mapH;
	int	m_prev;
	CString	m_curdir;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMapScript)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	setScript(char* dir);
	void	insertToEdit(char* buf);
	bool	saveScript();

	// Generated message map functions
	//{{AFX_MSG(CMapScript)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeEvent();
	afx_msg void OnChangeScript();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPSCRIPT_H__FACCAE22_5674_11D4_A024_00001CD7F9BE__INCLUDED_)
