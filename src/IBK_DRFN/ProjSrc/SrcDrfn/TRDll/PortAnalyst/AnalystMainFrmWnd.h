#if !defined(AFX_ANALYSTMAINFRMWND_H__09FF1BD0_5D3B_450E_8327_45CE292E3AB2__INCLUDED_)
#define AFX_ANALYSTMAINFRMWND_H__09FF1BD0_5D3B_450E_8327_45CE292E3AB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnalystMainFrmWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnalystMainFrmWnd frame
#include "../../DrCommon/Dr_Control/DrBtnCtrl.h"
#include "../../DrCommon/Dr_Control/DrTabCtrl.h"

#include "./AddControl/Grid_Comm.h"
#include "./AddControl/DrTreeCtrl.h"

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "../../../SrcSite/control/fx_misc/misctype.h"
#include "../../DrCommon/Dr_Control/DrTabCtrl.h"

typedef struct 
{
	CStringArray strKey;
	CStringArray strGroupName;
} ST_GwanSimGroup;

class CAnalystResultWnd;
class CAnalystMainFrmWnd : public CFrameWnd
{
public:
	CAnalystMainFrmWnd();           // protected constructor used by dynamic creation
	virtual ~CAnalystMainFrmWnd();

	int Modual_SendTr(CString TrNo, int nTrLen, BYTE *TrData);
	HRESULT ReceiveData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen , int nKey = 0);
	int ParseGroup(char* pReceiveBuff , CStringArray& arrKey, CStringArray& arrName);
	int ParseList(char* pReceiveBuff, CStringArray& szItemList);

//   	IDebugManager		*m_pDebugMng;
#ifdef _DEBUG
	int m_nTickCnt, m_nAfterTickCnt;
#endif

public:
	CUIntArray		m_siUserCodeRow;
	int m_nAnalystData_TargetType;		//# 0 : All, 1 : Kospi, 2 : Kosdaq

private:
	CFont				m_fontDefault, m_fontBold;
	CBrush			m_brushStatic;
	COLORREF		m_clrMainSkin;

	BOOL m_bExpFlag;
	
  
	int m_nKospiCnt, m_nKosdaqCnt;

	BOOL m_bRun;
	int m_nTrIndex;

	BOOL m_bInit;
	BOOL m_bRecMST;

public:
	CStatic			m_ctrlTitle;

	CDrTreeCtrl		m_treeCode;
	CGrid_Comm		m_gridCode;

	CDrBtnCtrl		m_btnSelectOk;
	CDrBtnCtrl		m_btnSelectCancel;
	CDrBtnCtrl		m_btnListSearch;

	CfxImgButton	m_btnExp;

	CfxImgButton	m_btnList_All;
	CfxImgButton	m_btnList_Kospi;
	CfxImgButton	m_btnList_Kosdaq;

	CGrid_Comm		m_gridResult;
	CStatic			m_ctrlTarget;

	CDateTimeCtrl	m_ctrlTarget_Data;  
	CComboBox		m_ctrlTargetCombo_Time;
	CfxImgButton	m_btnResultSearch;
	CStatic			m_ctrlCalculationTerm;

	CDrTabCtrl*		m_ptabAnalystResult;
	CRect			m_rtTab;
	CAnalystResultWnd*	m_pwndAnalystResult;
	CStatic			 m_ctrlMsg;


	CStatic			m_ctrlPass;
	CProgressCtrl	m_progressPass;
	CStatic			m_ctrlPassResult;

	CfxImgButton	m_btnSend;
	CStatic			m_ctrlHelpText;

//	CPCTrMngHelper	pctrHelper;
	CStringArray	m_saConditionSearchResultItemCode;
	BOOL			m_bLinkData;
	
	ST_GwanSimGroup m_GwanSimGroup;

	CStatic			m_stcStatusBar;

private:
	void OnSize();
	void OnSize_Left(CRect reClient);
	void OnSize_Right(CRect reClient);
	int	 GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }

	void SetSendTr_UpJong();

	void SetSendTitleBar(CString strMsg);
	void SetSharedDataToSend(ShardData *pstShardData);

public:
	void SetInit(CString strLinkData);
	void SetSendTr_Code();
	void SetSendMainMsg(CString strMsg);
	void SetCodeCnt();

	void SetWaitCursor(BOOL bState);
	void SetPassResult(int nData);
	void GetKwansimGroupName(CStringArray& arrGroupName);
	void GetItemsWithGroupName(CString strIndex, CStringArray& arrItems);

	BOOL GetItemNameAndMarket(CString& strCode, CString& strName, int& nMarket);
	void ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);

	void OnPF_Contorl_Move();
	void OnPF_Btn_Event(CString szCtrlName, int nEventID, CString strData = "", long nData = 0);
	void OnPF_Combo_Event(CString szCtrlName, int nEventID, CString strData = "", int nData = 0);
	void OnPF_Check_Event(CString szCtrlName, int nEventID, int nData = 0, CString strData = "");
	void OnPF_Calendar_Event(CString szCtrlName, int nEventID, CString strData = "", long nData = 0);
	

	void PlatFormBtnTotal();
	void PlatFormBtnKospi();
	void PlatFormBtnKosdaq();	
	void PlatFormBtnSearch();
	void PlatFormBtnSend();
	void PlatFormBtnComboTime(int nIndex);
	void PlatFormBtnCalendar(LPCTSTR szData);

	BOOL SetPortfolioReg(CString strOpenDlg, CStringArray *psaCode);

	void		SetBtnImg(CfxImgButton *pBtn, CString strFileName);
	void		SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE);
	
	CFont*		GetFont(BOOL bBold = FALSE);
	COLORREF	GetColor(int nColor);
	CString		Variant(int nComm, CString strData = "");
	
	COLORREF	GetAxColor(int nColor);
	HBRUSH		GetAxBrush(COLORREF clr);
	
	int			SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType);
	void		PutLinkData(CString strType, CString strCode);

	void		GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName);
	BOOL		GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey);
	void		RegPortItem(CStringArray* pSaItemCode);
	CWnd*		GetModuleMain();
	
	BOOL		GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub = 0);
	BOOL		GetUpMasterData(CStringArray *&strCode, CStringArray *&strName, int jgub);

	int			GetUserMaster(LPCSTR szKey, CStringArray &arrCode, CStringArray &arrName);
	CPen*		GetAxPen(COLORREF clr, int nWidth, int nStyle);

	void		OnRButtonClick(CPoint *point, CString strCode = "");

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalystMainFrmWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAnalystMainFrmWnd)
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG

	afx_msg void OnNotify_ChangeTab(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBtnSelectOk();
	afx_msg void OnBtnSelectCancel();
	afx_msg void OnBtnSearch_Code();
	afx_msg void OnBtnExp();
	afx_msg void OnBtnListAll();
	afx_msg void OnBtnKospi();
	afx_msg void OnBtnKosdaq();
	afx_msg void OnBtnSearch_Result();

	afx_msg void OnBtnSend();
	afx_msg void OnMenuResultCtrl(UINT nIndex);

	afx_msg LRESULT OnNotify_Grid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_Tree(WPARAM wParam, LPARAM lParam);
	afx_msg long OnGetData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	void SetCommMenu(POINT *point);

public:
	//--> Ελ½Ε
// 	IAUTrCommManager*	m_pITrCommManager;
// 	ITrComm2*			m_pITrComm,
// 						*m_pITrCommUP,
// 						*m_pITrCommCon;

	
// 	class CTrCommSite : public ITrCommSite
// 	{
// 	public:
// 		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
// 		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData);
// 		
// 		STDMETHOD(StatusChange) (int nState);
// 		STDMETHOD(ReceiveMessage) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage);
// 		
// 		
// 	} m_xTrCommSite;
// 	friend class CTrCommSite;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYSTMAINFRMWND_H__09FF1BD0_5D3B_450E_8327_45CE292E3AB2__INCLUDED_)
