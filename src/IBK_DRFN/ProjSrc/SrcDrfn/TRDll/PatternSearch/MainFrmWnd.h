#if !defined(AFX_MAINFRMWND_H__A8A882E3_60C6_4076_A588_15227EEB100A__INCLUDED_)
#define AFX_MAINFRMWND_H__A8A882E3_60C6_4076_A588_15227EEB100A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainFrmWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd frame

#include "AddControls/Grid_Comm.h"
#include "AddControls/ListImageWnd.h"
//#include "AddControls/DrBtnCtrl.h"
//#include "../../../../Common\KHResource.h"

#define CNT_DRAWITEM			20

#include "../../Chart/Chart_Common/ILButton.h"
#include "../../DrCommon/Dr_Control/DrTabCtrl.h"
#include "../../DrCommon/Dr_Control/DrCombo.h"
#include "../../DrCommon/Dr_Control/DrBtnCtrl.h"

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "../../../SrcSite/control/fx_misc/misctype.h"
#include "../../DrCommon/Dr_Control/DrTabCtrl.h"

/*#include "../../inc/ExOCXDLL.h"*/

#include "Definefiles/Tr_Theme.h"
#include "FixWaitCursor.h"

#include "./PatternChart/DlgChart_Stock.h"
#include "../../inc/IAUPartnerMng.h"
#include "../../inc/IAuTrCommMng.h"

typedef struct
{
	CStringArray strKey;
	CStringArray strGroupName;
} ST_GwanSimGroup;

class CDlgChart_Stock;
class CDrawPattenCtrl;
class CFieldEditDlg;
class CMainFrmWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CMainFrmWnd)
public:
	CMainFrmWnd();           // protected constructor used by dynamic creation
	virtual ~CMainFrmWnd();
	HRESULT ReceiveData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen, int nKey = 0 );
	HRESULT StatusChange(int nState) ;

	void	SendTr_ChartData(CString strCode, CString strDate);
	int ParseGroup(char* pReceiveBuff, CStringArray& arrKey, CStringArray& arrName);
	void Request_PortGroup();
private:
	CFixWaitCursor m_WaitCursor;
	ST_GwanSimGroup m_GwanSimGroup;
	
public:

	/*ITreeListManager2*	m_pTreeListMng;*/

	CFont m_fontDefault, m_fontBold;
	

	CBrush m_brushBK;
	CString m_strCode;
	CString m_strScreenNo;
	int		m_nThemeGCount, m_nThemeICount;
	COLORREF	m_clrMainSkin;
	
public:
//	IJMComboCtl*	m_pctrlCodeInput;

// 	CILButton m_btnTotal;
// 	CILButton m_btnKospi;
// 	CILButton m_btnKosdak;
	CDrCombo m_comboTarget_1st;
	CDrCombo m_comboTarget_2nd;

	CButton	  m_btnReverseChk;
	CButton	  m_btnExceptionChk;

	int	m_nPeriodCnt;
	CILButton m_btnDay;
	CILButton m_btnWeek;
	CILButton m_btnMonth;
	CILButton m_btnMin;
	CDrCombo  m_comboTime;

	CStatic m_stcStatusBar;

	CStatic	m_ctrlPatternName;
	CStatic	m_ctrlJongName;
	CStatic	m_ctrlSimilar;

	CStatic	m_ctrlTarget; //"검색대상" 텍스트
	CStatic	m_ctrlPeriod; //"검색기간" 텍스트
	CStatic	m_ctrlSimilarity; //"유사도" 텍스트
	
	CGrid_Comm m_gridResult;

	CDrBtnCtrl m_btnLoad;
	
	CDrawPattenCtrl	*m_pctrlDrawPatten;
	
//	CStatic		m_ctrlSliderVal;
	CSliderCtrl m_ctrlSlider;
	CButton		m_btnBongChk;
	CDrCombo	m_comboBongTerm;
	CDrCombo	m_comboSimilar;
	CEdit	m_editPeriod;		// 기간ㄴ

	/*CChartItem_Stock m_Chart;*/

	CStatic		m_ctrlTime;
	CStatic		m_ctrlNotice;
	CStatic		m_ctrlState;
	CStatic		m_ctrlCodeCntText;
	CStatic		m_ctrlCodeCnt;

	CListImageWnd	m_ctrlListItem;
	CListImageWnd	m_ctrlUserListItem;

	CfxImgButton	m_btnSave;
	CfxImgButton	m_btnReset;
	CfxImgButton	m_btnSearch;
	CfxImgButton	m_btnPrint;
	CfxImgButton	m_btnFieldEdit;
	CfxImgButton	m_btnResultCtrl;
	CfxImgButton	m_btnExpand;
	HeadFixInfo	m_stAlike;

private:
	int m_nDefult_TargetBaseType;
	int m_nDefult_Slider;
	int m_nDefult_BongTerm;
	int m_nDefult_BongTerm_User;
	int m_nDefult_Similar;
	int m_nDefult_ChartTerm;
	int m_nDefult_ChartTermMin;
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

	CRect m_reTopPattern, m_reTopChart, m_rcMiddle;
	CString m_strItemName; // 그리드에서 선택한 종목명
	CString m_strItemRight; // 그리드에서 선택한 종목의 정보

	CString m_strCaption;
	
	int m_nPrevScale;

	COLORREF	m_clrBkgnd;
	ST_TABCOLOR	m_stTabColor;
	int m_arCandleData[5];

	CDlgChart_Stock*    m_pDlgChart;
	IAUTrCommManager*	m_pITrCommManager;
	ITrComm2			*m_pITrComm;

private:
	void MakeUserPatternList();
	void MakeUserPatternImageList();
	BOOL MakePatternBitmap(CDC* pDC, CBitmap* bitmap, CRect rcBorder, CString strPatternFilename);
	void DrawPattern(CDC* pDC, CRect rcCanvas, CPtrArray *paData);
	void DrawGradientRect(CDC *pDC, CRect rect, COLORREF Start, COLORREF End, BOOL bTopDown=TRUE);
	BOOL LoadPatternData(CString strFile, CRect& rcCanvas, CPtrArray *paDrawData);

	void CreatePatternDrawCtrl();
	void CreateImageList();
	void CreateButton();
	void CreateStatic();
	void CreateCombo();
	void CreateMisc();

	void GetSimilarityFromGrid();
	CString GetFilePath(CString strLoadType);
	CString GetUserFilePath(CString strOnlyFileName);

	BOOL SetTempGroupReg(CStringArray *psaCode, CUIntArray *iaMarkType);
	BOOL SetPortfolioReg(CString strOpenDlg, CStringArray *psaCode);
	void SetSendSurfing(CStringArray *psaCode);
	void SetSaveTempGroup(CStringArray *psaCode);

	void _DoGridWidthSet(int nOption);		//0000207
	int	 GetFillString(CString& _szGetStr, int nStart, int nEnd, int nOneWordSize);

	int SetResultTemp();

public:

	int m_nTrIndex;
	DRAW_IN		m_stDrawIn;

	BOOL SetMakePacket(DRAW_IN	&stDrawIn);
	BOOL SetConvert_SendPatternPacket(DRAW_IN &stDrawIn, p0624InBlock* pst0624);
	long GetSearchTimeTerm();
	void SetBtyeOrdering_Packet(DRAW_IN	&stDrawIn);

	void InMakeFid_Packet(int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos,
									int nOut1=0, int nOut2=0);

	void SetTrData_MSTCode();
	void SetTrData_ThemeGroup();
	BOOL LookUpSearchCenterModify(CString& strType, CString& strUserFileName);
	void SearchUserItem(CString& strFullName);

	CString m_strTargetBaseType;		//# 0 : 전체, 1:Kospi, 2:Kosdak, 3:관심

	CStringArray m_saLimitsCode;
	CStringArray m_saPatternItemFile;
	CStringArray m_saUserPatternFiles;
	CStringArray m_saPatternItemTip;

	CString m_strChartCnt;
	CString m_strChartTerm;
	CString m_strDateTime_Select;
	
	CString GetDateTime();
	int Module_SendTr(CString szTrNo, int nTrLen, BYTE *TrData);
	BOOL CheckSavedMstcode();
	void SetConvert_ReceiveThemeItem(CString strData, int nItemCount);
	void ConvertMstData(LPCSTR szTR, LPVOID	aTRData, int dwTRDataLen);
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
	void InMakeFid_Mstcode(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos);


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
	void SetUpdateUserFieldData(CList<HeadInfoType, HeadInfoType&> *plistFixSys, CList<HeadFixInfo, HeadFixInfo&> *plistFixUser);
	BOOL SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser, CString strVersion);

	BOOL GetLoad_PatternItem(CStringArray &saPatternItem);
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

	void ChangeSkinColor();
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);
	void GetRgn_ExeptButton(CRect rect, CRgn* pRgn);
	void SetSearchResultState(LONG lCnt = 0, LONG lTime = -1);

	void SetResultPrint();	// 결과물 프린트
	void SetResultSave();
	void SetSaveGridToExcel(CString strFileName);
	void SetSaveGridToText(CString strFileName);
	void SetSharedDataToSend(ShardData *pstShardData);
	void ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor);

	void PlatFormBtn_OnSearh();
	void PlatFormBtn_OnPrint();
	void PlatFormBtn_OnItemEdit();
	void PlatFormBtn_OnItemSend();
	void PlatFormBtn_OnSave();
	void PlatFormBtn_OnInit();
	void OnPF_Contorl_Move();

	void SetFieldEditApply();

	void		SetBtnImg(CfxImgButton *pBtn, CString strFileName);
	void		SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE);
	
	CFont*		GetFont(BOOL bBold = FALSE);
	COLORREF	GetColor(int nColor);
	CString		Variant(int nComm, CString strData = "");
	
	COLORREF	GetAxColor(int nColor);
	HBRUSH		GetAxBrush(COLORREF clr);
	CPen*		GetAxPen(COLORREF clr, int nWidth, int nStyle);
	
	CString		GetFormCode();
	CWnd*		GetModuleMainWnd();

// 	ICtlBase*		m_pIbtnSearch;
// 	ICtlBase*		m_pIBtnPrint;
// 	ICtlBase*		m_pIBtnFeildEdit;
// 	ICtlBase*		m_pIBtnSend;
// 	ICtlBase*		m_pIBtnSave;
// 	ICtlBase*		m_pIBtnInit;
	BOOL			m_bSetControlBase;

// 	void SetControlBase(ICtlBase* _pBtnSearch,ICtlBase* _pBtnPrint, 
// 		ICtlBase*	_pBtnFeildEdit, ICtlBase* _pBtnSend, ICtlBase*	_pIBtnSave, ICtlBase*	m_pIBtnInit);

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
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnEdChangeScaleCnt();
	
	afx_msg void OnChgComboTarget1st();
	afx_msg void OnChgComboTarget2nd();

	afx_msg LONG OnDeleteUserItem(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBtnDay();
	afx_msg void OnBtnWeek();
	afx_msg void OnBtnMon();
	afx_msg void OnBtnMin();
  	afx_msg void OnBtnResultExpand();


	afx_msg void OnBtnSave();
	afx_msg void OnBtnLoad();
	afx_msg void OnBtnReset();
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnChkBong();
	afx_msg void OnChgBongCombo();
	afx_msg void OnChgSimCombo();

	
	afx_msg LRESULT OnNotify_Grid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_DrawPatten(WPARAM wParam, LPARAM lParam);

	afx_msg void OnClickListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickUserListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBtnPrint();	
	afx_msg void OnBtnFieldEdit();
	afx_msg void OnBtnResultCtrl();
	afx_msg void OnMenuResultCtrl(UINT nIndex);
	afx_msg LONG OnGetTipText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveTranData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveDirectFID(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
//유진
public:
	void DispatchCodeInput(COleDispatchDriver* pDispatch);
//	class CKSTRDllGateSite : public IKSTRDllGateSite
//	{
//	public:
//		STDMETHOD_(void, OncmbJCodeSelChanged)(LPCSTR strMarket, LPCSTR strCode, LPCSTR strName);
//		STDMETHOD_(void, SetCodeDragAndDrop)(CString strCode);
//		STDMETHOD_(long ,OnChangeChildFrameWindowHandle)( WPARAM wParam, LPARAM lParam) ;
//		STDMETHOD_(void , OnEuBtnClk)(UINT nIdIndex);
//	} m_xKSTRDllGateSite;
//	friend class CKSTRDllGateSite;	

	void SetCommMenu(POINT *point);

	CDrTabCtrl	*m_ptabCondition;
	CRect	m_rtTab;

	HeadInfoType GetMoveFieldPosInfo();
//	CString GetFilePath(CString strLoadType);
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = PROGRAM_TITLE, UINT nType = MB_OK);

	int GetMoveFieldCnt();
	BOOL SetSearch(BOOL bFirstSearch = TRUE);
	void SetPopupMenuResultCtrl(CPoint ptMenu);
	void SetSendMainMsg(CString strMsg);

	void GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor);

	int			m_nPreReceiveLen;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRMWND_H__A8A882E3_60C6_4076_A588_15227EEB100A__INCLUDED_)
