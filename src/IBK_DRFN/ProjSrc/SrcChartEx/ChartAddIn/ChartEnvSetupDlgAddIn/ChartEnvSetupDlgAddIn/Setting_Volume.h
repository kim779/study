#if !defined(AFX_SETTING_VOLUME_H__54C05E35_9F39_46BB_B508_F41A2562EBCE__INCLUDED_)
#define AFX_SETTING_VOLUME_H__54C05E35_9F39_46BB_B508_F41A2562EBCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_Volume.h : header file
//


#include "BaseDialog.h"
#include "ColorButton.h"


/////////////////////////////////////////////////////////////////////////////
// CSetting_Volume dialog
class CSetting_Volume : public CBaseDialog
{
// Construction
public:
	CSetting_Volume( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, CWnd *pTopWnd = NULL);   // standard constructor
	virtual ~CSetting_Volume();

// Dialog Data
	//{{AFX_DATA(CSetting_Volume)
	enum { IDD = IDD_VOLUME };
	CButton	m_btnFillUp;
	CButton	m_btnFillDown;
	CColorButton	m_btnUp;
	CColorButton	m_btnDown;
	CButton	m_FillDown_Bt;
	CButton	m_Fill_Bt;
	//}}AFX_DATA


// Attribute
private:
	int m_nSubGraphIndex;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_Volume)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual void Initial(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 >>
	virtual BOOL OnApply(IGraphInfo* p_pIGraphInfo);
	//virtual void OnApply(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 <<
	
	void OnApply_ColorButton( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex);

protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting_Volume)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdownVolume();
	afx_msg void OnSimpleVolume();
	afx_msg void OnSameBong();
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	afx_msg void OnFill();
	afx_msg void OnFillDown();
	afx_msg void OnCheckUp();
	afx_msg void OnCheckDown();
	afx_msg void OnUpdownPrice();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		
private:
	void Initial_ColorButton( IGraphInfo *p_pIGraphInfo, int p_nSubGraphIndex);

	int GetCheckToRadio();
	void ShowWindowColorListBox(const int nCmdShow);

private:
	//sy 2006.04.12. -> "거래량차트" title 색상 관련 옵션 유지
	int m_nTitleColorDrawCond;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_VOLUME_H__54C05E35_9F39_46BB_B508_F41A2562EBCE__INCLUDED_)
