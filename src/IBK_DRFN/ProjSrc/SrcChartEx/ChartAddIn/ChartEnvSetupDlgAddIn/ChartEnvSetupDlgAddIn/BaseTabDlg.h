#if !defined(AFX_BASETABDLG_H__2FF0B001_E339_401A_8CE5_F416EEDB9784__INCLUDED_)
#define AFX_BASETABDLG_H__2FF0B001_E339_401A_8CE5_F416EEDB9784__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseTabDlg.h : header file
//

#include "BitmapItemComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CBaseTabDlg dialog


class CBaseTabDlg;


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 10 / 10
// Explain		: 각 Tab Dialog의 부모 Dialog
// Using way	: 
//-----------------------------------------------------------------------------
class CBaseTabDlg : public CDialog
{
	DECLARE_DYNAMIC( CBaseTabDlg)

// Construction
public:
	CBaseTabDlg( int IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBaseTabDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Attribute
public:
//	enum { INDICATOR_SETUP, BASE_ENV_SETUP, ANALYSIS_SETUP, STYLE_SETUP };


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseTabDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	//{{AFX_MSG(CBaseTabDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	enum DLG_TYPE
	{
		INDI_SET = 0,
		ENV_SET,
		ANALYSIS_SET,
		STYLE_SET,
		DLG_TYPE_COUNT
	};

public:
	virtual bool ChangeTabPage( CBaseTabDlg *pSourceDlgTabPage );
	virtual CBaseTabDlg::DLG_TYPE GetDialogType() const = 0;
	virtual bool GetCurrentPrice(int& nCurPrice) const;


	void Initial_LineStyleComboBox( CBitmapItemComboBox &LineStyle_Combo, int nCount, int nCurSel = 0, int nType = 0);//void Initial_LineStyleComboBox( CBitmapItemComboBox &LineStyle_Combo, int nCurSel = 0);//20131211 이문수 >>
	void Initial_ThicknessComboBox( CBitmapItemComboBox &LineStyle_Combo, int nCount, int nCurSel = 0);
	void AddLineType( CComboBox *pCombo, int nCount );

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASETABDLG_H__2FF0B001_E339_401A_8CE5_F416EEDB9784__INCLUDED_)
