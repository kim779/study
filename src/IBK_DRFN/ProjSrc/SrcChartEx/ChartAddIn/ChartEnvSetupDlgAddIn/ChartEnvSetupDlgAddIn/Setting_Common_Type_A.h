#if !defined(AFX_SETTING_COMMON_TYPE_A_H__23187959_E35E_495E_909A_2439751814F1__INCLUDED_)
#define AFX_SETTING_COMMON_TYPE_A_H__23187959_E35E_495E_909A_2439751814F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_Common_Type_A.h : header file
//


#include "BaseDialog.h"
#include "ColorButton.h"
#include "ExListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSetting_Common_Type_A dialog

class CTabMainChartEnvSetUp;
//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CSetting_Common_Type_A : public CBaseDialog
{
// Construction
public:
	CSetting_Common_Type_A( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, CWnd *pTopWnd = NULL );   // standard constructor
	virtual ~CSetting_Common_Type_A();
	
	virtual void Initial(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 >>
	virtual BOOL OnApply(IGraphInfo* p_pIGraphInfo);
	//virtual void OnApply(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 <<

	// ESC 동작 때문에 추가 

// Dialog Data
	//{{AFX_DATA(CSetting_Common_Type_A)
	enum { IDD = IDD_COMMON_TYPE_A };
	CComboBox	m_cmbCompColor;
	CButton	m_btnFillColor;
	CColorButton	m_btnDown1Color;
	CColorButton	m_btnUp1Color;
	CColorButton	m_btnUpColor;
	CColorButton	m_btnSteadyColor;
	CColorButton	m_btnDownColor;
	CNumSpinCtrl	m_Horizon_Spin;
	CButton	m_Signal_Bt;
	CBaseListBox	m_HorizonLine_ListBox;
	CNumEdit	m_Horizon_Edit;
	CColorButton	m_Color_Bt;
	CStatic	m_stcValue5;
	CStatic	m_stcValue4;
	CStatic	m_stcValue3;
	CStatic	m_stcValue2;
	CStatic	m_stcValue1;
	CNumSpinCtrl	m_spnValue5;
	CNumSpinCtrl	m_spnValue4;
	CNumSpinCtrl	m_spnValue3;
	CNumSpinCtrl	m_spnValue2;
	CNumSpinCtrl	m_spnValue1;
	CNumEdit	m_edtValue5;
	CNumEdit	m_edtValue4;
	CNumEdit	m_edtValue3;
	CNumEdit	m_edtValue2;
	CNumEdit	m_edtValue1;
	CStatic	m_stcDataTitle;
	CEdit	m_edtCompositionTitleData;
	CBitmapItemComboBox	m_cmbThickness;
	CBitmapItemComboBox	m_cmbLineType;
	CStatic	m_stcCompositionTitle;
	CComboBox	m_cmbData;
	CColorButton	m_btnComositionColor;
	CListCtrl	m_lstComposition;
	int		m_nCompositionLineType;
	CBitmapItemComboBox	m_cmbThickness2;
	CBitmapItemComboBox	m_cmbLineType2;
	CString	m_strHorizonValue;
	//}}AFX_DATA

// Attribute
private:
	int m_nSubGraphIndex;
	BOOL m_bUseComposition;
	BOOL m_bUseBaseLine;

	BOOL m_bInitComplite;

	bool m_bUsePacket;



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_Common_Type_A)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void UseValueGroup( BOOL bUseValue );
	void UseValue1( BOOL bUseValue1 );
	void UseValue2( BOOL bUseValue2 );
	void UseValue3( BOOL bUseValue3 );
	void UseValue4( BOOL bUseValue4 );
	void UseValue5( BOOL bUseValue5 );
	void UsePacketData( bool bUsePacket );

	void UseCompositionGroup( BOOL bUseComposition );
	void UseSubColorBtn( BOOL bUseSubColorBtn, CString szBtnText = "", BOOL bUseThirdBtn = TRUE );
	void UseFillColorBtn( BOOL bUseFillColorBtn );
	void UseBaseLineGroup( BOOL bUseBaseLine );
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	void UseCompColorCtrl(BOOL bUseCompColorCtrl);

	BOOL CheckSpecialCond( const char *p_szGraphName);

	void Init_Variable_Group_Title( int p_nCndCount);
	void Init_Composition_Group( void);
	void Init_Composition_Group_SubGraphItem( void);
	void Init_Composition_Group_SubGraphTitle( int p_nSubGraphIndex);
	void Init_Composition_Group_SubGraphLine( int p_nSubGraphIndex);
	void Init_Composition_Group_SubGraphColor( int p_nSubGraphIndex);
	bool Init_Composition_Group_SubGraphColor_LineType( int p_nSubGraphIndex);
	bool Init_Composition_Group_SubGraphColor_BoxType( int p_nSubGraphIndex);
	void SetBottonColor(CColorButton& colorButton, const COLORREF& color);
	void Init_BaseLine_Group_Title( void);
	void Initial_Signal(IGraphInfo* pIndcatorInfo);
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	void Init_BarGraphCompareColor();
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	void Init_Composition_Group_BaseLine( int nSubGraphIndex );

	void Initial_SubGraphData(const int nIndex);
	void OnApplySubGraphData( void );
	
	void OnApplyBaseLine();
	void OnApplySignal(IGraphInfo* pIndcatorInfo);
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	void OnApplyPaintStyle();
	
	void DeleteString_ListBox();
	double GetFirstValue(const CString &data);
	bool IsShowBaseLineList() const;

protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting_Common_Type_A)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnCompositionColor();
	afx_msg void OnItemchangedListComposition(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListComposition(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmbLinetype();
	afx_msg void OnSelchangeCmbThickness();
	afx_msg void OnInsert();
	afx_msg void OnDelete();
	afx_msg void OnAlldelete();
	afx_msg void OnColorButton();
	afx_msg void OnBtnUpColor();
	afx_msg void OnBtnDownColor();
	afx_msg void OnBtnSteadyColor();
	afx_msg void OnBtnUp1();
	afx_msg void OnBtnDown1();
	afx_msg void OnChkFillColor();
	afx_msg void OnSelchangeCmbThickness2();
	afx_msg void OnSelchangeCmbLinetype2();
	afx_msg void OnSelchangeHorizontalList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_COMMON_TYPE_A_H__23187959_E35E_495E_909A_2439751814F1__INCLUDED_)
