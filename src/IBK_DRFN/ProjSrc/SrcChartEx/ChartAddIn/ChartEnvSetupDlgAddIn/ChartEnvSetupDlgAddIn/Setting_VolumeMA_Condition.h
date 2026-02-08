#if !defined(AFX_SETTING_VOLUMEMA_CONDITION_H__13C9F161_9B1D_4AAA_AA64_CBBAFF2330F1__INCLUDED_)
#define AFX_SETTING_VOLUMEMA_CONDITION_H__13C9F161_9B1D_4AAA_AA64_CBBAFF2330F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_VolumeMA_Condition.h : header file
//

#include "Include_AddIn.h"		// for IGraphInfo

#include "BaseDialog.h"
#include "ColorButton.h"
#include "NumMASpinCtrl.h"


const int VOLUME_MA_CONDITION_COUNT = 10;


/////////////////////////////////////////////////////////////////////////////
// CSetting_VolumeMA_Condition dialog

class CSetting_VolumeMA_Condition : public CDialog
{
// Construction
public:
	CSetting_VolumeMA_Condition( CWnd* pParent = NULL);   // standard constructor
	~CSetting_VolumeMA_Condition();
	
	virtual void Initial(IGraphInfo* p_pIGraphInfo);

// Dialog Data
	//{{AFX_DATA(CSetting_VolumeMA_Condition)
	enum { IDD = IDD_VOLUMEMA_CONDITION };
	CBitmapItemComboBox	m_cmbThicknessGraph9;
	CBitmapItemComboBox	m_cmbThicknessGraph8;
	CBitmapItemComboBox	m_cmbThicknessGraph7;
	CBitmapItemComboBox	m_cmbThicknessGraph6;
	CBitmapItemComboBox	m_cmbThicknessGraph5;
	CBitmapItemComboBox	m_cmbThicknessGraph4;
	CBitmapItemComboBox	m_cmbThicknessGraph3;
	CBitmapItemComboBox	m_cmbThicknessGraph2;
	CBitmapItemComboBox	m_cmbThicknessGraph1;
	CBitmapItemComboBox	m_cmbThicknessGraph0;
	CNumSpinCtrl	m_spnGraph9;
	CNumSpinCtrl	m_spnGraph8;
	CNumSpinCtrl	m_spnGraph7;
	CNumSpinCtrl	m_spnGraph6;
	CNumSpinCtrl	m_spnGraph5;
	CNumSpinCtrl	m_spnGraph4;
	CNumSpinCtrl	m_spnGraph3;
	CNumSpinCtrl	m_spnGraph2;
	CNumSpinCtrl	m_spnGraph1;
	CNumSpinCtrl	m_spnGraph0;
	CBitmapItemComboBox	m_cmbDrawStyleGraph9;
	CBitmapItemComboBox	m_cmbDrawStyleGraph8;
	CBitmapItemComboBox	m_cmbDrawStyleGraph7;
	CBitmapItemComboBox	m_cmbDrawStyleGraph6;
	CBitmapItemComboBox	m_cmbDrawStyleGraph5;
	CBitmapItemComboBox	m_cmbDrawStyleGraph4;
	CBitmapItemComboBox	m_cmbDrawStyleGraph3;
	CBitmapItemComboBox	m_cmbDrawStyleGraph2;
	CBitmapItemComboBox	m_cmbDrawStyleGraph1;
	CBitmapItemComboBox	m_cmbDrawStyleGraph0;
	CNumEdit	m_edtGraph9;
	CNumEdit	m_edtGraph8;
	CNumEdit	m_edtGraph7;
	CNumEdit	m_edtGraph6;
	CNumEdit	m_edtGraph5;
	CNumEdit	m_edtGraph4;
	CNumEdit	m_edtGraph3;
	CNumEdit	m_edtGraph2;
	CNumEdit	m_edtGraph1;
	CNumEdit	m_edtGraph0;
	CColorButton	m_btnColor9;
	CColorButton	m_btnColor8;
	CColorButton	m_btnColor7;
	CColorButton	m_btnColor6;
	CColorButton	m_btnColor5;
	CColorButton	m_btnColor4;
	CColorButton	m_btnColor3;
	CColorButton	m_btnColor2;
	CColorButton	m_btnColor1;
	CColorButton	m_btnColor0;
	CComboBox	m_cmbCalcMethod9;
	CComboBox	m_cmbCalcMethod8;
	CComboBox	m_cmbCalcMethod7;
	CComboBox	m_cmbCalcMethod6;
	CComboBox	m_cmbCalcMethod5;
	CComboBox	m_cmbCalcMethod4;
	CComboBox	m_cmbCalcMethod3;
	CComboBox	m_cmbCalcMethod2;
	CComboBox	m_cmbCalcMethod1;
	CComboBox	m_cmbCalcMethod0;
	CButton	m_btnShowGraph9;
	CButton	m_btnShowGraph8;
	CButton	m_btnShowGraph7;
	CButton	m_btnShowGraph6;
	CButton	m_btnShowGraph5;
	CButton	m_btnShowGraph4;
	CButton	m_btnShowGraph3;
	CButton	m_btnShowGraph2;
	CButton	m_btnShowGraph1;
	CButton	m_btnShowGraph0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_VolumeMA_Condition)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CButton* GetCheckButton(int nSubGraphIndex);
	CNumEdit* GetEditCtrl(int nSubGraphIndex);
	CNumSpinCtrl* GetSpinCtrl(int nSubGraphIndex);

	CColorButton* GetColorButton( int nSubGraphIndex );

	void SetDrawStyle( int nSubGraphIndex, int nCmbIndex );
	void SetLineThickness( int nSubGraphIndex, int nCmbIndex );			// ¼±±½±â ¼¼ÆÃ
	void SetLineColor( int nSubGraphIndex, int nColorIndex );
	CBitmapItemComboBox* GetDrawStyleCombo( int nSubGraphIndex );
	CBitmapItemComboBox* GetThicknessCombo( int nSubGraphIndex );

	CComboBox *GetCalcMethodCombo( int nSubGraphIndex );

	int GetSubGraphCount( void );
	void SetSubGraphCount( int nSubGraphCount );

	void ShowHideSubGraph( int nSubGraphIndex, int nCmdShow );

	void WhenAllSubGraphisHide( const int p_nCheckID);

private:
	int m_nSubGraphCount;

	IGraphInfo* m_pIGraphInfo;
	

protected:
	
	// Generated message map functions
	//{{AFX_MSG(CSetting_VolumeMA_Condition)
	virtual BOOL OnInitDialog();
	afx_msg void OnColorButton0();
	afx_msg void OnColorButton1();
	afx_msg void OnColorButton2();
	afx_msg void OnColorButton3();
	afx_msg void OnColorButton4();
	afx_msg void OnColorButton5();
	afx_msg void OnColorButton6();
	afx_msg void OnColorButton7();
	afx_msg void OnColorButton8();
	afx_msg void OnColorButton9();
	afx_msg void OnCheckShowGraph0();
	afx_msg void OnCheckShowGraph1();
	afx_msg void OnCheckShowGraph2();
	afx_msg void OnCheckShowGraph3();
	afx_msg void OnCheckShowGraph4();
	afx_msg void OnCheckShowGraph5();
	afx_msg void OnCheckShowGraph6();
	afx_msg void OnCheckShowGraph7();
	afx_msg void OnCheckShowGraph8();
	afx_msg void OnCheckShowGraph9();
	afx_msg void OnSelchangeDrawstyle0();
	afx_msg void OnSelchangeDrawstyle1();
	afx_msg void OnSelchangeDrawstyle2();
	afx_msg void OnSelchangeDrawstyle3();
	afx_msg void OnSelchangeDrawstyle4();
	afx_msg void OnSelchangeDrawstyle5();
	afx_msg void OnSelchangeDrawstyle6();
	afx_msg void OnSelchangeDrawstyle7();
	afx_msg void OnSelchangeDrawstyle8();
	afx_msg void OnSelchangeDrawstyle9();
	afx_msg void OnSelchangeThickness0();
	afx_msg void OnSelchangeThickness1();
	afx_msg void OnSelchangeThickness2();
	afx_msg void OnSelchangeThickness3();
	afx_msg void OnSelchangeThickness4();
	afx_msg void OnSelchangeThickness5();
	afx_msg void OnSelchangeThickness6();
	afx_msg void OnSelchangeThickness7();
	afx_msg void OnSelchangeThickness8();
	afx_msg void OnSelchangeThickness9();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	CBaseDialog *m_pParentDlg;

// 2008.01.25 by LYH >>
protected:
	HWND m_hOcxWnd;
	CString m_strMsgLineThick;
	CString m_strMsgLineShape;
	CString m_strMsgExistMin;
// 2008.01.25 by LYH <<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_VOLUMEMA_CONDITION_H__13C9F161_9B1D_4AAA_AA64_CBBAFF2330F1__INCLUDED_)
