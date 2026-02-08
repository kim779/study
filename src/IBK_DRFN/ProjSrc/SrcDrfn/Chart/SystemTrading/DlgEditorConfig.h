#if !defined(AFX_DLGEDITORCONFIG_H__B23A8DDC_BC08_4F9A_A3FD_1838BD64ADC0__INCLUDED_)
#define AFX_DLGEDITORCONFIG_H__B23A8DDC_BC08_4F9A_A3FD_1838BD64ADC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEditorConfig.h : header file
//

//#include "FODropColorButton.h"
#include "FontCombo.h"
//7#include "TipWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEditorConfig dialog

class CDlgEditorConfig : public CDialog
{
// Construction
public:
	CDlgEditorConfig(CFont* pFont, LPCSTR lpszSection, UINT nScriptType, CWnd* pParent = NULL);   // standard constructor
	void	CreateFont();
	void	LoadData();
	void	SetDefaultData();
	void	SetDefaultData_YES();
	void	SaveData();

	CFont*		m_pFontView;
	COLORREF	m_clrUserFunction;
	COLORREF	m_clrSelText;
	COLORREF	m_clrSelBkGrnd;
	COLORREF	m_clrReservedText;
	COLORREF	m_clrOperator;
	COLORREF	m_clrNumber;
	COLORREF	m_clrNormalText;
	COLORREF	m_clrNoCertFunc;
	COLORREF	m_clrComment;
	COLORREF	m_clrBackGround;

// Dialog Data
	//{{AFX_DATA(CDlgEditorConfig)
	enum { IDD = IDD_DLG_CONFIGEDITOR };
	CXTColorPicker	m_btnUserFunction;
	CXTColorPicker	m_btnSelText;
	CXTColorPicker	m_btnSelBackGrnd;
	CXTColorPicker	m_btnReserved;
	CXTColorPicker	m_btnOperator;
	CXTColorPicker	m_btnNumber;
	CXTColorPicker	m_btnNormalText;
	CXTColorPicker	m_btnNoCertFunc;
	CXTColorPicker	m_btnComment;
	CXTColorPicker	m_btnBackGround;
	CSpinButtonCtrl	m_spinHeight;
	CFontCombo		m_cmbFont;
	UINT	m_nTextHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEditorConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont*		m_pFont;
	char		m_szSection[20];
	char		m_szSavedFont[LF_FULLFACESIZE];
	UINT		m_nScriptType;

	// Generated message map functions
	//{{AFX_MSG(CDlgEditorConfig)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCmbFont();
	afx_msg void OnDeltaposSpinHeight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnReset();
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg void OnSelColorButtonNormalText();
	afx_msg void OnSelColorButtonBackground();
	afx_msg void OnSelColorButtonSelText();
	afx_msg void OnSelColorButtonSelBkGrnd();
	afx_msg void OnSelColorButtonUserFunction();
	afx_msg void OnSelColorButtonNoCertFunc();
	afx_msg void OnSelColorButtonComment();
	afx_msg void OnSelColorButtonNumber();
	afx_msg void OnSelColorButtonReserved();
	afx_msg void OnSelColorButtonOperator();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEDITORCONFIG_H__B23A8DDC_BC08_4F9A_A3FD_1838BD64ADC0__INCLUDED_)
