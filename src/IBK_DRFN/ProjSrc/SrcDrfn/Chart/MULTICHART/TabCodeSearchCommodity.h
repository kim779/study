#if !defined(AFX_TABCODESEARCHCOMMODITY_H__EC2FCE12_3806_4933_8EC2_1DCD25B02808__INCLUDED_)
#define AFX_TABCODESEARCHCOMMODITY_H__EC2FCE12_3806_4933_8EC2_1DCD25B02808__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCodeSearchCommodity.h : header file
//
#include "resource.h"
#include "TabDlg.h"
#include "../../inc/ExOCXDLL.h"
#include "../../inc/ExGridCtrl.h"
#include "./define/ChartMsgDef.h"

class CTabCodeSearchCommodity :
	public CTabDlg
{
public:
	CTabCodeSearchCommodity(CWnd* pParent = NULL);

	CRect	GetRectOfCtrl(INT nID,BOOL bInit = FALSE);
	void	RecalcLayout();
	void	InitGrid();
	void	InitGridRowCol();
	void	ChangeGridRowCol();
	void	InsertGridData();
	void	InsertMasterData(int index);
	CString	GetInsertGridDataText(int row, int col);
	BOOL	InitMasterData();
	int		GetGridRow(int nExercise);
	int		GetGridExercise(int row);
	void	UpdateHogaData(int nRow, int nCol, double dPrice, COLORREF crFgClr, int nPoint=2);
	long	GetRealExercise(int nPrice);
	BOOL	GetCodeInfo(int nType, int& row, int& col, CString& strCode);

	CExGridCtrl			*m_pGridCtrl;
	CList<codeinfo, codeinfo&>	m_listMaster;	// 콜 옵션 종목 정보 list
	codeinfo			m_nMonth;				// 콜 옵션 월물 정보
	long				m_lMaxLimit;			// max 행사가
	int					m_nIndex;
	IMasterDataManager* m_pDataManager;
	CString m_strDataDirPath;
	int					m_nOptionMarket;

// Dialog Data
	//{{AFX_DATA(CTabCodeSearchCommodity)
	enum { IDD = IDD_TABCODESEARCH_COMMODITY };
	CExComboBox	m_ctrlComboMonth;
	CListBox		m_listFuture;
	CListBox		m_listFuture2;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCodeSearchCommodity)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabCodeSearchCommodity)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnEditchangeComboMonth();
	afx_msg void OnSelchangeComboMonth();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeListFuture();
	afx_msg void OnSelchangeListFuture2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bSpread;
	afx_msg void OnBnClickedRdoFuture();
	afx_msg void OnBnClickedRdoSpread();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCODESEARCHCOMMODITY_H__EC2FCE12_3806_4933_8EC2_1DCD25B02808__INCLUDED_)
