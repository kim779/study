/ ExOCXDLL.cpp: implementation of the CExOCXDLL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExOCXDLL.h"
#include "../../Inc/MessageDefine.h"
#include "../../Inc/IAUPlatformDispMng.h"
#include "../../inc/commonDefines.h" // 0024696: 파생상품 코드체계 변경 제도개선

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CExOCXDLL, CExDialog)

/////////////////////////////////////////////////////////////////////////////
// CExOCXDLL dialog
CExOCXDLL::CExOCXDLL()
	: CExDialog(),
	m_pMapParam1(NULL),
	m_pMapParam2(NULL),
	m_pMapParam3(NULL),
	m_hChildFrame(NULL),
	m_hFocusChild(NULL),
	m_nChildHandlePos(0)
{
	INIT_DR_PLATFORM();
}

CExOCXDLL::CExOCXDLL(const UINT nID, CWnd* pParent /*=NULL*/)
	: CExDialog(nID, pParent),
	m_pMapParam1(NULL),
	m_pMapParam2(NULL),
	m_pMapParam3(NULL),
	m_hChildFrame(NULL),
	m_hFocusChild(NULL),
	m_nChildHandlePos(0)
{
	//{{AFX_DATA_INIT(CExOCXDLL)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	INIT_DR_PLATFORM();
}

BEGIN_MESSAGE_MAP(CExOCXDLL, CExDialog)
	//{{AFX_MSG_MAP(CExOCXDLL)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_LSHAREDCHANGED, OnLShareChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExOCXDLL message handlers

#ifdef _DEBUG
void CExOCXDLL::AssertValid() const
{
//	CWnd::AssertValid();
}
#endif

void CExOCXDLL::FormLoad()
{
}

void CExOCXDLL::FormLoadCompleted()
{
}

void CExOCXDLL::OnGSharedDataChange(LPCTSTR lpSharedName, LPCTSTR lpGSharedData, BOOL bWithStart)
{
}

void CExOCXDLL::OnLSharedDataChange(LPCTSTR lpSharedName, LPCTSTR lpLSharedData, BOOL bWithStart)
{
}

void CExOCXDLL::SetParameters1(CString strDLLParam)
{
	m_pMapParam1 = new CMapStringToString;
	
	int nIndex = 0;

	CString strParam, strKey, strValue;
	while(AfxExtractSubString(strParam, strDLLParam, nIndex++, _T(';')))
	{
		if(strParam.IsEmpty())
			continue;

		AfxExtractSubString(strKey, strParam, 0, _T('='));
		if(strKey.IsEmpty())
			continue;

		AfxExtractSubString(strValue, strParam, 1, _T('='));
		if(strValue.IsEmpty())
			continue;

		m_pMapParam1->SetAt(strKey, strValue);
	}
}

void CExOCXDLL::SetParameters2(CString strDLLParam)
{
	m_pMapParam2 = new CMapStringToString;
	
	int nIndex = 0;

	CString strParam, strKey, strValue;
	while(AfxExtractSubString(strParam, strDLLParam, nIndex++, _T(';')))
	{
		if(strParam.IsEmpty())
			continue;

		AfxExtractSubString(strKey, strParam, 0, _T('='));
		if(strKey.IsEmpty())
			continue;

		AfxExtractSubString(strValue, strParam, 1, _T('='));
		if(strValue.IsEmpty())
			continue;

		m_pMapParam2->SetAt(strKey, strValue);
	}
}

void CExOCXDLL::SetParameters3(CString strDLLParam)
{
	m_pMapParam3 = new CMapStringToString;
	
	int nIndex = 0;

	CString strParam, strKey, strValue;
	while(AfxExtractSubString(strParam, strDLLParam, nIndex++, _T(';')))
	{
		if(strParam.IsEmpty())
			continue;

		AfxExtractSubString(strKey, strParam, 0, _T('='));
		if(strKey.IsEmpty())
			continue;

		AfxExtractSubString(strValue, strParam, 1, _T('='));
		if(strValue.IsEmpty())
			continue;

		m_pMapParam3->SetAt(strKey, strValue);
	}
}

void CExOCXDLL::OnDestroy() 
{
	int nCount = m_arPermanentHandle.GetSize();

	for(int i = 0; i < nCount; i++)
		::RemovePermanentHandle(m_arPermanentHandle.GetAt(i));

	m_arPermanentHandle.RemoveAll();

	CExDialog::OnDestroy();

	RemoveParameter1();
	RemoveParameter2();
	RemoveParameter3();
}

CString CExOCXDLL::GetParameter1(LPCTSTR lpszKey)
{
	if(!m_pMapParam1)
		return _T("");

	CString strValue(_T(""));

	m_pMapParam1->Lookup(lpszKey, strValue);
	return strValue;
}

CString CExOCXDLL::GetParameter2(LPCTSTR lpszKey)
{
	if(!m_pMapParam2)
		return _T("");

	CString strValue(_T(""));

	m_pMapParam2->Lookup(lpszKey, strValue);
	return strValue;
}

CString CExOCXDLL::GetParameter3(LPCTSTR lpszKey)
{
	if(!m_pMapParam3)
		return _T("");

	CString strValue(_T(""));

	m_pMapParam3->Lookup(lpszKey, strValue);
	return strValue;
}

void CExOCXDLL::RemoveParameter1()
{
	if(m_pMapParam1)
	{
		m_pMapParam1->RemoveAll();

		delete m_pMapParam1;
		m_pMapParam1 = NULL;
	}
}

void CExOCXDLL::RemoveParameter2()
{
	if(m_pMapParam2)
	{
		m_pMapParam2->RemoveAll();

		delete m_pMapParam2;
		m_pMapParam2 = NULL;
	}
}

void CExOCXDLL::RemoveParameter3()
{
	if(m_pMapParam3)
	{
		m_pMapParam3->RemoveAll();

		delete m_pMapParam3;
		m_pMapParam3 = NULL;
	}
}

void CExOCXDLL::DoFullWindowSize()
{
	CRect rcDesk;
	::GetClientRect(m_pBaseDesk->GetDeskHwnd(), &rcDesk);

	MoveWindow(rcDesk);
}

long CExOCXDLL::Platform_GetBase(LPCTSTR szKey)
{
	if(::lstrcmp(szKey, _T("PACKIN")) == 0)
		return (long)&m_xImplPackIn;
	else if(::lstrcmp(szKey, _T("PACKOUT")) == 0)
		return (long)&m_xImplPackOut;

	return 0L;
}

long CExOCXDLL::Platform_GetKey(LPCTSTR szKey)
{
	return 0L;
}

void CExOCXDLL::Platform_SetKey(LPCTSTR szKey, long dwKey)
{
}

void CExOCXDLL::Platform_Initialize(long dwMainDllObj, long dwBaseDesk, long dwRscMng, CStringList* pAllProperties)
{
	{
//		pThis->m_pMainDll = (IMainDllObj*)dwMainDllObj;
//		char buf[MAX_PATH];
//		pThis->m_pMainDll->GetUserPath(buf, MAX_PATH);
//		AfxMessageBox(buf);
	}
	{	// BaseDesk처리
		m_pBaseDesk = (CBaseDesk2*)dwBaseDesk;
		if(m_pBaseDesk)
		{
			m_xGSharedCli.m_pGSharedSvr = (CDeskSharedServer*)m_pBaseDesk->GetSharedInterface();
			m_xGSharedCli.AddSharedNameToServer();
		}
	}
	
	{	// 리소스매니저
		m_pResMng = (CResourceManager*)dwRscMng;
	}
	
	{	// SetAllProperties처리
		
		if(pAllProperties != NULL)
		{
//			CString strParam1, strParam2, strParam3;

			NEO_CONVERT_BEGIN(pAllProperties)
				//	공통프로퍼티
				NEO_CONVERT_STRING(pAllProperties,_T("GS")	,m_sGlobalSharedList,m_sGlobalSharedList);
				NEO_CONVERT_STRING(pAllProperties,_T("LS")	,m_sLocalSharedList,m_sLocalSharedList);
/*
				NEO_CONVERT_STRING(pAllProperties,_T("D_P1")	,strParam1, "");	// Param1
				NEO_CONVERT_STRING(pAllProperties,_T("D_P2")	,strParam2, "");	// Param2
				NEO_CONVERT_STRING(pAllProperties,_T("D_P3")	,strParam3, "");	// Param3
*/
			NEO_CONVERT_END()

			m_xGSharedCli.MakeList(m_sGlobalSharedList);
			InitLSharedData();

/*
			if(!strParam1.IsEmpty())
				SetParameters1(strParam1);
			if(!strParam2.IsEmpty())
				SetParameters2(strParam2);
			if(!strParam3.IsEmpty())
				SetParameters3(strParam3);
*/
		}
	}
}

void CExOCXDLL::Platform_SetProperty(STDATAITEM *stData, int nDataCnt)
{
	LPSTR szKey = NULL;
	for(int i = 0; i < nDataCnt; i++)
	{
		szKey = stData[i].szKey;
		TRACE("szKey(%s)\n", szKey);
	}
}

long CExOCXDLL::Platform_GetState(int nStatus, long dwData1, long dwData2)
{
	return 0L;
}

long CExOCXDLL::Platform_SetState(int nStatus, long dwData1, long dwData2)
{
	return 0L;
}

LRESULT CExOCXDLL::Platform_WindowProc(UINT message, WPARAM wParam, LPARAM lParam, int &nContinue)
{
	nContinue = 1;

	if(message == RMSG_FORMINITIALCOMPLETED)
	{
		m_xGSharedCli.IniStartGShareData();

//		ListupControlHandle(this);
		FormLoadCompleted();
	}
	else if(message == RMSG_FORMSTATECHANGE)
	{
		if(wParam == 1000 && lParam == 100)
			SetFocus();
	}
	else if(message == WM_SETFOCUS)
		SetFocus();

	return 0L;
}

LRESULT CExOCXDLL::Platform_PreTranslateMessage(MSG* pMsg)
{
	return 0L;
}

void CExOCXDLL::Platform_OnSize(UINT nType, int cx, int cy)
{
	SetWindowPos(NULL,0, 0, cx, cy, SWP_NOMOVE);
}

int CExOCXDLL::Packet_GetData(char* buf, int bufLen)
{
	return BCP_ERROR;
}

int CExOCXDLL::Packet_GetPackDataSize(char* pIn)
{
	return 0;
}

int CExOCXDLL::Packet_SetData(char* buf, int bufLen)
{
	return BCP_OK;
}

int CExOCXDLL::Packet_SetData2(char* buf, int bufLen)
{
	return BCP_OK;
}

int CExOCXDLL::Packet_SetData3(char* buf, int bufLen)
{
	return BCP_OK;
}

int CExOCXDLL::Packet_SetData999(int nType, char* buf, int bufLen)
{
	return 0;
}

void CExOCXDLL::SendMessageString(LPCTSTR lpszMessage)
{
	m_pBaseDesk->SetMapMessage(0, lpszMessage);
}

CString CExOCXDLL::GetCodeName(LPCTSTR lpszCode)
{
	CString strName = m_pIMasterDataManager->GetCodeInfo(_T("JMASTER"), 0, lpszCode);
	if(!strName.IsEmpty())
		return strName;

	strName = m_pIMasterDataManager->GetCodeInfo(_T("KMASTER"), 0, lpszCode);
	if(!strName.IsEmpty())
	{
//		strName = _T("ⓚ") + strName;
		return strName;
	}

	if(IS_FUT_CODE_1(lpszCode[1]) || IS_SPREAD_CODE_1(lpszCode[1])) // 0024696: 파생상품 코드체계 변경 제도개선
		strName = m_pIMasterDataManager->GetCodeInfo(_T("FMASTER"), 0, lpszCode);
	else if(IS_CALL_CODE_1(lpszCode[1]) || IS_PUT_CODE_1(lpszCode[1]))
		strName = m_pIMasterDataManager->GetCodeInfo(_T("PMASTER"), 0, lpszCode);

	if(!strName.IsEmpty())
		return strName;

	return _T("");
}

void CExOCXDLL::ChangeFrameSize(UINT nFlag, short nSizeX, short nSizeY)
{
	::SendMessage(GetFrameHandle(), UDM_SETFRAME_SIZE, (WPARAM)nFlag, MAKELPARAM(nSizeX, nSizeY));
}

typedef struct {
     	UINT  dwKey;     		// 키값 VK_TAB, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT
     	BOOL  bShiftPress;  	// 쉬프트키의 눌린상태 1(눌림), 0(일반)
		HWND  hWnd;				// 메시지를 보내는 컨트롤의 핸들.
} STRtfmKeyState;
const UINT RMSG_TABFOCUSMOVE = ::RegisterWindowMessage(_T("RMSG_TABFOCUSMOVE"));
BOOL CExOCXDLL::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB)
	{
		BOOL bShifht = ::GetKeyState(VK_SHIFT) & 0xFF00;

		if(!m_arChildHandle.GetSize())
			return GotoNextWindowFocus(pMsg->wParam, bShifht);

		CWnd* pWnd = GetFocus();
		while(!pWnd->GetParent()->IsKindOf(RUNTIME_CLASS(CDialog)))
		{
			if(pWnd)
				pWnd = pWnd->GetParent();
			else
				break;
		}

		HWND hFocus = pWnd->GetSafeHwnd();

Label_NextFocus:

		BOOL bFindHandle = FALSE;

		if(bShifht)
		{
			for(int i = m_arChildHandle.GetSize() - 1; i >= 0 ; i--)
			{
				if(m_arChildHandle.GetAt(i) == hFocus)
				{
					bFindHandle = TRUE;
					m_nChildHandlePos = i;
					break;
				}
			}

			if(!bFindHandle)
			{
				::SetFocus(m_hFocusChild);
				return TRUE;
			}
			
			if(!m_nChildHandlePos)
				return GotoNextWindowFocus(pMsg->wParam, bShifht);

			m_nChildHandlePos--;
		}
		else
		{
			for(int i = 0; i < m_arChildHandle.GetSize(); i++)
			{
				if(m_arChildHandle.GetAt(i) == hFocus)
				{
					bFindHandle = TRUE;
					m_nChildHandlePos = i;
					break;
				}
			}

			if(!bFindHandle)
			{
				::SetFocus(m_hFocusChild);
				return TRUE;
			}

			if(m_nChildHandlePos == (UINT)m_arChildHandle.GetSize() - 1)
				return GotoNextWindowFocus(pMsg->wParam, bShifht);

			m_nChildHandlePos++;
		}
		
		HWND hFocusNew = m_arChildHandle.GetAt(m_nChildHandlePos);
		if(!::IsWindow(hFocusNew) || !::IsWindowVisible(hFocusNew) || !::IsWindowEnabled(hFocusNew))
		{
			if(m_nChildHandlePos == (UINT)m_arChildHandle.GetSize() - 1)
				return GotoNextWindowFocus(pMsg->wParam, bShifht);

			hFocus = hFocusNew;
			goto Label_NextFocus;
		}

		::SetFocus(hFocusNew);
		return TRUE;
	}

	return CExDialog::PreTranslateMessage(pMsg);
}

BOOL CExOCXDLL::GotoNextWindowFocus(UINT nKey, BOOL bShift)
{
	STRtfmKeyState keyStata;
	keyStata.dwKey = nKey;
	keyStata.bShiftPress = bShift;
	keyStata.hWnd = GetParent()->GetSafeHwnd();

	if( m_pBaseDesk)
		::SendMessage(m_pBaseDesk->GetDeskHwnd(), RMSG_TABFOCUSMOVE, 10, (LPARAM)&keyStata);
	return TRUE;
}

void CExOCXDLL::ListupControlHandle(CWnd* pParent)
{
	CWnd* pChild = pParent->GetWindow(GW_CHILD);
	while(pChild)
	{
		if(pChild->IsKindOf(RUNTIME_CLASS(CDialog)) || pChild->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
			ListupControlHandle(pChild);
		else if(pChild->GetStyle() & WS_TABSTOP)
			m_arChildHandle.Add(pChild->GetSafeHwnd());

		pChild = pChild->GetNextWindow();
	}
}

int CExOCXDLL::AddTabStopHandle(HWND hFocus, HWND hChild, ...)
{
	va_list varHandle;
	va_start(varHandle, hChild);
	
	HWND hAddChild = hChild;
	m_hFocusChild = hFocus;

	while(hAddChild)
	{
		m_arChildHandle.Add(hAddChild);
		hAddChild = va_arg(varHandle, HWND);
	}

	va_end(varHandle);

	return m_arChildHandle.GetSize();
}

void CExOCXDLL::ClearTabStopHandle()
{
	m_arChildHandle.RemoveAll();
}

void CExOCXDLL::OnSetFocus(CWnd* pOldWnd) 
{
//	CExDialog::OnSetFocus(pOldWnd);
	BOOL bShift = ::GetKeyState(VK_SHIFT) & 0xFF00;
	if(!m_arChildHandle.GetSize())
	{
		GotoNextWindowFocus(VK_TAB, bShift);
		return;
	}
	
	if(bShift)
		m_nChildHandlePos = (UINT)m_arChildHandle.GetSize() - 1;
	else
		m_nChildHandlePos = 0;

	HWND hFocus = m_arChildHandle.GetAt(m_nChildHandlePos);
	while(!::IsWindow(hFocus) || !::IsWindowVisible(hFocus) || !::IsWindowEnabled(hFocus))
	{
		if(m_nChildHandlePos >= (UINT)m_arChildHandle.GetSize())
		{
			GotoNextWindowFocus(VK_TAB, bShift);
			return;
		}

		if(bShift)
			hFocus = m_arChildHandle.GetAt(m_nChildHandlePos--);
		else
			hFocus = m_arChildHandle.GetAt(m_nChildHandlePos++);
	}

	::SetFocus(hFocus);
}

void CExOCXDLL::SetPermanentHandle(CWnd* pWnd)
{
//	ListupControlHandle(pWnd);
	::SetPermanentHandle(pWnd->GetSafeHwnd(), pWnd);
}

BOOL CExOCXDLL::OnInitDialog() 
{
	CExDialog::OnInitDialog();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CExOCXDLL::OnLShareChanged(WPARAM wParam, LPARAM lParam)
{
	LPST_LSHAREDATA pSHD = (LPST_LSHAREDATA)wParam;
	if(pSHD)
	{
		CString szShareName = pSHD->m_szShareName;
		CString szData;
		if(pSHD->m_nType==LSDT_DWORD)
			szData.Format("%ld", pSHD->m_dwShareData);
		else
			szData = pSHD->m_szShareData;

		return ParedShareDataChange(szShareName, szData);
	}

	return 0L;
}

void CExOCXDLL::SetOpenMapData(LPCTSTR lpszData)
{
	IAUDeskDispManager* pDesk = (IAUDeskDispManager*)m_pIGateManager->GetInterface(32);
	LPUNKNOWN pDisp = m_pBaseDesk->GetUnknown();
	long lKey = pDesk->Advise(pDisp);

	char szHandle[9] = {NULL, };
	::sprintf(szHandle, _T("%x"), m_hWnd);
	pDesk->SetOpenMapData(lKey, 0, _T(""), lpszData, ::lstrlen(lpszData), szHandle);
	pDisp->Release();
}

long CExOCXDLL::GetOpenMapData(CString& strData)
{
	IAUDeskDispManager* pDesk = (IAUDeskDispManager*)m_pIGateManager->GetInterface(32);
	LPUNKNOWN pDisp = m_pBaseDesk->GetUnknown();
	long lKey = pDesk->Advise(pDisp);
	
	strData = pDesk->GetOpenMapData(lKey);
	long lLength = strData.GetLength();
	
	pDesk->UnAdvise(lKey);
	pDisp->Release();

	return lLength;
}

int CExOCXDLL::ExMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption/* = NULL*/, UINT nType/* = MB_OK*/)
{
	IAUMainDispManager* pMain = (IAUMainDispManager*)m_pIGateManager->GetInterface(33);
	LPUNKNOWN pDisp = m_pBaseDesk->GetCtrlUnknown(SCRIPT_MAIN);
	long lKey = pMain->Advise(pDisp);
	
	char szCaption[40] = {NULL, };
	if(!lpszCaption || !::lstrlen(lpszCaption))
		::GetWindowText(GetFrameHandle(), szCaption, sizeof(szCaption));
	else
		::lstrcpy(szCaption, lpszCaption);

	nType &= ~(MB_ICONHAND|MB_ICONQUESTION|MB_ICONEXCLAMATION|MB_ICONASTERISK);
	int nReturn = pMain->MsgBox(lKey, lpszText, szCaption, nType);

	pMain->UnAdvise(lKey);

	return nReturn;
}