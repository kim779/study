//CMyFont class implementation
#include "stdafx.h"
#include "MyFont.h"

CMyFont::CMyFont()
{
	lf.lfHeight=-12;
	lf.lfWidth=0;
	lf.lfEscapement=0;
	lf.lfOrientation=0;
	lf.lfWeight=FW_NORMAL;
	lf.lfItalic=0;
	lf.lfUnderline=0;
	lf.lfStrikeOut=0;
	lf.lfCharSet=ANSI_CHARSET;
	lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lf.lfQuality=PROOF_QUALITY;
	lf.lfPitchAndFamily=VARIABLE_PITCH | FF_ROMAN;
	strcpy(lf.lfFaceName, "Times New Roman");

	CreateFontIndirect(&lf);

	fontColor=0;
	hDC=NULL;
}

CMyFont::CMyFont(CString facename, int height)
{
	lf.lfHeight = height;
	lf.lfWidth=0;
	lf.lfEscapement=0;
	lf.lfOrientation=0;
	lf.lfWeight=FW_NORMAL;
	lf.lfItalic=0;
	lf.lfUnderline=0;
	lf.lfStrikeOut=0;
	lf.lfCharSet=ANSI_CHARSET;
	lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lf.lfQuality=PROOF_QUALITY;
	lf.lfPitchAndFamily=VARIABLE_PITCH | FF_ROMAN;
	strcpy(lf.lfFaceName, (LPCTSTR)facename);

	CreateFontIndirect(&lf);

	fontColor=0;
	hDC=NULL;
}

CMyFont::CMyFont(LOGFONT& logfont)
{
	lf=logfont;
	CreateFontIndirect(&lf);
	
	fontColor=0;
	hDC=NULL;
}

CMyFont::CMyFont(CFont font)
{
	HFONT hFont=(HFONT)font;
	Attach((HFONT)hFont);

	GetLogFont(&lf);

	fontColor=0;
	hDC=NULL;
}

CMyFont::~CMyFont()
{
	DeleteObject();
}

LONG CMyFont::SetHeight(LONG height)
{
	LONG l=lf.lfHeight;

	DeleteObject();
	lf.lfHeight=height;
	CreateFontIndirect(&lf);

	return l;
}

LONG CMyFont::SetHeightA(LONG height)
{
	LONG l=lf.lfHeight;

	DeleteObject();
	if (height>0)
		height=0-height;
	lf.lfHeight=height;
	CreateFontIndirect(&lf);

	return l;
}

LONG CMyFont::SetWidth(LONG width)
{
	LONG l=lf.lfWidth;

	DeleteObject();
	lf.lfWidth=width;
	CreateFontIndirect(&lf);

	return l;
}

LONG CMyFont::SetEscapement(LONG esc)
{
	LONG l=lf.lfEscapement;

	DeleteObject();
	lf.lfEscapement=esc;
	CreateFontIndirect(&lf);

	return l;
}

LONG CMyFont::SetOrientation(LONG or)
{
	LONG l=lf.lfOrientation;

	DeleteObject();
	lf.lfOrientation=or;
	CreateFontIndirect(&lf);

	return l;
}

LONG CMyFont::SetWeight(LONG weight)
{
	LONG l=lf.lfWeight;

	DeleteObject();
	lf.lfWeight=weight;
	CreateFontIndirect(&lf);

	return l;
}

BYTE CMyFont::SetCharset(BYTE charset)
{
	BYTE b=lf.lfCharSet;

	DeleteObject();
	lf.lfCharSet=charset;
	CreateFontIndirect(&lf);

	return b;
}

BYTE CMyFont::SetOutPrecision(BYTE op)
{
	BYTE b=lf.lfOutPrecision;

	DeleteObject();
	lf.lfOutPrecision=op;
	CreateFontIndirect(&lf);

	return b;
}

BYTE CMyFont::SetClipPrecision(BYTE cp)
{
	BYTE b=lf.lfClipPrecision;

	DeleteObject();
	lf.lfClipPrecision=cp;
	CreateFontIndirect(&lf);

	return b;
}

BYTE CMyFont::SetQuality(BYTE qual)
{
	BYTE b=lf.lfQuality;

	DeleteObject();
	lf.lfQuality=qual;
	CreateFontIndirect(&lf);

	return b;
}

BYTE CMyFont::SetPitchAndFamily(BYTE paf)
{
	BYTE b=lf.lfPitchAndFamily;

	DeleteObject();
	lf.lfPitchAndFamily=paf;
	CreateFontIndirect(&lf);

	return b;
}

CString CMyFont::SetFaceName(CString facename)
{
	CString str=lf.lfFaceName;

	DeleteObject();
	
	strcpy(lf.lfFaceName, (LPCTSTR)facename);
	CreateFontIndirect(&lf);

	return str;
}

LPCTSTR CMyFont::SetFaceName(LPCTSTR facename)
{
	LPCTSTR str=lf.lfFaceName;

	DeleteObject();
	strcpy(lf.lfFaceName, facename);
	CreateFontIndirect(&lf);

	return str;
}

BOOL CMyFont::SetBold(BOOL B)
{
	BOOL b;

	if (B)
		b=SetWeight(FW_SEMIBOLD);
	else
		b=SetWeight(FW_NORMAL);

	if (b >= FW_MEDIUM)
		return TRUE;
	else
		return FALSE;
}

BOOL CMyFont::SetItalic(BOOL i)
{
	BOOL b=(BOOL)lf.lfItalic;

	DeleteObject();
	lf.lfItalic=i;
	CreateFontIndirect(&lf);

	return b;
}

BOOL CMyFont::SetUnderline(BOOL u)
{
	BOOL b=(BOOL)lf.lfUnderline;

	DeleteObject();
	lf.lfUnderline=u;
	CreateFontIndirect(&lf);

	return b;
}

BOOL CMyFont::SetStrikeOut(BOOL s)
{
	BOOL b=(BOOL)lf.lfStrikeOut;

	DeleteObject();
	lf.lfStrikeOut=s;
	CreateFontIndirect(&lf);

	return b;
}

void CMyFont::SetLogFont(LOGFONT& logfont)
{
	lf=logfont;
	DeleteObject();
	CreateFontIndirect(&lf);
}

LONG CMyFont::GetHeight()
{
	return lf.lfHeight;
}

LONG CMyFont::GetWidth()
{
	return lf.lfWidth;
}

LONG CMyFont::GetEscapement()
{
	return lf.lfEscapement;
}

LONG CMyFont::GetOrientation()
{
	return lf.lfEscapement;
}

LONG CMyFont::GetWeight()
{
	return lf.lfWeight;
}

BYTE CMyFont::GetCharset()
{
	return lf.lfCharSet;
}

BYTE CMyFont::GetOutPrecision()
{
	return lf.lfOutPrecision;
}

BYTE CMyFont::GetClipPrecision()
{
	return lf.lfClipPrecision;
}

BYTE CMyFont::GetQuality()
{
	return lf.lfQuality;
}

BYTE CMyFont::GetPitchAndFamily()
{
	return lf.lfPitchAndFamily;
}

LPCTSTR CMyFont::GetFaceName()
{
	return lf.lfFaceName;
}

BOOL CMyFont::GetBold()
{
	return lf.lfWeight >= FW_MEDIUM ? TRUE : FALSE;
}

BOOL CMyFont::GetItalic()
{
	return (BOOL)lf.lfItalic;
}

BOOL CMyFont::GetUnderline()
{
	return (BOOL)lf.lfUnderline;
}

BOOL CMyFont::GetStrikeOut()
{
	return (BOOL)lf.lfStrikeOut;
}

CString CMyFont::ContractFont()
{
	CString str, color;

	str.Format("%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%s",
		lf.lfHeight,
		lf.lfWidth,
		lf.lfEscapement,
		lf.lfOrientation,
		lf.lfWeight,
		lf.lfItalic,
		lf.lfUnderline,
		lf.lfStrikeOut,
		lf.lfCharSet,
		lf.lfOutPrecision,
		lf.lfClipPrecision,
		lf.lfQuality,
		lf.lfPitchAndFamily,
		lf.lfFaceName);
	color.Format("%u", fontColor);
	str+=",";
	str+=color;

	return str;
}

void CMyFont::ExtractFont(CString& str)
{
	lf.lfHeight=atol((LPCTSTR)GetToken(str, ","));
	lf.lfWidth=atol((LPCTSTR)GetToken(str, ","));
	lf.lfEscapement=atol((LPCTSTR)GetToken(str, ","));
	lf.lfOrientation=atol((LPCTSTR)GetToken(str, ","));
	lf.lfWeight=atol((LPCTSTR)GetToken(str, ","));
	lf.lfItalic=atoi((LPCTSTR)GetToken(str, ","));
	lf.lfUnderline=atoi((LPCTSTR)GetToken(str, ","));
	lf.lfStrikeOut=atoi((LPCTSTR)GetToken(str, ","));
	lf.lfCharSet=atoi((LPCTSTR)GetToken(str, ","));
	lf.lfOutPrecision=atoi((LPCTSTR)GetToken(str, ","));
	lf.lfClipPrecision=atoi((LPCTSTR)GetToken(str, ","));
	lf.lfQuality=atoi((LPCTSTR)GetToken(str, ","));
	lf.lfPitchAndFamily=atoi((LPCTSTR)GetToken(str, ","));
	strcpy(lf.lfFaceName, (LPCTSTR)GetToken(str, ","));

	DeleteObject();
	CreateFontIndirect(&lf);

	fontColor=atol((LPCTSTR)str);
}

CString CMyFont::GetToken(CString& str, LPCTSTR c)
{
	int pos;
	CString token;

	pos=str.Find(c);
	token=str.Left(pos);
	str=str.Mid(pos+1);

	return token;
}

void CMyFont::GetFontFromDialog(CFont *f, DWORD *color,
				CDC *pPrinterDC, CWnd *pParentWnd)
{
	LOGFONT tlf;
	if (f==NULL)
		tlf=lf;
	else
		f->GetLogFont(&tlf);

	CFontDialog dlg(&tlf, CF_EFFECTS | CF_SCREENFONTS,
		pPrinterDC, pParentWnd);
	dlg.m_cf.rgbColors=fontColor;
	
	if (dlg.DoModal()==IDOK)
	{
		dlg.GetCurrentFont(&lf);
		DeleteObject();
		CreateFontIndirect(&lf);
		f=(CFont *)this;
		color=&dlg.m_cf.rgbColors;
		SetFontColor(dlg.m_cf.rgbColors);
	}
}

void CMyFont::SetFontColor(COLORREF color)
{
	fontColor=color;
	if (hDC!=NULL)
		::SetTextColor(hDC, color);
}

COLORREF CMyFont::GetFontColor()
{
	return fontColor;
}

void CMyFont::SetDC(HDC dc)
{
	hDC=dc;
}

void CMyFont::SetMyFont(int height, CString szFaceName)
{
	SetHeight(height);
	SetFaceName(szFaceName);
}


