// ColorPickerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Dx_ColorPicker.h"
#include "ColorPickerWnd.h"
#include "DataDef.h"
#include "../../control/fx_grid/memdc.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorPickerWnd

CColorPickerWnd::CColorPickerWnd()
{
	EnableAutomation();
	m_sSession.Empty();
	m_sRoot.Empty();
	m_sUser.Empty();
	m_sFile.Empty();
	m_pParent = nullptr;
	m_ctlLogo = nullptr;
	m_clrSelColor = -1;
}

CColorPickerWnd::~CColorPickerWnd()
{
}

void CColorPickerWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.
	
	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CColorPickerWnd, CWnd)
	//{{AFX_MSG_MAP(CColorPickerWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CColorPickerWnd, CWnd)
//{{AFX_DISPATCH_MAP(CColorPickerWnd)
	DISP_FUNCTION(CColorPickerWnd, "setName", SetName, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CColorPickerWnd, "refresh", refresh, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IColorPickerWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {A60C3483-0472-461D-8F29-5F7B7CE8BC6B}
static const IID IID_IColorPickerWnd =
{ 0xa60c3483, 0x472, 0x461d, { 0x8f, 0x29, 0x5f, 0x7b, 0x7c, 0xe8, 0xbc, 0x6b } };

BEGIN_INTERFACE_MAP(CColorPickerWnd, CWnd)
INTERFACE_PART(CColorPickerWnd, IID_IColorPickerWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPickerWnd message handlers

void CColorPickerWnd::SetParam(_param *pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = pParam->name;
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = pParam->fonts;
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = pParam->options;
	
	CString tmp = m_Param.options;
	
	m_sSession = "abc";//OptionParser(tmp, "/s");
	m_sKey = "first";	// OptionParser(tmp, "/k");

	CheckSession();		// ini File Session-Value Check

	m_sUser = Variant(nameCC, "");
	m_sRoot = Variant(homeCC, "");
	
	m_sFile.Format("%s/%s/%s/%s", m_sRoot, USRDIR, m_sUser, TARGET_FILE);
}

BOOL CColorPickerWnd::CheckSession()
{
	BOOL ret = TRUE;
	if (m_sSession.IsEmpty())
	{
		AfxMessageBox("Fill in Session Name-Field!", MB_ICONSTOP);
		ret = FALSE;
	}

	if (m_sKey.IsEmpty())
	{
		AfxMessageBox("Fill in Key Name-Field!", MB_ICONSTOP);
		ret = FALSE;
	}

	return ret;
}

CString CColorPickerWnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;
	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}


CBrush* CColorPickerWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pParent->SendMessage(WM_USER, getBRUSH, (LPARAM)clr);
}

CFont* CColorPickerWnd::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR {};

	fontR.name = (LPSTR)fName.GetString();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	switch(style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (LPARAM)&fontR);
}

CPen* CColorPickerWnd::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR	penR {};

	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pParent->SendMessage(WM_USER, getPEN, (LPARAM)&penR);
}

CBitmap* CColorPickerWnd::getBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (LPARAM)path.GetString());
}

CString CColorPickerWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());
	
	if ((long)dta > 1)
		retvalue = dta;
	
	return retvalue;
}

CString CColorPickerWnd::Parser(CString &srcstr, CString substr)
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

LOGFONT CColorPickerWnd::setFont(CString fName, int fsize, int style)
{
	LOGFONT	lf{};

	lf.lfHeight = fsize * 10; 
	CopyMemory(lf.lfFaceName, fName, fName.GetLength());
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	switch(style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		lf.lfItalic = TRUE;
		break;
	case 3: // bold
		lf.lfWeight = FW_BOLD;
		break;
	case 4: // both
		lf.lfItalic = TRUE;
		lf.lfWeight = FW_BOLD;
		break;
	}
	
	return lf;
}

COLORREF CColorPickerWnd::GetColorData()
{
	char	wb[32]{};
	CString	sRed, sGreen, sBlue, sData;

	if (CheckSession() == FALSE)
		PostMessage(WM_CLOSE, 0, 0);
	else
	{
		const	DWORD	size = GetPrivateProfileString(m_sSession, m_sKey, DEFAULT_COLOR, wb, sizeof(wb), m_sFile);
		sData = CString(wb, size);
		if (sData == DEFAULT_COLOR && m_clrSelColor != -1)
			return m_clrSelColor;

		sRed = Parser(sData, SIGN_SEPER);
		sGreen = Parser(sData, SIGN_SEPER);
		sBlue = sData;
	}

	return PALETTERGB(atoi(sRed), atoi(sGreen), atoi(sBlue));
}

void CColorPickerWnd::SetColorData(COLORREF color)
{
	if (CheckSession() == FALSE)
		PostMessage(WM_CLOSE, 0, 0);
	else
	{		
		CString	sData;

		const	BYTE	r = (BYTE) (color);
		const	BYTE	g = (BYTE) (color >> 8);
		const	BYTE	b = (BYTE) (color >> 16);

		sData.Format("%d, %d, %d", r, g, b);		

		WritePrivateProfileString(m_sSession, m_sKey, sData, m_sFile);
	}
}

int CColorPickerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_clrSelColor = m_Param.tRGB;		// Color
	CRect rect;
	rect.SetRect(0, 0, m_Param.rect.Width(), m_Param.rect.Height());

	m_ctlLogo = std::make_unique<CColorButton>();
	m_ctlLogo->Create("", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rect, this, ID_BUTTON);

//	m_ctlLogo->Color		= ::GetSysColor(COLOR_BACKGROUND);
	m_ctlLogo->Color		= GetColorData();
	m_ctlLogo->DefaultColor		= ::GetSysColor(COLOR_APPWORKSPACE);
	m_ctlLogo->TrackSelection	= TRUE;
	m_ctlLogo->CustomText		= _T("사용자 정의");
	m_ctlLogo->DefaultText		= _T("자동");

	return 0;
}

void CColorPickerWnd::OnDestroy() 
{
	m_ctlLogo.reset();
	CWnd::OnDestroy();	
}

LONG CColorPickerWnd::OnSelEndOK(UINT lParam, LONG wParam)
{
	m_clrSelColor = (DWORD)lParam;
	SetColorData(m_clrSelColor);		// Save Color

	return TRUE;
}

void CColorPickerWnd::SetName(LPCTSTR sSession, LPCTSTR sKey) 
{
	m_sSession = sSession;
	m_sKey = sKey;

	CheckSession();	
}

void CColorPickerWnd::refresh() 
{
	m_ctlLogo->SetColor(GetColorData());
}
