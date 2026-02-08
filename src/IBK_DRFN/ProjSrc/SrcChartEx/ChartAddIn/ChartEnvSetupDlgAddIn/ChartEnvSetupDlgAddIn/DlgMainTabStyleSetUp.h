#if !defined(AFX_DLGMAINTABSTYLESETUP_H__3A5A9A29_4365_4A77_BA5F_525009E1367A__INCLUDED_)
#define AFX_DLGMAINTABSTYLESETUP_H__3A5A9A29_4365_4A77_BA5F_525009E1367A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMainTabStyleSetUp.h : header file
//

#include <AFXTEMPL.h>

#include "Include_AddIn.h"			// for IChartOCX
#include "BaseTabDlg.h"
#include "ColorButton.h"
#include "EnvironmentData.h"
#include "StyleDataManager.h"		// for CStyleDataManager

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabStyleSetUp dialog

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 10
// Comments		: 
// Using way	: 
// See			: class CDialog
//-----------------------------------------------------------------------------
class CDlgMainTabStyleSetUp : public CBaseTabDlg
{
	DECLARE_DYNAMIC( CDlgMainTabStyleSetUp)

// Construction
public:
	CDlgMainTabStyleSetUp( IChartOCX *p_pIChartOCX, CWnd *pParent = NULL );   // standard constructor
	~CDlgMainTabStyleSetUp();

// Dialog Data
	//{{AFX_DATA(CDlgMainTabStyleSetUp)
	enum { IDD = IDD_MAINTAB_PAGE_STYLE };
	CColorButton	m_numerical_Bt;
	CColorButton	m_analyze_Bt;
	CComboBox	m_divideCombo;
	CButton	m_del_style_Bt;
	CButton	m_reset_style_Bt;
	CComboBox		m_styleCombo;
	CComboBox		m_indicesCombo;
	CComboBox		m_averageCombo;
	CColorButton	m_up_Bt;
	CColorButton	m_down_Bt;
	CColorButton	m_equal_Bt;
	CColorButton	m_average_Bt;
	CButton			m_bDownFill;
	CButton			m_bUpFill;
	CColorButton	m_graph_background_Bt;
	CColorButton	m_graph_border_Bt;
	CColorButton	m_graph_grid_Bt;
	CColorButton	m_scale_background_Bt;
	CColorButton	m_scale_border_Bt;
	CColorButton	m_scale_text_Bt;
	CColorButton	m_scale_grid_Bt;
	int				m_grid_width;
	int				m_grid_height;
	int				m_divide_width;
	int				m_divide_height;
	int		m_divide_radio;

	CBitmapItemComboBox       m_cmbHScaleGridType_Day;
	CBitmapItemComboBox       m_cmbHScaleGridType_Week;
	CBitmapItemComboBox       m_cmbHScaleGridType_Mon;
	CBitmapItemComboBox       m_cmbHScaleGridType_Year;

	CBitmapItemComboBox       m_cmbScaleGridThickness_Day;
	CBitmapItemComboBox       m_cmbScaleGridThickness_Mon;
	CBitmapItemComboBox       m_cmbScaleGridThickness_Year;
	CBitmapItemComboBox       m_cmbScaleGridThickness_Week;

	CColorButton       m_btnHScaleGridColor_Day;
	CColorButton       m_btnHScaleGridColor_Week;
	CColorButton       m_btnHScaleGridColor_Mon;
	CColorButton       m_btnHScaleGridColor_Year;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMainTabStyleSetUp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMainTabStyleSetUp)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeStylecombo();
	afx_msg void OnSelchangeIndicescombo();
	afx_msg void OnSelchangeAveragecombo();
	afx_msg void OnUpbutton();
	afx_msg void OnDownbutton();
	afx_msg void OnAverageBt();
	afx_msg void OnUpfill();
	afx_msg void OnDownfill();
	afx_msg void OnGraphBackground();
	afx_msg void OnGraphBorder();
	afx_msg void OnGraphGrid();
	afx_msg void OnScaleBackground();
	afx_msg void OnScaleBorder();
	afx_msg void OnAddStyle();
	afx_msg void OnDelStyle();
	afx_msg void OnResetStyle();
	afx_msg void OnChangeGridWidth();
	afx_msg void OnChangeGridHeight();
	afx_msg void OnScaleGrid();
	afx_msg void OnScaleText();
	afx_msg void OnEditchangeStylecombo();
	afx_msg void OnSelchangeDividecombo();
	afx_msg void OnDivideRadio();
	afx_msg void OnDivideR1();
	afx_msg void OnDivideR2();
	afx_msg void OnDivideR3();
	afx_msg void OnAnalyzeColor();
	afx_msg void OnNumericalColor();
	afx_msg void OnDestroy();
	afx_msg void OnEqualbutton();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnRecover();
	afx_msg void OnBtnPreview();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnSelchangeCmbHscaleGridtype_Day();
	afx_msg void OnSelchangeCmbHscaleGridtype_Week();
	afx_msg void OnSelchangeCmbHscaleGridtype_Mon();
	afx_msg void OnSelchangeCmbHscaleGridtype_Year();
	
	afx_msg void OnSelchangeScalelineThickness_Day();
	afx_msg void OnSelchangeScalelineThickness_Week();
	afx_msg void OnSelchangeScalelineThickness_Mon();
	afx_msg void OnSelchangeScalelineThickness_Year();

	afx_msg void OnBtnHScaleGridColor_Day();
	afx_msg void OnBtnHScaleGridColor_Week();
	afx_msg void OnBtnHScaleGridColor_Mon();
	afx_msg void OnBtnHScaleGridColor_Year();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		
// Attribute
public:
	int m_styleIndex;
	BOOL m_bPreview;

private:
	IChartOCX *m_pIChartOCX;
	CStyleData*	m_pStyleData;
	

// (2006/8/20 - Seung-Won, Bae) Manage StyleData with Share.
protected:
	CStyleDataManager	m_styleDataManager;
	CList< CStyleData *, CStyleData * > m_lstTempUserStyle;		// Copy of User Style Setting for using before Update(OK).
	
	BOOL m_bColorChanged;				// ColorDialog에서 색을 변경했는지 알기위한 변수
	int	m_comboIndex;
	int m_nStyle;
	int m_divideIndex;
	int m_divideRadio;
	
	CFont* m_pTitleFont;


// Implementation
protected:
	CColorButton* GetAnalyzeButton();
	int WriteUserStyleFile( void);

public:
	void Save( void );
	void Recover( void );
	void PreView( void );
	virtual CBaseTabDlg::DLG_TYPE GetDialogType() const;

private:
	COLORREF DoModal_ColorDialog(CColorButton* pButton);
	void SetColor(CColorButton* pButton, const COLORREF& color);

	bool IsAddStyle() const;
	void AddUserStyle();
	void AddString_Combo(const int nType);

	CStyleData* GetStyleData(const CList<CStyleData*, CStyleData*>& styleList, const int nIndex) const;
	void DeleteAll(CList<CStyleData*, CStyleData*>& styleList);
	bool DeleteUserStyle(const int nIndex);
	void CopyStyleList(const CList<CStyleData*, CStyleData*>& sourceList, CList<CStyleData*, CStyleData*>& targetList);

// (2006/8/22 - Seung-Won, Bae) Shere the Analysis Tool Color within Analysis and Style Setting.
public:
	void	UpdateAnalysisColor( COLORREF p_clrAnalysis);

protected:
	void	OnSelchangeStylecomboMain( void);

// (2007/5/2 - Seung-Won, Bae) Analysis Color Notify Skip Flag
protected:
	BOOL	m_bSkipAnalysisColor;

// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
protected:
	BOOL	m_bSkipVerticalScaleGrid;
	BOOL	m_bSkipHorizontalScaleGrid;

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAINTABSTYLESETUP_H__3A5A9A29_4365_4A77_BA5F_525009E1367A__INCLUDED_)
