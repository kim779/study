// TransparentMgr.cpp: implementation of the CTransparentMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransparentMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef WS_EX_LAYERED 
#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1
#define LWA_ALPHA    2
#endif // WS_EX_LAYERED

static CMap <HWND, HWND, CTPInfo, CTPInfo> g_mapTPHwnd;
//BOOL (APIENTRY *SetLayeredWindowAttributes) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
BOOL(APIENTRY* SetLayer) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransparentMgr::CTransparentMgr()
{
	m_bAutoDelete = FALSE;
}

CTransparentMgr::~CTransparentMgr()
{
	
}

void CTransparentMgr::SetTPWnd(HWND hWnd, int nTime, BOOL bPlus, BOOL bDestroy)
{
	CTPInfo TpInfo;
	if (!bPlus)
		TpInfo.alpha = 0xff;
	if (g_mapTPHwnd.Lookup(hWnd, TpInfo))
	{
		
	}
	TpInfo.bPlus = bPlus;
	TpInfo.bDestroy = bDestroy;
	g_mapTPHwnd.SetAt(hWnd, TpInfo);

	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	::SetTimer(hWnd, (UINT)hWnd, nTime, OnTPTimer);
}

void CTransparentMgr::OnTPTimer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	CTPInfo TpInfo;
	if (g_mapTPHwnd.Lookup(hWnd, TpInfo))
	{
		HINSTANCE hUser32 = LoadLibrary(_T("USER32.DLL"));
		SetLayer = (BOOL (APIENTRY *) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags))GetProcAddress(hUser32, "SetLayeredWindowAttributes");
		if (!SetLayer)
		{
			KillTimer(hWnd, idEvent);
			return;
		}
		
		if (TpInfo.bPlus)
		{
			TpInfo.alpha += 10;
			if (TpInfo.alpha > 0xff)
				TpInfo.alpha = 0xff;
			g_mapTPHwnd.SetAt(hWnd, TpInfo);
			//SetLayeredWindowAttributes(hWnd, 0, TpInfo.alpha, LWA_ALPHA); //vc2019
			SetLayer(hWnd, 0, TpInfo.alpha, LWA_ALPHA);
			if (TpInfo.alpha == 0xff)
			{
				g_mapTPHwnd.RemoveKey(hWnd);
				if (TpInfo.bDestroy)
					::DestroyWindow(hWnd);
				KillTimer(hWnd, idEvent);
				SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
				RedrawWindow(hWnd, NULL, NULL, RDW_FRAME|RDW_INVALIDATE);
			}
		}
		else
		{
			TpInfo.alpha -= 10;
			if (TpInfo.alpha < 0)
				TpInfo.alpha = 0;
			g_mapTPHwnd.SetAt(hWnd, TpInfo);
			//SetLayeredWindowAttributes(hWnd, 0, TpInfo.alpha, LWA_ALPHA);
			SetLayer(hWnd, 0, TpInfo.alpha, LWA_ALPHA);
			if (TpInfo.alpha == 0)
			{
				g_mapTPHwnd.RemoveKey(hWnd);
				if (TpInfo.bDestroy)
					::DestroyWindow(hWnd);
				KillTimer(hWnd, idEvent);
			}
		}
		FreeLibrary(hUser32);
	}
	else
		KillTimer(hWnd, idEvent);
}
