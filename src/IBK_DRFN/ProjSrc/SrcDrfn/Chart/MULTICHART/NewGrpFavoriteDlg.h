#if !defined(AFX_NEWGRPFAVORITEDLG_H__4DA94F9A_5478_4329_9E0C_C33578DFF916__INCLUDED_)
#define AFX_NEWGRPFAVORITEDLG_H__4DA94F9A_5478_4329_9E0C_C33578DFF916__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewGrpFavoriteDlg.h : header file
//
#include "RscDlg.h"
#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CNewGrpFavoriteDlg dialog
class CNewGrpFavoriteDlg : public CRscDlg
{
// Construction
public:
	CNewGrpFavoriteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewGrpFavoriteDlg)
	enum { IDD = IDD_DLGNEWGRPFAVORITE };
	CString	m_strNewName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewGrpFavoriteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewGrpFavoriteDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWGRPFAVORITEDLG_H__4DA94F9A_5478_4329_9E0C_C33578DFF916__INCLUDED_)
