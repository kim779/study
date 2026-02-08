#if !defined(AFX_MAINFRMWND_H__A8A882E3_60C6_4076_A588_15227EEB100A__INCLUDED_)
#define AFX_MAINFRMWND_H__A8A882E3_60C6_4076_A588_15227EEB100A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainFrmWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd frame

#include "./AddControls/Grid_Comm.h"

#include "../../DrCommon/Dr_Control/DrTabCtrl.h"
#include "../../DrCommon/Dr_Control/DrCombo.h"
#include "../../Chart/Chart_Common/ILButton.h"
#include "../../DrCommon/Dr_Control/DrBtnCtrl.h"
#include "AddControls/ListImageWnd.h"
#include "Definefiles/Tr_Theme.h"
#include "MyCandle.h"
#include "FixWaitCursor.h"

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "../../../SrcSite/control/fx_misc/misctype.h"

#include "./CandleChart/DlgChart_Stock.h"
#include "../../inc/IAUPartnerMng.h"
#include "../../inc/IAuTrCommMng.h"

typedef struct
{
	CStringArray strKey;
	CStringArray strGroupName;
} ST_GwanSimGroup;

class CDlgChart_Stock;
class CFieldEditDlg;
class CMainFrmWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CMainFrmWnd)
public:
	CMainFrmWnd();           // protected constructor used by dynamic creation
	virtual ~CMainFrmWnd();
	HRESULT ReceiveData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen , int nKey = 0);
	HRESULT StatusChange(int nState) ;

	void	SendTr_ChartData(CString strCode, CString strDate);
	void SaveUserCandleSet();

	int ParseGroup(char* pReceiveBuff, CStringArray& arrKey, CStringArray& arrName);
	int ParseList(char* pReceiveBuff, CStringArray& arrName);
	void Request_PortGroup();
private:
	ST_GwanSimGroup m_GwanSimGroup;
	CFixWaitCursor m_WaitCursor;	
	CANDLESET_ARRAY m_CandleSetArray;
	CANDLESET_ARRAY m_UserCandleSetArray;
	CANDLE_SET* ParseToCandleSet(CString strLine);

	void ReadUserCandleSet();
	void ReadPresetCandle();
	
	void MakeCandleImageList();
	void MakeUserCandleImageList();
	void CheckUserPatternDir();

	void CreateImageList();
	void CreateButton();
	void CreateStatic();
	void CreateCombo();
	void CreateMisc();

	void EditChangeRate(int nIndex);
	BOOL SetTempGroupReg(CStringArray *psaCode, CUIntArray *iaMarkType);
	BOOL SetPortfolioReg(CString strOpenDlg, CStringArray *psaCode);
	void SetSendSurfing(CStringArray *psaCode);
	void SetSaveTempGroup(CStringArray *psaCode);

	CString GetFilePath(CString strLoadType);
	CString GetUserFilePath(CString strOnlyFileName);

	LPCSTR GetScreenKey() { return m_strScreenNo;}
	void _DoGridWidthSet(int nOption);		//0000207

	int GetFillString(CString& _szGetStr, int nStart, int nEnd, int nOneWordSize);
	int SetResultTemp();

public:
	COLORREF	m_clrMainSkin;
	CFont m_fontDefault, m_fontBold, m_fontSmall;
	
	CBrush m_brushBK;
	CString m_strCode;
	CString m_strScreenNo;	//m_szScreenKey

	ST_TABCOLOR m_stTabColor;

	CStringArray m_saDelItemCode; // 그리드의 삭제버튼으로 제외된 종목

private:
	CComboBox		m_comboTarget_1st;
	CComboBox		m_comboTarget_2nd;

	CButton			m_btnReverseChk;
	CButton			m_btnExceptionChk;
	CButton			m_btnIgnoreColorChk;

	int	m_nPeriodCnt;
	CILButton m_btnDay;
	CILButton m_btnWeek;
	CILButton m_btnMonth;
	CILButton m_btnMin;
	CComboBox		m_comboTime;

	CStatic			m_stcStatusBar;

	CStatic			m_ctrlCandleName;
	CStatic			m_ctrlJongName;

	CStatic			m_ctrlTarget; //"검색대상" 텍스트

	CStatic			m_ctrlBongCntText1; //"기준" 텍스트
	CEdit			m_editBongCnt;		// 봉수
	CStatic			m_ctrlBongCntText2; //"봉전" 텍스트

	CButton			m_btnBodyRateChk;
	CStatic			m_ctrlPersent[MAX_CANDLE]; //"%"
	CEdit			m_editUpdownRate[MAX_CANDLE];
	CfxImgButton	m_btnUpdown[MAX_CANDLE];

	CStatic			m_ctrlPrevTrendText; //"완성전" 텍스트
	CComboBox		m_comboPrevTrend; // 완성전 추세
 	CComboBox		m_comboTail;		 // 꼬리 관련 콤보

	CComboBox		m_btnLoad;

	CCandleManager	m_CandleMgr;
	CGrid_Comm		m_gridResult;

	CStatic			m_ctrlTime;
	CStatic			m_ctrlNotice;
	CStatic			m_ctrlState;
	CStatic			m_ctrlCodeCntText;
	CStatic			m_ctrlCodeCnt;

	CListImageWnd	m_ctrlListItem;
	CListImageWnd	m_ctrlUserListItem;

	CfxImgButton	m_btnSave;
	CfxImgButton	m_btnReset;
	CfxImgButton	m_btnSearch;
	CfxImgButton	m_btnPrint;
	CfxImgButton	m_btnFieldEdit;
	CfxImgButton	m_btnResultCtrl;
	CfxImgButton	m_btnExpand;

	CDrTabCtrl*		m_ptabCondition;
	CRect			m_rtTab;

	int m_arCandleData[5];

private:
	int m_nDefult_TargetBaseType;
	int m_nDefult_ChartTerm;
	int m_nDefult_ChartTermMin;
	int m_nDefult_Tail;
	int m_nDefult_AllColorChk;
	int m_nDefult_PreTrend;
	int m_nDefult_BongPass;
	int m_nDefult_ExceptPreferred;

	int m_nMarket;
	BOOL m_bLink;
	BOOL m_bExpand;

	CImageList	m_ImageListItem;
	CImageList	m_ImageUserListItem;
	CImageList	m_ImageListBtn;
	CImageList	m_ImageBK;

	CStringArray m_saCodeMST;
	CStringArray m_saCodeGroup;
	CStringArray m_saCodeTheme;

	CRect m_reTopCandle, m_reTopChart, m_rcMiddle;
	CString m_strItemName; // 그리드에서 선택한 종목명
	CString m_strItemPrice; // 그리드에서 선택한 종목의 가격
	CString m_strStartTime;
	CString m_strEndTime;
	CString m_strVerion;
	CRect m_rcBodyRate[MAX_CANDLE];

	BOOL m_bUpdownState[MAX_CANDLE];
	CString m_strCaption;

public:
	CDlgChart_Stock*    m_pDlgChart;
	IAUTrCommManager*	m_pITrCommManager;
	ITrComm2			*m_pITrComm;
	
public:
	CString m_strDebug;

	int m_nTrIndex;
	CANDLE_IN		m_stCandleIn;

	BOOL SetMakePacket(CANDLE_IN	&stCandleIn);

	long GetSearchTimeTerm();
	int GetBongGap();
	int GetPreTrand();	// 이전 추세 구분 
	int GetIgnoreColor();	// 양음봉 무시
	int GetIgnoreTail();	// 꼬리 무시 

	void SetBtyeOrdering_Packet(CANDLE_IN	&stCandleIn);

	int Modual_SendTr(CString szTrNo, int nTrLen, BYTE *TrData);

	void SetTrData_MSTCode();
	void ConvertMstData(LPCSTR szTR, LPVOID	aTRData, int dwTRDataLen);
	void SetTrData_ThemeGroup();
	BOOL LookUpSearchCenterModify(CString& strType, CString& strUserFileName);
	void SearchUserItem(CString& strFullName);

	CString m_strTargetBaseType;		//# 0 : 전체, 1:Kospi, 2:Kosdak, 3:관심

	CStringArray m_saLimitsCode;
	CStringArray m_saPatternItemFile;

	CString m_strChartCnt;
	CString m_strChartTerm;
	CString m_strDateTime_Select;
	CString GetDateTime();
	void	SetDataWindowData(WPARAM wParam, LPARAM lParam);

	BOOL SetReceiveData_Result(LPTSTR pData, DWORD dwDataLen);
	void SetByteOrderRevDouble(LPSTR pDouble);

	void SetInit();

	void OnSize_Menu(CRect reClient);
	void OnSize_Left(CRect reClient);
	void OnSize_Right(CRect reClient);

	void SetSharedDataToCode(CString strCode);

	void SetStateTitleBar(CString strMsg);
	void SetEnable(BOOL bEnable);
	void MoveControls();

	BOOL			m_bSetControlBase;

	void ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);

	CString m_strAppPath;
	CString m_strUserPatternPath;
	int m_nFixCol;
	CList <HeadInfoType, HeadInfoType&>							m_listField;

	CString m_strFixFieldVersion;
	CList<HeadInfoType, HeadInfoType&> m_listFixHeadSys;
	CList <HeadFixInfo, HeadFixInfo&>  m_listFixHeadUser;
	CList<HeadInfoType, HeadInfoType&> m_listSelectedHead;

	BOOL GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom);
	void SetLoad_FieldInfo();
	void SetGrid_Head(BOOL bView = FALSE);

	CString GetVersion_FieldFile(MSXML::IXMLDOMNodePtr pNIndex);
	BOOL GetLoadFixField_Def(CStringArray &saFieldId);
	BOOL GetLoadFixSysFieldData(MSXML::IXMLDOMNodePtr pNIndex, 
															CList<HeadInfoType, HeadInfoType&> &listFixList);
	BOOL GetLoadFixUserFieldData(MSXML::IXMLDOMNodePtr pNIndex,
															CList<HeadFixInfo, HeadFixInfo&> &listFixList);
	void SetUpdateUserFieldData(CList<HeadInfoType, HeadInfoType&> *plistFixSys, 
																									CList<HeadFixInfo, HeadFixInfo&> *plistFixUser);
	BOOL SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser,
																									CString strVersion);
	BOOL GetLoadSet_Def();
	
	void GetTargetBaseType(int &nTargetBaseType_0st, int &nTargetBaseType_1st);
	CString SetTargetBaseType(int nTargetBaseType_0st, int nTargetBaseType_1st = -1);

	BOOL SetLoad_TargetBase_2nd(CString strTargetBaseType);

	int	SetLoad_CUMaster();
	int SetLoad_CKUMaster();
	BOOL GetData_UpJongToCode(UINT nCode, CString strGroupName);

	int SetLoad_CTHMaster();
	void GetData_ThemaToCode(CString strThemeCode);


	int	SetLoad_PortGroup();
	void GetData_PortGroupToCode(CString szGroupKey, CStringArray *psaData);

	void GetRgn_ExeptButton(CRect rect, CRgn* pRgn);
	void EuBtnClk(UINT nID);
	void SetSearchResultState(LONG lCnt = 0, LONG lTime = -1);

	void SetResultPrint();	// 결과물 프린트
	void SetResultSave();
	void SetSaveGridToExcel(CString strFileName);
	void SetSaveGridToText(CString strFileName);
	void SetSharedDataToSend(ShardData *pstShardData);

	void DrawCandle(CDC* pDC, int cx, int cy, CANDLE_SET* CandleSet);
	void MakeCandleBitmap(CDC* pDC, CBitmap* bitmap, CRect rcBorder, CANDLE_SET* CandleSet);

	CString GetCandlePatternName(int nIndex);
	int Module_SendTr(CString szTrNo, int nTrLen, BYTE *TrData);
	BOOL CheckSavedMstcode();
	void InMakeFid_Mstcode(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos);
	BOOL SetConvert_SendPatternPacket(CANDLE_IN &stCandle_In, HKST03660000_IN1* pstHKST03660000_IN);

	void PlatFormBtn_OnSearh();
	void PlatFormBtn_OnPrint();
	void PlatFormBtn_OnItemEdit();
	void PlatFormBtn_OnItemSend();
	void PlatFormBtn_OnSave();
	void PlatFormBtn_OnInit();
	void OnPF_Contorl_Move();
	
	void GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor);
	void SetFieldEditApply();

	void		SetBtnImg(CfxImgButton *pBtn, CString strFileName);
	void		SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE);
	
	CFont*		GetFont(BOOL bBold = FALSE);
	COLORREF	GetColor(int nColor);
	CString		Variant(int nComm, CString strData = "");
	
	COLORREF	GetAxColor(int nColor);
	HBRUSH		GetAxBrush(COLORREF clr);
	CPen*		GetAxPen(COLORREF clr, int nWidth, int nStyle);
	CWnd*		GetModuleMainWnd();
	
	CString		GetMarketNameToCode(CString strMaketCode);

	void		GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName);
	BOOL		GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey);

public:
	BOOL GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub = 0);
	BOOL GetUpMasterData(CStringArray *&strCode, CStringArray *&strName, int jgub);

protected:
	LRESULT OnUserInfo(WPARAM wp, LPARAM lp); 
	
	IPartnerManager*	m_pPTManager;
	CString				m_szPTManager;
	void DoSetPartnerSite(IPartnerManager* pPtManager);
	
	class CPartnerSite : public IPartnerSite
	{
	public:
		STDMETHOD(RecieveFidData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
		
		STDMETHOD(RecieveTRData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
		
		STDMETHOD(RecieveRealData) (long dwKey, LONG dwData);
		
		STDMETHOD(RecieveKeyData)(int nKey, LPCSTR szData);
		
		STDMETHOD(ReceiveOnUser) (WPARAM wParam, LPARAM lParam);
	} m_xPartnerSite;
	friend class CPartnerSite;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrmWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainFrmWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEdChangeBongCnt();
	afx_msg void OnEdChangeRate1();
	afx_msg void OnEdChangeRate2();
	afx_msg void OnEdChangeRate3();
	afx_msg void OnEdChangeRate4();
	afx_msg void OnEdChangeRate5();
	afx_msg void OnKillfocusBongCnt();
	afx_msg void OnKillfocusRate1();
	afx_msg void OnKillfocusRate2();
	afx_msg void OnKillfocusRate3();
	afx_msg void OnKillfocusRate4();
	afx_msg void OnKillfocusRate5();
	//}}AFX_MSG
	
	afx_msg void OnChgComboTarget1st();
	afx_msg void OnChgComboTarget2nd();

	afx_msg void OnBtnDay();
	afx_msg void OnBtnWeek();
	afx_msg void OnBtnMon();
	afx_msg void OnChgTimeCombo();
	afx_msg void OnBtnMin();
  
	afx_msg void OnBtnChkBodyRate();

	afx_msg void OnBtnSave();
	afx_msg void OnBtnLoad();
	afx_msg void OnBtnReset();
	afx_msg void OnBtnSearch();

	
	afx_msg LRESULT OnNotify_Grid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_CandlePatten(WPARAM wParam, LPARAM lParam);

	afx_msg void OnClickListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickUserListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBtnPrint();	
	afx_msg void OnBtnFieldEdit();
	afx_msg void OnBtnResultCtrl();
	afx_msg void OnBtnResultExpand();
	afx_msg void OnMenuResultCtrl(UINT nIndex);
	afx_msg void OnClickBtnRate(UINT nID);
	afx_msg LONG OnDeleteUserCandle(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnHelpHtml(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnGetTipText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveTranData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveDirectFID(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void SetCommMenu(POINT *point);

	HeadInfoType GetMoveFieldPosInfo();
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = PROGRAM_TITLE, UINT nType = MB_OK);
	int GetMoveFieldCnt();

	void SetPopupMenuResultCtrl(CPoint ptMenu);
	void SetSendMainMsg(CString strMsg);
	CString GetThumbTitle(CString strTitle);

	int			m_nPreReceiveLen;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRMWND_H__A8A882E3_60C6_4076_A588_15227EEB100A__INCLUDED_)
