// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Notify.h"
#include "ControlWnd.h"
#include "ShMemory.h"

#include "../../h/fxCommonx.h"
#include "../../h/ledger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TYPE_SIGNAL_SYS		145
#define TYPE_SIGNAL_ALERT	142
#define TYPE_SIGNAL_NEWS	161
#define TYPE_REAL_NEWS		162

typedef CWnd*(*_DLL_SetLibOpen)(CWnd* pwndMain,HINSTANCE hInstance,CWnd* pWndModuleWnd);
typedef void (*_DLL_SetLibClose)(CWnd* pWndTarget);
typedef int (*_DLL_SetSignal)(BOOL bAdvise,CWnd* pwndMain,CString strType,CString sPBID="",CString strKey="*");

_DLL_SetLibOpen SignalLib_Open;
_DLL_SetLibClose SignalLib_Close;
_DLL_SetSignal SignalLib_SetSignal;

CControlWnd::CControlWnd()
{
	EnableAutomation();
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


//=================================================================================================
int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hSignalMng = ::LoadLibrary("lib_signalmng.dll");

	if(m_hSignalMng)
	{
		SignalLib_Open = (_DLL_SetLibOpen)GetProcAddress(m_hSignalMng,"_DLL_SetLibOpen");
		SignalLib_Close = (_DLL_SetLibClose)GetProcAddress(m_hSignalMng,"_DLL_SetLibClose");
		SignalLib_SetSignal = (_DLL_SetSignal)GetProcAddress(m_hSignalMng,"_DLL_SetSignal");

		m_pWndDummy = SignalLib_Open(this,NULL,m_pParent);
	}
	else
	{
		AfxMessageBox("시그널 컨트럴 생성에 실패 했습니다.",MB_OK);
	}

	return 0;
}

void CControlWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		break;
	case DLL_OUB:
		break;
	case DLL_ALERT:
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
	}
	return 0;
}

void CControlWnd::OnPaint() 
{
}
