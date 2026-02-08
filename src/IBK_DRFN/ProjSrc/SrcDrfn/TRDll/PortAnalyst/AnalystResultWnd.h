#if !defined(AFX_ANALYSTRESULTWND_H__168921C2_F4C3_4F38_B902_BF7369318808__INCLUDED_)
#define AFX_ANALYSTRESULTWND_H__168921C2_F4C3_4F38_B902_BF7369318808__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnalystResultWnd.h : header file
//

/*
typedef struct tagAnalystInfo
{
	//# Data
	CString strDate;
	CString strTime;

	int nCloseData_Kospi;
	int nCloseData_Kosdaq;
	
	int nCloseData[CODE_MAX_CNT];

	//# Result
	double dParallel;
	double dAccumulate;
	int nUpCnt;
	int nDownCnt;

	double dAccumulate_Kospi;
	int nUpCnt_Kospi;
	int nDownCnt_Kospi;

	double dAccumulate_Kosdaq;
	int nUpCnt_Kosdaq;
	int nDownCnt_Kosdaq;
} AnalystInfo;
*/

/////////////////////////////////////////////////////////////////////////////
// CAnalystResultWnd window

#include "./AddControl/Grid_Comm.h"
#include "./AddControl/DrLnChartCtrl.h"

class CAnalystResultWnd : public CWnd
{
// Construction
public:
	CAnalystResultWnd();
	virtual ~CAnalystResultWnd();

private:
  int				m_nCodeCnt;
	CList <AnalystInfo, AnalystInfo&>		m_listAnalystInfo;
	CList <AnalystInfo, AnalystInfo&>		m_SublistAnalystInfo;

public:
	CButton			m_btnTarget_Day;
	CButton			m_btnTarget_Min;

	CGrid_Comm		m_gridProfit;

	BOOL			m_bChartTest;
	CDrLnChartCtrl  m_chartProfit;
	COLORREF		m_clrResultSkin;
  
private:
	CFont			  m_fontDefault;
	CBrush			m_brush;

	CButton			m_btnChk_All;
	CButton			m_btnChk_Kospi;
	CButton			m_btnChk_Kosdaq;
	CButton			m_btnChk_KospiAll;
	CButton			m_btnChk_KosdaqAll;

	CStatic			m_ctrlResultBar;
	
	void OnSize(CRect reClient);
	void SetInit();
	
	void ChartTest();

public:
	void SetInit_Val();
	void SetSelectType(int nType);

	void SetData(GroupToBase_Out *pstGroupToBase_Out);
	void SetData(GroupToBase_UPOut *pstGroupToBase_UPOut);
	void SetDataCalculation(CUIntArray *psiUserCodeRow);
	void SetDataCalculation_Target(CUIntArray *psiUserCodeRow);

	void SetDrewIndexToLine(CodeInfo stCodeInfo);
	void ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);
	void SetResultSave();
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType);
	void SetInit_AnalystInfo();

private:
	BOOL m_bDataSave;

	void SetSaveGridToExcel(CString strFileName);
	void SetSaveGridToText(CString strFileName);
	BOOL GetLoadIniInfo(CString strFile);
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalystResultWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	

	// Generated message map functions
protected:
	//{{AFX_MSG(CAnalystResultWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG

	afx_msg void OnBtnTargetDay();
	afx_msg void OnBtnTargetMin();

	afx_msg void OnBtnChkAll();
	afx_msg void OnBtnChkKospi();
	afx_msg void OnBtnChkKosdak();
	afx_msg void OnBtnChkKospiAll();
	afx_msg void OnBtnChkKosdakAll();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYSRESULTWND_H__168921C2_F4C3_4F38_B902_BF7369318808__INCLUDED_)
