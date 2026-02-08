#if !defined(AFX_MAPNAMEDLG_H__37896563_4050_494F_BF18_5D9F35869602__INCLUDED_)
#define AFX_MAPNAMEDLG_H__37896563_4050_494F_BF18_5D9F35869602__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapNameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapNameDlg dialog

class CMapNameDlg : public CDialog
{
// Construction
public:
	CMapNameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMapNameDlg)
	enum { IDD = IDD_DIALOG_ID };
	CString	m_stCtrlCode;
	CString	m_strMapName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapNameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMapNameDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPNAMEDLG_H__37896563_4050_494F_BF18_5D9F35869602__INCLUDED_)
