#if !defined(AFX_TRMAINVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
#define AFX_TRMAINVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrMainView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "resource.h"

#include "../../Chart/Chart_Common/ILButton.h"
#include "./AddControls/Grid_Comm.h"
#include "../../DrCommon/Dr_Control/DrBtnCtrl.h"
#include "../../DrCommon/Dr_Control/DrCombo.h"

#include "Definefiles/Tr_Theme.h"
#include "FixWaitCursor.h"

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "../../../SrcSite/control/fx_misc/misctype.h"
#include "../../../SrcSite/include/CodeCtrl.h"

#include "../../Chart/Chart_Common/OutsideCommonInterface.h"

#include "../../inc/IAUPartnerMng.h"
#include "../../inc/IAuTrCommMng.h"

typedef struct
{
	CStringArray strKey;
	CStringArray strGroupName;
} ST_GwanSimGroup;

class CDlgChart_Stock;
class CFieldEditDlg;
class CSymbol;
class CScreenWnd;
class CMainFrmWnd : public CFrameWnd, public CMChartInterface
{
	DECLARE_DYNCREATE(CMainFrmWnd)
public:
	CMainFrmWnd();           // protected constructor used by dynamic creation
	virtual ~CMainFrmWnd();
	HRESULT ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen , int nKey  = 0 );
	int ParseGroup(char* pReceiveBuff, CStringArray& arrKey, CStringArray& arrName);
	void Request_PortGroup();
private:
	ST_GwanSimGroup m_GwanSimGroup;
	CBrush m_brushBK;
	CFixWaitCursor m_WaitCursor;

	BOOL bFirstClk;

	void CreateButton();
	void CreateCombo();
	void CreateStatic();
	void CreateEdit();
	void CreateMisc();

	BOOL SetTempGroupReg(CStringArray *psaCode, CUIntArray *iaMarkType);
	BOOL SetPortfolioReg(CString strOpenDlg, CStringArray *psaCode);
	void SetSendSurfing(CStringArray *psaCode);
	void SetSaveTempGroup(CStringArray *psaCode);

	int SetResultTemp();

public:
	COLORREF		m_clrMainSkin;
	ST_TABCOLOR		m_stTabColor;

	CButton			m_RadioPattern;
	CButton			m_RadioCandle;
	
	LONG			m_dwFormOpenManagerKey;
	
	CString			m_strTargetBaseType;//# 0:전체, 1: Kospi, 2:Kosdaq
										//# Sub -> 0:전체, 1:업종", "테마", "관심종목", "유니버스"
	CString			m_strScreenNo;		//CString		m_szScreenKey;

	CStringArray	m_saCodeMST;
	CStringArray	m_saCodeGroup;
	CStringArray	m_saCodeTheme;
	
	BOOL SetLoad_TargetBase_2nd(CString strTargetBaseType);

	int	SetLoad_CUMaster();		//코스피 업종
	int SetLoad_CKUMaster();    //코스닥 업종
	BOOL SetTr_CKUMaster(CString szGroupName);//코스닥, 코스피 종목
	
	int SetLoad_CTHMaster();    //테마 업종
	BOOL SetTr_CTHMaster(CString szGroupName);//테마 종목

	int	SetLoad_PortGroup();
	void GetData_PortGroupToCode(CString szGroupKey, CStringArray *psaData);

	void GetTargetBaseType(int &nTargetBaseType_0st, int &nTargetBaseType_1st);
	CString SetTargetBaseType(int nTargetBaseType_0st, int nTargetBaseType_1st=-1);

	void InMakeFid_Mstcode(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos);
	BOOL SetConvert_SendPatternPacket(DRAW_IN &stDrawIn, p0624InBlock* pst0624);
	BOOL SetConvert_SendPatternPacket(CANDLE_IN &stCandle_In, HKST03660000_IN1* pstHKST03660000_IN);

	void LoadUserData();
	void SaveUserData(CHARTSECTIONINFO* pUserInfo);
	BOOL ReadXML(CString& strUserFile, CHARTSECTIONINFO* pUserInfo);
	void ClearUserData();
	void DeleteUserData(CString& strItemName);
	void DeleteAllUserData();
	BOOL GetFindFile(CString& strFileName);
	
	CCodeCtrl*		m_pCodeInput;
	BOOL m_bSetControlBase;

	void OnBtnClickSearch();
	void OnBtnClickPrint();
	void OnBtnClickFieldEdit();
	void OnBtnClickTargetSend();
	void OnBtnClickSave();

	CString		GetMarketNameToCode(CString strMaketCode);
	void ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);
	void ProcessSymbolData(WPARAM wParam, LPARAM lParam);

private:
	int		m_nTrIndex;			
	int	   	m_nLastTrIndex;
	int		m_nMarket;	
	BOOL	m_bExpand;

	int		m_nDefult_TargetBaseType;
	int		m_nDefult_BongTerm;
	int		m_nDefult_BongTerm_User;
	int		m_nDefult_Similar;
	int		m_nDefult_ChartTerm;
	int		m_nDefult_ChartTermMin;
	int		m_nDefult_Tail;
	int		m_nDefult_AllColorChk;
	int		m_nDefult_PreTrend;
	int		m_nDefult_BongPass;
	int		m_nDefult_ExceptPreferred;
	
	CList<CString, CString&> m_SectionDataList;

	CString  m_strSectionDate;//선택 구간 시간 
//	char	 m_chType;       //일, 주 , 월, 분
	CString  m_interval;     //틱, 분, 일 간격
	short	 m_chartActivation; //0: 메인차트(WindowProc), 1:Section차트(CGrid_Comm::OnLClicked)
	
	CFont  m_fontDefault, m_fontBold; // 폰트

	CDrCombo	m_comboTarget_1st;
	CDrCombo	m_comboTarget_2nd;

	CButton		m_btnReverseChk;
	CButton		m_btnExceptionChk;
	CButton		m_btnIgnoreColorChk;

	CButton		m_btnBongChk;	// 
	CDrCombo	m_comboBongTerm;
	CDrCombo	m_comboSimilar;
 	CDrCombo	m_comboTail;		 // 꼬리 관련 콤보

	int	m_nPeriodCnt;

	CILButton	m_btnDay;
	CILButton	m_btnWeek;
	CILButton	m_btnMonth;
	CILButton	m_btnMin;
	CILButton	m_btnRequestMore; // 데이터 추가부르기
	CEdit		m_edtZoomValue;
	CEdit		m_editCntOfDatAll;
	CSpinButtonCtrl m_spinZoom;

	CDrCombo	m_comboTime;

	CStatic		m_stcStatusBar;
	CStatic		m_ctrlCodeName;
	CStatic		m_ctrlCodeCntText;
	CStatic		m_ctrlCodeCnt;
	CStatic		m_ctrlTarget; //"대상" 텍스트
	CStatic		m_ctrlSimilarity; //"유사도" 텍스트

// 	CStatic		m_ctrlChartCenter;
// 	CStatic		m_ctrlChartZoomIn;
	CStatic		m_ctrlBongCntText1; // "기준" 텍스트
	CEdit		m_editBongCnt;		// 봉수
	CStatic		m_ctrlBongCntText2; // "봉전" 텍스트
	CStatic		m_ctrlSlash; // "/" 텍스트

	CGrid_Comm	m_gridResult;
	CStatic		m_ctrlTime;
	CStatic		m_ctrlNotice;
	CStatic		m_ctrlState;

	CfxImgButton	m_btnPrint;
	CfxImgButton	m_btnFieldEdit;
	CfxImgButton	m_btnResultCtrl;
	CfxImgButton	m_btnExpand;
	CfxImgButton	m_btnSearch;

	CImageList	m_ImageListBtn;
	CRect		m_rcRdFrame;

	ArrChartSectionInfo		m_UserData;
	CString					m_strSectionStartDate;
	CString					m_strSectionEndDate;
	BOOL					m_bModifyFlag;		// 서치센터에서 오픈시 TRUE

	int m_arCandleData[5];
	int m_nSeparateChart;

	CString m_strCode;

private:
	CDlgChart_Stock*    m_pDlgChart;    	 //메인 차트 포인터   CenterChart
	CDlgChart_Stock*	m_pDlgChartSecond;   //검색된 차트 포인터
	CList<CString, CString&> m_ListChartData;

	LPCTSTR			GetDateTime(WPARAM wParam, LPARAM lParam);
	void			SetDataWindowData(CList<CString, CString&>* pListData);	//차트에서 종가 데이터 얻어옴

	void _DoClearOldData();	//기존 데이터 clear

	IAUTrCommManager*	m_pITrCommManager;
	ITrComm2		*m_pITrComm;

	CScreenWnd* m_pScreenWnd;
private:

	void OnSize();
	int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }
	
	CString GetCodeToName(CString strCode);

	void SetTrData_MSTCode();
	void SetTrData_ThemeGroup();
	void Receive_ChartData(char* pszBuff, long nDataLength);		//메인 차트
	void Receive_ChartData_Second(char* pszBuff, long nDataLength);	//검색된 차트
	BOOL SetMakePatternPacket(DRAW_IN	&stDrawIn, CList<CString, CString&>* pListChartData); //구간 데이터 
	BOOL SetMakeCandlePacket(CANDLE_IN	&stCandleIn, CList<CString, CString&>* pListChartData); //구간 데이터 
	long GetData_DrawPacket(int nTot, long* pDrawData);			    //종가->비율값 전환

	void SetSearchResultState(LONG lCnt = 0, LONG lTime = -1);
	void SetResultPrint();	// 결과물 프린트
	void SetResultSave();
	void SetSaveGridToExcel(CString strFileName);
	void SetSaveGridToText(CString strFileName);
	void SetSharedDataToSend(ShardData *pstShardData);

	void CreateImageList();

	BOOL MsgFunc_KeyDownZoomValue();
	BOOL MsgFunc_KeyDownCntOfDateAll();

	CString GetFilePath(CString strLoadType);
	CString GetUserFilePath(CString strOnlyFileName);

	LPCSTR GetScreenKey() { return m_strScreenNo;}
	void _DoGridWidthSet(int nOption);		//0000207
	int GetFillString(CString& _szGetStr, int nStart, int nEnd, int nOneWordSize);

public :

	void SetInit();
	void SetEnable(BOOL bEnable);
	void CreateDlgChart();  //차트 다이얼로그 생성
	void DestroyChart();
	int GetTrIndex();
	int GetBongGap();
	int GetIgnoreColor();	// 양음봉 무시
	int GetIgnoreTail();	// 꼬리옵션

	void SetOpenScreen(CString strMapNo, CString strSendData = "");
	void SetLinkCode(CString strCode, BOOL bTagData = FALSE);

	CString m_strAppPath;
	int		m_nFixCol;
	CList   <HeadInfoType, HeadInfoType&>							m_listField;

	CString m_strFixFieldVersion;
	CList   <HeadInfoType, HeadInfoType&> m_listFixHeadSys;
	CList   <HeadFixInfo, HeadFixInfo&>  m_listFixHeadUser;
	CList   <HeadInfoType, HeadInfoType&> m_listSelectedHead;

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

	void GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor);
	void SaveUserCondition();
	ArrChartSectionInfo* GetUserData()		{ return &m_UserData; }

	void LoadModifyFile(CString& strCode);
	BOOL LookUpSearchCenterModify(CString& strType, CString& strUserFileName);
	void SearchUserItem(CString& strCode, CString& strFullName);
	void SearchUserItem(CHARTSECTIONINFO* pInfo);

	void		SetBtnImg(CfxImgButton *pBtn, CString strFileName);
	void		SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE);
	
	CFont*		GetFont(BOOL bBold = FALSE);
	COLORREF	GetColor(int nColor);
	CString		Variant(int nComm, CString strData = "");
	
	COLORREF	GetAxColor(int nColor);
	HBRUSH		GetAxBrush(COLORREF clr);
	CPen*		GetAxPen(COLORREF clr, int nWidth, int nStyle);
	CWnd*		GetModuleMainWnd();

	void		GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName);
	BOOL		GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey);

public:
	//{{AFX_DATA(CMainFrmWnd)
	enum { IDD = IDD_STD_DIALOG };	
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	// Attributes
private:
	int m_nTabIndex;
	BOOL m_bLink;
	int m_nCntInView;		//차트의 눈에 보이는 가시권내 데이터 갯수. Default - 140
	int m_nAllCntInView;	//차트 데이터 전체 갯수.
	int m_nRestrictedMaxCnt;//차트 조회된 전체 갯수(요청한 갯수 보다 적게 올 경우)

	CString m_strStartTime;
	CString m_strEndTime;
	CString m_strChartTerm;
	
	// Operations
public:
	void GetData_ThemaToCode(CString strThemeCode);
	BOOL GetData_UpJongToCode(UINT nCode, CString strGroupName);

	void SendTr_ChartData(CString strCode);
	void SendTr_PatternData(CList<CString, CString&>* pListData);
	void Set_SectionDate(CString strDate);

	void SendTr_CandleData(CList<CString, CString&>* pListData);
	void ResendTR();

	void SetChartActivation(int chart);
	
	void SetByteOrdering_PatternIn(DRAW_IN *pstDraw_IN);
	void SetBtyeOrdering_CandleIn(CANDLE_IN	&stCandleIn);
	void SetByteOrderRevDouble(LPSTR pDouble);

	BOOL SetReceiveData_Pattern(LPTSTR pData, DWORD dwDataLen);   //구간 데이터
	BOOL SetReceiveData_Candle(LPTSTR pData, DWORD dwDataLen); // 캔들 검색 결과

	void SetSharedDataToCode(CString strCode);
	long GetSearchTimeTerm();

	BOOL OnPF_Contorl_Move();
	void SetFieldEditApply();

public:
	BOOL GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub = 0);
	BOOL GetUpMasterData(CStringArray *&strCode, CStringArray *&strName, int jgub);

	void SetChangeCode(CString strCode);

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

// 심볼
public:
	HINSTANCE	m_hiSymbol;				//종목명 심벌용
	CWnd*		m_pwndSymbol;
	void		SetInitSymbol();
	void		ParsingSymbol(char* datb);
	void		SendSymbol(CString strCode);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrmWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	// Generated message map functions
	//{{AFX_MSG(CMainFrmWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEdChangeBongCnt();
	afx_msg void OnEdKillfocusBongCnt();
	//}}AFX_MSG
	afx_msg void OnChgComboTarget1st();
	afx_msg void OnChgComboTarget2nd();
	afx_msg void OnDeltaposSpinzoomvalue(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBtnSearch();
	afx_msg void OnBtnDataMore();
	afx_msg void OnBtnDay();
	afx_msg void OnBtnWeek();
	afx_msg void OnBtnMonth();
	afx_msg void OnBtnMinute();
	afx_msg void OnPeriodSelection();
	afx_msg void OnBtnChkBong();
	afx_msg void OnChgBongCombo();
	afx_msg void OnChgSimCombo();
	
	afx_msg void OnRadioPattern();
	afx_msg void OnRadioCandle();

	afx_msg void OnBtnPrint();	
	afx_msg void OnBtnFieldEdit();
	afx_msg void OnBtnResultCtrl();
	afx_msg void OnBtnResultExpand();
	afx_msg void OnMenuResultCtrl(UINT nIndex);
	afx_msg void OnMenuConditionSave(UINT nIndex);

	afx_msg void OnChangeCode();

	afx_msg LRESULT OnNotify_Grid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetZoomValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveTranData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveDirectFID(WPARAM wParam, LPARAM lParam);
	LRESULT OnEventMessage( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	int SetMessageBox(LPCTSTR lpszText, UINT nType = MB_OK, LPCTSTR lpszCaption = PROGRAM_TITLE);
	void SetPopupMenuResultCtrl(CPoint ptMenu);
	void SetSendMainMsg(CString strMsg);

	int			m_nPreReceiveLen;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TR1111VIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
