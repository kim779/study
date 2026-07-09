// BaseMP.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "BaseMP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseMP

CBaseMP::CBaseMP()
{
	m_protect = false;
}

CBaseMP::~CBaseMP()
{
}


BEGIN_MESSAGE_MAP(CBaseMP, CWnd)
	//{{AFX_MSG_MAP(CBaseMP)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBaseMP message handlers

BOOL CBaseMP::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)	
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_TAB:
		case VK_RETURN:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
//			return TRUE;
		}
		break;

	case WM_CHAR:
		if (pMsg->hwnd == m_hWnd)
		{
			if (pMsg->wParam == 0x11)
			{
				m_protect = true;
				SetTimer(100, 2000, NULL);
			}
			if (!m_protect)
				SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			if (pMsg->wParam == 0x12)
			{
				m_protect = false;
				KillTimer(100);
			}
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CBaseMP::OnTimer(UINT_PTR nIDEvent) 
{
	switch (nIDEvent)
	{
	case 100:
		m_protect = false;
		KillTimer(100);
		break;
	default:
		break;
	}
	CWnd::OnTimer(nIDEvent);
}
