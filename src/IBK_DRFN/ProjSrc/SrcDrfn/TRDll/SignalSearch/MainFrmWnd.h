#if !defined(AFX_CMAINFRMWND_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
#define AFX_TRMAINVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrMainView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd frame
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <afxmt.h>
#include "resource.h"

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "../../../SrcSite/include/CodeCtrl.h"
#include "./AddControls/Grid_Comm.h"
#include "../../Chart/Chart_Common/ILButton.h"

#include "FieldEditDlg.h"
#include "../../../SrcSite/control/fx_misc/misctype.h"

#include "SignalChart/DlgChart_Stock.h"
#include "../../inc/IAUPartnerMng.h"
#include "../../inc/IAuTrCommMng.h"

class CDlgChart_Stock;
class CSymbol;
class CCompSigData
{
public:
	int nRealType;
	int m_nMarketType;
	CString m_strCode;
	CString m_strDate, m_strTime;
	int m_nPoint;
	
	BOOL CompData(SignalData &sigData)
	{
		if( sigData.nMarket == m_nMarketType &&
			sigData.strCode == m_strCode &&
			sigData.strTime == m_strTime &&
			sigData.nPoint == m_nPoint) return TRUE;
		
		return FALSE;
	}

	void DoCopy(SignalData &sigData)
	{
		m_nMarketType = sigData.nMarket;
		m_strCode = sigData.strCode;
		m_strTime = sigData.strTime;
		m_nPoint = sigData.nPoint;
	}
};

class CMainFrmWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CMainFrmWnd)

public:
	CMainFrmWnd();           // protected constructor used by dynamic creation
	virtual ~CMainFrmWnd();

	HWND			m_hFormMng;
	long			m_lBkColor;
	COLORREF		m_crBackColor;			      // 화면 백그라운드
	HWND			m_hChildFrame;
  
	CString   m_strCode;

	BOOL m_bIsFnUser;

	int         m_nInitFlag;
	CMapStringToOb	*m_MapControlName;	// 화면에 생성된 Control들을 관리하는 Map

	/*IJMComboCtl		m_ctrlJMCombo;*/

	int			m_nGridRow;	  //20080522 김성조 [101]

	CWnd*		m_pWndDummy;

	CWnd*		m_pModuleMainWnd;

private:
	// 0:일반기존타입, 1:유진 현재가 지원
	int			m_nScreenType;
	CFont m_fontDefault, m_fontBold;
	CBrush m_brushStatic;

	CMapStringToPtr m_mapSignalData_Code;
	CMapStringToPtr m_mapSignalData_Type;

	BOOL m_bExp_Left;

	CCodeCtrl*		m_pCodeInput;

	//CStatic       m_ctrlCodeName;
	//CfxImgButton m_btnInit_Code;

	CString		m_strChartTerm;
	CStatic		m_ctrlCodeName_Chart;
	int	m_nPeriodCnt;
	CILButton	m_btnDay;
	CILButton	m_btnWeek;
	CILButton	m_btnMonth;
	CILButton	m_btnMin;
	CILButton	m_btnTick;
	CComboBox	m_comboSelInterval;
	CImageList	m_ImageListBtn;
//	CWnd *m_pctrlChart;
//	CStatic     m_chartCodeName, m_ctrlUnitText;//20100514 김한두 >>

	CComboBox   m_comboSignalType;
	CfxImgButton  m_btnExp_Left;

	/*
	CDrBtnCtrl  m_btnSell;
	CDrBtnCtrl  m_btnBuy;
	*/
//@유진삭제		CBitmapButton   m_btnSell;
//@유진삭제		CBitmapButton   m_btnBuy;

	CfxImgButton  m_btnSignalSet;
	CfxImgButton  m_btnSearch;

	CGrid_Comm  m_gridResult;

	// 신호매니저와 AdviseKey
	LONG	m_dwAdviseKey;
	
	CString	m_strTime;
	
	CButton		m_radioTotal;
	CButton		m_radioCode;
	CButton		m_chkMarketCondition;
	CButton		m_chkTechIndex;
	CButton		m_chkDealAgency;
	CButton		m_chkSys;
	CButton		m_chkNews;
	
	HINSTANCE	m_hSignalMng;
	typedef CWnd*(*_DLL_SetLibOpen)(CWnd* pwndMain, HINSTANCE hInstance, CWnd* pWndModuleWnd);
	typedef void (*_DLL_SetLibClose)(CWnd* pWndTarget);
	typedef int	 (*_DLL_SetSiganl)(BOOL bAdvise, CWnd* pwndMain, CString strType, CString sPBID /* = ""*/, CString strKey/* = "*"*/);
	typedef void (*_DLL_GetSignalData)(CString strType, CMapStringToPtr &mapSignalSet);
	typedef void (*_DLL_SetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet);

	_DLL_SetLibOpen		Lib_Open;
	_DLL_SetLibClose	Lib_Close; 
	_DLL_SetSiganl		Lib_SetSignal;

	CCriticalSection m_CommCritical;

private:
	void SetInit();

	void OnSize();
	void OnSize_Left(CRect reClient);
	void OnSize_Right(CRect reClient);
	int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }

	void SetInit_MainWnd();
	BOOL SetAdvData_Alert_CurrentPrice(BOOL bReg);
	void SetAdvData_Alert(BOOL bReg);

	COLORREF GetPointToColor(int nPoint);
	BOOL GetConvert_RealToSignalData(int nRealType, LPSTR sRealData, SignalData &stSignalData);

	void SetAdd_SignalData(int nType, LPSTR sRealData, int nLen);
	void SetAdd_SignalData(SignalData stSignalData);

	void SetSearch();
	int SetFind_SignalData(BOOL bCode, CString strKey);

	void SetDelAll_SignalData();

	void ChangeGridColor(ST_GRIDCOLOR stGridColor);
	void CreateImageList();

public :
	int	GetScreenType() { return m_nScreenType;	}
		//nType 0:일반, 1:Dialog
	void SetOpenScreen(int nType, CString strMapNo, CString strSendData = "");
	void SetLinkCode(CString strCode, BOOL bTagData = FALSE);
	void SetLinkTime(CString strTime);
	void SetSendMainMsg(CString strMsg);

	void SetSearchData_Chart(CString strCode);
	void SetSearchData_Time(CString strTime);
	void SetSearchData_TimeSet();

	short GetCurSearchType();  //UI변경 되면서 기존 콤보박스 인덱스얻어오는 기능 대신함.

//20080522 김성조 <101
	void SetAdd_SignalNowPrice(STSIGNAL_NOWPRICE  stSignalNowPrice);
	void GetSignal_NowPrice(SIGNAL_CONDITION  stSIGNAL_CONDITION);	

	BOOL SendToEugeneMain(SignalData *pstSignalData);
	BOOL ConvertForEugeneSignalManager(SignalData *pstSignalData, EUGENE_SIGNALMSGINFO *pstEUGENE_SIGNALMSGINFO);
	void ReceiveChartReal(LPCTSTR pszData, UINT nLength, WPARAM wParam);


//	HWND GetKSControlHwnd(LPCSTR strCtrlName) { return (HWND)m_lRunForm.GetControlHwnd(strCtrlName);}
//20080522 김성조 101>

	// 종목명 라벨처리
	HWND	m_hKSJongName;
	CRect	m_rectJongName;
	
	int		m_nState;	//0:All, 1:개별종목
	void	DoBtnInit_Code(BOOL bClear=TRUE);
	void	SetControlDispatch();
	void	DoKSControlMove(LPCSTR szCtrlName, CWnd* pWnd);

	void	Module_SendTr(CString szTRNo);
	
	BOOL		Load_LibSignalMng();
	
	void		SetBtnImg(CfxImgButton *pBtn, CString strFileName);
	void		SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE);
	
	CFont*		GetFont(BOOL bBold = FALSE);
	COLORREF	GetColor(int nColor);
	CString		Variant(int nComm, CString strData = "");
	
	COLORREF	GetAxColor(int nColor);
	HBRUSH		GetAxBrush(COLORREF clr);
	CPen*		GetAxPen(COLORREF clr, int nWidth, int nStyle);
	int			SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = PROGRAM_TITLE, UINT nType = MB_OK);
	void		PutLinkData(CString strType, CString strCode);
	void		ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor);

	// 20111103 박성경: 항목편집 >>
public:
	int		m_nFixCol;
	BOOL	m_bSortUp;
	CString m_strUserPath;
	CString	m_strFixFieldVersion;		//고정필드 사용자 XML file Version
	COLORREF		m_clrBkgnd;
	ST_TABCOLOR		m_stTabColor;
	ST_GRIDCOLOR	m_stGridColor;
	CDrBtnCtrl		m_btnFieldEdit; // 항목편집 버튼
	CfxImgButton	m_btnItemEdit;
	CButton			m_chkSound;				//소리사용
	CString			m_strPathSound;
	CMapStringToString	m_mapResultFieldIndex;
	CList <HeadInfoType, HeadInfoType&>		m_listField;
	CList <HeadFixInfo, HeadFixInfo&>		m_listFixHeadUser;
	CList <HeadInfoType, HeadInfoType&>		m_listFixHeadSys;
	CList<HeadInfoType, HeadInfoType&>		m_listSelectedHead;

	void SetFieldEditApply();
	void SetGrid_Head(BOOL bView = FALSE);
	void SetLoad_FieldInfo();
	BOOL SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser, CString strVersion);
	BOOL GetLoadFixUserFieldData(MSXML::IXMLDOMNodePtr pNIndex,	CList<HeadFixInfo, HeadFixInfo&> &listFixList);
	BOOL GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom);
	BOOL GetLoadFixSysFieldData(MSXML::IXMLDOMNodePtr pNIndex, CList<HeadInfoType, HeadInfoType&> &listFixList);
	CString GetVersion_FieldFile(MSXML::IXMLDOMNodePtr pNIndex);
	void SetUpdateUserFieldData(CList<HeadInfoType, HeadInfoType&> *plistFixSys, CList<HeadFixInfo, HeadFixInfo&> *plistFixUser);
	BOOL GetLoadFixField_Def(CStringArray &saFieldId);
	void GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor);
	int GetCount_FixHead(BOOL &bHaveCodeFiled, CUIntArray &iaFixHeadId);
	CString GetFilePath(CString strLoadType);
	// 20111103 박성경: 항목편집 <<

	BOOL GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub);
	BOOL GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub);
	CString GetMarketNameToCode(CString strMaketCode);

	CDlgChart_Stock*    m_pDlgChart;
	IAUTrCommManager*	m_pITrCommManager;
	ITrComm2		*m_pITrComm;

	void Receive_ChartData(char* pszBuff, long nDataLength);

// 심볼
public:
	HINSTANCE	m_hiSymbol;				//종목명 심벌용
	CWnd*		m_pwndSymbol;
	void		SetInitSymbol();
	void		ParsingSymbol(char* datb);
	void		SendSymbol(CString strCode);
	void		PlaySound();

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

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrmWnd)

	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//}}AFX_VIRTUAL

// Implementation
protected:

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
	//}}AFX_MSG
//	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg void OnBtnInit_Code();
	afx_msg void OnBtnExp_Left();
//@유진삭제		afx_msg void OnBtnSell();
	afx_msg void OnChangeSignalType();

	afx_msg void OnBtnSignalSet();
	afx_msg void OnBtnSearch();

	afx_msg void OnMouseHover();

	//afx_msg long OnGetData(WPARAM wParam, LPARAM lParam);
	afx_msg long OnGetBrod(WPARAM wParam, LPARAM lParam);

	afx_msg long OnNotify_PortPolio(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRadioBtnClk(UINT nID);
	afx_msg void OnChkBtnClk(UINT nID);

	afx_msg void OnBtnFieldEdit();

	DECLARE_MESSAGE_MAP()
//@유진삭제	afx_msg void OnBtnSell();

	afx_msg void OnCodeEnter();
	afx_msg void OnCodeEnter_Chart();
	afx_msg void OnFormInitAfter();
	afx_msg void OnFormGetPageSave(LPCTSTR strFileName);	//메인 저장화면
	afx_msg void OnFormSetPageSave(LPCTSTR strFileName);	//메인 저장화면
	afx_msg LRESULT OnRMSGColorChange();	//메인의 스킨변경시 불린다.
	afx_msg void OnReceiveDataAfter(LPCTSTR strTrCode, VARIANT FAR* strVarOutInfo, VARIANT FAR* nVarOutInfo, VARIANT FAR* nVarRetValue, VARIANT FAR* nVarRetOption, long nDataCount);
	afx_msg	void OnEuBtnClk(UINT nID);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnBtnDay();
	afx_msg void OnBtnWeek();
	afx_msg void OnBtnMonth();
	afx_msg void OnBtnMinute();
	afx_msg void OnBtnTick();
	afx_msg void OnChangeInterval();

	// 사이징처리관련 추가
	CWnd* GetFrameWnd(CWnd* pWnd);
	void DoDllSizeEx(HWND _hChildFrame=NULL);
	

	void SetCommMenu(POINT *point);
	void OnMenuResultCtrl(UINT nIndex);

//20100503 김한두 >>
public:

	HRESULT ReceiveData( LPCSTR szTR, LPVOID aTRData, long dwTRDateLen);

	//CChartItem_Stock m_Chart;
	CString m_strAppPath;

	void SendTr_ChartData(CString strCode, CString strDate);// 차트조회..
	void SetInitControl();
//20100503 김한두 <<

	CCompSigData	m_oldSigData;

	void SetPopupMenuResultCtrl(CPoint &ptMenu);
	int  SetResultSearchTemp();

	void SetChangeCode(CString strCode);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TR1111VIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
