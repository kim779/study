// axInstance.cpp: implementation of the CAxInstance class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axis.h"
#include "axInstance.h"

#include "mainFrm.h"
#include "childFrm.h"
#include "axisDoc.h"
#include "axisView.h"
#include "axScrollView.h"
#include "axtabview.h"

#include "resource.h"
#include "../h/axisvar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAxInstance::CAxInstance(CString regkey)
{
	m_regkey          = regkey;
	m_classRegistered = false;
	m_uniqueClsName   = _T("Axis Workstation V03.00");
	m_hMutex = (HANDLE) 0;
}

CAxInstance::~CAxInstance()
{
	if (m_hMutex != (HANDLE) 0)
	{
		::ReleaseMutex(m_hMutex);
		::CloseHandle(m_hMutex);
	}
}

ULONG CAxInstance::ProcIDFromWnd(HWND hwnd)
{
	ULONG idProc;
	::GetWindowThreadProcessId(hwnd, &idProc);
	return idProc;
}

HWND CAxInstance::GetWinHandle(ULONG pid)
{
	HWND tempHwnd = ::FindWindowA(NULL, NULL); //AO¡íoA¡× A¨Ï¥ì¥ì¢¯i CU¥ìe A¡Ì¡¾a
	while(tempHwnd != NULL)
	{
		if(::GetParent(tempHwnd) == NULL)
		{
			if(pid == ProcIDFromWnd(tempHwnd))
			{
				return tempHwnd;
			}
		}
		tempHwnd = ::GetWindow(tempHwnd, GW_HWNDNEXT);
	}
	return 0;
}

void CAxInstance::SendDataToMain(char* pdata)
{
	CString strParameter;
	char szDir[1024*2];
	BOOL bFlag = TRUE;
	HANDLE handle;
	PROCESSENTRY32 Process32;

	Process32.dwSize = sizeof(PROCESSENTRY32);
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		
	if (Process32First(handle, &Process32))
	{
		while(Process32Next(handle, &Process32))
		{
			CString szExe = Process32.szExeFile;
			szExe.MakeUpper();
			if (szExe.CompareNoCase("AXIS.exe") == 0)
			{
				CString strname;
				
				COPYDATASTRUCT tip;
				tip.cbData = 9998;
				tip.lpData = (char*)pdata;
				
				HWND hwn = GetWinHandle(Process32.th32ProcessID);
				::SendMessage(hwn, WM_COPYDATA, (WPARAM)this , LPARAM(&tip));	
			}
		}
	}
		
	CloseHandle(handle);
}

bool CAxInstance::IsFirstInstance()
{
	m_hMutex = ::OpenMutex( MUTEX_ALL_ACCESS, TRUE, m_regkey );
	if ( m_hMutex != NULL )
	{
		SendDataToMain(NULL);
		::CloseHandle( m_hMutex );
		m_hMutex = NULL;
		return false;
	}

	int nCount = 0;
	while ( m_hMutex == NULL && nCount < 5 ) 
	{
		m_hMutex = ::CreateMutex( NULL, FALSE, m_regkey );
		nCount++;
	}
	return true;
}

bool CAxInstance::RegisterClass(HINSTANCE hInst, UINT uMainID)
{
	CWinApp* app;
	WNDCLASS wndcls;
	CString	root, iconN;

	ZeroMemory(&wndcls, sizeof(WNDCLASS));   // start with NULL defaults

	app  = AfxGetApp();
	root = app->GetProfileString(ENVIRONMENT, ROOTDIR);
	iconN.Format("%s\\%s\\%s", root, IMAGEDIR, "axis.ico");
	wndcls.hIcon = (HICON) LoadImage(NULL, iconN, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	if (wndcls.hIcon == NULL)
		wndcls.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(uMainID));

	wndcls.style         = CS_DBLCLKS|CS_PARENTDC|CS_CLASSDC;	//|CS_HREDRAW|CS_VREDRAW;
	wndcls.lpfnWndProc   = AfxWndProc;	// DefWindowProc;
	wndcls.cbClsExtra    = wndcls.cbWndExtra = 0;
	wndcls.hInstance     = hInst;
	wndcls.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
//	wndcls.hbrBackground = (HBRUSH) ::GetStockObject(WHITE_BRUSH);
	wndcls.lpszMenuName  = NULL;
	wndcls.lpszClassName = m_uniqueClsName;
	if (!AfxRegisterClass(&wndcls))
	{
		AfxThrowResourceException();
		return false;
	}

	m_classRegistered = true;
	return true;
}

void CAxInstance::SetClass(CREATESTRUCT& cs)
{
	cs.lpszClass = m_uniqueClsName;
}

void CAxInstance::UnsetClass(HINSTANCE hInst)
{
	if (m_classRegistered)
		::UnregisterClass(m_uniqueClsName, hInst);
}

bool CAxInstance::AddDocTemplate()
{
	CMultiDocTemplate* pDocTemplate;

	CAxisApp* axisApp = (CAxisApp *) AfxGetApp();
	pDocTemplate = new CMultiDocTemplate(	// normal view
		IDR_AXISTYPE,
		RUNTIME_CLASS(CAxisDoc),
		RUNTIME_CLASS(CChildFrame),	// custom MDI child frame && view
		RUNTIME_CLASS(CAxisView));
	ASSERT_VALID(pDocTemplate);
	axisApp->AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(	// scroll view
		IDR_AXISTYPE,
		RUNTIME_CLASS(CAxisDoc),
		RUNTIME_CLASS(CChildFrame),	// custom MDI child frame && scrollView
		RUNTIME_CLASS(CAxScrollView));
	ASSERT_VALID(pDocTemplate);
	axisApp->AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(	// tab view
		IDR_AXISTYPE,
		RUNTIME_CLASS(CAxisDoc),
		RUNTIME_CLASS(CChildFrame),	// custom MDI child frame && scrollView
		RUNTIME_CLASS(CAxTabView));
	ASSERT_VALID(pDocTemplate);
	axisApp->AddDocTemplate(pDocTemplate);
	return true;
}