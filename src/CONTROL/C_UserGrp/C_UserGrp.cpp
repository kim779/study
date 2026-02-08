// C_UserGrp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "C_UserGrp.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CC_UserGrpApp

BEGIN_MESSAGE_MAP(CC_UserGrpApp, CWinApp)
	//{{AFX_MSG_MAP(CC_UserGrpApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC_UserGrpApp construction

CC_UserGrpApp::CC_UserGrpApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_hDlg = nullptr;
	m_pView = nullptr;
	m_pImgCtrl = nullptr;

}

CC_UserGrpApp::~CC_UserGrpApp()
{
	if (m_pImgCtrl)
	{
		m_pImgCtrl->DeleteImageList();	
	}

	//if (m_hDlg)	FreeLibrary(m_hDlg);
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CC_UserGrpApp object

CC_UserGrpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CC_UserGrpApp initialization

BOOL CC_UserGrpApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}

char* CC_UserGrpApp::GetSampleData(int pInfo1, int pInfo2, int *pLen)
{
	if (!m_hDlg)
		m_hDlg = LoadLibrary("C_UserGrpDlg.dll");

	if (m_hDlg)
	{
		char*	(APIENTRY *axGetSample)(int, int, int *);
		axGetSample = (char* (APIENTRY *)(int, int, int *))
			GetProcAddress(m_hDlg, "axGetSample");

		if (axGetSample)
			return axGetSample(pInfo1, pInfo2, pLen);
	}

	return nullptr;
}

bool CC_UserGrpApp::CreateImage()
{
	if (m_pImgCtrl)	return true;

	if (!m_pView)	return false;

	CString	path;
	CString	root = (char *)m_pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
	path.Format("%s\\%s\\GTC_CTRL.bmp", root, IMAGEDIR);

	m_pImgCtrl = getImage(path, 16, 16, ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
	if (m_pImgCtrl)	return true;

	return false;
}

std::shared_ptr<CImageList> CC_UserGrpApp::getImage(CString path, int Width, int Height, UINT flag, COLORREF crMask)
{
	HBITMAP hbitmap = (HBITMAP) LoadImage(nullptr, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
	{
		CBitmap	bitmap;

		bitmap.DeleteObject();
		bitmap.Attach(hbitmap);
		auto pImage = std::make_shared<CImageList>();
		pImage->Create(Width, Height, flag, 0, 1);
		pImage->Add(&bitmap, crMask);
		bitmap.DeleteObject();
		return pImage;

	}
	return nullptr;
}

CFont* CC_UserGrpApp::GetFont(CWnd* pView, int point, CString name, int bold, bool italic)
{
	_fontR rFont;
	rFont.name = (char *)name.operator LPCTSTR();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*) pView->SendMessage(WM_USER, getFONT, long(&rFont));
}

CPen* CC_UserGrpApp::GetPen(CWnd* pView, int style, int width, COLORREF clr)
{
	_penR rPen;
	rPen.clr = clr;
	rPen.width = width;
	rPen.style = style;
	return (CPen*) pView->SendMessage(WM_USER, getPEN, long(&rPen));
}

CBrush* CC_UserGrpApp::GetBrush(CWnd* pView, COLORREF rColor)
{
	return (CBrush*) pView->SendMessage(WM_USER, getBRUSH, rColor);
}

COLORREF CC_UserGrpApp::GetColor(CWnd *pView, int index)
{
	if (index & 0x02000000)
		return index;

	return pView->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)index);
}

int CC_UserGrpApp::ExitInstance() 
{
	if (m_hDlg)	FreeLibrary(m_hDlg);
	
	return CWinApp::ExitInstance();
}
