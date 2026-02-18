#pragma once
// MapWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
#include "../../control/fx_misc/fxImgButton.h"
#include "sharemsg.h"
#include "GridWnd.h"
#include <afxmt.h>
#include "../../H/TickStore.h"

class CMainWnd : public CWnd
{
	friend class CIB202200App;
	// Construction
public:
	CMainWnd(CWnd *pWnd);

	// Attributes
public:
	CRect m_rectMain;
	CMapStringToString m_mapMemoCode{};
	// Operations
public:
	BOOL GetRTSVisible() { return m_bRTS; };
	void SetRTSVisible(BOOL bRTS) { m_bRTS = bRTS; };
	COLORREF getAxColor(int color);
	CFont *getAxFont(CString fName = _T("굴림체"), int point = 9, bool bBold = false, bool bItalic = false);
	CPen*  getAxPen(COLORREF clr, int width = 1, int style = PS_SOLID);
	CString GetMarketTime() { return m_strMarketTime; };  // KSJ 2012.11.26 장운영시간
	void IsChange(BOOL bChange) { m_bChange = bChange; }; // 2016.12.16 KSJ 종료할때 관심종목이 변경되었는지 세팅
	CString GetMarketEndTime() { return m_strMarketEndTime; };
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam); 
	afx_msg LONG OnMainMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	CFont* m_pFont{};
	CString _sTrigger;
	bool    _bInit{};

private:
	// MEMBERS
	// window members
	CWnd* m_pWnd{};
	class CIB202200App* _pApp{};
	std::unique_ptr<class CTreeWnd> m_pTreeWnd{};
	std::unique_ptr<class CToolWnd> m_pToolWnd{};
	std::unique_ptr<class CGroupWnd> m_pGroupWnd{};
	std::unique_ptr<class COptDlg>  m_pOptDlg{};


	bool m_bProc{};
	int m_nOption{};
	BOOL m_bDLLDATA{};
	BOOL m_bUser9555{}; // 2016.12.13 KSJ 메인에서 결과 기다릴때
	BOOL m_bUser9111{}; // 2016.12.13 KSJ 메인 닫힐 때
	BOOL m_bChange{};	  // 2016.12.16 KSJ 저장이 발생 했는지 체크

	// resource memebers
	CSize m_size;
	int m_nTree{};   // 트리 사이즈
	int m_nBasket{}; // 바구니 싸이즈
	int m_nGroup{};  // 그룹윈도우 싸이즈
		       // for resizing
	int m_nResize{};
	CPoint m_xPos;
	CPoint m_xPosMove;
	CRect m_rcDIVIDE;
	// data members
	CString m_user;
	CString m_id;
	CString m_home;
	BOOL m_bTree{};
	BOOL m_bRTS{};
	BOOL m_bBasket{};
	CRect m_rcDivide[3];
	CRect m_rcPanel[3]; // for rounding

	CfxImgButton m_btOption;
	
	bool m_bDestroy{};
	CBrush* m_pBrush{};
	int m_nOptHeight{};
	CString m_szRET;
	CString m_strWaveFile;
	BOOL m_bWaveApply{};

	COLORREF m_clrKospi{};
	COLORREF m_clrKosdaq{};
	BOOL m_bMKClrApply{};
	BOOL m_bPopupDlg{};
	BOOL m_bDispCode{};
	BOOL m_bStartMsg{};
	CString m_strTag;
	BOOL m_bNewsNChart{};    // 2011.12.02	KSJ
	BOOL m_bMargin{};	       // 2011.12.05	KSJ
	BOOL m_bInfo{};	       // 2011.12.05	KSJ
	BOOL m_bTicker{};	       // 2011.12.12	KSJ
	bool m_bDominoToolWnd{}; // 2012.01.13 KSJ 맵화면에서 관심종목을 열때 툴바가 생성되지 않았는데 열릴 때가 많음
	int m_nGroupIndex{};     // 2012.01.13 KSJ 그룹인덱스

	CString m_strMarketTime;    //시장구분 (동시호가) KSJ 2012.11.26
	CString m_strMarketEndTime; //동시호가
				    // etc...
	void DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);

	// OPERATION
	// windows operation
	CBrush *getAxBrush(COLORREF clr);
	void DrawMove(CRect rect);
	void SendOper(class CSendData *sdata);
	void SelectGroup(int kind);
	void CreateChild();
	void DestroyChild();
	void ResizeOper(int cx, int cy);
	LONG OperTreeVisible(int opt);
	LONG OperBasketVisible(int opt);
	void OperSize(int opt, CSize size);
	void OpenScreen(int type, CString data);
	LONG OperDLLOUB(CRecvData& data);
	void parsingTrigger(CString datB);
	CBitmap* getAxBitmap(CString path);
	DWORD SearchOption(WORD kind, DWORD option);
	CString SplitString(CString &strData, CString strToken);
	void CheckRTSTimer(bool bFirst);
	void parsingDomino(CString datB);
	// resource operation
	void DrawDivide(CDC *pDC);
	void SetPallette();
	void InvalidateDivide();

	// data operation
	void init();
	void loadinfo();
	void saveinfo();
	void sendTR(CString trCode, char *datB, int datL, int key);
	CString Variant(int comm, CString data = "");
	void SendMode(bool bSend = true);
	void Enable(int kind, bool bEnable = true);
	void SaveItemInfo(CString strKeyName, CString strValue); // 2011.12.13 KSJ 한아이템 ini파일에 추가함
	// etc...
	void GuideMessage(CString msg);

	// 2013.01.23 KSJ win7 or win8일때 관심종목의 사이즈가 x+2, y+1되는 현상이 있다.
	//모든버전이 그런지 확인 되지는 않았으나 우선 OnSize할때 x-2, y-1해준다.
//	bool IsWin7or8();
	bool m_bWin7or8{};

public: // RTM
	BOOL IsValidCode(CString strCode);
	CString GetDataTitle(int nKind);
	BOOL m_bRemove{}; // 2011.12.29 KDK
	void setAlertx(BOOL bflag) { m_bAlertx = bflag; }

	int m_icheckTime{};
	int m_DiffSec{};
	bool ShouldSkipRTSByTimeDiff(CString& pcTime,	const char* pRTSTime, int allowDiffSec,	bool  bEnableCheck	);
	bool IsEnableRTSTimeCheck(const CTime& currentTime,	const CTime& baseTime,	 int checkTimeSec);
	CTime ParseRTSTime(const CString& sTime, const COleDateTime& today);
	std::unordered_map<std::string, CTime> m_lastRTSTimeMap;
	bool ShouldSkipRTSByServerTime(const std::string& code, const char* pServerTime, int  minIntervalSec);
private:			 

private:
	MINMAXINFO m_MMI{};

	CString m_strPal;
	CString GetPalette();

	BOOL m_bAlertx; // 변경됨  이값이  FALSE 이면 리얼타임 안됨. 쓰레드들 잠시 중지를 위해서 필요.

	void SetMarketTime(char *datB); // 2012.11.26 KSJ 장운영구분 조회
	void Wait(DWORD dwMillisecond); // 2015.04.08 KSJ 메인을 꺼버리면 수정된게 저장되지 않아 딜레이 시킴.
public:
	void RequestMarketTime(); // 2012.11.26 KSJ 장운영구분 조회
	void uploadBackup();	  // 2016.07.12 KSJ 관심종목 설정과 같이 'b' 보내줌	

	CString m_slog;
	int m_iTick{};
	BOOL m_bSending{};
	CString CheckIP();
	bool isIPInRange(CString ip, CString network_s, CString network_e);
	unsigned int IPToUInt(CString ip);

	bool m_bSkipRTS{};

	CString m_strBeginTimeEnd{};  //동시호가 종료시간 xx:59:59
	CString m_strEndTime{};       //동시호가 시작시간 xx:10:00
	CString m_strEndTimeEnd{};    //동시호가 종료시간 xx:59:59
	CString m_strBeginTime{};     //동시호가 시작시간 xx:10:00
	const int getDelayTime() const { return m_iTime; }

	int  m_iTime{};
	bool m_bcustomer{};
	
	
	void Request_GroupList();
	void Request_GroupCode(int iseq);
	void receiveOub(int key, CString data);
	bool IsFileExist(CString filename);  //test mod
	void setOptimizeCPUAffinity();
	void sendMemo(CString sCode, char type = 'S');

	CWnd* m_pMainFrame{};
	void GetMainFrameWnd();
	void SetSendTRtoMainFrame(CString trCode, char* datB, int datL, BYTE stat,int key);
	void RegisterRealtimeCodes(const std::vector<CString>& codeList);
	void UnregisterRealtime();


	void GetRTSFromMainFrame(CString scode);

	std::vector<std::string> m_codes;
	std::vector<int> m_symbols;
};
