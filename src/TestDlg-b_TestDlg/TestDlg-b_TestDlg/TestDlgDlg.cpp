
// TestDlgDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "TestDlg.h"
#include "TestDlgDlg.h"
#include "afxdialogex.h"
#include "defines.h"
#include "CControlWnd.h"
#include "CTestScroll.h"
#include "testcompress.h"



#include <Lm.h>
#pragma comment(lib, "netapi32.lib")
#include <stdio.h>
#include <shlobj_core.h>
#include <TlHelp32.h>
#include <vadefs.h>
#include <Windows.h>
#include <VersionHelpers.h>  // Windows 버전 확인을 위한 헤더 파일
#include <Psapi.h>
#include <minwindef.h>
#include <afxinet.h>

#include <afxwin.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


//#import "msxml.dll"
//#import "MSXML6.dll" rename_namespace(_T("MSXML"))
//#include <msxml6.h>
//using namespace MSXML;
//
//typedef MSXML::IXMLDOMElementPtr				XMLELEMENT;
//typedef MSXML::IXMLDOMDocumentPtr				DOCUMENT;
//typedef MSXML::IXMLDOMNodeListPtr				NODELIST;
//typedef MSXML::IXMLDOMNodePtr					NODE;
//typedef MSXML::IXMLDOMNamedNodeMapPtr			ATTRIBUTE;
//typedef MSXML::IXMLDOMProcessingInstructionPtr	PROCESSINGINSTRCT;
//typedef MSXML::IXMLDOMParseErrorPtr				PARSEERROR;
//extern HANDLE g_hMutex = NULL;
//HANDLE g_hMutex = nullptr;
//void InitalizeMutex()
//{
//	if (g_hMutex == nullptr)
//	{
//		// g_hMutex 초기화 또는 사용
//		g_hMutex = CreateMutex(nullptr, FALSE, _T("Global\\MyMutexName"));
//		if (g_hMutex == nullptr)
//		{
//			// g_hMutex 사용
//			//CloseHandle(g_hMutex);
//			//g_hMutex = nullptr;
//		}
//	}
//}

BOOL GetWindowsVersionfuck(DWORD& dwMajor, DWORD& dwMinor)
{
	static DWORD dwMajorCache = 0, dwMinorCache = 0;
	if (0 != dwMajorCache)
	{
		dwMajor = dwMajorCache;
		dwMinor = dwMinorCache;
		return TRUE;
	}

	LPWKSTA_INFO_100 pBuf = NULL;
	if (NERR_Success != NetWkstaGetInfo(NULL, 100, (LPBYTE*)&pBuf))
		return FALSE;

	dwMajor = dwMajorCache = pBuf->wki100_ver_major;
	dwMinor = dwMinorCache = pBuf->wki100_ver_minor;
	NetApiBufferFree(pBuf);

	return TRUE;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace name_test {
	int iname;
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestDlgDlg 대화 상자

#define  TID_CHANGEHANGLE 9877

//CRITICAL_SECTION g_CriticalSection;
CTestDlgDlg::CTestDlgDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTDLG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//InitializeCriticalSection(&g_CriticalSection);
//	BrowserWindowEdge::InitInstance(theApp.m_hInstance);
}

void CTestDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_TEST, m_cbTest);
	DDX_Control(pDX, IDC_EDIT_CMD, m_EditSearch);
	DDX_Control(pDX, IDC_BTN_CLEAR, m_btnClear);
}

BEGIN_MESSAGE_MAP(CTestDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BRN_BIT, &CTestDlgDlg::OnBnClickedBrnBit)
	ON_BN_CLICKED(IDC_BRN_BIT2, &CTestDlgDlg::OnBnClickedBrnBit2)
	ON_BN_CLICKED(IDC_BTN_mfc140, &CTestDlgDlg::OnBnClickedBtnmfc140)
	ON_EN_CHANGE(IDC_EDIT_DATA, &CTestDlgDlg::OnEnChangeEditData)
	ON_BN_CLICKED(IDC_BTN_FORMAT, &CTestDlgDlg::OnBnClickedBtnFormat)
	ON_BN_CLICKED(IDC_BTN_MAP, &CTestDlgDlg::OnBnClickedBtnMap)
	ON_BN_CLICKED(IDC_BTN_RACE, &CTestDlgDlg::OnBnClickedBtnRace)
	ON_BN_CLICKED(IDC_BTN_CONSUMER, &CTestDlgDlg::OnBnClickedBtnConsumer)
	ON_BN_CLICKED(IDC_BTN_CONSUMER2, &CTestDlgDlg::OnBnClickedBtnConsumer2)
	ON_BN_CLICKED(IDC_BTN_FUTURE, &CTestDlgDlg::OnBnClickedBtnFuture)
	ON_BN_CLICKED(IDC_BTN_FUTURE2, &CTestDlgDlg::OnBnClickedBtnFuture2)
	ON_BN_CLICKED(IDC_BTN_ASYNC, &CTestDlgDlg::OnBnClickedBtnAsync)
	ON_BN_CLICKED(IDC_BTN_RLEF, &CTestDlgDlg::OnBnClickedBtnRlef)
	ON_BN_CLICKED(IDC_BTN_PAIR, &CTestDlgDlg::OnBnClickedBtnPair)
	ON_BN_CLICKED(IDC_BTN_TUPLE, &CTestDlgDlg::OnBnClickedBtnTuple)
	ON_BN_CLICKED(IDC_BTN_LAMDA, &CTestDlgDlg::OnBnClickedBtnLamda)
	ON_BN_CLICKED(IDC_BTN_LAMDA_FIND, &CTestDlgDlg::OnBnClickedBtnLamdaFind)
	ON_BN_CLICKED(IDC_BTN_OS, &CTestDlgDlg::OnBnClickedBtnOs)
	ON_BN_CLICKED(IDC_BTN_OSVERSION, &CTestDlgDlg::OnBnClickedBtnOsversion)
	ON_BN_CLICKED(IDC_BTN_HASHKEY, &CTestDlgDlg::OnBnClickedBtnHashkey)
	ON_BN_CLICKED(IDC_BTN_WIN, &CTestDlgDlg::OnBnClickedBtnWin)
	ON_BN_CLICKED(IDC_BTN_ICON, &CTestDlgDlg::OnBnClickedBtnIcon)
	ON_BN_CLICKED(IDC_BTN_PRSFIND, &CTestDlgDlg::OnBnClickedBtnPrsfind)
	ON_BN_CLICKED(IDC_BTN_STDCOPY, &CTestDlgDlg::OnBnClickedBtnStdcopy)
	ON_BN_CLICKED(IDC_BTN_HEX, &CTestDlgDlg::OnBnClickedBtnHex)
	ON_BN_CLICKED(IDC_BTN_CODE, &CTestDlgDlg::OnBnClickedBtnCode)
	ON_BN_CLICKED(IDC_BTN_IE, &CTestDlgDlg::OnBnClickedBtnIe)
	ON_BN_CLICKED(IDC_BTN_INT2STR, &CTestDlgDlg::OnBnClickedBtnInt2str)
	ON_BN_CLICKED(IDC_BTN_REMOVEICON, &CTestDlgDlg::OnBnClickedBtnRemoveicon)
	ON_BN_CLICKED(IDC_BTN_CEF, &CTestDlgDlg::OnBnClickedBtnCef)
	ON_BN_CLICKED(IDC_BTN_SERIAL, &CTestDlgDlg::OnBnClickedBtnSerial)
	ON_BN_CLICKED(IDC_BTN_SERIAL2, &CTestDlgDlg::OnBnClickedBtnSerial2)
	ON_BN_CLICKED(IDC_BTN_SERIAL3, &CTestDlgDlg::OnBnClickedBtnSerial3)
	ON_BN_CLICKED(IDC_BTN_HANGUL, &CTestDlgDlg::OnBnClickedBtnHangul)
	ON_BN_CLICKED(IDC_BTN_LOADOCX, &CTestDlgDlg::OnBnClickedBtnLoadocx)
	ON_BN_CLICKED(IDC_BTN_CHART, &CTestDlgDlg::OnBnClickedBtnChart)
	ON_BN_CLICKED(IDC_BTN_RECT, &CTestDlgDlg::OnBnClickedBtnRect)
	ON_BN_CLICKED(IDC_BTN_REGREAD, &CTestDlgDlg::OnBnClickedBtnRegread)
	ON_BN_CLICKED(IDC_BTN_SHARED, &CTestDlgDlg::OnBnClickedBtnShared)
	ON_BN_CLICKED(IDC_BTN_TIME, &CTestDlgDlg::OnBnClickedBtnTime)
	ON_BN_CLICKED(IDC_BTN_IPC, &CTestDlgDlg::OnBnClickedBtnIpc)
	ON_BN_CLICKED(IDC_BTN_TEST, &CTestDlgDlg::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_FINDPS, &CTestDlgDlg::OnBnClickedBtnFindps)
	ON_BN_CLICKED(IDC_BTN_SHARED_WRITE, &CTestDlgDlg::OnBnClickedBtnSharedWrite)
	ON_BN_CLICKED(IDC_BTN_SHARED_READ, &CTestDlgDlg::OnBnClickedBtnSharedRead)
	ON_BN_CLICKED(IDC_BTN_PLAYSOUND, &CTestDlgDlg::OnBnClickedBtnPlaysound)
	ON_BN_CLICKED(IDC_BTN_KEYMAKE, &CTestDlgDlg::OnBnClickedBtnKeymake)
	ON_BN_CLICKED(IDC_BTN_FILECHECK, &CTestDlgDlg::OnBnClickedBtnFilecheck)
	ON_BN_CLICKED(IDC_BTN_CALCWINDOWRECT, &CTestDlgDlg::OnBnClickedBtnCalcwindowrect)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SCROLLDLG, &CTestDlgDlg::OnBnClickedBtnScrolldlg)
	ON_BN_CLICKED(IDC_UNORDEREDMAP, &CTestDlgDlg::OnBnClickedUnorderedmap)
	ON_BN_CLICKED(IDC_BTN_PLYER, &CTestDlgDlg::OnBnClickedBtnPlyer)
	ON_BN_CLICKED(IDC_BTN_SLIDER, &CTestDlgDlg::OnBnClickedBtnSlider)
	ON_BN_CLICKED(IDC_BTN_INTFLOAT, &CTestDlgDlg::OnBnClickedBtnIntfloat)
	ON_CBN_SELCHANGE(IDC_CB_TEST, &CTestDlgDlg::OnCbnSelchangeCbTest)
	ON_BN_CLICKED(IDC_BTN_INTFLOAT2, &CTestDlgDlg::OnBnClickedBtnIntfloat2)
	ON_BN_CLICKED(IDC_BTN_TCP, &CTestDlgDlg::OnBnClickedBtnTcp)
	ON_BN_CLICKED(IDC_BTN_REF, &CTestDlgDlg::OnBnClickedBtnRef)
	ON_BN_CLICKED(IDC_BTN_AFXEX, &CTestDlgDlg::OnBnClickedBtnAfxex)
	ON_BN_CLICKED(IDC_BTN_VECTOR, &CTestDlgDlg::OnBnClickedBtnVector)
	ON_BN_CLICKED(IDC_BTN_VECTOR2, &CTestDlgDlg::OnBnClickedBtnVector2)
	ON_BN_CLICKED(IDC_BTN_IME, &CTestDlgDlg::OnBnClickedBtnIme)
	
	ON_BN_CLICKED(IDC_BTN_THREAD, &CTestDlgDlg::OnBnClickedBtnThread)
	ON_BN_CLICKED(IDC_BTN_THREAD2, &CTestDlgDlg::OnBnClickedBtnThread2)
	ON_BN_CLICKED(IDC_BTN_THREAD3, &CTestDlgDlg::OnBnClickedBtnThread3)
	ON_BN_CLICKED(IDC_BTN_THREAD4, &CTestDlgDlg::OnBnClickedBtnThread4)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CRITICAL, &CTestDlgDlg::OnBnClickedBtnCritical)
	ON_BN_CLICKED(IDC_BTN_CRITICAL2, &CTestDlgDlg::OnBnClickedBtnCritical2)
	ON_BN_CLICKED(IDC_BTN_CRI1, &CTestDlgDlg::OnBnClickedBtnCri1)
	ON_BN_CLICKED(IDC_BTN_CRI2, &CTestDlgDlg::OnBnClickedBtnCri2)
	ON_BN_CLICKED(IDC_BTN_THREAD5, &CTestDlgDlg::OnBnClickedBtnThread5)
	ON_BN_CLICKED(IDC_BTN_EDGEDLG, &CTestDlgDlg::OnBnClickedBtnEdgedlg)
	ON_BN_CLICKED(IDC_BTN_LOADLIB, &CTestDlgDlg::OnBnClickedBtnLoadlib)
	ON_BN_CLICKED(IDC_BTN_WEBVIEW2, &CTestDlgDlg::OnBnClickedBtnWebview2)
	ON_BN_CLICKED(IDC_BTN_WEBVIEW3, &CTestDlgDlg::OnBnClickedBtnWebview3)
	ON_BN_CLICKED(IDC_BTN_UPDOWN, &CTestDlgDlg::OnBnClickedBtnUpdown)
	ON_BN_CLICKED(IDC_BTN_REMOVIF, &CTestDlgDlg::OnBnClickedBtnRemovif)
	ON_BN_CLICKED(IDC_BTN_TRANS, &CTestDlgDlg::OnBnClickedBtnTrans)
	ON_BN_CLICKED(IDC_BTN_FIND, &CTestDlgDlg::OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_BTN_IEBROWSER, &CTestDlgDlg::OnBnClickedBtnIebrowser)
	ON_BN_CLICKED(IDC_BTN_IME2, &CTestDlgDlg::OnBnClickedBtnIme2)
	ON_BN_CLICKED(IDC_BTN_CMD, &CTestDlgDlg::OnBnClickedBtnCmd)
	ON_BN_CLICKED(IDC_BTN_ENDIAN, &CTestDlgDlg::OnBnClickedBtnEndian)
	ON_BN_CLICKED(IDC_BTN_CMDPACKAGE, &CTestDlgDlg::OnBnClickedBtnCmdpackage)
	ON_BN_CLICKED(IDC_BTN_WMIC, &CTestDlgDlg::OnBnClickedBtnWmic)
	ON_BN_CLICKED(IDC_BTN_REDIS, &CTestDlgDlg::OnBnClickedBtnRedis)
	ON_BN_CLICKED(IDC_BTN_INTERFILE, &CTestDlgDlg::OnBnClickedBtnInterfile)
	ON_BN_CLICKED(IDC_BTN_INTERFILE2, &CTestDlgDlg::OnBnClickedBtnInterfile2)
	ON_BN_CLICKED(IDC_BTN_URLENCODE1, &CTestDlgDlg::OnBnClickedBtnUrlencode1)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CTestDlgDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_THREAD6, &CTestDlgDlg::OnBnClickedBtnThread6)
	ON_BN_CLICKED(IDC_BTN_STDMAP, &CTestDlgDlg::OnBnClickedBtnStdmap)
	ON_BN_CLICKED(IDC_BTN_DOWM, &CTestDlgDlg::OnBnClickedBtnDowm)
	ON_BN_CLICKED(IDC_BTN_QSORT, &CTestDlgDlg::OnBnClickedBtnQsort)
		ON_BN_CLICKED(IDC_BTN_EDGESELF, &CTestDlgDlg::OnBnClickedBtnEdgeself)
		ON_BN_CLICKED(IDC_BTN_INTSTR, &CTestDlgDlg::OnBnClickedBtnIntstr)
		ON_BN_CLICKED(IDC_BTN_URLEN2, &CTestDlgDlg::OnBnClickedBtnUrlen2)
		ON_BN_CLICKED(IDC_BTN_GETSECTION, &CTestDlgDlg::OnBnClickedBtnGetsection)
		ON_BN_CLICKED(IDC_BTN_CMD2, &CTestDlgDlg::OnBnClickedBtnCmd2)
		ON_BN_CLICKED(IDC_BTN_SOCK, &CTestDlgDlg::OnBnClickedBtnSock)
		ON_BN_CLICKED(IDC_BTN_SESSION, &CTestDlgDlg::OnBnClickedBtnSession)
		ON_BN_CLICKED(IDC_BTN_COMPRESS, &CTestDlgDlg::OnBnClickedBtnCompress)
		ON_BN_CLICKED(IDC_BTN_CALENDER, &CTestDlgDlg::OnBnClickedBtnCalender)
		ON_BN_CLICKED(IDC_BTN_CALENDER2, &CTestDlgDlg::OnBnClickedBtnCalender2)
		ON_BN_CLICKED(IDC_BTN_EXCUTE, &CTestDlgDlg::OnBnClickedBtnExcute)
		ON_BN_CLICKED(IDC_BTN_OSVER, &CTestDlgDlg::OnBnClickedBtnOsver)
		ON_BN_CLICKED(IDC_BTN_ANY, &CTestDlgDlg::OnBnClickedBtnAny)
		ON_BN_CLICKED(IDC_BTN_POP, &CTestDlgDlg::OnBnClickedBtnPop)
		ON_BN_CLICKED(IDC_BTN_MAC, &CTestDlgDlg::OnBnClickedBtnMac)
		ON_BN_CLICKED(IDC_BTN_IPCONFIGALL, &CTestDlgDlg::OnBnClickedBtnIpconfigall)
		ON_BN_CLICKED(IDC_BTN_FILELOAD, &CTestDlgDlg::OnBnClickedBtnFileload)
		ON_BN_CLICKED(IDC_BTN_ROUND, &CTestDlgDlg::OnBnClickedBtnRound)
		ON_BN_CLICKED(IDC_BTN_RE, &CTestDlgDlg::OnBnClickedBtnRe)
		ON_BN_CLICKED(IDC_BTN_CMAP, &CTestDlgDlg::OnBnClickedBtnCmap)
		ON_BN_CLICKED(IDC_BTN_FORTHREAD, &CTestDlgDlg::OnBnClickedBtnForthread)
		ON_BN_CLICKED(IDC_BTN_COMMARCO, &CTestDlgDlg::OnBnClickedBtnCommarco)
		ON_BN_CLICKED(IDC_BTN_KEY, &CTestDlgDlg::OnBnClickedBtnKey)
		ON_WM_MOUSEMOVE()
		ON_BN_CLICKED(IDC_BTN_KEY2, &CTestDlgDlg::OnBnClickedBtnKey2)
		ON_BN_CLICKED(IDC_BTN_LOCAL, &CTestDlgDlg::OnBnClickedBtnLocal)
		ON_BN_CLICKED(IDC_BTN_BIT, &CTestDlgDlg::OnBnClickedBtnBit)
		ON_BN_CLICKED(IDC_BTN_MUTEXWRITE, &CTestDlgDlg::OnBnClickedBtnMutexwrite)
		ON_BN_CLICKED(IDC_BTN_MUTEXWRITE2, &CTestDlgDlg::OnBnClickedBtnMutexwrite2)
		ON_BN_CLICKED(IDC_BTN_MUTEXWRITE3, &CTestDlgDlg::OnBnClickedBtnMutexwrite3)
		ON_BN_CLICKED(IDC_BTN_MUTEXWRITE4, &CTestDlgDlg::OnBnClickedBtnMutexwrite4)
		ON_BN_CLICKED(IDC_BTN_MUTEXWRITE5, &CTestDlgDlg::OnBnClickedBtnMutexwrite5)
		ON_BN_CLICKED(IDC_BTN_MUTEXWRITE6, &CTestDlgDlg::OnBnClickedBtnMutexwrite6)
		ON_BN_CLICKED(IDC_BTN_LOCAL2, &CTestDlgDlg::OnBnClickedBtnLocal2)
		ON_BN_CLICKED(IDC_BTN_LOCAL3, &CTestDlgDlg::OnBnClickedBtnLocal3)
		ON_BN_CLICKED(IDC_BTN_LOCAL4, &CTestDlgDlg::OnBnClickedBtnLocal4)
		ON_BN_CLICKED(IDC_BTN_HEN, &CTestDlgDlg::OnBnClickedBtnHen)
		ON_BN_CLICKED(IDC_BTN_EDGEPOP, &CTestDlgDlg::OnBnClickedBtnEdgepop)
		ON_BN_CLICKED(IDC_BTN_ENC, &CTestDlgDlg::OnBnClickedBtnEnc)
		ON_BN_CLICKED(IDC_BTN_DEC, &CTestDlgDlg::OnBnClickedBtnDec)
		ON_BN_CLICKED(IDC_BTN_HEXENC, &CTestDlgDlg::OnBnClickedBtnHexenc)
		ON_BN_CLICKED(IDC_BTN_HEXDEC, &CTestDlgDlg::OnBnClickedBtnHexdec)
		ON_BN_CLICKED(IDC_BTN_SHARED2, &CTestDlgDlg::OnBnClickedBtnShared2)
		ON_BN_CLICKED(IDC_BTN_SHARED_WRITE2, &CTestDlgDlg::OnBnClickedBtnSharedWrite2)

		ON_MESSAGE(WM_USER, OnMessage)
		ON_BN_CLICKED(IDC_BTN_LISTBOX, &CTestDlgDlg::OnBnClickedBtnListbox)
		ON_BN_CLICKED(IDC_BTN_LISTBOX2, &CTestDlgDlg::OnBnClickedBtnListbox2)
		ON_BN_CLICKED(IDC_BTN_MONITOR, &CTestDlgDlg::OnBnClickedBtnMonitor)
		ON_BN_CLICKED(IDC_BTN_MONITOR2, &CTestDlgDlg::OnBnClickedBtnMonitor2)
		ON_BN_CLICKED(IDC_BTN_WRITE32, &CTestDlgDlg::OnBnClickedBtnWrite32)
		ON_BN_CLICKED(IDC_BTN_MONITOR3, &CTestDlgDlg::OnBnClickedBtnMonitor3)

		//ON_NOTIFY(NM_CLICK, IDC_MY_CONTROL, &CTestDlgDlg::OnNotify)
		ON_EN_SETFOCUS(IDC_EDIT_SHARED, &CTestDlgDlg::OnSetfocusEditShared)
		ON_EN_CHANGE(IDC_EDIT_SHARED, &CTestDlgDlg::OnChangeEditShared)
		ON_EN_UPDATE(IDC_EDIT_SHARED, &CTestDlgDlg::OnUpdateEditShared)
		ON_BN_CLICKED(IDC_BTN_CHKIME, &CTestDlgDlg::OnBnClickedBtnChkime)
		ON_NOTIFY(NM_THEMECHANGED, IDC_EDIT_SHARED, &CTestDlgDlg::OnThemechangedEditShared)
		ON_BN_CLICKED(IDC_BTN_CHKBIT, &CTestDlgDlg::OnBnClickedBtnChkbit)
		ON_BN_CLICKED(IDC_BTN_CHKBIT2, &CTestDlgDlg::OnBnClickedBtnChkbit2)
		ON_BN_CLICKED(IDC_BTN_CHKBIT3, &CTestDlgDlg::OnBnClickedBtnChkbit3)
		ON_BN_CLICKED(IDC_BTN_LAMDASETTIMER, &CTestDlgDlg::OnBnClickedBtnLamdasettimer)
		ON_BN_CLICKED(IDC_BTN_LAMDASETTIMER2, &CTestDlgDlg::OnBnClickedBtnLamdasettimer2)
		ON_BN_CLICKED(IDC_AES_ENC, &CTestDlgDlg::OnBnClickedAesEnc)
		ON_BN_CLICKED(IDC_AES_ENC2, &CTestDlgDlg::OnBnClickedAesEnc2)
		ON_BN_CLICKED(IDC_FDS, &CTestDlgDlg::OnBnClickedFds)
		ON_BN_CLICKED(IDC_FDS_SEND, &CTestDlgDlg::OnBnClickedFdsSend)
		ON_BN_CLICKED(IDC_FDS_CLOSE, &CTestDlgDlg::OnBnClickedFdsClose)
		ON_BN_CLICKED(IDC_NXT_MID, &CTestDlgDlg::OnBnClickedNxtMid)
		ON_BN_CLICKED(IDC_SFCODE, &CTestDlgDlg::OnBnClickedSfcode)
		ON_BN_CLICKED(IDC_SFCODE2, &CTestDlgDlg::OnBnClickedSfcode2)
		ON_BN_CLICKED(IDC_DPI, &CTestDlgDlg::OnBnClickedDpi)
		ON_BN_CLICKED(IDC_GRID, &CTestDlgDlg::OnBnClickedGrid)
		ON_BN_CLICKED(IDC_GRID2, &CTestDlgDlg::OnBnClickedGrid2)
		ON_BN_CLICKED(IDC_FDSFILE, &CTestDlgDlg::OnBnClickedFdsfile)
		ON_BN_CLICKED(IDC_FDSFILE2, &CTestDlgDlg::OnBnClickedFdsfile2)
		ON_BN_CLICKED(IDC_PUBIP, &CTestDlgDlg::OnBnClickedPubip)
		ON_BN_CLICKED(IDC_CREATE_PROCESS, &CTestDlgDlg::OnBnClickedCreateProcess)
		ON_BN_CLICKED(IDC_SHA, &CTestDlgDlg::OnBnClickedSha)
		ON_BN_CLICKED(IDC_PCINFO, &CTestDlgDlg::OnBnClickedPcinfo)
		ON_BN_CLICKED(IDC_MAP_LIST, &CTestDlgDlg::OnBnClickedMapList)
		ON_BN_CLICKED(IDC_VECTOR, &CTestDlgDlg::OnBnClickedVector)
		ON_BN_CLICKED(IDC_WINPOP, &CTestDlgDlg::OnBnClickedWinpop)
		ON_BN_CLICKED(IDC_COMBOPOP, &CTestDlgDlg::OnBnClickedCombopop)
		ON_BN_CLICKED(IDC_BTN_ALGO, &CTestDlgDlg::OnBnClickedBtnAlgo)
		ON_BN_CLICKED(IDC_STD, &CTestDlgDlg::OnBnClickedStd)
		ON_BN_CLICKED(IDC_BTN_FILEMOVE, &CTestDlgDlg::OnBnClickedBtnFilemove)
		ON_BN_CLICKED(IDC_CAL, &CTestDlgDlg::OnBnClickedCal)
		ON_BN_CLICKED(IDC_MFC_REGE, &CTestDlgDlg::OnBnClickedMfcRege)
		ON_BN_CLICKED(IDC_MFC_REGE2, &CTestDlgDlg::OnBnClickedMfcRege2)
		ON_BN_CLICKED(IDC_MFC_SEARCH, &CTestDlgDlg::OnBnClickedMfcSearch)
		ON_BN_CLICKED(IDC_DUMP, &CTestDlgDlg::OnBnClickedDump)
		END_MESSAGE_MAP()


// CTestDlgDlg 메시지 처리기
LRESULT CTestDlgDlg::OnMessage(WPARAM wParam, LPARAM lParam)
{
	int len = 0;
	switch (LOBYTE(LOWORD(wParam)))
	{
		case 0x01:
		{
			char* pdata = (char*)lParam;
			m_slog.Format("%s", pdata);
			AfxMessageBox(m_slog);
		}
		break;
		case 0x04:
		{
			char* pdata = (char*)lParam;
			m_slog.Format("0x02  %s", pdata);
			AfxMessageBox(m_slog);
		}
		break;
	}

	return true;
}

BOOL CTestDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	HANDLE hprocess = GetCurrentProcess();
	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetToolTiplist();

	/*
	* 	CAccountCtrl* pCtrl = new CAccountCtrl(pParent, pParam);
	
	if (!pCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP, pParam->rect, pParent, 100))
	*/
	CRect rec; 
	((CWnd*)GetDlgItem(IDC_STATIC_WND))->GetWindowRect(rec);
	ScreenToClient(rec);
	m_pWnd = new CTestScroll;
	int iret= m_pWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_VSCROLL, rec, this, 1000);
	m_pWnd->SetScrollSizes(CSize(rec.Width() + 10, rec.Height() + 10));
	
	//m_pWnd->MoveWindow(rec);

	CString strHwnd;
	((CWnd*)GetDlgItem(IDC_EDIT_DATA))->SetWindowText("SubAxis");

	m_EditSearch.m_pParent = this;  //0422
	m_EditSearch.m_path = "D:\\IBKS_TEST";
	m_EditSearch.Init();
	m_EditSearch.Invalidate();
	//m_EditSearch.SendMessage(WM_LBUTTONDOWN, 1, 1);

	HWND hWnd = m_btnClear.GetParent()->GetSafeHwnd();

	// 윈도우를 top-most로 설정합니다.
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTestDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rec;
		m_EditSearch.GetWindowRect(&rec);
		m_EditSearch.DrawBitmap(&dc, rec);
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//-연산
void CTestDlgDlg::OnBnClickedBrnBit()
{
	//int xval = 0x2a;
	//int xdata = 0x08;

	//int iret = xval & xdata;
	//if (xval & xdata)
	//	TRACE("TRUE");
	//else
	//	TRACE("FALSE");

	struct	_rtmH {
		unsigned char	rtmK;		// the kind of RTM
		unsigned char	stat;		// stat
		unsigned char	datK;		// the kind of RTM data
		char	datL[3];		// data length
	};

	int ilen = sizeof(struct _rtmH);


	int remain, maxprice, compprice, nCurr, dndiff;
	remain = 21500; //이익보전
	dndiff = 1500; //최고가대비
	nCurr = 22000;
	maxprice = nCurr;

	if (remain != 0)
	{
		if (maxprice >= remain)	//최고가가 이익보전값보다 이상인 경우
		{
			compprice = maxprice - dndiff;

			if (nCurr <= compprice)		//현재가가 최고가대비 설정 가격보다 이하인 경우
			{
				remain = 0;
			}
		}
		

			if (nCurr > maxprice)
		    	maxprice = nCurr;
	}
}

double RoundDown(double data) //매체수수료(절삭)
{
	return ((int)((data * 0.1)) / 0.1);
}

//-매도수수료
void CTestDlgDlg::OnBnClickedBrnBit2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dMaip{}, m_dMass{}, m_dSave{}, symt{};
	m_dMass = 0.0010;
	CString m_maip;
	m_maip = "5690000";
	dMaip = atof(m_maip) + RoundDown((atof(m_maip) + symt) * m_dMass) + m_dSave;//매입가
}

//-mfc140
void CTestDlgDlg::OnBnClickedBtnmfc140()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileFind ff;
	CString strpath, strfile;
	((CWnd*)GetDlgItem(IDC_EDIT_DATA))->GetWindowText(strfile);
	strfile.Format("C:\\Windows\\System32\\%s", strfile);

	if (ff.FindFile(strfile))
	{
		AfxMessageBox("find!!!");
	}
	else
	{
		AfxMessageBox("no find!!!");
	}
}


void CTestDlgDlg::OnEnChangeEditData()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

inline CString FORMAT(const TCHAR* formatString, ...)
{
	CString s;
	va_list valist;
	va_start(valist, formatString);
	s.FormatV(formatString, valist);
	va_end(valist);
	return s;
}

//-foramt
void CTestDlgDlg::OnBnClickedBtnFormat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	str = FORMAT("%s|%s|%s|%s", "1", "2", "3", "4");
}


//-MAP
using _hjcode = struct hjcodex;
void CTestDlgDlg::OnBnClickedBtnMap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	std::map <CString, _hjcode*> _mapCODEx;
	std::vector<_hjcode*> _vETNx;
	std::map<CString, std::pair<CString, int>> _mapACODE;
	std::map<CString, CString>			      _mapNAME;

	CFile	file;
	CString	string = AxStd::FORMAT("%s", "hjcode3.dat");
	if (AxStd::isFileExist(string))
	{
		if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;

		const int  size = gsl::narrow_cast<int>(file.GetLength());
		m_hcB = std::make_unique<char[]>(size + 1);
		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		if (size > len)
		{
			m_hcB.reset();
			return;
		}
		int ilen = sizeof(struct hjcodex);
		const int count = len / sizeof(struct hjcodex);
		struct	hjcodex* hjc = (struct hjcodex*)m_hcB.get();

		for (int ii = 0; ii < count; ii++, hjc++)
		{
			CString sCode = CString(hjc->code, HCodeLen).Mid(1);
			_mapCODEx.emplace(std::move(sCode), hjc);
			if (hjc->nxtf == 'Y')
				m_slog.Format("\n %s is NXT", sCode);  
			else if (hjc->nxtf == 'N')
				m_slog.Format("\n %s is not NXT", sCode);
		
			OutputDebugString(m_slog);

			_mapACODE.emplace(std::move(sCode), std::move(std::make_pair(CString(hjc->hnam, HNameLen).Mid(1).Trim(), 0)));
			_mapNAME.emplace(std::move(CString(hjc->hnam, HNameLen).Mid(1).Trim()), std::move(sCode));
		} //for

	} //if

	return;

	std::map<CString, struct hjcodex*> _mapCODE;
	auto future = std::async([&]() {
		for (auto mt : _mapCODEx)
			_mapCODE.emplace(mt);
		});

	const std::chrono::milliseconds wtime(100);
	while (future.wait_for(wtime) != std::future_status::ready)
		AxStd::_Msg("Thread runing");

	std::vector<hjcodex*> vData;
	for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item) {
		/*if (item.second->kosd == jmKOSPI &&
			item.second->ssgb != jmSINJU &&
			item.second->ssgb != jmSINJS &&
			item.second->ssgb != jmHYFND &&
			item.second->ssgb != jmELW &&
			item.second->ssgb != jmFOREIGN)*/
		vData.emplace_back(item.second);
		});
}

void worker(int& counter)
{
	for (int i = 0; i < 10000; i++)
		counter += 1;
}
void workerWm(int& result, std::mutex& m)
{
	for (int i = 0; i < 10000; i++)
	{
		m.lock();
		result += 1;
		m.unlock();
	}
}


//-thread
void CTestDlgDlg::OnBnClickedBtnRace()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int  counter = 0;

	std::vector<std::thread> workers;
	for (int i = 0; i < 4; i++)
	{
		workers.push_back(std::thread(worker, std::ref(counter)));
	}

	for (int i = 0; i < 4; i++)
	{
		workers[i].join();
	}

	CString stmp;
	stmp.Format("%d", counter);
	TRACE(stmp);


	counter = 0;
	std::mutex m;
	std::vector<std::thread> workerWms;
	for (int i = 0; i < 4; i++)
	{
		workerWms.push_back(std::thread(workerWm, std::ref(counter), std::ref(m)));
	}

	for (int i = 0; i < 4; i++)
	{
		workerWms[i].join();
	}

	stmp.Format("%d", counter);
	TRACE(stmp);
}


void producer(std::queue<std::string>* downloaded_pages, std::mutex* m, int index)
{
	CString slog;
	for (int i = 0; i < 5; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));
		std::string content = "웹사이트 : " + std::to_string(i) + " from thread(" + std::to_string(index) + ")";
		slog.Format("+++++ producer [%s]\n", content.c_str());
		OutputDebugString(slog);
		m->lock();
		downloaded_pages->push(content);
		m->unlock();
	}
}

void consumer(std::queue<std::string>* downloaded_pages, std::mutex* m, int* num_processed)
{
	//전체 처리하는 페이지 개수가 5 * 5 = 25 개
	CString slog;
	while (*num_processed < 25)
	{
		m->lock();
		if (downloaded_pages->empty())
		{
			m->unlock();
			std::this_thread::sleep_for(std::chrono::microseconds(10));
			continue;
		}

		//맨 앞의 페이지를 읽고 대기 목록에서 제거한다.
		std::string content = downloaded_pages->front();
		downloaded_pages->pop();

		(*num_processed)++;
		m->unlock();

		//content를 처리한다.
		slog.Format("----consumer %s----\n", content.c_str());
		//	OutputDebugString(slog);
		std::this_thread::sleep_for(std::chrono::microseconds(80));
	}
}


//-생산자소비자
void CTestDlgDlg::OnBnClickedBtnConsumer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::queue<std::string> downloaded_pages;
	std::mutex m;

	std::vector<std::thread> producers;
	for (int i = 0; i < 5; i++)
	{
		producers.push_back(std::thread(producer, &downloaded_pages, &m, i + 1));
	}

	int num_processed = 0;
	std::vector<std::thread> consumers;

	for (int i = 0; i < 3; i++)
	{
		consumers.push_back(std::thread(consumer, &downloaded_pages, &m, &num_processed));
	}

	for (int i = 0; i < 5; i++)
		producers[i].join();

	for (int i = 0; i < 3; i++)
		consumers[i].join();

}

void producer_c(std::queue<std::string>* downloaded_pages, std::mutex* m, int index, std::condition_variable* cv)
{
	for (int i = 0; i < 5; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));
		std::string content = "웹사이트 :" + std::to_string(i) + " from thread(" + std::to_string(index) + ")\n";
		m->lock();
		downloaded_pages->push(content);
		m->unlock();

		cv->notify_one();
	}
}

void consumer_c(std::queue<std::string>* downloaded_pages, std::mutex* m, int* num_processed, std::condition_variable* cv)
{
	CString slog;
	while (*num_processed < 25)
	{
		std::unique_lock<std::mutex> lk(*m);
		cv->wait(
			lk, [&] { return !downloaded_pages->empty() || *num_processed == 25; }
		);
		if (*num_processed == 25)
		{
			lk.unlock();
			return;
		}

		//맨앞의 페이지를 읽고 대기 목록에서 제거한다.
		std::string content = downloaded_pages->front();
		downloaded_pages->pop();

		(*num_processed)++;
		lk.unlock();

		//content를 처리한다.
		slog.Format("----consumer [%s]--", content.c_str());
		OutputDebugString(slog);
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
	}
}


//-condition valueable
void CTestDlgDlg::OnBnClickedBtnConsumer2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::queue <std::string> downloaded_pages;
	std::mutex m;
	std::condition_variable cv;
	//ref vector의 신기한 활용
	std::vector<std::thread> producers;
	for (int i = 0; i < 5; i++) {
		producers.push_back(
			std::thread(producer_c, &downloaded_pages, &m, i + 1, &cv));
	}

	int num_processed = 0;
	std::vector<std::thread> consumers;
	for (int i = 0; i < 3; i++) {
		consumers.push_back(
			std::thread(consumer_c, &downloaded_pages, &m, &num_processed, &cv));
	}
	for (int i = 0; i < 5; i++) {
		producers[i].join();
	}

	cv.notify_all();

	for (int i = 0; i < 3; i++) {
		consumers[i].join();
	}


}
HANDLE m_dlgEvent;
void fworker(std::promise<string>* p)
{//약속을 이행하는 모습 해당 결과는 future 에 들어간다.
	
	m_dlgEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	ResetEvent(m_dlgEvent);

	CFileFind find;
	BOOL bfind{};

	while (1)
	{
		bfind = find.FindFile(_T("C:\\IBKS\\IBK투자증권 HTS\exe\\aaa.txt"));
		if (bfind)
			break;
	}


	CString str;
	switch (WaitForSingleObject(m_dlgEvent, 4000))
	{
	case WAIT_OBJECT_0:
	{
		str.Format("\r\n[test][%s] **********************   WAIT_OBJECT_0  *********************", __FUNCTION__);
		OutputDebugString(str);
	}
	break;
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	{
		str.Format("\r\n[test][%s]     WAIT_FAILED    WAIT_ABANDONED error=[%d]", __FUNCTION__, GetLastError());
		OutputDebugString(str);
		p->set_value("some data");
	}
	break;
	case WAIT_TIMEOUT:
	{
		p->set_value("some data");
		str.Format("\r\n[test][%s]    CDlg_Thread의  WAIT_TIMEOUT", __FUNCTION__);
		OutputDebugString(str);
	}
	break;
	default:
	{
		str.Format("\r\n[test][%s] CDlg_Thread의  default", __FUNCTION__);
		OutputDebugString(str);
	}
	break;
	}
	
}

//-future 
void CTestDlgDlg::OnBnClickedBtnFuture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	
		std::promise<string> p;

		//미래에 string 데이터를 돌려 주겠다는 약속
		std::future<string> data = p.get_future();
		std::thread t(fworker, &p);

		//미래에 약속된 데이터를 받을 때 까지 기다린다.
		data.wait(); //이코드가 실행되면 

		//wait 이 리턴했다는 뜻이 future에 데이터가 준비되었다는 의미

		CString stmp;
		stmp.Format("------[%s] ---- - \n", data.get().c_str());
		t.join();
	
}


void pcworker(std::promise<string>* p)
{
	try
	{
		throw std::runtime_error("some error");
	}
	catch (...)
	{
		p->set_exception(std::current_exception());
	}
}

/*
	auto future =  std::async([&]() {
		for (auto mt : m_pApp->_mapCODEx)
			_mapCODE.emplace(mt);
	});

	const std::chrono::milliseconds wtime(100);
	while (future.wait_for(wtime) != std::future_status::ready)
		AxStd::_Msg("Thread runing");
*/


//-future c-pattern
void CTestDlgDlg::OnBnClickedBtnFuture2()  //생산자 소비자 패턴을  future-promise로 구현
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (1)
	{
		CFileFind find;
		BOOL bfind{};
		BOOL bover{};
		auto future = std::async([&]() {
				while (1)
				{
					bfind = 1;
					Sleep(10);
					if (bfind)
						break;
					if (bover)
						break;
				}
			});

		const std::chrono::milliseconds wtime(5000);
		if (future.wait_for(wtime) != std::future_status::ready)
			bover = true;
	/*	while (future.wait_for(wtime) != std::future_status::ready)
			AxStd::_Msg("Thread runing");*/

		CString str;

	}
	else
	{
		std::promise<string> p;

		std::future<string> data = p.get_future();

		std::thread t(pcworker, &p);

		data.wait();

		try
		{
			data.get();
		}
		catch (const std::exception& e)
		{
			CString slog;
			slog.Format("except [%s]", e.what());
		}
	}
}




int test_sum(const std::vector<int>& v, int start, int end)
{
	int total = 0;

	for (int i = start; i < end; i++)
	{
		total += v[i];
	}
	return total;
}

int parallel_sum(const std::vector<int>& v)
{
	std::future<int> lower_half_future =
		std::async(std::launch::async, test_sum, cref(v), 0, v.size() / 2);

	int upper_half = test_sum(v, v.size() / 2, v.size());

	return lower_half_future.get() + upper_half;
}


//-asynce
void CTestDlgDlg::OnBnClickedBtnAsync()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::vector<int> v;
	v.reserve(1000);

	for (int i = 0; i < 1000; ++i)
	{
		v.push_back(i + 1);
	}

	int result;
	result = parallel_sum(v);


	std::vector<std::pair<CString, CString>> _va;

	CString stmp, stemp;
	for (int ii = 0; ii < 100; ii++)
	{
		stmp.Format("%d", ii);
		stemp.Format("%d", ii + 1);
		_va.emplace_back(std::make_pair(stmp, stemp));
	}

	std::vector<std::tuple<int, CString, CString>> _vta;
	int i = 0;
	auto future = std::async([&]() {
		for (auto mt : _va)
			_vta.emplace_back(std::make_tuple(i++, mt.first, mt.second));
		});

	const std::chrono::milliseconds wtime(100);
	while (future.wait_for(wtime) != std::future_status::ready)
		AxStd::_Msg("Thread runing");

}

class A
{
public:
	int idata;

	A(int ival) { idata = ival; }
	A(const  A& a) { idata = a.idata; }
};


class MyString {
	char* string_content;  // 문자열 데이터를 가리키는 포인터
	int string_length;     // 문자열 길이

	int memory_capacity;  // 현재 할당된 용량

public:
	MyString();

	// 문자열로 부터 생성
	MyString(const char* str);

	// 복사 생성자
	MyString(const MyString& str);

	void reserve(int size);
	MyString operator+(const MyString& s);
	~MyString();

	int length() const;

	void print();
	void println();
};

MyString::MyString() {
	std::cout << "생성자 호출 ! " << std::endl;
	string_length = 0;
	memory_capacity = 0;
	string_content = nullptr;
}

MyString::MyString(const char* str) {
	std::cout << "생성자 호출 ! " << std::endl;
	string_length = strlen(str);
	memory_capacity = string_length;
	string_content = new char[string_length];

	for (int i = 0; i != string_length; i++) string_content[i] = str[i];
}
MyString::MyString(const MyString& str) {
	std::cout << "복사 생성자 호출 ! " << std::endl;
	string_length = str.string_length;
	memory_capacity = str.string_length;
	string_content = new char[string_length];

	for (int i = 0; i != string_length; i++)
		string_content[i] = str.string_content[i];
}
MyString::~MyString() { delete[] string_content; }
void MyString::reserve(int size) {
	if (size > memory_capacity) {
		char* prev_string_content = string_content;

		string_content = new char[size];
		memory_capacity = size;

		for (int i = 0; i != string_length; i++)
			string_content[i] = prev_string_content[i];

		if (prev_string_content != nullptr) delete[] prev_string_content;
	}
}
MyString MyString::operator+(const MyString& s) {
	MyString str;
	str.reserve(string_length + s.string_length);
	for (int i = 0; i < string_length; i++)
		str.string_content[i] = string_content[i];
	for (int i = 0; i < s.string_length; i++)
		str.string_content[string_length + i] = s.string_content[i];
	str.string_length = string_length + s.string_length;
	return str;
}
int MyString::length() const { return string_length; }
void MyString::print() {
	for (int i = 0; i != string_length; i++) std::cout << string_content[i];
}
void MyString::println() {
	for (int i = 0; i != string_length; i++) std::cout << string_content[i];

	std::cout << std::endl;
}

int main() {
	MyString str1("abc");
	MyString str2("def");
	std::cout << "-------------" << std::endl;
	MyString str3 = str1 + str2;
	str3.println();
}

//-right reference
void CTestDlgDlg::OnBnClickedBtnRlef()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	A a(1);
	A b(a);
}


//-pair
void CTestDlgDlg::OnBnClickedBtnPair()
{
	std::map< CString, std::pair<CString, int>> _mapACODE;
	std::map <CString, CString> _mapNAME;

	CFile file;
	CString string = AxStd::FORMAT("%s", "hjcode3.dat");
	if (AxStd::isFileExist(string))
	{
		if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;

		const int size = gsl::narrow_cast<int>(file.GetLength());
		m_hcB = std::make_unique<char[]>(size + 1);

		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		m_slog.Format("%s", m_hcB.get());
	}

	/*std::map<CString, std::pair<CString, int>> _mapACODE;
	std::map<CString, CString> _mapNAME;

	CFile	file;
	CString	string = AxStd::FORMAT("%s", "hjcode3.dat");
	if (AxStd::isFileExist(string))
	{
		if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;

		const int size = gsl::narrow_cast<int>(file.GetLength());
		m_hcB = std::make_unique<char[]>(size + 1);
		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		if (size > len)
		{
			m_hcB.reset();
			return;
		}

		const int count = len / sizeof(struct hjcodex);
		struct hjcodex* hjc = (struct hjcodex*)m_hcB.get();

		for (int ii = 0; ii < count; ii++, hjc++)
		{
			CString sCode = CString(hjc->code, HCodeLen).Mid(1);
			_mapACODE.emplace(std::move(sCode), std::move(std::make_pair(CString(hjc->hnam, HNameLen).Mid(1).Trim(), 0)));
			_mapNAME.emplace(std::move(CString(hjc->hnam, HNameLen).Mid(1).Trim()), std::move(sCode));
		}

	}*/
}
//------------------------------------------------------------------------------------------------------------------------------------------
struct _efopcode
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	code[6]{};			/*종목코드*/
	char	hnam[39]{};			/*종목명*/
	char	opcd[6]{};			/*운용사 코드*/
	char	etfm[1]{};			/*테마코드*/
	char	etfl[1]{};			/*국내외 구분*/
};

struct _efopitem
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	opcd[6]{};			/*운용사 코드*/
	char	hnam[30]{};			/*운용사 한글약명*/
	char	lnfd[1]{};			/*라인피드*/

	//	_efopcode *efopcode;
};


struct _efoptema
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	tmcd[1]{};			/*테마코드*/
	char	tmnm[20]{};			/*테마명*/
	char	lnfd[1]{};			/*라인피드*/
};

struct _efopfore
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	frcd[1]{};			/*국내외 구분('D':국내, 'F':해외)*/
	char	frnm[10]{};			/*국내외 구분명*/
	char	lnfd[1]{};			/*라인피드*/
};

//void test_compare(std::vector<std::pair<CString, CString>> a, std::vector<std::pair<CString, CString>> b)
template <typename T>
void test_compare()
{
	CString tmp, temp;
	bool operator()(const T & a, const T & b)  const {}
}


//-tuple
void CTestDlgDlg::OnBnClickedBtnTuple()
{
	std::vector<std::tuple<char, CString, CString>>	_vETFitem;
	std::vector<std::tuple<char, char, CString>>	_vETFtema;
	std::vector<std::tuple<char, char, CString>>	_vETFfore;
	std::vector<std::tuple<char, CString, CString, CString, char, char>> _vETFcode;

	CString sfile;
	sfile.Format("%s", "efcode.dat");

	CStdioFile cfile(sfile, CFile::modeRead);

	CString line;
	while (cfile.ReadString(line))
	{
		switch (const char gubn = line.GetAt(0); gubn)
		{
		case '1':
		{
			const _efopitem* efopitem = (struct _efopitem*)line.GetBuffer();
			_vETFitem.emplace_back(std::move(std::tuple(efopitem->type[0],
				CString(efopitem->opcd, sizeof(efopitem->opcd)).Trim(),
				CString(efopitem->hnam, sizeof(efopitem->hnam)).Trim())));
		}
		break;
		case '2':
		{
			const _efoptema* efoptema = (struct _efoptema*)line.GetBuffer();
			_vETFtema.emplace_back(std::move(std::tuple(efoptema->type[0],
				efoptema->tmcd[0],
				CString(efoptema->tmnm, sizeof(efoptema->tmnm)).Trim())));
		}
		break;
		case '3':
		{
			const _efopfore* efopfore = (struct _efopfore*)line.GetBuffer();
			_vETFfore.emplace_back(std::move(std::tuple(efopfore->type[0], efopfore->frcd[0],
				CString(efopfore->frnm, sizeof(efopfore->frnm)).Trim())));
		}
		break;
		case '4':
		{
			const _efopcode* efopcode = (struct _efopcode*)line.GetBuffer();
			_vETFcode.emplace_back(std::move(std::tuple(efopcode->type[0],
				CString(efopcode->code, sizeof(efopcode->code)).Trim(),
				CString(efopcode->hnam, sizeof(efopcode->hnam)).Trim(),
				CString(efopcode->opcd, sizeof(efopcode->opcd)).Trim(),
				efopcode->etfm[0],
				efopcode->etfl[0]
			)));
		}
		break;
		}
	}

	std::vector <std::pair<CString, CString>> _vData;
	for_each(_vETFcode.begin(), _vETFcode.end(), [&](const auto item) {
		auto [type, code, hnam, opcd, etfm, etfl] = item;
		_vData.emplace_back(std::move(std::make_pair(code, hnam)));
		});

	std::sort(_vData.begin(), _vData.end());

	CString name;
	for_each(_vData.begin(), _vData.end(), [&](const auto item) {
		name = item.second;
		auto [CString, CSting] = item;
		name.MakeUpper();
		});

	auto item = _vData.at(0);
	CString strdata;
	strdata = std::get<0>(item);
	/*
	for_each(vSearch.begin(), vSearch.end(), [&](const auto item){
		name = item.second;
		name.MakeUpper();
		sName.MakeUpper();
		if (name.Find(sName) >= 0)
			vText.emplace_back(item);
	});
	*/
}

template <typename Iter>
void testprint(Iter begin, Iter end)
{
	OutputDebugString("--------------------------\n");
	CString stmp;
	while (begin != end)
	{
		stmp.Format(" %d\n", *begin);
		OutputDebugString(stmp);
		begin++;
	}
}


//-lamda
void CTestDlgDlg::OnBnClickedBtnLamda()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	auto func = [](int i) { return i % 2 == 1; };
//	auto ret = func(4);


	std::vector<int> vec;
	vec.push_back(5);
	vec.push_back(3);
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(6);
	vec.push_back(4);

	testprint(vec.begin(), vec.end());

	//정렬코드
	//std::sort(vec.begin(), vec.end());
	//testprint(vec.begin(), vec.end());

	std::partial_sort(vec.begin(), vec.begin() + 3, vec.end());  //이렇게 부분정렬을 하는 이유는 상위 10% 같은거 할때 시간낭비없이 좋다
	testprint(vec.begin(), vec.end());

	std::vector<int>::iterator itr = vec.begin();
	int inum;
	for (; itr != vec.end(); itr++)
	{
		inum = *itr;
		m_slog.Format("\r\n--- %d", inum);
		OutputDebugString(m_slog);
	}



	int num_erased = 0;
	vec.erase(std::remove_if(vec.begin(), vec.end(),
		[&num_erased](int i) {
			if (num_erased >= 2)
				return false;
			else if (i % 2 == 1) {
				num_erased++;
				return true;
			}
			return false;
		}),
		vec.end());
	testprint(vec.begin(), vec.end());



	static auto SetProcessDpiAwarenessContextfunc = []() {
		return reinterpret_cast<decltype(&::SetProcessDpiAwarenessContext)>(
			::GetProcAddress(GetUser32Module(), "SetProcessDpiAwarenessContext"));
	}();

	if (SetProcessDpiAwarenessContextfunc)
	{
		// Windows 10 1703+: SetProcessDpiAwarenessContext
		SetProcessDpiAwarenessContextfunc(DPI_AWARENESS_CONTEXT_UNAWARE);
	}

	std::cout << "\r\ntest\n" << std::endl;
	printf("\r\n tsettset");
}

HMODULE CTestDlgDlg::GetUser32Module()
{
	static HMODULE user32Module = nullptr;
	if (user32Module == nullptr)
	{
		user32Module = LoadLibraryA("User32.dll");
	}
	return user32Module;
}

void getHjcodeMap(std::map<CString, _hjcode>& map)
{
	std::map <CString, _hjcode*> _mapCODEx;
	std::vector<_hjcode*> _vETNx;
	std::map<CString, std::pair<CString, int>> _mapACODE;
	std::map<CString, CString>			      _mapNAME;
	std::unique_ptr<char[]> m_hcB;

	CFile	file;
	CString	string = AxStd::FORMAT("%s", "hjcode3.dat");
	if (AxStd::isFileExist(string))
	{
		if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;

		const int  size = gsl::narrow_cast<int>(file.GetLength());
		m_hcB = std::make_unique<char[]>(size + 1);
		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		if (size > len)
		{
			m_hcB.reset();
			return;
		}

		const int count = len / sizeof(struct hjcodex);
		struct	hjcodex* hjc = (struct hjcodex*)m_hcB.get();

		for (int ii = 0; ii < count; ii++, hjc++)
		{
			CString sCode = CString(hjc->code, HCodeLen).Mid(1);
			_mapCODEx.emplace(std::move(sCode), hjc);

			map.emplace(sCode, *hjc);

			//	_mapACODE.emplace(std::move(sCode), std::move(std::make_pair(CString(hjc->hnam, HNameLen).Mid(1).Trim(), 0)));
			//	_mapNAME.emplace(std::move(CString(hjc->hnam, HNameLen).Mid(1).Trim()), std::move(sCode));
		} //for
	} //if

	//std::map<CString, struct hjcodex*> _mapCODE;
	//auto future = std::async([&]() {
	//	for (auto mt : _mapCODEx)
	//		_mapCODE.emplace(mt);
	//	});

	//const std::chrono::milliseconds wtime(100);
	//while (future.wait_for(wtime) != std::future_status::ready)
	//	AxStd::_Msg("Thread runing");

}

//-lamda find
void CTestDlgDlg::OnBnClickedBtnLamdaFind()
{
	std::map<CString, _hjcode> _map;
	getHjcodeMap(_map);
	//strdata = std::get<0>(item);const auto hjc
	std::vector< _hjcode> _vt;
	CString name;
	for (auto& item : _map)
	{
		_hjcode hjc = item.second;
		name.Format("%s", hjc.hnam);

		if( name.Find("타") >= 0)
		{
			_vt.emplace_back(std::move(hjc));
		}
	}
	//auto it  = std::remove_if(_map.begin(), _map.end(), [&](const auto item)->bool{
	//		name = CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1);
	//	
	//		CString strUpperS, strUpperO;
	//		strUpperS = cName;
	//		strUpperO = name.GetAt(ii);
	//		strUpperS.MakeUpper();
	//		strUpperO.MakeUpper();

	//		if (strUpperS != strUpperO)
	//			return true;
	//		return false;
	//	});

}

//-OS BIT
typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;
void CTestDlgDlg::OnBnClickedBtnOs()
{
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	
	if (fnIsWow64Process == NULL)
		AfxMessageBox("check fail");

	bool bProcess64 = false;
#if defined(_WIN64)
	bProcess64 = FALSE;
#elif defined(_WIN32)
	BOOL f64 = FALSE;
	bProcess64 = fnIsWow64Process(GetCurrentProcess(), &f64) && f64;
#endif

}

//-OS version
void CTestDlgDlg::OnBnClickedBtnOsversion()
{
	DWORD major, minor;
	GetWindowsVersionfuck(major, minor);
}

UINT AFXAPI Hashdata(LPCSTR key)
{
	UINT nHash = 0;
	while (*key)
		nHash = (nHash << 5) + nHash + *key++;
	return nHash;
}


//-HASHKEY
void CTestDlgDlg::OnBnClickedBtnHashkey()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sname;
	sname = "131122";
	CString sdata;
	sdata.Format("%08u", Hashdata((LPCSTR)sname));
	
	//	sdata.Format("%08u", HashKey((LPCSTR)sname));
	int idata = 300000000;
}

//-wnd add
void CTestDlgDlg::OnBnClickedBtnWin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	long data = (long)this;
	CString sdata;
//	sdata.Format("%x", data); 
//	int idata = sizeof(CWnd*);
	((CWnd*)data)->SendMessage(WM_CLOSE, 0, 0);
}

// LPTSTR pszFilePath : 실행파일경로  및 파일명.확장자
// LPTSTR pszTitle :  \\파일명.lnk
// LPTSTR pszDescription : 표시할 툴팁
// BOOL bDesktop : 바탕화면 생성 유무 TRUE., FALSE
// BOOL bQuickLanch : quicklaunch 생성 유무 TRUE., FALSE
// BOOL bStartMenu : 시작메뉴 생성 유무 TRUE., FALSE
// BOOL bOverWrite :  기존 구성 덮어쓰기 유무 TRUE., FALSE

BOOL CTestDlgDlg::CreateShortcut(LPTSTR pszFilePath, LPTSTR pszTitle, LPTSTR pszDescription, BOOL bDesktop, BOOL bQuickLanch, BOOL bStartMenu, BOOL bOverWrite)
{
	char pBuffer[MAX_PATH];
	SHGetSpecialFolderPath(GetSafeHwnd(), (LPSTR)pBuffer, CSIDL_DESKTOP, 0);
	//SHGETFOLDERPATH fnShGetFolderPath;
/*


	if (!bDesktop && !bQuickLanch && !bStartMenu)  
		return TRUE;


	HMODULE hModule = LoadLibrary("SHFOLDER.DLL");

	if (hModule != NULL) {

		fnShGetFolderPath = (SHGETFOLDERPATH)GetProcAddress(hModule, "SHGetFolderPathA");


		if (fnShGetFolderPath == NULL) {

			FreeLibrary(hModule);

			//MessageBox("아이콘 등록에 실패하였습니다. Shell Version이 낮습니다.", "에러", MB_ICONSTOP);

			return FALSE;

		}

	}



	//  실제 대상파일이 존재하는지 체크하는 루틴추가.

	//if( !IsFileExist( pszFilePath ))  return TRUE;



	CString strTempPath;

	// 아이콘의 이름 설정이 안되었을 경우 기본 이름으로 설정 : *.exe의 바로가기

	if (pszTitle == NULL) {

		TCHAR szFileName[MAX_PATH];

		memset(szFileName, 0x00, sizeof(szFileName));



		TCHAR szFilePath[MAX_PATH];

		memset(szFilePath, 0x00, sizeof(szFilePath));



		lstrcpy(szFilePath, pszFilePath);



		TCHAR* pszToken;

		pszToken = strtok(szFilePath, _T("\\"));



		while (pszToken != NULL) {

			lstrcpy(szFileName, pszToken);

			pszToken = strtok(NULL, _T("\\"));

		}



		strTempPath.Format(_T("\\%s%s"), szFileName, _T("의 바로가기.lnk"));

		pszTitle = strTempPath.GetBuffer(strTempPath.GetLength());

	}



	BOOL bResult = TRUE;



	HRESULT hResult;

	IShellLink* pShellLink;

	IPersistFile* pPersistFile;



	TCHAR szDesktopPath[MAX_PATH];

	TCHAR szQuickLanchPath[MAX_PATH];

	TCHAR szStartMenuPath[MAX_PATH];



	memset(szDesktopPath, 0x00, sizeof(szDesktopPath));

	memset(szQuickLanchPath, 0x00, sizeof(szQuickLanchPath));

	memset(szStartMenuPath, 0x00, sizeof(szStartMenuPath));



	// 바탕화면 아이콘이 저장될 경로 설정

	//SHGetSpecialFolderPath(NULL, szDesktopPath, MAX_PATH );

	if (FAILED(fnShGetFolderPath(NULL, CSIDL_DESKTOP, NULL, 0, szDesktopPath)))  return FALSE;

	LPSTR strDesktopPath = lstrcat(szDesktopPath, pszTitle);



	// 빠른 실행 아이콘이 저장될 경로 설정

	if (FAILED(fnShGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szQuickLanchPath)))  return FALSE;

	LPSTR strQuickLanchPath = lstrcat(szQuickLanchPath, _T("\\Microsoft\\Internet Explorer\\Quick Launch"));

	strQuickLanchPath = lstrcat(szQuickLanchPath, pszTitle);



	// 시작메뉴

	if (FAILED(fnShGetFolderPath(NULL, CSIDL_PROGRAMS, NULL, 0, szStartMenuPath)))  return FALSE;

	LPSTR strStartMenuPath = lstrcat(szStartMenuPath, pszTitle);



	if (!bOverWrite) {

		// 바탕화면 아이콘이 있으면 

		if (IsFileExist((LPCTSTR)strDesktopPath))		bDesktop = FALSE;

		// 빠른실행 아이콘이 있으면 

		if (IsFileExist((LPCTSTR)szQuickLanchPath))		bQuickLanch = FALSE;

		// 시작메뉴  아이콘이 있으면 

		if (IsFileExist((LPCTSTR)szStartMenuPath))		bStartMenu = FALSE;



		if (!bDesktop && !bQuickLanch && !bStartMenu)		return TRUE;

	}



	// COM Object 초기화

	CoInitialize(NULL);



	// IShellLink 객체를 생성하고 포인터를 구함

	hResult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pShellLink);



	if (SUCCEEDED(hResult)) {

		// 단축아이콘의 대상체와 설명을 설정한다.



		CString exe_dir_str;

		TCHAR drive[_MAX_DRIVE] = _T(""), dir[_MAX_DIR] = _T("");

		TCHAR fname[_MAX_FNAME] = _T(""), ext[_MAX_EXT] = _T("");

		_tsplitpath(pszFilePath, drive, dir, fname, ext);

		exe_dir_str.Format("%s%s", drive, dir);



		pShellLink->SetPath(pszFilePath);

		pShellLink->SetDescription(pszDescription);

		pShellLink->SetArguments(_T(""));

		pShellLink->SetWorkingDirectory(exe_dir_str);



		// 저장하기 위해 IPersistFile 객체 생성

		hResult = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pPersistFile);



		// 유니코드로 파일 패스를 변경한 후 저장

		if (SUCCEEDED(hResult)) {



			WORD wszDesktop[MAX_PATH];

			memset(wszDesktop, 0x00, sizeof(wszDesktop));

			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strDesktopPath, -1, LPWSTR(wszDesktop), MAX_PATH);



			WORD wszQuickLanch[MAX_PATH];

			memset(wszQuickLanch, 0x00, sizeof(wszQuickLanch));

			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strQuickLanchPath, -1, LPWSTR(wszQuickLanch), MAX_PATH);



			WORD wszStartMenu[MAX_PATH];

			memset(wszStartMenu, 0x00, sizeof(wszStartMenu));

			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strStartMenuPath, -1, LPWSTR(wszStartMenu), MAX_PATH);





			// shortcut 등록

			if (bDesktop) {

				if (!pPersistFile->Save(LPWSTR(wszDesktop), TRUE))	 bResult = FALSE;

			}



			//Quick Lauch에 등록 

			if (bQuickLanch) {

				if (!pPersistFile->Save(LPWSTR(wszQuickLanch), TRUE))	bResult = FALSE;

			}



			//bStartMenu에 등록 

			if (bStartMenu) {

				if (!pPersistFile->Save(LPWSTR(wszStartMenu), TRUE))	bResult = FALSE;

			}



			pPersistFile->Release();



			// 아이콘 재설정

			::SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST | SHCNF_FLUSH, NULL, NULL);

		}

		pShellLink->Release();



		//COM 해제

		CoUninitialize();

		FreeLibrary(hModule);

		return bResult;
	
	}
	else {

		FreeLibrary(hModule);

	}

	return bResult;
	*/
return FALSE;
}


//-아이콘
void CTestDlgDlg::OnBnClickedBtnIcon()
{
	//CString strpath;          //  %systemdrive%\users\%username%\desktop
	//strpath.Format("del %s",  "\"%systemdrive%\users\%username%\desktop\IBK hot Trading.lnk\"");
	//ShellExecute(NULL, _T("open"), "C:\\test.bat", NULL, NULL, SW_SHOWNORMAL);
	//ShellExecute(NULL, strpath, NULL, NULL, NULL, SW_SHOWNORMAL);
	//ShellExecute(NULL, _T("open"), "C:\\test.bat", NULL, NULL, SW_SHOWNORMAL);
	//char pBuffer[MAX_PATH];
	//SHGetSpecialFolderPath(GetSafeHwnd(), (LPSTR)pBuffer, CSIDL_DESKTOP, 0);

	//SHGETFOLDERPATH fnShGetFolderPath;
	//HMODULE hModule = LoadLibrary("SHFOLDER.DLL");
	//if (hModule != NULL) {

	//	fnShGetFolderPath = (SHGETFOLDERPATH)GetProcAddress(hModule, "SHGetFolderPathA");



	//	if (fnShGetFolderPath == NULL) {

	//		FreeLibrary(hModule);

	//		//MessageBox("아이콘 등록에 실패하였습니다. Shell Version이 낮습니다.", "에러", MB_ICONSTOP);

	//		return FALSE;

	//	}

	}
	



bool GetProcessModule(DWORD dwPID, char* sProcessName,  char* pProcess)
{
	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32 = { 0 };
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	CString stmp;
	stmp.Format("%s", pProcess);
	if (stmp.Find("axis.exe") >= 0)
		TRACE("123");

	if (hModuleSnap == (HANDLE)-1)
		return (FALSE);

	me32.dwSize = sizeof(MODULEENTRY32);
	CString slog;
	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면
	
	if (Module32First(hModuleSnap, &me32))
	{
		do
		{
			printf("process name : %s\n", me32.szModule);
			slog.Format("[mac] GetProcessModule =[%s] \r\n", me32.szModule);
			//OutputDebugString(slog);
			if (strcmp(me32.szModule, sProcessName) == 0)
			{
				CloseHandle(hModuleSnap);
				return true;
			}
		} while (Module32Next(hModuleSnap, &me32));
	}

	CloseHandle(hModuleSnap);
	return false;
}
//
HANDLE CTestDlgDlg::ProcessFind(char* strProcessName)
{

	HANDLE         hProcessSnap = NULL;
	BOOL           bRet = FALSE;
	PROCESSENTRY32 pe32 = { 0 };
	CString strProcess, strTarget;
	strTarget.Format("%s", strProcessName);
	strTarget.TrimRight();

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == (HANDLE)-1)
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	CString slog;
	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32))
	{
		BOOL          bCurrent = FALSE;
		MODULEENTRY32 me32 = { 0 };

		do
		{
		//	bCurrent = GetProcessModule(pe32.th32ProcessID, strProcessName, pe32.szExeFile);
			slog.Format("[MAC] [%s] \r\n", pe32.szExeFile);
			OutputDebugString(slog);
			OutputDebugString("\r\n-----------------------------------------------\r\n");
			strProcess.Format("%s", pe32.szExeFile);
			if (strProcess.Find(strTarget) >= 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				//if (hProcess)
				//	TerminateProcess(hProcess, 0);
				return hProcess;
			}
		} while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	}
	CloseHandle(hProcessSnap);

	return NULL;
}


//-프로세스
void CTestDlgDlg::OnBnClickedBtnPrsfind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HANDLE hwnd = ProcessFind("axis.exe");
	if (hwnd)
	{
		TerminateProcess(hwnd, 0);
	}
	CString str;
}



//-std::copy
void CTestDlgDlg::OnBnClickedBtnStdcopy()
{
	int inum = 9600;
	CString  stest;
	stest = "1";

	if (stest == "0" && ((inum >= 7600) && (inum <= 9600)))
		TRACE("TEST");


	std::vector<std::shared_ptr<_intersx>> m_inters;
	std::vector<std::shared_ptr<_intersx>> m_Inters;

	CString stmp;
	for (int ii = 0; ii < 10; ii++)
	{
		auto& pdata = m_inters.emplace_back(std::make_unique< _intersx>());
		stmp.Format("%d", ii);
		pdata->code = stmp;
	}


	for (int ii = 0; ii < 2; ii++)
	{
		const auto& vInters = m_inters;
		const int count = 10;
		std::copy(vInters.begin(), vInters.begin() + count, std::back_insert_iterator(m_Inters));
	}
}


//-isHexnumeric
void CTestDlgDlg::OnBnClickedBtnHex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str, stemp;
	str = "1234a";
	if ((str.GetAt(0) < '0' || str.GetAt(0) > '9') && (str.GetAt(0) < 'A' || str.GetAt(0) > 'G'))
		OutputDebugString(str);
	OutputDebugString("123");

	for (int ii = 1; ii < str.GetLength(); ii++)
		if ((str.GetAt(ii) < '0' || str.GetAt(ii) > '9') &&
			(str.GetAt(ii) < 'a' || (str.GetAt(ii) > 'z' && str.GetAt(ii) != 's')) &&
			(str.GetAt(ii) < 'A' || (str.GetAt(ii) > 'Z' && str.GetAt(ii) != 'S')))
			return ;
	return ;

	for (int ii = 1; ii < str.GetLength(); ii++)
	{
		stemp.Format("\r\n%c",str.GetAt(ii));
		OutputDebugString(stemp);
		if (str.GetAt(ii) < '0' || str.GetAt(ii) > '9')
			OutputDebugString("!!!!!\r\n");
		if (str.GetAt(ii) < 'a' || (str.GetAt(ii) > 'z' && str.GetAt(ii) != 's'))
			OutputDebugString("#####\r\n");
		if (str.GetAt(ii) < 'A' || (str.GetAt(ii) > 'Z' && str.GetAt(ii) != 'S'))
			OutputDebugString("&&&&\r\n");
	}
	
	
}

//-파생코드
void CTestDlgDlg::OnBnClickedBtnCode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char ch1 = '1';

	switch (ch1)
	{
	case '1':
	case 'D':
	{
		TRACE("123");
	}
	break;
	}
}

//-IE
void CTestDlgDlg::OnBnClickedBtnIe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strurl;
	//strurl = "microsoft - edge:"; 
	strurl = "https://www.ibks.com/LoadService.jsp?url=/event/event_overseasExchange.jsp&popup=Y&auth=_q579%3A%2B9%3E%3A%262%3A8%2C%3E%3A%2Ccai%3E%2Clv3%2F%3A%3A%2F8%2F9%3D1909%3B4.jn3g%60p%3B%3D9.q%7B31w%2F%7B9e&keyB_YN=Y&nPro_YN=N&aos_YN=N";
	//strurl += "naver.com";
	ShellExecute(NULL, "open", "chrome.exe" , strurl, NULL, SW_SHOWNORMAL);
	
}

//using namespace name_test;


#define	vtypeMSK	0xf0		
#define	vtypeERR		0x00
#define	vtypeNRM		0x01
#define	vtypeVBX		0x02
#define	vtypeDLL		0x03
#define	vtypeGRX		0x04
#define	vtypeHTM		0x05


void Writelog(int scnt, ...)
{
	va_list argList;
	va_start(argList, scnt);

	CString sTmp, sResult;
	int icnt = scnt;
	for (int i = 0; i < icnt; i++)
	{
		sTmp.Format("[%s]", va_arg(argList, LPCTSTR));
		sResult += sTmp;
		sResult += "   ";
	}
	va_end(argList);

	OutputDebugString(sResult);
}

struct pidouini_item
{
	char usid[8];   /* 사용자 ID                */
	char innm[100];  /* INI 파일명               */
	char senm[100];  /* 섹션명                   */
	char skey[100];  /* Key                      */
	char valu[500];  /* Value                    */
	char date[8];   /* 업데이트일자             */
};

struct pidouini_mid
{
	char gubn[1];
	struct pidouini_item item;
};
#define L_pidouini_mid		sizeof(struct pidouini_mid)

struct user_th
{
	char trxc[8];
	char key;
	char stat;
};
#define L_user_th sizeof(struct user_th)

void CTestDlgDlg::SendTR(LPCSTR trnm, int tkey, int stat, LPCSTR data, int size)
{
	//vector<char> buff(L_user_th + size);
	char* buff = new char[L_user_th + size];
	memset(buff, ' ', L_user_th + size);
	struct user_th* th = (struct user_th*)&buff[0];
	th->key = (char)tkey;
	th->stat = (char)stat;
	memcpy(th->trxc, trnm, 8);
	memcpy(&buff[L_user_th], data, size);

	CString stmp;
	stmp.Format("%s", data);
	int ilen = stmp.GetLength();
}


//-nt2CommaStr
void CTestDlgDlg::OnBnClickedBtnInt2str()
{
	struct pidouini_mid* pmid;
	char* pdata = new char[L_pidouini_mid];
	memset(pdata, ' ', L_pidouini_mid);
	pmid = (struct pidouini_mid*)pdata;
	pmid->gubn[0] = 'Q';
	memcpy(pmid->item.usid, "test", 4 );

	memcpy(pmid->item.innm, "IBKSCONNECTOR", 13 );
	memcpy(pmid->item.senm, "ALLOW_USER", 10);
	memcpy(pmid->item.skey,  "ENABLE", 6);
	memcpy(pmid->item.date, "1234567", 6);

	// 
	//struct pidouini_mid mid;
	//ZeroMemory(&mid, L_pidouini_mid);
	//mid.gubn[0] = 'Q';
	//sprintf_s(mid.item.usid, "%s", (LPCSTR)"test");
	//sprintf_s(mid.item.innm, "IBKSCONNECTOR");
	//sprintf_s(mid.item.senm, "ALLOW_USER");
	//sprintf_s(mid.item.skey, "ENABLE");
	//sprintf_s(mid.item.date, "1234567");

	SendTR("pidouini", 1, 0, (LPCSTR)pmid, L_pidouini_mid);


	
	/*CString slog;
	Writelog(3, __FUNCTION__ , "123", "3333");*/
	//long ii = MAKELONG(0x05, 36);
	////HIWORD(kind) & 0xff00
	//WORD wd = HIWORD(ii);
	//wd = HIWORD(ii) & 0xff00;
	////iname = 0;
	//
	//int isize = sizeof(WORD);
	//int idata = 0xf0;
	//idata = ~idata;

	//int i = 0;;
	//double d = 0.0;
	// idata = (int)d;
	////if (d == 0)
	////	return;
	// idata = i / d;
	//CString str;

	//int  type = 33;
	//idata = type & ~vtypeMSK;
	//switch (type & ~vtypeMSK)
	//{
	//case vtypeNRM: 
	//	TRACE("0");
	//	break;
	//case vtypeDLL:  
	//	TRACE("1");
	//	break;
	//case vtypeGRX: 
	//	TRACE("2");
	//	break;
	//case vtypeVBX: 
	//	TRACE("3");
	//	break;
	//}

	//switch (type )
	//{
	//case vtypeNRM:
	//	TRACE("0");
	//	break;
	//case vtypeDLL:
	//	TRACE("1");
	//	break;
	//case vtypeGRX:
	//	TRACE("2");
	//	break;
	//case vtypeVBX:
	//	TRACE("3");
	//	break;
	//}

	
	int val{};
	val = 12345678;
	char dst[32]{}, src[32]{};
	char* buf{};
	char* str{};

	int len{};
	len = sprintf_s(src, "%d", val);

	if (val < 0)
	{
		dst[0] = '-'; 
		str = &src[1];
		buf = &dst[1];
		--len;
	}
	else
	{
		str = &src[0];
		buf = &dst[0];
	}

	int ival = (len - 1) % 3;
	switch ((len - 1) % 3) 
	{
	case 3: // fake label to make gcc happy 
		while (*str) {
			*buf++ = ',';
	case 2: *buf++ = *str++;
	case 1: *buf++ = *str++;
	case 0: *buf++ = *str++;
		}
	}
	*buf = '\0';
}

//- remove icon
void CTestDlgDlg::OnBnClickedBtnRemoveicon()
{
		// 바탕화면 핸들 검색
		HWND hDesktopWnd = ::GetDesktopWindow();

		// 바탕화면의 첫 번째 아이콘 핸들 검색
		HWND hIconWnd = ::FindWindowEx(hDesktopWnd, NULL, "WorkerW", NULL);
		if (hIconWnd)
		{
			// 바탕화면 창 핸들의 부모 핸들 검색
			HWND hDesktopParent = ::GetParent(hDesktopWnd);

			// 부모 핸들의 프로세스 ID 검색
			DWORD dwProcessId = 0;
			::GetWindowThreadProcessId(hDesktopParent, &dwProcessId);

			// 프로세스 핸들 검색
			HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
			if (hProcess)
			{
				// 프로세스의 모듈 핸들 검색
				HMODULE hModule = nullptr;
				DWORD dwNeeded = 0;
				if (::EnumProcessModules(hProcess, &hModule, sizeof(hModule), &dwNeeded))
				{
					// 모듈 경로 검색
					WCHAR szPath[MAX_PATH] = { 0 };
					::GetModuleFileNameExW(hProcess, hModule, szPath, MAX_PATH);

					// Windows 탐색기 프로세스인지 확인
				//	if (::wcsstr(szPath, "explorer.exe"))
					{
						// 바탕화면 아이콘 리스트 뷰 핸들 검색
						HWND hIconListView = ::FindWindowEx(hIconWnd, NULL, "SysListView32", NULL);
						if (hIconListView)
						{
							// 바탕화면 아이콘 핸들 검색
							HWND hTargetIcon = ::FindWindowEx(hIconListView, NULL, "SysListView32", "IBK hot Trading");
							if (hTargetIcon)
							{
								// 아이콘 핸들에 대해 삭제 메시지 보내기
								::SendMessage(hIconListView, LVM_DELETEITEM, 0, (LPARAM)hTargetIcon);
							}
						}
					}
				}
				::CloseHandle(hProcess);
			}
		}
}

//-CEF
void CTestDlgDlg::OnBnClickedBtnCef()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//-serial_write
void CTestDlgDlg::OnBnClickedBtnSerial()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//"C:\IBKS\IBK투자증권 HTS\user\1172747575\Chart"
	CString strFileName;
	//strFileName = "C:\\IBKS\\IBK투자증권 HTS\\user\\1172747575\\Chart\\OrderLine1.cfg";
	strFileName = "C:\\IBKS\\IBK투자증권 HTS\\user\\1172747575\\Chart\\test.txt";
	int data1 = 5;
	double data2 = 12.3;
	CString str1, str2, str3;
	str1 = "문자열입니다1";
	str2 = "문자열입니다2";
	str3 = "문자열입니다3";

	CStringList strlist;
	strlist.AddTail(str1);
	strlist.AddTail(str2);
	strlist.AddTail(str3);

	CFile  file;
	// 파일을 쓰기모드로 연다.
	if (file.Open(strFileName, CFile::modeCreate | CFile::modeWrite)) {
		// 정상적으로 열린 파일을 CArchive 에 저장하기 모드로 전달한다.
		CArchive ar(&file, CArchive::store);

		try {
			//// 상수를 저장한다.
			//ar << str1;
			//// int 형 데이터를 저장한다.
			//ar << str2;
			//// double 형 데이터를 저장한다.
			//ar << str3;
			//// 문자열 데이터를 저장한다. char 배열은 << 연산자를 사용할 수 없다.
			//ar << str3;
			strlist.Serialize(ar);
		}
		catch (CFileException* fe) {
			// 예외가 발생하면 메세지박스를 통하여 사용자에게 알린다.
			fe->ReportError();
		}
		catch (CArchiveException* ae) {
			// 예외가 발생하면 메세지박스를 통하여 사용자에게 알린다.
			ae->ReportError();
		}
		// CArchive 를 닫는다.
		ar.Close();
		// 파일을 닫는다.
		file.Close();
	}

	
}

//-serial_read
void CTestDlgDlg::OnBnClickedBtnSerial2()
{
	int temp;
	int data1;
	double data2;
	CString str, str1;
	CString strFileName;
	strFileName = "C:\\IBKS\\IBK투자증권 HTS\\user\\1172747575\\Chart\\OrderLine.cfg";
	//strFileName = "C:\\IBKS\\IBK투자증권 HTS\\user\\1172747575\\Chart\\test.txt";
	CFile  file;
	// 파일을 읽기모드로 연다.
	if (file.Open(strFileName, CFile::modeRead)) {
		// 정상적으로 열린 파일을 CArchive 에 불러오기 모드로 전달한다.
		CArchive ar(&file, CArchive::load);
		CStringList strlist;
		try {
			// 상수를 불러오기 한다. 저장된 데이터 타입이 정수인 경우 temp 변수는 int 형이어야 하고,
			// 실수형 데이터인 경우 double 형이어야 한다.
			strlist.Serialize(ar);
		}
		catch (CFileException* fe) {
			// 예외가 발생하면 메세지박스를 통하여 사용자에게 알린다.
			fe->ReportError();
		}
		catch (CArchiveException* ae) {
			// 예외가 발생하면 메세지박스를 통하여 사용자에게 알린다.
			ae->ReportError();
		}
		// CArchive 를 닫는다.
		ar.Close();
		// 파일을 닫는다.
		file.Close();
	
		for (POSITION pos = strlist.GetHeadPosition(); pos; )
		{
			str = strlist.GetNext(pos);
		}
	}
}


struct _trkey
{
	UINT	kind : 5;
	UINT	group : 3;
};


//-interface
void CTestDlgDlg::OnBnClickedBtnSerial3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//cl_test cl;

	char	key{};
	_trkey* trkey = (struct _trkey*)&key;
	//trkey->kind = 0x18;
	trkey->group =0x8;

	int ival = trkey->kind;
	ival = trkey->group;
	int isze = sizeof(char);
	char pchr = 62;


	int igubn = 1; 
	switch (igubn)
	{
		case 1:
		{
			CString str;
			if (igubn)
			{
				break;
			}

			CString str1;
		}
		break;
		case 2:
		{
			CString str;
		}
		default:
		{
			CString str;
		}
		break;
	}

	ival = sizeof(_treeID);

	CString str;
	int itree = 4;
	itree  = MAKE_TREEID(17, 9);

	int ikind = CAST_TREEID(itree)->kind; 
	int iskind = CAST_TREEID(itree)->skind;
}



//-한글
void CTestDlgDlg::OnBnClickedBtnHangul()
{
	CString stmp;
	stmp = "!김나박이";
	char ar = stmp.GetAt(0);
	
	if (ar == 88)
		TRACE("12313");
}


//-ocx load
void CTestDlgDlg::OnBnClickedBtnLoadocx()
{
	/*CString path;
*/

	char	buf[1024];
	GetWindowsDirectory(buf, sizeof(buf));
	CFileDialog	dlg(TRUE, "exe", NULL, OFN_OVERWRITEPROMPT,
		"모든파일(*.*)|*.*||", NULL);
	if (dlg.DoModal() != IDOK)	return;

	CString path;
	path = dlg.GetPathName();
	HINSTANCE hLib{};
	hLib = LoadLibrary(path);

	if (hLib < (HINSTANCE)HINSTANCE_ERROR)
		hLib = LoadLibraryEx(path, NULL, LOAD_LIBRARY_AS_DATAFILE);

	FARPROC	lpDllEntryPoint;
	(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, _T("DllRegisterServer"));

	if (lpDllEntryPoint == nullptr)
	{
		int iret = GetLastError();
		FreeLibrary(hLib);
		AfxMessageBox("nullptr dllregisterserver");
	}

	//GetDlgItem(IDC_PNAME)->SetWindowText(dlg.GetFileTitle());
	//GetDlgItem(IDC_FNAME)->SetWindowText(dlg.GetPathName());
}


BOOL CTestDlgDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_tooltip.RelayEvent(pMsg);

	if (pMsg->message == WM_LBUTTONUP)
	{
		CString str;
		// WM_LBUTTONUP 메시지 처리
		// 이 곳에 원하는 동작을 구현합니다.

		// 메시지를 무시하고 처리하지 않도록 return FALSE;를 사용할 수 있습니다.
		// return FALSE;

	}

	if (pMsg->message == WM_KEYDOWN)
	{
		m_slog.Format("[IMM] key down=[%c][%d]", (char)pMsg->wParam, pMsg->wParam);
		OutputDebugString(m_slog);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTestDlgDlg::SetToolTiplist()
{
	m_tooltip.Create(this);

	// 툴팁이 사용할 컨트롤의 ID가 IDC_STATIC 이라고 하면 아래와 같이 사용할
	// 내용을 명시해야 한다.
	m_tooltip.AddTool(GetDlgItem(IDC_BTN_SERIAL2), _T("★STATIC 툴팁★"));  //
	m_tooltip.AddTool(GetDlgItem(IDC_BRN_BIT), _T("16진수 비트 연산"));  //
	m_tooltip.AddTool(GetDlgItem(IDC_BRN_BIT2), _T("Round 계산(수수료에 이용됨)"));  
	m_tooltip.AddTool(GetDlgItem(IDC_BTN_PAIR), _T("pare 를 이용한 c++17 stl"));  

	// 툴팁에 사용할 내용이 너무 긴 경우에 한줄로 표시되면 사용자가 불편할 수 있다.
	// 이런 경우에는 툴팁의 폭을 제한하여 여러줄로 출력하게 만들면 된다.
	// 명시한 툴팁 내용의 폭이 300을 넘어서면 다음줄로 넘겨서 출력한다.
	m_tooltip.SetMaxTipWidth(300);

	// 툴팁에 별다른 설정을 하지 않으면 툴팁이 출력되었다가 일정시간이 지나면
	// 사라져 버린다. 보통 이 시간이 너무 짧아서 사용자가 불편함을 느끼는 경우가 있는데
	// 이러한 경우에는 아래의 코드처럼 시간을 늘려주면 된다.
	// 툴팁의 출력지속 시간을 20초로 늘린다. ( 20000 -> 20초 )
	m_tooltip.SetDelayTime(TTDT_AUTOPOP, 20000);

	// 동일한 클래스에서 추가적인 컨트롤에 툴팁을 사용하고 싶다면 툴팁 컨트롤을
	// 추가할 필요없이 AddTool 함수만 추가로 호출하면 된다.
	// 추가적으로 툴팁을 사용할 컨트롤의 ID가 IDC_BUTTON1 이라고 하면 아래와 같이
	// 툴팁의 내용을 명시하면 된다.
	
	
}


//-chart test
#include "CDlg_chart.h"
void CTestDlgDlg::OnBnClickedBtnChart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDlg_chart dlg;
	dlg.DoModal();
}

//-union rect
void CTestDlgDlg::OnBnClickedBtnRect()
{
	CRect rec1, rec2, rec3;
	rec1.SetRect(0, 0, 100, 100);
	rec2 = rec1;
	rec2.OffsetRect(50, 0);

	rec3 = rec1 & rec2;

	int const arTick[] = { 0, 14, 15, 16, 22, 23, 24, 27, 28, 29,    30, 31, 32, 33, 34, 41, 42, 44, 45, 46,      47, 48, 49, 301, 207, 251,  252,  253, 254, 255,    
	256, 326, 333, 334, 335, 336, 337, 338, 339, 340,     341, 342, 343, 344, 345, 346, 347, 348, 361, 501,   502, 503, 600 };


	int isize = sizeof(arTick) / sizeof(int);
}


void CTestDlgDlg::OnBnClickedBtnRegread()
{
	LONG lResult{};
	HKEY hKey{};
	DWORD keyValue[20];
	memset(keyValue, 0, sizeof(keyValue));
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(keyValue);

	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Edge\\IEToEdge"), 0, KEY_QUERY_VALUE, &hKey);
	if (lResult != ERROR_SUCCESS)
		return ;

	lResult = RegQueryValueEx(hKey, _T("SiteListUrlBucket"), 0, &dwType, (LPBYTE)keyValue, &dwSize);
	int ivalue = keyValue[0];

	CString stmp;
	stmp.Format("%02d", 0);
	
	CString date;
	CTime time;
	time = CTime::GetCurrentTime();
	date.Format("%02d%02d%02d",  time.GetHour(), time.GetMinute(), time.GetSecond());
}

void CTestDlgDlg::OnBnClickedBtnTime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTime time;
	CString date;
	time = CTime::GetCurrentTime();
	date.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	int iyear = time.GetYear();
	int imon = time.GetMonth();
	int iday = time.GetDay();
	int ihour = time.GetHour();
	int imin = time.GetMinute();

	COleDateTime oletime;
	oletime.SetDateTime(time.GetYear(), time.GetMonth(), time.GetDay(), 9, 0, 0);
	
	CTime calTime(oletime.GetYear(), oletime.GetMonth(), oletime.GetDay(), oletime.GetHour(), oletime.GetMinute(), oletime.GetSecond());
	date.Format("%04d%02d%02d%02d%02d", calTime.GetYear(), calTime.GetMonth(), calTime.GetDay(),
		calTime.GetHour(), calTime.GetMinute(), calTime.GetSecond());
	TRACE(date);

	CTimeSpan span(0, 0, 15, 0);

	CTime resultTime = calTime - span;
	date.Format("%04d%02d%02d%02d%02d", resultTime.GetYear(), resultTime.GetMonth(), resultTime.GetDay(), 
		resultTime.GetHour(), resultTime.GetMinute(), resultTime.GetSecond());

	CTime test = CalTimeBySpan(time.GetYear(), time.GetMonth(), time.GetDay(), 9, 0, 0, span, FALSE);

	date.Format("%04d%02d%02d%02d%02d", test.GetYear(), test.GetMonth(), test.GetDay(),
		test.GetHour(), test.GetMinute(), test.GetSecond());

	TRACE(date);
}

CTime CTestDlgDlg::CalTimeBySpan(int year, int month, int day, int hour, int minute, int secound, CTimeSpan spam, BOOL bPlus)
{
	COleDateTime oletime;
	oletime.SetDateTime(year, month, day, hour, minute, secound);

	CTime calTime(year, month, day, hour, minute, secound);
	//CTime resultTime = calTime - spam;
	CTime resultTime;
	if(bPlus)
		resultTime = calTime + spam;
	else
		resultTime = calTime - spam;

	return resultTime;
}

void CTestDlgDlg::OnBnClickedBtnIpc()
{
	CString strHwnd;
	((CWnd*)GetDlgItem(IDC_EDIT_DATA))->GetWindowText(strHwnd);

	//HWND hwnd = (HWND)_tcstol(strHwnd, nullptr, 16);
	OnBnClickedBtnFindps();
	if (m_hTarget != nullptr)
	{
	//	DWORD dwResult;
	//	COPYDATASTRUCT data;

	//	memset(&data, 0x00, sizeof(data));

	//	ST_COPY_DATA stdata{};
	//	stdata.iNum = PS_CREATE_WND;
	////	if(m_pWnd)
	////		stdata.hwnd = m_pWnd->m_hWnd; 
	//	memcpy(stdata.atcData, "test", 4);

	//	CRect rec;
	//	if (m_pWnd)
	//		m_pWnd->GetClientRect(rec);

	//	struct	_param param;
	//	param.rect = rec;
	//	stdata.pParam = param;
	//	stdata.pParam.rect = rec;

	//	data.dwData = 0;
	//	data.cbData = sizeof(stdata);
	//	data.lpData = &stdata;

	//	dwResult = (DWORD) ::SendMessage(m_hTarget, WM_COPYDATA, 0, (LPARAM)&data);
	}
}

void Cal(int min, int cHH, int cMM, int cSS)
{
	CString stmp;
	stmp.Format("\r\n !!!!!!! %d %d %d", cHH, cMM, cSS);
	OutputDebugString(stmp);

	int m_iShh = 9;
	int m_iSmm = 0;
	int	gap = (cHH - m_iShh) * 60 + (cMM - m_iSmm);

	if (gap % min == 0)
	{
		if (cSS == 0)
			gap = m_iSmm + gap;
		else
			gap = m_iSmm + gap + min;
	}
	else
	{
		int ival = m_iSmm + gap;
		ival = gap / min;
		ival = (gap / min) * min;
		gap = m_iSmm + (gap / min) * min + min;
	}

	int nHH, nMM, nSS;
	nHH = m_iShh + gap / 60;
	nMM = gap % 60;
	nSS = 0;

	stmp.Format("\r\n ------- %d %d %d", nHH, nMM, nSS);
	OutputDebugString(stmp);
}

void Cal2(int min, int cHH, int cMM, int cSS)  //min 봉(1~5) 
{
	CString stmp;
	stmp.Format("\r\n !!!!!!! %d %d %d", cHH, cMM, cSS);
	OutputDebugString(stmp);

	int m_iShh = 8;
	int m_iSmm = 45;
	int	gap = (cHH - m_iShh) * 60 + (cMM - m_iSmm);

	if (gap % min == 0)
	{
		if (cSS == 0)
			gap = m_iSmm + gap;
		else
			gap = m_iSmm + gap + min;
	}
	else
	{
		gap = m_iSmm + (gap / min) * min + min;
	}

	int nHH, nMM, nSS;
	nHH = m_iShh + gap / 60;
	nMM = gap % 60;
	nSS = 0;

	stmp.Format("\r\n ------- %d %d %d\r\n", nHH, nMM, nSS);
	OutputDebugString(stmp);
	string sss;
}


void test(unsigned char& nHH, unsigned char& nMM, unsigned char& nSS)
{
	int ihour = 8;
	int imin = 45;
	int isec = 0;
	CString str;
	nHH = ihour;
	nMM = imin;
	nSS = isec;
}
static constexpr UINT s_runAsyncWindowMessage = WM_APP;
void CTestDlgDlg::RunAsync(std::function<void()> callback)
{
	auto* task = new std::function<void()>(std::move(callback));
	PostMessage( s_runAsyncWindowMessage, reinterpret_cast<WPARAM>(task), 0);
}

void CTestDlgDlg::AsyncMessageBox(CString message, CString title)
{
	RunAsync([this, message = std::move(message), title = std::move(title)]
		{ MessageBox(message, title, MB_OK); });
}



//using namespace std;

int testfucn(char* pdata)
{
	CString str;
	str.Format("%s", pdata);
	return 0;
}

int mytest(int(*ptest)(char*), int ival)
{
	int val = ival;
	ptest("test");
	return 0;
}

void testfunc1(int ival) { 
	CString slog;
	slog.Format("\r\n ----111111111111111------ %d", ival);
	OutputDebugString(slog); }
void testfunc2(int ival) {
	CString slog;
	slog.Format("\r\n ----222222222222222------ %d", ival);
	OutputDebugString(slog); }
void testfunc3(int ival) { 
	CString slog;
	slog.Format("\r\n ----333333333333333------ %d", ival);
	OutputDebugString(slog); }

#define DF_RTSCODE "005930,001790"
#define	FA_SKIP		0x00000200	// internal field (No I/O)

#pragma warning(disable : 4996)

void RunAsync(std::function<void()> callback)
{
	auto* task = new std::function<void()>(callback);
	//PostMessage(WM_APP, reinterpret_cast<WPARAM>(task), 0);
}

void testAsync()
{
	OutputDebugString("\r\n testAsync");
}

auto getLimitedlength = [](const CString& sdata) {
	return sdata.GetLength() >= 10 ? sdata.Left(10) : sdata;
};

void CTestDlgDlg::OnBnClickedBtnTest()   //ontest
{
	char pval[100] = "123\n123"; // 읽기/쓰기 가능한 배열
	
	CString stm1p{};
	stm1p.Format("%c%s%c%d",  0x09, "1777", 0x7f, 2);

	strcat(pval, (LPSTR)(LPCTSTR)stm1p);





	CString strGubn, code;
	code = "1FPVB000";
	strGubn = code.Mid(1, 2);


		if (strGubn >= "B0" && strGubn <= "ZZ")	//2014.10.21 KSJ 주식선물 신규종목 연동
		{
			TRACE("\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\t\n");
		}



	return;


	m_slog.Format("%s", getLimitedlength("1234567890123"));

	RunAsync([this] { 
		testAsync(); 
		}
	);





	//if (1)
	//	return;

	CString fname;
	fname ="BLDINFO.INI";
	CFileFind finder;
	if (finder.FindFile(fname))
	{
		WritePrivateProfileString("Build", "BuildNumber", "1.4.1.71", fname);
		WritePrivateProfileString("Build", "State", "1", fname);
		WritePrivateProfileString("Build", "Version", "1.4.1.1", fname);
	}
	else
	{
		WritePrivateProfileString("Build", "BuildNumber", "1.7.0.83", fname);
		WritePrivateProfileString("Build", "State", "3", fname);
		WritePrivateProfileString("Build", "Version", "1.7.0.3", fname);
	}

	struct	_jinfo {
		char    codx[12];		// 종목코드
		char    hnam[50];		// 종목명
		char    jgub[1];		// 장구분 : '1':거래소 '2':코스닥, '3' :제3시장, '4':ETF, '6':ELW, '7':ELW-조기종료 
		char    size[12];		// 기업규모
		char    k2gb[20];		// 소구분 전기,전자
		char    usun[10];		// 보통주/우선주
		char    ksmm[2];		// 결산월
		char    amga[20];		// 액면가 8->20 단위포함
		char    jbkm[20];		// 자본금(단위:억) 12->20 단위포함
		char    siga[12];		// 전일시가총액(단위:억)
		char    sjjs[12];		// 상장주식수(단위:천주)
		char	jrat[3];		// 증거금율
		char	nrat[3];		// 신용증거금율 20070718 
		char    krgb[12];		// "거래정지", "거래중단", "관리종목", "이상급등", "투자유의", "정상"
		char    jggb[12];		// 장운영 구분
		char    lock[12];		// 락구분
		char    dyga[8];		// 대용가
		char    ujgb[12];		// 업종구분
		char    ujcd[6];		// RTS 업종코드
		char    jisu[8];		// 업종지수
		char    diff[8];		// 업종전일대비
		char    rate[8];		// 업종등략율
		char    gvol[12];		// 업종거래량(단위:천주)
		char    unio[6];		// KRX 100 (0:일반 1:통합지수)
		char	rned[2];		// 임의종료
		char	dist[2];		// 배분정보
		char	frgb[8];		// 외국인 보유비중
		char	rlnk[1];		// 대출등급('A'~'E', 'X', ' ')
		char    vist[1];		// 변동성 완화 장치
		char    vitime[6];		// 변동성 완화 장치 발동 시각
		char	vitype[1];		// 변동성 완화 장치 타입(정적,동적,정/동적)
		char	sijang[40];		// 시장조치
	};

	struct  _jinfo3 {			// 신 HTS 종목정보 (사이즈 줄인것)
		char    codx[12];		// 종목코드
		char    hnam[50];		// 종목명
		char    jgub[1];		// 장구분 : '1':거래소 '2':코스닥, '3' :제3시장, '4':ETF, '6':ELW, '7':ELW-조기종료
		char	jrat[3];		// 증거금율
		char	nrat[3];		// 신용증거금율 20070718 
		char    krgb[12];		// 관리구분
		char	rned[2];		// 임의종료
		char	dist[2];		// 배분정보
		char    vist[1];		// 변동성 완화 장치
		char    vitime[6];		// 변동성 완화 장치 발동 시각
		char	vitype[1];		// 변동성 완화 장치 타입(정적,동적,정/동적)
		char	fill[12];
	};

	char buff[100] = {0, };
	sprintf(buff, "%d", fabs(0.0));


	int isize = sizeof(struct _jinfo3);

	CString stmp;

	m_slog.Format("\r\n[memo]-[dll_oubx]-[%-35s] --> key=[%d] ret=[%c]  msg=[%.64s] ",
		"TRKEY_MEMO_SAVE||TRKEY_MEMO_UPDATE", 1, '2', "Test");
	m_slog.Trim();
	OutputDebugString(m_slog);

	m_slog.Format("\r\n[memo]-[dll_oubx]-[%-35s] --> key=[%d] ret=[%c]  msg=[%.64s] ",
		"TRKEY_MEMO", 1, '2', "Test");
	m_slog.Trim();
	OutputDebugString(m_slog);


	CString tttest = "123\t456";
	int pfunc = (int)testfucn;
	((int(*)(char*))pfunc)("Set");

	int( * ptest)(char*) = testfucn;
	ptest("test");

	mytest(testfucn, 10);

	void(*pflist[3])(int) = { testfunc1, testfunc2 , testfunc3 };

	pflist[0](1);
	pflist[1](2);
	pflist[2](3);

	CString strstr;
	strstr = " +0";

	strstr = "9400";
	double dval = fabs(atof(strstr));

	CString sKey = "한글9";
	int iilen = sKey.GetLength();
	CString dat = "한글도 하고 \
		english also \
		unique letter!@#$ % ^";
	int lSize = dat.GetLength();
	stmp.Format("%-12s%-4d%s", sKey, lSize, dat);
	

	int idx = strstr.FindOneOf("+-0");
	if ((idx = strstr.FindOneOf("+-0")) == 1)
	{
		strstr.TrimRight();
	}
	else
	{
		strstr.TrimRight();
	}

	DWORD dtest = 8768;

	bool dret = dtest & FA_SKIP;

	dtest = 8256;
	dret = dtest & FA_SKIP;

	CString ssdata;
	ssdata = "1710";

	if (ssdata.GetAt(0) == '1' || (ssdata.GetAt(1) == '7') && (ssdata.GetAt(2) == '0'))
	{
		TRACE("test");
	}

	int& c = (int&)*ssdata.GetBuffer(0);
	int inum = 888;
	c = (int&)inum;
	//int& d = 1;

	int iflag = 2;
	bool bret = iflag & 0x02;
	
	int ibit = 0x83;
	ibit = ibit >> 2;
	int ival = 56;
	CString str;
	str = "m00004";
	str = str.Mid(1, str.GetLength() - 1);
	str.Format("m%05d", 6);



	char chtest[100] = "벤";
	int x = chtest[0];
	int y = chtest[1];
	CString xnum;
	xnum.Format("%x %d", x, y);
	char chtmp = 'a';
	//AsyncMessageBox("test message", "title ");
	//int aa = 9;
	//int a = 1;
	//int* b = (int*)1; 
	//int& c = a;
	//int& d = *&a;
	//d = aa;

	int aa = 9;
	int bb = 8;
	int* a = &aa;  //a라는 포인터형 변수에 주소지정연산자&를 이용해서 aa의 주소를 넣었다.
	//그래서 a의 실제 주소에는 aa의 주소가 들어있다. 
	a = &bb;  //그런데 a에 bb의 주소를 넣어주었다. 
	//그러면 a가 가지고  있는 주소는 bb의 주소고
	aa = *a;  //그값을 aa에 넣은것이다. 
	//그렇다면 aa 와 bb 의 주소는 같을까?  다르다!! *a는 주소가 아니라 값을 넣어준것이다. aa 라는 변수는 bb 와 다른 주소를 가지고 있는 변수에
	//값만(주소가 아닌!!! 주소는 값이 아닌 L_value  이다. 다른 주소가 같은 값을 가지고 있는것!!) 넣어준것이다.


	int arra[3] = { 1,2,3 };
	int value_a = arra[0];
	int* pa = &arra[0];


	CRect rec;
	GetClientRect(rec);
	rec.DeflateRect(1, 2, 10, 20);
	rec.InflateRect(1, 2, 10, 20);
	rec.OffsetRect(CPoint(0, 100));
	rec.OffsetRect(CPoint(100, 0));




	int m_wDisableBar = SB_VERT;
	SCROLLINFO info;
	info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS;
	GetScrollInfo(SB_VERT, &info);

	int iret = SB_VERT ? SIF_DISABLENOSCROLL : 0;
	int ires = m_wDisableBar & iret;
	ires = SIF_RANGE | SIF_PAGE;
	ires = ires | SIF_TRACKPOS;
	info.fMask |= (m_wDisableBar & SB_VERT ? SIF_DISABLENOSCROLL : 0);

	//std::array<int, 3> arr;
	//arr.at(0) = 1;
	//arr.back() = 2;
	//
	//Cal(10, 13, 5, 0);
	Cal2(1, 8, 45, 0);

	//Cal(10, 13, 6, 0);
	//Cal2(10, 13, 6, 0);

	//Cal(10, 13, 7, 0);
	//Cal2(10, 13, 7, 0);

	//Cal(10, 13, 8, 0);
	//Cal2(10, 13, 8, 0);

	//Cal(10, 13, 9, 0);
	//Cal2(10, 13, 9, 0);

	//Cal(10, 13, 10, 0);
	//Cal2(10, 13, 10, 0);

	//Cal(10, 13, 11, 0);
	//Cal2(10, 13, 11, 0);
	
	/*char buff[128];
	CString iniConf = "F:\\util\\HTS\\IBK투자증권 HTS\\tab\\axis.ini";
	GetPrivateProfileString("move", "file", "", buff, sizeof(buff) - 1, iniConf);

	CString strpath, strfile;
	strpath.Format("%s", buff);
	strpath.TrimRight();

	strfile = parser(strpath, ";");

	iniConf = "F:\\util\\HTS\\IBK투자증권 HTS\\tab\\";
	iniConf += strfile;
	CString strOripath;
	strOripath = iniConf;

	CFileFind cfFind;
	BOOL bFind = cfFind.FindFile(iniConf);

	if (bFind)
	{
		int ifind = iniConf.ReverseFind('\\');
		iniConf = iniConf.Left(ifind);
		iniConf += "\\";
		iniConf += strfile;
		iniConf.Replace("tab", strpath);
		CopyFile(strOripath, iniConf, FALSE);
		DeleteFile(strOripath);
	}*/

	CString stest;
	stest = "12034";
	 idx = stest.FindOneOf("+-0");
	stest = "12035";
	idx = stest.FindOneOf("+-");


	int index = m_cbTest.AddString(stest);
	m_cbTest.SetItemData(index, (LPARAM)(LPSTR)(LPCTSTR)stest);

	stest = "testsetsetst";
	 index = m_cbTest.AddString(stest);
	m_cbTest.SetItemData(index, (LPARAM)(LPSTR)(LPCTSTR)stest);

	m_slog.Format("%s", m_cbTest.GetItemData(index));
	//index = m_cbTest.GetCount();

	m_cbTest.GetLBText(0, stest);
	index = m_cbTest.FindStringExact(-1, stest);

	m_cbTest.GetLBText(1, stest);
	index = m_cbTest.FindStringExact(-1, stest);

	stest.Empty();
	stest.Format("%s", m_cbTest.GetItemData(0));

	CString strTime;
	strTime = "8시 30분 ~ 8시 40분";
	const char ch = 0x7e;
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);


	strTime = "8시 40분 ~ 9시 0분";
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);

	

	strTime = "9시 00분 ~ 15시 20분";
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);


	strTime = "15시 20분 ~ 15시 30분";
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);


	strTime = "15시 30분 ~ 16시 00분";
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);


	strTime = "16시 00분 ~ 18시 00분";
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);






	strTime = "11시 0분 ~ 11시 10분";
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);


	strTime = "11시 10분 ~ 11시 30분";
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);


	strTime = "11시 30분 ~ 15시 50분";
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);


	strTime = "15시 50분 ~ 16시 00분";
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);


	strTime = "16시 0분 ~ 16시 30분";
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);


	strTime = "16시 30분 ~ 17시 0분";
	strTime.Trim();
	strTime.Replace("+", "");
	strTime.Replace("-", "");
	strTime.Replace(" ", "");
	stest = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
	stest = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);

	 CString(DF_RTSCODE).Trim();
	if (CString(DF_RTSCODE).Find("005930") >= 0)
	{
		TRACE("Test");
	}
}

CString CTestDlgDlg::CalMaketTime(CString strTime, bool bEnd)
{
	CString strHour, strMinute, strData;

	int startIndex = strTime.Find(_T("시"));
	strHour = strTime.Left(startIndex);

	strMinute = strTime.Mid(startIndex);
	strMinute.Replace("시", "");
	strMinute.Replace("분", "");

	if (bEnd)  //마감 1초 빼줘야 한다.
	{
		if (atoi(strMinute) == 0){
			strData.Format("%02d%02d59", atoi(strHour) - 1, 59);
		}
		else {
			strData.Format("%02d%02d59", atoi(strHour), atoi(strMinute) - 1);
		}
	}
	else  //시작시간
	{
		strData.Format("%02d%02d00", atoi(strHour), atoi(strMinute));
	}

	return strData;
}

CString CTestDlgDlg::parser(CString& srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}



bool CTestDlgDlg::GetProcessModule(DWORD dwPID, char* sProcessName)
{
	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32 = { 0 };
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	if (hModuleSnap == (HANDLE)-1)
		return (FALSE);

	me32.dwSize = sizeof(MODULEENTRY32);
	CString slog;
	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면
	if (Module32First(hModuleSnap, &me32))
	{
		do
		{
			printf("process name : %s\n", me32.szModule);
			slog.Format("[mac] GetProcessModule =[%s] \r\n", me32.szModule);
			//	OutputDebugString(slog);
			if (strcmp(me32.szModule, sProcessName) == 0)
			{
				CloseHandle(hModuleSnap);
				return true;
			}
		} while (Module32Next(hModuleSnap, &me32));
	}

	CloseHandle(hModuleSnap);
	return false;
}

void CTestDlgDlg::OnBnClickedBtnFindps()
{
	CString strHwnd;
	((CWnd*)GetDlgItem(IDC_EDIT_DATA))->GetWindowText(strHwnd);
	m_Targethandle = ProcessFind((char*)strHwnd.GetBuffer(0));
	m_hTarget = GetHwndFromProcessHandle(m_Targethandle);
}

void CTestDlgDlg::OnBnClickedBtnShared()
{
	DWORD processID = GetCurrentProcessId();
	m_strSharedMName.Format("%s%d", "_SM_TEST", processID);
	m_hKeyFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, m_strSharedMName);

	if (m_hKeyFile == nullptr)
	{
		m_hKeyFile = CreateFileMapping((HANDLE)0xffffffff,
			nullptr,
			PAGE_READWRITE,
			0,
			1024 * 50,
			m_strSharedMName);
	}

	m_slog.Format("%d", m_hKeyFile);
	((CWnd*)GetDlgItem(IDC_EDIT_SHARED))->SetWindowText(m_slog);

}
#define SIZE_DATA_DEFINE			4
#define SIZE_DATA_START			4
#define SIZE_DATA_LEN				4
void CTestDlgDlg::OnBnClickedBtnSharedWrite()  //write
{
	CString sdata;
	((CWnd*)GetDlgItem(IDC_EDIT_SHARED))->GetWindowText(sdata);

	HANDLE hSharedMemory{};
	hSharedMemory =(HANDLE) atoi(sdata);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LPVOID pSharedMemory = MapViewOfFile(
		hSharedMemory,  // 공유 메모리 핸들
		FILE_MAP_WRITE, // 공유 메모리 영역에 대한 액세스 제어
		0,              // 공유 메모리 영역 오프셋 상위 32비트
		0,              // 공유 메모리 영역 오프셋 하위 32비트
		0               // 공유 메모리 영역 크기 (0이면 전체 영역)
	);

	InitializeCriticalSection(&csMapHandle);
	EnterCriticalSection(&csMapHandle);

	char* pdata = "Test_test";                  
	if (pSharedMemory != NULL) {
		// 공유 메모리에 데이터를 씁니다.
		//BYTE pdata = m_pWnd;
		//memcpy(pSharedMemory, (int)strlen(pdata), 4);
		//pSharedMemory = (int)strlen(pdata);
		int idata = 12;
		int ilen = sizeof(m_pWnd);
		BYTE* pbyte = (BYTE*)pSharedMemory;
		memcpy(pbyte + SIZE_DATA_DEFINE, (int*)&idata, SIZE_DATA_START);								//Data 시작 index
		memcpy(pbyte + SIZE_DATA_DEFINE + SIZE_DATA_START, (int*)&ilen, SIZE_DATA_LEN);     //Data 길이
		memcpy(pbyte + SIZE_DATA_DEFINE + SIZE_DATA_START + SIZE_DATA_LEN, m_pWnd, ilen);  //Data 실제
		//int ibyte = (int)pbyte + SIZE_DATA_DEFINE + SIZE_DATA_START + SIZE_DATA_LEN;
	
		//(int)(pbyte + SIZE_DATA_DEFINE + SIZE_DATA_START + SIZE_DATA_LEN) = (int)m_pWnd;

		int ival = *(int*)pbyte;
		int  istart = *(int*)(pbyte + 4);    //위치
		int ilenval = *(int*)(pbyte + 8);    //길이

		char* pval = new char[ilenval + 1];
		memset(pval, 0x00, ilenval + 1);
		memcpy(pval, pbyte + 12, ilenval);

		LeaveCriticalSection(&csMapHandle);
		DeleteCriticalSection(&csMapHandle);
		// 공유 메모리 매핑 해제
		UnmapViewOfFile(pSharedMemory);
	}
}

void CTestDlgDlg::OnBnClickedBtnSharedRead()
{
	HANDLE hSharedMemory = OpenFileMapping(
		FILE_MAP_READ,      // 공유 메모리 영역에 대한 액세스 제어
		FALSE,              // 현재 프로세스의 보안 컨텍스트 사용 여부
		m_strSharedMName// 공유 메모리 이름
	);

	if (hSharedMemory != NULL) {
		// 공유 메모리 영역을 프로세스의 메모리에 매핑
		LPVOID pSharedMemory = MapViewOfFile(
			hSharedMemory,  // 공유 메모리 핸들
			FILE_MAP_READ,  // 공유 메모리 영역에 대한 액세스 제어
			0,              // 공유 메모리 영역 오프셋 상위 32비트
			0,              // 공유 메모리 영역 오프셋 하위 32비트
			0               // 공유 메모리 영역 크기 (0이면 전체 영역)
		);

		if (pSharedMemory != NULL) {
			// 공유 메모리에서 데이터를 읽어옵니다.
			// 읽은 데이터를 사용합니다.
			
			// 읽은 데이터를 사용합니다.
			CString strdata;
			char* pShare = (char*)pSharedMemory;
			int iStart{}, iLen{};
			// 공유메모리 
			int istart = *(int*)(pShare + SIZE_DATA_DEFINE);
			int ilen = *(int*)(pShare + SIZE_DATA_DEFINE + SIZE_DATA_START);

			char* pResult = new char[ilen + 1];
			memset(pResult, 0x00, ilen + 1);
			memcpy(pResult, pShare + 12, ilen);

			CControlWnd* pwnd = (CControlWnd*)pResult;
			pwnd->CheckWnd("test");

			// 공유 메모리 매핑 해제
			UnmapViewOfFile(pSharedMemory);
		}

		// 공유 메모리 핸들 닫기
		CloseHandle(hSharedMemory);
	}

}

void CTestDlgDlg::OnBnClickedBtnPlaysound()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString filePath;
	filePath.Format("%s", "C:\\PhonePad\\v00018.snd");
	

	//// SND 파일 재생
	//BOOL bret = sndPlaySound(filePath, SND_FILENAME | SND_ASYNC);
	bool bret = sndPlaySound(filePath, SND_FILENAME | SND_ASYNC);
}


void CTestDlgDlg::OnBnClickedBtnKeymake()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int ii = 0x20; ii < 0xff; ii++)
		m_major.Add(ii);

	for (int ii = 0x20; ii < 0xff; ii++)
		_vMajor.push_back(ii);
}


void CTestDlgDlg::OnBnClickedBtnFilecheck()
{
	CString filePath;
	int maxSizeInKB = 120;
	filePath = "C:\\IBKS\\IBK투자증권 HTS\\exe\\xc_3 - 복사본.log";
	CFileStatus fileStatus;
	if (CFile::GetStatus(filePath, fileStatus))
	{
		ULONGLONG fileSize = fileStatus.m_size;

		if (fileSize >= (ULONGLONG)maxSizeInKB * 1024)
		{
			// 파일 크기가 maxSizeInKB 이상인 경우 파일을 삭제합니다.
			DeleteFile(filePath);
		}
		else
		{
			TRACE(_T("File size is smaller than %d KB: %s\n"), maxSizeInKB, (LPCTSTR)filePath);
		}
	}
	else
	{
		TRACE(_T("Failed to get file status: %s\n"), (LPCTSTR)filePath);
	}
}


void CTestDlgDlg::OnBnClickedBtnCalcwindowrect()
{
	CRect rectClient;
	GetWindowRect(rectClient);
	CRect rect = rectClient;
	CalcWindowRect(rect);
	rectClient.left += rectClient.left - rect.left;
	rectClient.top += rectClient.top - rect.top;
	rectClient.right -= rect.right - rectClient.right;
	rectClient.bottom -= rect.bottom - rectClient.bottom;
	rectClient.OffsetRect(-rectClient.left, -rectClient.top);
}


void CTestDlgDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rect;

	GetClientRect(rect);

	if(m_pWnd)
		m_pWnd->MoveWindow(rect);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
                                                                                   
#include "CScrollDlg.h"
void CTestDlgDlg::OnBnClickedBtnScrolldlg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CScrollDlg dlg;
	dlg.DoModal();
}

/*
struct	_Ralert {
	CString code;
	int	stat{};				// DLL_ALERT.stat
	int	size{};				// data record count
	std::unique_ptr<char[]> ptr[999]{};	// data record array
};
*/
void CTestDlgDlg::OnBnClickedUnorderedmap()
{
	std::unordered_map<int, int> myMap;
	myMap[3] = 10;
	int ival{}, ival2{};
	int arr[] = { 1,2,3,4,5,6,7,8,9,10 };

	for (const auto symbol : arr)
	{
		auto  at = myMap.find(symbol);
		if (at != myMap.end())
		{
			ival = at->first;
			ival2 = at->second;
		}
	}

	const auto mt = _m_mapRealData.emplace("testcode", std::make_unique<struct _Ralert>());
	CString strdata;
	mt.first->second->ptr[0].reset();
	strdata.Format("%s", mt.first->second->ptr[0].get());

	std::map<CString, int>    _mRealtime;

	const auto mRt = _mRealtime.emplace(std::make_pair("test", 99));
	//	CString stmp = mRt.first;; //= "test";
	ival = mRt.first->second;
	//	mRt.second = 2;
	
};

#include "CDlg_player.h"
void CTestDlgDlg::OnBnClickedBtnPlyer()
{
	CDlg_player dlg;
	dlg.DoModal();
}

#include "CDLG_SLIDER.h"
void CTestDlgDlg::OnBnClickedBtnSlider()
{
	CDLG_SLIDER dlg;
	dlg.DoModal();
}


void CTestDlgDlg::OnBnClickedBtnIntfloat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CArray <double, double> _arr;
	_arr.Add(1.1);
	_arr.Add(2.1);
	_arr.Add(3.11);
	_arr.Add(4.12);

	double fdata;
	for (int ii = 0; ii < 4; ii++)
	{
		fdata =round( _arr.GetAt(ii) * 100) / 100 ;
		fdata = floor(_arr.GetAt(ii) + 0.05);
	}

	double ddata = 1.01;
	double ddata2 = 1.09;
	long idata = 20;
	idata = idata * ddata;
	idata = idata * ddata2;
}


void CTestDlgDlg::OnCbnSelchangeCbTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;

	int index = m_cbTest.GetCurSel();
}

#include "CDlg_Thread.h"
void CTestDlgDlg::OnBnClickedBtnIntfloat2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDlg_Thread dlg;
	int ivla = dlg.DoModal();
}


void CTestDlgDlg::OnBnClickedBtnTcp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString serverips;
	UINT	port = 15201;
	CSocket	sock;
	serverips = "211.255.204.134";
	sock.Create();
	if ( sock.Connect(serverips, port))
	{
		AfxMessageBox("success");
	}
	else
	{
		AfxMessageBox("fail");
	}

}

void CTestDlgDlg::testref(BOOL& ref)
{
	CString str;
	ref = !ref;
}

void CTestDlgDlg::OnBnClickedBtnRef()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bref = false;
	testref(bref);
	CString str;
}


void CTestDlgDlg::OnBnClickedBtnAfxex()
{
	int ii = 0;
	CString strData;
	strData = "1\t2\t3\t4\t5\t6\t7\t8\t";

	CString entry;
	entry = "Test";
	CString slog;
//	ii = 13;
	//while (AfxExtractSubString(entry, strData, ii, '\t'))
	//{
	//	if (ii >= 20)
	//		break;
	//	slog.Format("\r\n-- %s  [%d] --", entry, ii);
	//	ii++;
	//	OutputDebugString(slog);
	//}

	while (!strData.IsEmpty())
	{
		entry = parser(strData, "\t");
		slog.Format("\r\n-- %s  [%d] --", entry, ii);
		ii++;
		OutputDebugString(slog);
	}
}


void CTestDlgDlg::OnBnClickedBtnVector()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString stmp;
	/*for (int ii = 0; ii < 100; ii++)
	{
		std::unique_ptr<struct _Ralert> pdata = std::make_unique<_Ralert>();
		stmp.Format("%d", ii);
		pdata->code = stmp;
		for (int jj = 0; jj < 999; jj++)
		{
			pdata->ptr[jj].reset(std::make_unique<char[]>(10).release());
			ZeroMemory(pdata->ptr[jj].get(), 10);
			pdata->ptr[jj].get()[0] = '1';
		}
		_vETNx.push_back(std::move(pdata));
	}*/

	//CArray < std::unique_ptr<struct _Ralert>> _arr;
	for (int ii = 0; ii < 100; ii++)
	{
		struct _ralert* palert = new struct _ralert;
		stmp.Format("%d", ii);
		palert->code = "1";
		//memcpy(palert->code, "1", 1);
		for (int jj = 0; jj < 999; jj++)
		{
			palert->ptr[jj] = new char[10];
			ZeroMemory(palert->ptr[jj], 10);
			memcpy(palert->ptr[jj], "1", 1);
		}
		sarr.Add(palert);
	}
}

/*
struct	_Ralert {
	CString code;
	int	stat{};				// DLL_ALERT.stat
	int	size{};				// data record count
	std::unique_ptr<char[]> ptr[999]{};	// data record array
};
*/
void CTestDlgDlg::OnBnClickedBtnVector2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//_Ralert* pdata = _vETNx[1].get();
	//int ii = 0;
	//int isize = _vETNx.size();
	//for (ii = 0; ii < isize; ii++)
	//{
	//	_Ralert* pdata = _vETNx[ii].get();
	//	pdata->code.ReleaseBuffer(0);
	//	/*for (int jj = 0; jj < 999; jj++)
	//		pdata->ptr[jj].reset();*/
	//	pdata = nullptr; 
	//	
	//}

	//CString str;

	while (1)
	{
		if (sarr.GetSize() <= 0)
			break;

		_ralert* palert = sarr.GetAt(0);
		sarr.RemoveAt(0);
		for (int jj = 0; jj < 999; jj++)
		{
			delete[] palert->ptr[jj];
			palert->ptr[jj] = nullptr;
		}

		palert->code.ReleaseBuffer(0);
		delete palert;
		palert = nullptr;
		
	}

	CString str;

}

#include "CDlg_Thread.h"
void CTestDlgDlg::OnBnClickedBtnThread()
{
	CDlg_Thread dlg;
	int ivla = dlg.DoModal();

}

HANDLE hMutex1;
HANDLE hMutex2;

DWORD WINAPI ThreadFunc1(LPVOID lpParam)
{
	HANDLE hMutex1 = static_cast<HANDLE>(lpParam);

	// 먼저 Mutex 1을 얻음
	WaitForSingleObject(hMutex1, INFINITE);

	std::cout << "Thread 1: Got Mutex 1" << std::endl;
	int icnt = 0;
	CString slog;
	while (1)
	{
		Sleep(500);
		slog.Format("\r\n -- ThreadFunc1 --[%d]", icnt);
		OutputDebugString(slog);
		icnt++;
	}

	// Mutex 1 릴리스
	ReleaseMutex(hMutex1);

	return 0;
}

DWORD WINAPI ThreadFunc2(LPVOID lpParam)
{
	HANDLE hMutex1 = static_cast<HANDLE>(lpParam);

	// 먼저 Mutex 2를 얻음
	WaitForSingleObject(hMutex2, INFINITE);

	std::cout << "Thread 2: Got Mutex 2" << std::endl;

	// 작업 수행...
	int icnt = 0;
	CString slog;
	while (1)
	{
		Sleep(500);
		slog.Format("\r\n -- ThreadFunc2 --[%d]", icnt);
		OutputDebugString(slog);
		icnt++;
		if (icnt == 10)
			break;
	}

	// Mutex 2 릴리스
	ReleaseMutex(hMutex2);

	// Mutex 1 얻으려고 시도
	WaitForSingleObject(hMutex1, INFINITE); // 여기서 데드락 발생

	AfxMessageBox("hMutex1 를 얻었다고?");

	// 작업 수행...

	// Mutex 1 릴리스
	ReleaseMutex(hMutex1);

	return 0;
}


void CTestDlgDlg::OnBnClickedBtnThread2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	 hMutex1 = CreateMutex(NULL, FALSE, NULL);
	 hMutex2 = CreateMutex(NULL, FALSE, NULL);

	HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunc1, hMutex1, 0, NULL);
	HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunc2, hMutex1, 0, NULL);

	//WaitForSingleObject(hThread1, INFINITE);
	//WaitForSingleObject(hThread2, INFINITE);

	//CloseHandle(hMutex1);
	//CloseHandle(hMutex2);
	//CloseHandle(hThread1);
	//CloseHandle(hThread2);

}


void CTestDlgDlg::OnBnClickedBtnThread3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	SetTimer(9999, 1000, nullptr);
}



__declspec (thread) CStringArray tls_arr;
static DWORD tlsIndex = TlsAlloc();
void CTestDlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TID_CHANGEHANGLE:
		{
			KillTimer(TID_CHANGEHANGLE);
			OnBnClickedBtnMonitor3();
		}
		break;
		case 9999:
		{
			if (0)
			{
				ThreadData* threadData = new ThreadData();
				threadData->threadID = 98;
				threadData->strdata = m_slog;
				TlsSetValue(tlsIndex, threadData);
			}
			else
			{
				m_slog.Format("[@] %d", GetTickCount());
				m_arr.Add(m_slog);
			}
			
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}



CCriticalSection g_cs;
CCriticalSection g_2cs;
int g_data;
UINT td_test1(void* pdata)
{
	CString m_slog;
//	m_slog.Format("\r\n[@] 111_111 g_data =[%d]  pdata=[%s]", g_data, pdata);
//	OutputDebugString(m_slog);
	CTestDlgDlg* pdlg;
	pdlg = (CTestDlgDlg*)pdata;
	m_slog.Format("\r\n[@] 111_111 pdata=[%x]",  pdata);
	OutputDebugString(m_slog);
	while (1)
	{
		if (pdlg->m_bTdStop)
			break;
		g_cs.Lock();
		g_data = 100;
	
		if (1)
		{
			if(pdlg->m_arr.GetSize() > 0)
			{
				ThreadData* threadData = new ThreadData();
				threadData->threadID = 99;
				threadData->strdata = pdlg->m_arr.GetAt(0);
				TlsSetValue(tlsIndex, threadData);
				ThreadData* pdata = reinterpret_cast<ThreadData*>(TlsGetValue(tlsIndex));
				m_slog.Format("\r\n[@] 111_222    g_data=[%d] cnt=[%d] tls=[%s]", g_data, pdlg->m_arr.GetSize(), pdata->strdata);
				OutputDebugString(m_slog);
				pdlg->m_arr.RemoveAt(0);
			}
		}
		else
		{
			if (tls_arr.GetSize() > 0)
			{
				m_slog = tls_arr.GetAt(0);
				OutputDebugString(m_slog);
				tls_arr.RemoveAt(0);
			}
		}

		Sleep(2000);
		g_cs.Unlock();
	}
	m_slog.Format("\r\n[@] 111 thread return  g_data=[%d]", g_data);
	TlsFree(tlsIndex);
	OutputDebugString(m_slog);
	return 0;
}

UINT td_test2(void* pdata)
{
	CTestDlgDlg* pdlg;
	pdlg = (CTestDlgDlg*)pdata;

	CString m_slog;
	m_slog.Format("\r\n[@] 222_111 g_data =[%d] pdata=[%x]", g_data, pdata);
	OutputDebugString(m_slog);
	
	while (1)
	{
		if (pdlg->m_bTdStop)
			break;

	//	g_2cs.Lock();
		g_data = 200;
		Sleep(1000);
		m_slog.Format("\r\n[@] 222_222 g_data =[%d] ", g_data);
		OutputDebugString(m_slog);
	//	g_2cs.Unlock();
	}

	m_slog.Format("\r\n[@] 222 thread return  g_data=[%d]", g_data);
	OutputDebugString(m_slog);
	return 0;
}

void CTestDlgDlg::OnBnClickedBtnCritical()
{//criticalsection
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OutputDebugString("\r\n[@]");
	OutputDebugString("\r\n[@] -------------------------------------------------------\r\n");

	m_bTdStop = FALSE;
	int itick = GetTickCount();
	m_slog.Format("%d", itick);
	char* pdata = new char[9];
	ZeroMemory(pdata, 9);
	memcpy(pdata, m_slog.GetString(), 8);
	HANDLE h1 = AfxBeginThread(td_test1, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	HANDLE h2 = AfxBeginThread(td_test2, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	/*m_csTls = new cs_TLS();

	ThreadData* threadData = new ThreadData();
	threadData->threadID = m_csTls->tlsIndex;
	threadData->strdata = m_slog;
	m_csTls->SetThreadLocalData(threadData);

	ThreadData* ppdata = reinterpret_cast<ThreadData*>(m_csTls->GetThreadLocalData());*/

	ThreadData* threadData = new ThreadData();
	threadData->threadID = 99;
	threadData->strdata = m_slog;
//	m_csTls->SetThreadLocalData(threadData);
	TlsSetValue(tlsIndex, threadData);


	SetTimer(9999, 2000, nullptr);


//	threadData->threadID = GetCurrentThreadId();
//	delete m_csTls;
	Sleep(0);
//	m_slog.Format("\r\n[@] 333 g_data =[%d] pdata=[%s] [%x][%x]", g_data, pdata, h1, h2);
//	OutputDebugString(m_slog);


	OutputDebugString("\r\n[@] -------------------------------------------------------\r\n\r\n");
}

void CTestDlgDlg::OnBnClickedBtnThread4()
{//thread  끝내기
	m_bTdStop = TRUE;
}

void CTestDlgDlg::OnBnClickedBtnCritical2()
{//res criticalsection
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OutputDebugString("\r\n[@]");
	OutputDebugString(" \r\n[@] ===========================\r\n");

	m_slog.Format("\r\n[@]  333_111 g_data =[%d]", g_data);
	OutputDebugString(m_slog);
	char* pdata = new char[5];
	memcpy(pdata, "12345", 5);

	
	//Sleep(0);
	HANDLE h1{}, h2{};
	if (GetTickCount() % 2 == 0)
	{
		OutputDebugString(" \r\n[@] 1 THREAD\r\n");
		g_cs.Unlock();
		h1 = AfxBeginThread(td_test1, pdata, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	else
	{
		OutputDebugString(" \r\n[@] 2 THREAD\r\n");
		g_2cs.Unlock();
		h2 = AfxBeginThread(td_test2, pdata, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	
	m_slog.Format("\r\n[@]  333_222 g_data =[%d]  [%x][%x]", g_data, h1, h2);
	OutputDebugString(m_slog);
	OutputDebugString("\r\n[@]  ===========================-\r\n\r\n");
}


void CTestDlgDlg::OnBnClickedBtnCri1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_cs.Unlock();
}


void CTestDlgDlg::OnBnClickedBtnCri2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_2cs.Unlock();
}

#include "CDlg_SSib.h"
void CTestDlgDlg::OnBnClickedBtnThread5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDlg_SSib dlgs;
	dlgs.DoModal();
}


//#include "CDlg_Edge.h"
void CTestDlgDlg::OnBnClickedBtnEdgedlg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDlg_Edge dlg;
	//dlg.DoModal();
}

#include "CDlg_load.h"
void CTestDlgDlg::OnBnClickedBtnLoadlib()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDlg_load dlg;
	dlg.DoModal();
	
}


void CTestDlgDlg::OnBnClickedBtnWebview2()
{
	HKEY hKey;
	LSTATUS result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\EdgeUpdate\\ClientState\\{56EB18F8-B008-4CBD-B6D2-8C97FE7E9062}", 0, KEY_READ, &hKey);
	//LSTATUS result = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\EdgeUpdate\\ClientState\\{56EB18F8-B008-4CBD-B6D2-8C97FE7E9062}", 0, KEY_READ, &hKey);

	if (result == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return ;
	}

	CString str;
	str.Format("설칭");
}


bool RegistryKeyExists(HKEY hKey, CString& subKey) {
	HKEY hResultKey;
	LONG result = RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hResultKey);

	if (result == ERROR_SUCCESS) {
		// 키가 성공적으로 열렸으면 존재한다고 판단
		RegCloseKey(hResultKey);
		return true;
	}
	else {
		// 키를 열지 못했으면 존재하지 않는다고 판단
		return false;
	}
}

void CTestDlgDlg::OnBnClickedBtnWebview3()
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	CString subKey = "SOFTWARE\\Microsoft\\EdgeUpdate\\ClientState";

	HKEY hResultKey;
	LONG result = RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hResultKey);

	if (result == ERROR_SUCCESS) {
		// 키가 성공적으로 열렸으면 존재한다고 판단
		RegCloseKey(hResultKey);
		OutputDebugString("key exist");
	}
	else {
		// 키를 열지 못했으면 존재하지 않는다고 판단
		OutputDebugString("key not exist");
	}

}


int GetValueUpDownfloor(int ival, int idelta, bool bUP = true)
{
	//내림 
	int number = ival;
	int result_floor{};

	if (number % idelta == 0)
		result_floor = ival;
	else if (bUP)
		result_floor = number + (idelta - (number % idelta));
	else
		result_floor = number - (number % idelta);

	return result_floor;
}

CString GetOrderPriceByMarket(int iprice, CString strCode )
{
	CString slog;
	CString strPrice;
	int code_type;

	enum codetype { kospiType = 1, kosdaqType = 2, thirdType = 3, etfType = 6, etnType = 27, elwType = 9 };

	code_type = codetype::etfType;

	if (code_type == kospiType)
	{
		if (iprice < 1000)
			strPrice.Format("%d", iprice);
		else if (iprice >= 1000 && iprice < 5000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
		else if (iprice >= 5000 && iprice < 10000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 10));
		else if (iprice >= 10000 && iprice < 50000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 50));
		else if (iprice >= 50000 && iprice < 100000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 100));
		else if (iprice >= 100000 && iprice < 500000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 500));
		else
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 1000));
	}
	else if (code_type == kosdaqType)
	{
		if (iprice < 1000)
			strPrice.Format("%d", iprice);
		else if (iprice >= 1000 && iprice < 5000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
		else if (iprice >= 5000 && iprice < 10000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 10));
		else if (iprice >= 10000 && iprice < 50000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 50));
		else
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 100));
	}
	else if (code_type == thirdType)
	{
		if (iprice < 1000)
			strPrice.Format("%d", iprice);
		else if (iprice >= 1000 && iprice < 5000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
		else if (iprice >= 5000 && iprice < 10000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 10));
		else if (iprice >= 10000 && iprice < 50000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 50));
		else if (iprice >= 50000 && iprice < 100000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 100));
		else if (iprice >= 100000 && iprice < 500000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 500));
		else
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 1000));
	}
	else if (code_type == etfType || code_type == etnType)
	{
		if (iprice >= 2000)
		{
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
		}
	}
	else if (code_type == elwType)
	{
		strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
	}
	else
	{
		if (iprice < 1000)
			strPrice.Format("%d", iprice);
		else if (iprice >= 1000 && iprice < 5000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 5));
		else if (iprice >= 5000 && iprice < 10000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 10));
		else if (iprice >= 10000 && iprice < 50000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 50));
		else if (iprice >= 50000 && iprice < 100000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 100));
		else if (iprice >= 100000 && iprice < 500000)
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 500));
		else
			strPrice.Format("%d", GetValueUpDownfloor(iprice, 1000));
	}
	return strPrice;
}

void CTestDlgDlg::OnBnClickedBtnUpdown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sval = GetOrderPriceByMarket(2001, "");
	CString stmp;
}


struct is_odd {
	int num_delete;

	is_odd() : num_delete(0) {}
	
	bool operator()(const int& i) {
		if (num_delete >= 2)
			return false;
		if (i % 2 == 1) {
			num_delete++;
			return true;
		}
		return false;
	}
};


struct is_ptrodd {
	int* num_delete;

	is_ptrodd(int* num_delete) : num_delete(num_delete) {}

	bool operator()(const int& i) {
		if (*num_delete >= 2)
			return false;
		if (i % 2 == 1) {
			(*num_delete)++;
			return true;
		}
		return false;
	}
};


void CTestDlgDlg::OnBnClickedBtnRemovif()
{
	std::vector<int> vec;
	vec.push_back(5);
	vec.push_back(3);
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(6);
	vec.push_back(4);

	testprint(vec.begin(), vec.end());
	int num_delete = 0;
	//vec.erase(std::remove_if(vec.begin(), vec.end(), is_odd()), vec.end());   
	//위처럼 하면 결과가 4개가 아니라 3개가 나온다.
	//remove_if 할때 is_odd 여러번 생성되면서 num_delete 가 다르게 되서 그런다고 한다.
	//그래서 아래처럼 외부에서 생성한값의 주소를 보내줘서 하나의 변수가 영향 받게 하면 된다.
	vec.erase(std::remove_if(vec.begin(), vec.end(), is_ptrodd(&num_delete)), vec.end());
	testprint(vec.begin(), vec.end());

	//[capture list] (받는 인자) -> 리턴 타입 { 함수 본체 } 
	//람다식이라는 함수포인터 같은 녀석을 쓴다 
	auto plm = [](int* i) { 
		return (*i) + 1; 
	};

	auto lm = [](int i) {
		return i + 1;
	};

	int val = 4;
	int ival = lm(val);


	//람다직을 remove_if 에서 ,
	vec.push_back(7);
	vec.push_back(8);
	vec.push_back(9);
	testprint(vec.begin(), vec.end());


	auto lmda = [](int i) { if (i % 2 == 0)
		return true;
	else
		return false;
	};
	//vec.erase(std::remove_if(vec.begin(), vec.end(), [](int i) -> bool { return i % 2 == 1; }), vec.end());
	vec.erase(std::remove_if(vec.begin(), vec.end(), lmda), vec.end());  //짝수를 지웠다....
	testprint(vec.begin(), vec.end());
}

void CTestDlgDlg::OnBnClickedBtnTrans()
{
	std::vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	for (int ii = 0; ii < 10; ii++)
		vec.push_back(ii);

	//transform (시작 반복자, 끝 반복자, 결과를 저장할 컨테이너의 시작 반복자, Pred)
	testprint(vec.begin(), vec.end());
	std::transform(vec.begin(), vec.end(), vec.begin(), [](int i) {     
		return i + 1;
		});
	testprint(vec.begin(), vec.end());
}

void CTestDlgDlg::OnBnClickedBtnFind()
{
	std::vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	for (int ii = 0; ii < 10; ii++)
		vec.push_back(ii);

	testprint(vec.begin(), vec.end());
	auto rt = std::find(vec.begin(), vec.end(), 7);  //7을 가지고 있는 반복자
	int ival = std::distance(vec.begin(), rt);
	ival = vec.at(ival);

	auto current = vec.begin();
	while (true) {
		current = std::find(current, vec.end(), 3);
		if (current == vec.end()) break;
		auto rt = std::distance(vec.begin(), current) + 1;
		std::cout << "3 은 " << std::distance(vec.begin(), current) + 1
			<< " 번째 원소" << std::endl;
		current++;
	}



	auto lamda = [](int i) { if (i == 2 )
		return true;
	else
		return false;
	};

	current = vec.begin();
	while (true)
	{
		current = std::find_if(vec.begin(), vec.end(), lamda);
		if (current == vec.end()) 
			break;
		int ival = *current;
		
		m_slog.Format("\r\n%d", ival);
		OutputDebugString(m_slog);
		current++;
	}
}

#include "CDlg_IE.h"
void CTestDlgDlg::OnBnClickedBtnIebrowser()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDlg_IE dlg;
	dlg.DoModal();
}


void CTestDlgDlg::OnBnClickedBtnIme()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HKEY hKey;
	LONG IRes = RegOpenKeyEx(HKEY_CURRENT_USER, "Keyboard Layout\\Preload", 0, KEY_WRITE, &hKey);
	if (IRes == ERROR_SUCCESS)
	{
		DWORD dwValule = 411041041;

	}
}

void CTestDlgDlg::OnBnClickedBtnIme2()
{
	//HKEY hKey;
	//DWORD dwDisposition;
	//CString regPath = _T("Software\\Microsoft\\Windows\\CurrentVersion\\IME\\IME\\<your_IME_name>");
	//LONG lResult = RegCreateKeyEx(HKEY_CURRENT_USER, regPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition);

	//if (lResult == ERROR_SUCCESS) {
	//	DWORD dwValue = 1; // 예시로 1로 설정
	//	RegSetValueEx(hKey, _T("your_IME_setting"), 0, REG_DWORD, reinterpret_cast<BYTE*>(&dwValue), sizeof(DWORD));
	//	RegCloseKey(hKey);
	//}



	HKEY hKey;
	CString  previousIMEName = "IMEKR"; // 변경하려는 이전 버전의 IME 이름
	// 변경하려는 IME의 레지스트리 경로 설정
	CString regPath = "Software\\Microsoft\\CTF\\TIP\\{previousIMEName}";
	// IME를 변경하기 위해 레지스트리 값을 수정
	LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_WRITE, &hKey);

	if (lResult == ERROR_SUCCESS) {
		AfxMessageBox("open success");
		DWORD dwValue = 1; // 이전 버전의 IME를 사용하도록 설정
		RegSetValueEx(hKey, "Enable", 0, REG_DWORD, reinterpret_cast<BYTE*>(&dwValue), sizeof(DWORD));
		RegCloseKey(hKey);
		std::cout << "이전 버전의 Microsoft IME가 설정되었습니다." << std::endl;
	}
	else {
		AfxMessageBox("open fail");
		std::cout << "레지스트리를 열 수 없습니다." << std::endl;
	}
}

void CTestDlgDlg::OnBnClickedBtnCmd()
{
	CString stmp;

	if (1)
	{
		//CString command = "cmd.exe /C dir C:\\"; // 실행할 명령어
		CString command = "cmd.exe dir"; // 실행할 명령어

		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;

		// cmd.exe 프로세스 시작
		if (CreateProcess(NULL, command.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else {
			std::cout << "cmd.exe 실행 실패!" << std::endl;
		}
	}
	else if(0)
	{
		CString command = "reg query HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion"; // 쿼리할 레지스트리 키

		

		FILE* pipe = _popen(command, "rt");

		if (pipe) {
			char buffer[128];

			while (!feof(pipe)) {
				if (fgets(buffer, 128, pipe) != NULL) {
					stmp.Format("\r\n %s", buffer);
					OutputDebugString(stmp);
				}
			}

			_pclose(pipe);
		}
		else {
			std::cout << "reg query 실행 실패!" << std::endl;
		}
	}
	else if (0)
	{
//#include <cstdlib> // system 함수를 사용하기 위해 필요
//#include <iostream>
		char* command = "reg modify HKEY_CURRENT_USER\\SOFTWARE\\IBK투자증권MAC\\AXIS Workstation V04.00\\Workstation /v CLOUDELOGIN /t REG_DWORD /d 0 /f";

			int result = system(command);
			int ierr = GetLastError();
			if (result == 0) {
				std::cout << "레지스트리 값 수정 성공!" << std::endl;
			}
			else {
				std::cout << "레지스트리 값 수정 실패!" << std::endl;
			}

	}
 
}


void CTestDlgDlg::OnBnClickedBtnEndian()
{
	int test = 1;
	char* ptr = reinterpret_cast<char*>(&test);

	if (*ptr == 1)
		OutputDebugString("\r\n------- little endian----\r\n");
	else
		OutputDebugString("\r\n------- big endian----\r\n");

	
}


void CTestDlgDlg::OnBnClickedBtnCmdpackage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//
	CString stmp;

	if (0)
	{
		CString command = _T("cmd.exe /C wmic qfe get HotFixID > plist.txt");
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		// cmd.exe 프로세스 시작
		if (CreateProcess(NULL, (LPSTR)(LPCTSTR)command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}
	else if (0)
	{
		CString command = _T("cmd.exe");
		CString parameter = _T("/C wmic qfe get HotFixID > plist.txt");
		HRESULT hRes = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		HINSTANCE hInst = ShellExecute(nullptr, nullptr, command, parameter, nullptr, SW_SHOW);
		WaitForSingleObject(hInst, INFINITE);
	}
	else if (0)
	{
		CString command = _T("wmic qfe get HotFixID > plist.txt");
		HINSTANCE hInst = ShellExecute(nullptr, _T("open"), _T("cmd.exe"), _T("/C ") + command, nullptr, SW_HIDE);
		if ((int)hInst <= 32) {
			// ShellExecute가 실패한 경우 처리
			// 오류 메시지 출력 등
			TRACE(" cmd wmic fail");
		}
	}
	
	std::vector<CString> vec;
	CString command = _T("cmd.exe /C wmic qfe get HotFixID");
	
	FILE* pipe = _popen(command, "rt");

	if (pipe) {
		char buffer[128];

		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL) {
				stmp.Format("%s", buffer);
				stmp.Trim();
				vec.push_back(stmp);
				OutputDebugString(stmp);
			}
		}
		_pclose(pipe);
	}
	else {
		std::cout << "reg query 실행 실패!" << std::endl;
	}

	if (!vec.empty())
	{
		int isizeV = vec.size();
		for (const auto& ivec : vec)
		{
			TRACE(ivec + "\r\n");
		}
	}

}

#include "CDlg_wmic.h"
	void CTestDlgDlg::OnBnClickedBtnWmic()
	{
		CDlg_wmic dlg;
		dlg.DoModal();

		return;

		CString stmp;

		if (0)
		{
			CString command = _T("wmic product where \"name like 'Microsoft Visual C++ Redistributable%'\" get Version");
			HANDLE g_hChildStd_OUT_Rd = nullptr;
			HANDLE g_hChildStd_OUT_Wr = nullptr;

			//Anonymous pipe to capture command output
			SECURITY_ATTRIBUTES saAttr;
			saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			saAttr.bInheritHandle = TRUE;
			saAttr.lpSecurityDescriptor = nullptr;

			if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)) {
				AfxMessageBox("pipe create fail");
				return;
			}

			//set properties to ensure the read handle to the pipe for STDOUT is not inherited
			SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0);

			//Create Process
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			si.hStdError = g_hChildStd_OUT_Wr;
			si.hStdOutput = g_hChildStd_OUT_Wr;
			si.dwFlags |= STARTF_USESTDHANDLES;
		}
		else
		{
			CString command;
			command.Format(_T("wmic product get \"%s\", \"%s\""), "VestCert", "Version");
			//command.Format(" /C wmic %s get %s", wnic[0].skey, wnic[0].sval);
			CString cmdLine = _T("cmd.exe") + command;

			FILE* pipe = _popen(cmdLine, "rt");

			if (pipe) {
				char buffer[128];

				while (!feof(pipe)) {
					if (fgets(buffer, 128, pipe) != NULL) {
						stmp.Format("%s", buffer);
						stmp.Trim();

						OutputDebugString(stmp);
					}
				}
	}

	//if (CreateProcess(nullptr, cmdLine.GetBuffer(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi))
	//{
	//	CloseHandle(pi.hProcess);
	//	CloseHandle(pi.hThread);

	//	//Read output from the command
	//	const int BUFSIZE = 4096;
	//	DWORD dwRead;
	//	CHAR chBuf[BUFSIZE];
	//	BOOL bSuccess = FALSE;
	//	CString output = _T("");

	//	for (;;)
	//	{
	//		bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, nullptr);
	//		if (!bSuccess || dwRead == 0)
	//			break;

	//		chBuf[dwRead] = '\0';
	//		output += CString(chBuf);
	//	}


	}
	/*


		for (;;) {
			bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, nullptr);
			if (!bSuccess || dwRead == 0)
				break;

			chBuf[dwRead] = '\0';
			output += CString(chBuf);
		}

		// Output contains the information fetched from wmic command
		// 'output' variable has the Visual Studio Redistributable Package version information
		// You can process 'output' here as needed
		// ...

		CloseHandle(g_hChildStd_OUT_Wr);
		CloseHandle(g_hChildStd_OUT_Rd);
	}
	else {
		// Error handling for process creation failure
	}
	*/
}


void CTestDlgDlg::OnBnClickedBtnRedis()
{
	CString stmp;
	if (1)
	{
		HKEY hKey;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			char szSubKey[MAX_PATH];
			DWORD dwIndex = 0;
			DWORD dwSize = MAX_PATH;

			//해당레지스트리의 하위키를 열거 한다. 
			while (RegEnumKeyEx(hKey, dwIndex, szSubKey, &dwSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
			{
				HKEY hSubKey;
				if (RegOpenKeyEx(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
				{
					OutputDebugString("\r\n-------------------------------------------------------------");
					stmp.Format("\r\n szSubKey=[%s], dwindex=[%d]", szSubKey, dwIndex);
					OutputDebugString(stmp);
					char szDisplayName[MAX_PATH];
					DWORD dwSize = sizeof(szDisplayName);

					//패키지명 버전 확인
					if (RegQueryValueEx(hSubKey, _T("Displayname"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szDisplayName), &dwSize) == ERROR_SUCCESS)
					{  //1D8E6291-B0D5-35EC-8441-6616F567A0F7
						CString strDisplayName(szDisplayName);

						stmp.Format("\r\n strDisplayName =[%s] szSubKey=[%s], dwindex=[%d]", strDisplayName, szSubKey, dwIndex);
						OutputDebugString(stmp);


						if (strDisplayName.Find("Microsoft Visual C++") >= 0)
						{
							char szDisplayVersion[MAX_PATH];
							dwSize = sizeof(szDisplayVersion);

							if (RegQueryValueEx(hSubKey, _T("DisplayVersion"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szDisplayVersion), &dwSize) == ERROR_SUCCESS)
							{
								CString strDisplayVersion(szDisplayVersion);
								stmp.Format("\r\n DisplayVersion =[%s] szSubKey=[%s], dwindex=[%d]", strDisplayVersion, szSubKey, dwIndex);
								OutputDebugString(stmp);
							}
						}
					}

				}
				dwSize = MAX_PATH;
				dwIndex++;
			}//while
		}//if
	}
	else
	{
		HKEY hKey;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
			TCHAR szSubKey[MAX_PATH];
			DWORD dwIndex = 0;
			DWORD dwSize = MAX_PATH;

			// 해당 레지스트리 하위 키를 열거합니다.
			while (RegEnumKeyEx(hKey, dwIndex, szSubKey, &dwSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
				HKEY hSubKey;
				if (RegOpenKeyEx(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
					TCHAR szDisplayName[MAX_PATH];
					DWORD dwSize = sizeof(szDisplayName);

					// DisplayName 값을 읽어서 원하는 패키지를 찾습니다.
					if (RegQueryValueEx(hSubKey, _T("DisplayName"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szDisplayName), &dwSize) == ERROR_SUCCESS) {
						CString strDisplayName(szDisplayName);

						// 원하는 패키지명과 버전을 확인할 수 있습니다.
						if (strDisplayName.Find(_T("Microsoft Visual C++")) != -1) {
							TCHAR szDisplayVersion[MAX_PATH];
							dwSize = sizeof(szDisplayVersion);

							if (RegQueryValueEx(hSubKey, _T("DisplayVersion"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szDisplayVersion), &dwSize) == ERROR_SUCCESS) {
								CString strDisplayVersion(szDisplayVersion);
								std::wcout << _T("Package: ") << strDisplayName << _T(", Version: ") << strDisplayVersion << std::endl;
							}
						}
					}
					RegCloseKey(hSubKey);
				}
				dwSize = MAX_PATH;
				dwIndex++;
			}
			RegCloseKey(hKey);
		}
	}
}



/*
	std::string buffer;
	buffer.resize(10,' ');

	buffer = "1234567890";

	const gsl::span<std::string> spanInter((std::string*)(buffer.data() + 1), 9);

	std::string str;
	/*for_each(spanInter.begin(), spanInter.end(), [&](auto & std::string){

		});*/




BOOL CTestDlgDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	NMHDR* pNMHDR = (NMHDR*)lParam;

	// Edit 컨트롤이 포커스를 받을 때 EN_SETFOCUS 메시지를 처리
	HWND hwndFrom = pNMHDR->hwndFrom;

	if (GetDlgItem(IDC_EDIT_SHARED)->m_hWnd == hwndFrom)
	{
		if (pNMHDR->code == EN_SETFOCUS)
		{
			// 여기서 에디트 컨트롤이 포커스를 받은 것을 처리할 수 있음
			AfxMessageBox(_T("Edit Control received focus!"));
			return TRUE; // 메시지를 처리했음을 알림
		}
	}
	
	return CDialogEx::OnNotify(wParam, lParam, pResult);
}


void CTestDlgDlg::OnClickedBtnJung()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTestDlgDlg::OnBnClickedBtnInterfile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFile	fileH;
	CString	filePath;
	CString stmp;


	struct	_bookmarkinfo {
		char	gubn[1]{};		// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
		char	code[12]{};		// 종목코드
		char	name[32]{};		// 종목명
		char	bookmark[1]{};		// 북마크 여부
	};
#define	sz_bookmark	sizeof(struct _bookmarkinfo)
	struct _bookmarkinfo st_book;
	struct _bookmarkinfo st_copy;
	filePath = "bookmark.i01";
	if (fileH.Open(filePath, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct _bookmarkinfo));
		for (int ii = 0; ii < countC; ii++)
		{
			fileH.Read(&st_book, sizeof(struct _bookmarkinfo));
			memcpy(st_copy.gubn, st_book.gubn, 1);
			//.Format("\r\n  %1s  %12s  %1s  %32s", st_book.gubn, st_book.code, st_book.bookmark, st_book.name);
			OutputDebugString(stmp);
		}
	}

	fileH.Close();
}

void CTestDlgDlg::OnBnClickedBtnInterfile2()
{
	CString filePath, filebackpath;
	filePath = "bookmark.i01";
	filebackpath = "bookmark.i01.tmp";

	CopyFile(filePath, filebackpath, FALSE);
	DeleteFile(filePath);

	CopyFile(filebackpath, filePath,  FALSE);

	OnBnClickedBtnInterfile();
}

//start-------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include<stdlib.h>
char _x2c(__in char hex_up, __in char hex_low) 
    { 
	   char digit;    
       digit = 16 * (hex_up >= 'A' ? ((hex_up & 0xdf) - 'A') + 10 : (hex_up - '0'));    
	   digit += (hex_low >= 'A' ? ((hex_low & 0xdf) - 'A') + 10 : (hex_low - '0'));        
	   return digit;
} 
DWORD url_decode(__out std::string& output_string, __in std::string& encode_string)
{ 
		DWORD error = ERROR_SUCCESS;    
		 do
		 { 
				if (encode_string.empty()) 
				{ 
					error = ERROR_INVALID_PARAMETER;            
					break;
				}        

			   std::string temp_output_string;        

			 for (size_t i = 0; i < encode_string.size(); i++)
			 { 
					switch (encode_string.at(i))
					{ 
						case '%': 
						{               
							temp_output_string.push_back(_x2c(encode_string.at(i + 1), encode_string.at(i + 2)));               
							i += 2;            
						}           
						break;            
						default:
						{               
							temp_output_string.push_back(encode_string.at(i));           
						}            
						break;
						} 
			}     
			if (false == temp_output_string.empty())
			{ 
				output_string.assign(temp_output_string); 
			}
		  } while (false);     
	    return error;
} 
DWORD url_encode(__out std::string& output_string, __in std::string& plain_string) 
{
	  DWORD error = ERROR_SUCCESS;    
	  do
	  {
		if (plain_string.empty())
		{ 
			error = ERROR_INVALID_PARAMETER;            
			break;
		}         
		std::string temp_output_string;         
		for (size_t i = 0; i < plain_string.size(); i++)
		{
			char ch = plain_string.at(i);           
			if (isdigit(ch))
			{ 
				temp_output_string.push_back(ch); 
			}
			else if (isalpha(ch))
			{ 
				temp_output_string.push_back(ch); 
			}
			else if (ch == '$' || // Safe characters 이지만 모두 encoding 해도 무관함.                
				ch == '-' ||               
				ch == '_' ||                
				ch == '.' ||               
				ch == '+' ||               
				ch == '!' ||                
				ch == '*' ||                
				ch == '\'' ||                
				ch == '(' ||                
				ch == ')' ||                
				ch == ',') {
				temp_output_string.push_back(ch);           
			}            
			else 
			{               
				char temp[3] = "";                
				sprintf_s(temp, "%02x", ch);                
				temp_output_string.push_back('%');               
				temp_output_string.push_back(temp[0]);               
				temp_output_string.push_back(temp[1]);           
			}       
		}      

		if (false == temp_output_string.empty()) 
		{   
			output_string.assign(temp_output_string);     
		}    
	  } while (false);     
	return error;
}
//start-------------------------------------------------------------------------------------------------------------
#pragma warning (disable : 4996)  //괜찮은거
int urlencode(unsigned char* source, unsigned char* dest)
{
	unsigned char hex[4];
	unsigned char* sbuf;
	int size = 0;
	sbuf = dest;
	while (*source)
	{
		if ((*source > 47 && *source < 57) ||
			(*source > 64 && *source < 92) ||
			(*source > 96 && *source < 123) ||
			*source == '-' || *source == '.' || *source == '_')
		{
			*sbuf = *source;
		}
		else
		{
			sprintf((char*)hex, "%%%02X", *source);
			strncat((char*)sbuf, (char*)hex, 3);
			*sbuf++;
			*sbuf++;
			size += 2;
		}
		*source++;
		*sbuf++;
		size++;
	}
	return size;
}
				


//start-------------------------------------------------------------------------------------------------------------
CString URLEncode(const char* lpszURL)
{
	CString dest("");

	struct Encode
	{
	private:
		BOOL isexcept(char ch)
		{
			const char excepts[] = {
				'_', '.', 0x00
			};

			for (int i = 0; excepts[i]; i++)
				if (ch == excepts[i])
					return TRUE;
			return FALSE;
		}
	public:
		CString value;

		Encode(char ch)
		{
			if (isalpha(ch) || (ch >= '0' && ch <= '9') || isexcept(ch))
				value = ch;
			else
				value.Format("%%%02X", ch);
		}
	};

	for (int i = 0; i < (int)strlen(lpszURL); i++)
	{
		dest += Encode(lpszURL[i]).value;
	}

	return dest;
}

//start-------------------------------------------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
std::string URLEncodeUTF8(const std::string& strUTF8) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : strUTF8) {
		// 안전한 문자는 그대로 유지, 특수문자는 URL 인코딩
	//	if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
		if ( c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
		}
		else if (c == ' ') {
			escaped << '+';
		}
		else {
			escaped << '%' << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(c));
		}
	}

	return escaped.str();
}

std::string URLEncodeUniqueUTF8(const std::string& strUTF8) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : strUTF8) {
		// 안전한 문자는 그대로 유지, 특수문자는 URL 인코딩
	//	if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
		if (c == ':' || c == '/' || c == '?' || c == '&' || c == '=') {
			escaped << '%' << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(c));
		}
		else
			escaped << c;
	}

	return escaped.str();
}

std::string ToUpperCase(const std::string& input) {
	std::string result = input; // 입력 문자열을 변경하지 않고 복사본을 만듭니다.

	// 각 문자를 대문자로 변환합니다.
	for (char& c : result) {
		c = std::toupper(c);
	}

	return result;
}

#define	HANGULMSK	0x80
CString ExtractHangulCharactersAndURLIncode(const CString& input) {
	CString result;
	if (1)
	{
		CString stmp;

		int len = input.GetLength();
		int i = 0;
		WORD wHangul;
		while (i < len) {
			wchar_t ch = input[i];
			char cName;
			cName = input.GetAt(i);
			if (cName & HANGULMSK)
			{
				stmp = input.Mid(i, 2);

				std::wstring strUni = CA2W(stmp); // std::wstring에 어떤 값이 들어가 있는지에 따라 선언
				std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
				std::string strUTF8 = converter.to_bytes(strUni);
				std::string stdres = ToUpperCase(URLEncodeUTF8(strUTF8));
				CString strEnCode;
				strEnCode = stdres.c_str();
				result += strEnCode;

				i++;
			}
			else if (cName == '(' || cName == ')')
			{
				stmp = input.Mid(i, 1);

				std::wstring strUni = CA2W(stmp); // std::wstring에 어떤 값이 들어가 있는지에 따라 선언
				std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
				std::string strUTF8 = converter.to_bytes(strUni);
				std::string stdres = ToUpperCase(URLEncodeUTF8(strUTF8));
				CString strEnCode;
				strEnCode = stdres.c_str();
				result += strEnCode;
			}
			else
			{
				result += CString(input[i], 1).Trim();
			}
			i++;
		}
	}
	else
	{
		CString stmp;

		int len = input.GetLength();
		int i = 0;
		WORD wHangul;
		while (i < len) {
			wchar_t ch = input[i];

			char cName;
			cName = input.GetAt(i);
			if (cName & HANGULMSK)
			{
				stmp = input.Mid(i, 2);
				result += stmp;
				i++;
			}
			else if (cName == '(' || cName == ')')
			{
				stmp = input.Mid(i, 1);
				result += stmp;
			}
			i++;
		}
	}

	return result;
}







std::string StringToHexString(const std::string& input) {
	std::ostringstream hexStream;

	for (char c : input) {
		hexStream << '%' << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (int)(unsigned char)c;
	}

	return hexStream.str();
}
WORD MakeHangul(char cChar1, char cChar2)
{	// 2Byte문자로부터 한글을 맹글어주라~~
	const WORD	wHangul = (cChar1 << 8) | (0x00FF & cChar2);
	return wHangul;
}


/*
CString result;

	int len = input.GetLength();
	int i = 0;

	while (i < len) {
		wchar_t ch = input[i];

		// 한글 유니코드 범위 확인
		char cName;
		cName = input.GetAt(i);
		if (cName & HANGULMSK)
		{
			result += ch;
			// 한글은 2바이트로 표현되므로, 다음 문자 위치로 이동
			i++;
		}

		i++;
	}
*/
void CTestDlgDlg::OnBnClickedBtnUrlencode1()
{
	//"https:\\download.ibks.com\customer\벤처기업투자신탁저축약관.pdf"
	//"https:\\download.ibks.com\customer\(표준약관)수익증권저축약관.pdf"
	CString strdata;
	strdata = "https://file.funddoctor.co.kr/app/file_download.asp?memb_cd=7570&pfund_cd=12326006&file_gb=R3";
	strdata = "IBK투자증권 HTS(직원용)";
	CString strkor = ExtractHangulCharactersAndURLIncode(strdata);
	CString sUrl;


	std::wstring strUni = CA2W(strkor); // std::wstring에 어떤 값이 들어가 있는지에 따라 선언
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string strUTF8 = converter.to_bytes(strUni);
	std::string stdres= ToUpperCase(URLEncodeUTF8(strUTF8));
	CString strEnCode;
	strEnCode = stdres.c_str();
	strdata.Replace(strkor, strEnCode);

}

#include "CDlg_login.h"

void CTestDlgDlg::OnBnClickedBtnLogin()
{
	CDlg_login dlg;
	dlg.DoModal();
}

#include "CDlg_book.h"
void CTestDlgDlg::OnBnClickedBtnThread6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDlg_book dlg;
	dlg.DoModal();
}

//Decription
// 북마크 파일 처리
// 1 : 이름을 바꾼다
// 2: 삭제한다. : bookmark.ini 파일이 없으면 저장하지 않는다.
void CTestDlgDlg::BookFileProcess(int itype)
{

}


void CTestDlgDlg::OnBnClickedBtnStdmap()
{
	std::vector<std::pair<CString, CString>> vGroupName;
	vGroupName.emplace_back(std::make_pair("11", "01"));
	vGroupName.emplace_back(std::make_pair("22", "02"));
	vGroupName.emplace_back(std::make_pair("33", "03"));
	vGroupName.emplace_back(std::make_pair("44", "04"));
	vGroupName.emplace_back(std::make_pair("55", "05"));
	vGroupName.emplace_back(std::make_pair("66", "06"));

	std::map <int, CString> _map;
	for_each(vGroupName.begin(), vGroupName.end(), [&](const auto item) {
		_map.emplace(std::make_pair(atoi(item.second), item.first));
		});

	CString stmp;
	for_each(_map.begin(), _map.end(), [&](const auto item) {
		stmp.Format("\r\n[%d] [%s]", item.first, item.second);
		OutputDebugString(stmp);
		});
}


void CTestDlgDlg::OnBnClickedBtnDowm()
{
	HRESULT hr = URLDownloadToFile(NULL, "https://go.microsoft.com/fwlink/p/?LinkId=2124703",
		".\\MicrosoftEdgeWebview2Setup.exe", 0, 0);
	//동기 인듯 하다. 
	if (hr == S_OK)
	{
		int returnCode;
		//Either Package the WebView2 Bootstrapper with your app or download it using fwlink
		//then invoke install at Runtime

		SHELLEXECUTEINFO shExInfo = { 0 };
		shExInfo.cbSize = sizeof(shExInfo);
		shExInfo.fMask = SEE_MASK_NOASYNC;
		shExInfo.hwnd = 0;
		shExInfo.lpVerb = "runas";
		shExInfo.lpFile = "MicrosoftEdgeWebview2Setup.exe";
		shExInfo.lpParameters = " /silent /install";
		shExInfo.lpDirectory = 0;
		shExInfo.nShow = 0;
		shExInfo.hInstApp = 0;

		if (ShellExecuteEx(&shExInfo))
		{
			returnCode = 0;  //Install successful
		}
		else
		{
			returnCode = 1; //Install failed
		}

		m_slog.Format("returnCode =[%d]", returnCode);
		AfxMessageBox(m_slog);
	}
}

//#include "qsort.h"
struct _sortStr
{
	CString code;
	CString name;
};

CString g_log;
int _sortName(const void* arg1, const void* arg2)
{
	struct _sortStr* pLVal = (struct _sortStr*)arg1;
	struct _sortStr* pRVal = (struct _sortStr*)arg2;

g_log.Format("\r\n [%s] [%s]", pLVal->code, pRVal->code);
OutputDebugString(g_log);

	return pLVal->code.Compare(pRVal->code);
}

void CTestDlgDlg::OnBnClickedBtnQsort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	struct elwcode {
		char	codx[ELWCodeLen];// ELW종목코드(6) J######
		char	hnam[ELWHNameLen];	// 한글명
	};


	//구조체를 sort 할건데.... 구조체 멤버가 두개다... 특정 멤버를 중심으로 구조체를 소트한다는것이다.

	struct elwcode* code;

	struct _sortStr* ss1, * ss2;
	int num1 = 0, num2 = 0;
	int m_elwcount;

////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::map <CString, _hjcode*> _mapCODEx;
	std::vector<_hjcode*> _vETNx;
	std::map<CString, std::pair<CString, int>> _mapACODE;
	std::map<CString, CString>	_mapNAME;

	CFile	file;
	CString	string = AxStd::FORMAT("%s", "hjcode3.dat");
	if (AxStd::isFileExist(string))
	{
		if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;

		const int size = gsl::narrow_cast<int>(file.GetLength());
		m_hcB = std::make_unique<char[]>(size + 1);
		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		if (size > len)
		{
			m_hcB.reset();
			return;
		}

		m_elwcount = len / sizeof(struct hjcodex);
		ss1 = new _sortStr[m_elwcount];
		ss2 = new _sortStr[m_elwcount];

		struct	hjcodex* hjc = (struct hjcodex*)m_hcB.get();

		for (int ii = 0; ii < m_elwcount; ii++, hjc++)
		{
			CString sCode = CString(hjc->code, HCodeLen).Mid(1);
			CString sNAme = CString(hjc->hnam, ENameLen).Mid(1);
			ss1[ii].code = sCode;
			ss1[ii].name = sNAme;
		}

		qsort((void*)ss1, 5, sizeof(struct _sortStr), _sortName);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////
	//기초자산(이름순)
	

	TRACE("TEST");
}



void CTestDlgDlg::OnBnClickedBtnEdgeself()
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	HWND edge_handle = nullptr;
	CWnd* cWnd = nullptr;
	CString strParam;
	SHELLEXECUTEINFO sinfo = { 0 };

	//CString url;
	//url = "<http://google.com>";
	strParam.Format("--app=\"%s\"", "http://www.ibks.com/HTSHELP/helptrnum.ids?trnum=3008");

	sinfo.cbSize = sizeof(SHELLEXECUTEINFO);
	sinfo.lpFile = "msedge.exe";
	sinfo.lpParameters = strParam;
	sinfo.nShow = SW_SHOW;
	sinfo.fMask = SEE_MASK_NOCLOSEPROCESS;

	if (ShellExecuteEx(&sinfo))
	{
		::WaitForInputIdle(sinfo.hProcess, INFINITE);
		cWnd = FindWindow("Chrome_WidgetWin_1", nullptr);

		edge_handle = cWnd->GetSafeHwnd();
		::MoveWindow(edge_handle, 0, 0, 200, 300, TRUE);
		return;
	}

	return;

}


CString Int2Str(int data, int round)
{
	char buff[32];
	if (round == 0)
	{
		sprintf(buff, "%d", data);
	}
	else
	{
		int pval = (int)pow(10, round);
		if (data < 0)
			sprintf(buff, "-%d.%0.*d", abs(data) / pval, round, abs(data) % pval);
		else
			sprintf(buff, "%d.%0.*d", data / pval, round, data % pval);
	}
	return buff;
}

void CTestDlgDlg::OnBnClickedBtnIntstr()
{
	int ival = 10949;
	int ival2 = 10756;
	double frate = (10949 - 10756) / 10756;
	double frate1 = (ival - ival2) / (double)ival2;

	double d1, d2, d3;
	d1 = ((double)ival / 100) - ((double)ival2) / 100;
	d2 = (double)ival2 / 100;
	d3 = d1 / d2;
	CString str;
	str = Int2Str((int)d3, 2);

}


void CTestDlgDlg::OnBnClickedBtnUrlen2()
{
	//"https:\\download.ibks.com\customer\벤처기업투자신탁저축약관.pdf"
//"https:\\download.ibks.com\customer\(표준약관)수익증권저축약관.pdf"

	CString strdata;
	strdata = "https://file.funddoctor.co.kr/app/file_download.asp?memb_cd=7570&pfund_cd=12326006&file_gb=R3";


	std::wstring strUni = CA2W(strdata); // std::wstring에 어떤 값이 들어가 있는지에 따라 선언
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string strUTF8 = converter.to_bytes(strUni);
	std::string stdres = ToUpperCase(URLEncodeUniqueUTF8(strUTF8));
	CString strEnCode;
	strEnCode = stdres.c_str();
	
}


#define	HANGULMSK	0x80

CString ConvertToKorean(const unsigned char* memory) {
	// 2바이트 문자를 유니코드로 변환하여 CString 객체에 저장
	CString korean(reinterpret_cast<const wchar_t*>(memory));

	return korean;
}

#include "CSRWSR.h"
void CTestDlgDlg::OnBnClickedBtnGetsection()
{
	//cs_RW _cs;
	////BOOL bres = _cs.MakeFile_MB("test.ini", "");

	//BOOL buni = _cs.IsUniCodeFile("D:\\IBKS_TEST\\src\\APPL\\TestDlg\\Debug\\testuni.ini");
	//buni = _cs.IsUniCodeFile("D:\\IBKS_TEST\\src\\APPL\\TestDlg\\Debug\\test.ini");

	//CStringW wstrResult;
	//CString strtmp;
	//_cs.ReadFile_Uni("D:\\IBKS_TEST\\src\\APPL\\TestDlg\\Debug\\testuni.ini", "MAKEFILE", wstrResult, strtmp);
	//
	//CString strResult;
	//_cs.ReadFile_MB("D:\\IBKS_TEST\\src\\APPL\\TestDlg\\Debug\\testuni.ini", "MAKEFILE", strResult);
	//_cs.ReadFile_MB("D:\\IBKS_TEST\\src\\APPL\\TestDlg\\Debug\\test.ini", "MAKEFILE", strResult);

	//_cs.GetFile_MAKE_Date("D:\\IBKS_TEST\\src\\APPL\\TestDlg\\Debug\\test.ini", strResult);

	//USES_CONVERSION;
	//CStringW str;
	//char* pdata = "abcd한efg";
	//_cs.convert_ansi_to_unicode(str, pdata, strlen(pdata));
	//CStringW str1;
	//str1 = CA2W(pdata, CP_ACP);
	//strtmp  = CA2W(pdata, CP_ACP);
	// 

	//strResult.Empty();
	//_cs.convert_unicode_to_ansi(strResult, str.GetBuffer(0), str.GetLength());

	//
	//wchar_t* pwchar = L"abcd한efg";
	////2바이트로 알아서 저장
	////CString strtemp(pwchar);
	//CString strtemp;
	//strtemp = CW2A(pwchar, CP_ACP); //utf8로 저장  61 62 63 64 ed 95 9c 65 66 67
	//

	//strtemp.Empty();
	//
	//int ilen = lstrlenW(pwchar);
	//ilen = str.GetLength();
	//str1 = CA2W(pdata, CP_UTF8);  //61 00 62 00 63 00 64 00 fd ff fd ff 65 00 66 00 67 00
	//strtemp = CA2A(pdata, CP_UTF8);
	//_cs.convert_unicode_to_utf8(strtemp, pwchar, ilen);  //61 62 63 64 ed 95 9c 65 66 67

	//str1.Empty();
	//str1 = CW2W(pwchar, CP_UTF8);
	//strtemp = str1;

	//CStringW tt = L"abcd한efg";
	//strtemp = tt;
}

/*
if (stmp.GetAt(ii) & HANGULMSK)
			{
				WORD wHangul{};
				//stemp.Format("%c%c", stmp.GetAt(ii), stmp.GetAt(ii + 1));  //60824
			//	wHangul = MakeHangul(stmp.GetAt(ii + 1),stmp.GetAt(ii));
				wHangul = MakeHangul(stmp.GetAt(ii ), stmp.GetAt(ii + 1));
				std::wstring koreanCharacter;
				koreanCharacter.push_back(static_cast<wchar_t>(wHangul));
				stemp.Format("%c%c", 0xed,0x98);  //60824
				CString result(koreanCharacter.c_str());

				//CString koreanString = ConvertToKorean((const unsigned char*)stmp[ii]);
				//stemp.Format("%c", MakeHangul(stmp.GetAt(ii), stmp.GetAt(ii + 1))  );
				//stemp.Format("%2s", stmp.GetAt(ii));
				//char temp[3] = "";
				//sprintf_s(temp, "%02x", stmp[ii]);
				ii += 2;
			}
			else
			{
				stemp.Format("%c", stmp[ii]);
				ii += 1;
			}
			string += stemp;
*/

void CTestDlgDlg::OnBnClickedBtnCmd2()
{
	/*
	무선 LAN 어댑터 Wi-Fi:

   연결별 DNS 접미사. . . . : 
   링크-로컬 IPv6 주소 . . . . : fe80::ed88:93f0:1337:cf01%3
   IPv4 주소 . . . . . . . . . : 192.168.100.57
   서브넷 마스크 . . . . . . . : 255.255.255.0
   기본 게이트웨이 . . . . . . : 192.168.100.1

이더넷 어댑터 이더넷:

   미디어 상태 . . . . . . . . : 미디어 연결 끊김
   연결별 DNS 접미사. . . . : Davolink

이더넷 어댑터 이더넷 3:

   연결별 DNS 접미사. . . . : 
   링크-로컬 IPv6 주소 . . . . : fe80::86f2:3249:910:1459%11
   IPv4 주소 . . . . . . . . . : 192.168.56.1
   서브넷 마스크 . . . . . . . : 255.255.255.0
   기본 게이트웨이 . . . . . . : 
	*/
	CString  strcmd, slog;
	((CWnd*)GetDlgItem(IDC_EDIT_CMD))->SetWindowText("ipconfig");
	((CWnd*)GetDlgItem(IDC_EDIT_CMD))->GetWindowText(strcmd);

	CString command;
	//command.Format("cmd.exe /C %s %s", "ipconfig", "/all");
	command.Format("cmd.exe /C %s ", "ipconfig");

	// CreateProcess에 필요한 변수 선언
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&sa, sizeof(sa));
	si.cb = sizeof(si);
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;

	// 표준 출력 파이프 생성
	HANDLE hPipeRead, hPipeWrite;
	if (!CreatePipe(&hPipeRead, &hPipeWrite, &sa, 0)) {
		AfxMessageBox("표준 출력 파이프 생성에 실패했습니다.");
		return ;
	}

	// 명령 실행
	si.hStdOutput = hPipeWrite;
	si.dwFlags |= STARTF_USESTDHANDLES;
	if (CreateProcess(NULL, command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		// 프로세스가 생성되었으면 표준 출력을 읽기
		CloseHandle(hPipeWrite);
	//	WaitForSingleObject(pi.hProcess, INFINITE); //이거 이상하다

		// 결과 읽기
		DWORD bytesRead;
		CHAR buffer[4096];
		std::stringstream resultStream;
		while (ReadFile(hPipeRead, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead != 0) {
			resultStream << buffer;
		}

		// 결과 문자열 출력
		CStringW stmp;
		 stmp = CA2W(CString(resultStream.str().c_str()), CP_ACP);
		
		 CString sResult;
		 int pos = 0;
		 while (1)
		 {
			 pos = stmp.Find(L"\r\n");
			 if (pos < 0)
				 break;
			 sResult = stmp.Left(pos);
			 OutputDebugString("\r\n" + sResult);
			 if (sResult.Find("IPv4") >= 0)
			 {
				 int ifind = 0;
				 ifind = sResult.ReverseFind(':');
				 if (ifind)
				 {
					 sResult = sResult.Mid(ifind + 1);
					 sResult.Trim();
					 ifind = sResult.ReverseFind('.');  
					 //IPv4 주소: 192.168.56.1 가 이런식으로 게이트웨이?의 주소로 존재하면 아니다
					 if (sResult.Mid(ifind + 1).Trim() != "1")
						 break;
				 }
				 
			 }
			 stmp = stmp.Mid(2+pos);
		 }
		
		// 프로세스 종료 후 핸들 해제
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else {
		// 프로세스 실행에 실패한 경우 에러 메시지 출력
		AfxMessageBox("ipconfig 실행에 실패했습니다.");
	}

}


#include "CSockDlg.h"
void CTestDlgDlg::OnBnClickedBtnSock()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSockDlg dlg;
	dlg.DoModal();

	



}



void CTestDlgDlg::OnBnClickedBtnSession()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CInternetSession session;
	bool m_bServerError;
	try {
		CInternetFile* p_html = (CInternetFile*)session.OpenURL("123");

		if (p_html != NULL) {
			CString str, html_text;

			p_html->SetReadBufferSize(200);
			while (1) {
				if (!p_html->ReadString(str)) break;
				html_text += str;
			}

			p_html->Close();
			str.Format(_T("result=%s"), "N");
			if (html_text.Find(str) >= 0)
			{
				m_bServerError = true;
				return;
			}

			delete p_html;
			session.Close();
		}
	}
	catch (...)
	{
		TRACE("test");
	}
}


void CTestDlgDlg::OnBnClickedBtnCompress()
{
	char* pbuff = { "123451234512345123451234512345" };
	char* pdest = new char[100];
	memset(pdest, 0x00, 100);
	m_pzip = new cl_testcompress;
	m_pzip->compress((BYTE*)pbuff, strlen(pbuff), (BYTE*)pdest);

	int iLWZ_len = strlen(pdest);
	char* pdecomdest = new char[100];
	memset(pdecomdest, 0x00, 100);
	m_pzip->decompress((BYTE *)pdest, strlen(pdest), (BYTE * )pdecomdest);

	std::string original = "abcabcabcabc";

	std::string compressed = m_pzip->string_compress(original);
	std::string decommpressed = 	m_pzip->string_decompress(compressed);
	TRACE("test");

}

#define UG_CLASS_DECL AFX_CLASS_EXPORT
#include "monthWnd.h"

BOOL ConvertStringToDate(CString& string, COleDateTime& date)
{
	return date.ParseDateTime(string, VAR_DATEVALUEONLY);

}

#include "CDlg_Calendar.h"
void CTestDlgDlg::OnBnClickedBtnCalender()
{
	CDlg_Calendar dlg;
	dlg.DoModal();
}


void CTestDlgDlg::OnBnClickedBtnCalender2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


DWORD g_dwProcessId = 0;

// EnumWindows 프로시저를 위한 콜백 함수
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == g_dwProcessId) {
		// 메인 윈도우를 찾았을 때, 포커스를 설정
		SetForegroundWindow(hwnd);
		return FALSE; // 검색 중단
	}
	return TRUE; // 검색 계속
}

void CTestDlgDlg::OnBnClickedBtnExcute()
{
	DWORD size = MAX_PATH;
	char buff[MAX_PATH];

	int err = AssocQueryStringA(ASSOCF_INIT_IGNOREUNKNOWN, ASSOCSTR_EXECUTABLE, ".html", NULL, buff, &size);
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	CStringA command_line;
	CStringA target_url("http://google.com/");


	if (strcmp("chrome.exe", PathFindFileNameA(buff)) == 0)
		command_line.Format("%s --new-window %s", buff, target_url);
	else if (strcmp("firefox.exe", PathFindFileNameA(buff)) == 0)
		command_line.Format("%s -new-instance %s", buff, target_url);
	else
		command_line.Format("%s %s", buff, target_url);


	if (!CreateProcessA(NULL,     // No module name (use command line)
		command_line.GetBuffer(),
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inhberitable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		//... error handling
		return;
	}

	g_dwProcessId = pi.dwProcessId;

	Sleep(1000); // 예제를 위한 단순 대기, 실제 사용시 적절한 대기 메커니즘 고려

	  // EnumWindows를 사용하여 모든 윈도우를 열거하고, EnumWindowsProc 콜백을 통해 메인 윈도우를 찾음
	EnumWindows(EnumWindowsProc, 0);
	
}


void CTestDlgDlg::OnBnClickedBtnOsver()
{
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	HMODULE hMod{};
	RtlGetVersion_FUNC func{};


	OSVERSIONINFOEXW* osw = (OSVERSIONINFOEXW * )&os;

	hMod = LoadLibrary(TEXT("ntdll.dll"));
	if (hMod)
	{
		func = (RtlGetVersion_FUNC)GetProcAddress(hMod, "RtlGetVersion");
		if (func == 0)
		{
			FreeLibrary(hMod);
			return ;
		}
		ZeroMemory(osw, sizeof(*osw));
		osw->dwOSVersionInfoSize = sizeof(*osw);
		func(osw);

		os.dwBuildNumber = osw->dwBuildNumber;
		os.dwMajorVersion = osw->dwMajorVersion;
		os.dwMinorVersion = osw->dwMinorVersion;
		os.dwPlatformId = osw->dwPlatformId;
		os.dwOSVersionInfoSize = sizeof(os);
	}
	else
		return ;
	FreeLibrary(hMod);
	//if (!GetVersion(&osvi))
	//	return

}


void CTestDlgDlg::OnBnClickedBtnAny()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::any a1 = 3;
	const std::type_info& aa = a1.type();
	if (a1.type()== typeid(int))
		TRACE("int type");

	if (a1.has_value())
		TRACE("a1 has value");

	int anycaseaa = std::any_cast<int&>(a1);
	auto ptr_test = std::any_cast<int>(&a1);
	auto ptr_test1 = std::any_cast<std::string>(&a1);;

	/*
	cs_RW _csRW;
	std::any a2 = _csRW;
	const std::type_info& type_a2 = a2.type();
	if(a2.type() == typeid(cs_RW))
		TRACE("class type");

	if (a2.has_value())
		TRACE("a1 has value");
		*/
}


void CTestDlgDlg::OnBnClickedBtnPop()
{
	CString strPath;
	char chfile[500]{};
	GetModuleFileName(nullptr, chfile, 260);
	strPath.Format("%s", chfile);
	strPath.Trim();
	int ifind = strPath.ReverseFind('\\');
	strPath = strPath.Left(ifind);



	HINSTANCE result = ShellExecute(NULL, "explore", strPath, NULL, NULL, SW_SHOWNORMAL);
	if ((int)result <= 32) {
		// ShellExecute 함수가 실패한 경우
		AfxMessageBox("탐색기를 열 수 없습니다.", MB_OK | MB_ICONERROR);
		return;
	}
}

#include <nb30.h>
#include <Iphlpapi.h>
#pragma comment (lib, "IpHlpApi.lib")


CString GetCurrentMACAddress() {
	ULONG bufLen = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES adapterAddresses = (IP_ADAPTER_ADDRESSES*)malloc(bufLen);

	if (adapterAddresses == NULL) {
		return CString("Error: Memory allocation failure");
	}

	DWORD result = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapterAddresses, &bufLen);
	if (result == ERROR_BUFFER_OVERFLOW) {
		free(adapterAddresses);
		adapterAddresses = (IP_ADAPTER_ADDRESSES*)malloc(bufLen);
		if (adapterAddresses == NULL) {
			return CString("Error: Memory allocation failure");
		}
		result = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapterAddresses, &bufLen);
	}

	CString currentMACAddress;
	CString stemp, stmp;
	if (result == NO_ERROR) {
		PIP_ADAPTER_ADDRESSES adapter = adapterAddresses;
		while (adapter) {  //일부러 거르지는 않았다 맨 위에 있는 놈이 실질적 mac address  조건이 너무 어려움
		//	if (adapter->OperStatus == IfOperStatusUp && adapter->IfType == IF_TYPE_ETHERNET_CSMACD)
			{
				stemp.Format("%16x", adapter->PhysicalAddress[0]);
				stemp.Trim();
				currentMACAddress += stemp;

		
				for (UINT i = 1; i < adapter->PhysicalAddressLength; i++) {
					currentMACAddress.AppendFormat(_T(":%02X"), adapter->PhysicalAddress[i]);
				}
			//	break; // 현재 사용 중인 네트워크 어댑터의 MAC 주소를 찾았으므로 반복문 종료
			}
			currentMACAddress += "\n";
			adapter = adapter->Next;
		}
	}
	else {
		currentMACAddress.Format(_T("Error: Unable to retrieve adapter addresses. Error code: %lu"), result);
	}

	free(adapterAddresses);
	return currentMACAddress;
}

void CTestDlgDlg::OnBnClickedBtnMac()
{
	CString strtestmac;
	strtestmac = GetCurrentMACAddress();
	
	ULONG bufLen = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO adapterInfo = (IP_ADAPTER_INFO*)malloc(bufLen);

	if (adapterInfo == NULL) {
		//return CString("Error: Memory allocation failure");
		return;
	}

	DWORD result = GetAdaptersInfo(adapterInfo, &bufLen);
	if (result == ERROR_BUFFER_OVERFLOW) {
		free(adapterInfo);
		adapterInfo = (IP_ADAPTER_INFO*)malloc(bufLen);
		if (adapterInfo == NULL) {
			//return CString("Error: Memory allocation failure");
			return;
		}
		result = GetAdaptersInfo(adapterInfo, &bufLen);
	}

	CString stmp, strmac;
	CString macAddress;
	if (result == NO_ERROR) {
		PIP_ADAPTER_INFO adapter = adapterInfo;
		while (adapter) {
		//	if (adapter->Type == MIB_IF_TYPE_ETHERNET)
			{
				stmp.Format("%16x", adapter->Address[0]);
				stmp.Trim();
				macAddress += stmp;
				strmac += stmp;
				for (UINT i = 1; i < adapter->AddressLength; i++) {
					macAddress.AppendFormat(_T(":%02X"), adapter->Address[i]);
					strmac.AppendFormat(_T(":%02X"), adapter->Address[i]);
				}
				macAddress += "\n";

				
				strmac.Empty();
			}
			adapter = adapter->Next;
		}
	}
	else {
		macAddress.Format(_T("Error: Unable to retrieve adapter information. Error code: %lu"), result);
	}

	free(adapterInfo);
}


void CTestDlgDlg::OnBnClickedBtnIpconfigall()
{
	CString  strcmd, slog, sResult;
	

	CString command;
	command.Format("cmd.exe /C %s %s", "ipconfig", "/all");

	// CreateProcess에 필요한 변수 선언
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&sa, sizeof(sa));
	si.cb = sizeof(si);
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;

	// 표준 출력 파이프 생성
	HANDLE hPipeRead, hPipeWrite;
	if (!CreatePipe(&hPipeRead, &hPipeWrite, &sa, 0)) {
		AfxMessageBox("표준 출력 파이프 생성에 실패했습니다.");
		return;
	}

	// 명령 실행
	si.hStdOutput = hPipeWrite;
	si.dwFlags |= STARTF_USESTDHANDLES;
	if (CreateProcess(NULL, command.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		// 프로세스가 생성되었으면 표준 출력을 읽기
		CloseHandle(hPipeWrite);
		//	WaitForSingleObject(pi.hProcess, INFINITE); //이거 이상하다

			// 결과 읽기
		DWORD bytesRead;
		CHAR buffer[4096]{};

		while (ReadFile(hPipeRead, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead != 0) {
			strcmd += CString(buffer).Trim();
			strcmd += "\r\n";
		}


		int pos = 0;
		std::map<int, CString> intmap;
	
		int iIndex{};
		while (1)
		{
			pos = strcmd.Find("\r\n");
			if (pos < 0)
				break;
			sResult = strcmd.Left(pos);
			OutputDebugString("\r\n" + sResult);
			if (sResult.Find("물리적 주소") >= 0)
			{
				iIndex++;
				
				int ifind = 0;
				ifind = sResult.ReverseFind(':');
				if (ifind)
				{
					sResult = sResult.Mid(ifind + 1);
					sResult.Trim();
					intmap.insert(std::pair<int, CString>(iIndex, sResult));
				}
				else
				{
					intmap.insert(std::pair<int, CString>(iIndex, ""));
				}
			}
			if (sResult.Find("기본 게이트웨이") >= 0)
			{
				std::map<int, CString>::iterator it = intmap.find(iIndex);
				if (it != intmap.end())
				{
					CString stmp;
					stmp = it->second.GetString();
					if (!stmp.IsEmpty())
					{
						int ifind = 0;
						ifind = sResult.ReverseFind(':');
						if (ifind)
						{
							sResult = sResult.Mid(ifind + 1);
							sResult.Trim();
							if (!sResult.IsEmpty())
							{
								stmp.Replace("-", "");
								CString strMacAddress = stmp;   //각각 어댑터에서 기본게이트웨이가 활성화 되있는 사용되는 녀석이다
							}
						}
					}
				}
			}
			strcmd = strcmd.Mid(2 + pos);
		}

		// 프로세스 종료 후 핸들 해제
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else {
		// 프로세스 실행에 실패한 경우 에러 메시지 출력
		AfxMessageBox("ipconfig 실행에 실패했습니다.");
	}
}


void CTestDlgDlg::OnBnClickedBtnFileload()
{
	CString path;
	CFile	file;

	//m_pJCode.RemoveAll();

	//path.Format("%s\\%s\\%s", tabPath, TABDIR, HJCODEx);
	//if (!file.Open(path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	//{
	//	CString	msg;
	//	msg.Format("[%s] file open error!!", path);
	//	AfxMessageBox(msg);
	//	return ;
	//}

	//int	size = (int)file.GetLength();
	//std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);

	//const	int	ival = file.Read(pWb.get(), size);
	//file.Close();
	//if (ival != size)
	//{
	//	CString	msg;
	//	msg.Format("[%s] file read error!!", path);
	//	AfxMessageBox(msg);
	//	pWb.reset();
	//	return ;
	//}

	//struct	_JCode	jCode {};
	//struct  hjcodex* pcodex = (struct hjcodex*)pWb.get();
	//size /= sizeof(struct hjcodex);
	//for (int ii = 0; ii < size; ii++, pcodex++)
	//{
	//	switch (pcodex->ssgb)
	//	{
	//	case jmSINJS:
	//	case jmSINJU:
	//	case jmHYFND:
	//		continue;
	//	default:
	//		break;
	//	}

	//	jCode.Code = CString(pcodex->code, HCodeLen);
	//	jCode.Code.TrimRight();
	//	if (jCode.Code.IsEmpty())
	//		continue;

	//	jCode.Name = CString(pcodex->hnam, HNameLen);
	//	jCode.Name.MakeUpper();
	//	jCode.Name.TrimRight();
	//	if (pcodex->ssgb != jmELW)
	//		jCode.Name = jCode.Name.Mid(1);
	//	m_pJCode.Add(jCode);
	//}

	//m_pJCode.QuickSort();
	//pWb.reset();
}


double Round(double data) //0.003제세금(반올림)
{
	return (int)(data + 0.5);
}

double RoundDown2(double data)
{
	return ((int)Round(data));
}

void CTestDlgDlg::OnBnClickedBtnRound()
{
	double dData = RoundDown2(1.0095);
	 dData = RoundDown(1.0095);
	CString str;

	double koga = 2.35;
	//koga = pow(koga, 1);

	double siga = 2.3;
	//siga = pow(siga, 1);

	double jega = 0.12;
	//jega = pow(jega, 1);
	int irc_height = 28;
	double m_curr{};
	m_curr = 0.11;
	double res = koga - jega;
	double ret = (koga - siga) / (koga - jega);
	int	sigaY = (int)((irc_height * (koga - siga)) / (koga - jega));
	int	jegaY = (int)((irc_height * (koga - jega)) / (koga - jega));
	int    jjgaY = (int)((irc_height * (koga - m_curr)) / (koga - jega));
	//int	pivotY = (int)((irc_height * (koga - pivot)) / (koga - jega));

	CString stmp;
	stmp = "00.01";
	m_curr = atof(stmp);

	if (stmp.GetAt(0) == '0')
		stmp = stmp.Mid(1);
}


BOOL ConnectWithTimeout(const char* ip, int port, int timeoutMS)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return FALSE;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		WSACleanup();
		return FALSE;
	}

	// Non-blocking 모드
	u_long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);

	sockaddr_in addr;
	ZeroMemory(&addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	int ret = connect(sock, (sockaddr*)&addr, sizeof(addr));

	if (ret == SOCKET_ERROR)
	{
		int err = WSAGetLastError();

		// 연결 진행중이면 정상
		if (err != WSAEWOULDBLOCK &&
			err != WSAEINPROGRESS &&
			err != WSAEINVAL)
		{
			closesocket(sock);
			WSACleanup();
			return FALSE;
		}
	}

	fd_set writeSet;
	FD_ZERO(&writeSet);
	FD_SET(sock, &writeSet);

	timeval tv;
	tv.tv_sec = timeoutMS / 1000;
	tv.tv_usec = (timeoutMS % 1000) * 1000;

	ret = select(0, NULL, &writeSet, NULL, &tv);

	BOOL bConnected = FALSE;

	if (ret > 0)
	{
		int so_error = 0;
		int len = sizeof(so_error);

		getsockopt(sock,
			SOL_SOCKET,
			SO_ERROR,
			(char*)&so_error,
			&len);

		if (so_error == 0)
			bConnected = TRUE;
	}

	closesocket(sock);
	WSACleanup();

	return bConnected;
}

void CTestDlgDlg::OnBnClickedBtnRe()
{//회사망체크
	if (ConnectWithTimeout("172.16.202.106", 15201, 500))
	{
		AfxMessageBox(_T("회사망입니다."));
	}
	else
	{
		AfxMessageBox(_T("외부망입니다."));
	}
}

#include <Windows.h>
#include <atlstr.h> // CString 클래스를 포함하는 헤더 파일

CString URLEncode(const CString& url) {
	CString encodedURL;
	DWORD dwSize = ::InternetCanonicalizeUrl(url, nullptr, 0, ICU_DECODE); // 디코딩하여 필요한 버퍼 크기 확인
	LPSTR lpEncodedURL = new CHAR[dwSize]; // 인코딩된 URL을 저장하기 위한 버퍼 생성

	if (::InternetCanonicalizeUrl(url, lpEncodedURL, &dwSize, ICU_ENCODE_SPACES_ONLY) == TRUE) {
		encodedURL = CString(lpEncodedURL);
	}
	else {
		// 인코딩 실패 처리
	}

	delete[] lpEncodedURL; // 버퍼 해제
	return encodedURL;
}

void CTestDlgDlg::OnBnClickedBtnCmap()
{
	std::unordered_map<int, int> myMap;
	myMap[3] = 10;
	int ival{}, ival2{};
	int arr[] = { 1,2,3,4,5,6,7,8,9,10 };

	for (const auto symbol : arr)
	{
		auto  at = myMap.find(symbol);
		if (at != myMap.end())
		{
			ival = at->first;
			ival2 = at->second;
		}
	}


	int isize = _m_mapRealData.size();

	auto it = _m_mapRealData.find("005931");
	if (it != _m_mapRealData.end())
	{
		_Ralert* pval = it->second.get();
		//memset(item.second.get()->ptr[0].get(), 0x00, strlen(item.second.get()->ptr[0].get()));
		pval->ptr[0].reset(std::make_unique<char[]>(5).release());
		memcpy(pval->ptr[0].get(), "9999", 4);
	}
	else
	{
		const auto mt = _m_mapRealData.emplace("005932", std::make_unique<struct _Ralert>());
		mt.first->second->ptr[0].reset(std::make_unique<char[]>(4).release());
		ZeroMemory(mt.first->second->ptr[0].get(), 4);
		memcpy((char*)mt.first->second->ptr[0].get(), "123", 3);
	}

	isize = _m_mapRealData.size();
	
	it = _m_mapRealData.find("005932");
	if (it != _m_mapRealData.end())
	{
		_Ralert* pval = it->second.get();
		//memset(item.second.get()->ptr[0].get(), 0x00, strlen(item.second.get()->ptr[0].get()));
		pval->ptr[0].reset(std::make_unique<char[]>(5).release());
		memcpy(pval->ptr[0].get(), "9999", 4);
		isize = _m_mapRealData.size();
	}
	
	
	CString str;

	//std::map<CString, int>    _mRealtime;

	//const auto mRt = _mRealtime.emplace(std::make_pair("test", 99));
	//ival = mRt.first->second;
}


DWORD WINAPI ErrorWriting(void* lpParameter)
{
	CTestDlgDlg* pdlg = (CTestDlgDlg*)lpParameter;
	
	int returnCode = 2;
	int cnt{};
	CString stmp;
	while (1)
	{
		stmp.Format("\r\n-------------thread log = [%s]----------", pdlg->m_strThreadData);
		OutputDebugString(stmp);
		cnt++;
		Sleep(1000);
		if (cnt >= 5)
			break;
	}
	pdlg->m_bWritingThread = FALSE;

/*
	HRESULT hr = URLDownloadToFile(NULL, "https://go.microsoft.com/fwlink/p/?LinkId=2124703",
		".\\MicrosoftEdgeWebview2Setup.exe", 0, 0);
	//동기 인듯 하다. 
	if (hr == S_OK)
	{
		//Either Package the WebView2 Bootstrapper with your app or download it using fwlink
		//then invoke install at Runtime

		SHELLEXECUTEINFO shExInfo = { 0 };
		shExInfo.cbSize = sizeof(shExInfo);
		shExInfo.fMask = SEE_MASK_NOASYNC;
		shExInfo.hwnd = 0;
		shExInfo.lpVerb = "runas";
		shExInfo.lpFile = "MicrosoftEdgeWebview2Setup.exe";
		shExInfo.lpParameters = " /silent /install";
		shExInfo.lpDirectory = 0;
		shExInfo.nShow = 0;
		shExInfo.hInstApp = 0;

		if (ShellExecuteEx(&shExInfo))
		{
			returnCode = 0;  //Install successful
		}
		else
		{
			returnCode = 1; //Install failed
		}
	}

	appWindow->InstallComplete(returnCode);
	appWindow->Release();
	
	return returnCode;
	*/
	return 0;
}



void CTestDlgDlg::OnBnClickedBtnForthread()
{
	BOOL bStartThread{};
	for (int ii = 0; ii < 100; ii++)
	{
		if (!m_bWritingThread)
		{
			m_bWritingThread = TRUE;
			bStartThread = TRUE;
			OutputDebugString("\r\n !!!!!!! thread!!!!!!");
			m_strThreadData.Format("ii=[%d]", ii);
			_beginthreadex(0, 0, (_beginthreadex_proc_type)ErrorWriting, (void*)this, 0, 0);
		}
	}
}

BOOL CTestDlgDlg::GetETNMst(CStringArray*& pastrUpCode, CStringArray*& pastrUpName, CStringArray*& pastrUpEngName, int nOption)
{
	int isize = pastrUpCode->GetCount();
	 isize = pastrUpName->GetCount();
	 isize = pastrUpEngName->GetCount();
	 isize = nOption;
	return FALSE;
}

BOOL CTestDlgDlg::GetETNMst1(CStringArray*& pastrUpCode, CStringArray*& pastrUpName, CStringArray*& pastrUpEngName, int nOption)
{
	int isize = pastrUpCode->GetCount();
	isize = pastrUpName->GetCount();
	isize = pastrUpEngName->GetCount();
	isize = nOption;
	return FALSE;
}

void test1(int*& pdata)
{
	CString str;
	*pdata = 123;
}

#include "CUploadFile.h"

void CTestDlgDlg::OnBnClickedBtnCommarco()
{
	int ival = 99;
	int* pval = &ival;
	test1(pval);


	CStringArray arr,arr1, arr2;
	arr.Add("123");
	arr1.Add("456");
	arr2.Add("789");

	CStringArray* pArr = &arr;
	CStringArray* pArr1 = &arr1;
	CStringArray* pArr2 = &arr2;
	GetETNMst(pArr, pArr1, pArr2, 1);

	GetETNMst((CStringArray *&)arr, (CStringArray*&)arr1, (CStringArray*&)arr2, 1);
	GetETNMst(reinterpret_cast<CStringArray*&>(arr), reinterpret_cast<CStringArray*&>(arr1), reinterpret_cast<CStringArray*&>(arr2), 1);

	CUploadFile cs_test;
	//cs_test.upload("test");

	CString sFolderPath, filename;
	sFolderPath = "F:\\util\\HTS\\IBK투자증권 HTS\\user\\1694194482\\Crashlog";

	WIN32_FIND_DATA FindFileData;
	HANDLE		hFind;
	CFile		cfile;
	CString		allfile;

	allfile = sFolderPath;
	allfile += "\\*.dmp";
	hFind = FindFirstFile(allfile, &FindFileData);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
		//	filename.Format("%s\\%s\\%s\\crash\\%s", Axis::home, USRDIR, Axis::user, FindFileData.cFileName);
			filename = FindFileData.cFileName;
			OutputDebugString("\r\n");
			OutputDebugString(filename);
			//if (cfile.Open(filename, CFile::modeRead | CFile::typeBinary))
			//{
			//	const int	fileL = (int)cfile.GetLength();
			//	//char*	sndB = new char [fileL + 1];
			//	std::unique_ptr<char[]> sndB = std::make_unique<char[]>(fileL + 1);

			//	ZeroMemory(sndB.get(), fileL + 1);
			//	cfile.Read(sndB.get(), fileL);
			//	cfile.Close();

			//	//	m_elogfileName = filename;
			//	//	sendTR("pihoerpt", sndB.get(), fileL, US_PASS, 'r');
			//		//	delete [] sndB;
			//	return ;
			//}
		}
		if (!FindNextFile(hFind, &FindFileData))	break;
	}
}


void CTestDlgDlg::OnBnClickedBtnKey()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int  skey{}, ukey{}, index{};
	skey = 1;
	ukey = 2;
	index = 3;

	int	key{}, keyx{};

	key = MAKELONG(MAKEWORD(skey, ukey), index);

	int ival = HIWORD(key);
	 ival = LOBYTE(LOWORD(key));
	 ival = HIBYTE(LOWORD(key));

	char pdat[4] = "\t";
	 ival = 0xfe;
	memcpy(&pdat[0],&ival, 1);

	CString sdata, stmp, strResult{};
	sdata = "200311*10^200312*01^200409*0^200410*0^200424*0^200423*0^200301*F ^200301^200302* ^200302^200303* ^200303^200304*   ^200304^200305*   ^200305^200313*0^200313^200315*0  ^200315^200306^$207300*202000120                00000                                                                                1021&001&1301&1304&2023&2024&2033&2027&2101&2041&2025&2026&2061&2106&^200331*0000^200332*1^200901*001^c_upgb*001^c_upgb^200334*00^200333*1^200022^200307^200308^";
	/*
		case 0x09:
			stmp = "^";
			break;
		case 0x0a:
			stmp = "&";
			break;
		case 0x7f:
			stmp = "*";
			break;
		default:
			stmp.Format("%.1s", (char*)&(client->m_stream->m_sndB[ii]));
			break;
	*/
	for (int ii = 0; ii < sdata.GetLength(); ii++)
	{
		stmp = sdata.GetAt(ii);
		if (stmp == "^")
			stmp = "\t";
		else 	if (stmp == "&")
			stmp.Format("%c", 0x0A);
		else 	if (stmp == "*")
			stmp.Format("%c", 0x7f);
	
		strResult += stmp;
	}

	stmp.Format("%c", 0xfe);
	TRACE(stmp);

	strResult.Empty();

	unsigned char data[124]{};
	data[0] = 0x82;
	data[1] = 'a';
	data[2] = 'b';

	int len = strlen((char*)data);
	wstring wstr,wstr1;
	for (int ii = 0; ii < len; ii++)
	{
		wchar_t wch;
		// 하나의 멀티바이트 문자를 wide 문자로 변환
		int result = MultiByteToWideChar(CP_ACP, 0, (char*)&data[ii], 1, &wch, 1);
		if(result > 0)
			wstr1 += wch;
	}

	OutputDebugStringW(wstr1.c_str());

	for (int ii = 0; ii < 3; ii++)
	{
		stmp.Format("%c ", data[ii]);
		strResult += stmp;
	}
	
	TRACE(stmp);
}


void CTestDlgDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	CString stmp;
//stmp.Format("\r\n %d  %d ", point.x, point.y);
//OutputDebugString(stmp);
//
//DWORD	vals[5];
//int isize = sizeof(DWORD);
//isize = sizeof(vals[0]);
//	vals[0] = reinterpret_cast<DWORD>(this);


	//int	jjgaY = (int)((28 * (koga - m_curr)) / (koga - jega)/*(shga - hhga)*/);			//--

	CDialogEx::OnMouseMove(nFlags, point);
}


void GetWeekDay(CString strDate, CString& strResult)
{
	

	// 현재 년, 월, 일을 가져오기
	int year = atoi(strDate.Left(4));
	int month = atoi(strDate.Mid(4,2));
	int day = atoi(strDate.Right(2));

	COleDateTime currentDate;
	currentDate.SetDate(year, month, day);

	// 해당 월의 첫 번째 날 구하기
	COleDateTime firstDayOfMonth(year, month, 1, 0, 0, 0);

	// 첫 번째 날의 요일 (0 = 일요일, 1 = 월요일, ..., 6 = 토요일)
	int firstDayOfWeek = firstDayOfMonth.GetDayOfWeek() - 1; // 0부터 시작하도록 조정

	// 현재 날짜의 요일 (0 = 일요일, 1 = 월요일, ..., 6 = 토요일)
	int currentDayOfWeek = currentDate.GetDayOfWeek() - 1; // 0부터 시작하도록 조정

	// 첫 번째 날이 속한 주의 시작 요일 계산
	int offset = (7 - firstDayOfWeek) % 7;

	// 월 첫 번째 날로부터 지난 일수 계산
	int daysFromFirst = day + firstDayOfWeek;

	// 월중 몇 번째 주인지 계산 (첫 번째 주는 1주차)
	int weekOfMonth = (daysFromFirst + 6) / 7;

	if (firstDayOfWeek == 5 || firstDayOfWeek == 6)
		weekOfMonth -= 1;

	CString stmp;
	if (currentDayOfWeek == 1)
	{
		stmp.Format("%d", weekOfMonth);
		strResult = "M" + stmp;
	}
	else if (currentDayOfWeek == 3)
	{
		stmp.Format("%d", weekOfMonth);
		strResult = "W" + stmp;
	}
}

void CTestDlgDlg::OnBnClickedBtnKey2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*CString sdate;
	CTime time;
	time = CTime::GetCurrentTime();
	sdate.Format("%02d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());

	int year = _ttoi(sdate.Mid(0, 4));
	int month = _ttoi(sdate.Mid(4, 2));
	int day = _ttoi(sdate.Mid(6, 2));

	COleDateTime date(year, month, day, 0, 0, 0);
	int dayOfWeek = date.GetDayOfWeek() - 1;
	int num = date.GetDayOfYear();
	int weekNumber = (date.GetDayOfYear() + 6) / 7;*/


	CString strResult;
	GetWeekDay("20240612", strResult);
	TRACE(strResult);
}


void MyGlobalFunction(CString str)
{
	CString slog;
	slog.Format("%s", str);
	//AfxMessageBox(slog);
}

void MyGlobalFunction2(CString str, ...)
{
	CString slog;
	slog.Format("%s", str);
	//AfxMessageBox(slog);
}

void testfunptr2(void(*pval)(CString), CString str)
{
	//void (*globalFunctionPtr)() = &MyGlobalFunction;
	pval(str);
}

void testfuncptr(void(*pval)(CString), CString str)
{
	pval(str);
}

void CTestDlgDlg::OnBnClickedBtnLocal()
{
	
	void (*lFunctionPtr)(CString) = &MyGlobalFunction;
	testfuncptr(lFunctionPtr, _T("test"));
	//SendMessage(WM_SETCURSOR, nullptr, nullptr);
	

	m_hCursor  = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
	}
}
BOOL CTestDlgDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//if (m_hCursor)
	//{
	//	::SetCursor(m_hCursor);
	//	return TRUE;
	//}

//	return CTestDlgDlg::OnSetCursor(pWnd, nHitTest, message);
	return FALSE;
}

void CTestDlgDlg::OnBnClickedBtnBit()
{
// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//				bin				dec					hex
//				    1					1					0x1
//			      10					2					0x2
//			    100					4					0x4  
//			  1000					8					0x8  
//			10000					16					0x10  
//		  100000					32					0x20  
//		1000000					62					0x40
//   10000000					128				0x80

	int gubn_trd;

	unsigned char just_krx = 0x1;
	unsigned char just_atx = 0x2;
	unsigned char just_other = 0x4;
	unsigned char all_trd = just_krx;// | just_other | just_atx;

	if (all_trd & just_krx)
	{
		TRACE("???");
	}

	 if (all_trd & just_other)
	{
		TRACE("ok");
	}

	 if (all_trd & just_atx)
	 {
		 TRACE("ok");
	 }

	// cs_RW _cs;

	// char* pansi = "가나다123!abc";
	// CStringW str;
	// _cs.convert_ansi_to_unicode(str, pansi, strlen(pansi));

	// CStringW fname;
	//// fname = L"D:\\IBKS_TEST\\src\\APPL\\TestDlg\Debug\\ansi_uni.ini";
	// fname = L"c:\\uni.ini";
	// bool bret = WritePrivateProfileStringW(L"UNICODE", L"wstring", str, fname);


	// CString anfname;
	// //anfname = "D:\\IBKS_TEST\\src\\APPL\\TestDlg\Debug\\ansi_uni.ini";
	// anfname = "c:\\uni.ini";
	// bret = WritePrivateProfileString("ANSI","ANSI", "test", anfname);

	// char buff[256]{};
	// GetPrivateProfileString("UNICODE", "wstring", "", buff, sizeof(buff) - 1, anfname);

	// CString anstr;
	// anstr.Format("%s", buff);

}



//#include <Python.h>




//#include "D:\\IBKS_TEST\\src\\libtensorflow-cpu-windows-x86_64-2.6.0\\include\\tensorflow\\c\\c_api.h"

BOOL CTestDlgDlg::CheckMutextNotUsing()
{
	HANDLE handle;
	handle = CreateMutex(nullptr, FALSE, _T("Global\\MyMutexName"));
	if (handle)
	{
		DWORD iErrnum = GetLastError();
		if (iErrnum == ERROR_ALREADY_EXISTS)
			return FALSE;
		else
		{
			g_hMutex = handle;
			return TRUE;
		}
	}
	return FALSE;
}

void CTestDlgDlg::OnBnClickedBtnMutexwrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	ReleaseMutex(g_hMutex);
	CloseHandle(g_hMutex);
	g_hMutex = nullptr;
}


void CTestDlgDlg::OnBnClickedBtnMutexwrite2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//	HANDLE htest = OpenMutex(SYNCHRONIZE, true, "Global\\MyMutexName");
		if (CheckMutextNotUsing())
		{
			//if (CreateMutex(nullptr, FALSE, _T("Global\\MyMutexName")))
			{
				CString anfname;
				anfname = "c:\\uni.ini";
				bool bret = WritePrivateProfileString("ANSI", "ANSI", "test", anfname);
				//CloseHandle(g_hMutex);
			}
		}
}


void CTestDlgDlg::OnBnClickedBtnMutexwrite3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CheckMutextNotUsing())
	{
		//if (CreateMutex(nullptr, FALSE, _T("Global\\MyMutexName")))
		{
			CString anfname;
			anfname = "c:\\uni.ini";
			bool bret = WritePrivateProfileString("ANSI", "ANSI", "test", anfname);
			CloseHandle(g_hMutex);
		}
	}
}


#define DF_LIMIT 5
UINT th_WriteFile(LPVOID lparam)
{
	CTestDlgDlg* pWnd = (CTestDlgDlg*)lparam;
	CString _tlog;

	_tlog.Format(_T("\r\n%lu "), GetCurrentThreadId());
	OutputDebugString(_tlog);
	int _tval = 0;
	while (1)
	{
		Sleep(1000);
		pWnd->m_cs.Lock();
		if (!pWnd->m_bUseing)
		{
			pWnd->m_cs.Unlock();
			pWnd->StartNextThread();
			break;
		}
		_tlog.Format(_T("\r\n--->thread=[%lu]  val=[%d]  que=[%d]<---"), GetCurrentThreadId(), _tval , pWnd->m_thQueue.size());
		OutputDebugString(_tlog);
		_tval++;
		if (_tval > DF_LIMIT)
		{
			pWnd->m_bUseing = FALSE;
			pWnd->m_cs.Unlock();
			pWnd->StartNextThread();
			break;
		}
		pWnd->m_cs.Unlock();
	}
	return 0;
}

void CTestDlgDlg::StartNextThread()
{
	if (!m_thQueue.empty())
	{
		void* nextParam = m_thQueue.front();
		m_thQueue.pop();
		uintptr_t hthread = _beginthreadex(0, 0, (_beginthreadex_proc_type)th_WriteFile, nextParam, 0, 0);

		m_bUseing = true;
	}
}

void CTestDlgDlg::OnBnClickedBtnMutexwrite5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_cs.Lock();
	m_bUseing = FALSE;
	m_cs.Unlock();
}

/*
* CString anfname;
				anfname = "c:\\uni.ini";
				bool bret = WritePrivateProfileString("ANSI", "ANSI", "test", anfname);
*/

#include "CWC_FileSync.h"

void CTestDlgDlg::OnBnClickedBtnMutexwrite4()   //위에놈
{
	
	CString anfname;
	anfname = "c:\\uni.ini";

	std::unique_ptr < CWC_FileSync> _csFileSync = std::make_unique< CWC_FileSync>();
	_csFileSync.get()->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_VSCROLL, CRect(0,0,0,0), this, 9988);
	_csFileSync->synWritePrivateProfileString("ANSI", "ANSI", "test", anfname);

	return;

	/*char* pdata = nullptr;
	m_slog.Format("%s", pdata);
	if (m_slog.IsEmpty())
		TRACE("test");

	if (CheckThreadUsing())
		return;

	uintptr_t hthread = _beginthreadex(0, 0, (_beginthreadex_proc_type)th_WriteFile, (void*)this, 0, 0);*/
}

void CTestDlgDlg::OnBnClickedBtnMutexwrite6()
{
	if (CheckThreadUsing())
		return;

	uintptr_t hthread = _beginthreadex(0, 0, (_beginthreadex_proc_type)th_WriteFile, (void*)this, 0, 0);
}

BOOL CTestDlgDlg::CheckThreadUsing()
{
	m_cs.Lock();
	if (m_bUseing)
	{
		m_thQueue.push((void*)this);
		m_cs.Unlock();
		return TRUE;
	}

	m_bUseing = TRUE;
	m_cs.Unlock();
	return FALSE;
}


DWORD convert_ansi_to_unicode(CStringW& swUnicode, const char* ansi, const int isize)
{
	if(ansi == nullptr)
		return ERROR_INVALID_PARAMETER;

	CStringW str1;
	swUnicode = CA2W(ansi, CP_ACP);
	return ERROR_SUCCESS;
}

DWORD convert_unicode_to_ansi(CString& sAnsi, const wchar_t* wchr, const int isize)
{
	if (wchr == nullptr)
		return ERROR_INVALID_PARAMETER;

	USES_CONVERSION;
	sAnsi = W2A(wchr);

	return ERROR_SUCCESS;
}

int  wGetAccHistoryFile(CString strSec, CString strKey, char* pbuf, int len, CString strFile)
{
	CString sName;
	strFile.Replace("1934870343.ini", _T("AccHistory.ini"));

	int ret{};
	if (PathFileExists(strFile))
	{
		CStringW wstrSec, wstrKey, wstrVal, wstrFile;
		convert_ansi_to_unicode(wstrSec, (LPCSTR)(LPCTSTR)strSec, strSec.GetLength());
		convert_ansi_to_unicode(wstrKey, (LPCSTR)(LPCTSTR)strKey, strKey.GetLength());
		convert_ansi_to_unicode(wstrFile, (LPCSTR)(LPCTSTR)strFile, strFile.GetLength());

		WCHAR pdata[10 * 1024]{};

		GetPrivateProfileStringW(wstrSec, wstrKey, L"", pdata, sizeof(pdata), wstrFile);

		CString stmp;
		convert_unicode_to_ansi(stmp, pdata, sizeof(pdata) / sizeof(WCHAR));

		strncpy(pbuf, stmp.GetString(), len);
		ret = stmp.GetLength();
	}
	else
		ret = GetPrivateProfileString(strSec, strKey, _T(""), (char*)pbuf, len, strFile);

	return ret;
}

void CTestDlgDlg::OnBnClickedBtnLocal2()   //파일 읽기
{//F:\\util\\HTS\\IBK투자증권 HTS\\user\\1934870343.ini
	CString Path;
	char	readb[10 * 1024]{};
	CString strKey = "AN0110";
	Path = "F:\\util\\HTS\\IBK투자증권 HTS\\user\\1934870343\\1934870343.ini";

	int ret = wGetAccHistoryFile(_T("AccountHistory"), strKey,  readb, sizeof(readb), Path);
}


int  wWriteAccHistoryFile(CString strSec, CString strKey, CString strVal, CString strFile)
{
	CString sName;
	sName = "1934870343.ini";
	strFile.Replace(sName, _T("AccHistory.ini"));

	CStringW wstrSec, wstrKey, wstrVal, wstrFile;
	convert_ansi_to_unicode(wstrSec, (LPCSTR)(LPCTSTR)strSec, strSec.GetLength());
	convert_ansi_to_unicode(wstrKey, (LPCSTR)(LPCTSTR)strKey, strKey.GetLength());
	convert_ansi_to_unicode(wstrVal, (LPCSTR)(LPCTSTR)strVal, strVal.GetLength());
	convert_ansi_to_unicode(wstrFile, (LPCSTR)(LPCTSTR)strFile, strFile.GetLength());

	WritePrivateProfileStringW(wstrSec, wstrKey, wstrVal, wstrFile);
	return 0;
}

void CTestDlgDlg::OnBnClickedBtnLocal3()  //파일쓰기
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CString Path;
	//CString strKey = "AN00";


	//char	readb[10 * 1024]{};
	//Path = "F:\\util\\HTS\\IBK투자증권 HTS\\user\\1934870343\\1934870343.ini";
	//int len = GetPrivateProfileString("AccountHistory", strKey, _T(""), (char*)readb, sizeof(readb), Path);
	//CString stmp;
	//stmp.Format("%s", readb);
	//stmp.TrimRight();
	//wWriteAccHistoryFile("AccountHistory", strKey, stmp,  Path);
	
	
	//ref vector ex
	std::vector <int> _vec{};
	_vec.reserve(1000);
	int isum{};

	for (int i = 0; i < 1000; ++i)
	{
		_vec.push_back(GetTickCount64());
	}

	for_each(_vec.begin(), _vec.end(), [&](auto item) {
		if (item % 2 == 0)
			isum++;
		});

	CString str;
	str.Format("%d", isum);


	return;





	CString pszCommand;
	pszCommand = "powershell.exe -NoProfile -Command \"Start-Process 'C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe' --app='https://www.google.com'\"";
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// CreateProcess 함수 호출
	if (CreateProcess(NULL,   // No module name (use command line)
		(LPSTR)(LPCSTR)pszCommand,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		// 프로세스가 종료될 때까지 기다림
		WaitForSingleObject(pi.hProcess, INFINITE);

		// 프로세스와 스레드 핸들을 닫음
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		// 오류 처리
		AfxMessageBox("fail");
	}
}

#include "CDlg_note.h"
void CTestDlgDlg::OnBnClickedBtnLocal4()
{
	CDlg_note dlg;
	dlg.DoModal();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


UINT AFXAPI HashDataAXIS(LPCSTR key)
{
	UINT nHash = 0;
	while (*key)
		nHash = (nHash << 5) + nHash + *key++;
	return nHash;
}

bool IsNumber(CString str)
{
	if (str.GetAt(0) < 0 || str.GetAt(0) > 127)
		return false;

	BOOL bRet = TRUE;
	const int nLen = str.GetLength();
	for (int i = 0; i < nLen; ++i)
	{
		if (isdigit(str.GetAt(i)) == FALSE)
		{
			bRet = FALSE;
			break;
		}
	}
	return bRet;
}

void CTestDlgDlg::OnBnClickedBtnHen()
{
	int ival = sizeof(int);
	 ival = sizeof(UINT);

	CString strResult;
	CString sId;
	sId = "##ibk";

	bool bret = IsNumber(sId);

	strResult.Format("%08u", HashDataAXIS((LPCSTR)sId));
	OutputDebugString(strResult);

	CString str, title, stmp;
	stmp.Format("901\t%s\t902\t%s\t904\t%s\t996\t%s\t988\t%s\t923\t%s\t906\t%s\t922\t%s",
		"00110012107", "rhkd62", "29", "11", "체결", "14420233",
		"테스트계좌", "3");
	str += stmp;
	stmp.Format("\t905\t %s\t997\t %s\t912\t %s\t907\t %s\t908\t %s\t992\t %s\t931\t %s\t916\t%s",
		"0", "HTS", "매도", "A005930", "삼성전자", "453", "1", "72100", "1");
	str += stmp;
	stmp.Format("\t909\t%s\t910\t%s\t921\t%s\t975\t%s\t924\t%s\t925\t%s\t926\t%s\t993\t%s",
		"0", "0", "0", "- 현금매도", "시장가", "KSE", "00", "00");
	str += stmp;
	stmp.Format("\t999\t%s\t974\t%s\t984\t%s",
		"0", "0", "10	023 - 55500");
	str += stmp;

	OutputDebugString(str);
}


void CTestDlgDlg::OnBnClickedBtnEdgepop()
{
	CMapStringToString map;
	map.SetAt("123", "");
	CString val;
	if (map.Lookup("123", val))
		TRACE("234");

	map.RemoveKey("123");

	if (map.Lookup("123", val))
		TRACE("234");
	else
		TRACE("234");

	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	HWND edge_handle = nullptr;
	CWnd* cWnd = nullptr;
	CString strParam;
	SHELLEXECUTEINFO sinfo = { 0 };

	CString url;
	url = "http://google.com";
	strParam.Format("--app=\"%s\"", url);

	sinfo.cbSize = sizeof(SHELLEXECUTEINFO);
	sinfo.lpFile = "msedge.exe";
	sinfo.lpParameters = strParam;
	sinfo.nShow = SW_SHOW;
	sinfo.fMask = SEE_MASK_NOCLOSEPROCESS;

	if (ShellExecuteEx(&sinfo))
	{
		::WaitForInputIdle(sinfo.hProcess, INFINITE);
		cWnd = FindWindow("Chrome_WidgetWin_1", nullptr);

		edge_handle = cWnd->GetSafeHwnd();
		::MoveWindow(edge_handle, 0, 0, 200, 300, TRUE);
		return ;
	}

	return ;
}

void CTestDlgDlg::OnBnClickedBtnEnc()
{
	CString rtnEnc, enc;
	CString path;

	enc = "최적화 문제: 알고리즘의 효율성을 높이기 위해 다양한 최적화 문제를 풀 때 미분을 사용합니다. 예를 들어, 머신러닝 모델의 학습 과정에서 손실 함수를 미분하여 최적의 파라미터를 찾습니다. \
		이 외에도 미분은 다양한 분야에서 많은 실질적인 응용 사례가 있습니다.기초적인 개념을 이해하고 있으면, 복잡한 문제를 해결하는 데 도움을 줄 수 있습니다.";

	enc = "알고리즘의 효율성을 높이기 위해 다양한 최적화 문제를 풀 때 미분을 사용합니다 ";

		path.Format("C:\IBKS\\IBK투자증권 HTS\\dev\\%s", "CX_AES.DLL");;
	HMODULE hModule = LoadLibrary(path);
	char* data = (LPSTR)(LPCTSTR)enc;
	char* aes{};
	char* pdata = new char[1024];
	memset(pdata, 0x00, 1024);
	CString s;
	if (hModule)
	{
		typedef long (WINAPI* GETSHAFUNC)(char*, int);
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(hModule, "axEncrypt");
		aes = (char*)func(data, 6);
	
		memcpy(pdata, aes, strlen(aes));
		if (func)
			m_slog.Format("%s", pdata);
	}


	char* dedata = (LPSTR)(LPCTSTR)m_slog;

	char* deaes{};

	if (hModule)
	{
		typedef long (WINAPI* GETSHAFUNC)(char*);

		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(hModule, "axDecrypt");

		//	OutputDebugString("FUNCTION CALL\n");

		if (func)
		{
			deaes = (char*)func(dedata);

			rtnEnc.Format("%s", deaes);

			s.Format("AES SRC [%s] DEC [%s]\n", dedata, deaes);
			//	OutputDebugString(s);
		}
	}

	if (hModule)
		FreeLibrary(hModule);
}





//#include <sstream>
//#include <iomanip>

// 헥사 인코딩 함수
std::string ToHex(const std::string input) {
	std::ostringstream oss;
	for (unsigned char c : input) {
		oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
	}
	return oss.str();
}

// 헥사 디코딩 함수
std::string FromHex(const std::string& input) {
	std::string output;
	for (size_t i = 0; i < input.length(); i += 2) {
		std::string byte = input.substr(i, 2);
		char chr = static_cast<char>(std::stoi(byte, nullptr, 16));
		output.push_back(chr);
	}
	return output;
}

void CTestDlgDlg::OnBnClickedBtnDec()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString spath{};
	spath = "F:\\util\\HTS\\IBK투자증권 HTS\\user\\1934870343\\Crashlog\\Hlog.ini";

	CFile file;
	//CString string = AxStd::FORMAT("%s", "hjcode3.dat");
	if (AxStd::isFileExist(spath))
	{
		if (!file.Open(spath, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;

		const int size = gsl::narrow_cast<int>(file.GetLength());
		m_hcB = std::make_unique<char[]>(size + 1);
		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		m_slog.Format("%s", m_hcB.get());
		m_slog.Replace("[", "5b");
		m_slog.Replace("]", "5d");

		m_slog.Replace("\r", "0d");
		m_slog.Replace("\n", "0a");
		m_slog.Replace("=", "3d");

		std::string decoding((m_slog));
		std::string decodedString = FromHex(decoding);
		CString decodedCString(decodedString.c_str());  // std::string을 CString으로 변환

		spath = "F:\\util\\HTS\\IBK투자증권 HTS\\user\\1934870343\\Crashlog\\Hlog_dec.ini";

		FILE* fp;
		fopen_s(&fp, spath, "a+");
		if (!fp)
			return;

		const char* cstr = (LPCTSTR)decodedCString;  // CString을 C 스타일 문자열로 변환

	   // 파일에 CString 값 쓰기
		fprintf(fp, "%s\n", cstr);

		fclose(fp);
		
	}
	
}

void CTestDlgDlg::OnBnClickedBtnHexenc()
{
	USES_CONVERSION;
	CString str = _T("]");
	std::string inputString((str));  // CString을 std::string으로 변환

	// 헥사 인코딩
	std::string hexEncoded = ToHex(inputString);
	CString hexCString(hexEncoded.c_str());  // std::string을 CString으로 변환

	AfxMessageBox(hexCString);

	// 헥사 디코딩
	std::string decodedString = FromHex(hexEncoded);
	CString decodedCString(decodedString.c_str());  // std::string을 CString으로 변환
	AfxMessageBox(decodedCString);
}


void CTestDlgDlg::OnBnClickedBtnHexdec()
{
	CString file{};
	file = "F:\\util\\HTS\\IBK투자증권 HTS\\user\\1934870343\\Hlog.ini";
	char	ssb[1024 * 4];
	const DWORD ssL = GetPrivateProfileSection("sConf", ssb, sizeof(ssb), file);

	CString subitem, keys, value, string = CString(ssb, ssL);
	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\0');
		if (idx == -1)	break;

		subitem = string.Left(idx++);
		string = string.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	
			continue;

		keys = subitem.Left(idx++);
		value = subitem.Mid(idx);

		std::string stkey((keys));
		keys = FromHex(stkey).c_str();

		std::string svalue((value));
		value = FromHex(svalue).c_str();
	}
}


void CTestDlgDlg::OnBnClickedBtnShared2()
{
	char	buf[1024];
	GetWindowsDirectory(buf, sizeof(buf));
	CFileDialog	dlg(TRUE, "exe", NULL, OFN_OVERWRITEPROMPT,
		"모든파일(*.*)|*.*||", NULL);
	if (dlg.DoModal() != IDOK)	return;

	CString path;
	path = dlg.GetPathName();
	
	m_hSharedLib = LoadLibrary(path);

	CWnd* (APIENTRY * axCreate)(CWnd*, void*);
	axCreate = (CWnd*(APIENTRY*)(CWnd*, void*))GetProcAddress(m_hSharedLib, "axCreate");


	char* pchar = new char[100];
	memset(pchar, 0x00, 100);
	memcpy(pchar, "1234", 4);
	m_pSharedMemory = axCreate(this, (LPVOID)pchar);

	m_pSharedMemory->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(0x01, 1), 1), (LPARAM)this);
}


void CTestDlgDlg::OnBnClickedBtnSharedWrite2()
{
	char* pchar = new char[100];
	memset(pchar, 0x00, 100);
	memcpy(pchar, "breadcasting", 12);
	m_pSharedMemory->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(0x02, 1), 12), (LPARAM)pchar);

	CSharedFile sbuffer;
}

#include "Clbox.h"

void CTestDlgDlg::OnBnClickedBtnListbox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CPoint pt{};
	Clbox* pbox{};
	pbox = new Clbox();
	GetCursorPos(&pt);
	CRect wRc(0, 0, 100, 300);

	// 클라이언트 좌표를 스크린 좌표로 변환
	ClientToScreen(&wRc);

	CRect rec;
	rec.SetRect(pt.x, pt.y, pt.x + wRc.Width(), pt.y + wRc.Height());

	// CListBox 생성
	if (!pbox->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, // Extended window styles
		nullptr, // No window class name
		NULL,    // No window name
		WS_POPUP | WS_BORDER | WS_VISIBLE | LBS_OWNERDRAWFIXED| LBS_HASSTRINGS, // Window styles
		rec,     // Rectangle for the window
		this,    // Parent window
		NULL,    // No menu
		NULL))   // No additional parameters
	{
		delete pbox;
		return;
	}
	pbox->AddString("test");
}

#include "History.h"
#include "CLBparent.h"
void CTestDlgDlg::OnBnClickedBtnListbox2()
{
	// 화면 해상도
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 화면 DPI
	HDC hdc = ::GetDC(m_hWnd);
	int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
	int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
	ReleaseDC(CDC::FromHandle(hdc));

	// 실제 크기 계산 (인치 단위)
	double widthInInches = static_cast<double>(screenWidth) / dpiX;
	double heightInInches = static_cast<double>(screenHeight) / dpiY;



	CPoint pt{};
	Clbox* pbox{};
	pbox = new Clbox();
	GetCursorPos(&pt);
	CRect wRc(0, 0, 240, 200);

	// 클라이언트 좌표를 스크린 좌표로 변환
	ClientToScreen(&wRc);

	CRect rec;
	rec.SetRect(pt.x, pt.y, pt.x + wRc.Width(), pt.y + wRc.Height());

	if (1)
	{
		CLBparent* pPrtWnd{};
		pPrtWnd = new CLBparent;


		BOOL bret = pPrtWnd->CreatePopUpWindow(this, rec);
		CRect pwrec;
		pPrtWnd->GetWindowRect(pwrec);
		pPrtWnd->MoveWindow(pwrec);
		if (bret)
		{
			pPrtWnd->CreateListBox("000070  삼양홀딩스	024110  기업은행	047040  대우건설	071970  HD현대마린엔진	353590  오토앤	009520  포스코엠텍	005490  POSCO홀딩스	039130  하나투어	030960  양지사	042700  한미반도체	000087  하이트진로2우B	330590  롯데리츠	032830  삼성생명	256840  한국비엔씨	011690  와이투솔루션	310210  보로노이	065650  하이퍼코퍼레이션	307750  국전약품	226360  KH 건설	005360  모나미	270870  뉴트리	035720  카카오	000020  동화약품");
			pPrtWnd->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		CHistory* m_child = new CHistory(nullptr, "005930\t삼성전자");
		if (!((CHistory*)m_child)->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, // Extended window styles
		nullptr, // No window class name
		NULL,    // No window name
		WS_POPUP | WS_BORDER | WS_VISIBLE | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_NOTIFY, // Window styles
		rec,     // Rectangle for the window
		this,    // Parent window
		NULL,    // No menu
			nullptr))   // No additional parameters
		{
			delete pbox;
			return;
		}
		m_child->SetOwner(this);
	}
}


#define byte BYTE
#include <windows.h>         // 반드시 먼저 포함
#include <Wbemidl.h>         // WMI 관련 헤더 파일
#pragma comment(lib, "wbemuuid.lib")

void GetMonitorInfo()
{
	HRESULT hres;

	// Step 1: Initialize COM.
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		std::cout << "Failed to initialize COM library." << std::endl;
		return;
	}

	// Step 2: Set general COM security levels.
	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities
		NULL                         // Reserved
	);

	if (FAILED(hres))
	{
		std::cout << "Failed to initialize security." << std::endl;
		CoUninitialize();
		return;
	}

	// Step 3: Obtain the initial locator to WMI.
	IWbemLocator* pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID*)&pLoc);

	if (FAILED(hres))
	{
		std::cout << "Failed to create IWbemLocator object." << std::endl;
		CoUninitialize();
		return;
	}

	// Step 4: Connect to WMI through the IWbemLocator::ConnectServer method.
	IWbemServices* pSvc = NULL;

	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\WMI"), // Namespace
		NULL,                  // User name
		NULL,                  // User password
		0,                     // Locale
		NULL,                  // Security flags
		0,                     // Authority
		0,                     // Context object
		&pSvc                  // IWbemServices proxy
	);

	if (FAILED(hres))
	{
		std::cout << "Could not connect." << std::endl;
		pLoc->Release();
		CoUninitialize();
		return;
	}

	// Step 5: Set security levels on the proxy.
	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // Client identity
		EOAC_NONE                    // Proxy capabilities
	);

	if (FAILED(hres))
	{
		std::cout << "Could not set proxy blanket." << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return;
	}

	// Step 6: Use the IWbemServices pointer to make requests of WMI.
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM WmiMonitorDescriptorMethods"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		std::cout << "WMI Query Failed." << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return;
	}

	// Step 7: Retrieve the data from the query.
	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		// Retrieve the EDID data
		hr = pclsObj->Get(L"InstanceName", 0, &vtProp, 0, 0);
		if (SUCCEEDED(hr))
		{
			// Display the instance name for debugging (optional)
			std::wcout << "InstanceName: " << vtProp.bstrVal << std::endl;
		}

		hr = pclsObj->Get(L"DisplayWidth", 0, &vtProp, 0, 0);
		double displayWidthMM = vtProp.uintVal; // Value in millimeters
		VariantClear(&vtProp);

		hr = pclsObj->Get(L"DisplayHeight", 0, &vtProp, 0, 0);
		double displayHeightMM = vtProp.uintVal; // Value in millimeters
		VariantClear(&vtProp);

		// Convert width and height from millimeters to inches
		double displayWidthInches = displayWidthMM / 25.4;
		double displayHeightInches = displayHeightMM / 25.4;

		// Calculate diagonal size in inches
		double diagonalSizeInInches = sqrt(displayWidthInches * displayWidthInches + displayHeightInches * displayHeightInches);
		std::cout << "Monitor Size: " << diagonalSizeInInches << " inches (diagonal)" << std::endl;

		pclsObj->Release();
	}

	// Step 8: Cleanup
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();
}

void CTestDlgDlg::OnBnClickedBtnMonitor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetMonitorInfo();
}


#include "CDlg_IMG.h"
void CTestDlgDlg::OnBnClickedBtnMonitor2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.




	CDlg_IMG dlg;
	dlg.DoModal();
}


void CTestDlgDlg::OnBnClickedBtnWrite32()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*CString stmp{}, sCode, sName, sFilepath;
	sFilepath = "F:\\util\\HTS\\IBK투자증권 HTS\\tab\\axPOPMenu.INI";*/
}

#include <imm.h>
void CTestDlgDlg::CheckIME()
{
	DWORD dwConversion = 0, dwSentence = 0;

	HIMC hImc = ImmGetContext(((CWnd*)GetDlgItem(IDC_EDIT_SHARED))->m_hWnd);
	if (hImc) // hImc가 유효한지 확인
	{
		if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
		{
			if (!(dwConversion & IME_CMODE_HANGEUL)) // 한글 모드가 아닌 경우
			{
				((CWnd*)GetDlgItem(IDC_EDIT_DATA))->SetWindowText("CheckIME한글모드 아니다");
				OutputDebugString("\r\n[IMM]CheckIME------- 아니다 한글모드 ");
			}
			else
			{
				((CWnd*)GetDlgItem(IDC_EDIT_DATA))->SetWindowText("CheckIME 한글모드");
				OutputDebugString("\r\n[IMM]CheckIME--------   한글모드");
			}
		}
		ImmReleaseContext(this->m_hWnd, hImc); // IME 컨텍스트 해제
	}
}

void CTestDlgDlg::OnBnClickedBtnMonitor3()
{
	if (1)
	{
		OutputDebugString("\r\n[IMM]OnBnClick");
		((CEdit*)GetDlgItem(IDC_EDIT_SHARED))->SetFocus();
		INPUT input = { 0 };
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = VK_HANGUL;  // 한영 전환 키
		SendInput(1, &input, sizeof(INPUT));

		
		//((CEdit*)GetDlgItem(IDC_EDIT_SHARED))->SetFocus();
		//((CEdit*)GetDlgItem(IDC_EDIT_SHARED))->PostMessage(WM_KEYDOWN, VK_HANGUL, 0);
		//((CWnd*)GetDlgItem(IDC_EDIT_DATA))->PostMessage( WM_KEYDOWN, 229, 0); WM_KEYDOWN
		
		//PostMessage(WM_IME_NOTIFY, IMN_SETCONVERSIONMODE, 0);
		return;
	}

	((CWnd*)GetDlgItem(IDC_EDIT_DATA))->SetWindowText("");
	DWORD dwConversion = 0, dwSentence = 0;
	((CEdit*)GetDlgItem(IDC_EDIT_SHARED))->SetFocus();
	HIMC hImc = ImmGetContext(((CWnd*)GetDlgItem(IDC_EDIT_SHARED))->m_hWnd);
	if (hImc) // hImc가 유효한지 확인
	{
		if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
		{
			if (!(dwConversion & IME_CMODE_HANGEUL)) // 한글 모드가 아닌 경우
			{
				((CWnd*)GetDlgItem(IDC_EDIT_DATA))->SetWindowText("아니다 한글모드그래서 한글로 바꾼다");
				OutputDebugString("\r\n[IMM]OnBnClickedBtnMonitor3------- 아니다 한글모드그래서 한글로 바꾼다 ");
				if (1)
				{
					dwConversion |= IME_CMODE_HANGEUL; // 한글 모드로 변경

					bool bret = ImmSetConversionStatus(hImc, dwConversion, dwSentence);
					m_slog.Format("[IMM] ImmSetConversionStatus ret =[%d]", bret);
					OutputDebugString(m_slog);
					//((CWnd*)GetDlgItem(IDC_EDIT_SHARED))->SendMessage(WM_IME_NOTIFY, IMN_SETCONVERSIONMODE, 0);
					SendMessage(WM_IME_NOTIFY, IMN_SETCONVERSIONMODE, 0);
				}
				else
				{
						dwConversion = IME_CMODE_ALPHANUMERIC;
						ImmSetConversionStatus(hImc, dwConversion, dwSentence);
				}
			
			}
			else
			{
				((CWnd*)GetDlgItem(IDC_EDIT_DATA))->SetWindowText("한글모드");
				OutputDebugString("\r\n[IMM]OnBnClickedBtnMonitor3-------  한글모드");
			}
		}

		ImmReleaseContext(this->m_hWnd, hImc); // IME 컨텍스트 해제
	}
}

void CTestDlgDlg::OnSetfocusEditShared()
{  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!에디트에 포커스가 왔다!!!!!!!!!!!!!!!!!!!!!!!
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnBnClickedBtnMonitor3();
	//SetTimer(TID_CHANGEHANGLE, 10, NULL);
	m_slog.Format("\r\n[IMM]<%d>OnSetfocusEditShared", __LINE__);
	OutputDebugString(m_slog);
	//((CEdit*)GetDlgItem(IDC_EDIT_SHARED))->SetSel(0, 1);
	
}

void CTestDlgDlg::OnChangeEditShared()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	OutputDebugString("\r\n[IMM]------- OnChangeEditShared");
	//CheckIME();
	OnBnClickedBtnMonitor3();
}


void CTestDlgDlg::OnUpdateEditShared()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OutputDebugString("\r\n[IMM]------- OnUpdateEditShared");
	
}


void CTestDlgDlg::OnBnClickedBtnChkime()
{
	CheckIME();
}


void CTestDlgDlg::OnThemechangedEditShared(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 이 기능을 사용하려면 Windows XP 이상이 필요합니다.
	// _WIN32_WINNT 기호는 0x0501보다 크거나 같아야 합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CString str;
}
struct	_axisH {
	unsigned char	msgK;		// kind of msg
	unsigned char	stat;		// state
	unsigned char	auxs;		// aux state

	unsigned char	winK;		// kind of window
	unsigned char	unit;		// unit window
	unsigned char	trxK;		// TR key code (INB)
	unsigned char	trxS;		// TR key state (OUB)

	char	svcN[4];		// service name
	char	trxC[8];		// TR code
	char	datL[5];		// following data length
}AXISH;

#define	auxsMAP		0x01		// request MAP change								 0000 0001
#define	auxsFDS		0x02		// include fds data (for IBK 20151221)         0000 0010
#define	auxsCA		0x08		// CA data														 0000 1000					
#define	auxsOOP		0x10		// symbol data interface                               0001 0000
#define	auxsCLOSE	0x20 		// window closed (only valid axisH.winK)   0010 0000
#define mask_ALL_MARKET  0x80  // 1000 0000  //NXT 거래소구분자 신규추가   //mod ntx
#define mask_NXT_MARKET  0x40  // 0100 0000
#define mask_KRX_MARKET  0x00  // 0000 0000

void checkaut(struct	_axisH*  axis)
{
	CString strmarket;
	if (axis->auxs & mask_ALL_MARKET)
	{
		strmarket = "[ALL_MARKET]";
	}
	else if (axis->auxs & mask_NXT_MARKET)
	{
		strmarket = "[NXT_MARKET]";
	}
	else if (axis->auxs & mask_KRX_MARKET)
	{
		strmarket = "[KRX_MARKET]";
	}
	else
	{
		printf("Unknown market type.\n");
	}
}

void checkautstatck(struct	_axisH& axis)
{
	CString strmarket;
	if (axis.auxs & mask_ALL_MARKET)
	{
		strmarket = "[ALL_MARKET]";
	}
	else if (axis.auxs & mask_NXT_MARKET)
	{
		strmarket = "[NXT_MARKET]";
	}
	else if (axis.auxs & mask_KRX_MARKET)
	{
		strmarket = "[KRX_MARKET]";
	}
	else
	{
		printf("Unknown market type.\n");
	}
}

void CTestDlgDlg::OnBnClickedBtnChkbit()
{


	struct _axisH* paxis =  new struct _axisH;
	memset(paxis, 0x00, sizeof(struct _axisH));
	paxis->auxs = auxsCA;
	paxis->auxs |= mask_ALL_MARKET;
	checkaut(paxis);

	_axisH axis{};
	axis.auxs = auxsCA;
	axis.auxs |= mask_NXT_MARKET;
	checkautstatck(axis);
}


void CTestDlgDlg::OnBnClickedBtnChkbit2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString path;
	path = "C:\\IBKS\\IBK투자증권 HTS\\tab\\sfcode.dat";
	CFile file;
	vector<struct sfcode> m_fjcode;
	if (file.Open(path, CFile::modeRead))
	{
		m_fjcode.resize(gsl::narrow_cast<const int>(file.GetLength() / sizeof(struct sfcode)));
		file.Read(&m_fjcode[0], m_fjcode.size() * sizeof(struct sfcode));
		file.Close();
	}
	int itotal = m_fjcode.size();
	for (size_t i = 0; i < m_fjcode.size(); i++)
	{
		CString str(m_fjcode[i].gnam, sizeof(m_fjcode[i].gnam));
		CString tmp;

		str.TrimLeft();
		str.TrimRight();

		CString scode(m_fjcode[i].codx, sizeof(m_fjcode[i].codx));


		scode.TrimLeft();
		scode.TrimRight();

		if ((scode[1] >= '1') && (scode[1] < '6'))
		{
			m_slog.Format("\r\n [1059]  [%s]  [%s]", scode, CString(m_fjcode[i].hnam, 50).TrimRight());
			OutputDebugString(m_slog);
		}
		else if (scode.Mid(1, 2) >= "10" && scode.Mid(1, 2) <= "59")
		{
			m_slog.Format("\r\n [1059]  [%s]  [%s]", scode, CString(m_fjcode[i].hnam,50).TrimRight());
			OutputDebugString(m_slog);
		}
		else	if (scode.Mid(1, 2) >= "B0" && scode.Mid(1, 2) <= "ZZ")
		{
			m_slog.Format("\r\n [B0ZZ]  [%s]  [%s]", scode, CString(m_fjcode[i].hnam, 50).TrimRight());
			//OutputDebugString(m_slog);
		}
		else	if (scode.Mid(1, 2) >= "0A" && scode.Mid(1, 2) <= "0Z")
		{
			m_slog.Format("\r\n [0A0Z]   [%s]  [%s]", scode, CString(m_fjcode[i].hnam, 50).TrimRight());
			//OutputDebugString(m_slog);
		}
		else if (scode.Mid(1, 2) >= "9A" && scode.Mid(1, 2) <= "9Z")
		{
			m_slog.Format("\r\n  [9A9Z]   [%s]  [%s]", scode, CString(m_fjcode[i].hnam, 50).TrimRight());
		//	OutputDebugString(m_slog);
		}
		else
		{
			m_slog.Format("\r\n  [[????]]   [%s]  [%s]", scode, CString(m_fjcode[i].hnam, 50).TrimRight());
			OutputDebugString(m_slog);
		}

	//	m_slog.Format("\r\n $$$$  [%s]  [%s]", scode, CString(m_fjcode[i].hnam, 50).TrimRight());
	//	OutputDebugString(m_slog);
	}

	CString strGubn;
	strGubn = "0Q";
	if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
		|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z")
		|| (strGubn >= "6A" && strGubn <= "6Z") || (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z"))
	{
		TRACE("123123");
	}
}


void CTestDlgDlg::OnBnClickedBtnChkbit3()
{
	CString tmpS;
	tmpS.Empty();
	//int key = 42175;//47286;
//	int key = 47286;

	int key = 42161;
	tmpS += HIBYTE(LOWORD(key));
	tmpS += LOBYTE(LOWORD(key));

	key = 42175;
	tmpS += HIBYTE(LOWORD(key));
	tmpS += LOBYTE(LOWORD(key));
}

void StartTimerWithLambda(UINT nInterval, std::function<void()> callback)
{
	
}
void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{

}

void CTestDlgDlg::OnBnClickedBtnLamdasettimer()
{

	SetTimer(1, 5000, [](HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
	{
			CWnd* pWnd = CWnd::FromHandle(hwnd);
			pWnd->SendMessage(WM_CLOSE, 0, 0);
			
	});
}
void CALLBACK TimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	TRACE("\r\n Timer Triggered");
}

void CTestDlgDlg::OnBnClickedBtnLamdasettimer2()
{
	SetTimer(1, 5000, TimerCallback);
}




CString Base64Encode(const BYTE* pData, DWORD dwDataLen)
{
	static const char* base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	CString base64String;
	int padding = 0;

	for (DWORD i = 0; i < dwDataLen; i += 3)
	{
		DWORD n = pData[i] << 16;

		if (i + 1 < dwDataLen)
		{
			n |= pData[i + 1] << 8;
		}
		else
		{
			padding++;
		}

		if (i + 2 < dwDataLen)
		{
			n |= pData[i + 2];
		}
		else
		{
			padding++;
		}

		base64String.AppendChar(base64Chars[(n >> 18) & 63]);
		base64String.AppendChar(base64Chars[(n >> 12) & 63]);

		if (padding < 2) base64String.AppendChar(base64Chars[(n >> 6) & 63]);
		else base64String.AppendChar('=');

		if (padding < 1) base64String.AppendChar(base64Chars[n & 63]);
		else base64String.AppendChar('=');
	}

	return base64String;
}

bool Base64Decode(const CString& base64String, BYTE** ppData, DWORD& outLen)
{
	static const int decodingTable[256] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
		-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
		-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	};

	// Base64 문자열 길이 확인
	int inputLen = base64String.GetLength();
	if (inputLen % 4 != 0) return false; // Base64는 항상 4의 배수 길이여야 함

	// 패딩 확인 ('=' 문자의 개수)
	int padding = 0;
	if (inputLen >= 2 && base64String[inputLen - 1] == '=')
		padding++;
	if (inputLen >= 3 && base64String[inputLen - 2] == '=')
		padding++;

	// 디코딩된 데이터 길이 계산
	outLen = (inputLen / 4) * 3 - padding;
	*ppData = new BYTE[outLen];

	int buffer = 0;
	int bufferBits = 0;
	int dataIndex = 0;

	for (int i = 0; i < inputLen; i++)
	{
		int decoded = decodingTable[(unsigned char)base64String[i]];
		if (decoded == -1) continue; // 유효하지 않은 Base64 문자 무시

		buffer = (buffer << 6) | decoded;
		bufferBits += 6;

		if (bufferBits >= 8)
		{
			bufferBits -= 8;
			(*ppData)[dataIndex++] = (BYTE)((buffer >> bufferBits) & 0xFF);
		}
	}

	return true;
}

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32")
#include <wincrypt.h>
void CTestDlgDlg::OnBnClickedAesEnc()
{
	HCRYPTPROV hCryptProv;
	HCRYPTHASH hHash;
	HCRYPTKEY hKey;

	if (CryptAcquireContext(&hCryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0) == FALSE) {
		m_slog.Format("1  %x", GetLastError());
		OutputDebugString(m_slog);
	
		if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_NEWKEYSET) == FALSE) {
			m_slog.Format("2 %x", GetLastError());
			OutputDebugString(m_slog);
			m_slog.Format("[HUB]   CryptAcquireContext error ");
			OutputDebugString(m_slog);
			AfxMessageBox(m_slog);
			return;
		}
	}

	const DWORD AES_KEY_LENGTH = 16;
	struct {
		BLOBHEADER hdr;
		DWORD cbKeySize;
		BYTE rgbKeyData[AES_KEY_LENGTH];
	} keyBlob;

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.reserved = 0;
	keyBlob.hdr.aiKeyAlg = CALG_AES_128;
	keyBlob.cbKeySize = AES_KEY_LENGTH;
	CopyMemory(keyBlob.rgbKeyData, "!O7#8aksjdf67h53", AES_KEY_LENGTH);

	if (CryptImportKey(hCryptProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey) == FALSE) {
		m_slog.Format("[HUB]   CryptImportKey error ");
		OutputDebugString(m_slog);
		AfxMessageBox(m_slog);
		return ;
	}

	DWORD dwMode = CRYPT_MODE_ECB;
	if (CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&dwMode, 0) == FALSE) {
		m_slog.Format("[HUB]   CryptSetKeyParam1 error ");
		OutputDebugString(m_slog);
		AfxMessageBox(m_slog);
		return;
	}

	dwMode = PKCS5_PADDING;
	if (CryptSetKeyParam(hKey, KP_PADDING, (BYTE*)&dwMode, 0) == FALSE) {
		m_slog.Format("[HUB]   CryptSetKeyParam2 error ");
		OutputDebugString(m_slog);
		AfxMessageBox(m_slog);
		return;
	}

	CString plainID;
	plainID.Format("%s", "1231");

	BYTE pData[128];
	DWORD dwStrLen = plainID.GetLength();
	DWORD dwPadding = 16 - (dwStrLen & 0xF);
	DWORD dwDataLen = dwStrLen;
	DWORD dwBufferLen = dwStrLen + dwPadding;

	memset(pData, 0, sizeof(pData));
	CopyMemory(pData, (LPSTR)(LPCTSTR)plainID, dwStrLen);

	BOOL result = CryptEncrypt(hKey, 0, TRUE, 0, pData, &dwDataLen, dwBufferLen);
	if (!result) {
		OutputDebugString(_T("CryptEncrypt 실패!"));
		return;
	}

	// Base64 인코딩
	CString sEncBase64 = Base64Encode((BYTE*)pData, dwBufferLen);
	CString debugMsg;
	debugMsg.Format(_T("\r\n[encrypt1] len =[%d]-----------------------------%s"), sEncBase64.GetLength(), sEncBase64);
	OutputDebugString(debugMsg);












	// Base64 디코딩 및 복호화
	BYTE* pDecodedData = nullptr;
	DWORD decodedLen = 0;

	bool bDecodeSuccess = Base64Decode(sEncBase64, &pDecodedData, decodedLen);
	if (!bDecodeSuccess) {
		OutputDebugString(_T("Base64 디코딩 실패!"));
		return;
	}

	// CryptDecrypt를 위한 준비
	DWORD outLen = decodedLen;
	result = CryptDecrypt(hKey, 0, TRUE, 0, pDecodedData, &outLen);
	if (!result) {
		OutputDebugString(_T("CryptDecrypt 실패!"));
		delete[] pDecodedData;
		return;
	}

	// 복호화된 데이터 출력
	pDecodedData[outLen] = '\0'; // Null-terminate
	CString decryptedText((char*)pDecodedData);
	debugMsg.Format(_T("\r\n[decrypt]-----------------------------%s"), decryptedText);
	OutputDebugString(debugMsg);

	delete[] pDecodedData;





















	CString rtnEnc;
	CString path;

	path.Format("C:\\IBKS\\IBK투자증권 HTS\\dev\\%s",  "CX_AES.DLL");
	HMODULE hModule = LoadLibrary(path);

	CString enc;
	enc = "khs779";
	char* data = (LPSTR)(LPCTSTR)enc;
	char* aes{};

	CString s;
	if (hModule)
	{
		typedef long (WINAPI* GETSHAFUNC)(char*, int);
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(hModule, "axEncrypt");
		aes = (char*)func(data, 6);
		char* pdata = new char[strlen(aes) + 1];
		memset(pdata, 0x00, strlen(aes) + 1);
		memcpy(pdata, aes, strlen(aes));
		if (func)
			rtnEnc.Format("%s", pdata);

		
		OutputDebugString(plainID);

		typedef long (WINAPI* GETAESFUNC)(char*);
		GETAESFUNC func1 = (GETAESFUNC)GetProcAddress(hModule, "axDecrypt");
		char* pdec = (char*)func1(pdata);
		plainID.Format("\r\n[encrypt3]-----------------------------%s", pdec);
		OutputDebugString(plainID);
	}

	if (hModule)
		FreeLibrary(hModule);

	hModule = NULL;






#include <afx.h>
#include <ctime>

	//bool CheckLogTimestamp(const CString & logFilePath)
	//{
		
	CString logFilePath;
	logFilePath = "F:\\util\\HTS\\IBK투자증권 HTS\\exe\\axMisc.log";
		// 1. log.ini 파일 열기
		CStdioFile file;
		if (!file.Open(logFilePath, CFile::modeRead | CFile::typeText)) {
			AfxMessageBox(_T("log.ini 파일을 열 수 없습니다."));
			return ;
		}

		CString line;
		// 2. 첫 번째 라인 읽기
		if (!file.ReadString(line)) {
			AfxMessageBox(_T("log.ini 파일이 비어 있습니다."));
			return ;
		}
		file.Close();

		// 3. 시간 문자열 추출
		line.Replace("[", "");
		line.Replace("]", "");
		CString timestamp = line; // "[2024-12-03 14:22:19]"에서 "2024-12-03 14:22:19" 추출

		// 4. 문자열을 SYSTEMTIME으로 변환
		SYSTEMTIME logTime = { 0 };
		int year, month, day, hour, minute, second;
		if (_stscanf_s(timestamp, _T("%4d-%2d-%2d %2d:%2d:%2d"), &year, &month, &day, &hour, &minute, &second) != 6) {
			AfxMessageBox(_T("시간 형식이 올바르지 않습니다."));
			return ;
		}
		logTime.wYear = year;
		logTime.wMonth = month;
		logTime.wDay = day;
		logTime.wHour = hour;
		logTime.wMinute = minute;
		logTime.wSecond = second;

		// 5. SYSTEMTIME을 FILETIME으로 변환
		FILETIME logFileTime, currentFileTime;
		SystemTimeToFileTime(&logTime, &logFileTime);

		// 6. 현재 시간 가져오기
		SYSTEMTIME currentTime;
		GetLocalTime(&currentTime);
		SystemTimeToFileTime(&currentTime, &currentFileTime);

		// 7. FILETIME을 ULARGE_INTEGER로 변환 (64비트 정수 계산)
		ULARGE_INTEGER logTime64, currentTime64;
		memcpy(&logTime64, &logFileTime, sizeof(FILETIME));
		memcpy(&currentTime64, &currentFileTime, sizeof(FILETIME));

		// 8. 시간 차이 계산 (100 나노초 단위, 1초 = 10,000,000)
		const ULONGLONG diffInSeconds = (currentTime64.QuadPart > logTime64.QuadPart) ?
			(currentTime64.QuadPart - logTime64.QuadPart) / 10000000 :
			(logTime64.QuadPart - currentTime64.QuadPart) / 10000000;

		// 9. 3초 이상 차이가 나는지 확인
		if (diffInSeconds > 3) {
			return ;
		}

		return ;
//	}
}


void CTestDlgDlg::OnBnClickedAesEnc2()
{
	CString rtnEnc;
	CString path, plainID;

	path.Format("F:\\util\\HTS\\IBK투자증권 HTS\\dev\\%s", "CX_SecureDataEngine.dll");
	HMODULE hModule = LoadLibrary(path);

	CString enc;
	enc = "khs779123";
	char* data = (LPSTR)(LPCTSTR)enc;
	char* aes{};

	CString s;
	if (hModule)
	{
		typedef long (WINAPI* GETSHAFUNC)(char*);
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(hModule, "axENCAES");
		aes = (char*)func(data);
		char* pdata = new char[strlen(aes) + 1];
		memset(pdata, 0x00, strlen(aes) + 1);
		memcpy(pdata, aes, strlen(aes));
		if (func)
			rtnEnc.Format("%s", pdata);


		typedef long (WINAPI* GETAESDECFUNC)(char*);
		GETAESDECFUNC func1 = (GETAESDECFUNC)GetProcAddress(hModule, "axDECAES");
		char* pdec = (char*)func1((LPSTR)(LPCTSTR)rtnEnc);
		plainID.Format("\r\n[encrypt3]-----------------------------%s", pdec);
		OutputDebugString(plainID);
	}

	if (hModule)
	  FreeLibrary(hModule);


	AfxMessageBox(plainID);
}



void CTestDlgDlg::OnBnClickedFds()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	short val = 12;
	char cval[2]{};

	cval[0] = val & 0xFF;
	cval[1] = (val >> 8) & 0xFF;

	int ilen = (unsigned char)cval[0] + ((unsigned char)cval[1] << 8);
	 ilen =cval[0] + (cval[1] << 8);

	typedef bool (*NPGetHtsWData_Func) (__in int nThreadTimeOut, __out char* szWData);

		CString sDllPath = _T("");
		int nThreadTimeOut = 20;	// thread timeout
		char szWData[2048] = { 0, };

		sDllPath.Format(_T("I3GManager.dll"));

		// 프로그램 시작시 한번 호출하면 된다. load dll
		if (m_hDll == NULL) {
			m_hDll = LoadLibrary(sDllPath);
		}

		// 데이터 수집
		if (m_hDll) {
			FARPROC pF = GetProcAddress(m_hDll, "NPGetHtsWData");
			if (pF)
			{
				NPGetHtsWData_Func npFunc = (NPGetHtsWData_Func)pF;

				bool bResult = npFunc(nThreadTimeOut, szWData);
				if (bResult) {
					int	nRet = strlen(szWData);
					char desB[1024];
					CopyMemory((char*)desB, szWData, nRet);
					m_slog.Format("[%d][%s]", nRet, szWData);
					AfxMessageBox(m_slog);
#ifdef DF_LOG
					CString stmp;
					stmp.Format("data = %s", desB);
					WriteLog(strFilePath, stmp);
#endif
					if (m_hDll)
					{
						FreeLibrary(m_hDll);
						m_hDll = NULL;
					}
				}
				else {
					AfxMessageBox("failed GetHtsWData");
				}
			}
		}



}


void CTestDlgDlg::OnBnClickedFdsSend()
{
	struct	_fdsR {
		char	flag{};		// fds flag
		char	rsvB[1]{};	// reserved
		short	fdsL{};		// fds data length
	};

	int isize = sizeof(struct _fdsR);

	if (m_hDll)
	{
		char szID[MAX_PATH] = { 0, };					//사용자 ID
		char szLoginSF[MAX_PATH] = { 0, };       //로그인 성공 실패 여부
		char szUserNm[MAX_PATH] = { 0, };      //사용자 이름
		char szSvcCode[MAX_PATH] = { 0, };		//서비스 코드
		char szReserve[MAX_PATH] = { 0, };        //추가정보 (우선 예약)
		int nTimeOut = 3000;	// Default TimeOut

		sprintf(szID, "testID123");
		sprintf(szLoginSF, "1");
		sprintf(szUserNm, "testUser");
		sprintf(szSvcCode, "1020400001");
		sprintf(szReserve, "testMeta");

		typedef int (*NPSndIPinside_Test_Func)
			(char* pGSvr, char* pWSvr, int nGSvrPort, int nWSvrPort, char* cID, char* cLoginSF, char* cUserName, int   nTimeOut, char* cSvcCode, char* cReserve);


		// 데이터 수집/전송
		if (m_hDll) {
			FARPROC pF = GetProcAddress(m_hDll, "NPSndWData");
			if (pF)
			{
				NPSndIPinside_Test_Func npFunc = (NPSndIPinside_Test_Func)pF;
				npFunc("256.256.256.256", "192.168.124.226", 80, 443, szID, szLoginSF, szUserNm, nTimeOut, szSvcCode, szReserve);
			}
		}
	}
}


void CTestDlgDlg::OnBnClickedFdsClose()
{
	// 프로그램 종료시 한번 하면 된다. free dll
	if (m_hDll)
	{
		typedef int (*NPSndIPinside_TestNew_Func) ();
		FARPROC pF = GetProcAddress(m_hDll, "NPReleaseSndWData");
		if (pF)
		{
			NPSndIPinside_TestNew_Func npFunc = (NPSndIPinside_TestNew_Func)pF;
			npFunc();
		}

		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}

}


void CTestDlgDlg::OnBnClickedNxtMid()
{
	bool prev;
	double value = 6000050;
	CString string;
	string = "6001050";

	prev = false;
	CString sOneVal, sCaledVal;  //원래 에디트값 저장
	sOneVal = string;
	sCaledVal.Format("%f", value);  //플랫폼이 계산한 값이다
	int itick{};
	if (prev) //true  
		itick = atoi(sCaledVal) - atoi(sOneVal);
	else
		itick = atoi(sOneVal) - atoi(sCaledVal);

	int nuint = itick;
	int imod = (int)value % (int)nuint;

	//prev -> false 내리기  prev->true 올리기
	if (imod != 0)
	{
		if (prev)
			nuint = nuint - imod;  //ex 1000   -  50  = 950
		else
			nuint = imod;          //ex   50
	}

	if (prev)
		value = value + nuint;   //ex   6001050  + 
	else
		value = value - nuint;

	sCaledVal.Format("%f", value);
	char* data = new char[100];
	memset(data, 0x00, 100);
	strcpy_s(data, 128, sCaledVal);
}


void CTestDlgDlg::OnBnClickedSfcode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString scode;
	scode = "1FRW3000";
	char tmp2[3] = {};  // 문자열 저장을 위해 3바이트(2문자 + 널 종료) 확보

	memcpy(tmp2, scode.Mid(2, 2), 2);  // 2번째(인덱스 2)부터 2글자 복사
	
	int pid{}, pid2{};
	int ival{};
	/* 주식선물 투자자 :: KRDRVFUSxx */
/*                    0123456789 */
/*                            ^  */
	//if (memcmp(tmpb, SF_TD_SPID2, 8) == 0)
	
	// B0 ~ ZZ
	//sprintf(tmp2, "%.2s", &tmpb[8]);
	if (tmp2[0] >= 'B')
	{
		/* ASCII 0:48, 7:55, B:66, =:61, G:71, H:72, W:87, Z:90 */
		pid = 61 + (tmp2[0] - 66) * 36;

		if (tmp2[1] >= 'A' && tmp2[1] <= 'Z')
			pid2 = tmp2[1] - 55;
		else
			pid2 = tmp2[1] - 48;

		ival =  pid + pid2 + 100;            /* B0:61+0+100=161, B2:61+2+100=163, GZ:61+180(5*36)+35=276  HA:61+216+10=287 HN:61+216+23=300   HZ:61+216(6*36)+35=312 */
	}
	else if (tmp2[0] >= '0' && tmp2[0] <= '9')
	{
		pid = (tmp2[0] - 48) * 36;

		if (tmp2[1] >= 'A' && tmp2[1] <= 'Z')
			pid2 = tmp2[1] - 55;
		else
			pid2 = tmp2[1] - 48;            /* 0A:0+10+400=410, 0Z:0+35+400=435, 11=36+1+400=437, 1A=36+10+400=446, 57=175+7+400=582, 9Z=324+35+400=759 */

		ival =  pid + pid2 + 400;
	}
	else
		ival =  atoi(tmp2) + 100; /* 10 ~ 59 + 100 */
	
	CString stmp;
	stmp.Format("%d", ival);
	AfxMessageBox(stmp);
}


void CTestDlgDlg::OnBnClickedSfcode2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString stmp, stemp;
	int ival = 0, cnt = 0;
	stmp = "8;10;20;20;4;40;20;16;23;16;16;16;16;16;16;40;10;40;13;16;16;16;16;16;16;9;10;12;16;16;16;16;16;3;40;16;16;13;16;16;16;16;16;20;20;16;16;16;16;16;16;16;16;16;16;16;16;16;8;100;40;";

	while (stmp.GetLength() > 0)
	{
		stemp = parser(stmp, ";");
		ival += atoi(stemp);
		cnt++;
	}

	TRACE("!23'");

	int irowcnt = 3;
	int idata;
	int ilinecnt = 10;

	CString sData , sLine;
	for (int ii = 0; ii < 45; ii++)
		sData += "1";

	sData = "1234567890abcdfghijk1264567890";

	CString ssval;
	ssval = "20250228+00000000150120125524         대체입금            1005대체입금                                정상                +000000000000000+000000000000000.000000+000000000000000+000000000000000+000000000000000+000000000000000+000000000000000+000000000000000HTS                                     +000000000                                        +000000000.00+000000000000000+000000000000000+000000000000000+000000000000000+000000000000000+000000000000000173734023+000000046            +000000000010000+000000000000000+000000000000000+000000000000000+000000000000000034IBKWM센터강남                           +000000000000000+000000000000000+000000000.00+000000000010000+000000000000000+000000000000000+000000000000000+000000000000000+00000000.0000000000+00000000.0000000000+000000000000000+000000000000000+000000000000000+000000000000000+000000000000000+000000000000000+000000000000000+000000000000000+000000000000000+000000000000000+000000000010000+000000000000000+000000000000000                                                                                                                                                    ";
	int iilen = ssval.GetLength();

	for (int ir = 0; ir <= irowcnt; ir++)
	{
		idata = 0;
		sLine = sData.Left(ilinecnt);
		sData = sData.Right(sData.GetLength() - ilinecnt);

	}
}

//DPI
void CTestDlgDlg::OnBnClickedDpi() 
{
	int x = 100;
	int iValue = 0;
	HWND hwnd = CWnd::GetDesktopWindow()->m_hWnd;
	HDC hdc = ::GetDC(hwnd);


	int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
	int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
	::ReleaseDC(m_hWnd, hdc);


	iValue = MulDiv(x, dpiX, 96);

}

#include "CDlg_Grid.h"
void CTestDlgDlg::OnBnClickedGrid()
{

	std::unique_ptr< CDlg_Grid> pdlg;
	pdlg = std::make_unique< CDlg_Grid>();
	pdlg.get()->DoModal();
	/*if (dlg.DoModal() == IDOK)
	{
		CString str;
	}
	else
	{
		CString str;
	}*/
	pdlg.release();
}


void CTestDlgDlg::OnBnClickedGrid2()
{
#ifndef dd
	std::vector<std::pair<CString, CString>> _varr;

	auto& [a, b] = _varr.emplace_back("234", "567");


	MSG msg{};
	bool m_fDone{};
	for (m_fDone = FALSE; !m_fDone; WaitMessage())
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			OutputDebugString("\r\n 1");
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CString str;




#else

	const auto& [itdata, inserted] = map.try_emplace(strCode.GetString(), strtemp.GetString()); if (!inserted) itdata->second = strtemp;



	CString tmp;
	int idx = 0;

	CString arr1[50]{ "", };
	CString sIMAXInfo;
	sIMAXInfo = "1\t2\t3\t";
	while (AfxExtractSubString(tmp, sIMAXInfo, idx++, '\t'))
	{
		arr1[idx - 1] = tmp;
	}

	CString str;



	CStringArray arr;
	CMapStringToString smap;

	//＃ ♭
	arr.Add("Dm7");
	smap.SetAt("Dm7", "F A C E 검엄     [1]: key(D)에서 1.5업, [2]: 1에서2온업 , [3]: key(D)에서 1다운,  [4]: key(D)에서 1업");

	arr.Add("G7");
	smap.SetAt("G7", "약중 F A B E         [1]: key(G)에서 1다운, [2]: key(G)에서 1업, [3]: 2에서 1업,  [4]: key(G)에서 1.5다운");

	arr.Add("CM7");
	smap.SetAt("CM7", "E G B D 검엄      [1] : key(C)에서 2업, [2] : 1에서1.5업 [3]:  key(C)에서1.5다운  [4]: key(C)에서 1업");




	arr.Add("Gm7");
	smap.SetAt("Gm7", "B♭ D F A 검엄   [1]: key(G)에서 1.5업, [2]: 1에서2온업 , [3]: key(G)에서 1다운,  [4]: key(G)에서 1업");

	arr.Add("C7");
	smap.SetAt("C7", "약중 B♭ D E A      [1]: key(C)에서 1다운, [2]: key(C)에서 1업, [3]: 2에서 1업,  [4]: key(C)에서 1.5다운");

	arr.Add("FM7");
	smap.SetAt("FM7", "A C E G 검엄    1 : key(F)에서 2업, 2 : 1에서1.5업");




	arr.Add("Cm7");
	smap.SetAt("Cm7", "E♭ G B♭ D 검엄");
	arr.Add("F7");
	smap.SetAt("F7", "약중 E♭ G A D");
	arr.Add("B♭M7");
	smap.SetAt("B♭M7", "D F A C 검엄");


	arr.Add("Fm7");
	smap.SetAt("Fm7", "A♭ C E♭ G 검엄");
	arr.Add("B♭7");
	smap.SetAt("B♭7", "약중 A♭ C D G");
	arr.Add("E♭M7");
	smap.SetAt("E♭M7", "G B♭ D F 검엄");


	arr.Add("B♭m7");
	smap.SetAt("B♭m7", "D♭ F A♭ C 검엄");
	arr.Add("E♭7");
	smap.SetAt("E♭7", "약중 D♭ F G C");
	arr.Add("A♭M7");
	smap.SetAt("A♭M7", "C E♭ G B♭ 검엄");


	arr.Add("Am7");
	smap.SetAt("Am7", "C E G B 검엄");
	arr.Add("D7");
	smap.SetAt("D7", "약중 C E F＃ B");
	arr.Add("GM7");
	smap.SetAt("GM7", "B D F＃ A 검엄");


	arr.Add("Em7");
	smap.SetAt("Em7", "G B D F# 검엄");
	arr.Add("A7");
	smap.SetAt("A7", "약중 G B C F#");
	arr.Add("DM7");
	smap.SetAt("DM7", "F＃ A C# E  검엄");


	arr.Add("Bm7");
	smap.SetAt("Bm7", "D F# A C# 검엄");
	arr.Add("E7");
	smap.SetAt("E7", "약중 D F# G# C#");
	arr.Add("AM7");
	smap.SetAt("AM7", "C  E  G# B  검엄");

	arr.Add("F#m7");
	smap.SetAt("F#m7", "A C# E G# 검엄");
	arr.Add("B7");
	smap.SetAt("B7", "약중 A C# D# G#");
	arr.Add("EM7");
	smap.SetAt("EM7", "G#  B  D# F#  검엄");


	srand((unsigned)time(NULL));
	int nRandomIndex = rand() % arr.GetSize();

	// 선택된 코드 출력
	CString strMessage;
	strMessage.Format(_T("랜덤 코드: %s"), arr[nRandomIndex]);
	

	int result = AfxMessageBox(strMessage, MB_YESNO | MB_ICONQUESTION);

	if (result == IDYES)
	{
		CString sval;
		if (smap.Lookup(arr[nRandomIndex], sval))
		{
			AfxMessageBox(sval);
		}
	}
	else
	{
		AfxMessageBox(_T("사용자가 'No'를 선택했습니다."));
	}
#endif
}



void CTestDlgDlg::OnBnClickedFdsfile()
{
	CString strFind;
	strFind = "AXLOGONC";
	bool bfind{};
	CFile	file;
	CString sBuf;
	CString sPath{};
	CString strFilePath{};
	strFilePath = "F:\\util\\HTS\\IBK투자증권 HTS";
	sPath.Format("%s\\tab\\FDSSERVICE.DAT", strFilePath);
	if (file.Open(sPath, CFile::modeRead | CFile::shareDenyNone))
	{
		const	long	len = (long)file.GetLength();
		std::unique_ptr<char[]> buf = std::make_unique<char[]>(len + 1);
		file.Read(buf.get(), len);
		sBuf = CString(buf.get(), len);
		sBuf.MakeUpper();
		buf.reset();
		file.Close();
	}

	CString line;
	bool inFDSSection = false;
	CStdioFile stdfile(sPath, CFile::modeRead);
	while (stdfile.ReadString(line))
	{
		line.Trim(); // 양쪽 공백 제거

		// 섹션 시작점 찾기
		if (line == _T("[FDS]"))
		{
			inFDSSection = true;
			continue;
		}

		if(line.Left(1) == "[")
			inFDSSection = false;

		// 다른 섹션 시작되면 FDS 섹션 끝났다고 간주
		if (inFDSSection)
			CString str;
		//if (inFDSSection && line.Left(0).Compare(_T("[")))
		if (inFDSSection && line.Compare(strFind) == 0)
		{
			bfind = true;
			break;
		}
	}
	stdfile.Close();

	if (bfind)
		AfxMessageBox("찾음 " + strFind);
}

using HCODEX = struct hjcodex;
void CTestDlgDlg::OnBnClickedFdsfile2()
{
	////
	CString ETFile;
	CString data, line;

	ETFile = "C:\\IBKS\\IBK투자증권 HTS\\tab\\efcode.dat";
	CStdioFile file(ETFile, CFile::modeRead);
	std::vector<std::tuple<char, CString, CString, CString, char, char>>	_vETFcode;
	while (file.ReadString(line))
	{
		switch (const char gubn = line.GetAt(0); gubn)
		{
		case '1':
		{
			const _efopitem* efopitem = (struct _efopitem*)line.GetBuffer();
		
		}
		break;
		case '2':
		{
			const _efoptema* efoptema = (struct _efoptema*)line.GetBuffer();
	
		}
		break;
		case '3':
		{
			const _efopfore* efopfore = (struct _efopfore*)line.GetBuffer();
	
		}
		break;
		case '4':
		{
			int maxBytes = 39;
			CString stmp, sval;
			const _efopcode* efopcode = (struct _efopcode*)line.GetBuffer();
			stmp = CString(efopcode->hnam, sizeof(efopcode->hnam)).Trim();
			CString sname;
			if (stmp.GetLength() >= maxBytes)
			{
				int ilen = sizeof(efopcode->hnam);
				int safeLen = 0;

				// 한글이 깨지지 않도록 safeLen 계산
				for (int i = 0; i < maxBytes; )
				{
					int charLen = IsDBCSLeadByte((BYTE)efopcode->hnam[i]) ? 2 : 1;

					// 다음 글자가 maxBytes를 넘으면 중단
					if (i + charLen > maxBytes)
						break;

					i += charLen;
					safeLen = i;
				}

				// 안전한 길이만큼 복사
				CStringA cstrA(efopcode->hnam, safeLen);

				// 필요시 CStringW로 변환
				CString cstr(cstrA);
				sname = cstr;
			}
			else
			{
				sname = CString(efopcode->hnam, sizeof(efopcode->hnam)).Trim();
			}

			_vETFcode.emplace_back(std::move(std::tuple(efopcode->type[0],
				CString(efopcode->code, sizeof(efopcode->code)).Trim(),
				sname,
				CString(efopcode->opcd, sizeof(efopcode->opcd)).Trim(),
				efopcode->etfm[0],
				efopcode->etfl[0])));
		}
		break;
		default:
			break;
		}
	}
	file.Close();

   //////
	//std::map<CString, HCODEX*>			      _mapCODEx;
	//CFile	file;
	//CString	string = "C:\\IBKS\\IBK투자증권 HTS\\tab\\hjcode3.dat";
	//
	//{
	//	if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	//		return ;

	//	const int size = gsl::narrow_cast<int>(file.GetLength());
	//	m_hcB = std::make_unique<char[]>(size + 1);
	//	const int len = file.Read(m_hcB.get(), size);
	//	file.Close();

	//	

	//	const int count = len / sizeof(struct hjcodex);
	//	struct	hjcodex* hjc = (struct hjcodex*)m_hcB.get();

	//	for (int ii = 0; ii < count; ii++, hjc++)
	//	{
	//		CString sCode = CString(hjc->code, HCodeLen).Mid(1);
	//		_mapCODEx.emplace(std::move(sCode), hjc);
	//		if (hjc->ssgb == jmELW)
	//			continue;

	//		if (hjc->ssgb == jmETN)	//2014.10.28 KSJ ETN 추가
	//		{
	//			_vETNx.emplace_back(hjc);
	//			continue;
	//		}
	//		_mapACODE.emplace(std::move(sCode), std::move(std::make_pair(CString(hjc->hnam, HNameLen).Mid(1).Trim(), 0)));
	//		_mapNAME.emplace(std::move(CString(hjc->hnam, HNameLen).Mid(1).Trim()), std::move(sCode));
	//	}
		////////////



		std::vector<std::pair<CString, CString>> 	_vData, _vSelect;

		for_each(_vETFcode.begin(), _vETFcode.end(), [&](const auto item) {
			auto [type, code, hnam, opcd, etfm, etfl] = item;
			_vData.emplace_back(std::move(std::make_pair(code, hnam)));
			});


		std::copy(_vData.begin(), _vData.end(), std::back_inserter(_vSelect));

		///////////////


		std::vector<std::pair<CString, CString>> vSearch;

		bool bsearch = false;

		std::copy(_vSelect.begin(), _vSelect.end(), std::back_inserter(vSearch));

		CString sName;
		sName = "1";
		char cName{};
		const int sLen = sName.GetLength();
		CString name;
		auto ft = std::remove_if(vSearch.begin(), vSearch.end(), [&](const auto item) 
		{
			name = item.second;

			CString strUpperS = sName;
			CString strUpperO = name;
			strUpperS.MakeUpper();

			OutputDebugString(strUpperO + "\r\n");

			if (strUpperO.GetLength() == 0)
			{
				AfxMessageBox("123");
				strUpperO.MakeUpper();
			}
				strUpperO.MakeUpper();

		/*	if (strncmp(strUpperS, strUpperO, sLen) != 0)
				return true;*/
			return false;
			});
	}

#include <afxinet.h> 

#include <windows.h>
//#include <winhttp.h>


	void CTestDlgDlg::OnBnClickedPubip()
	{
		int ival = 1;
		
		if (ival == 0)
		{
			CInternetSession session;
			CHttpFile* pFile = nullptr;
			CString strIP;

			// api.ipify.org: text 형식으로 공인 IP만 반환
			pFile = (CHttpFile*)session.OpenURL(
				_T("http://api.ipify.org?format=text"),
				1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD);

			if (pFile)
			{
				pFile->ReadString(strIP);
				delete pFile;
			}
			session.Close();
		}
		else if (ival == 1)
		{
			/*HINTERNET hSession = WinHttpOpen(L"MyAgent/1.0",
				WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
				WINHTTP_NO_PROXY_NAME,
				WINHTTP_NO_PROXY_BYPASS, 0);
			if (!hSession) return ;

			HINTERNET hConnect = WinHttpConnect(hSession,
				L"api.ipify.org",
				INTERNET_DEFAULT_HTTP_PORT, 0);
			if (!hConnect) { WinHttpCloseHandle(hSession); return ; }

			HINTERNET hRequest = WinHttpOpenRequest(hConnect,
				L"GET", L"/?format=text",
				nullptr, WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
			CString strIP;
			if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
				WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
				WinHttpReceiveResponse(hRequest, nullptr))
			{
				DWORD dwSize = 0;
				WinHttpQueryDataAvailable(hRequest, &dwSize);
				if (dwSize)
				{
					char* buffer = new char[dwSize + 1];
					ZeroMemory(buffer, dwSize + 1);
					DWORD dwRead = 0;
					WinHttpReadData(hRequest, buffer, dwSize, &dwRead);
					strIP = CString(buffer);
					delete[] buffer;
				}
			}

			WinHttpCloseHandle(hRequest);
			WinHttpCloseHandle(hConnect);
			WinHttpCloseHandle(hSession);*/

		}
		
	}


	void CTestDlgDlg::OnBnClickedCreateProcess()
	{
		char szMain[MAX_PATH] = { 0, };
		CString m_root;
		//::GetModuleFileName(NULL, szMain, sizeof(szMain));
		m_root.Format("%s", "F:\\util\\HTS\\IBK투자증권 HTS");
	/*	int ifind = m_root.ReverseFind('\\');
		m_root = m_root.Left(ifind);
		ifind = m_root.ReverseFind('\\');
		m_root = m_root.Left(ifind);

		m_slog.Format("[axiscore] m_root =[%s] m_regKey=[%s]\n", m_root, m_regKey);
		OutputDebugString(m_slog);*/


		CString	string;
		char	wb[1024];
		BOOL	fBool;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		//string = GetProfileString(ENVIRONMENT, ROOTDIR);
		//if (string.IsEmpty())
		//{
		//	::MessageBox(NULL, "Nothing install information!!", "Install error", MB_ICONWARNING);
		//	return FALSE;
		//	WriteReg();
		//}
		CString stmp;
		stmp.Format("[core] string [%s]", m_root);
		OutputDebugString(stmp);

		//	if (IsInfoErr())	return FALSE;
		sprintf(wb, "%s\\exe\\axis.exe", m_root);
		m_slog.Format("%s", wb);
		OutputDebugString(m_slog);
#if 0
		//	deb mode
		string.Format(" %s /k \"%s\" /n %s /p %c /s %c", m_lpCmdLine, m_regKey, m_pszExeName, m_pk, 0x7f);
#else
		//	real mode
		CString	sKey, eKey;
		



#endif

		ZeroMemory(&si, sizeof(STARTUPINFO));
		ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOWNORMAL;
		OutputDebugString("[core] " + string);
		fBool = CreateProcess(wb,					// application name
			(char*)string.operator LPCTSTR(),	// command line
			NULL,					// process attribute
			NULL,					// thread attribute
			FALSE,					// is inherit handle
			0,					// creation flags
			NULL,					// environment
			NULL,					// current directory
			&si,					// STARTUPINFO
			&pi);					// PROCESS_INFORMATION
		m_slog.Format("[core] wb=%s", wb);
		OutputDebugString(m_slog);
		m_slog.Format("[core] %d", GetLastError());
		OutputDebugString(m_slog);
	}


	void CTestDlgDlg::OnBnClickedSha()
	{
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		if (1)
		{
			const char obfAxisExe[] = "\x61\x78\x69\x73\x2E\x65\x78\x65";
			char wb[32]{};
			sprintf(wb, "%s", obfAxisExe);   // 복호화된 "axis.exe"

			CString str(wb);
			AfxMessageBox(str);

			CString strPath;
			char chfile[500]{};
			GetModuleFileName(nullptr, chfile, 260);
			strPath.Format("%s", chfile);

			int nPos = strPath.ReverseFind('\\');

			CString strExeName;
			if (nPos != -1)
			{
				// 마지막 '\' 다음부터 문자열 끝까지 (실행 파일명)을 추출합니다.
				strExeName = strPath.Mid(nPos + 1);
			}
			else
			{
				// '\' 문자가 없을 경우 전체 경로가 실행 파일명이라고 가정합니다.
				strExeName = strPath;
			}
		}
		CString filename;
		HMODULE hModule = LoadLibrary("F:\\util\\HTS\\IBK투자증권 HTS\\dev\\CX_SHA256.DLL");


		typedef long (WINAPI* GETSHAFUNC)(char*);

		//D:\\IBKS_TEST\\src\\APPL\\TestDlg\\TestDlg\\Release\\TestDlg.exe
		char* pName = "D:\\IBKS_TEST\\src\\APPL\\TestDlg\\TestDlg\\Release\\TestDlg.exe";
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(hModule, "axFileEncrypt");
		if (func)
		{
			char*  sha256 = (char*)func(pName);
			filename.Format("%s", sha256);
			AfxMessageBox(filename);
		}

	}


	typedef struct _MYMEMORYSTATUSEX
	{
		DWORD dwLength;
		DWORD dwMemoryLoad;
		DWORDLONG ullTotalPhys;
		DWORDLONG ullAvailPhys;
		DWORDLONG ullTotalPageFile;
		DWORDLONG ullAvailPageFile;
		DWORDLONG ullTotalVirtual;
		DWORDLONG ullAvailVirtual;
		DWORDLONG ullAvailExtendedVirtual;
	} MYMEMORYSTATUSEX, * LPMYMEMORYSTATUSEX;
	typedef BOOL(WINAPI* pGlobalMemoryStatusEx)(LPMYMEMORYSTATUSEX lpBuffer);

	void CTestDlgDlg::OnBnClickedPcinfo()
	{
		//if (isWindowsNT() & (m_dwWinMajor >= 5))
		{
			// Windows 2000+. Use GlobalMemoryStatusEx to detect memory size more than 4GB
			MYMEMORYSTATUSEX MemStatEx;

			MemStatEx.dwLength = 0;
			MemStatEx.dwMemoryLoad = 0;
			MemStatEx.ullTotalPhys = 0;
			MemStatEx.ullAvailPhys = 0;
			MemStatEx.ullTotalPageFile = 0;
			MemStatEx.ullAvailPageFile = 0;
			MemStatEx.ullTotalVirtual = 0;
			MemStatEx.ullAvailVirtual = 0;
			MemStatEx.dwLength = sizeof(MemStatEx);

			HMODULE hmKernelDll{};
			pGlobalMemoryStatusEx m_pGlobalMemoryStatusEx = NULL;
			if ((hmKernelDll = ::LoadLibrary(_T("kernel32.dll"))) != NULL)
			{
				m_pGlobalMemoryStatusEx = (pGlobalMemoryStatusEx)::GetProcAddress(hmKernelDll, "GlobalMemoryStatusEx");
				if (m_pGlobalMemoryStatusEx != NULL)
				{
					if ((*m_pGlobalMemoryStatusEx)(&MemStatEx))
					{
						DWORD m_dwMemoryLoad = MemStatEx.dwMemoryLoad;
						ULONGLONG m_dwTotalPhys = MemStatEx.ullTotalPhys;
						ULONGLONG m_dwAvailPhys = MemStatEx.ullAvailPhys;
						ULONGLONG m_dwTotalPageFile = MemStatEx.ullTotalPageFile;
						ULONGLONG m_dwAvailPageFile = MemStatEx.ullAvailPageFile;
						ULONGLONG m_dwTotalVirtual = MemStatEx.ullTotalVirtual;
						ULONGLONG m_dwAvailVirtual = MemStatEx.ullAvailVirtual;

						CString sval;
						sval.Format("%lu", m_dwMemoryLoad);
						printf("Memory Load     : %lu %%\n", m_dwMemoryLoad);
						printf("Total Phys RAM  : %.2f GB\n", m_dwTotalPhys / (1024.0 * 1024 * 1024));
						printf("Available RAM   : %.2f GB\n", m_dwAvailPhys / (1024.0 * 1024 * 1024));

						int bSuccess = TRUE;
					};
				};
				::FreeLibrary(hmKernelDll);
			};
		}
	}

	//"D:\\IBKS_TEST\\src\\APPL\\TestDlg\\TestDlg\\resource.ini"
		//	CStringArray arrLines{};
	void CTestDlgDlg::OnBnClickedMapList()
	{
		CString stmp;
		CStringArray arrLines{};
		

		CFile file;
		const UINT chunkSize = 35;

		if (!file.Open("D:\\IBKS_TEST\\src\\APPL\\TestDlg\\TestDlg\\resource.ini", CFile::modeRead| CFile::typeBinary))
		{
			AfxMessageBox(_T("파일을 열 수 없습니다."));
			return;
		}

		BYTE buffer[chunkSize];
		UINT bytesRead = 0;

		while ((bytesRead = file.Read(buffer, chunkSize)) > 0)
		{
			// 바이트 버퍼를 문자열처럼 처리하되 널 무시
			std::string tempStr;
			for (UINT i = 0; i < bytesRead; ++i)
			{
				stmp.Format("%.1s", (char*)&buffer[i]);
				// 널문자도 문자로 포함 (즉, 그대로 삽입)
				tempStr += stmp;
			}

			// ANSI → CString으로 변환 (널 포함한 상태 그대로 유지)
			CString strChunk(tempStr.c_str(), (int)tempStr.length()); // 길이 명시 중요!

			arrLines.Add(strChunk);
		}

		file.Close();

		for (int ii = 0; ii < arrLines.GetSize(); ii++)
		{
			OutputDebugString(arrLines.GetAt(ii) );
			OutputDebugString("\r");
		}
	}

	struct CITEMS
	{
		CString code;
		CString name;
	};

	void CTestDlgDlg::OnBnClickedVector()
	{ 
		COleDateTime oTime;
		oTime = COleDateTime::GetCurrentTime();
		CString strCurTime;
		strCurTime.Format(_T("%02d%02d%02d"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

		int h1 = _ttoi(strCurTime.Mid(0, 2));
		int m1 = _ttoi(strCurTime.Mid(2, 2));
		int s1 = _ttoi(strCurTime.Mid(4, 2));
		CTime timecur(oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(), h1, m1, s1);

		CString m_strBeginTimeEnd;
		m_strBeginTimeEnd = "085959";
		CTime m_timeEnd;
		oTime = COleDateTime::GetCurrentTime();
		 h1 = _ttoi(m_strBeginTimeEnd.Mid(0, 2));
		 m1 = _ttoi(m_strBeginTimeEnd.Mid(2, 2));
		 s1 = _ttoi(m_strBeginTimeEnd.Mid(4, 2));
		m_timeEnd = CTime(oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(), h1, m1, s1);\

		CTimeSpan diff = timecur - m_timeEnd;

		int num = diff.GetTotalSeconds();
		m_slog.Format("%d", diff.GetTotalSeconds());
		CString code;
		code = "005930";
		std::string scode = CStringA(code);

		code.Format("%s", scode.c_str());
		//std::unordered_map

		CFile	file;
		CString	string = AxStd::FORMAT("%s", "hjcode3.dat");
		if (AxStd::isFileExist(string))
		{
			if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
				return;
		}

		const int  size = gsl::narrow_cast<int>(file.GetLength());
		m_hcB = std::make_unique<char[]>(size + 1);
		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		if(size > len)
		{
			m_hcB.reset();
			return;
		}
		int ilen = sizeof(struct hjcodex);
		const int count = len / sizeof(struct hjcodex);
		struct	hjcodex* hjc = (struct hjcodex*)m_hcB.get();

		std::vector<CITEMS> vTrash, vList;

		for (int ii = 0; ii < count; ii++, hjc++)
		{
			CString sCode = CString(hjc->code, HCodeLen).Mid(1);
			CString name = CString(hjc->hnam, ENameLen).Mid(1);
			

			auto& item = vList.emplace_back(std::move(CITEMS()));
			item.code = sCode;
			item.name = name;

			if (hjc->ssgb == 22)
			{
				OutputDebugString(name +"\r");
			}

			if (hjc->ssgb == 21)
			{
				OutputDebugString(name + "\r");
			}
		}

		auto first = std::remove_if(vList.begin(), vList.end(), [&vTrash](const auto item) {
			const CString& name = item.name;
			if (name.Find("@") >= 0)
			{
				vTrash.push_back(item);
				OutputDebugString(item.name);
				OutputDebugString("\r");
				return true;
			}

			if (name.GetAt(0) == 88 || name.GetAt(0) == 64 || name.GetAt(0) == 47 || name.GetAt(0) == 36 || name.GetAt(0) == 37 || name.GetAt(0) == 38 || name.GetAt(0) == 33)
			{
				vTrash.push_back(item);
				return true;
			}
			else
				return false;
			});
		vList.erase(first, vList.end());
		/*std::vector<CITEMS> vTrash;
		auto first = std::remove_if(_vList.begin(), _vList.end(), [&vTrash](const auto item) {
			const CString& name = item.name;
			if (name.GetAt(0) == 88 || name.GetAt(0) == 64 || name.GetAt(0) == 47 || name.GetAt(0) == 36 || name.GetAt(0) == 37 || name.GetAt(0) == 38 || name.GetAt(0) == 33)
			{
				vTrash.push_back(item);
				return true;
			}
			else
				return false;
			});
		_vList.erase(first, _vList.end());

		m_list1.SetItemCountEx(_vList.size());
		listsort(column);

		_vList.insert(_vList.end(), vTrash.begin(), vTrash.end());
		m_list1.SetItemCountEx(_vList.size());

		deselcMark(&m_list1);
		selcMark(&m_list1, 0);
		vTrash.clear();*/
	}


	void CTestDlgDlg::OnBnClickedWinpop()
	{
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		CString sClassName = AfxRegisterWndClass(0);

		if (m_pPopup)
			m_pPopup.reset();

		
		m_pPopup = std::make_unique<CMarketPopWnd>();

		//m_pPopup.get()->m_pParent = this;
		int cx = 225;
		int cy = 294;
		CRect rc;
		CPoint pt{};
		GetCursorPos(&pt);

		if (!m_pPopup->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, sClassName, NULL,
			WS_POPUP | WS_BORDER, CRect(pt.x, pt.y, pt.x + cx, pt.y + cy), this, NULL, NULL))
		//if (!m_pPopup->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_VSCROLL, CRect(rc.left, rc.bottom, rc.left + cx, rc.bottom + cy), this, 1000))
		{
			m_pPopup = NULL;
			AfxMessageBox("create fail");
			return;
		}
		else
		{
			m_pPopup->ShowWindow(SW_SHOWNORMAL);
			m_pPopup->UpdateWindow();
			m_pPopup->SetForegroundWindow();
		}
	}


	void CTestDlgDlg::OnBnClickedCombopop()
	{
		CString sClassName = AfxRegisterWndClass(0);

		if (m_pPopcombo)
			m_pPopcombo.reset();

		m_pPopcombo = std::make_unique<CComboBox>();

		int cx = 100;
		int cy = 200;
		CRect rc;
		CPoint pt{};
		GetCursorPos(&pt);

		if (!m_pPopcombo->CreateEx(
			WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME,
			_T("ComboBox"), NULL,
			WS_POPUP | WS_BORDER | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
			CRect(pt.x, pt.y, pt.x + cx, pt.y + cy), this, NULL))
		{
			m_pPopcombo = NULL;
			AfxMessageBox("create fail");
			return;
		}
		else
		{
			SetWindowTheme(m_pPopcombo->GetSafeHwnd(), L"", L"");
			m_pPopcombo->ShowWindow(SW_SHOWNORMAL);
			m_pPopcombo->UpdateWindow();
			m_pPopcombo->SetForegroundWindow();
			m_pPopcombo->AddString("123");
			m_pPopcombo->AddString("456");
			m_pPopcombo->AddString("789");
			m_pPopcombo->SetCurSel(0);
			m_pPopcombo->ShowDropDown(TRUE);
		}
	}


	void CTestDlgDlg::OnBnClickedBtnAlgo()
	{

		std::map<int, CString> testMap;

		auto it = testMap.emplace(std::move(std::make_pair(1, "test")));
		
		if (it.second)
			it.first->second = "test";

		CString val;
		val.Format("%s", it.first->second);
	}

#include "CDlgSTD.h"
	void CTestDlgDlg::OnBnClickedStd()
	{
		CDlgSTD dlgSTD;
		/*std::unique_ptr< CDlgSTD> dlg;
		dlg = std::make_unique< CDlgSTD>();
		dlg.get()->DoModal();*/
		dlgSTD.DoModal();
	}


	CString Parser(CString& srcstr, CString substr)
	{
		if (srcstr.Find(substr) == -1)
		{
			CString temp = srcstr;
			srcstr.Empty();
			return temp;
		}
		else
		{
			CString  temp = srcstr.Left(srcstr.Find(substr));
			srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
			return temp;
		}
		return "";
	}

	void CTestDlgDlg::OnBnClickedBtnFilemove()
	{
		CString m_sRoot;
		CString strVal, strMovePath, strtmp, strfile, strOriPath, stmp, stemp;

		char sbuf[1024]{};
		GetModuleFileName(nullptr, sbuf, 260);
		m_sRoot.Format("%s", sbuf);
		m_sRoot.TrimRight();
		m_sRoot = m_sRoot.Left(m_sRoot.Find("exe"));

		m_sRoot = "F:\\util\\HTS\\IBK투자증권 HTS\\";   //test

		char buff[128];
		CString iniConf = m_sRoot + "tab\\AXISFILE.ini";
		GetPrivateProfileString("move", "cnt", "", buff, sizeof(buff), iniConf);
		stmp.Format("%s", buff);
		int cnt = atoi(stmp);

		// --- 기존 히스토리 읽기
		char histBuff[2048]{};
		GetPrivateProfileStringA("history", "file", "", histBuff, sizeof(histBuff), CT2CA(iniConf));
		CString strHistory(histBuff);

		for (int ii = 0; ii < cnt; ii++)
		{
			stmp.Format("%d", ii);

			GetPrivateProfileString("move", stmp, "", buff, sizeof(buff), iniConf);
			strVal.Format("%s", buff);
			strVal.TrimRight();

			//ex) gex:mapc2_30-39.xml;icss\finddata\map     다운로드위치:파일명 ; 옮길위치
			strtmp = Parser(strVal, ";");
			strtmp.TrimRight();
			stmp = Parser(strtmp, ":");
			strtmp.TrimRight();
			strfile = strtmp;
			strOriPath.Format("%s%s\\%s", m_sRoot, stmp, strfile);

			stemp = strVal;
			stemp.TrimRight();
			strMovePath.Format("%s%s", m_sRoot, stemp);

			CFileFind ff;
			if (!ff.FindFile(strMovePath))
				::CreateDirectory(strMovePath, NULL);

			strMovePath.Format("%s%s\\%s", m_sRoot, stemp, strfile);

			CFileFind cfFind;
			BOOL bFind = cfFind.FindFile(strOriPath);

			if (bFind)
			{
			//	if (strHistory.Find(strfile) != -1)
			//		continue; // 이미 처리됨
				
				CopyFile(strOriPath, strMovePath, FALSE);
				DeleteFile(strOriPath);

				//// 기존 히스토리 읽기
				//char histBuff[1024]{};
				//GetPrivateProfileStringA("history", "file", "", histBuff, sizeof(histBuff), CT2CA(iniConf));

				//CString strHistory(histBuff);
				//if (!strHistory.IsEmpty())
				//	strHistory += ";";

				//// 파일명 추가
				//strHistory += strfile;

				//// 다시 기록
				//WritePrivateProfileStringA("history", "file", CT2CA(strHistory), CT2CA(iniConf));
			}
		}




		

		
	}


	//void CTestDlgDlg::OnBnClickedBtnMsgbox()
	//{
		/*TASKDIALOGCONFIG config = { 0 };
		config.cbSize = sizeof(config);
		config.hwndParent = nullptr;
		config.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION;
		config.pszWindowTitle = L"TaskDialogIndirect 예제";
		config.pszMainInstruction = L"이건 MessageBox 대체입니다.";
		config.pszContent = L"TaskDialogIndirect는 Vista 이후에서 사용할 수 있고,\n"
			L"MessageBox보다 풍부한 UI를 제공합니다.";
		config.dwCommonButtons = TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON;
		config.pszMainIcon = TD_INFORMATION_ICON;

		int nButtonPressed = 0;
		TaskDialogIndirect(&config, &nButtonPressed, nullptr, nullptr);*/

		
	//}
	double RoundDown(double data, int digits)
	{
		double factor = pow(10.0, digits);
		return floor(data * factor) / factor;

	}

	void CTestDlgDlg::OnBnClickedCal()
	{
		double dval = 123.19;
		 dval = RoundDown(dval, 1);

		m_slog.Format("%f", dval);
		OutputDebugString(m_slog);
	}

	CString CTestDlgDlg::GetVCSystemVersion()
	{
		CString bestVersion{};

		LPCTSTR baseKeys[] = {
		_T("SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\x86"),
	/*	_T("SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\15.0\\VC\\Runtimes\\x86"),
		_T("SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\16.0\\VC\\Runtimes\\x86"),
		_T("SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\17.0\\VC\\Runtimes\\x86"),*/
		};
		
		for (auto keyPath : baseKeys)
		{
			HKEY hKey = nullptr;
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
			{
				DWORD dwType = 0;
				TCHAR szVersion[256] = { 0 };
				DWORD dwSize = sizeof(szVersion);

				if (RegQueryValueEx(hKey, _T("Version"), nullptr, &dwType, (LPBYTE)szVersion, &dwSize) == ERROR_SUCCESS
					&& dwType == REG_SZ)
				{
					CString curVer(szVersion);
					auto ParseVer = [](const CString& ver)->std::vector<int>
					{
						std::vector<int> nums;
						CString temp(ver), token;
						int pos = 0;
						while ((pos = temp.Find(_T('.'))) != -1)
						{
							token = temp.Left(pos);
							nums.push_back(_ttoi(token));
							temp = temp.Mid(pos + 1);
						}
						if (!temp.IsEmpty()) nums.push_back(_ttoi(temp));
						return nums;
					};

					auto CompareVer = [&](const CString& v1, const CString& v2) -> int
					{
						std::vector<int> a = ParseVer(v1);
						std::vector<int> b = ParseVer(v2);

						while (a.size() < b.size()) a.push_back(0);
						while (b.size() < a.size()) b.push_back(0);

						for (size_t i = 0; i < a.size(); i++)
						{
							if (a[i] > b[i]) return 1;
							if (a[i] < b[i]) return -1;
						}
						return 0;
					};

					if (bestVersion.IsEmpty() || CompareVer(curVer, bestVersion) > 0)
					{
						bestVersion = curVer;
					}
					//v14.42.34438.00
					RegCloseKey(hKey);
				}
			}
		}//for

		return bestVersion;
	}








	//구버전용
	// 문자열 버전을 "." 단위로 나눠서 숫자 배열로 변환
	void ParseVer(const CString& ver, int* arr, int& count)
	{
		CString temp(ver);
		int pos = 0;
		CString token;
		count = 0;

		while ((pos = temp.Find(_T('.'))) != -1 && count < 10)
		{
			token = temp.Left(pos);
			arr[count++] = _ttoi(token);
			temp = temp.Mid(pos + 1);
		}
		if (!temp.IsEmpty() && count < 10)
			arr[count++] = _ttoi(temp);
	}
	// 버전 문자열 비교 (v1 > v2 : 1,  v1 < v2 : -1,  같으면 0)
	int CompareVer(const CString& v1, const CString& v2)
	{
		int a[10] = { 0 }, b[10] = { 0 };
		int ac = 0, bc = 0;

		ParseVer(v1, a, ac);
		ParseVer(v2, b, bc);

		int maxCount = (ac > bc) ? ac : bc;
		for (int i = 0; i < maxCount; i++)
		{
			if (a[i] > b[i]) return 1;
			if (a[i] < b[i]) return -1;
		}
		return 0;
	}
	//구버전 설치
	CString GetInstalledVCRuntimeVersion()
	{
		CString bestVersion = _T("");

		LPCTSTR baseKeys[] = {
			_T("SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\x86"),
			// 필요하면 아래 주석 해제
			//_T("SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\15.0\\VC\\Runtimes\\x86"),
			//_T("SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\16.0\\VC\\Runtimes\\x86"),
			//_T("SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\17.0\\VC\\Runtimes\\x86"),
		};

		int keyCount = sizeof(baseKeys) / sizeof(baseKeys[0]);
		for (int i = 0; i < keyCount; i++)
		{
			HKEY hKey = NULL;
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, baseKeys[i], 0, KEY_READ, &hKey) == ERROR_SUCCESS)
			{
				DWORD dwType = 0;
				TCHAR szVersion[256] = { 0 };
				DWORD dwSize = sizeof(szVersion);

				if (RegQueryValueEx(hKey, _T("Version"), NULL, &dwType, (LPBYTE)szVersion, &dwSize) == ERROR_SUCCESS
					&& dwType == REG_SZ)
				{
					CString curVer(szVersion);

					if (bestVersion.IsEmpty() || CompareVer(curVer, bestVersion) > 0)
						bestVersion = curVer;
				}
				RegCloseKey(hKey);
			}
		}
		return bestVersion;
	}

	void CTestDlgDlg::OnBnClickedMfcRege()
	{
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

		CString highestVer, targetVersion;
		highestVer = GetInstalledVCRuntimeVersion();
		targetVersion = "14.42.34438.00";

		BOOL bInstall;
		if (highestVer.IsEmpty() || CompareVer(highestVer, targetVersion) < 0)
		{
			bInstall = InstallVCReInstall("C:\\IBKS\\IBK투자증권 HTS\\exe\\VC_redist.x86.exe");
		}
	
		CString strdata;
	}

















	BOOL  CTestDlgDlg::InstallVCReInstall(CString spath)
	{
		CString installerPath = spath;

		SHELLEXECUTEINFO shExecInfo = { 0 };
		shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		shExecInfo.hwnd = NULL;
		shExecInfo.lpVerb = _T("runas");  // 관리자 권한 필요
		shExecInfo.lpFile = installerPath;
		shExecInfo.lpParameters = _T("/quiet /norestart");
		shExecInfo.lpDirectory = NULL;
		shExecInfo.nShow = SW_HIDE;
		shExecInfo.hInstApp = NULL;


		if (ShellExecuteEx(&shExecInfo))
		{
			// 설치가 끝날 때까지 대기 (필요하면)
			WaitForSingleObject(shExecInfo.hProcess, INFINITE);
			CloseHandle(shExecInfo.hProcess);

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	void CTestDlgDlg::OnBnClickedMfcRege2()
	{
		BOOL bSUCCED{};

		CString highestVer, targetVersion;
		highestVer = GetVCSystemVersion();
		targetVersion = "14.42.34438.00";

		auto ParseVer = [](const CString& ver)->std::vector<int>
		{
			std::vector<int> nums;
			CString temp(ver), token;
			int pos = 0;
			while ((pos = temp.Find(_T('.'))) != -1)
			{
				token = temp.Left(pos);
				nums.push_back(_ttoi(token));
				temp = temp.Mid(pos + 1);
			}
			if (!temp.IsEmpty()) nums.push_back(_ttoi(temp));
			return nums;
		};

		auto CompareVer = [&](const CString& v1, const CString& v2) -> int
		{
			std::vector<int> a = ParseVer(v1);
			std::vector<int> b = ParseVer(v2);

			while (a.size() < b.size()) a.push_back(0);
			while (b.size() < a.size()) b.push_back(0);

			for (size_t i = 0; i < a.size(); i++)
			{
				if (a[i] > b[i]) return 1;
				if (a[i] < b[i]) return -1;
			}
			return 0;
		};

		BOOL bInstall{};
		if (highestVer.IsEmpty() || CompareVer(highestVer, targetVersion) < 0)
		{
			bInstall =  InstallVCReInstall("C:\\IBKS\\IBK투자증권 HTS\\exe\\VC_redist.x86.exe");
		}

		CString strdata;
	}




	bool IsHangulChar(TCHAR ch)
	{
#ifdef _UNICODE
		// --- 유니코드 빌드일 경우 (기본 설정) ---
		// 한글 완성형 (가~힣)
		if (ch >= 0xAC00 && ch <= 0xD7A3)
			return true;

		// 자모 범위 (ㄱ~ㅎ, ㅏ~ㅣ)
		if ((ch >= 0x3131 && ch <= 0x318E) || (ch >= 0x1100 && ch <= 0x11FF))
			return true;

		return false;
#else
		// --- MBCS (ANSI) 빌드일 경우 ---
		BYTE b = (BYTE)ch;
		// 한글 첫 바이트는 0xB0~0xC8 사이 (완성형 기준)
		if (b >= 0xB0 && b <= 0xC8)
			return true;

		return false;
#endif
	}

	bool isInitialConsonant(WORD ch, WORD& start, WORD& end)
	{
		if (0xA4A1 > ch || ch > 0xA4BE)
			return false;


		const WORD	aInitial[19] =
		{
			0xA4A1, 0xA4A2, 0xA4A4, 0xA4A7, 0xA4A8, 0xA4A9, 0xA4B1,	// ㄱ,ㄲ,ㄴ,ㄷ,ㄸ,ㄹ,ㅁ
			0xA4B2, 0xA4B3, 0xA4B5, 0xA4B6, 0xA4B7, 0xA4B8, 0xA4B9,	// ㅂ,ㅃ,ㅅ,ㅆ,ㅇ,ㅈ,ㅉ
			0xA4BA, 0xA4BB, 0xA4BC, 0xA4BD, 0xA4BE			// ㅊ,ㅋ,ㅌ,ㅍ,ㅎ
		};
		const WORD	aStart[19] =
		{
			0xB0A1, 0xB1EE, 0xB3AA, 0xB4D9, 0xB5FB, 0xB6F3, 0xB8B6,	// 가,까,나,다,따,라,마
			0xB9D9, 0xBAFC, 0xBBE7, 0xBDCE, 0xBEC6, 0xC0DA, 0xC2A5,	// 바,빠,사,싸,아,자,짜
			0xC2F7, 0xC4AB, 0xC5B8, 0xC6C4, 0xC7CF			// 차,카,타,파,하
		};
		const WORD	aEnd[19] =
		{
			0xB1ED, 0xB3A9, 0xB4D8, 0xB5FA, 0xB6F2, 0xB8B5,
			0xB9D8, 0xBAFB, 0xBBE6, 0xBDCD, 0xBEC5, 0xC0D9, 0xC2A4,
			0xC2F6, 0xC4AA, 0xC5B7, 0xC6C3, 0xC7CE, 0xC8FE
		};

		int ii = 0;
		for (ii = 0; ii < 19; ii++)
		{
			if (ch == aInitial[ii])
			{
				start = aStart[ii];
				end = aEnd[ii];
				return true;
			}
		}

		start = WORD(aStart[ii] - 1);
		end = WORD(aEnd[ii] + 1);
		return true;
		//return false;
	}

	WORD makeMultiByte(char ch1, char ch2)	// make multibyte with 2 bytes
	{
		return WORD((ch1 << 8) | (0x00FF & ch2));
	}


	bool findWord(const CString& sfind, const CString& sListData)
	{
		//우선 초반 거르기를 위한 길이
		int len_sfind = sfind.GetLength();
		int len_sListData = sListData.GetLength();

		if (len_sfind == 0 || len_sfind > len_sListData)
			return false;

		//실제 오프셋을 전진시키면서  찾는..
		for (int offset = 0; offset <= len_sListData - len_sfind; sListData[offset] & 0x80 ? offset += 2 : offset++)
		{
			//내부에서는 찾을 문자열을 한글이든 비한글이든 전진시키면서 매칭이 되는지를 확인한다
			bool matched = true;
			int searchIdx = 0;
			while(searchIdx < len_sfind)
			{//한글이면 2바이트씩 오프셋 증가
				if(sfind[searchIdx] & 0x80   && sListData[offset + searchIdx] )
				{
					//한글 - 인덱스 초과하면 매칭 없음 로직
					if (searchIdx + 1 >= len_sListData || offset + searchIdx + 1 >= len_sListData)
					{
						matched = false;
						break;
					}

					//한글이니까 word 만큼 추출해서 
					WORD input = makeMultiByte(sfind.GetAt(searchIdx), sfind.GetAt(searchIdx + 1));
					WORD start = 0, end = 0;
					if (isInitialConsonant(input, start, end))  
					{//초성
						WORD wname = makeMultiByte(sListData.GetAt(offset + searchIdx), sListData.GetAt(offset + searchIdx + 1));
						if (!IS_WITHIN(start, end, wname))
						{
							matched = false;
							break;
						}
					}
					else
					{//초성 아니면
						WORD wfind = input;
						WORD wname = makeMultiByte(sListData.GetAt(offset + searchIdx), sListData.GetAt(offset + searchIdx + 1));
						if (wfind != wname)
						{
							matched = false;
							break;
						}
					}

					searchIdx = searchIdx + 2;
				 }
				else
				{
					if (sfind[searchIdx] != sListData[offset + searchIdx])
					{
						matched = false;
						break;
					}
					searchIdx += 1;
					searchIdx++;
				}
			}
			if (matched)
				return true;
		}
		return false;
	}

	void CTestDlgDlg::OnBnClickedMfcSearch()
	{
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		std::vector<CString> _vList;
		CFile	file;
		CString	string = AxStd::FORMAT("%s", "hjcode3.dat");
		if (AxStd::isFileExist(string))
		{
			if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
				return;

			const int  size = gsl::narrow_cast<int>(file.GetLength());
			m_hcB = std::make_unique<char[]>(size + 1);
			const int len = file.Read(m_hcB.get(), size);
			file.Close();

			if (size > len)
			{
				m_hcB.reset();
				return;
			}
			int ilen = sizeof(struct hjcodex);
			const int count = len / sizeof(struct hjcodex);
			struct	hjcodex* hjc = (struct hjcodex*)m_hcB.get();

			for (int ii = 0; ii < count; ii++, hjc++)
			{
				CString sCode = CString(hjc->code, HCodeLen).Mid(1);
				CString sName = CString(hjc->hnam, HNameLen);
				sName.Trim();
				_vList.push_back(sName);
			} //for
		} //if
		
		CString sFind = "ㄴㄷ";
		
		for (auto& vVal : _vList)
		{
			if (findWord(sFind, vVal))
			{
				m_slog.Format("\r\n %s", vVal);
				OutputDebugString(m_slog);
			} 
		}

	}


	void CTestDlgDlg::OnBnClickedDump()
	{
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	}
