#if !defined(AFX_PARENTMULTICHARTDLG_H__ED24E70C_AE5E_49D9_BAA0_7F8B9806FC7E__INCLUDED_)
#define AFX_PARENTMULTICHARTDLG_H__ED24E70C_AE5E_49D9_BAA0_7F8B9806FC7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParentMultiChartDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParentMultiChartDlg dialog
//#ifndef IDD_DLGMULTICHART
//#define IDD_DLGMULTICHART               6030
//#endif

class CParentChartDlg : public CDialog
{
// Construction
public:
	CParentChartDlg(UINT nID, CWnd* pParent = NULL);   // standard constructor

	virtual void    SetRealInfo		(
										void* pDrds,     // Drds포인터인데 NULL로 보냄.
										HWND hWnd,   // Chart OCX의 윈도우 핸들
										UINT id, 	// CPacket* 
										LPSTR DataName,	//  ex: "S31"
										LPSTR FieldName, 	//  ex: "time"
										LPSTR KeyName) = 0;	//  ex: "000660"


	virtual void    UnSetRealInfo	(
										void* pDrds,     // Drds포인터인데 NULL로 보냄.
										HWND hWnd,   // Chart OCX의 윈도우 핸들
										UINT id, 	// CPacket* 
										LPSTR DataName,	//  ex: "S31"
										LPSTR FieldName, 	//  ex: "time"
										LPSTR KeyName) = 0;	//  ex: "000660"

	virtual void	OnChartMouseMove ( UINT nFlags, CPoint pt) = 0;
/*
// Dialog Data
	//{{AFX_DATA(CParentMultiChartDlg)
	enum { IDD = IDD_DLGMULTICHART };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParentMultiChartDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParentMultiChartDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()*/
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARENTMULTICHARTDLG_H__ED24E70C_AE5E_49D9_BAA0_7F8B9806FC7E__INCLUDED_)
