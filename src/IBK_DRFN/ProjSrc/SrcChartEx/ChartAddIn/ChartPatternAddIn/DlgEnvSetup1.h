#if !defined(AFX_DLGENVSETUP1_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_)
#define AFX_DLGENVSETUP1_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPatternEnvSetup.h : header file
//

#include "resource.h"

#include "PatternDBItems.h"			// Pattern DB Items


/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup1 dialog

class CDlgEnvSetup1 : public CDialog
{
// Construction
public:
	CDlgEnvSetup1(CWnd* pParent = NULL);   // standard constructor


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
	//{{AFX_DATA(CDlgEnvSetup1)
	enum { IDD = IDD_DLG_TABENVSETUP1 };
	//CButton	m_chkNotDrawAll;
	//CEdit	m_edtNextPtAllowable;
	CEdit	m_edtHLPtAllowable;
	//CButton	m_chkMMa;
	//CButton	m_chkLowPtSupportLine;
	//CButton	m_chkLowPtHLine;
	//CButton	m_chkHighPtRegistLine;
	//CButton	m_chkHighPtHLine;
	//CButton	m_chkHighLowPtLine;
	int		m_radiovalMMA;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEnvSetup1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEnvSetup1)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioMma();
	afx_msg void OnRadioCandle();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGENVSETUP1_H__BE7D29AB_3E8D_444A_96A9_DD5033D1E82A__INCLUDED_)
