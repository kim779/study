#if !defined(AFX_DLGANALYSIS_H__05358C07_3A48_449A_B5D8_5E4418B96018__INCLUDED_)
#define AFX_DLGANALYSIS_H__05358C07_3A48_449A_B5D8_5E4418B96018__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAnalysis.h : header file
//

#include "resource.h"

//#include "control/Label.h"			// for CLabel


/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysis dialog

class CPatternMaker;
class CGridCtrl;
class CDlgAnalysis : public CDialog
{
// Construction
public:
	CDlgAnalysis(CWnd* pParent,  CPatternMaker* pPatternMaker);   // standard constructor

protected:
	CPatternMaker*		m_pPatternMaker;
	CGridCtrl*			m_pGridCandleRtpType1;
	CGridCtrl*			m_pGridWaveRptType1;


	CFont				m_Font;
	LOGFONT				m_logFont;	
	CRect				m_rectGrid;
	CBrush				m_brBack;		// Dialog BG Color

	void				InitFont();

// Wave Pattern Grid
public:
	void				InitGrid_WaveRptType1();
	void				InitGridRowCol_WaveRptType1();
	void				ResizeGridRowCol_WaveRptType1();
	void				InitGridFormat_WaveRptType1();

	void				SetFoundWavePtnToGrid_RptType1( CObject *pObjWavePatternData, double *p_dTime, double *p_dHigh, double *p_dLow, int nTotalDataCnt);

// Candle Pattern Grid
public:
	void				InitGrid_CandleRptType1();
	void				InitGridRowCol_CandleRptType1();
	void				ResizeGridRowCol_CandleRptType1();
	void				InitGridFormat_CandleRptType1();

	void				SetFoundCandlePtnToGrid_RptType1( CObject *pObjCandlePatternData);
	void				SetPercentNColor( CGridCtrl* pGridCtrl, int nRow, int nCol, double dValue);			// Set Percent Text and Color

	void				InsertGridRow(CGridCtrl* pGridCtrl, int nAddRowCnt);								// ¿­ »ðÀÔ
	void				InitInsertedGirdRow(CGridCtrl* pGridCtrl);											// ¿­ »ðÀÔÈÄ ÃÊ±âÈ­

public:
	void				SetReportTitleBasicInfo( double dStartDate, double dEndDate, int nDataCnt);

public:
	void				GetSelectedCandlePatternData( int nRow);
	void				SetShareData(CGridCtrl* pGrid, int nRow);



// Dialog Data
	//{{AFX_DATA(CDlgAnalysis)
	enum { IDD = IDD_DLG_ANALYSIS };
	CStatic	m_staticGridBase;
	int		m_rdo_nRptType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAnalysis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAnalysis)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRdoWave();
	afx_msg void OnRdoCandle();
	afx_msg void OnBtnEnvSetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
// 2008.02.20 by LYH >>
protected:
	HWND m_hOcxWnd;
// 2008.02.20 by LYH <<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGANALYSIS_H__05358C07_3A48_449A_B5D8_5E4418B96018__INCLUDED_)
