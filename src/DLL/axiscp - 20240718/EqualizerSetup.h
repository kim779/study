#if !defined(AFX_EQUALIZERSETUP_H__8BEDF3D7_4744_4CAE_9F6F_7F1734A69F8A__INCLUDED_)
#define AFX_EQUALIZERSETUP_H__8BEDF3D7_4744_4CAE_9F6F_7F1734A69F8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SetupDialog.h"

#define PROP_SECTION		("EQUALIZER")

class CEqualizerSetup : public CSetupDialog
{
// Construction
public:
	CEqualizerSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEqualizerSetup)
	enum { IDD = IDD_EQUALIZER };
	CButton	m_chk_mado;
	CButton	m_chk_masu;
	CComboBox	m_cb_mado;
	CComboBox	m_cb_masu;
	CSliderCtrl	m_slider5;
	CSliderCtrl	m_slider4;
	CSliderCtrl	m_slider3;
	CSliderCtrl	m_slider2;
	CSliderCtrl	m_slider1;
	CStatic	m_mspic;
	CStatic	m_mdpic;
	CString	m_level1;
	CString	m_level2;
	CString	m_level3;
	CString	m_level4;
	CString	m_level5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEqualizerSetup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEqualizerSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnKillfocusEdit2();
	afx_msg void OnKillfocusEdit3();
	afx_msg void OnKillfocusEdit4();
	afx_msg void OnKillfocusEdit5();
	afx_msg void OnChkMado();
	afx_msg void OnChkMasu();
	afx_msg void OnSetfocusEdit1();
	afx_msg void OnSetfocusEdit2();
	afx_msg void OnSetfocusEdit3();
	afx_msg void OnSetfocusEdit4();
	afx_msg void OnSetfocusEdit5();
	afx_msg void OnBtnFMado();
	afx_msg void OnBtnFMasu();
	afx_msg void OnBtnLMado();
	afx_msg void OnBtnLMasu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CBitmap m_mdb, m_msb;
	int m_eq_type;
	int m_eq_vol[5];
	CString *m_eq_lev[5], m_def_wav;
	CSliderCtrl *m_sliders[5];
	COLORREF m_mdcol[5];
	COLORREF m_mscol[5];
	void DrawEqaulizer();    //CPaintDC *pdc
	void DrawEqaulizer(CPaintDC* pdc);
	virtual void DefaultSetup();
	virtual void ApplySetup();
	void LoadSetup();
	void CheckEdit(int idx, CEdit *pEdit, CEdit *pPrevEdit, CEdit *pNextEdit);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQUALIZERSETUP_H__8BEDF3D7_4744_4CAE_9F6F_7F1734A69F8A__INCLUDED_)
