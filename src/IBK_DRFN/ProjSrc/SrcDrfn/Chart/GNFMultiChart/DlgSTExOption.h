#if !defined(AFX_DLGSTEXOPTION_H__FADB0B3A_E1A4_40C1_A4C3_BAAE77553DE5__INCLUDED_)
#define AFX_DLGSTEXOPTION_H__FADB0B3A_E1A4_40C1_A4C3_BAAE77553DE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
// #include "afxwin.h"
#include "../chart_common/ILButton.h"
#include "../../inc/ExOCXDLL.h"
#include "./control/XListCtrl.h"

// CDlgSTExOption 대화 상자입니다.

class CDlgSTExOption : public CDialog
{
//	DECLARE_DYNAMIC(CDlgSTExOption)

public:
	CDlgSTExOption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgSTExOption();

public:
	void InitColumn(CXListCtrl* pListCtrl);	// 리스트컨트롤 초기화 루틴.

	CWnd*	m_pWndTrader; 

// 대화 상자 데이터입니다.
	//{{AFX_DATA(CDlgSTExOption)
	enum { IDD = IDD_DLGSTEXOPTION };
	CExComboBox	m_CtlComboRep;
	CExComboBox	m_CtlComboDiv;
	CXListCtrl	m_CtlList;
	CILButton	m_BtnSTApply;
	CILButton	m_BtnOption;
	CILButton	m_BtnSTReport;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSTExOption)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSTExOption)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedStshowoption();
	afx_msg void OnBnClickedStreport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CRect m_rcList;

public:
	HINSTANCE m_hTradeInst;
	int		GetDivCount();		// 분할수
	double	GetRepRate();		// 중첩율
	void	   OpenRefort();
	BOOL	m_bIsApplyBtn;
	void		SetInitDlg();
	afx_msg void OnBnClickedStapply();
	afx_msg void OnDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTEXOPTION_H__FADB0B3A_E1A4_40C1_A4C3_BAAE77553DE5__INCLUDED_)
