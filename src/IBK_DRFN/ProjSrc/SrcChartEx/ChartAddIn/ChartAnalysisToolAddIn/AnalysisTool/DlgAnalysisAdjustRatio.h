#if !defined(AFX_DLGANALYSISADJUSTRATIO_H__35795418_41E1_40F8_BA8B_01026D21BE27__INCLUDED_)
#define AFX_DLGANALYSISADJUSTRATIO_H__35795418_41E1_40F8_BA8B_01026D21BE27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAnalysisAdjustRatio.h : header file
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysisAdjustRatio dialog

class CDlgAnalysisAdjustRatio : public CDialog
{
// Construction
public:
	CDlgAnalysisAdjustRatio(HWND hOcxWnd = NULL, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAnalysisAdjustRatio)
	enum { IDD = IDD_ANALYSIS_ADJUST_RATIO };
	CListCtrl m_ctrlList;
	//}}AFX_DATA

	void SetListItem(int nPos,char *chData);
	void SetValue(int nIndex,double dValue);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAnalysisAdjustRatio)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	double m_dValue1;
	double m_dValue2;
	double m_dValue3;

protected:
	HWND m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_hOcxWnd = p_hOcxWnd;	}

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAnalysisAdjustRatio)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGANALYSISADJUSTRATIO_H__35795418_41E1_40F8_BA8B_01026D21BE27__INCLUDED_)
