// FixWaitCursor.cpp: implementation of the CFixWaitCursor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FixWaitCursor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define	WAIT_TIMER_ID	20000

CFixWaitCursor::CFixWaitCursor()
{
	m_nWaitCount = 0;
	m_pParentWnd = NULL;
	m_bIntelli = FALSE;
}

CFixWaitCursor::CFixWaitCursor(CWnd * pParentWnd)
{
	SetParentWnd(pParentWnd);	
}

CFixWaitCursor::~CFixWaitCursor()
{

}

BOOL CFixWaitCursor::BWaitCursor(UINT nRqIndex, CWnd * pFocusWnd /*=NULL*/, int nTime /*=120000*/)
{
	if(!m_pParentWnd) return FALSE;

	m_nRqIndexArray.Add(nRqIndex);

	m_pParentWnd->KillTimer(WAIT_TIMER_ID);
	
	if(nTime > 0)
	{
		if(!m_pParentWnd->SetTimer(WAIT_TIMER_ID, nTime, NULL))
		{
			TRACE("Fail SetTimer\n");
		}
	}
	
	m_nWaitCount++;

	if(!pFocusWnd)
	{
		CPoint pt;
		::GetCursorPos(&pt);	
		pFocusWnd = CWnd::WindowFromPoint(pt);
	}
	if(pFocusWnd->GetSafeHwnd() == m_pParentWnd->GetSafeHwnd() || ::IsChild(m_pParentWnd->GetSafeHwnd(),pFocusWnd->GetSafeHwnd()))
	{
		::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	}

	//해당 창이 DISABLE로 되어 있으면 마우스관련 메세지를 받을 수 없으므로 대기커서를 변경할 수 없다.
	m_pParentWnd->EnableWindow(TRUE);

	return TRUE;
}

BOOL CFixWaitCursor::BWaitCursorIntelli(CWnd * pFocusWnd /*=NULL*/, int nTime /*=120000*/)
{
	if(!m_pParentWnd) return FALSE;

	m_pParentWnd->KillTimer(WAIT_TIMER_ID);
	
	if(nTime > 0)
	{
		if(!m_pParentWnd->SetTimer(WAIT_TIMER_ID, nTime, NULL))
		{
			TRACE("Fail SetTimer\n");
		}
	}
	
	m_nWaitCount++;

	m_bIntelli = TRUE;

	if(!pFocusWnd)
	{
		CPoint pt;
		::GetCursorPos(&pt);	
		pFocusWnd = CWnd::WindowFromPoint(pt);
	}
	if(pFocusWnd->GetSafeHwnd() == m_pParentWnd->GetSafeHwnd() || ::IsChild(m_pParentWnd->GetSafeHwnd(),pFocusWnd->GetSafeHwnd()))
	{
		::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	}

	//해당 창이 DISABLE로 되어 있으면 마우스관련 메세지를 받을 수 없으므로 대기커서를 변경할 수 없다.
	m_pParentWnd->EnableWindow(TRUE);

	return TRUE;
}

BOOL CFixWaitCursor::EWaitCursor(UINT nReceiveIndex, BOOL bTimerEnd /* = FALSE */)
{
	if(!m_pParentWnd) return FALSE;

	int i;
	for( i = 0 ; i < m_nRqIndexArray.GetSize(); i++ )
	{
		if(m_nRqIndexArray.GetAt(i) == (UINT)nReceiveIndex)
			break;	
	}	
	if(i >= m_nRqIndexArray.GetSize())
		return FALSE;

	m_nRqIndexArray.RemoveAt(i);

	//일반 호출..
	if(!bTimerEnd)
	{	
		m_nWaitCount--;	
		if(m_nWaitCount <= 0)
		{	
			m_nWaitCount = 0;
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));	
			m_pParentWnd->KillTimer(WAIT_TIMER_ID);
			
			//대기커서가 풀리는 시점을 해당 윈도우에게 알린다.
			m_pParentWnd->SendMessage(WMU_EWAIT_CURSOR);
		}
	}
	else
	{//타임아웃 호출 (강제 호출이다.)

		m_nWaitCount = 0;
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		m_pParentWnd->KillTimer(WAIT_TIMER_ID);

		//대기커서가 풀리는 시점을 해당 윈도우에게 알린다.
		m_pParentWnd->SendMessage(WMU_EWAIT_CURSOR);
	}
	return TRUE;
}

BOOL CFixWaitCursor::EWaitCursor(BOOL bTimerEnd /* = FALSE */)
{
	if(!m_pParentWnd) return FALSE;
	
	m_nRqIndexArray.RemoveAll();

	//일반 호출..
	if(!bTimerEnd)
	{	
		m_nWaitCount--;	
		if(m_nWaitCount <= 0)
		{	
			m_nWaitCount = 0;
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));	
			m_pParentWnd->KillTimer(WAIT_TIMER_ID);
			
			//대기커서가 풀리는 시점을 해당 윈도우에게 알린다.
			m_pParentWnd->SendMessage(WMU_EWAIT_CURSOR);
		}
	}
	else
	{//타임아웃 호출 (강제 호출이다.)

		m_nWaitCount = 0;
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		m_pParentWnd->KillTimer(WAIT_TIMER_ID);

		//대기커서가 풀리는 시점을 해당 윈도우에게 알린다.
		m_pParentWnd->SendMessage(WMU_EWAIT_CURSOR);
	}
	return TRUE;
}

BOOL CFixWaitCursor::WaitCursorProc(MSG* pMsg)
{
	if(!m_pParentWnd) return FALSE;

	if(m_nWaitCount > 0 && ( pMsg->hwnd == m_pParentWnd->GetSafeHwnd() || ::IsChild(m_pParentWnd->GetSafeHwnd(), pMsg->hwnd) ))
	{
		if(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
		{
			::SetCursor(::LoadCursor(NULL, IDC_WAIT));
			return TRUE;
		}
		else if(pMsg->message == WM_LBUTTONDOWN	|| pMsg->message == WM_RBUTTONDOWN		||
			pMsg->message == WM_LBUTTONDBLCLK	|| pMsg->message ==	WM_RBUTTONDBLCLK	||
			pMsg->message == WM_LBUTTONUP		|| pMsg->message == WM_RBUTTONUP		||
			pMsg->message == WM_MOUSEMOVE		||
			pMsg->message == WM_NCLBUTTONDOWN	|| pMsg->message == WM_NCRBUTTONDOWN	||
			pMsg->message == WM_NCLBUTTONDBLCLK	|| pMsg->message ==	WM_NCRBUTTONDBLCLK	||
			pMsg->message == WM_NCLBUTTONUP		|| pMsg->message == WM_NCRBUTTONUP		||
			pMsg->message == WM_NCMOUSEMOVE		)
		{
			::SetCursor(LoadCursor(NULL, IDC_WAIT));
			return TRUE;
		}
		
	}

	return FALSE;
}

void CFixWaitCursor::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == WAIT_TIMER_ID)
		EWaitCursor();
}