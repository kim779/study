#if !defined(AFX_SETTING_GLANCEBALANCE_H__544DF5B0_124F_4B43_8B88_6BFC3F9CE4F1__INCLUDED_)
#define AFX_SETTING_GLANCEBALANCE_H__544DF5B0_124F_4B43_8B88_6BFC3F9CE4F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_GlanceBalance.h : header file
//


#include "BaseDialog.h"
#include "ColorButton.h"
#include "ExListCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CSetting_GlanceBalance dialog

class CTabMainChartEnvSetUp;
class CSetting_GlanceBalance : public CBaseDialog
{
// Construction
public:
	CSetting_GlanceBalance( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, CWnd *pTopWnd = NULL);   // standard constructor
	virtual ~CSetting_GlanceBalance();

	virtual void Initial(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 >>
	virtual BOOL OnApply(IGraphInfo* p_pIGraphInfo);
	//virtual void OnApply(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 <<

// Dialog Data
	//{{AFX_DATA(CSetting_GlanceBalance)
	enum { IDD = IDD_GLANCEBALANCE };
	CStatic	m_stcValue5;
	CStatic	m_stcValue4;
	CStatic	m_stcValue3;
	CStatic	m_stcValue2;
	CStatic	m_stcValue1;
	CNumEdit	m_edtValue5;
	CNumEdit	m_edtValue4;
	CNumEdit	m_edtValue3;
	CNumEdit	m_edtValue2;
	CNumEdit	m_edtValue1;
	CNumSpinCtrl	m_spnValue5;
	CNumSpinCtrl	m_spnValue4;
	CNumSpinCtrl	m_spnValue3;
	CNumSpinCtrl	m_spnValue2;
	CNumSpinCtrl	m_spnValue1;
	CEdit	m_edtCompositionTitleData;
	CBitmapItemComboBox	m_cmbThickness;
	CBitmapItemComboBox	m_cmbLineType;
	CComboBox	m_cmbData;
	CColorButton	m_btnUp1Color;
	CColorButton	m_btnUpColor;
	CColorButton	m_btnSteadyColor;
	CColorButton	m_btnDown1Color;
	CColorButton	m_btnDownColor;
	CColorButton	m_btnCompositionColor;
	CStatic	m_stcDataTitle;
	CListCtrl	m_lstComposition;
	int		m_nFillType;
	int		m_nCompositionLineType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_GlanceBalance)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	
// Attribute
private:
	int m_nSubGraphIndex;

// Implementation
private:
	void UseCompositionGroup();
	void UseSubColorBtn( BOOL bUseSubColorBtn, CString szBtnText = "", BOOL bUseThirdBtn = TRUE );

	void Init_Composition_Group( IGraphInfo *p_pIGraphInfo);
	void Init_Composition_Group_SubGraphItem( IGraphInfo *p_pIGraphInfo);
	void Init_Composition_Group_SubGraphTitle( int p_nSubGraphIndex);
	void Init_Composition_Group_SubGraphLine( int p_nSubGraphIndex);
	void Init_Composition_Group_SubGraphColor( int p_nSubGraphIndex);
	bool Init_Composition_Group_SubGraphColor_LineType( int p_nSubGraphIndex);
	bool Init_Composition_Group_SubGraphColor_BoxType( int p_nSubGraphIndex);
	void SetBottonColor(CColorButton& colorButton, const COLORREF& color);

	void Initial_SubGraphData(const int nIndex);

	void OnApplySubGraphData( void );
	void OnApply_GraphStyle( int p_nSubGraphIndex);
	void SetGraphStyle( int p_nSubGraphIndex);
	
	void DeleteString_ListBox();
	void UseFillType( BOOL bUse );
	void SetGraphStyle_PreSpen( int p_nSubGraphIndex);

	// 일목균형표 설정 커서 적용 - ojtaso (20071109)
	void OnApply_SpanCursor();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting_GlanceBalance)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnUpColor();
	afx_msg void OnBtnDownColor();
	afx_msg void OnBtnSteadyColor();
	afx_msg void OnBtnUp1();
	afx_msg void OnBtnDown1();
	afx_msg void OnBtnCompositionColor();
	afx_msg void OnClickListComposition(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListComposition(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmbLinetype();
	afx_msg void OnSelchangeCmbThickness();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_GLANCEBALANCE_H__544DF5B0_124F_4B43_8B88_6BFC3F9CE4F1__INCLUDED_)
