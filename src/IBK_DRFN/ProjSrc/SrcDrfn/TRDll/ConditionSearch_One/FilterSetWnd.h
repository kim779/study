#if !defined(AFX_FILTERSETWND_H__EC714124_42DC_422B_ADDA_DF7A19121522__INCLUDED_)
#define AFX_FILTERSETWND_H__EC714124_42DC_422B_ADDA_DF7A19121522__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterSetWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilterSetWnd window

const		LPCTSTR		TITLE_MARKET[] = { _T("전 체"), _T("코스피"), _T("코스닥") }; 

const		LPCTSTR		TITLE_CHANGEVAL[] = {_T("상한"), _T("상승"), _T("보합"), 
_T("하한"), _T("하락") };

const		LPCTSTR		TITLE_DIRECTION[] = { _T("관리"), _T("이상급등"), _T("거래정지"), 
_T("정리매매"), _T("투자유의"), _T("불성실공시"), 
_T("우선주") };

//# Control
#include "../../DrCommon/Dr_Control/DrBtnCtrl.h"
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "./AddControl/SetGrid.h"

class CFilterSetWnd : public CWnd
{
public:
	CFilterSetWnd();
	virtual ~CFilterSetWnd();
	
	CFont m_fontDefault, m_fontBold;
	CBrush  m_brushStatic;
	
public:
	BOOL m_bCreateState;
	CStringArray	m_saOutCode;
	
	BOOL m_bLoad_Filter;
	FilterInfo m_stFilterInfo;
	
	CList <SubFilterInfo, SubFilterInfo&>	m_listDealTimeVol;
	CList <SubFilterInfo, SubFilterInfo&>	m_listDealTimePrice;
	
public:
	CStatic			m_ctrlTitle_In;
	
	CButton			m_btnBaseGroup;
	CButton			m_btnMarketRao[_countof(TITLE_MARKET)];
	
	CButton			m_btnPriceGroup;
	CButton			m_btnNowPriceChk;
	CEdit			m_editNowPriceUp, m_editNowPriceDown;
	CStatic			m_ctrlNowPrice1, m_ctrlNowPrice2;
	CButton			m_btnChangRateChk;
	CEdit			m_editChangRateUp, m_editChangRateDown;
	CStatic			m_ctrlChangRate1, m_ctrlChangRate2;
	
	CButton			m_btnChangGroup;
	CButton			m_btnChangValChk[_countof(TITLE_CHANGEVAL)];
	
	CButton			m_btnDealVolGroup;
	CButton			m_btnDealVolRao;
	CEdit			m_editDealVol;
	CStatic			m_ctrlDealVol;
	CButton			m_btnDealVolTimeRao;
	CfxImgButton	m_btnSetDealVolTime;
	
	CButton			m_btnDealPriceGroup;
	CButton			m_btnDealPriceRao;
	CEdit			m_editDealPrice;
	CStatic			m_ctrlDealPrice;
	CButton			m_btnDealPriceTimeRao;
	CfxImgButton	m_btnSetDealPriceTime;
	
	CButton			m_btnConcludeGroup;
	CButton			m_btnDealVolChk_Conclude;
	CEdit			m_editDealVol_Conclude;
	CStatic			m_ctrlDealVol_Conclude;
	CButton			m_btnDealPriceChk_Conclude;
	CEdit			m_editDealPrice_Conclude;
	CStatic			m_ctrlDealPrice_Conclude;
	
	CStatic			m_ctrlTitle_Out;
	
	CButton			m_btntDirectionGroup;
	CButton			m_btntDirectionChk[_countof(TITLE_DIRECTION)];
	
	CButton			m_btnOutCodeGroup;
	CStatic			m_ctrlCodeInput;
	CStatic			m_ctrlCodeName;
	CfxImgButton	m_btnCodeAdd;
	CfxImgButton	m_btnCodeDelete;
	CfxImgButton	m_btnCodeDeleteAll;
	
	CSetGrid		m_gridOutCode;

	CWnd*			m_pMainWnd;
	
	
public:
	void SetInit();
	void SetInit_Base();
	
	void OnSize(CRect reClient);
	void OnSize_Left(CRect reClient);
	void OnSize_Right(CRect reClient);
	int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }
	
	void SetInfo();
	BOOL GetInfo();
	
	BOOL Create(CWnd* pParentWnd, UINT nID, CWnd* pMainWnd);
	BOOL SetInfo_Filter(BOOL bLoad, BOOL bDefultVal = FALSE);
	
	CString GetCodeToName(CString strCode);
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterSetWnd)
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CFilterSetWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	
	afx_msg void OnBtnMarketRadio(UINT nID);
	
	afx_msg void OnBtnNowPriceChk();
	afx_msg void OnBtnChangeRateChk();
	
	afx_msg void OnBtnChangeValChk(UINT nID);
	
	afx_msg void OnBtnDealVolRadio();
	afx_msg void OnBtnDealVolTimeRadio();
	afx_msg void OnBtnDealPriceRadio();	
	afx_msg void OnBtnDealPriceTimeRadio();
	
	afx_msg void OnBtnSetDealVolTime();
	afx_msg void OnBtnSetDealPriceTime();
	
	afx_msg void OnBtnDirectionChk(UINT nID);
	
	afx_msg void OnBtnCodeAdd();
	afx_msg void OnBtnCodeDelete();
	afx_msg void OnBtnCodeDeleteAll();
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERSETWND_H__EC714124_42DC_422B_ADDA_DF7A19121522__INCLUDED_)
