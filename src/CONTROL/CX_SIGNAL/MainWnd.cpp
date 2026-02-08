// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_SIGNAL.h"
#include "MainWnd.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MainWnd
#define SIGNAL_NAME_LEN		40
#define SIGNAL_DEGREE_LEN	2
#define IMG_INDEX_LEN		2
#define TIME_LEN		6
#define DATE_LEN		8
#define CODE_LEN		6
#define CODE_NAME_LEN		40

#define TYPE_SIGNAL_SYS		145
#define TYPE_SIGNAL_ALERT	142
#define TYPE_SIGNAL_NEWS	161
#define TYPE_REAL_NEWS		162

#define WM_GET_BROD		WM_USER + 703

typedef CWnd*(*_DLL_SetLibOpen)(CWnd* pwndMain,HINSTANCE hInstance,CWnd* pWndModuleWnd);
typedef void (*_DLL_SetLibClose)(CWnd* pWndTarget);
typedef int (*_DLL_SetSignal)(BOOL bAdvise, CWnd* pwndMain, CString strType, CString sPBID, CString strKey);
//typedef int (*_DLL_SetSignal)(BOOL bAdvise,CWnd* pwndMain,CString strType,CString sPBID="",CString strKey="*");

_DLL_SetLibOpen  SignalLib_Open;
_DLL_SetLibClose SignalLib_Close;
_DLL_SetSignal   SignalLib_SetSignal;

typedef struct stRealData
{
	BYTE	chCount;
	char	szKeyCode[4];
	int	nSize;
	LPSTR	pDataBuf;
	int	lRealStruct;
	BYTE	chRealType;
} ST_REALDATA;

typedef struct tagSIGNAL_COMMON
{
	char	sMarket[1];
	char	sCode[CODE_LEN];
	char	sPrice[8];
	char	sSign;
	char	sChange[8];
	char	sChgRate[6];
	char	sVolume[9];
	char    sVolumeRect[6];
	char	sResultField[1040];
} SIGNAL_COMMON;

typedef struct tagSIGNAL_CONDITION
{
	char	sCode[CODE_LEN];
	char	sName[20];
	char	sJobFlag[1];
	char    sEntryCnt[4];
	char	sDate[DATE_LEN];
	char	sTime[TIME_LEN];
	char	sSignalImageIndex[IMG_INDEX_LEN];
	char	sSignalDegree[SIGNAL_DEGREE_LEN];
	char	sOrderType[1];
	char	sSignalKey[4];
	char	sSignalName[SIGNAL_NAME_LEN];
	SIGNAL_COMMON	stCommon;
} SIGNAL_CONDITION;

#define SZ_SIGNAL_CONDITION	sizeof(SIGNAL_CONDITION);

MainWnd::MainWnd()
{
	EnableAutomation();

	m_pParent = nullptr;
	m_hSignalMng = nullptr;
	m_pWndDummy = nullptr;
}

MainWnd::~MainWnd()
{
}

void MainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(MainWnd, CWnd)
	//{{AFX_MSG_MAP(MainWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER,OnMessage)
	ON_MESSAGE(WM_GET_BROD,OnGetBroad)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(MainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(MainWnd)
	DISP_PROPERTY_NOTIFY(MainWnd, "DataList", m_dataList, OnDataListChanged, VT_BSTR)
	DISP_FUNCTION(MainWnd, "SetSignal", SetSignal, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {075ECF07-EBC0-4E6A-AC49-B6775BC55978}
static const IID IID_IMainWnd =
{ 0x75ecf07, 0xebc0, 0x4e6a, { 0xac, 0x49, 0xb6, 0x77, 0x5b, 0xc5, 0x59, 0x78 } };

BEGIN_INTERFACE_MAP(MainWnd, CWnd)
	INTERFACE_PART(MainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MainWnd message handlers

int MainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_arSymbol.RemoveAll();
	m_hSignalMng = ::LoadLibrary("lib_signalmng.dll");
	
	if(m_hSignalMng)
	{
		SignalLib_Open = (_DLL_SetLibOpen)GetProcAddress(m_hSignalMng,"_DLL_SetLibOpen");
		SignalLib_Close = (_DLL_SetLibClose)GetProcAddress(m_hSignalMng,"_DLL_SetLibClose");
		SignalLib_SetSignal = (_DLL_SetSignal)GetProcAddress(m_hSignalMng,"_DLL_SetSiganl");
		
		m_pWndDummy = SignalLib_Open(this,NULL,m_pParent);

		if(m_pWndDummy)
		{
			AfxMessageBox("SIGNAL OPEN SUCCESS!");
		}
		else
		{
			AfxMessageBox("SIGNAL OPEN FAIL!");
		}
	}
	else
	{
		AfxMessageBox("시그널 컨트럴 생성에 실패 했습니다.",MB_OK);
	}

	CString file, root, strNm;
	char	buff[128]{};

	root = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), (LPARAM)0);
	file.Format("%s\\tab\\SIGNAL.INI", root);

	const	int	nCount = GetPrivateProfileInt("SIGNAL", "COUNT", 0, file);
	for (int ii = 0; ii < nCount; ii++)
	{
		strNm.Format("%02d",ii);
		GetPrivateProfileString("SIGNAL", strNm, "", buff, sizeof(buff), file);

		m_arSymbol.Add(buff);
	}
	
	return 0;
}

long MainWnd::OnMessage( WPARAM wParam,LPARAM lParam )
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		break;
	case DLL_TRIGGER:	// Remain Data Send to Map!
		{
			
		}
		break;
	case DLL_DOMINO:
		break;
	case DLL_SETFONT:
		{
		}
		break;
	case DLL_NOTICE:
		break;
	default:break;
	}
	return 0;
}

void MainWnd::SetSignal(LPCTSTR symbol) 
{
	if(!m_hSignalMng)
		return;

	if (symbol == "A")
	{
		CString strRealType;

		strRealType.Format("%d", (int)TYPE_SIGNAL_ALERT);
		for (int ii = 0; ii < m_arSymbol.GetSize(); ii++)
		{
			SignalLib_SetSignal(TRUE, this, strRealType, "", m_arSymbol.GetAt(ii));
		}
	}
	else
	{
		CString strRealType;

		strRealType.Format("%d",(int)TYPE_SIGNAL_ALERT);
		SignalLib_SetSignal(TRUE, this, strRealType, "", symbol);

		m_arSymbol.Add(symbol);
	}
}

long MainWnd::OnGetBroad( WPARAM wParam,LPARAM lParam )
{
	ST_REALDATA* pstRealData = (ST_REALDATA*)lParam;
	SIGNAL_CONDITION *pstSIGNAL_CONDITION = (SIGNAL_CONDITION*)pstRealData->pDataBuf;

	m_dataList.Format("%s", (char*)pstSIGNAL_CONDITION);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnDblClk/*DblClick*/)),
				(LPARAM)m_param.name.GetString());
	return 0L;
}

void MainWnd::SetParam(_param *pParam)
{
	m_param.key     = pParam->key;
	m_param.name    = CString(pParam->name, pParam->name.GetLength());
	m_param.rect    = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts   = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point   = pParam->point;
	m_param.style   = pParam->style;
	m_param.tRGB    = pParam->tRGB;
	m_param.pRGB    = pParam->pRGB;
	m_param.options = CString(pParam->options, pParam->options.GetLength());
}

void MainWnd::OnDataListChanged() 
{
	// TODO: Add notification handler code

}

void MainWnd::OnClose() 
{
	CString strRealType;

	strRealType.Format("%d", (int)TYPE_SIGNAL_ALERT);
	for (int ii = 0; ii < m_arSymbol.GetSize(); ii++)
	{
		SignalLib_SetSignal(FALSE, this, strRealType, "", m_arSymbol.GetAt(ii));
	}

	SignalLib_Close(m_pWndDummy);
	CWnd::OnClose();
}
