#if !defined(AFX_JONGSTATUSINTAB_H__63EA849A_B170_4F32_9E17_6794DB806264__INCLUDED_)
#define AFX_JONGSTATUSINTAB_H__63EA849A_B170_4F32_9E17_6794DB806264__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JongStatusInTab.h : header file
//
#include "RscDlg.h"
#include "Resource.h"
#include "../../inc/ExOCXDLL.h"
/////////////////////////////////////////////////////////////////////////////
// CJongStatusInTab dialog
//#include "./control/BtnST.h"
class CJongStatusInTab : public CRscDlg
{
// Construction
public:
	CJongStatusInTab(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJongStatusInTab)
	enum { IDD = IDD_JONGSTATUS };
	CExStatic	m_stRate;
	CExStatic	m_stUpLimit;
	CExStatic	m_stUp;
	CExStatic	m_stSteady;
	CExStatic	m_stDownLimit;
	CExStatic	m_stDown;
	CExStatic	m_stCnt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJongStatusInTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetStatus(long lUpLimit,long lUp, long lSteady, long lDown, long lDownLimit, long lCnt, double dDaebiRate);
	void UpdateStatus(char chSignNew, char chSignOld, double dDaebiRateChg);
	void InitStatus();
	CRect	GetRectOfCtrl(int nID,CRect rectClient);
private:
	long m_lUpLimit;
	long m_lUp;
	long m_lSteady;
	long m_lDown;
	long m_lDownLimit;
	long m_lCnt;
	double m_dDaebiRate;
	CBrush	m_brUpLimit;
	CBrush	m_brUp;
	CBrush	m_brSteady;
	CBrush	m_brDown;
	CBrush	m_brDownLimit;
	COLORREF	m_crUpLimit;
	COLORREF	m_crUp;
	COLORREF	m_crSteady;
	COLORREF	m_crDown;
	COLORREF	m_crDownLimit;
	CBrush	m_brBkColor;
	COLORREF m_crBk;

	void UpdateInCtrl();

	//ToolTip Add by LYH 2007.01.03
	CToolTipCtrl	m_ToolTip;
protected:

	// Generated message map functions
	//{{AFX_MSG(CJongStatusInTab)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JONGSTATUSINTAB_H__63EA849A_B170_4F32_9E17_6794DB806264__INCLUDED_)
