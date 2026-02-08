#if !defined(AFX_SETTING_PRICEMA_CONDITION_H__5727B4A7_C38C_4381_ABAD_C81A8AEA2710__INCLUDED_)
#define AFX_SETTING_PRICEMA_CONDITION_H__5727B4A7_C38C_4381_ABAD_C81A8AEA2710__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_PriceMA_Condition.h : header file
//

#include "Include_AddIn.h"		// for IGraphInfo

#include "BaseDialog.h"
#include "ColorButton.h"
#include "NumMASpinCtrl.h"


const int PRICE_MA_CONDITION_COUNT = 20;


/////////////////////////////////////////////////////////////////////////////
// CSetting_PriceMA_Condition dialog

class CSetting_PriceMA_Condition : public CDialog
{
// Construction
public:
	CSetting_PriceMA_Condition( CWnd *pTopWnd = NULL);   // standard constructor
	~CSetting_PriceMA_Condition();

	virtual void Initial(IGraphInfo* p_pIGraphInfo);

// Dialog Data
	//{{AFX_DATA(CSetting_PriceMA_Condition)
	enum { IDD = IDD_PRICEMA_CONDITION };
	CBitmapItemComboBox	m_cmbThicknessGraph9;
	CBitmapItemComboBox	m_cmbThicknessGraph8;
	CBitmapItemComboBox	m_cmbThicknessGraph7;
	CBitmapItemComboBox	m_cmbThicknessGraph6;
	CBitmapItemComboBox	m_cmbThicknessGraph5;
	CBitmapItemComboBox	m_cmbThicknessGraph4;
	CBitmapItemComboBox	m_cmbThicknessGraph3;
	CBitmapItemComboBox	m_cmbThicknessGraph2;
	CBitmapItemComboBox	m_cmbThicknessGraph19;
	CBitmapItemComboBox	m_cmbThicknessGraph18;
	CBitmapItemComboBox	m_cmbThicknessGraph17;
	CBitmapItemComboBox	m_cmbThicknessGraph16;
	CBitmapItemComboBox	m_cmbThicknessGraph15;
	CBitmapItemComboBox	m_cmbThicknessGraph14;
	CBitmapItemComboBox	m_cmbThicknessGraph13;
	CBitmapItemComboBox	m_cmbThicknessGraph12;
	CBitmapItemComboBox	m_cmbThicknessGraph11;
	CBitmapItemComboBox	m_cmbThicknessGraph10;
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
	CNumSpinCtrl	m_spnGraph19;
	CNumSpinCtrl	m_spnGraph18;
	CNumSpinCtrl	m_spnGraph17;
	CNumSpinCtrl	m_spnGraph16;
	CNumSpinCtrl	m_spnGraph15;
	CNumSpinCtrl	m_spnGraph14;
	CNumSpinCtrl	m_spnGraph13;
	CNumSpinCtrl	m_spnGraph12;
	CNumSpinCtrl	m_spnGraph11;
	CNumSpinCtrl	m_spnGraph10;
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
	CBitmapItemComboBox	m_cmbDrawStyleGraph19;
	CBitmapItemComboBox	m_cmbDrawStyleGraph18;
	CBitmapItemComboBox	m_cmbDrawStyleGraph17;
	CBitmapItemComboBox	m_cmbDrawStyleGraph16;
	CBitmapItemComboBox	m_cmbDrawStyleGraph15;
	CBitmapItemComboBox	m_cmbDrawStyleGraph14;
	CBitmapItemComboBox	m_cmbDrawStyleGraph13;
	CBitmapItemComboBox	m_cmbDrawStyleGraph12;
	CBitmapItemComboBox	m_cmbDrawStyleGraph11;
	CBitmapItemComboBox	m_cmbDrawStyleGraph10;
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
	CNumEdit	m_edtGraph19;
	CNumEdit	m_edtGraph18;
	CNumEdit	m_edtGraph17;
	CNumEdit	m_edtGraph16;
	CNumEdit	m_edtGraph15;
	CNumEdit	m_edtGraph14;
	CNumEdit	m_edtGraph13;
	CNumEdit	m_edtGraph12;
	CNumEdit	m_edtGraph11;
	CNumEdit	m_edtGraph10;
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
	CColorButton	m_btnColor19;
	CColorButton	m_btnColor18;
	CColorButton	m_btnColor17;
	CColorButton	m_btnColor16;
	CColorButton	m_btnColor15;
	CColorButton	m_btnColor14;
	CColorButton	m_btnColor13;
	CColorButton	m_btnColor12;
	CColorButton	m_btnColor11;
	CColorButton	m_btnColor10;
	CColorButton	m_btnColor1;
	CColorButton	m_btnColor0;
	CComboBox	m_cmbPacket9;
	CComboBox	m_cmbPacket8;
	CComboBox	m_cmbPacket7;
	CComboBox	m_cmbPacket6;
	CComboBox	m_cmbPacket5;
	CComboBox	m_cmbPacket4;
	CComboBox	m_cmbPacket3;
	CComboBox	m_cmbPacket2;
	CComboBox	m_cmbPacket19;
	CComboBox	m_cmbPacket18;
	CComboBox	m_cmbPacket17;
	CComboBox	m_cmbPacket16;
	CComboBox	m_cmbPacket15;
	CComboBox	m_cmbPacket14;
	CComboBox	m_cmbPacket13;
	CComboBox	m_cmbPacket12;
	CComboBox	m_cmbPacket11;
	CComboBox	m_cmbPacket10;
	CComboBox	m_cmbPacket1;
	CComboBox	m_cmbPacket0;
	CComboBox	m_cmbCalcMethod9;
	CComboBox	m_cmbCalcMethod8;
	CComboBox	m_cmbCalcMethod7;
	CComboBox	m_cmbCalcMethod6;
	CComboBox	m_cmbCalcMethod5;
	CComboBox	m_cmbCalcMethod4;
	CComboBox	m_cmbCalcMethod3;
	CComboBox	m_cmbCalcMethod2;
	CComboBox	m_cmbCalcMethod19;
	CComboBox	m_cmbCalcMethod18;
	CComboBox	m_cmbCalcMethod17;
	CComboBox	m_cmbCalcMethod16;
	CComboBox	m_cmbCalcMethod15;
	CComboBox	m_cmbCalcMethod14;
	CComboBox	m_cmbCalcMethod13;
	CComboBox	m_cmbCalcMethod12;
	CComboBox	m_cmbCalcMethod11;
	CComboBox	m_cmbCalcMethod10;
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
	CButton	m_btnShowGraph19;
	CButton	m_btnShowGraph18;
	CButton	m_btnShowGraph17;
	CButton	m_btnShowGraph16;
	CButton	m_btnShowGraph15;
	CButton	m_btnShowGraph14;
	CButton	m_btnShowGraph13;
	CButton	m_btnShowGraph12;
	CButton	m_btnShowGraph11;
	CButton	m_btnShowGraph10;
	CButton	m_btnShowGraph1;
	CButton	m_btnShowGraph0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_PriceMA_Condition)
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

	CComboBox *GetPacketCombo( int nSubGraphIndex );
	CComboBox *GetCalcMethodCombo( int nSubGraphIndex );

	int GetSubGraphCount( void );
	void SetSubGraphCount( int nSubGraphCount );

	void ShowHideSubGraph( int nSubGraphIndex, int nCmdShow );

	void SetOriginalRect( CRect rcOriginRect );

	void WhenAllSubGraphisHide( const int p_nCheckID);

private:
	int m_nSubGraphCount;

	IGraphInfo* m_pIGraphInfo;

	// dialog size as you see in the resource view (original size)
	CRect	m_rcOriginalRect;
		
	// actual scroll position
	int		m_nScrollPos;

	// actual dialog height
	int		m_nCurHeight;


protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting_PriceMA_Condition)
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnColorButton0();
	afx_msg void OnColorButton1();
	afx_msg void OnColorButton10();
	afx_msg void OnColorButton11();
	afx_msg void OnColorButton12();
	afx_msg void OnColorButton13();
	afx_msg void OnColorButton14();
	afx_msg void OnColorButton15();
	afx_msg void OnColorButton16();
	afx_msg void OnColorButton17();
	afx_msg void OnColorButton18();
	afx_msg void OnColorButton19();
	afx_msg void OnColorButton2();
	afx_msg void OnColorButton3();
	afx_msg void OnColorButton4();
	afx_msg void OnColorButton5();
	afx_msg void OnColorButton6();
	afx_msg void OnColorButton7();
	afx_msg void OnColorButton8();
	afx_msg void OnColorButton9();
	afx_msg void OnSelchangeDrawstyle0();
	afx_msg void OnSelchangeDrawstyle1();
	afx_msg void OnSelchangeDrawstyle10();
	afx_msg void OnSelchangeDrawstyle11();
	afx_msg void OnSelchangeDrawstyle12();
	afx_msg void OnSelchangeDrawstyle13();
	afx_msg void OnSelchangeDrawstyle14();
	afx_msg void OnSelchangeDrawstyle15();
	afx_msg void OnSelchangeDrawstyle16();
	afx_msg void OnSelchangeDrawstyle17();
	afx_msg void OnSelchangeDrawstyle18();
	afx_msg void OnSelchangeDrawstyle19();
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
	afx_msg void OnSelchangeThickness10();
	afx_msg void OnSelchangeThickness11();
	afx_msg void OnSelchangeThickness12();
	afx_msg void OnSelchangeThickness13();
	afx_msg void OnSelchangeThickness14();
	afx_msg void OnSelchangeThickness15();
	afx_msg void OnSelchangeThickness16();
	afx_msg void OnSelchangeThickness17();
	afx_msg void OnSelchangeThickness18();
	afx_msg void OnSelchangeThickness19();
	afx_msg void OnSelchangeThickness2();
	afx_msg void OnSelchangeThickness3();
	afx_msg void OnSelchangeThickness4();
	afx_msg void OnSelchangeThickness5();
	afx_msg void OnSelchangeThickness6();
	afx_msg void OnSelchangeThickness7();
	afx_msg void OnSelchangeThickness8();
	afx_msg void OnSelchangeThickness9();
	afx_msg void OnCheckShowGraph0();
	afx_msg void OnCheckShowGraph1();
	afx_msg void OnCheckShowGraph10();
	afx_msg void OnCheckShowGraph11();
	afx_msg void OnCheckShowGraph12();
	afx_msg void OnCheckShowGraph13();
	afx_msg void OnCheckShowGraph14();
	afx_msg void OnCheckShowGraph15();
	afx_msg void OnCheckShowGraph16();
	afx_msg void OnCheckShowGraph17();
	afx_msg void OnCheckShowGraph18();
	afx_msg void OnCheckShowGraph19();
	afx_msg void OnCheckShowGraph2();
	afx_msg void OnCheckShowGraph3();
	afx_msg void OnCheckShowGraph4();
	afx_msg void OnCheckShowGraph5();
	afx_msg void OnCheckShowGraph6();
	afx_msg void OnCheckShowGraph7();
	afx_msg void OnCheckShowGraph8();
	afx_msg void OnCheckShowGraph9();
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

#endif // !defined(AFX_SETTING_PRICEMA_CONDITION_H__5727B4A7_C38C_4381_ABAD_C81A8AEA2710__INCLUDED_)
