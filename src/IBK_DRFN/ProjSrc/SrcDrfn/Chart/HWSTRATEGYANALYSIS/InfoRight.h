//{{AFX_INCLUDES()

//}}AFX_INCLUDES
#if !defined(AFX_INFORIGHT_H__B4DC73D0_4ABC_4430_8258_B0FDCF10259C__INCLUDED_)
#define AFX_INFORIGHT_H__B4DC73D0_4ABC_4430_8258_B0FDCF10259C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoRight.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoRight dialog
#include "resource.h"

//#include "InputLeft.h"
#include "Control/XTabCtrl.h"
//#include "Control/xSkinButton.h"

//Tab Header Add :KHD///
#include "TabAnalChart.h"
#include "TabEarnChart.h"
#include "TabResult.h"
#include "TabSonikChart.h"
#include "TabStep.h"
#include "TabTrade.h"
/////////////////////////
//#include "GridCtrl_src/GridCtrl.h"
//#include "explorer.h"
#include "../../inc/ISysTabExChartItem.h"
#include "../MultiChart/ChartItem.h"
#include "afxwin.h"
#include "../../inc/ExStatic.h"
#include "../../inc/ExDrawButton.h"
#include <AFXMT.H>
//Add by KHD : 2006.08.28
typedef struct ChartHeader
{
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[90];    	/* Chart Bojo Data  */
} ST_CHARTHEADER;

typedef struct ChartData
{
	char    date[8];			/* 날짜     		*/
	char    open[10];			/* 시가     		*/
	char    high[10];			/* 고가     		*/
	char    low[10];			/* 저가     		*/
	char    close[10];			/* 종가   			*/
	char    volume[10];     	/* 거래량   		*/
} ST_CHARTDATA;

//END
class CDlgAnalysis;
class CInputLeft;
class CInfoRight : public CEscDialog
{
// Construction
public:
	CInfoRight(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInfoRight)
	enum { IDD = IDD_DLG_INFORIGHT };
	CButton	m_CtlResult;
	CButton    m_CtlShowAll;
	CButton    m_CtlSpread;
	CXTabCtrl	m_CtlTab;
	CExDrawButton		m_btnExportExcel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoRight)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfoRight)
	afx_msg void OnBtnShow();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnShowall();
	afx_msg void OnBtnResult();
	afx_msg void OnBtnExportExcel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	BOOL m_bShowToggle;
	CInputLeft* m_Input;
	CRect m_rcParent;
	CBrush	m_brBkColor;
	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	CArray<CDlgInterface*,CDlgInterface*> m_arrayTab;
	CString m_strFilePath;
//Tab Dlg
	//CTabAnalChart *m_AnalChart;
	CTabResult *m_Result;
	CTabStep *m_Step ;
	CTabTrade *m_Trade;
	CTabEarnChart *m_EarnChart;
	CTabSonikChart *m_SonikChart;
	//CGridCtrl m_GridStatus;
	CString m_html;
	CArray<ST_DATAINDEX,ST_DATAINDEX> m_pstDataIndex;

	short m_nNum;
	
	void SetRefresh(int);//File Refresh
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}
	void SetLeftInfo(CInputLeft* pdlg);
	void SetParentsRect(CRect rt);
	void SetEnableButton(BOOL bEnable);
	void SetFocusIndex(int nIndex);

	void InitTab();
	void InitSkin();
	void InitGrid();
	void InitHeader();
	void RefreshWebView();
	void MakeHtmlFile();
	CString ParsingstrHtml(CString strHtml);

	void InitCodeGubun(BOOL IsGubun);
	//Chart Function!! 
	void AllUpdateDlg();
	void InitWebView();
	//KHD : 2006.07.27
	void ApplyST();
	CDlgAnalysis* m_pDlgAnalysis;
	ISysTabExChartItem* m_pChartItem;
	void DrawGraph();
	CExComboBox  m_ctlComboDivide;
	afx_msg void OnCbnSelchangeComboDivide();
//	afx_msg void OnTcnFocusChangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTab1(NMHDR *pNMHDR, LRESULT *pResult);
	//상위 Static
	CExStatic m_ctlstName;
	CExStatic m_ctlSTFILENAME;
	CExStatic m_ctlstPosition;
	CExStatic m_ctlstPeriod;
	CExStatic m_ctlstNujuk;
	//이름
	CExStatic m_stJName1;
	CExStatic m_stSTName;
	CExStatic m_stPosition;
	CExStatic m_stPeriod;
	CExStatic m_stNujuk;

	CFont	m_Font;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFORIGHT_H__B4DC73D0_4ABC_4430_8258_B0FDCF10259C__INCLUDED_)
