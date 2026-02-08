// IB410100.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <math.h>
#include "IB410100.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "message.h"

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
// CIB410100App

BEGIN_MESSAGE_MAP(CIB410100App, CWinApp)
	//{{AFX_MSG_MAP(CIB410100App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIB410100App construction

CIB410100App::CIB410100App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIB410100App object

CIB410100App theApp;


CFont* CIB410100App::GetFont(CWnd *pView, int point, int bold, CString name, bool italic)
{
	_fontR rFont;
	rFont.name = (char *)name.operator LPCTSTR();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*) pView->SendMessage(WM_USER, getFONT, long(&rFont));
}

CPen* CIB410100App::GetPen(CWnd* pView, COLORREF clr, int style, int width)
{
	_penR rPen;
	rPen.clr = clr;
	rPen.width = width;
	rPen.style = style;
	return (CPen*) pView->SendMessage(WM_USER, getPEN, long(&rPen));
}

CBrush* CIB410100App::GetBrush(CWnd* pView, COLORREF rColor)
{
	return (CBrush*) pView->SendMessage(WM_USER, getBRUSH, rColor);
}

CBitmap* CIB410100App::GetBitmap(CWnd *pView, CString path)
{
	return (CBitmap*)pView->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

COLORREF CIB410100App::GetIndexColor(CWnd *pView, int index)
{
	if (index & 0x02000000)		return index;

	return pView->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)index);
}

LOGFONT CIB410100App::GetLogFont(int Size, bool bBold, CString fName)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = Size*10; 
	
	strcpy(lf.lfFaceName, fName);   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;

	lf.lfWeight = (bBold ? FW_BOLD : FW_NORMAL);
	lf.lfItalic = FALSE;

	return lf;
}

CString CIB410100App::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

CString CIB410100App::FillSpace(CString input, int len)
{
	CString buf = input;
	const int	bufsize = buf.GetLength();
	if (len < bufsize)
	{
		buf = buf.Left(len);
	}
	for (int ii = 0; ii < len - bufsize; ii++)
	{
		buf += " ";// 0x20;
	}
	return buf;
}

CString CIB410100App::CommaModify(CString string)
{
	CString stripData = string;
	stripData.TrimLeft(); stripData.TrimRight();

	if (stripData.GetLength() <= 3)
		return stripData;

	char dChar = 0x00;
	switch (stripData[0])
	{
	case '+':
		dChar = '+'; stripData.Delete(0);
		break;
	case '-':
		dChar = '-'; stripData.Delete(0);
		break;
	}

	stripData.MakeReverse();
	const int	len = stripData.GetLength();
	string = _T("");
	for (int ii = 0; ii < len; ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			string += ',';

		string += stripData[ii];
	}

	string.MakeReverse();
	if (dChar != 0x00) string.Insert(0, dChar);

	return string;
}

CString CIB410100App::StringModify(int data, int kind)
{
	CString rValue;
	switch (kind)
	{
	case 0:
		rValue.Format("%d", data);
		break;
	case 2:
		rValue.Format("%02d", data);
		break;
	case 3:
		rValue.Format("%03d", data);
		break;
	}

	rValue.Format("%d", data);

	return rValue;
}


CString CIB410100App::StringModify(double data, int dPoint)
{
	CString rValue;

	switch (dPoint)
	{
	case 0:
		rValue.Format("%.lf", data);
		break;
	case 1:
		rValue.Format("%.1lf", data);
		break;
	case 3:
		rValue.Format("%.3lf", data);
		break;
	case 4:
		rValue.Format("%.4lf", data);
		break;
	default:
		rValue.Format("%.2lf", data);
		break;
	}

	return rValue;
}

double CIB410100App::BuhoModify(CString sData)
{
	double rValue = atof(sData);

	if (sData.GetLength())
	{
		if (sData.GetAt(0) == _T('+') || sData.GetAt(0) == _T('-'))
		{
			sData.Delete(0);
		}
		
		rValue = atof(sData);
	}

	return rValue;
}

CString CIB410100App::JisuToStrike(double ajisu)
{
	CString jisu, result;
	double dval=0, dsum=0, dresult=0;

	dval = ajisu - floor(ajisu/10)*10;
	dsum = ajisu - dval;

	if (dval < 2.5)
	{
		if (dval - 0 > 2.5 - dval)
			dresult = 2.5;
		else
			dresult = 0;
	}
	else if (dval < 5.0)
	{
		if (dval - 2.5 > 5.0 - dval)
			dresult = 5.0;
		else
			dresult = 2.5;
	}
	else if (dval < 7.5)
	{
		if (dval - 5.0 > 7.5 - dval)
			dresult = 7.5;
		else
			dresult = 5.0;
	}
	else
	{
		if (dval - 7.5 > 10 - dval)
			dresult = 10;
		else
			dresult  = 7.5;
	}

	result.Format("%.0f", floor(dsum + dresult));
	if (result[result.GetLength()-1] == '2' || result[result.GetLength()-1] == '7')
		result += ".5";
	else
		result += ".0";
	return result;
}

int CIB410100App::ExitInstance() 
{
	return CWinApp::ExitInstance();
}

double CIB410100App::getOptionStrike(CString opCode)
{
	double strike;

	strike = atof(opCode.Mid(5, 3));
	if (opCode[7] == '2' || opCode[7] == '7')
		strike = strike + 0.5;

	return strike;
}

