// C_TickEx.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "C_TickEx.h"

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
// CC_TickExApp

BEGIN_MESSAGE_MAP(CC_TickExApp, CWinApp)
	//{{AFX_MSG_MAP(CC_TickExApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC_TickExApp construction

CC_TickExApp::CC_TickExApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pImgCtrl = nullptr;
}

CC_TickExApp::~CC_TickExApp()
{
	if (m_pImgCtrl)
		m_pImgCtrl->DeleteImageList();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CC_TickExApp object

CC_TickExApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CC_TickExApp initialization

BOOL CC_TickExApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	m_Path = _T("");

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

#include "../../h/axisvar.h"
void CC_TickExApp::Init(CWnd *pView)
{
	CString	root = (char *)pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
	CString name = (char *)pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), 0);

	if (m_Path.IsEmpty())
		m_Path.Format("%s\\%s\\%s\\", root, USRDIR, name);
}

bool CC_TickExApp::ReadCondition(CString path, struct _tickinfo *tickinfo)
{
	return readFile(path, (char*)tickinfo, sz_TICKINFO);
}

void CC_TickExApp::SaveCondition(CString path, struct _tickinfo *tickinfo)
{
	writeFile(path, (char*)tickinfo, sz_TICKINFO);
}

bool CC_TickExApp::readFile(CString path, char *pBytes, int nBytes)
{
	CFile f;
	CFileException e;

	if( !f.Open( path, CFile::modeRead, &e ) )
	{
		TRACE("[READ] %s file open error(%d)\n", path, e.m_cause);
		return false;
	}

	const UINT nByteRead = f.Read(pBytes, nBytes);

	if (nByteRead != UINT(nBytes))
	{
		f.Close();
		return false;
	}
	f.Close();

	return true;
}

bool CC_TickExApp::writeFile(CString path, char *pBytes, int nBytes)
{
	CFile f;
	CFileException e;

	if( !f.Open( path, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		TRACE("[WRITE] %s file open error(%d)\n", path, e.m_cause);
		return false;
	}

	f.Write(pBytes, nBytes);
	f.Close();

	return true;
}

bool CC_TickExApp::CreateImage(CWnd *pView)
{
	if (m_pImgCtrl)	return true;

	if (!pView)	return false;

	CString	path;
	CString	root = (char *)pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
	path.Format("%s\\%s\\GTC_CTRL.bmp", root, IMAGEDIR);

	m_pImgCtrl = getImage(path, 16, 16, ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
	if (m_pImgCtrl)	return true;

	return false;
}

std::shared_ptr<CImageList> CC_TickExApp::getImage(CString path, int Width, int Height, UINT flag, COLORREF crMask)
{
	//CImageList* pImage = nullptr;
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

//CImageList* CC_TickExApp::GetImage(CWnd *pView, CString path, int Width, int Height, UINT flag, COLORREF crMask)
//{
//	CBitmap *pBitmap = (CBitmap*) pView->SendMessage(WM_USER, getBITMAP, long(path.operator LPCTSTR()));
//
//	CImageList* pImage = nullptr;
//	if (pBitmap)
//	{
//		pImage = new CImageList();
//		pImage->Create(Width, Height, flag, 0, 1);
//		pImage->Add(pBitmap, crMask);
//	}
//
//	return pImage;
//}

COLORREF CC_TickExApp::GetColor(CWnd *pView, int color)
{
	if (color & 0x02000000)
		return color;

	return pView->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CFont* CC_TickExApp::GetFont(CWnd *pView, int point, CString name, int bold, bool italic)
{
	_fontR rFont;
	rFont.name = (char *)name.operator LPCTSTR();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*) pView->SendMessage(WM_USER, getFONT, long(&rFont));
}

CPen* CC_TickExApp::GetPen(CWnd *pView, int style, int width, COLORREF clr)
{
	_penR rPen;
	rPen.clr = clr;
	rPen.width = width;
	rPen.style = style;
	return (CPen*) pView->SendMessage(WM_USER, getPEN, long(&rPen));
}

CBrush* CC_TickExApp::GetBrush(CWnd *pView, COLORREF rColor)
{
	return (CBrush*) pView->SendMessage(WM_USER, getBRUSH, rColor);
}
