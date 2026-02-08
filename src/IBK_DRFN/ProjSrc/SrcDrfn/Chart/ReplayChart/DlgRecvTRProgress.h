#if !defined(AFX_DLGRECVTRPROGRESS_H__45FDEC1A_A5E6_4EB9_ADBC_5458E5924B92__INCLUDED_)
#define AFX_DLGRECVTRPROGRESS_H__45FDEC1A_A5E6_4EB9_ADBC_5458E5924B92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRecvTRProgress.h : header file
//

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgRecvTRProgress dialog

class CDlgRecvTRProgress : public CDialog
{
// Construction
public:
	CDlgRecvTRProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRecvTRProgress)
	enum { IDD = IDD_DLG_TR_RECV };
	CProgressCtrl	m_progRecvTR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRecvTRProgress)
	public:
	virtual BOOL Create(CWnd *pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nTRTotal;
	int m_nTRCur;
	
	void SetTRTotalCount( int nTRTotal );
	void AddCount(int nGap=1);
	




protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRecvTRProgress)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECVTRPROGRESS_H__45FDEC1A_A5E6_4EB9_ADBC_5458E5924B92__INCLUDED_)
