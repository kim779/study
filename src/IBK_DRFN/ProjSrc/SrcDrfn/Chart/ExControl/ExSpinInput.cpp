// ExSpinInput.cpp : implementation file
//

#include "stdafx.h"
#include "../../Inc/ExSpinInput.h"
#include "../../Inc/ExOCXDLL.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCSTR CExSpinInput::CX2000BaseDesk::GetBasePath()
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
	m_szPath = pThis->m_pBaseDesk2->GetBasePath();
	return m_szPath;
}

HWND  CExSpinInput::CX2000BaseDesk::GetDeskHwnd()
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->GetDeskHwnd();
}

LPDISPATCH CExSpinInput::CX2000BaseDesk::GetUnknown()
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->GetUnknown();
}

LPUNKNOWN CExSpinInput::CX2000BaseDesk::GetCtrlUnknown(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->GetCtrlUnknown(szName);
}

HWND CExSpinInput::CX2000BaseDesk::GetCtrlHwnd(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->GetCtrlHwnd(szName);
}

LPVOID CExSpinInput::CX2000BaseDesk::GetSharedInterface()
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->GetSharedInterface();
}

BOOL CExSpinInput::CX2000BaseDesk::InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->InvokeEvent(_szEventName, _szParam);
}

BOOL CExSpinInput::CX2000BaseDesk::InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->InvokeEvent(_szEventName, _szParam);
}

BOOL CExSpinInput::CX2000BaseDesk::SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl) 
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->SendKey(_hWnd, nChar, _bIsShift, _bIsCtrl);
}

LPVOID CExSpinInput::CX2000BaseDesk::GetLSharedData()
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->GetLSharedData();
}



///////////////////////////////////////////////////////////////////////////////
// 만든이            : 한동혁(Dong-Hyuk, Han)
// Email             : aviman@dooriic.co.kr
// 만든 날짜         : 2002/2/1
// 함수 이름         : CExSpinInput::CX2000BaseDesk::CallCommand
// 리턴 타입         : void
// 파라미터
//		LPCSTR _szCmd  : 
// 함수 설명         : 
///////////////////////////////////////////////////////////////////////////////
void CExSpinInput::CX2000BaseDesk::CallCommand(LPCSTR _szCmd)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		pThis->m_pBaseDesk2->CallCommand(_szCmd);
}

// * 사용자 컬러 정보를 리턴한다.
OLE_COLOR CExSpinInput::CX2000BaseDesk::GetUserColor(short nIndex, LPCTSTR szName, short nFlag)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->GetUserColor(nIndex, szName, nFlag);
}

// * 현재 아무것도 키값에 대한 정의가 되어있지 않으므로 널리턴.
// * 요구 또는 필요가 생길경우 여기에서 구현한다.
long CExSpinInput::CX2000BaseDesk::GetDeskInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->GetDeskInfo(szKey);
}

long CExSpinInput::CX2000BaseDesk::GetUserInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->GetUserInfo(szKey);
}

long CExSpinInput::CX2000BaseDesk::GetFrameInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->GetFrameInfo(szKey);
}

long CExSpinInput::CX2000BaseDesk::GetMainInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->GetMainInfo(szKey);
}




//* 수정 시작 (2002.8.22 001) 모니터링 툴 추가
BOOL CExSpinInput::CX2000BaseDesk::FP_vAdviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->FP_vAdviseDebug(sName, pMonitoringCommand);
}


void CExSpinInput::CX2000BaseDesk::FP_vUnadviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		pThis->m_pBaseDesk2->FP_vUnadviseDebug(sName, pMonitoringCommand);
}

LPCSTR CExSpinInput::CX2000BaseDesk::FP_sGetMyVBName(HWND hWnd)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->FP_sGetMyVBName(hWnd);
}

void CExSpinInput::CX2000BaseDesk::FP_vSendDebug(LPCSTR csSection, LPCSTR csKey, LPCSTR csValue)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		pThis->m_pBaseDesk2->FP_vSendDebug(csSection, csKey, csValue);
}
//* 수정 끝 (2002.8.22 001) 


// CControlPos의 m_awndControls 맴버변수의 포인터를 리턴한다.
// 이 변수는 리사이즈 될 컴포넌트의 정보를 들고 있다.
long CExSpinInput::CX2000BaseDesk::FP_nGetCtrlPos()
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		return pThis->m_pBaseDesk2->FP_nGetCtrlPos();
}


// 컴포넌트를 리사이즈 시킨다.
void CExSpinInput::CX2000BaseDesk::FP_vResizeCtrl()
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		pThis->m_pBaseDesk2->FP_vResizeCtrl();
}

// {{ OCX에서 DESK로 메세지 보낼 수 있도록 요청(김용식) 2003.05.19
void CExSpinInput::CX2000BaseDesk::SetMapMessage(long lType, LPCSTR lpData)
{
	MYMETHOD_PROLOGUE(CExSpinInput, 2000BaseDesk)
		
		pThis->m_pBaseDesk2->SetMapMessage(lType, lpData);
}
// }}

/////////////////////////////////////////////////////////////////////////////
// CExSpinInput

CExSpinInput::CExSpinInput()
{
	m_pLoader = new CLoaderSpinInput(this);
	m_bCreated = FALSE;
	m_pParent = NULL;
}

CExSpinInput::~CExSpinInput()
{
	delete m_pLoader;
}

/////////////////////////////////////////////////////////////////////////////
// CExSpinInput message handlers
BOOL CExSpinInput::InitSpinInput(CDialog* pParent, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);
	
	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitSpinInput(pParent, rcControl, nID, pListProperty, pBaseDesk);
}

BOOL CExSpinInput::InitSpinInput(CDialog* pParent, CRect& rcControl, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitSpinInput();
	
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
	
	LPVOID lpControl = m_pLoader->CreateSpinInput(m_pParent, &rcControl, (long)&m_x2000BaseDesk, nID, pListProperty);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

BOOL CExSpinInput::InitSpinInput(CDialog* pParent, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);
	
	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitSpinInput(pParent, rcControl, nID, lpszProperties, nSize, pBaseDesk);
}

BOOL CExSpinInput::InitSpinInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitSpinInput();
	
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
	
	LPVOID lpControl = m_pLoader->CreateSpinInput(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, lpszProperties, nSize);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

BOOL CExSpinInput::InitSpinInput(CDialog* pParent, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);
	
	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitSpinInput(pParent, rcControl, nID, lpszProperties, pBaseDesk);
}

BOOL CExSpinInput::InitSpinInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitSpinInput();
	
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
	
	LPVOID lpControl = m_pLoader->CreateSpinInput(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, lpszProperties);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

BOOL CExSpinInput::InitSpinInput(CDialog* pParent, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);
	
	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitSpinInput(pParent, rcControl, nID, nProperties, pBaseDesk);
}

BOOL CExSpinInput::InitSpinInput(CDialog* pParent, CRect& rcControl, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitSpinInput();
	
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
	
	LPVOID lpControl = m_pLoader->CreateSpinInput(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, nProperties);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

BOOL CExSpinInput::InvokeEvent(LPCSTR lpszEventName, LPCSTR lpszParam)
{
	ST_CTRLEVENT stEvent;
	stEvent.lpszEventName = lpszEventName;
	stEvent.lpszParam = lpszParam;

	m_pParent->SendMessage(RMSG_EVENT_SPININPUT, (WPARAM)GetSafeHwnd(), (LPARAM)&stEvent);
	return TRUE;
}

void CExSpinInput::SetMyBaseDesk2(CBaseDesk2* pBaseDesk)
{
	m_pBaseDesk2 = (CX2000BaseDesk*)pBaseDesk;
	m_x2000BaseDesk.m_hDeskHwnd = m_pBaseDesk2->m_hDeskHwnd;
	m_x2000BaseDesk.m_pDeskUnk = m_pBaseDesk2->m_pDeskUnk;
	m_x2000BaseDesk.m_pDisp = m_pBaseDesk2->m_pDisp;
	m_x2000BaseDesk.m_szPath = m_pBaseDesk2->m_szPath;
}

void CExSpinInput::SetInitValue(long lValue)
{
	m_pLoader->SetInitValue(lValue);
}

long CExSpinInput::GetInitValue()
{
	return m_pLoader->GetInitValue();
}

void CExSpinInput::SetDataText(LPCTSTR lpszText)
{
	m_pLoader->SetDataText(lpszText);
}

CString CExSpinInput::GetData()
{
	return m_pLoader->GetData();
}

void CExSpinInput::SetEnabled(BOOL bEnable)
{
	m_pLoader->SetEnabled(bEnable);
}

BOOL CExSpinInput::GetEnabled()
{
	return m_pLoader->GetEnabled();
}

void CExSpinInput::SetVariation(long lValue)
{
	m_pLoader->SetVariation(lValue);
}

long CExSpinInput::GetVariation()
{
	return m_pLoader->GetVariation();
}

long CExSpinInput::GetBase(LPCTSTR lpszKey)
{
	return m_pLoader->GetBase(lpszKey);
}

void CExSpinInput::SetFocus()
{
	m_pLoader->SetFocus();
}

OLE_HANDLE CExSpinInput::GetHWnd()
{
	return m_pLoader->GetHWnd();
}

HWND CExSpinInput::GetSafeHwnd()
{
	return m_pLoader->GetSafeHwnd();
}

void CExSpinInput::SetRange(int nMin, int nMax)
{
	m_pLoader->SetRange(nMin, nMax);
}

void CExSpinInput::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/)
{
	m_pLoader->MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CExSpinInput::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	m_pLoader->MoveWindow(lpRect, bRepaint);
}

BOOL CExSpinInput::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
{
	return m_pLoader->SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);
}

BOOL CExSpinInput::ShowWindow(int nCmdShow)
{
	return m_pLoader->ShowWindow(nCmdShow);
}

void CExSpinInput::GetWindowRect(LPRECT lpRect)
{
	m_pLoader->GetWindowRect(lpRect);
}

void CExSpinInput::GetClientRect(LPRECT lpRect)
{
	m_pLoader->GetClientRect(lpRect);
}

void CExSpinInput::ClearEditText()
{
	m_pLoader->ClearEditText();
}