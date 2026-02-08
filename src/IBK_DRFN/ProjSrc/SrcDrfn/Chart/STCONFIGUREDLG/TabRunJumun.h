#if !defined(AFX_TABRUNJUMUN_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_)
#define AFX_TABRUNJUMUN_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabRunJumun.h : header file
//

#include "Resource.h"
#include "RscDlg.h"

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExStatic.h"
#include "../../inc/ExEdit.h"
#include "../../inc/ExComboBox.h"
#include "../../inc/ExStatic.h"
//#include "../../inc/ExKoscomCtrl.h"
#include "../../inc/IMainInfoMng.h"
#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : 콜백함수 받는데 사용

/////////////////////////////////////////////////////////////////////////////
// CTabRunJumun dialog
//class AFX_EXT_CLASS CTAupAccountCtrl;
class CTabRunJumun : public CRscDlg
{
// Construction
public:
	CTabRunJumun(CWnd* pParent = NULL);   // standard constructor
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}

// Dialog Data
	//{{AFX_DATA(CTabRunJumun)
	enum { IDD = IDD_DLG_RUNJUMUN };
	CExComboBox		m_cbAcct;
	CExComboBox		m_cbBuy;
	CExComboBox		m_cbSell;
	CExComboBox		m_cbJMethod;
	CExComboBox		m_cbJTime;
	CExComboBox		m_cbJumunSS;
	CExDrawButton	m_btnFileBuy;
	CExDrawButton	m_btnPlayBuy;
	CExDrawButton	m_btnFileSell;
	CExDrawButton	m_btnPlaySell;
	CStatic			m_sttAccountName;		// 계좌명 컨트롤
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabRunJumun)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	void LoadAcctInfo();			// 계좌관련 리스트를 로딩한다.
	BOOL CheckInputPwd();
	void ReadAccType(CStringArray& arrAccType);
	BOOL IsTradableAcct(CString strAcct, CStringArray& arrAccType);

	virtual void LoadInfo();
	virtual BOOL SaveInfo();
	virtual void SetDefault();

protected:
	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	CWnd*	m_pDlgSTOption;
	CWnd*	m_pWndOrgAccountParent;

	IMainInfoManager* m_pMainMng;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabRunJumun)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnFileBuy();
	afx_msg void OnBtnFileSell();
	afx_msg void OnBtnPlayBuy();
	afx_msg void OnBtnPlaySell();
	afx_msg void OnSelchangeCmbAcct();
	afx_msg void OnSelchangeCmbMethod();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void fnBtnFileSearch(UINT nID) ;
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedChkMarketpriceclear();
	afx_msg void OnBnClickedChkMarketpriceentry();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABRUNJUMUN_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_)
