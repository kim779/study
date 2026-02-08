// WndMapView.cpp : implementation file
//

#include "stdafx.h"
//#include "MultiSrcDef.h"
//#include "FrameDlg.h"
//#include "WndMapView.h"
#include "../include/ContextMenuManager.h"
#include "../../INCLUDE_CONTENTS/Account.h"
#include "./define/ChartMsgDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT RMSG_SETFOCUSTODESK = ::RegisterWindowMessage("RMSG_SETFOCUSTODESK");
const UINT RMSG_GETCLIENTDATA = ::RegisterWindowMessage("RMSG_GETCLIENTDATA");
const UINT RMSG_SETMLTSCREENFUNC = ::RegisterWindowMessage(_T("RMSG_SETMLTSCREENFUNC"));
const UINT RMSG_OPENMAP = ::RegisterWindowMessage(_T("RMSG_OPENMAP"));
const UINT RMSG_SETOPENSHAREDATA	= ::RegisterWindowMessage(_T("RMSG_SETOPENSHAREDATA"));
const UINT RMSG_DELETEMULITSCREEN	= ::RegisterWindowMessage(_T("RMSG_DELETEMULITSCREEN"));
const UINT RMSG_DEFAULTRECT = ::RegisterWindowMessage(_T("RMSG_DEFAULTRECT"));
const UINT RMSG_GETMAPTR = ::RegisterWindowMessage("RMSG_GETMAPTR");
const UINT RMSG_SETSHAREDATA = ::RegisterWindowMessage(_T("RMSG_SETSHAREDATA"));

#define IDC_NEODESKCTRL1 1013

//typedef struct {
//	CString m_szKey;		// 공유명
//	CString m_szValue;		// 공유값
//}STSHAREDDATA;

typedef struct _ST_GETGROUPINFO
{
	int nConID;
	int nTabNum;
	CString strGroup;
	CString strUseGroupCtrl;
	CString strSameXfmResize;
}ST_GETGROUPINFO;

void NotifyFromParentView(LPVOID pThis, UINT nType, LPSTR szKey, LPSTR szValue)
{
	CWndMapView* pView = (CWndMapView*)pThis;
	if(nType == 0)
	{
		pView->InitGShareDataAccount();
		pView->m_ctrlNeoDesk.SetNotifyCommand(szKey);
	}
	else if(nType == 1)
		pView->m_ctrlNeoDesk.NotifyFromMain(szKey, szValue);
	else if(nType == 2)
	{
		::strcpy(szKey, pView->m_xGSharedMng.GetLastUseAccount());

		CString strCode;
		pView->m_xGSharedMng.GetData(_T("코드-코코2"), strCode);
		::strcpy(szValue, strCode);
	}
}

LPCSTR CWndMapView::CXBDeskParent::GetBasePath()
{
	METHOD_PROLOGUE(CWndMapView, DeskParent)

	return m_szBasePath;
}

HWND  CWndMapView::CXBDeskParent::GetParentHwnd()
{
	METHOD_PROLOGUE(CWndMapView, DeskParent)
	return pThis->m_hWnd;
}

LPDISPATCH CWndMapView::CXBDeskParent::GetParentDisp()
{
	METHOD_PROLOGUE(CWndMapView, DeskParent)

	CCmdTarget* pIDispatch = (CCmdTarget*)::SendMessage(pThis->m_hWndParentView, RMSG_GETCLIENTDATA, 2, 0);
	return pIDispatch->GetIDispatch(TRUE);
}

long CWndMapView::CXBDeskParent::GetBaseCodeTable()
{
//	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
//	if(pMain)
//	{
//		return pMain->m_pBaseCodeTable;
//	}
	return NULL;
}

BOOL CWndMapView::CXBDeskParent::OpenMap(LPCSTR _szMapID, int _nFlag, LPCSTR _szParam)
{
	METHOD_PROLOGUE(CWndMapView, DeskParent)
	return TRUE;
}

BOOL CWndMapView::CXBDeskParent::InvokeEx1(LPCSTR _szFunc, LPCSTR _szParam, CString& szResult)
{
	METHOD_PROLOGUE(CWndMapView, DeskParent)
	return TRUE;
}

BOOL CWndMapView::CXBDeskParent::InvokeEx2(LPCSTR _szFunc, long _dwParam, long& dwResult)
{
	METHOD_PROLOGUE(CWndMapView, DeskParent)
	return TRUE;
}

void CWndMapView::CXBDeskParent::CallCommand(LPCSTR _szCmd)
{
	METHOD_PROLOGUE(CWndMapView, DeskParent)

	pThis->CallCommand(_szCmd);
}

void CWndMapView::CXBDeskParent::CallCommand(HWND _hDesk, LPCSTR _szCmd){}
long CWndMapView::CXBDeskParent::GetKeyData(LPCSTR _szKey1, LPCSTR _szKey2){ return NULL; }
long CWndMapView::CXBDeskParent::GetKeyData(LPCSTR _szKey, int _nKey){ return NULL; }

// * 사용자의 정보에 관한 것을 요청한다.
// * 필요할 때마다 요청해서 추가한다. 일단 컨트롤에 추가하면 고정.
long CWndMapView::CXBDeskParent::GetUserInfo(LPCSTR szKey)
{
	return NULL;
}

// * 맵이 올라가는 프레임의 정보에 관한 것을 요청한다.
// * 필요할 때마다 요청해서 추가한다. 일단 컨트롤에 추가하면 고정.
long CWndMapView::CXBDeskParent::GetFrameInfo(LPCSTR szKey)
{
	METHOD_PROLOGUE(CWndMapView, DeskParent)

	if(!pThis->m_hWndParentView || !::IsWindow(pThis->m_hWndParentView))
		return NULL;

	if(::strcmp(szKey, "XGPNAME") == 0)
		return (long)pThis->m_szMapID;

	if(::strcmp(szKey, "USERID") == 0)
		return ::SendMessage(pThis->m_hWndParentView, RMSG_GETCLIENTDATA, 4, 0);

	if(::strcmp(szKey, "GROUPACTIVE")==0)
	{
		ST_GETGROUPINFO stInfo;
		::SendMessage(pThis->m_hWndParentView, RMSG_GETCLIENTDATA, 9, (LPARAM)&stInfo);
		
		return (long)(LPTSTR)(LPCTSTR)stInfo.strSameXfmResize;		
	}

	if(::strcmp(szKey, "FRAMEHADLE")==0)
		return ::SendMessage(pThis->m_hWndParentView, RMSG_GETCLIENTDATA, 5, 0);

	if(::strcmp(szKey, "TABINDEX")==0)
		return ::SendMessage(pThis->m_hWndParentView, RMSG_GETCLIENTDATA, 6, 0);

	return NULL;
}

// * 메인(CMainFrame, CWinApp)등에서 관리하는 정보에 관한 것을 요청한다.
// * 필요할 때마다 요청해서 추가한다. 일단 컨트롤에 추가하면 고정.
long CWndMapView::CXBDeskParent::GetMainInfo(LPCSTR szKey)
{
	METHOD_PROLOGUE(CWndMapView, DeskParent)
	if(strcmp(szKey, "USERCOLORTABLE")==0)
		return (long)(pThis->m_pColorTable);
	else if(strcmp(szKey, "MAINHANDLE") == 0) 
		return ::SendMessage(pThis->m_hWndParentView, RMSG_GETCLIENTDATA, 7, 0);
	else if(strcmp(szKey, "FRAMEHANDLE") == 0) 
		return ::SendMessage(pThis->m_hWndParentView, RMSG_GETCLIENTDATA, 8, 0);

	return NULL;
}

void CWndMapView::OnMapMessageNeodeskctrl1(short _nType, LPCTSTR _szMessage, short _nLen) 
{
	CString sMessage;
//	sMessage.Format("%05d<%5.5s>%*.*s\n", m_nMsgNo++, _szMessage, _nLen-5, _nLen-5, &_szMessage[5]);
//	sMessage.TrimLeft();
//	sMessage.TrimRight();

	TRACE("%s\n", _szMessage);
	if(_nLen>16)
	{
		// 메세지 앞에 "AP"가 들어가는 경우만 두자리를 뺀다. 
		int nIndex = _strnicmp("AP", &_szMessage[13], 2)==0 ? 15 : 13 ;
		sMessage.Format("[%5.5s]%-s", &_szMessage[7], &_szMessage[nIndex]);
		
		if(m_pMessageMng && m_pMessageMng->GetSafeHwnd() && sMessage.GetLength()>0)
			m_pMessageMng->AddMessage((LPSTR)(LPCSTR)sMessage);		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWndMapView

CWndMapView::CWndMapView(LPSTR szFile, LPSTR szRootPath, HWND hSocket, LPVOID pMainRscMng, LPVOID pColorTable, BOOL bMsgBox, HWND hParentView, BOOL bUseSetTR) :
	m_szFile(szFile),  m_szRootPath(szRootPath), m_hSocket(hSocket), m_pColorTable(pColorTable), m_bHasMessageBar(bMsgBox),
	m_pMessageMng(NULL), m_hWndParentView(hParentView), m_pDSS(NULL), m_pParentShare(NULL),
	m_pMnManager(NULL), m_bUseSetTR(bUseSetTR)
{
//	EnableAutomation();

	m_pMainRscManager = (CMainResourceManager*)pMainRscMng;
	CreateResourceManager();

	::memset(m_szMapID, 0x00, sizeof(m_szMapID));
}

CWndMapView::CWndMapView()
{
//	EnableAutomation();
	CreateResourceManager();

	m_bHasMessageBar = FALSE;
	m_pMessageMng = NULL;
	m_hWndParentView = NULL;
	m_pMnManager = NULL;
	m_bUseSetTR = FALSE;

	::memset(m_szMapID, 0x00, sizeof(m_szMapID));
}

CWndMapView::~CWndMapView()
{
	if(m_pMnManager)
	{
		delete m_pMnManager;
		m_pMnManager = NULL;
	}
}

void CWndMapView::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CWndMapView, CWnd)
	//{{AFX_MSG_MAP(CWndMapView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CWndMapView, CWnd)
	//{{AFX_DISPATCH_MAP(CWndMapView)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IWndMapView to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {7BC5CE93-CF7F-4F8B-A15A-D6EE2787A7CB}
static const IID IID_IWndMapView =
{ 0x7bc5ce93, 0xcf7f, 0x4f8b, { 0xa1, 0x5a, 0xd6, 0xee, 0x27, 0x87, 0xa7, 0xcb } };

BEGIN_INTERFACE_MAP(CWndMapView, CWnd)
	INTERFACE_PART(CWndMapView, IID_IWndMapView, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWndMapView message handlers

void CWndMapView::InitGShareDataAccount()
{
	if(!m_pParentShare)
		return;

	CAcctHelper* pAcctHelper = (CAcctHelper*)::SendMessage(m_hWndParentView, RMSG_GETCLIENTDATA, 1, 0);
	m_xGSharedMng.SetAcctHelper(pAcctHelper);

	char* aSharePre = "!NP_Acct";
	char* aShareKey[] = {"01"};//, "06"};

	CString rString;
	for(int i = 0; i < sizeof(aShareKey) / sizeof(char*); i++)
	{
		CString szSharePost = aShareKey[i];
		CString szKey = aSharePre + szSharePost;
//		pAcctHelper->GetKeyString(aShareKey[i], rString);
		
		m_pParentShare->GetData(szKey, rString);

		m_xGSharedMng.SetOrAddData(szKey, rString, NULL, TRUE);
		rString.Empty();
	}

//	pAcctHelper->GetAllString2(rString);

	char* szKey[] = {"!NP_AcctAll", "!NP_UserID", "지점코드", "지점계좌변경"};
	int nSize = sizeof(szKey) / sizeof(char*);
	for(i = 0; i < nSize; i++)
	{
		m_pParentShare->GetData(szKey[i], rString);
		m_xGSharedMng.SetOrAddData(szKey[i], rString, NULL, TRUE);

		rString.Empty();
	}
}

int CWndMapView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


//	m_pParentShare = (CBaseChildShareManager*)::SendMessage(m_hWndParentView, RMSG_GETCLIENTDATA, 0, 0);
//	if(!m_pParentShare)
//	{		
//		m_hWndParentView = ::GetParent(m_hWndParentView);
//		m_pParentShare = (CBaseChildShareManager*)::SendMessage(m_hWndParentView, RMSG_GETCLIENTDATA, 0, 0);
//	}
	InitGShareDataAccount();

	if(m_bHasMessageBar)
	{
		m_pMessageMng = new CMessageWindow();
		if((m_pMessageMng->CreateMessageWnd(this)) == FALSE)
		{
			delete m_pMessageMng;
			m_pMessageMng = NULL;
		}	
	}

	CRect rect(lpCreateStruct->x, lpCreateStruct->y, lpCreateStruct->cx, lpCreateStruct->cy);

	if(!m_ctrlNeoDesk.Create(NULL, WS_CHILD|WS_VISIBLE, rect, this, IDC_NEODESKCTRL1))
		return 0;
	
	GetMapInfo(m_szFile);

	CString strID(m_szFile);
	::strcpy(m_szMapID, (LPSTR)(LPCTSTR)strID.Right(9).Left(5));

	m_xDeskParent.m_szBasePath = m_szRootPath;
	m_ctrlNeoDesk.InitInfo((long)&m_xDeskParent);
	m_ctrlNeoDesk.SetSocketHandle(_T("default"), (long)m_hSocket);
//	m_pDSS = (CDeskSharedServer*)m_ctrlNeoDesk.SetSharedInterface(NULL);
//	m_pDSS = (CDeskSharedServer*)m_ctrlNeoDesk.SetSharedInterface((long)(&m_xGSharedMng));
	m_pDSS = (CDeskSharedServer*)m_ctrlNeoDesk.SetSharedInterface((long)(&m_xGSharedMng));

	CCmdTarget* pIDispatch = (CCmdTarget*)::SendMessage(m_hWndParentView, RMSG_GETCLIENTDATA, 2, 0);

	m_ctrlNeoDesk.SetParentDispatch(pIDispatch->GetIDispatch(FALSE));
	m_ctrlNeoDesk.SetRscMng((long)m_pResourceManager);
	m_ctrlNeoDesk.SetOpenMapData2(NULL);
	m_ctrlNeoDesk.LoadXfm(m_szFile);
	m_ctrlNeoDesk.ResizeFont(m_nWidth, m_nHeight, "굴림체");
	m_ctrlNeoDesk.SendMessage(RMSG_SETFOCUSTODESK);
//	m_ctrlNeoDesk.SetGSharedNotifyFlag(FALSE);

	// 컨텍스트 메뉴 세팅
	m_szUserPath = (LPSTR)::SendMessage(m_hWndParentView, RMSG_GETCLIENTDATA, 10, 0);

	m_pMnManager = new CContextMenuManager(m_szRootPath ,m_szUserPath, this);
	m_pMnManager->SetData(&m_ctrlNeoDesk, m_hWnd , _T("90200"));

	// 컨텍스트 메뉴 추가
	m_pMnManager->AddContextMenu();

	::SendMessage(m_hWndParentView, RMSG_SETMLTSCREENFUNC, (WPARAM)(LPVOID)NotifyFromParentView, (LPARAM)this);

	return 0;
}

#include "../include/XfmInfo.h"
void CWndMapView::GetMapInfo(LPSTR szFile)
{
	CXfmInfo info;
	info.LoadXfm(szFile, 0);
	CString szKey, rValue, szValue;
	for(POSITION pos=info.m_xmiFD.m_mapNamedItems.GetStartPosition(); pos; )
	{
		info.m_xmiFD.m_mapNamedItems.GetNextAssoc(pos, szKey, szValue);
		if(szKey.CompareNoCase("위치")==0) {
			info.m_xmiFD.GetString(szKey, rValue);
			for(int i=0; i<2; i++)	// 위치=0, 0, xxx, xxx 
			{
				rValue = rValue.Mid(rValue.Find(",")+1);
			}
			rValue.TrimLeft();
			rValue.TrimRight();
			int cx = 0, cy = 0;
			SetDlgFormSize((LPSTR)(LPCTSTR)rValue);		
		}
		else if(szKey.CompareNoCase("설명")==0) {
			info.m_xmiFD.GetString(szKey, rValue);
			if(rValue.GetLength()>0) {
				SetWindowText(rValue);
			}
		}
	} 
}

void CWndMapView::CreateResourceManager()
{
	m_pResourceManager = (CResourceManager*)((CMainResourceManager*)m_pMainRscManager)->CreateResourceManager();
	if(m_pResourceManager)
	{
		m_pResourceManager->CreateBaseFont("굴림체", 12);
	}	
}

void CWndMapView::DeleteResourceManager()
{
	if(m_pResourceManager)
		((CMainResourceManager*)m_pMainRscManager)->DeleteResourceManager((long)m_pResourceManager);
}

void CWndMapView::OnDestroy() 
{
	::SendMessage(m_hWndParentView, RMSG_DELETEMULITSCREEN, 0, (LPARAM)this);

	if(m_pMessageMng) 
	{ 
		delete m_pMessageMng; 
		m_pMessageMng = NULL; 
	}
	
	if(m_ctrlNeoDesk.GetSafeHwnd())
		m_ctrlNeoDesk.DestroyWindow();

	DeleteResourceManager();

	CWnd::OnDestroy();
}

BOOL CWndMapView::SetDlgFormSize(LPSTR szFile)
{
	if(m_ctrlNeoDesk.GetSafeHwnd())
	{
		int cx = 0;
		int cy = 0;
		CString szSize = szFile;
		CString szTemp;
		int nIndex = szSize.Find(",");
		if(nIndex<0) 	return FALSE;

		szTemp = szSize.Left(nIndex);
		szSize = szSize.Mid(nIndex+1);
		szTemp.TrimLeft();	szTemp.TrimRight();
		szSize.TrimLeft();	szSize.TrimRight();
		cx = atoi((LPSTR)(LPCSTR)szTemp);
		cy = atoi((LPSTR)(LPCSTR)szSize);
		cx /= 15;	// 트윕 -> 픽셀 (1픽셀 = 15트윕)			
		cy /= 15;

		m_nWidth = cx;
		m_nHeight = cy;

		// 데스트 크기 조절 
		m_ctrlNeoDesk.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE);
		m_ctrlNeoDesk.SendMessage(RMSG_DEFAULTRECT, (WPARAM)cx, (LPARAM)cy);

		// 좌우 여백 
		cx += 9;//6;

		// 상하 여백 + 타이틀
		int nMsgBarHeight = (m_bHasMessageBar ? 23 : 0);
		cy += (26 + nMsgBarHeight);	// 메시지바 추가

		// 다이얼로그 크기 조절 
		SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE);

		// 메세지 윈도우 처리
		// SetWindowPos() 이후에 이 루틴을 넣어야 함!!! 
		// 파라미터는 의미없음
		if(m_bHasMessageBar)
			m_pMessageMng->ResizeMessageWnd( 0, 0, 0);

		return	TRUE;
	}

	return FALSE;
}

void CWndMapView::CallCommand(LPCSTR szCmd)
{
	CString szCommand, szParam;
	szCommand = szCmd;
	int i = szCommand.Find(":");
	if(i<1)
		return;
	szParam = szCommand.Mid(i+1);
	szCommand = szCommand.Left(i);

	if(szCommand.CompareNoCase("CMD")==0)
	{
		if(szParam.CompareNoCase("CLOSE")==0)
		{
			SendMessage(WM_COMMAND, IDOK);
		}
	}

	TRACE("%s=%s\n", szCommand, szParam);
}

LRESULT CWndMapView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message >= 0xC000)
	{
		if(message == RMSG_OPENMAP || message == RMSG_CONTEXTMENU)
		{
			CString strCode;
			m_xGSharedMng.GetData(_T("코드-코코2"), strCode);
			
			::SendMessage(m_hWndParentView, RMSG_SETOPENSHAREDATA, (WPARAM)m_xGSharedMng.GetLastUseAccount(), (LPARAM)(LPSTR)(LPCTSTR)strCode);
		}
		else if(message == RMSG_SETSHAREDATA)
		{
			STSHAREDDATA* pSData = (STSHAREDDATA*)lParam;
			SettingData(pSData->m_szKey, pSData->m_szValue);
			return CWnd::WindowProc(message, wParam, lParam);
		}
		else if(message == RMSG_GETMAPTR)
		{
			if(m_bUseSetTR)
			{
				char* szTR = (char*)lParam;
				::strcpy(szTR, m_szMapID);
				return CWnd::WindowProc(message, wParam, lParam);
			}
		}
		
		return ::SendMessage(m_hWndParentView, message, wParam, lParam);
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

void CWndMapView::SetGShareData(LPVOID pShareData)
{
	STSHAREDDATA* pData = (STSHAREDDATA*)pShareData;
	m_xGSharedMng.SetOrAddData(pData->m_szKey, pData->m_szValue, NULL, TRUE);
//	if(m_pDSS)
//		m_pDSS->ChangedData(pData->m_szKey, pData->m_szValue);
}

void CWndMapView::GetGShareData(LPSTR szValue, CString& strValue)
{
	m_xGSharedMng.GetData(szValue, strValue);
}

void CWndMapView::SettingData(LPCTSTR _szName, LPCTSTR _szValue)
{
	CString strName = _szName;
	strName.TrimRight(';');
	if(strName=="코드-코코2")
	{
		GetParent()->SendMessage(UMSG_UPDATECODE,(WPARAM)this,(LPARAM)_szValue);
	}	
}

void CWndMapView::SetParentShareData(LPCTSTR _szName, LPCTSTR _szValue)
{
	POSITION pos = m_pParentShare->m_ShareList.GetHeadPosition();
	while(pos)
	{
		CShareData* pShareData = m_pParentShare->m_ShareList.GetNext(pos);
		if(pShareData->m_szKey.CompareNoCase(_szName) == 0)
		{
			pShareData->m_szValue = _szValue;
			return;
		}
	}
}

void CWndMapView::SetParentAllShareData()
{
	POSITION pos = m_xGSharedMng.m_ShareList.GetHeadPosition();
	while(pos)
	{
		CShareData* pShareData = m_xGSharedMng.m_ShareList.GetNext(pos);
		SetParentShareData((LPCTSTR)pShareData->m_szKey, (LPCTSTR)pShareData->m_szValue);
	}
}

void CWndMapView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	ClientToScreen(&point);
//	GetParent()->ScreenToClient(&point);
//	GetParent()->SendMessage(WM_RBUTTONUP,nFlags,MAKELONG(point.x,point.y));
//	CWnd::OnRButtonUp(nFlags, point);
}

void CWndMapView::RefreshPopupMenu()
{
	if(m_ctrlNeoDesk.GetSafeHwnd() && m_pMnManager)
		m_pMnManager->AddContextMenu();
}