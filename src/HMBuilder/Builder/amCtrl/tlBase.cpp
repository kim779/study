// tlBase.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlBase.h"

#include "../ChildView.h"
#include "../h/mapform.h"
#include "../h/mapvar.h"
#include "../h/mainvar.h"
#include "../amCom/formItem.h"
#include "../amLib/amlib.h"

#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

// CTlBase

CTlBase::CTlBase(CChildView* view, CAmDraw* draw, CFormItem* formItem)
{
	m_view = view;
	m_name = _T("");
	m_dc = (CDC *)NULL;
	m_draw = draw;

	m_display = m_view->m_display;

	m_formItem = formItem;
}

CTlBase::~CTlBase()
{
}

// CTlBase 멤버 함수

void CTlBase::MouseDown(CPoint point)
{
	CBrush*	brush;

	m_fN = m_formItem->GetCount() - 1;
	m_view->SetCapture();

	m_dc     = new CClientDC(m_view);
	m_saveDC = m_dc->SaveDC();
	brush = m_draw->SetBrush(m_dc, clWHITE);
	m_draw->SetPen(m_dc, clBLACK);
	m_draw->SetPen(m_dc, RGB(0, 0, 0), 1, PS_DOT);

	setFont(m_dc);
	m_lastpt = m_endpt = point;

	drawBegin(point);
}

void CTlBase::MouseMove(CPoint point)
{
	m_endpt = point;

	m_bound.SetRect(m_lastpt.x, m_lastpt.y, m_endpt.x, m_endpt.y);
	m_bound.NormalizeRect();

	drawTo(point);
}

void CTlBase::MouseUp()
{
	drawEnd();
	
	// Layout isLayout?

	// Layout Default
	//DefaultLayout();

	::ReleaseCapture();
	if (m_dc)
		m_dc->RestoreDC(m_saveDC);

	if (m_name.CompareNoCase(_T("Pointer")))
		m_view->InvalidateRect(&m_bound);

	if (m_dc)
	{
  		delete m_dc;
		m_dc = (CDC *) NULL;
	}
}
// layout
void CTlBase::DefaultLayout()
{
	CMapStringToString* map =  m_view->m_mapH->maplayout;

	if (!map || map->GetCount() < 1)
		return;
	
	if (m_view->m_selF == -1)
		return;

	struct	_formR*	formR = m_formItem->GetForm(m_view->m_selF);
	CString sIndex, sData;	
	for (POSITION pos = map->GetStartPosition(); pos;)
	{
		struct _Layout* layout = new _Layout;
		ZeroMemory(layout, sizeof(_Layout));	

		map->GetNextAssoc(pos, sIndex, sData);

		if (formR->layout->GetCount() > 0)
		{
			BOOL valid = formR->layout->Lookup(sData, (void*&)layout);
			if (valid && !layout->bfirst)
				continue;
		}
		layout->left = formR->left;
		layout->right = formR->right;
		layout->top = formR->top;
		layout->bottom = formR->bottom;
		layout->bfirst = true;
		layout->properties = PR_VISIBLE;
		
		formR->layout->SetAt(sData, layout);
	}
}

void CTlBase::ReleaseBlock()
{
	CRect	tRc;

	m_view->m_block->count = 0;
	m_view->m_block->bRc.SetRectEmpty();

	tRc.SetRectEmpty();
        for (int ii = 0; ii < m_formItem->GetCount(); ii++)
        {
		if (m_formItem->GetForm(ii)->block)
		{
			m_formItem->GetForm(ii)->block = false;
			tRc.UnionRect(tRc, getFormRect(ii));
		}
        }
	if (!tRc.IsRectEmpty())
	{
		tRc.InflateRect(5, 5);
		m_view->InvalidateRect(&tRc);
	}
}

CRect CTlBase::getRect(int fN)
{
	CRect	tRc;

	if (fN == -1)
		fN = m_fN;

	if (fN >= m_formItem->GetCount())
		tRc.SetRectEmpty();
	else
		tRc.SetRect(m_formItem->GetForm(fN)->left, m_formItem->GetForm(fN)->top, m_formItem->GetForm(fN)->right, m_formItem->GetForm(fN)->bottom);

	return tRc;
}

COLORREF CTlBase::getRGB(int color)
{
	if (color & 0x02000000)
		return (COLORREF) color;

	return (COLORREF)m_view->SendMessage(WM_USER, MAKEWPARAM(getCOLOR, 0), (LPARAM)color);
}

void CTlBase::setPen(CDC* dc, COLORREF rgb, int width)
{
	m_draw->SetPen(dc, rgb, width);
}

void CTlBase::setPen(CDC* dc, int width)
{
	m_draw->SetPen(dc, getRGB(m_formItem->GetForm(m_fN)->pRGB), width);
}

CBrush* CTlBase::setBrush(CDC* dc, COLORREF rgb)
{
	return m_draw->SetBrush(dc, rgb);
}

CBrush* CTlBase::setBrush(CDC* dc)
{
	return m_draw->SetBrush(dc, getRGB(m_formItem->GetForm(m_fN)->pRGB));
}

void CTlBase::setFont(CDC* dc)
{
	bool	italic = false;
	int	weight = FW_NORMAL;
	int	point;
	CString	fontN;
	TEXTMETRIC metric;

	if (m_fN >= m_formItem->GetCount() || m_fN < 0 || !m_formItem->GetForm(m_fN)->fStyle)
	{
		point = m_view->m_mapH->fPoint;
		fontN = CString(m_view->m_mapH->fName);
		if (m_view->m_mapH->fStyle == FS_ITALIC || m_view->m_mapH->fStyle == FS_IBOLD)
			italic = true;
		if (m_view->m_mapH->fStyle == FS_BOLD || m_view->m_mapH->fStyle == FS_IBOLD)
			weight = FW_BOLD;
	}
	else
	{
		point = m_formItem->GetForm(m_fN)->fPoint;
		fontN = m_formItem->GetFormStr(m_fN, _T("fName"));
		if (m_formItem->GetForm(m_fN)->fStyle == FS_ITALIC || m_formItem->GetForm(m_fN)->fStyle == FS_IBOLD)
			italic = true;
		if (m_formItem->GetForm(m_fN)->fStyle == FS_BOLD || m_formItem->GetForm(m_fN)->fStyle == FS_IBOLD)
			weight = FW_BOLD;
	}
	
	m_draw->SetFont(dc, (char *)fontN.operator LPCTSTR(), point ? point : 10, italic, weight);
	dc->GetTextMetrics(&metric);
	m_fheight = (BYTE)abs(metric.tmHeight);
	m_fwidth  = (BYTE)abs(metric.tmAveCharWidth);
}

void CTlBase::setCellFont(CDC* dc, struct _cellR* cellR)
{
	bool	italic = false;
	int	weight = FW_NORMAL;
	int	point;
	CString	fontN;
	TEXTMETRIC metric;
	
	point = cellR->fPoint;
	fontN = CString(cellR->fName);
	if (cellR->fStyle == FS_ITALIC || cellR->fStyle == FS_IBOLD)
		italic = true;
	if (cellR->fStyle == FS_BOLD || cellR->fStyle == FS_IBOLD)
		weight = FW_BOLD;

	m_draw->SetFont(dc, (char *)fontN.operator LPCTSTR(), point ? point : 10, italic, weight);
	dc->GetTextMetrics(&metric);
	m_fheight = (BYTE)abs(metric.tmHeight);
	m_fwidth  = (BYTE)abs(metric.tmAveCharWidth);	
}


void CTlBase::setTextColor(CDC* dc, COLORREF rgb)
{
	dc->SetTextColor(rgb);
}

void CTlBase::setTextColor(CDC* dc)
{
	dc->SetTextColor(getRGB(m_formItem->GetForm(m_fN)->tRGB));
}

void CTlBase::setBkColor(CDC* dc, COLORREF rgb)
{
	dc->SetBkMode(OPAQUE);
	dc->SetBkColor(rgb);
}

void CTlBase::setBkColor(CDC* dc)
{
	dc->SetBkMode(OPAQUE);
	dc->SetBkColor(getRGB(m_formItem->GetForm(m_fN)->pRGB));
}

void CTlBase::getPath(CString &path)
{
	int	offs = path.Find(_T('\\'));
	if (offs != -1)
		return;

	CString	tmps = path;
	
	path.Format(_T("%s\\%s\\%s"), getRoot(), IMAGEDIR, tmps);
}

CString CTlBase::getRoot()
{
	//return (char *)m_view->SendMessage(WM_USER, MAKEWPARAM(getROOT, 0));
	return (LPCTSTR)(char *)m_view->SendMessage(WM_USER, MAKEWPARAM(getROOT, 0));
}

void CTlBase::drawBMP(CDC* dc, CRect rc, CString txt)
{
	CBrush*	brush;

	setTextColor(dc, clBLACK);
	brush = setBrush(dc, clWHITE);
	dc->FillRect(rc, brush);
	dc->SetBkMode(TRANSPARENT);
	dc->DrawText(txt, rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
}

void CTlBase::drawPNG(CDC* dc, CRect rc, CString txt)
{
	getPath(txt);
	Graphics g(dc->GetSafeHdc());
	Image	img(txt.AllocSysString());
	g.DrawImage(&img, rc.left, rc.top, rc.Width(), rc.Height());
}

void CTlBase::editAmpersand(CString& str)
{
	int	idx  = 0;
	CString	tmps = _T("");

	for (int ii = 0; ii < str.GetLength(); ii++)
	{
		tmps += str[ii];
		if (str[ii] == _T('&'))
			tmps += _T('&');
	}

	str = tmps;
}

void CTlBase::editComma(CString& str, int len)
{
	for (int ii = 0;  ii < len; ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			str +=  _T(',');
		str += _T('9');
	}

	str.MakeReverse();
}

void CTlBase::setInOutFace(CString& str, int len, int type)
{
	int 	ii;

	switch (type)
	{
	case IO_ALPHA:
		for (ii = 0; ii < len; ii++)
			str += _T('A');
		break;

	case IO_HAN:
		len /= 2;
		for (ii = 0; ii < len; ii++)
			str += _T("한");
		break;

	case IO_MIXED:
		for (ii = 0; ii < len; ii++)
			str += _T('M');
		break;

	case IO_NUM:
	default:
		for (ii = 0; ii < len; ii++)
			str += _T('9');
		break;
	}
}

void CTlBase::drawEdge(CDC* dc, CRect rect, UINT edge, UINT flags)
{
	CRect	tRc;
	CPoint	pts[6];
	CBrush*	brush;
	int	gap = 1;

	switch (edge)
	{
	case EDGE_BUMP:		// raised_outer | sunken_inner
		tRc = rect;
		tRc.DeflateRect(1, 1, 0, 0);
		setPen(dc, clEDGE);
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(&tRc);

		tRc = rect;
		tRc.DeflateRect(0, 0, 1, 1);
		setPen(dc, clYWHITE);
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(&tRc);
		break;

	case EDGE_ETCHED:	// sunken_outer | raised_inner
		dc->DrawEdge(&rect, edge, BF_RECT);
		break;

	case EDGE_RAISED:	// raised_outer | raised_inner
		pts[0].x = rect.right-gap;	pts[0].y = rect.top;
		pts[1].x = rect.left;		pts[1].y = rect.top;
		pts[2].x = rect.left;		pts[2].y = rect.bottom-gap;
		pts[3].x = rect.left;		pts[3].y = pts[2].y;
		pts[4].x = pts[3].x;		pts[4].y = rect.top;
		pts[5].x = pts[0].x;		pts[5].y = pts[4].y;
		setPen(dc, clYWHITE);
		brush = setBrush(dc, clYWHITE);
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);

		pts[0].x = rect.right-gap;	pts[0].y = rect.top;
		pts[1].x = pts[0].x;		pts[1].y = rect.bottom-gap;
		pts[2].x = rect.left;		pts[2].y = pts[1].y;
		pts[3].x = rect.left;		pts[3].y = rect.bottom-gap;
		pts[4].x = rect.right-gap;	pts[4].y = rect.bottom-gap;
		pts[5].x = rect.right-gap;	pts[5].y = rect.top;
		setPen(dc, clEDGE);
		brush = setBrush(dc, clEDGE);
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);
		break;

	case EDGE_SUNKEN:	// sunken_outer | sunken_inner
		pts[0].x = rect.right-gap;	pts[0].y = rect.top;
		pts[1].x = rect.left;		pts[1].y = rect.top;
		pts[2].x = rect.left;		pts[2].y = rect.bottom-gap;
		pts[3].x = rect.left;		pts[3].y = pts[2].y;
		pts[4].x = pts[3].x;		pts[4].y = rect.top;
		pts[5].x = pts[0].x;		pts[5].y = pts[4].y;
		setPen(dc, clEDGE);
		brush = setBrush(dc, clEDGE);
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);

		pts[0].x = rect.right-gap;	pts[0].y = rect.top;
		pts[1].x = pts[0].x;		pts[1].y = rect.bottom-gap;
		pts[2].x = rect.left;		pts[2].y = pts[1].y;
		pts[3].x = rect.left;		pts[3].y = rect.bottom-gap;
		pts[4].x = rect.right-gap;	pts[4].y = rect.bottom-gap;
		pts[5].x = rect.right-gap;	pts[5].y = rect.top;
		setPen(dc, clYWHITE);
		brush = setBrush(dc, clYWHITE);
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);
		break;
	}
}

CRect CTlBase::GroupRect(int nGroup)
{
	CRect rc, tRc;

	rc.SetRectEmpty();
	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		if (m_formItem->GetForm(ii)->group == (struct _formR*)nGroup)
		{
			tRc.SetRect(m_formItem->GetForm(ii)->left, m_formItem->GetForm(ii)->top, m_formItem->GetForm(ii)->right, m_formItem->GetForm(ii)->bottom);
			rc.UnionRect(rc, tRc);
		}
	}
	return rc;
}

// flag = -1 : group check, 0 : only form rect, 1 : tab intersect rect
CRect CTlBase::getFormRect(int idx, int flag)
{
	if (idx == -1)
		return CRect(0, 0, 0, 0);

	CRect tRc;
	if (flag == -1 && m_formItem->GetForm(idx)->group)
		return GroupRect((int)m_formItem->GetForm(idx)->group);

	tRc.SetRect(m_formItem->GetForm(idx)->left, m_formItem->GetForm(idx)->top, m_formItem->GetForm(idx)->right, m_formItem->GetForm(idx)->bottom);
	return tRc;
}

COLORREF CTlBase::getDefaultBRGB(CString sKind, int nDefault)
{
	CString	path;

	path.Format(_T("%s\\%s\\awDefault.ini"), getRoot(), TABDIR);
	return GetPrivateProfileInt(sKind, _T("BRGB"), nDefault, path);
}

COLORREF CTlBase::getDefaultTRGB(CString sKind, int nDefault)
{
	CString	path;

	path.Format(_T("%s\\%s\\awDefault.ini"), getRoot(), TABDIR);
	return GetPrivateProfileInt(sKind, _T("TRGB"), nDefault, path);
}

COLORREF CTlBase::getDefaultPRGB(CString sKind, int nDefault)
{
	CString	path;

	path.Format(_T("%s\\%s\\awDefault.ini"), getRoot(), TABDIR);
	return GetPrivateProfileInt(sKind, _T("PRGB"), nDefault, path);
}

int CTlBase::getDefaultHeight(CString sKind, int nDefault)
{
	CString	path;

	path.Format(_T("%s\\%s\\awDefault.ini"), getRoot(), TABDIR);
	return GetPrivateProfileInt(sKind, _T("HEIGHT"), nDefault, path);
}

int CTlBase::getDefaultWidth(CString sKind, int nDefault)
{
	CString	path;

	path.Format(_T("%s\\%s\\awDefault.ini"), getRoot(), TABDIR);
	return GetPrivateProfileInt(sKind, _T("WIDTH"), nDefault, path);
}

void CTlBase::setBoldFont(CDC* dc, bool isBold)
{
	bool	italic = false;
	int	weight = FW_NORMAL;
	int	point;
	CString	fontN;
	TEXTMETRIC metric;

	if (m_fN >= m_formItem->GetCount() || m_fN < 0 || !m_formItem->GetForm(m_fN)->fStyle)
	{
		point = m_view->m_mapH->fPoint;
		fontN = CString(m_view->m_mapH->fName);
		if (m_view->m_mapH->fStyle == FS_ITALIC || m_view->m_mapH->fStyle == FS_IBOLD)
			italic = true;
		if (isBold)
			weight = FW_BOLD;
	}
	else
	{
		point = m_formItem->GetForm(m_fN)->fPoint;
		fontN = m_formItem->GetFormStr(m_fN, _T("fName"));
		if (m_formItem->GetForm(m_fN)->fStyle == FS_ITALIC || m_formItem->GetForm(m_fN)->fStyle == FS_IBOLD)
			italic = true;
		if (isBold)
			weight = FW_BOLD;
	}


	m_draw->SetFont(dc, (char *)fontN.operator LPCTSTR(), point ? point : 10, italic, weight);
	dc->GetTextMetrics(&metric);
	m_fheight = (BYTE)abs(metric.tmHeight);
	m_fwidth  = (BYTE)abs(metric.tmAveCharWidth);
}

CString CTlBase::parse(CString &src, CString subs)
{
	CString	tmps;
	int	idx = src.Find(subs);

	if (idx == -1)
	{
		tmps = src;
		src = _T("");
	}
	else
	{
		tmps = src.Left(idx);
		src  = src.Mid(idx + subs.GetLength());
	}
	return tmps;
}

CString CTlBase::convertVString(CString str)
{
	CString rtStr = _T("");

	for (int ii = 0; ii < str.GetLength(); ii++)
	{
		if (str[ii] & 0x80)
		{
			rtStr += str.Mid(ii, 2);
			ii++;
		}
		else
			rtStr += str.Mid(ii, 1);
		rtStr += _T('\n');
	}
	return rtStr;
}