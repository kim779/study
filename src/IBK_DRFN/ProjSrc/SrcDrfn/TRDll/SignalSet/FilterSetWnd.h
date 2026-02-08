#if !defined(AFX_FILTERSETWND_H__EC714124_42DC_422B_ADDA_DF7A19121522__INCLUDED_)
#define AFX_FILTERSETWND_H__EC714124_42DC_422B_ADDA_DF7A19121522__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterSetWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilterSetWnd window

const		LPCTSTR		TITLE_INCODE[] = { _T("시장구분"), _T("관심종목") }; 
const		LPCTSTR		TITLE_CHANGEVAL[] = {_T("상한"), _T("상승"), _T("보합"), 
                                        _T("하한"), _T("하락") };
//const		LPCTSTR		TITLE_DIRECTION[] = { _T("관리"), _T("이상급등"), _T("거래정지"), 
const		LPCTSTR		TITLE_DIRECTION[] = { _T("관리종목"), _T("투자주의"), _T("투자경고"),_T("투자위험예고"),_T("투자위험"),
									_T("거래정지"), _T("정리매매"), _T("투자주의환기"), _T("불성실공시"), 
									_T("증거금100%"), _T("우선주") };
const		LPCTSTR		TITLE_NEWS[] = { _T("전체"), _T("뉴스"), _T("공시") }; 


#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "./AddControl/Grid_Comm.h"

typedef struct 
{
	CStringArray strKey;
	CStringArray strGroupName;
} ST_GwanSimGroup;

class CFilterSetWnd : public CWnd
{
// Construction
public:
	CFilterSetWnd();
	virtual ~CFilterSetWnd();

public:
	CFont m_fontDefault, m_fontBold;
	CBrush  m_brushStatic;
	CBrush  m_brushSubTitlePoint;

	CStringArray	m_saOutCode;
	FilterInfo m_stFilterInfo;
	CList <SubFilterInfo, SubFilterInfo&>	m_listDealTimeVol;
	CList <SubFilterInfo, SubFilterInfo&>	m_listDealTimePrice;

public:
	COLORREF		m_clrMainSkin;

	CStatic			m_ctrlLeftTitlePoint;
	CStatic			m_ctrlTitle_In;
  
	CButton			m_btnInCodeGroup;
	CButton			m_btnInCodeRao[_countof(TITLE_INCODE)];
	CComboBox		m_comboMarket;
	CComboBox		m_comboPortGroup;

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

	CButton			m_btnNewsGroup;
	CButton			m_btnNewsRao[_countof(TITLE_NEWS)];

	CStatic			m_ctrlRightTitlePoint;
	CStatic			m_ctrlTitle_Out;

	CButton			m_btnDirectionGroup;
	CButton			m_btnDirectionChk[_countof(TITLE_DIRECTION)];

	CButton			m_btnOutCodeGroup;
	CButton			m_btnSoundGroup;
	CButton			m_chkSound;				//소리사용
	CEdit			m_editSound;
	CfxImgButton	m_btnSound;
	CfxImgButton	m_btnSearchSound;
		

	CStatic 		m_ctrlCodeInput;

	CWnd*			m_pctrlCodeInput;

	CCodeCtrl*		m_pctrlJMCombo;

	CStatic			m_ctrlCodeName;
	CfxImgButton	m_btnCodeAdd;
	CfxImgButton	m_btnCodeDelete;
	CfxImgButton	m_btnCodeDeleteAll;
	
	CGrid_Comm		m_gridOutCode;
	BOOL			m_bShow_PF_CTL;

	ST_GwanSimGroup m_GwanSimGroup;
	CString			m_strPathSound;
	BOOL			m_bchksound;

public:
	void SetInit(CMapStringToOb	*pMapControlName);
	void SetCodeComboInfo(CCodeCtrl* pCodeCombo, HWND hCodeCombo);
	void SetInit_SetVal();

	void GetPortData();
	void SetAdd_OutCode();

	void OnSize_Left(CRect reClient);
	void OnSize_Right(CRect reClient);

	int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }

	void SetInfo();
	BOOL GetInfo();

	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = PROGRAM_TITLE, UINT nType = MB_OK);
	void SetPosCodeInput();
	void OnPF_Contorl_Move();
	void PlatFormBtnAddEx();
	void PlatFormBtnRemoveEx();
	void PlatFormBtnRemoveAllEx();
	void PlatFormBtnVolTime();
	void PlatFormBtnPriceTime();
	
	void ChangeSkinColor(COLORREF clrBkgnd);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterSetWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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

	afx_msg void OnBtnInCodeRadio(UINT nID);

	afx_msg void OnBtnNowPriceChk();
	afx_msg void OnBtnChangeRateChk();

	afx_msg void OnBtnChangeValChk(UINT nID);

	afx_msg void OnBtnDealVolRadio();
	afx_msg void OnBtnDealVolTimeRadio();
	afx_msg void OnBtnDealPriceRadio();	
	afx_msg void OnBtnDealPriceTimeRadio();

	afx_msg void OnBtnSetDealVolTime();
	afx_msg void OnBtnSetDealPriceTime();

    afx_msg void OnBtnNewsRadio(UINT nID);
	afx_msg void OnBtnDirectionChk(UINT nID);

	afx_msg void OnBtnCodeAdd();
	afx_msg void OnBtnCodeDelete();
	afx_msg void OnBtnCodeDeleteAll();
	afx_msg void OnChkSound();
	afx_msg void OnBtnSearchsound();
	afx_msg void OnBtnSound();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERSETWND_H__EC714124_42DC_422B_ADDA_DF7A19121522__INCLUDED_)
