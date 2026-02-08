#if !defined(AFX_SETTING_PRICEMA_H__3AB73A79_A58A_4B85_9A34_70A7CDB31BBD__INCLUDED_)
#define AFX_SETTING_PRICEMA_H__3AB73A79_A58A_4B85_9A34_70A7CDB31BBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_PriceMA.h : header file
//

#include "BaseDialog.h"

#include "ColorButton.h"
#include "NumMASpinCtrl.h"
#include "DropBtn.h"

const int MAX_SHOW = 12;

class CSetting_PriceMA_Condition;


/////////////////////////////////////////////////////////////////////////////
// CSetting_PriceMA dialog

class CSetting_PriceMA : public CBaseDialog
{
// Construction
public:
	CSetting_PriceMA( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, CWnd *pTopWnd = NULL);   // standard constructor
	virtual ~CSetting_PriceMA();
	
	virtual void Initial(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 >>
	virtual BOOL OnApply(IGraphInfo* p_pIGraphInfo);
	//virtual void OnApply(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 <<

// Dialog Data
	//{{AFX_DATA(CSetting_PriceMA)
	enum { IDD = IDD_PRICEMA };
	CBitmapItemComboBox	m_cmbThicknessGraphAll;
	CBitmapItemComboBox	m_cmbDrawStyleGraphAll;
	CButton	m_btnShowGraphAll;
	BOOL	m_bShowArrow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_PriceMA)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting_PriceMA)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckAll();
	afx_msg void OnSelchangeDrawstyleAll();
	afx_msg void OnSelchangeThicknessAll();
	afx_msg LONG OnSelectionChanged(UINT wParam, LONG lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void OK( void );
	void Cancel( void );
		

// Method
protected:
	void Initial_Control( void );

	BOOL CreateDropButton( CDropButton &pBtn, CString strTitle, const UINT uID, const UINT uID2, bool bChangeTitleWithSelection );

	// MA Condition Dialog 관련
	void SetMAConditionDlgPosition( void );
	void SetAllMAConditionData( void );

	// 필요한 Window Ctrl 리턴.
	CButton* GetCheckButton(int nSubGraphIndex);
	CNumEdit* GetEditCtrl(int nSubGraphIndex);
	CNumSpinCtrl* GetSpinCtrl(int nSubGraphIndex);
	CComboBox* GetCalcMethodCombo( int nSubGraphIndex );
	CComboBox* GetPacketCombo( int nSubGraphIndex );
	CColorButton *GetColorButton( int nSubGraphIndex );
	CBitmapItemComboBox* GetDrawStyleCombo( int nSubGraphIndex );
	CBitmapItemComboBox* GetThicknessCombo( int nSubGraphIndex );

	void SetShowHide(int nSubGraphIndex, int nCheck);
	void SetConditionValue( int nSubGraphIndex, int nConditionValue );
	void SetCalMethod(int nSubGraphIndex);			// 각각의 이평의 계산방법을 콤보박스에 세팅
	void SetPacketName(int nSubGraphIndex);			// 각각의 이평의 계산할 패킷명을 콤보박스에 세팅
	void SetLineColor( int nSubGraphIndex );		// 선색깔 세팅.
	void SetDrawStyle( int nSubGraphIndex );	
	void SetLineThickness( int nSubGraphIndex );	// 선굵기 세팅

	void SetShowArrow();				// 그래프신호값 세팅한다.

	void ShowHideSubGraph( int nSubGraphIndex, int nCmdShow );

	// 변경값을 적용한다.
	void ApplyAllGraphData();

// Field
private:
	CWnd* m_pParent;	
	bool m_bInitialize;

	CDropButton	m_btnPacketAll;
	CDropButton	m_btnCalcMethodAll;
	
	CSetting_PriceMA_Condition *m_pDlgMACondition;

	int m_nSubGraphCount;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_PRICEMA_H__3AB73A79_A58A_4B85_9A34_70A7CDB31BBD__INCLUDED_)
