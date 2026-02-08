#if !defined(AFX_SETTING_COMMON_TYPE_B_H__81BD06C3_5D34_435C_A70F_782E87705B98__INCLUDED_)
#define AFX_SETTING_COMMON_TYPE_B_H__81BD06C3_5D34_435C_A70F_782E87705B98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_Common_Type_B.h : header file
//

#include "BaseDialog.h"
#include "ColorButton.h"
#include "ExListCtrl.h"


class CTabMainChartEnvSetUp;


/////////////////////////////////////////////////////////////////////////////
// CSetting_Common_Type_B dialog

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 5 / 2
// Comments		: BinaryWave 지표의 변수의 개수가 7개라서 다른 지표와 설정창을 
//				: 공통으로 사용하기 어려워서 별도로 분리함.
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CSetting_Common_Type_B : public CBaseDialog
{
// Construction
public:
	CSetting_Common_Type_B( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, CWnd *pTopWnd = NULL );   // standard constructor
	virtual ~CSetting_Common_Type_B();
	
	virtual void Initial(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 >>
	virtual BOOL OnApply(IGraphInfo* p_pIGraphInfo);
	//virtual void OnApply(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 <<

// Dialog Data
	//{{AFX_DATA(CSetting_Common_Type_B)
	enum { IDD = IDD_COMMON_TYPE_B };
	CNumSpinCtrl	m_Horizon_Spin;
	CButton	m_Signal_Bt;
	CBaseListBox	m_HorizonLine_ListBox;
	CNumEdit	m_Horizon_Edit;
	CColorButton	m_Color_Bt;
	CStatic	m_stcValue7;
	CStatic	m_stcValue6;
	CStatic	m_stcValue5;
	CStatic	m_stcValue4;
	CStatic	m_stcValue3;
	CStatic	m_stcValue2;
	CStatic	m_stcValue1;
	CNumSpinCtrl	m_spnValue7;
	CNumSpinCtrl	m_spnValue6;
	CNumSpinCtrl	m_spnValue5;
	CNumSpinCtrl	m_spnValue4;
	CNumSpinCtrl	m_spnValue3;
	CNumSpinCtrl	m_spnValue2;
	CNumSpinCtrl	m_spnValue1;
	CNumEdit	m_edtValue7;
	CNumEdit	m_edtValue6;
	CNumEdit	m_edtValue5;
	CNumEdit	m_edtValue4;
	CNumEdit	m_edtValue3;
	CNumEdit	m_edtValue2;
	CNumEdit	m_edtValue1;
	CEdit	m_edtCompositionTitleData;
	CBitmapItemComboBox	m_cmbThickness;
	CBitmapItemComboBox	m_cmbLineType;
	CStatic	m_stcCompositionTitle;
	CColorButton	m_btnComositionColor;
	CListCtrl	m_lstComposition;
	int		m_nCompositionLineType;
	double	m_dHorizonValue;
		// NOTE: the ClassWizard will add data members here
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
	//{{AFX_VIRTUAL(CSetting_Common_Type_B)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void UseCompositionGroup( BOOL bUseComposition );
	void UseSubColorBtn( BOOL bUseSubColorBtn, CString szBtnText = "", BOOL bUseThirdBtn = TRUE );
	void UseFillColorBtn( BOOL bUseFillColorBtn );
	void UseBaseLineGroup( BOOL bUseBaseLine );

	void Init_Composition_Group_SubGraphItem( void);
	void Init_Composition_Group_SubGraphTitle( int p_nSubGraphIndex);
	void Init_Composition_Group_SubGraphLine( int p_nSubGraphIndex);
	void Init_Composition_Group_SubGraphColor( int p_nSubGraphIndex);
	bool Init_Composition_Group_SubGraphColor_LineType( int p_nSubGraphIndex);
	bool Init_Composition_Group_SubGraphColor_BoxType( int p_nSubGraphIndex);
	void SetButtonColor(CColorButton& colorButton, const COLORREF& color);
	void Init_BaseLine_Group_Title( void);
	void Initial_Signal(IGraphInfo* pIndcatorInfo);

	void Initial_SubGraphData(const int nIndex);
	void OnApplySubGraphData( void );
	
	void OnApplyBaseLine();
	void OnApplySignal(IGraphInfo* pIndcatorInfo);
	
	void DeleteString_ListBox();
	bool IsShowBaseLineList() const;


protected:
	// Generated message map functions
	//{{AFX_MSG(CSetting_Common_Type_B)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_COMMON_TYPE_B_H__81BD06C3_5D34_435C_A70F_782E87705B98__INCLUDED_)
