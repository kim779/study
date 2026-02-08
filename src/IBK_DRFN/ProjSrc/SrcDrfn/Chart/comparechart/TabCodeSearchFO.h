#if !defined(AFX_TABCODESEARCHFO_H__EC2FCE12_3806_4933_8EC2_1DCD25B02808__INCLUDED_)
#define AFX_TABCODESEARCHFO_H__EC2FCE12_3806_4933_8EC2_1DCD25B02808__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCodeSearchFO.h : header file
//
#include "resource.h"
#include "TabDlg.h"
#include "../../inc/ExOCXDLL.h"
#include "../../inc/ExGridCtrl.h"
#include "./define/ChartMsgDef.h"

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchFO dialog

//typedef struct _codeinfo{
//	int	nInfo[5];
//
//	struct _codeinfo()
//	{
//		for (int i=0; i < 5; i++)
//			nInfo[i] = 0;
//	}
//
//}codeinfo;

class CTabCodeSearchFO : public CTabDlg
{
// Construction
public:
	CTabCodeSearchFO(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabCodeSearchFO)
	enum { IDD = IDD_TABCODESEARCH_FO };
	CExComboBox	m_ctrlComboFOType;
	CExComboBox	m_ctrlComboMonth;
	CListBox		m_listFuture;
	CButton			m_ctrlRadioKospi200;
	CButton			m_ctrlRadioMiniKospi;
	CButton			m_ctrlRadioWeeklyKospi;
	//}}AFX_DATA
	CExGridCtrl			*m_pGridCtrl;
	CList<codeinfo, codeinfo&>	m_listMaster;	// 콜 옵션 종목 정보 list
	codeinfo			m_nMonth;				// 콜 옵션 월물 정보
	CString				m_strWeeklyNo[12]; // 0024696: 파생상품 코드체계 변경 제도개선
	//int					m_nWeeklyNo[12];		// 위클리옵션 만기정보
	char				m_cYear[12];			// 콜 옵션 연도 정보
	BOOL				m_bMonday[12];	
	long				m_lMaxLimit;			// max 행사가
	int					m_nIndex;
	//char				m_cYear;
	int					m_nJumpCnt; // 0024696: 파생상품 코드체계 변경 제도개선
	IMasterDataManagerLast* m_pDataManager;
	CString m_strCodeNName;
	CString m_strDataDirPath;
	CString m_strSelCodeNName;

	int					m_nRadioSel;
	int					m_nComboSel;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCodeSearchFO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
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
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabCodeSearchFO)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnEditchangeComboMonth();
	afx_msg void OnSelchangeComboMonth();
	afx_msg void OnSelchangeComboFO();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeListFuture();
	afx_msg void OnDblclkListFuture();
	afx_msg void OnClickRadioKospi200();
	afx_msg void OnClickRadioMiniKospi();
	afx_msg void OnClickRadioWeeklyKospi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCODESEARCHFO_H__EC2FCE12_3806_4933_8EC2_1DCD25B02808__INCLUDED_)
