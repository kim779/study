// CX_FODepth.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CX_FODepth.h"

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
// CCX_FODepthApp

BEGIN_MESSAGE_MAP(CCX_FODepthApp, CWinApp)
	//{{AFX_MSG_MAP(CCX_FODepthApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCX_FODepthApp construction

CCX_FODepthApp::CCX_FODepthApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCX_FODepthApp object

CCX_FODepthApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCX_FODepthApp initialization

BOOL CCX_FODepthApp::InitInstance()
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

COLORREF CCX_FODepthApp::GetColor(CWnd *pWizard, int color)
{
	if (color & 0x02000000)
		return color;

	return pWizard->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CPen* CCX_FODepthApp::GetPen(CWnd *pWizard, int color, int width, int style)
{
	struct	_penR	penR{};

	penR.clr   = color;
	penR.width = 1;
	penR.style = style;
	return (CPen *)pWizard->SendMessage(WM_USER, MAKEWPARAM(getPEN, 0), (LPARAM)&penR);
}

CBrush* CCX_FODepthApp::GetBrush(CWnd* pWizard, COLORREF rColor)
{
	return (CBrush*) pWizard->SendMessage(WM_USER, getBRUSH, rColor);
}

CFont* CCX_FODepthApp::GetFont(CWnd *pWizard, int point, CString name, int bold, bool italic)
{
	struct	_fontR	rFont {};

	rFont.name = (char*)name.GetString();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*) pWizard->SendMessage(WM_USER, getFONT, (LPARAM)&rFont);
}

CString CCX_FODepthApp::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}

void CCX_FODepthApp::VariableComma(CString &dispTxt, double dVal, int point, int maxDigit)
{
	CString sval;
	CString	edit1, edit2;
	double	dotL = 0, dotR = 0;
	bool	bSign = false;

	if (dVal < 0)
	{
		bSign = true;
		dVal = dVal * (-1);
	}

	switch (point)
	{
	case 2:
		dotL = (int)(dVal / 100);
		//dotR = (int(dVal) % 100);
		dotR = dVal - dotL*100; 
	
		break;
	default:
		dotL = dVal;
		break;
	}

	dispTxt = _T("");
	edit1.Format("%.f", dotL);
	edit1.MakeReverse();
	for (int ii = 0; ii < edit1.GetLength(); ii++)
	{
		if (ii && !(ii % 3))
			edit2 += ',';
		edit2 += edit1[ii];
	}
	if (bSign) edit2 += "-";
	edit2.MakeReverse();

	edit1= _T("");
	switch (point)
	{
	case 2:
		edit1.Format(".%02d", (int)(dotR+0.5));
		break;
	default:
		break;
	}

	const	int	trimComma = edit2.GetLength() + edit1.GetLength() - maxDigit;
	if (trimComma > 0)
	{
		int trim = 0;
		for (int ii = 0; ii < edit2.GetLength(); ii++)
		{
			if (edit2[ii] == ',' && trim < trimComma)
			{
				trim++;
				continue;
			}
			dispTxt += edit2[ii];
		}
		dispTxt += edit1;
	}
	else
	{
		dispTxt = edit2 + edit1;
	}
}