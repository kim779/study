#include "stdafx.h"
#include "C_MBong.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CC_MBongApp, CWinApp)
	//{{AFX_MSG_MAP(CC_MBongApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CC_MBongApp::CC_MBongApp()
{
}

CC_MBongApp theApp;

BOOL CC_MBongApp::InitInstance()
{
	COleObjectFactory::RegisterAll();

	return TRUE;
}

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

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}

CFont* CC_MBongApp::GetFont(CWnd* pView, int point, CString name, int bold, bool italic)
{
	_fontR rFont;
	rFont.name = (char *)name.operator LPCTSTR();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;

	return (CFont*) pView->SendMessage(WM_USER, getFONT, long(&rFont));
}

CPen* CC_MBongApp::GetPen(CWnd* pView, int style, int width, COLORREF clr)
{
	_penR rPen;
	rPen.clr = clr;
	rPen.width = width;
	rPen.style = style;

	return (CPen*) pView->SendMessage(WM_USER, getPEN, long(&rPen));
}

CBrush* CC_MBongApp::GetBrush(CWnd* pView, COLORREF rColor)
{
	return (CBrush*) pView->SendMessage(WM_USER, getBRUSH, rColor);
}

COLORREF CC_MBongApp::GetColor(CWnd *pView, int color)
{
	if (color & 0x02000000)
		return color;

	return pView->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CImageList* CC_MBongApp::GetImage(CWnd* pView, CString path, int Width, int Height, UINT flag, COLORREF crMask)
{
	CBitmap *pBitmap = (CBitmap*) pView->SendMessage(WM_USER, getBITMAP, long(path.operator LPCTSTR()));

	CImageList* pImage = nullptr;
	if (pBitmap)
	{
		pImage = std::make_unique<CImageList>().release();
		pImage->Create(Width, Height, flag, 0, 1);
		pImage->Add(pBitmap, crMask);
	}

	return pImage;
}

