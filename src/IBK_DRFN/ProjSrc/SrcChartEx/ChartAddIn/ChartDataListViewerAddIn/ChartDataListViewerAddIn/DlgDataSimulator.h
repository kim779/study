#if !defined(AFX_DLGDATASIMULATOR_H__0FE9B31C_E4D2_4407_BBB8_6CA55D570BB9__INCLUDED_)
#define AFX_DLGDATASIMULATOR_H__0FE9B31C_E4D2_4407_BBB8_6CA55D570BB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDataSimulator.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDataSimulator dialog
#include "../Include_GridCtrl/GridCtrl.h"
#include "ChartDataSimulatorAddInImp.h"

class CChartDataGridManager;
class CDlgDataSimulator : public CDialog
{
// Construction
public:
	CDlgDataSimulator( CChartInfo::CHART_MODE p_eChartMode, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDataSimulator)
	enum { IDD = IDD_DLG_DATA_SIMULATOR };
	CString	m_strTitle;
	//}}AFX_DATA

public:	

	HWND					m_hOcxWnd;

	CGridCtrl				m_Grid;

	CChartDataGridManager *	m_pChartDataGridManager;

	CChartDataList			m_CopyChartDataList;

	CString					m_strUserFilePath;

	CChartRecordSet		*	m_pChartLastRecordSet;

	CString					m_strLastDateTime;//년월일시분초 => 실시간 업데이트에서 업데이트 여부로 사용

public:	

	void	InitGrid();

	void	MakeTitle();

	void	InitChartData();

	void	MakeCopyChartData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDataSimulator)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDataSimulator)
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnExcelExport();	
	afx_msg void OnBtnExport();
	afx_msg void OnBtnImport();
	afx_msg void OnBtnInit();
	afx_msg void OnBtnRemoveAll();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnOk();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// (2009/9/14 - Seung-Won, Bae) for World-On
protected:
	CChartInfo::CHART_MODE	m_eChartMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDATASIMULATOR_H__0FE9B31C_E4D2_4407_BBB8_6CA55D570BB9__INCLUDED_)
