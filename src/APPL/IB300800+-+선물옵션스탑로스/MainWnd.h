#pragma once
// MainWnd.h : header file
//
#include "LogDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
typedef struct _pibofodr_mid {
	char nrec[4];
	char odgb[1];    /* 주문유형 : 1:일반 2:선옵 3:예약                  */
	char mkgb[1];    /* Filler                                           */
	char mmgb[1];    /* 매매구분 : 1:매도 2:매수 3:정정 4: 취소          */
	char accn[11];   /* 계좌번호                                         */
	char pswd[8];    /* 비밀번호                                         */
	char ojno[12];   /* 원주문번호                                       */
	char cod2[12];   /* 단축코드                                         */
	char jqty[8];    /* 주문수량                                         */
	char jprc[10];   /* 주문단가                                         */
	char hogb[2];    /* 호가구분                                         */
	char cdgb[1];    /* 조문조건 : 0:X   1:IOC 2: FOK                    */
	char mdgb[1];    /* 정정취소 : 1:일부 2:전부                         */
	char prgb[1];    /* 처리구분 : 'A,B,C,D' : 이하 가변 'X' : 사용안함  */
	char usrf[40];
} pibofodr_mid;

typedef  struct  mid {
    char    func[1];        /* 'Q' : 조회 'I' 입력  */
	char    mapn[8];        /* 화면번호 */            
    char    kind[1];        /* 로그종류 */            
                            /* 화면별 세부구분 */     
    char    data[512];        /* 로그 데이타(MAX 1024) */
} pidologm_mid;

struct _List
{
	CRect	rect;
	int		icol;
	int		itoggleIdx;
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

enum {JK_FUTURE, JK_SPREAD, JK_CALLOPT, JK_PUTOPT, JK_JCALLOPT, JK_JPUTOPT, JK_STAR, JK_SFUTURE, JK_SFSPREAD, JK_KTBFUTURE, JK_USDFUTURE, JK_GOLDFUTURE, JK_PIGFUTURE, JK_MINIGOLDFUTURE};

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd*	pAxWnd);

// Attributes
public:
	CWnd*								m_pAxMain;
	CWnd*								m_pQueryModalWnd;
	CLogDlg*							m_pLogDlg;

	CString								m_strUserID, m_strDeptCode, m_strPass, m_strRootDir, m_strImgPath, m_strLogPath, m_strUserName;
	COLORREF							m_clrConditionColor[3], m_bgcolor, m_toolcolor, m_gridhdrbgcolor, m_gridLinecolor, m_gridRowColor[2], 
										m_gridhdrtxtcolor, m_txtcolor, m_panelcolor[2], m_gridFocuscolor;
	COLORREF							m_lbllinecolor,  m_lblbackcolor, m_realcolor, m_bodycolor, m_guidecolor, m_guideTxtcolor; 
	COLORREF							m_MaedoColor[5], m_MaesuColor[5], m_DangaColor[2];

	CString								m_strLedger;
	CFont*								m_pFont;

	_gridHdr							m_GridHdr[JCOLCOUNT+1];

protected:
	CCriticalSection					m_section;
	CAccountCtrl*						m_pAccnCtrl;
	CMxFlatEdit*						m_pPassEdit;
	CButton*							m_pAutoBtn;
	CfxImgButton*						m_pToolBar[7];
	CfxGrid*							m_pRemainGrid;
	CButton*							m_pTestBtn;
	CComboBox*							m_pTestCbo;
	CString								m_strGuideMessage;
	CRect								m_pClientRect;

	CButton*							m_pConfigBtn;


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
	HINSTANCE							m_hiSha256;	//2013.12.23 KSJ 일방향암호화 추가



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
	CString								GetLedger(CString svcd, CString strAccount, char codrf='1', CString sNext=_T(""), int iRowCnt=10, CString sPswd="");	//2013.12.23 KSJ 일방향암호화 추가
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
	void								SetRealData(CString sRealCode, CMapStringToString& arRealList);
	void								SetRealData(CString sRealCode, DWORD* data);
	void								SetCalcData(int irow);
	void								CalculateTotolData();
	int									getBase(CString m_code);
	int									getJKind(CString m_code);

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

	BOOL								SendTR(LPCSTR name, char type, LPCSTR data, int dlen, char key);
	void								make_fodr( vector<char> &buff, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR code, int jqty, int jprc, int cdgb, LPCSTR hogb);
	void								merge_order( vector<char> &buff, vector< vector<char> > &orders );
	struct _ledgerH*					GetLedger( struct _ledgerH *ledger );
	LPCTSTR								Variant( int cmd, LPCTSTR data );

	

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
	CString								GetPrice_SetupValue(int irow, int iunitType, int istopType, double iconfigval, int iCodeType,CString type="매 도");


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

	CString								GetEncPassword(CString sPswd);		//2013.12.23 KSJ 일방향암호화 추가












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

