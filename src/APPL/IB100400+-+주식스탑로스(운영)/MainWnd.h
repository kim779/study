#pragma once
// MainWnd.h : header file
//
#include "LogDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
typedef  struct  mid {
    char    func[1];        /* 'Q' : 조회 'I' 입력  */
    char    mapn[8];        /* 화면번호 */            
    char    kind[1];        /* 로그종류 */            
		            /* 화면별 세부구분 */     
    char    data[512];      /* 로그 데이타(MAX 1024) */
} pidologm_mid;

struct _List
{
	CRect	rect;
	int		icol{};
	int		itoggleIdx{};
	CString	sHeader;

	_List(CRect rc, int icol1, int icol2)
	{
		rect = rc;
		icol = icol1;
		itoggleIdx = icol2;
		sHeader = "";
	};

	_List()
	{
		itoggleIdx = -1;
	};
};
typedef CArray<_List, _List> C_ListArray;

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd*	pAxWnd);

// Attributes
public:
	CWnd*								m_pAxMain;
	CWnd*								m_pQueryModalWnd;
	std::unique_ptr<CLogDlg>			m_pLogDlg;

	CString								m_strUserID, m_strDeptCode, m_strPass, m_strRootDir, m_strImgPath, m_strLogPath, m_strUserName;
	COLORREF							m_clrConditionColor[3], m_bgcolor, m_toolcolor, m_gridhdrbgcolor, m_gridLinecolor, m_gridRowColor[2], 
										m_gridhdrtxtcolor, m_txtcolor, m_panelcolor[2], m_gridFocuscolor;
	COLORREF							m_lbllinecolor,  m_lblbackcolor, m_realcolor, m_bodycolor, m_guidecolor, m_guideTxtcolor; 
	COLORREF							m_MaedoColor[5], m_MaesuColor[5], m_DangaColor[2];

	CString								m_strLedger;
	CFont*								m_pFont;

	_gridHdr							m_GridHdr[JCOLCOUNT+1];

	CStringArray						m_arrlog;
protected:
	CCriticalSection					m_section;
	std::unique_ptr<CAccountCtrl>		m_pAccnCtrl;
	std::unique_ptr<CMxFlatEdit>		m_pPassEdit;
	std::unique_ptr<CButton>			m_pAutoBtn;
	std::unique_ptr<CfxImgButton>		m_pToolBar[7];
	std::unique_ptr<CfxGrid>			m_pRemainGrid;
	std::unique_ptr<CButton>			m_pTestBtn;
	std::unique_ptr<CComboBox>			m_pTestCbo;
	CString								m_strGuideMessage;
	CRect								m_pClientRect;

	std::unique_ptr<CButton>			m_pConfigBtn;


	CRect								m_pToolLblRect[5], m_pAccnNameRect;/*계좌,비밀번호,주문확인창,반자동*/
	CString								m_strToolLblList[5], m_strAccountName;
	UINT								m_LblTxtFormat[5];
	CStringArray						m_arGridHdrList;
	BOOL								m_bClose, m_bFirst;


	CArray<_config, _config>			m_arConfigList;
	C_ListArray							m_arGridToggleList;
	_config								m_stAllconfig;

	int									m_iSelectRow;
	CString								m_strNextKey;
	BOOL								m_bNextList, m_bExecMode, m_bDebugMode;
	CString								m_strDebugTxt[10];

	
	BOOL								m_bInit, m_bChangeFont;
	int									m_iOldFont, m_iCurFont, m_iDefaultFont, m_iDefaultSize[2], m_iOldSize[2];
	int									m_igridRowHeight, m_iGuideHeight, m_iGridHeadrHeight, m_iToolheight;
	double								m_lSizeRate[2];
	
	HBITMAP								m_hRoundBitmap[4], m_hGuideBitmap;
	CRect								m_pContentRect;
	int									m_nLogCount;
	HINSTANCE							m_hiSha256;	//2013.12.23 KSJ 일방향암호화
	CString								m_nSJJS;


// Operations
public:
	LRESULT								SendAxisMessage(WPARAM wParam, LPARAM lPAram);
	HBITMAP								getBitmap(CString path);
	int									getCodeType(CString strcode);
	CFont*								getAxFont(CString fName=_T("굴림"), int point=9, bool bBold=false, bool bItalic=false);


	void								SetInitValue();
	void								LoadConfigValue();
	void								SaveConfigValue();
	void								CreateControl();
	CString								GetLedger(CString svcd, CString strAccount, char codrf='1', CString sNext=_T(""), int iRowCnt=10, CString sPswd="");	//2013.12.23 KSJ 일방향암호화
	CString								GetCode(CString sRCode);

	void								SetGuideMessage(CString sMessage);
	void								SendCodeTrigger(int irow);
	int									SendTransaction(CString strTrCode, CString strData, char cstate, char ckey);

	void								DrawCornerRoundBitmap(CDC* pDC, CRect rc);
protected:
	void								OnTest();
	void								OnAddCase();

	void								SetOubData(WPARAM wParam, LPARAM lParam);
	void								SetJangoData(char* szReceive, int iKey=SJGOLIST, int irow=0);
	void								SetSiseData(char* szReceive, int iLen, int iKey=GOOPKEY, int irow=0);
	void								SetjumunData(char* szdata, int irow);

	void								ParseRealData(char* szdata);
	void								ParseRealData(struct _alertR* alertR);
	void								SetRealData(CString sRealCode, const CMapStringToString& arRealList);
	void								SetRealData(CString sRealCode, DWORD* data);
	void								SetCalcData(int irow);
	void								CalculateTotolData();

	int									insertGridRow(int row, bool binsert=true, bool bheader=false);
	void								SetGridPalletColor(int row, bool bheader=false);

	int									SetJangoFormat(CString sRows, CStringArray& arList);
	void								DrawGuideBar(CDC* pDC, CRect rc);

	void								DrawGridHeader(CDC* pDC);
	void								DeleteAllItem_RemainGrid();
	void								ClearData();
	void								ResetPalleteColor(bool bInvalidate=false);
	void								ResetGridPalleteColor();
	void								ResetData();
	void								CheckPassWord(CString sAcc, CString sPass) ;
	void								CheckPassword();





	

	void								ExecJangoQuery(CString strCode=_T(""), CString sNext=_T(""), int irow=-1);
	void								ExecPOOPQuery(CString strCode=_T(""), int irow=-1);
	int									ExecSetup(int itype=0, int irow=-1);
	void								ExecResetSetup(int itype=0);
	void								ExecViewLog();
	void								ExecJumunQuery(int irow);
	void								ExecHelp();



	void								SetCheckCase(int irow, _config cfg, BOOL bFirst=FALSE);
	void								ResetCaseValue(int irow);
	void								ResetJumunValue(int irow);
	void								ResetVolumn(int irow, int ijumunvol);
	CString								GetPrice_SetupValue(int irow, int iunitType, int istopType, int iconfigval, int iCodeType);


	void								CheckStopLossValue(int irow);
	void								SetNoticeData(char* szReceive);
	void								ChangeAccount(char* szReceive);

	bool								CheckSetupVol(int irow);
	bool								IsvalidateSetupCase(int irow);
	

	bool								GetLedgerMessage(CString sLedger, CString& sMessage, bool& bNext);

	void								WriteLog(CString sType, CString sLog, CString sCode=_T(""), CString sItem=_T(""), bool bsend=true);
	void								SendLog(CString sData);
	CString								GetCaseTextLog(_config cfg);
	void								SetJangoType(int irow);
	CString								GetCrtText(int iCode);
	

	void								EnableButtons(BOOL bEnable);
	CString								GetHogaType(int itype);

	void								CheckCase_changejangoVolume(int irow);
	CString								GetJumunCrtCode(CString sCrtCode, CString& sTxtValue);
	bool								IsJusikCode(CString sCode);
	void								SetGamsi(int irow);

	void								UploadLogData();
	void								SendLargeLog(int count,CString sData);




	void								DebugLog(CString strText) ;


	CBitmap	m_pMaskBitmap;				
	void								ChangeFont(int ifontsize, CString sFont);
	void								Resize(int cx, int cy);
	void								ResizeGridColumnWidth();
	void								DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap);
	
	HBITMAP								CreateBitmapMask(HBITMAP hSourceBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTransColor);
	void								LoadRoundBitmap();

	CString								GetEncPassword(CString sPswd);	//2013.13.23 KSJ 일방향암호화












// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMyPassword(WPARAM wParam, LPARAM lParam);
	LONG OnUser_Axis(WPARAM wParam, LPARAM lParam);
	LONG OnUser(WPARAM wParam, LPARAM lParam);
	
	
	DECLARE_MESSAGE_MAP()
private:
	bool IsNumber(CString str);
};

