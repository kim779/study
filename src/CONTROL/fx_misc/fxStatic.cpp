// fxStatic.cpp : implementation file
//

#include "stdafx.h"
#include "fxStatic.h"
#include "memdc.h"
#include "misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	idFLASH		10000

#define	cUP		0x18
#define	cDOWN		0x19
#define	cFLAT		0x1d
#define	cDELTA		0x1e

const COLORREF upCOLOR = PALETTERGB(255,   0,   0);
const COLORREF dnCOLOR = PALETTERGB(  0,   0, 255);
const COLORREF eqCOLOR = PALETTERGB(  0,   0,   0);

#define	ARR_UP		"¡è"
#define	ARR_DN		"¡é"
#define	TRI_UP		"¡ã"
#define	TRI_DN		"¡å"

/////////////////////////////////////////////////////////////////////////////
// CfxStatic

CfxStatic::CfxStatic(CFont* pFont/*=nullptr*/)
{
	m_pFont = pFont;

	m_style     = ssTEXT|ssCOLOR;
	m_text      = _T("");
	m_alignment = alCENTER;
	m_clrBACK   = GetSysColor(COLOR_3DFACE);
	m_clrBACK2  = GetSysColor(COLOR_3DFACE);
	m_clrFORE   = GetSysColor(COLOR_WINDOWTEXT);
	m_clrBORDER = RGB(0, 0, 0);
	m_orgBACK   = m_clrBACK;
	m_brBACK.CreateSolidBrush(m_clrBACK);
	m_brBACK2.CreateSolidBrush(m_clrBACK2);
	m_brBORDER.CreateSolidBrush(m_clrBORDER);

	m_flash     = false;
	m_attr      = 0;
	m_float     = 0;
	m_parent    = (CWnd *) 0;

	m_upCOL = upCOLOR;
	m_dnCOL = dnCOLOR;
	m_eqCOL = eqCOLOR;

	m_bCornerRound = FALSE;
	for(int i=0; i<4; i++)
		m_hRoundBitmap[i] = nullptr;
	m_iRoundPoint = 5;

	m_iRoundTick[0] = 0;
	m_iRoundTick[1] = 0;
	m_iRoundTick[2] = 0;
	m_iRoundTick[3] = 0;
	m_hBitmap = nullptr;

	m_BitmapSize = CSize(0,0);

}

CfxStatic::~CfxStatic()
{
	m_brBACK.DeleteObject();
	m_brBACK2.DeleteObject();
	m_brBORDER.DeleteObject();
}


BEGIN_MESSAGE_MAP(CfxStatic, CStatic)
	//{{AFX_MSG_MAP(CfxStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_WM_PAINT()	
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfxStatic message handlers

BOOL CfxStatic::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_text   = lpszText;
	m_parent = pParentWnd;

	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

void CfxStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CStatic::OnLButtonDown(nFlags, point);
	if (m_parent)
		m_parent->PostMessage(WM_LBUTTONDOWN, (WPARAM)nFlags, 
			(LPARAM)MAKELPARAM(point.x, point.y));
}

void CfxStatic::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CStatic::OnRButtonDown(nFlags, point);
	if (m_parent)
		m_parent->PostMessage(WM_RBUTTONDOWN, (WPARAM)nFlags, 
		(LPARAM)MAKELPARAM(point.x, point.y));
}

void CfxStatic::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);
	m_parent->SetFocus();	
}

void CfxStatic::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case idFLASH:
		KillTimer(idFLASH);
		m_flash = false;
		SetBkColor(m_orgBACK);
		break;
	}
	CStatic::OnTimer(nIDEvent);
}

void CfxStatic::SetFloat(int float_size, bool redraw)
{
	m_float = float_size;
	if (redraw)
		Invalidate();
}

int CfxStatic::GetFloat()
{
	return m_float;
}

void CfxStatic::Init(DWORD style, DWORD attr, CString text, CString images, COLORREF clrBACK, COLORREF clrBACK2, COLORREF clrFORE,
			     BYTE alignment, int float_size)
{
	bool	redraw = false;

	m_attr  = attr;
	m_float = float_size;

	m_alignment = alignment;
	ModifyStyle(SS_LEFT|SS_CENTER|SS_RIGHT, 0);
	switch (alignment)
	{
	case alLEFT:
		ModifyStyle(0, SS_LEFT);	break;
	case alRIGHT:
		ModifyStyle(0, SS_RIGHT);	break;
	case alCENTER:
	default:
		ModifyStyle(0, SS_CENTER);	break;
	}

	if (m_style != style)
	{
		m_style = style;
		redraw  = true;
	}

//	ModifyStyle(SS_BITMAP, SS_CENTERIMAGE);
	if (m_style & ssCOLOR || m_style & ssGRADIENT)
	{
		m_clrBACK = clrBACK;
		m_clrBACK2 = clrBACK2;
		m_clrFORE = clrFORE;
		redraw    = true;
	}
	if (m_style & ssTEXT)
	{
		m_text = makeData(text);
		redraw = true;
	}
	if (m_style & ssIMAGE)
	{
		ModifyStyle(0, SS_BITMAP);
		SetBitmap(images, false);
		redraw = true;
	}
	if (redraw)
		Invalidate();
}

void CfxStatic::SetStyle(DWORD style, bool redraw)
{
	m_style = style;

	ModifyStyle(SS_BITMAP, SS_CENTERIMAGE);
	if (m_style & ssIMAGE)
		ModifyStyle(0, SS_BITMAP);

	if (redraw)
		Invalidate();
}

void CfxStatic::SetText(CString text, bool redraw)
{
//	if (!m_text.CompareNoCase(text))
//		return;

	m_text = makeData(text);
	if (redraw)
		Invalidate();
}

CString CfxStatic::GetText()
{
	CString text = m_text;
	if (m_attr & attrCOMMA)
		text.Remove(',');
	return text;
}

void CfxStatic::SetBitmap(CString path, bool redraw)
{
	HBITMAP hbitmap = (HBITMAP) LoadImage(nullptr, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
		CStatic::SetBitmap(hbitmap);

	if (redraw)
		Invalidate();
}

void CfxStatic::SetBkColor(COLORREF clrBACK, bool redraw)
{
	if (m_clrBACK == clrBACK)
		return;

	m_clrBACK = clrBACK;
	m_brBACK.DeleteObject();
	m_brBACK.CreateSolidBrush(m_clrBACK);

	if (redraw)
		Invalidate();
}

void CfxStatic::SetBk2Color(COLORREF clrBACK, bool redraw)
{
	if (m_clrBACK2 == clrBACK)
		return;

	m_clrBACK2 = clrBACK;
	m_brBACK2.DeleteObject();
	m_brBACK2.CreateSolidBrush(m_clrBACK2);

	if (redraw)
		Invalidate();
}

void CfxStatic::SetFgColor(COLORREF clrFORE, bool redraw)
{
	if (m_clrFORE == clrFORE)
		return;
	m_clrFORE = clrFORE;
	if (redraw)
		Invalidate();
}

void CfxStatic::SetBorderColor(COLORREF clrBORDER, bool redraw)
{
	if (m_clrBORDER == clrBORDER)
		return;
	m_clrBORDER = clrBORDER;
	m_brBORDER.DeleteObject();
	m_brBORDER.CreateSolidBrush(m_clrBORDER);

	if (redraw)
		Invalidate();
}


COLORREF CfxStatic::GetBkColor()
{
	return m_clrBACK;
}

COLORREF CfxStatic::GetBk2Color()
{
	return m_clrBACK2;
}

COLORREF CfxStatic::GetFgColor()
{
	return m_clrFORE;
}

COLORREF CfxStatic::GetBorderColor()
{
	return m_clrBORDER;
}

void CfxStatic::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	if (redraw) Invalidate();
}

void CfxStatic::SetAlignment(BYTE alignment, bool redraw)
{
	m_alignment = alignment;
	ModifyStyle(SS_LEFT|SS_CENTER|SS_RIGHT, 0);
	switch (alignment)
	{
	case alLEFT:
		ModifyStyle(0, SS_LEFT);	break;
	case alRIGHT:
		ModifyStyle(0, SS_RIGHT);	break;
	case alCENTER:
	default:
		ModifyStyle(0, SS_CENTER);	break;
	}

	if (redraw)
		Invalidate();
}

void CfxStatic::ModifyAttribute(DWORD attrRM, DWORD attrADD)
{
	m_attr &= ~attrRM;
	m_attr |= attrADD;
}

void CfxStatic::Blink(COLORREF color, int interval)
{
	if (m_flash)
	{
		KillTimer(idFLASH);
		SetTimer(idFLASH, interval, nullptr);
		return;
	}

	m_orgBACK = GetBkColor();
	m_flash   = true;
	SetBkColor(color);
	SetTimer(idFLASH, interval, nullptr);
}

CString CfxStatic::makeData(CString text)
{
	if (text.IsEmpty() || !(m_attr & attrNUMERIC))
		return text;

	int	pos{};
	char	sign = '\0';
	CString	string, tmps;
	CString left, right;
	
	string = tmps = _T("");
	
	char contrast = 0x00;
	if (m_attr & attrCONTRAST && !text.IsEmpty())
	{
		contrast = text.GetAt(0);
		text = text.Mid(1);
	}
	else
	{
		pos = text.FindOneOf("+-");
		if (pos >= 0)
		{
			sign = text.GetAt(pos);
			text = text.Mid(pos+1);
		}
	}

	if (m_attr & attrZEROSUP)
	{
		while (text.GetLength())
		{
			if (text[0] == _T('0'))
			{
				text.Delete(0);
				continue;
			}
			break;
		}

		if (text.IsEmpty())
			text += _T("0");
	}

	text.TrimLeft();
	text.TrimRight();

	if (m_attr & attrFLOAT && m_float > 0)
	{
		if (text.GetLength() <= m_float)
		{
			left = "0." + CString('0', m_float - text.GetLength());
			text = left + text;
		}
		else
		{
			
			right = "." + text.Right(m_float);
			left = text.Left(text.GetLength() - m_float);
			text = left + right;
		}
	}

	if (m_attr & attrCOMMA && !text.IsEmpty())
	{
		right = _T("");
		if ((pos = text.Find('.')) > 0)
		{
			right = text.Mid(pos);
			text = text.Left(pos);
		}
			
		text.MakeReverse();
		for (int ii = 0; ii < text.GetLength(); ii++)
		{
			if (ii && !(ii % 3))
				tmps += ',';
			tmps += text[ii];
		}
		tmps.MakeReverse();
		text = tmps + right;
	}

	if (m_attr & (attrCORGB | attrCORGBx | attrCORGBm))
	{
		m_clrFORE = m_eqCOL;
		if (sign != '\0')
		{
			
			if (sign == '+')
				m_clrFORE = m_upCOL;
			else if (sign == '-')
				m_clrFORE = m_dnCOL;
			else
				m_clrFORE = m_eqCOL;
		}
		if ((m_attr & attrCORGB) && sign != '\0')
			text.Insert(0, sign);
		if ((m_attr & attrCORGBm) && sign == '-')
			text.Insert(0, sign);
	}
	else
	{
		if (sign == '-')
			text.Insert(0, sign);
	}
	if (contrast != 0x00)
		string = contrast+text;
	else
		string = text;

	return string;

}

CString CfxStatic::commaModify(CString string)
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
	string = _T("");
	for (int ii = 0; ii < stripData.GetLength(); ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			string += ',';

		string += stripData[ii];
	}

	string.MakeReverse();
	if (dChar != 0x00) string.Insert(0, dChar);
	return string;
}

void CfxStatic::OnPaint() 
{
	CPaintDC dc(this);
	
	CRect	rect, rc;
	CString	string;
	UINT	format = DT_SINGLELINE|DT_VCENTER;
	CGdiObject* font{};

	GetClientRect(&rect);
	rc = rect;
	if (m_style & ssGRADIENT)
	{
		CRect rcx;
		int rs{}, gs{}, bs{}, re{}, ge{}, be{};
		float rStep{}, gStep{}, bStep{};
		CBrush brush;


		rs = GetRValue(m_clrBACK); gs = GetGValue(m_clrBACK); bs = GetBValue(m_clrBACK);
		re = GetRValue(m_clrBACK2); ge = GetGValue(m_clrBACK2); be = GetBValue(m_clrBACK2);

		rStep = float(rs - re)/float(rect.Height());
		gStep = float(gs - ge)/float(rect.Height());
		bStep = float(bs - be)/float(rect.Height());

		for (int ii = 0; ii < rect.Height(); ii++)
		{
			rcx.SetRect(rect.left, rect.top + ii, rect.right, rect.top + ii + 1);
   
			brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
			dc.FillRect(&rcx, &brush); brush.DeleteObject();
		}
	} else
		dc.FillSolidRect(rect, m_clrBACK);
	
	string = m_text;
	const int mode = dc.SetBkMode(TRANSPARENT);
	const COLORREF clr = dc.SetTextColor(m_clrFORE);

	if (m_pFont != nullptr) font = dc.SelectObject(m_pFont);
	else font = dc.SelectStockObject(SYSTEM_FONT);

	if (m_style & ssBORDER)
	{ 
		dc.FrameRect(rect, &m_brBORDER);
	}

	switch (m_alignment)
	{
	case alLEFT:
		format |= DT_LEFT;	break;
	case alRIGHT:
		format |= DT_RIGHT;	break;
	case alCENTER:
	default:
		format |= DT_CENTER;	break;
	}

	if ((m_attr & attrCONTRAST) == attrCONTRAST && !string.IsEmpty())
	{
		COLORREF fgcol = dnCOLOR;
		CString	temps;
		switch (string[0])
		{
		case '1':
			temps = ARR_UP; fgcol = m_upCOL; break;
		case '+':
		case '2':
			temps = TRI_UP; fgcol = m_upCOL; break;
		case '4':
			temps = ARR_DN; fgcol = m_dnCOL; break;
		case '-':
		case '5':
			temps = TRI_DN; fgcol = m_dnCOL; break;
		default:
			temps = _T("  "); fgcol = m_eqCOL; break;
		}

		const CSize size = dc.GetOutputTextExtent(temps);

		dc.SetTextColor(fgcol);
		const int topx = (rect.Height() - size.cy) / 2;
		CRect	contRC(rect.left+3, rect.top+topx, rect.left+3+size.cx, rect.top+topx+size.cy);
		dc.DrawText(temps, contRC, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

		string = string.Mid(1);
		rect.left += size.cx;
	}
	rect.DeflateRect(2, 0);
	int pos = 0;
	if ((pos = string.Find('&')) > 0)
		string.Insert(pos, '&');

	if(m_bCornerRound && m_BitmapSize.cx)
		rect.left += (m_BitmapSize.cx + m_iLeftSpace);

	dc.DrawText(string, rect, format);

	if(m_bCornerRound)
	{
		CRect rc2 = rc;
		rc2.DeflateRect(m_iRoundTick[0], m_iRoundTick[1], m_iRoundTick[2], m_iRoundTick[3]);
		if(m_iRoundTick[0] > 0)
			dc.FillSolidRect(CRect(0, 0,  m_iRoundTick[0], rc.bottom), m_clrBORDER); 

		if(m_iRoundTick[1] > 0)
			dc.FillSolidRect(CRect(0, 0,  rc.right, m_iRoundTick[1]), m_clrBORDER); 

		if(m_iRoundTick[2] > 0)
			dc.FillSolidRect(CRect(rc.right-m_iRoundTick[2], 0,  rc.right, rc.bottom), m_clrBORDER); 

		if(m_iRoundTick[3] > 0)
			dc.FillSolidRect(CRect(0, rc.bottom-m_iRoundTick[3],  rc.right, rc.bottom), m_clrBORDER); 

		rc.DeflateRect(m_iRoundTick[0], m_iRoundTick[1], m_iRoundTick[2], m_iRoundTick[3]); 
		
		CRect rr = rc;
		rr.DeflateRect(1,1,1,1); 

		DrawBitmap(&dc, CRect(rc.left, rc.top, rc.left+m_iRoundPoint, rc.top +m_iRoundPoint), m_hRoundBitmap[0]);
		DrawBitmap(&dc, CRect(rc.right-m_iRoundPoint, rc.top, rc.right, rc.top +m_iRoundPoint), m_hRoundBitmap[1]);
		DrawBitmap(&dc, CRect(rc.left, rc.bottom-m_iRoundPoint, rc.left+m_iRoundPoint, rc.bottom), m_hRoundBitmap[2]);
		DrawBitmap(&dc, CRect(rc.right-m_iRoundPoint, rc.bottom-m_iRoundPoint, rc.right, rc.bottom), m_hRoundBitmap[3]);
	
		rr.top = max(2, (rr.Height() - m_BitmapSize.cy ) / 2);
		rr.left = m_iLeftSpace;
		
		rr.right = rr.left + m_BitmapSize.cx;
		rr.bottom = rr.top + m_BitmapSize.cy;

		DrawBitmap(&dc, rr, m_hBitmap);
	}

	dc.SelectObject(font);
	dc.SetTextColor(clr);
	dc.SetBkMode(mode);
}

LRESULT CfxStatic::messageToParent(int nMessage, CString text)
{
	if (!IsWindow(m_hWnd))
		return 0;

	NM_STATIC nmst;
	nmst.hdr.hwndFrom = GetSafeHwnd();
	nmst.hdr.idFrom   = GetDlgCtrlID();
	nmst.hdr.code     = nMessage;
	nmst.text	  = text;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		return pOwner->SendMessage(WM_NOTIFY, nmst.hdr.idFrom, (LPARAM)&nmst);
	
	return 0;
}

void CfxStatic::OnClicked() 
{
	messageToParent(SNM_CLICK, GetText());
}

void CfxStatic::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CStatic::OnShowWindow(bShow, nStatus);
}

void CfxStatic::SetConditionColor(COLORREF upCOL, COLORREF dnCOL, COLORREF eqCOL)
{
	m_upCOL = upCOL;
	m_dnCOL = dnCOL;
	m_eqCOL = eqCOL;
}


void CfxStatic::SetCornerRound(HBITMAP hBitmap[4], int iPoint, int* iLineTick, BOOL bredraw)
{
	m_bCornerRound=TRUE;
	m_iRoundPoint = iPoint;

	for(int i=0; i<4; i++)
		m_hRoundBitmap[i] = hBitmap[i];

	if(iLineTick)
	{
		for(int i=0; i<4; i++)
			m_iRoundTick[i] = iLineTick[i];
	}

	if(bredraw)
		Invalidate(TRUE);

}

#pragma comment (lib, "Msimg32.lib")
void CfxStatic::DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap)
{
	HDC hDC = ::CreateCompatibleDC (nullptr);
	SelectObject (hDC, hBitmap); //icon
//	::BitBlt( pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(),	hDC, 0, 0, SRCCOPY);
	::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);

	::DeleteDC (hDC);
}


void CfxStatic::SetBitmap(HBITMAP hBitmap, int ileft)
{
	m_hBitmap = hBitmap;
	m_iLeftSpace = ileft;

	if(hBitmap)
	{
		BITMAP BM;
		GetObject(hBitmap, sizeof(BITMAP), &BM);
		m_BitmapSize = CSize((int)BM.bmWidth, (int)BM.bmHeight);
	}
}

void CfxStatic::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	Invalidate(TRUE);
}
