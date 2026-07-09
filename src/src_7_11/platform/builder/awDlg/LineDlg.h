#if !defined(AFX_LINEDLG_H__154AA747_316B_4F28_BBEF_8A41B15C5CBB__INCLUDED_)
#define AFX_LINEDLG_H__154AA747_316B_4F28_BBEF_8A41B15C5CBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineDlg.h : header file
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CLineDlg dialog

class AFX_EXT_CLASS CLineDlg : public CDialog
{
// Construction
public:
	CLineDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineDlg)
	enum { IDD = IDD_LINEDLG };
	int		m_lineNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLineDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEDLG_H__154AA747_316B_4F28_BBEF_8A41B15C5CBB__INCLUDED_)
