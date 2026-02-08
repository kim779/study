#if !defined(AFX_DLGENVSETUP2_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_)
#define AFX_DLGENVSETUP2_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPatternEnvSetup.h : header file
//

#include "resource.h"

#include "PatternDBItems.h"			// Pattern DB Items

// 2008.02.20 by LYH >> 7.0 -> 6.0 
//#include "../chart_common/ColorButton.h"
//#include "../chart_common/BitmapItemComboBox.h"
#include "control/ColorButton.h"
#include "control/BitmapItemComboBox.h"
// 2008.02.20 by LYH <<


/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup2 dialog

class CDlgEnvSetup2 : public CDialog
{
// Construction
public:
	CDlgEnvSetup2(CWnd* pParent = NULL);   // standard constructor


protected:
	CBrush			m_brBack;				// Dialog BG Color
	CBrush			m_brWhite;				// Brush Whiet
	
	CPatternEnvData	*m_pPatternEnvData;		// 패턴 환경설정 데이터


public:
	void			SetPatternEnvData(CPatternEnvData *pPatternEnvData);
	void			SetPatternEnvDataToControl();	// 패턴 환경설정을 컨트롤에 적용
	virtual void OnOK();


public:

// Dialog Data
	//{{AFX_DATA(CDlgEnvSetup2)
	enum { IDD = IDD_DLG_TABENVSETUP2 };
	CBitmapItemComboBox	m_cbLowThick;
	CColorButton	m_btnLowClr;
	CBitmapItemComboBox	m_cbHighThick;
	CColorButton	m_btnHighClr;
	CButton	m_chkHighPtRegistLine;
	CButton	m_chkLowPtSupportLine;
	CEdit	m_edtNextPtAllowable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEnvSetup2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEnvSetup2)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBtnHighClr();
	afx_msg void OnBtnLowClr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGENVSETUP2_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_)
