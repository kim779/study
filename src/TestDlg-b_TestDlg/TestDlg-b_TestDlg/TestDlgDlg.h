
// TestDlgDlg.h: 헤더 파일
//
#include <queue>

#pragma once

#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"

#include "EditX.h"
#include "CEditSearch.h"
#include "CMarketPopWnd.h"
#ifndef _gMuVal
//extern HANDLE g_hMutex;
#define _gMuVal
#endif
//#include "BrowserWindowEdge.h"

struct	_Ralert {
	CString code;
	int	stat{};				// DLL_ALERT.stat
	int	size{};				// data record count
	std::unique_ptr<char[]> ptr[999]{};	// data record array

	~_Ralert() {
		for (int i = 0; i < 999; i++) {
			ptr[i].reset();
		}
	}
};
#define	L_sRalertR	sizeof(struct _Ralert)


struct	_ralert {
	//char code[10]{};
	CString code;
	int	stat{};				// DLL_ALERT.stat
	int	size{};				// data record count
	char* ptr[999]{};	// data record array

	/*~_ralert() {
		for (int i = 0; i < 999; i++) {
			ptr[i].reset();
		}
	}*/
};
#define	L_RalertR	sizeof(struct _ralert)



#include "cs_TLS.h"

#define STDMETHOD_(type,method)  virtual COM_DECLSPEC_NOTHROW type STDMETHODCALLTYPE method

// CTestDlgDlg 대화 상자
class CTestDlgDlg : public CDialogEx
{
	STDMETHOD_(BOOL, GetETNMst)(CStringArray*& pastrJongCode, CStringArray*& pastrJongName, CStringArray*& pastrJongEngName, int nOption = 0);
	BOOL GetETNMst1(CStringArray*& pastrUpCode, CStringArray*& pastrUpName, CStringArray*& pastrUpEngName, int nOption);
	
// 생성입니다.
public:
	CTestDlgDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	std::unique_ptr<char[]> m_hcB{};

	//std::unique_ptr<BrowserWindowEdge> browserEdge;

	wil::com_ptr<ICoreWebView2Controller> _edgeControl;
	wil::com_ptr<ICoreWebView2>	      _edgeWnd;

	char* m_ledger;
	CToolTipCtrl		m_tooltip;
	void SetToolTiplist();
	CString m_slog;
	class CTestScroll* m_pWnd{};
	HANDLE ProcessFind(char* strProcessName);
	bool GetProcessModule(DWORD dwPID, char* sProcessName);
	HANDLE m_Targethandle{};
	HWND m_hTarget{};


	CByteArray	m_major;
	CMap	<int, int, class CWorks*, CWorks*> m_clients;


	//vector, map
	std::map<CString, std::unique_ptr<struct _Ralert>> _m_mapRealData;


	std::vector <BYTE> _vMajor;

	std::unique_ptr< CMarketPopWnd> m_pPopup;
	std::unique_ptr< CComboBox> m_pPopcombo;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTDLG_DIALOG };
#endif

public:
	//int iname;
	CEditSearch m_EditSearch;
	//CEdit m_EditSearch;


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	BOOL CreateShortcut(LPTSTR pszFilePath, LPTSTR pszTitle, LPTSTR pszDescription, BOOL bDesktop, BOOL bQuickLanch, BOOL bStartMenu, BOOL bOverWrite);
	void SendTR(LPCSTR trnm, int tkey, int stat, LPCSTR data, int size);
	void testref(BOOL& ref);
	int compress(BYTE* rbuf, int rlen, BYTE* cbuf);

	class cl_testcompress* m_pzip{};


	HCURSOR m_hCursor{};
// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrnBit();
	afx_msg void OnBnClickedBrnBit2();
	afx_msg void OnBnClickedBtnmfc140();
	afx_msg void OnEnChangeEditData();
	afx_msg void OnBnClickedBtnFormat();
	afx_msg void OnBnClickedBtnMap();
	afx_msg void OnBnClickedBtnRace();
	afx_msg void OnBnClickedBtnConsumer();
	afx_msg void OnBnClickedBtnConsumer2();
	afx_msg void OnBnClickedBtnFuture();
	afx_msg void OnBnClickedBtnFuture2();
	afx_msg void OnBnClickedBtnAsync();
	afx_msg void OnBnClickedBtnRlef();
	afx_msg void OnBnClickedBtnPair();
	afx_msg void OnBnClickedBtnTuple();
	afx_msg void OnBnClickedBtnLamda();
	afx_msg void OnBnClickedBtnLamdaFind();
	afx_msg void OnBnClickedBtnOs();
	afx_msg void OnBnClickedBtnOsversion();
	afx_msg void OnBnClickedBtnHashkey();
	afx_msg void OnBnClickedBtnWin();
	afx_msg void OnBnClickedBtnIcon();
	afx_msg void OnBnClickedBtnPrsfind();
	afx_msg void OnBnClickedBtnStdcopy();
	afx_msg void OnBnClickedBtnHex();
	afx_msg void OnBnClickedBtnCode();
	afx_msg void OnBnClickedBtnIe();
	afx_msg void OnBnClickedBtnInt2str();
	afx_msg void OnBnClickedBtnRemoveicon();
	afx_msg void OnBnClickedBtnCef();
	afx_msg void OnBnClickedBtnSerial();
	afx_msg void OnBnClickedBtnSerial2();
	afx_msg void OnBnClickedBtnSerial3();
	afx_msg void OnBnClickedBtnHangul();
	afx_msg void OnBnClickedBtnLoadocx();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnChart();
	afx_msg void OnBnClickedBtnRect();
	afx_msg void OnBnClickedBtnRegread();
	afx_msg void OnBnClickedBtnShared();

	
private:
	//공유 메모리
	HANDLE	m_hKeyFile;
	CString m_strSharedMName{};
	CRITICAL_SECTION csMapHandle{};

	// 시간계산
	CTime CalTimeBySpan(int year, int month, int day, int hour, int minute, int secound,  CTimeSpan spam, BOOL bPlus);
	CString CalMaketTime(CString strTime, bool bEnd);

	CString parser(CString& srcstr, CString substr);

	//vector
	std::vector<std::unique_ptr<struct _Ralert>> _vETNx;
	CArray < std::unique_ptr<struct _Ralert>> _arr;
	CArray <struct _ralert*> sarr;


	//DPI lamda
	static HMODULE GetUser32Module();

	cs_TLS m_csTls;

	//bookmark
	void BookFileProcess(int itype);
public:
	//thread
	BOOL m_bTdStop = FALSE;
	CStringArray m_arr;
	BOOL m_bWritingThread{};
	CString m_strThreadData{};

	//asyncmessagebox
	void RunAsync(std::function<void()> callback);
	void AsyncMessageBox(CString message, CString title);


	BOOL CheckMutextNotUsing();
	HANDLE g_hMutex{};
	BOOL m_bUseing{};
	CCriticalSection m_cs{};
	BOOL CheckThreadUsing();
	std::queue<void*> m_thQueue;
	void StartNextThread();


	HINSTANCE m_hSharedLib{};
	CWnd* m_pSharedMemory{};


	//FDS
	HMODULE	m_hDll{};

	//vc version
	CString GetVCSystemVersion();
	BOOL  InstallVCReInstall(CString spath);
public:
	afx_msg void OnBnClickedBtnTime();
	afx_msg void OnBnClickedBtnIpc();
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedBtnFindps();
	afx_msg void OnBnClickedBtnSharedWrite();
	afx_msg void OnBnClickedBtnSharedRead();
	afx_msg void OnBnClickedBtnPlaysound();
	afx_msg void OnBnClickedBtnKeymake();
	afx_msg void OnBnClickedBtnFilecheck();
	afx_msg void OnBnClickedBtnCalcwindowrect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnScrolldlg();
	afx_msg void OnBnClickedUnorderedmap();
	afx_msg void OnBnClickedBtnPlyer();
	afx_msg void OnBnClickedBtnSlider();
	afx_msg void OnBnClickedBtnIntfloat();
	CComboBox m_cbTest;
	
	afx_msg void OnCbnSelchangeCbTest();
	afx_msg void OnBnClickedBtnIntfloat2();
	afx_msg void OnBnClickedBtnTcp();
	afx_msg void OnBnClickedBtnRef();
	afx_msg void OnBnClickedBtnAfxex();
	afx_msg void OnBnClickedBtnVector();
	afx_msg void OnBnClickedBtnVector2();
	afx_msg void OnBnClickedBtnIme();
	
	afx_msg void OnBnClickedBtnThread();
	afx_msg void OnBnClickedBtnThread2();
	afx_msg void OnBnClickedBtnThread3();
	afx_msg void OnBnClickedBtnThread4();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnCritical();
	afx_msg void OnBnClickedBtnCritical2();
	afx_msg void OnBnClickedBtnCri1();
	afx_msg void OnBnClickedBtnCri2();
	afx_msg void OnBnClickedBtnThread5();
	afx_msg void OnBnClickedBtnEdgedlg();
	afx_msg void OnBnClickedBtnLoadlib();
	afx_msg void OnBnClickedBtnWebview2();
	afx_msg void OnBnClickedBtnWebview3();
	afx_msg void OnBnClickedBtnUpdown();
	afx_msg void OnBnClickedBtnRemovif();
	afx_msg void OnBnClickedBtnTrans();
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnBnClickedBtnIebrowser();
	afx_msg void OnBnClickedBtnIme2();
	afx_msg void OnBnClickedBtnCmd();
	afx_msg void OnBnClickedBtnEndian();
	afx_msg void OnBnClickedBtnCmdpackage();
	afx_msg void OnBnClickedBtnWmic();
	afx_msg void OnBnClickedBtnRedis();
	afx_msg void OnBnClickedBtnJung();


	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnClickedBtnJung();
	afx_msg void OnBnClickedBtnInterfile();
	afx_msg void OnBnClickedBtnInterfile2();
	afx_msg void OnBnClickedBtnUrlencode1();
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnThread6();
	afx_msg void OnBnClickedBtnStdmap();
	afx_msg void OnBnClickedBtnDowm();
	afx_msg void OnBnClickedBtnQsort();
	afx_msg void OnBnClickedBtnEdgeself();
	afx_msg void OnBnClickedBtnIntstr();
	afx_msg void OnBnClickedBtnUrlen2();
	afx_msg void OnBnClickedBtnGetsection();
	afx_msg void OnBnClickedBtnCmd2();
	afx_msg void OnBnClickedBtnSock();
	afx_msg void OnBnClickedBtnSession();
	afx_msg void OnBnClickedBtnCompress();
	afx_msg void OnBnClickedBtnCalender();
	afx_msg void OnBnClickedBtnCalender2();
	afx_msg void OnBnClickedBtnExcute();
	afx_msg void OnBnClickedBtnOsver();
	afx_msg void OnBnClickedBtnAny();
	afx_msg void OnBnClickedBtnPop();
	afx_msg void OnBnClickedBtnMac();
	afx_msg void OnBnClickedBtnIpconfigall();
	afx_msg void OnBnClickedBtnFileload();
	afx_msg void OnBnClickedBtnRound();
	afx_msg void OnBnClickedBtnRe();
	CButton m_btnClear;
	afx_msg void OnBnClickedBtnCmap();
	afx_msg void OnBnClickedBtnForthread();
	afx_msg void OnBnClickedBtnCommarco();
	afx_msg void OnBnClickedBtnKey();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnKey2();
	afx_msg void OnBnClickedBtnLocal();
	afx_msg void OnBnClickedBtnBit();
	afx_msg void OnBnClickedBtnMutexwrite();
	afx_msg void OnBnClickedBtnMutexwrite2();
	afx_msg void OnBnClickedBtnMutexwrite3();
	afx_msg void OnBnClickedBtnMutexwrite4();
	afx_msg void OnBnClickedBtnMutexwrite5();
	afx_msg void OnBnClickedBtnMutexwrite6();
	afx_msg void OnBnClickedBtnLocal2();
	afx_msg void OnBnClickedBtnLocal3();
	afx_msg void OnBnClickedBtnLocal4();
	afx_msg void OnBnClickedBtnHen();
	afx_msg void OnBnClickedBtnEdgepop();
	afx_msg void OnBnClickedBtnEnc();
	afx_msg void OnBnClickedBtnDec();
	afx_msg void OnBnClickedBtnHexenc();
	afx_msg void OnBnClickedBtnHexdec();
	afx_msg void OnBnClickedBtnShared2();
	afx_msg void OnBnClickedBtnSharedWrite2();

	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnListbox();
	afx_msg void OnBnClickedBtnListbox2();
	afx_msg void OnBnClickedBtnMonitor();
	afx_msg void OnBnClickedBtnMonitor2();
	afx_msg void OnBnClickedBtnWrite32();
	afx_msg void OnBnClickedBtnMonitor3();
	afx_msg void OnSetfocusEditShared();
	afx_msg void OnChangeEditShared();
	afx_msg void OnUpdateEditShared();


	void CheckIME();
	afx_msg void OnBnClickedBtnChkime();
	afx_msg void OnThemechangedEditShared(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnChkbit();
	afx_msg void OnBnClickedBtnChkbit2();
	afx_msg void OnBnClickedBtnChkbit3();
	afx_msg void OnBnClickedBtnLamdasettimer();
	afx_msg void OnBnClickedBtnLamdasettimer2();
	afx_msg void OnBnClickedAesEnc();
	afx_msg void OnBnClickedAesEnc2();
	afx_msg void OnBnClickedFds();
	afx_msg void OnBnClickedFdsSend();
	afx_msg void OnBnClickedFdsClose();
	afx_msg void OnBnClickedNxtMid();
	afx_msg void OnBnClickedSfcode();
	afx_msg void OnBnClickedSfcode2();
	afx_msg void OnBnClickedDpi();
	afx_msg void OnBnClickedGrid();
	afx_msg void OnBnClickedGrid2();
	afx_msg void OnBnClickedFdsfile();
	afx_msg void OnBnClickedFdsfile2();
	afx_msg void OnBnClickedPubip();
	afx_msg void OnBnClickedCreateProcess();
	afx_msg void OnBnClickedSha();
	afx_msg void OnBnClickedPcinfo();
	afx_msg void OnBnClickedMapList();
	afx_msg void OnBnClickedVector();
	afx_msg void OnBnClickedWinpop();
	afx_msg void OnBnClickedCombopop();
	afx_msg void OnBnClickedBtnAlgo();
	afx_msg void OnBnClickedStd();
	afx_msg void OnBnClickedBtnFilemove();
	//afx_msg void OnBnClickedBtnMsgbox();
	afx_msg void OnBnClickedCal();
	afx_msg void OnBnClickedMfcRege();
	afx_msg void OnBnClickedMfcRege2();
	afx_msg void OnBnClickedMfcSearch();


	public:

		afx_msg void OnBnClickedDump();
};
static cs_TLS* m_csTls;
