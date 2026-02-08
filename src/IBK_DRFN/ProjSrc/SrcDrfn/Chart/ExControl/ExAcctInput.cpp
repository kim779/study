// ExAcctInput.cpp : implementation file
//

#include "stdafx.h"
#include "../../Inc/ExAcctInput.h"
#include "../../Inc/ExOCXDLL.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCSTR CExAcctInput::CX2000BaseDesk::GetBasePath()
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	m_szPath = pThis->m_pBaseDesk2->GetBasePath();
	return m_szPath;
}

HWND  CExAcctInput::CX2000BaseDesk::GetDeskHwnd()
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)
	
	return pThis->m_pBaseDesk2->GetDeskHwnd();
}

LPDISPATCH CExAcctInput::CX2000BaseDesk::GetUnknown()
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetUnknown();
}

LPUNKNOWN CExAcctInput::CX2000BaseDesk::GetCtrlUnknown(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetCtrlUnknown(szName);
}

HWND CExAcctInput::CX2000BaseDesk::GetCtrlHwnd(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetCtrlHwnd(szName);
}

LPVOID CExAcctInput::CX2000BaseDesk::GetSharedInterface()
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetSharedInterface();
}

BOOL CExAcctInput::CX2000BaseDesk::InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->InvokeEvent(_szEventName, _szParam);
}

BOOL CExAcctInput::CX2000BaseDesk::InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->InvokeEvent(_szEventName, _szParam);
}
	
BOOL CExAcctInput::CX2000BaseDesk::SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl) 
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->SendKey(_hWnd, nChar, _bIsShift, _bIsCtrl);
}

LPVOID CExAcctInput::CX2000BaseDesk::GetLSharedData()
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetLSharedData();
}



///////////////////////////////////////////////////////////////////////////////
// 만든이            : 한동혁(Dong-Hyuk, Han)
// Email             : aviman@dooriic.co.kr
// 만든 날짜         : 2002/2/1
// 함수 이름         : CExAcctInput::CX2000BaseDesk::CallCommand
// 리턴 타입         : void
// 파라미터
//		LPCSTR _szCmd  : 
// 함수 설명         : 
///////////////////////////////////////////////////////////////////////////////
void CExAcctInput::CX2000BaseDesk::CallCommand(LPCSTR _szCmd)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	pThis->m_pBaseDesk2->CallCommand(_szCmd);
}

// * 사용자 컬러 정보를 리턴한다.
OLE_COLOR CExAcctInput::CX2000BaseDesk::GetUserColor(short nIndex, LPCTSTR szName, short nFlag)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetUserColor(nIndex, szName, nFlag);
}

// * 현재 아무것도 키값에 대한 정의가 되어있지 않으므로 널리턴.
// * 요구 또는 필요가 생길경우 여기에서 구현한다.
long CExAcctInput::CX2000BaseDesk::GetDeskInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetDeskInfo(szKey);
}

long CExAcctInput::CX2000BaseDesk::GetUserInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)
		
	return pThis->m_pBaseDesk2->GetUserInfo(szKey);
}

long CExAcctInput::CX2000BaseDesk::GetFrameInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)
		
	return pThis->m_pBaseDesk2->GetFrameInfo(szKey);
}

long CExAcctInput::CX2000BaseDesk::GetMainInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)
		
	return pThis->m_pBaseDesk2->GetMainInfo(szKey);
}




//* 수정 시작 (2002.8.22 001) 모니터링 툴 추가
BOOL CExAcctInput::CX2000BaseDesk::FP_vAdviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->FP_vAdviseDebug(sName, pMonitoringCommand);
}


void CExAcctInput::CX2000BaseDesk::FP_vUnadviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	pThis->m_pBaseDesk2->FP_vUnadviseDebug(sName, pMonitoringCommand);
}

LPCSTR CExAcctInput::CX2000BaseDesk::FP_sGetMyVBName(HWND hWnd)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->FP_sGetMyVBName(hWnd);
}

void CExAcctInput::CX2000BaseDesk::FP_vSendDebug(LPCSTR csSection, LPCSTR csKey, LPCSTR csValue)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	pThis->m_pBaseDesk2->FP_vSendDebug(csSection, csKey, csValue);
}
//* 수정 끝 (2002.8.22 001) 


// CControlPos의 m_awndControls 맴버변수의 포인터를 리턴한다.
// 이 변수는 리사이즈 될 컴포넌트의 정보를 들고 있다.
long CExAcctInput::CX2000BaseDesk::FP_nGetCtrlPos()
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->FP_nGetCtrlPos();
}


// 컴포넌트를 리사이즈 시킨다.
void CExAcctInput::CX2000BaseDesk::FP_vResizeCtrl()
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)
	
	pThis->m_pBaseDesk2->FP_vResizeCtrl();
}

// {{ OCX에서 DESK로 메세지 보낼 수 있도록 요청(김용식) 2003.05.19
void CExAcctInput::CX2000BaseDesk::SetMapMessage(long lType, LPCSTR lpData)
{
	MYMETHOD_PROLOGUE(CExAcctInput, 2000BaseDesk)

	pThis->m_pBaseDesk2->SetMapMessage(lType, lpData);
}
// }}

/////////////////////////////////////////////////////////////////////////////
// CExAcctInput

CExAcctInput::CExAcctInput()
{
	m_pLoader = new CLoaderAcctInput(this);
	m_bCreated = FALSE;
	m_pParent = NULL;
}

CExAcctInput::~CExAcctInput()
{
	delete m_pLoader;
}

/////////////////////////////////////////////////////////////////////////////
// CExAcctInput message handlers

BOOL CExAcctInput::InvokeEvent(LPCSTR lpszEventName, LPCSTR lpszParam)
{
	ST_CTRLEVENT stEvent;
	stEvent.lpszEventName = lpszEventName;
	stEvent.lpszParam = lpszParam;

	if(m_pParent)
		m_pParent->SendMessage(RMSG_EVENT_ACCTINPUT, (WPARAM)GetSafeHwnd(), (LPARAM)&stEvent);

	return TRUE;
}

void CExAcctInput::SetMyBaseDesk2(CBaseDesk2* pBaseDesk)
{
	m_pBaseDesk2 = (CX2000BaseDesk*)pBaseDesk;
	m_x2000BaseDesk.m_hDeskHwnd = m_pBaseDesk2->m_hDeskHwnd;
	m_x2000BaseDesk.m_pDeskUnk = m_pBaseDesk2->m_pDeskUnk;
	m_x2000BaseDesk.m_pDisp = m_pBaseDesk2->m_pDisp;
	m_x2000BaseDesk.m_szPath = m_pBaseDesk2->m_szPath;
}

BOOL CExAcctInput::InitAcctInput(CDialog* pParent, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);
	
	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitAcctInput(pParent, rcControl, nID, pListProperty, pBaseDesk);
}

BOOL CExAcctInput::InitAcctInput(CDialog* pParent, CRect& rcControl, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitAcctInput();
	
	if(pBaseDesk)
	{
		m_pParent = pParent;
		m_bCreated = TRUE;
		SetMyBaseDesk2(pBaseDesk);
	}
	else
	{
		m_pParent = pParent;

		while(!pParent->IsKindOf(RUNTIME_CLASS(CExOCXDLL)))
		{
			pParent = (CDialog*)pParent->GetParent();
			if(!pParent || !IsWindow(pParent->GetSafeHwnd()))
				return FALSE;
		}

		m_bCreated = TRUE;
		SetMyBaseDesk2(((CExOCXDLL*)pParent)->m_pBaseDesk);
	}
	
	LPVOID lpControl = m_pLoader->CreateAcctInput(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, pListProperty);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

BOOL CExAcctInput::InitAcctInput(CDialog* pParent, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);

	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitAcctInput(pParent, rcControl, nID, lpszProperties, nSize, pBaseDesk);
}

BOOL CExAcctInput::InitAcctInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitAcctInput();
	
	if(pBaseDesk)
	{
		m_pParent = pParent;
		m_bCreated = TRUE;
		SetMyBaseDesk2(pBaseDesk);
	}
	else
	{
		m_pParent = pParent;

		while(!pParent->IsKindOf(RUNTIME_CLASS(CExOCXDLL)))
		{
			pParent = (CDialog*)pParent->GetParent();
			if(!pParent || !IsWindow(pParent->GetSafeHwnd()))
				return FALSE;
		}

		m_bCreated = TRUE;
		SetMyBaseDesk2(((CExOCXDLL*)pParent)->m_pBaseDesk);
	}

	LPVOID lpControl = m_pLoader->CreateAcctInput(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, lpszProperties, nSize);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

BOOL CExAcctInput::InitAcctInput(CDialog* pParent, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);

	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitAcctInput(pParent, rcControl, nID, lpszProperties, pBaseDesk);
}

BOOL CExAcctInput::InitAcctInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitAcctInput();
	
	if(pBaseDesk)
	{
		m_pParent = pParent;
		m_bCreated = TRUE;
		SetMyBaseDesk2(pBaseDesk);
	}
	else
	{
		m_pParent = pParent;

		while(!pParent->IsKindOf(RUNTIME_CLASS(CExOCXDLL)))
		{
			pParent = (CDialog*)pParent->GetParent();
			if(!pParent || !IsWindow(pParent->GetSafeHwnd()))
				return FALSE;
		}

		m_bCreated = TRUE;
		SetMyBaseDesk2(((CExOCXDLL*)pParent)->m_pBaseDesk);
	}

	LPVOID lpControl = m_pLoader->CreateAcctInput(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, lpszProperties);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);
	
	return (BOOL)lpControl;
}

BOOL CExAcctInput::InitAcctInput(CDialog* pParent, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);

	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitAcctInput(pParent, rcControl, nID, nProperties, pBaseDesk);
}

BOOL CExAcctInput::InitAcctInput(CDialog* pParent, CRect& rcControl, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitAcctInput();
	
	if(pBaseDesk)
	{
		m_pParent = pParent;
		m_bCreated = TRUE;
		SetMyBaseDesk2(pBaseDesk);
	}
	else
	{
		m_pParent = pParent;

		while(!pParent->IsKindOf(RUNTIME_CLASS(CExOCXDLL)))
		{
			pParent = (CDialog*)pParent->GetParent();
			if(!pParent || !IsWindow(pParent->GetSafeHwnd()))
				return FALSE;
		}

		m_bCreated = TRUE;
		SetMyBaseDesk2(((CExOCXDLL*)pParent)->m_pBaseDesk);
	}

	LPVOID lpControl = m_pLoader->CreateAcctInput(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, nProperties);
	::SetPermanentHandle((HANDLE)m_pLoader->GetHWnd(), (CObject*)lpControl);
	
	return (BOOL)lpControl;
}

long CExAcctInput::GetBase(LPCTSTR lpszKey)
{
	return m_pLoader->GetBase(lpszKey);
}

void CExAcctInput::SetFocus()
{
	m_pLoader->SetFocus();
}

OLE_HANDLE CExAcctInput::GetHWnd()
{
	return m_pLoader->GetHWnd();
}

HWND CExAcctInput::GetSafeHwnd()
{
	return m_pLoader->GetSafeHwnd();
}

CString CExAcctInput::GetDataText()
{
	return m_pLoader->GetDataText();
}

CString CExAcctInput::GetText()
{
	return m_pLoader->GetText();
}

void CExAcctInput::SetText(LPCTSTR lpszText)
{
	m_pLoader->SetText(lpszText);
}

void CExAcctInput::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/)
{
	m_pLoader->MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CExAcctInput::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	m_pLoader->MoveWindow(lpRect, bRepaint);
}

BOOL CExAcctInput::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
{
	return m_pLoader->SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);
}

BOOL CExAcctInput::ShowWindow(int nCmdShow)
{
	return m_pLoader->ShowWindow(nCmdShow);
}

void CExAcctInput::GetWindowRect(LPRECT lpRect)
{
	m_pLoader->GetWindowRect(lpRect);
}

void CExAcctInput::GetClientRect(LPRECT lpRect)
{
	m_pLoader->GetClientRect(lpRect);
}

void CExAcctInput::SetEnabled(BOOL bEnable)
{
	m_pLoader->SetEnabled(bEnable);
}

BOOL CExAcctInput::GetEnabled()
{
	return m_pLoader->GetEnabled();
}
