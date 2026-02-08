// ExCodeInput.cpp : implementation file
//

#include "stdafx.h"
#include "../../Inc/ExCodeInput.h"
#include "../../Inc/ExOCXDLL.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCSTR CExCodeInput::CX2000BaseDesk::GetBasePath()
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	//m_szPath = pThis->m_pBaseDesk2->GetBasePath();
	m_szPath = "D:\\Prj_동부증권";
	return m_szPath;
}

HWND  CExCodeInput::CX2000BaseDesk::GetDeskHwnd()
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return NULL;
//	return pThis->m_pBaseDesk2->GetDeskHwnd();
}

LPDISPATCH CExCodeInput::CX2000BaseDesk::GetUnknown()
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetUnknown();
}

LPUNKNOWN CExCodeInput::CX2000BaseDesk::GetCtrlUnknown(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetCtrlUnknown(szName);
}

HWND CExCodeInput::CX2000BaseDesk::GetCtrlHwnd(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetCtrlHwnd(szName);
}

LPVOID CExCodeInput::CX2000BaseDesk::GetSharedInterface()
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return NULL;
	//return pThis->m_pBaseDesk2->GetSharedInterface();
}

BOOL CExCodeInput::CX2000BaseDesk::InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->InvokeEvent(_szEventName, _szParam);
}

BOOL CExCodeInput::CX2000BaseDesk::InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->InvokeEvent(_szEventName, _szParam);
}
	
BOOL CExCodeInput::CX2000BaseDesk::SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl) 
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->SendKey(_hWnd, nChar, _bIsShift, _bIsCtrl);
}

LPVOID CExCodeInput::CX2000BaseDesk::GetLSharedData()
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetLSharedData();
}



///////////////////////////////////////////////////////////////////////////////
// 만든이            : 한동혁(Dong-Hyuk, Han)
// Email             : aviman@dooriic.co.kr
// 만든 날짜         : 2002/2/1
// 함수 이름         : CExCodeInput::CX2000BaseDesk::CallCommand
// 리턴 타입         : void
// 파라미터
//		LPCSTR _szCmd  : 
// 함수 설명         : 
///////////////////////////////////////////////////////////////////////////////
void CExCodeInput::CX2000BaseDesk::CallCommand(LPCSTR _szCmd)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	pThis->m_pBaseDesk2->CallCommand(_szCmd);
}

// * 사용자 컬러 정보를 리턴한다.
OLE_COLOR CExCodeInput::CX2000BaseDesk::GetUserColor(short nIndex, LPCTSTR szName, short nFlag)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetUserColor(nIndex, szName, nFlag);
}

// * 현재 아무것도 키값에 대한 정의가 되어있지 않으므로 널리턴.
// * 요구 또는 필요가 생길경우 여기에서 구현한다.
long CExCodeInput::CX2000BaseDesk::GetDeskInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->GetDeskInfo(szKey);
}

long CExCodeInput::CX2000BaseDesk::GetUserInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)
	
	return 0;
	//return pThis->m_pBaseDesk2->GetUserInfo(szKey);
}

long CExCodeInput::CX2000BaseDesk::GetFrameInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return 0;
	//return pThis->m_pBaseDesk2->GetFrameInfo(szKey);
}

long CExCodeInput::CX2000BaseDesk::GetMainInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)
		
	return pThis->m_pBaseDesk2->GetMainInfo(szKey);
}




//* 수정 시작 (2002.8.22 001) 모니터링 툴 추가
BOOL CExCodeInput::CX2000BaseDesk::FP_vAdviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->FP_vAdviseDebug(sName, pMonitoringCommand);
}


void CExCodeInput::CX2000BaseDesk::FP_vUnadviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	pThis->m_pBaseDesk2->FP_vUnadviseDebug(sName, pMonitoringCommand);
}

LPCSTR CExCodeInput::CX2000BaseDesk::FP_sGetMyVBName(HWND hWnd)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->FP_sGetMyVBName(hWnd);
}

void CExCodeInput::CX2000BaseDesk::FP_vSendDebug(LPCSTR csSection, LPCSTR csKey, LPCSTR csValue)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	pThis->m_pBaseDesk2->FP_vSendDebug(csSection, csKey, csValue);
}
//* 수정 끝 (2002.8.22 001) 


// CControlPos의 m_awndControls 맴버변수의 포인터를 리턴한다.
// 이 변수는 리사이즈 될 컴포넌트의 정보를 들고 있다.
long CExCodeInput::CX2000BaseDesk::FP_nGetCtrlPos()
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	return pThis->m_pBaseDesk2->FP_nGetCtrlPos();
}


// 컴포넌트를 리사이즈 시킨다.
void CExCodeInput::CX2000BaseDesk::FP_vResizeCtrl()
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)
	
	pThis->m_pBaseDesk2->FP_vResizeCtrl();
}

// {{ OCX에서 DESK로 메세지 보낼 수 있도록 요청(김용식) 2003.05.19
void CExCodeInput::CX2000BaseDesk::SetMapMessage(long lType, LPCSTR lpData)
{
	MYMETHOD_PROLOGUE(CExCodeInput, 2000BaseDesk)

	pThis->m_pBaseDesk2->SetMapMessage(lType, lpData);
}
// }}

/////////////////////////////////////////////////////////////////////////////
// CExCodeInput

CExCodeInput::CExCodeInput()
{
	m_pLoader = new CLoaderCodeInput(this);
	m_bCreated = FALSE;
	m_pParent = NULL;
}

CExCodeInput::~CExCodeInput()
{
	delete m_pLoader;
}

/////////////////////////////////////////////////////////////////////////////
// CExCodeInput message handlers
BOOL CExCodeInput::InitCodeInput(CDialog* pParent, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);
	
	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitCodeInput(pParent, rcControl, nID, pListProperty, pBaseDesk);
}

BOOL CExCodeInput::InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitCodeInput();
	
	if(pBaseDesk)
	{
		m_pParent = pParent;
		m_bCreated = TRUE;
		SetMyBaseDesk2(pBaseDesk);
	}
	else
	{
		m_pParent = pParent;

//		while(!pParent->IsKindOf(RUNTIME_CLASS(CExOCXDLL)))
//		{
//			pParent = (CDialog*)pParent->GetParent();
//			if(!pParent || !IsWindow(pParent->GetSafeHwnd()))
//				return FALSE;
//		}

		m_bCreated = TRUE;
//		SetMyBaseDesk2(((CExOCXDLL*)pParent)->m_pBaseDesk);
	}

	LPVOID lpControl = m_pLoader->CreateCodeInput(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, pListProperty);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

BOOL CExCodeInput::InitCodeInput(CDialog* pParent, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);
	
	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitCodeInput(pParent, rcControl, nID, lpszProperties, nSize, pBaseDesk);
}

BOOL CExCodeInput::InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitCodeInput();
	
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
	
	LPVOID lpControl = m_pLoader->CreateCodeInput(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, lpszProperties, nSize);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

BOOL CExCodeInput::InitCodeInput(CDialog* pParent, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);
	
	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitCodeInput(pParent, rcControl, nID, lpszProperties, pBaseDesk);
}

BOOL CExCodeInput::InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitCodeInput();
	
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
	
	LPVOID lpControl = m_pLoader->CreateCodeInput(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, lpszProperties);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

BOOL CExCodeInput::InitCodeInput(CDialog* pParent, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	CRect rcControl;
	pParent->GetDlgItem(nID)->GetWindowRect(rcControl);
	pParent->ScreenToClient(rcControl);
	
	pParent->GetDlgItem(nID)->ShowWindow(SW_HIDE);

	return InitCodeInput(pParent, rcControl, nID, nProperties, pBaseDesk);
}

BOOL CExCodeInput::InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk/* = NULL*/)
{
	if(m_bCreated)
		m_pLoader->ExitCodeInput();
	
	if(pBaseDesk)
	{
		m_pParent = pParent;
		m_bCreated = TRUE;
		SetMyBaseDesk2(pBaseDesk);
	}
	else
	{
		m_pParent = pParent;

//		while(!pParent->IsKindOf(RUNTIME_CLASS(CExOCXDLL)))
//		{
//			pParent = (CDialog*)pParent->GetParent();
//			if(!pParent || !IsWindow(pParent->GetSafeHwnd()))
//				return FALSE;
//		}

		m_bCreated = TRUE;
//		SetMyBaseDesk2(((CExOCXDLL*)pParent)->m_pBaseDesk);
	}

	LPVOID lpControl = m_pLoader->CreateCodeInput(m_pParent, &rcControl, nID, (long)&m_x2000BaseDesk, nProperties);
	::SetPermanentHandle((HANDLE)m_pLoader->GetSafeHwnd(), (CObject*)lpControl);

	return (BOOL)lpControl;
}

void CExCodeInput::SetMyBaseDesk2(CBaseDesk2* pBaseDesk)
{
	m_pBaseDesk2 = (CX2000BaseDesk*)pBaseDesk;
	m_x2000BaseDesk.m_hDeskHwnd = m_pBaseDesk2->m_hDeskHwnd;
	m_x2000BaseDesk.m_pDeskUnk = m_pBaseDesk2->m_pDeskUnk;
	m_x2000BaseDesk.m_pDisp = m_pBaseDesk2->m_pDisp;
	m_x2000BaseDesk.m_szPath = m_pBaseDesk2->m_szPath;
}

BOOL CExCodeInput::InvokeEvent(LPCSTR lpszEventName, LPCSTR lpszParam)
{
	ST_CTRLEVENT stEvent;
	stEvent.lpszEventName = lpszEventName;
	stEvent.lpszParam = lpszParam;

	m_pParent->SendMessage(RMSG_EVENT_CODEINPUT, (WPARAM)GetSafeHwnd(), (LPARAM)&stEvent);
	return TRUE;
}

short CExCodeInput::GetGubunFromCode(LPCTSTR szCode)
{
	return m_pLoader->GetGubunFromCode(szCode);
}

void CExCodeInput::SetDataText(LPCTSTR szText)
{
	m_pLoader->SetDataText(szText);
}

void CExCodeInput::SetDataText2(LPCTSTR szCode)
{
	m_pLoader->SetDataText2(szCode);
}

CString CExCodeInput::GetDataText()
{
	return m_pLoader->GetDataText();
}

long CExCodeInput::GetBase(LPCTSTR lpszKey)
{
	return m_pLoader->GetBase(lpszKey);
}

void CExCodeInput::SetEnabled(BOOL bEnable)
{
	m_pLoader->SetEnabled(bEnable);
}

BOOL CExCodeInput::GetEnabled()
{
	return m_pLoader->GetEnabled();
}

void CExCodeInput::SetFocus()
{
	m_pLoader->SetFocus();
}

void CExCodeInput::SetCurSel(short nIndex)
{
	m_pLoader->SetCurSel(nIndex);
}

OLE_HANDLE CExCodeInput::GetHWnd()
{
	return m_pLoader->GetHWnd();
}

HWND CExCodeInput::GetSafeHwnd()
{
	return m_pLoader->GetSafeHwnd();
}

void CExCodeInput::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/)
{
	m_pLoader->MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CExCodeInput::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	m_pLoader->MoveWindow(lpRect, bRepaint);
}

BOOL CExCodeInput::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
{
	return m_pLoader->SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);
}

BOOL CExCodeInput::ShowWindow(int nCmdShow)
{
	return m_pLoader->ShowWindow(nCmdShow);
}

void CExCodeInput::GetWindowRect(LPRECT lpRect)
{
	m_pLoader->GetWindowRect(lpRect);
}

void CExCodeInput::GetClientRect(LPRECT lpRect)
{
	m_pLoader->GetClientRect(lpRect);
}
