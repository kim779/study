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
	m_uniqueClsName   = _T("Axis Workstation V04.00");
}

CAxInstance::~CAxInstance()
{
	if (m_hMutex)	::ReleaseMutex(m_hMutex);
}

bool CAxInstance::IsFirstInstance()
{
	//CString slog;

	m_hMutex = ::OpenMutex( MUTEX_ALL_ACCESS, TRUE, m_regkey );
	/*slog.Format("[axis][CAxInstance]  m_regkey=[%s] m_hMutex=[%x] \n", m_regkey, m_hMutex);
	OutputDebugString(slog);*/
	if ( m_hMutex != NULL )
	{
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
	{
		if( m_regkey.Find("STAFF") >= 0 )
		{
			Axis::isCustomer = false;
			wndcls.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(uMainID));
		}
		else
		{
			Axis::isCustomer = true;  
			wndcls.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_CUSTOMER));
		}
	}

	wndcls.style         = CS_DBLCLKS|CS_PARENTDC|CS_CLASSDC;	//|CS_HREDRAW|CS_VREDRAW;
	wndcls.lpfnWndProc   = AfxWndProc;	// DefWindowProc;
	wndcls.cbClsExtra    = wndcls.cbWndExtra = 0;
	wndcls.hInstance     = hInst;
	wndcls.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
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

#pragma warning (disable : 26400)
#pragma warning (disable : 26409)

bool CAxInstance::AddDocTemplate()
{
	CMultiDocTemplate* pDocTemplate{};

	CAxisApp* axisApp = (CAxisApp *) AfxGetApp();
	pDocTemplate = new CMultiDocTemplate(	// normal view
		//IDR_AXISTYPE,
		IDI_CUSTOMER,
		RUNTIME_CLASS(CAxisDoc),
		RUNTIME_CLASS(CChildFrame),	// custom MDI child frame && view
		RUNTIME_CLASS(CAxisView));
	ASSERT_VALID(pDocTemplate);
	axisApp->AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(	// scroll view
		//IDR_AXISTYPE,
		IDI_CUSTOMER,
		RUNTIME_CLASS(CAxisDoc),
		RUNTIME_CLASS(CChildFrame),	// custom MDI child frame && scrollView
		RUNTIME_CLASS(CAxScrollView));
	ASSERT_VALID(pDocTemplate);
	axisApp->AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(	// tab view
		//IDR_AXISTYPE,
		IDI_CUSTOMER,
		RUNTIME_CLASS(CAxisDoc),
		RUNTIME_CLASS(CChildFrame),	// custom MDI child frame && scrollView
		RUNTIME_CLASS(CAxTabView));
	ASSERT_VALID(pDocTemplate);
	axisApp->AddDocTemplate(pDocTemplate);
	return true;
}

#pragma warning (default : 26400)
#pragma warning (default : 26409)