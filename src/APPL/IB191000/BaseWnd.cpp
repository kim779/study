// BaseWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB191000.h"
#include "BaseWnd.h"
#include "sharemsg.h"
#include "memDc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseWnd

CBaseWnd::CBaseWnd(CWnd* pMainWnd)
{
	m_bBitmap = false;
	m_pMainWnd = pMainWnd;
	m_bInit = FALSE;
}


CBaseWnd::~CBaseWnd()
{
}


BEGIN_MESSAGE_MAP(CBaseWnd, CWnd)
	//{{AFX_MSG_MAP(CBaseWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBaseWnd message handlers

LONG CBaseWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	switch (LOWORD(wParam))
	{
	case MK_SETPAL:
		m_strPal = GetPalette();

		m_clrPallete = (COLORREF)lParam;
		OperPallette();
		Invalidate(FALSE);
		break;
	}
	return ret;
}

void CBaseWnd::OperDraw(CDC* pDC)
{
	pDC->FillSolidRect(m_rect, m_clrPallete);
}


void CBaseWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (m_bBitmap)
	{
		CMemDC	mdc(&dc);
		CFont*	oldfont = mdc.SelectObject(m_pFont);
		OperDraw(&mdc);
		mdc.SelectObject(oldfont);
	}
	else
	{
		CFont*	oldfont = dc.SelectObject(m_pFont);
		OperDraw(&dc);
		dc.SelectObject(oldfont);
	}
}

void CBaseWnd::OperInit()
{

}

int CBaseWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pViewWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	m_root.Format("%s", m_pMainWnd->SendMessage(WM_MANAGE, MK_GETROOT));
	m_user.Format("%s", m_pMainWnd->SendMessage(WM_MANAGE, MK_GETUSER));
	m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_CONFIG); 
	m_pFont = GetAxFont();
	SetFont(m_pFont);
	OperInit();	
	return 0;
}

void CBaseWnd::OperDestory()
{

}

void CBaseWnd::OnDestroy() 
{
	CWnd::OnDestroy();	
	OperDestory();
}

void CBaseWnd::OperResize(int cx, int cy)
{

}

void CBaseWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	m_rect = CRect(0, 0, cx, cy);
	
	if (cx <= 0 || cy <= 0)
		return;

	OperResize(cx, cy);
	m_bInit = TRUE;
}

void CBaseWnd::OperPallette()
{

}


LRESULT CBaseWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_ERASEBKGND)
		return 0;
	return CWnd::WindowProc(message, wParam, lParam);
}

CString CBaseWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");

//  	if( comm == guideCC )
//  	{
//   		MessageBox("["+data+"]", "IBK투자증권");
// 		return retvalue;
//  	}

	char* dta = (char*)m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue.Format("%s", dta);
	
	return retvalue;
}


void CBaseWnd::ShowGuideMessage(CString strMessage)
{	
	Variant(guideCC, strMessage);
}

CPen* CBaseWnd::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pViewWnd->SendMessage(WM_USER, getPEN, (long)&penR);
}

CPen* CBaseWnd::GetAxPen(struct _penR* pen)
{
	return GetAxPen(pen->clr, pen->width, pen->style);	
}

CFont* CBaseWnd::GetAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)m_pViewWnd->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CFont* CBaseWnd::GetAxFont(struct _fontR* font)
{
	return GetAxFont(font->name, font->point, (font->bold == FW_BOLD) ? true : false, font->italic);
}

CBrush* CBaseWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pViewWnd->SendMessage(WM_USER, getBRUSH, (long)clr);
}

COLORREF CBaseWnd::GetAxColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

void CBaseWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind , int nUp)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");

		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");

		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");

		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");

		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

void CBaseWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;

	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;

	memDC.CreateCompatibleDC(pDC);

	if (memDC.m_hDC != NULL) 
		memDC.SelectObject(BitMap);

	::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);

	DeleteObject(BitMap);
	memDC.DeleteDC();
}

CString CBaseWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	CString strPath = Variant(homeCC, "");

	file.Format("%s\\%s\\%s", strPath, "tab", "palette.ini");

	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;

	return strPal;
}