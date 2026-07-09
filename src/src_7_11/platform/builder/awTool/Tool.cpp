// tool.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "tool.h"

#include "../childview.h"
#include "../mapform.h"
#include "../mapvar.h"
#include "../mainvar.h"

#include "../../dll/lib/axislib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTOOL

IMPLEMENT_DYNCREATE(CTOOL, CObject)

CTOOL::CTOOL(CChildView* view)
{
	m_name  = _T("");
	m_view  = view;
	m_dc    = (CDC *) NULL;
	m_draw  = m_view->m_draw;
	
	m_pDisplayMode = NULL;

	m_pFormItem = new CFormItem(m_view->m_mapH);
}

CTOOL::~CTOOL()
{
	if (m_dc)	delete m_dc;
	delete m_pFormItem;
}

/////////////////////////////////////////////////////////////////////////////
// CTOOL member function
//
void CTOOL::MouseDown(CPoint point)
{
	m_fN = m_view->m_pFormItem->getFormCount() - 1;
	m_view->SetCapture();

	m_dc    = new CClientDC(m_view);
	m_brush = m_draw->SetBrush(m_dc, clWHITE);
	m_draw->SetPen(m_dc, clBLACK);
	m_draw->SetPen(m_dc, RGB(0, 0, 0), 1, PS_DOT);

	m_saveDC = m_dc->SaveDC();

	setFont(m_dc);
	m_lastpt = m_endpt = point;

	drawBegin(point);
}

void CTOOL::MouseMove(CPoint point)
{
	m_endpt = point;

	m_bound.SetRect(m_lastpt.x, m_lastpt.y, m_endpt.x, m_endpt.y);
	m_bound.NormalizeRect();

	drawTo(point);
}

void CTOOL::MouseUp()
{
	drawEnd();

	::ReleaseCapture();
	if (m_dc)
		m_dc->RestoreDC(m_saveDC);

	if (m_name.CompareNoCase("Pointer"))
		m_view->InvalidateRect(&m_bound);

	if (m_dc)
	{
  		delete m_dc;
		m_dc = (CDC *) NULL;
	}
}

void CTOOL::ReleaseBlock()
{
	CRect	tRc;

	m_view->m_block->count = 0;
	m_view->m_block->bRc.SetRectEmpty();

	tRc.SetRectEmpty();
        for (int ii = 0; ii < m_view->m_pFormItem->getFormCount(); ii++)
        {
		if (m_pFormItem->getFormPoint(ii)->block)
		{
			m_pFormItem->getFormPoint(ii)->block = false;
			tRc.UnionRect(tRc, getFormRect(ii));
		}
        }
	if (!tRc.IsRectEmpty())
	{
		tRc.InflateRect(5, 5);
		m_view->InvalidateRect(&tRc);
	}
}

CRect CTOOL::getRect(int fN)
{
	CRect	tRc;

	if (fN == -1)
		fN = m_fN;

	if (fN >= m_view->m_pFormItem->getFormCount())
		tRc.SetRectEmpty();
	else
		tRc.SetRect(m_pFormItem->getFormPoint(fN)->left, m_pFormItem->getFormPoint(fN)->top, m_pFormItem->getFormPoint(fN)->right, m_pFormItem->getFormPoint(fN)->bottom);

	return tRc;
}

COLORREF CTOOL::getRGB(int color)	// 0x02bbggrr : PALETTERGB
{
	if (color & 0x02000000)
		return (COLORREF) color;

	return m_view->m_palette->GetPaletteRGB(color);
}

void CTOOL::setPen(CDC* dc, COLORREF rgb, int width)
{
	m_draw->SetPen(dc, rgb, width);
}

void CTOOL::setPen(CDC* dc, int width)
{
	if (m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT)
	{
		dc->SelectStockObject(NULL_PEN);
		return;
	}

	m_draw->SetPen(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB), width);
}

CBrush* CTOOL::setBrush(CDC* dc, COLORREF rgb)
{
	return m_draw->SetBrush(dc, rgb);
}

CBrush* CTOOL::setBrush(CDC* dc)
{
	return m_draw->SetBrush(dc, getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
}

void CTOOL::setFont(CDC* dc)
{
	bool	italic = false;
	int	weight = FW_NORMAL;
	int	point;
	CString	fontN;
	TEXTMETRIC metric;

	if (m_fN >= m_pFormItem->getFormCount() || m_fN < 0 || !m_pFormItem->getFormPoint(m_fN)->fStyle)
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
		point = m_pFormItem->getFormPoint(m_fN)->fPoint;
		fontN = m_pFormItem->getFormStr(m_fN, "fName");
		if (m_pFormItem->getFormPoint(m_fN)->fStyle == FS_ITALIC || m_pFormItem->getFormPoint(m_fN)->fStyle == FS_IBOLD)
			italic = true;
		if (m_pFormItem->getFormPoint(m_fN)->fStyle == FS_BOLD || m_pFormItem->getFormPoint(m_fN)->fStyle == FS_IBOLD)
			weight = FW_BOLD;
	}


	m_draw->SetFont(dc, (char *)fontN.operator LPCTSTR(), point ? point : 10, italic, weight);
	dc->GetTextMetrics(&metric);
	m_fheight = (BYTE)abs(metric.tmHeight);
	m_fwidth  = (BYTE)abs(metric.tmAveCharWidth);
}

void CTOOL::setBoldFont(CDC *dc, bool isBold)
{
	bool	italic = false;
	int	weight = FW_NORMAL;
	int	point;
	CString	fontN;
	TEXTMETRIC metric;

	if (m_fN >= m_pFormItem->getFormCount() || m_fN < 0 || !m_pFormItem->getFormPoint(m_fN)->fStyle)
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
		point = m_pFormItem->getFormPoint(m_fN)->fPoint;
		fontN = m_pFormItem->getFormStr(m_fN, "fName");
		if (m_pFormItem->getFormPoint(m_fN)->fStyle == FS_ITALIC || m_pFormItem->getFormPoint(m_fN)->fStyle == FS_IBOLD)
			italic = true;
		if (isBold)
			weight = FW_BOLD;
	}


	m_draw->SetFont(dc, (char *)fontN.operator LPCTSTR(), point ? point : 10, italic, weight);
	dc->GetTextMetrics(&metric);
	m_fheight = (BYTE)abs(metric.tmHeight);
	m_fwidth  = (BYTE)abs(metric.tmAveCharWidth);
}

void CTOOL::setTextColor(CDC* dc, COLORREF rgb)
{
	dc->SetTextColor(rgb);
}

void CTOOL::setTextColor(CDC* dc)
{
	dc->SetTextColor(getRGB(m_pFormItem->getFormPoint(m_fN)->tRGB));
}

void CTOOL::setBkColor(CDC* dc, COLORREF rgb)
{
	if (m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT)
		dc->SetBkMode(TRANSPARENT);
	else
	{
		dc->SetBkMode(OPAQUE);
		dc->SetBkColor(rgb);
	}
}

void CTOOL::setBkColor(CDC* dc)
{
	if (m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT)
		dc->SetBkMode(TRANSPARENT);
	else
	{
		dc->SetBkMode(OPAQUE);
		dc->SetBkColor(getRGB(m_pFormItem->getFormPoint(m_fN)->pRGB));
	}
}

void CTOOL::getPath(CString &path)
{
	int	offs = path.Find('\\');
	if (offs != -1)
		return;

	CString	tmps = m_view->m_root + "\\" + IMAGEDIR + "\\" + path;
	path = tmps;
}

void CTOOL::drawBMP(CDC* dc, CRect rc, CString txt)
{
	CBrush*	brush;

	setTextColor(dc, clBLACK);
	brush = setBrush(dc, clWHITE);
	dc->FillRect(rc, brush);
	dc->SetBkMode(TRANSPARENT);
	dc->DrawText(txt, rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
}

void CTOOL::editAmpersand(CString& str)
{
	int	idx  = 0;
	CString	tmps = _T("");

	for (int ii = 0; ii < str.GetLength(); ii++)
	{
		tmps += str[ii];
		if (str[ii] == '&')
			tmps += '&';
	}

	str = tmps;
}

void CTOOL::editComma(CString& str, int len)
{
	for (int ii = 0;  ii < len; ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			str +=  ',';
		str += '9';
	}

	str.MakeReverse();
}

void CTOOL::setInOutFace(CString& str, int len, int type)
{
	int 	ii;

	switch (type)
	{
	case IO_ALPHA:
		for (ii = 0; ii < len; ii++)
			str += 'A';
		break;

	case IO_HAN:
		len /= 2;
		for (ii = 0; ii < len; ii++)
			str += "วั";
		break;

	case IO_MIXED:
		for (ii = 0; ii < len; ii++)
			str += 'M';
		break;

	case IO_NUM:
	default:
		for (ii = 0; ii < len; ii++)
			str += '9';
		break;
	}
}

void CTOOL::drawEdge(CDC* dc, CRect rect, UINT edge, UINT flags)
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

CRect CTOOL::GroupRect(int nGroup)
{
	CRect rc, tRc;
	rc.SetRectEmpty();
	for (int ii = 0; ii < m_view->m_pFormItem->getFormCount(); ii++)
		if (m_pFormItem->getFormPoint(ii)->group == (struct _formR*)nGroup)
		{
			tRc.SetRect(m_pFormItem->getFormPoint(ii)->left, m_pFormItem->getFormPoint(ii)->top, m_pFormItem->getFormPoint(ii)->right, m_pFormItem->getFormPoint(ii)->bottom);
			rc.UnionRect(rc, tRc);
		}
	return rc;
}

// flag = -1 : group check, 0 : only form rect, 1 : tab intersect rect
CRect CTOOL::getFormRect(int idx, int flag)
{
	if (idx == -1)
		return CRect(0);
	CRect tRc;
	if (flag == -1 && m_pFormItem->getFormPoint(idx)->group)
		return GroupRect((int)m_pFormItem->getFormPoint(idx)->group);

	tRc.SetRect(m_pFormItem->getFormPoint(idx)->left, m_pFormItem->getFormPoint(idx)->top, m_pFormItem->getFormPoint(idx)->right, m_pFormItem->getFormPoint(idx)->bottom);
	if (flag == 1 && m_pFormItem->getFormPoint(idx)->tabN)
	{
		struct _formR *tabN = m_pFormItem->getFormPoint(idx)->tabN;

		if (IsInPage(idx) && !tabN->group)
		{
			if (tabN->tabN)
				tRc.IntersectRect(tRc, getFormRect(m_pFormItem->getIdxOfPtr(tabN), 1));
			else
				tRc.IntersectRect(tRc, getFormRect(m_pFormItem->getIdxOfPtr(tabN), 0));
		}
		else
			tRc.SetRectEmpty();
	}
	return tRc;
}

bool CTOOL::IsInPage(int idx)
{
	struct _formR *formR = m_pFormItem->getFormPoint(idx);
	if (formR->tabN->checked != formR->pageN)
		return false;
	if (formR->tabN->tabN)
		return IsInPage(m_pFormItem->getIdxOfPtr(formR->tabN));
	return true;
}

CString CTOOL::convertVString(CString str)
{
	CString rtStr = "";
	for (int ii = 0; ii < str.GetLength(); ii++)
	{
		if (str[ii] & 0x80)
		{
			rtStr += str.Mid(ii, 2);
			ii++;
		}
		else
			rtStr += str.Mid(ii, 1);
		rtStr += '\n';
	}
	return rtStr;
}

CString CTOOL::Parser(CString &srcstr, CString substr)
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

int CTOOL::getDefaultWidth(CString sKind, int nDefault)
{
	if (m_sTabDir.IsEmpty())
		m_sTabDir.Format("%s\\tab\\", (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR));
	return GetPrivateProfileInt(sKind, "WIDTH", nDefault, m_sTabDir + "awDefault.ini");
}

int CTOOL::getDefaultHeight(CString sKind, int nDefault)
{
	if (m_sTabDir.IsEmpty())
		m_sTabDir.Format("%s\\tab\\", (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR));
	return GetPrivateProfileInt(sKind, "HEIGHT", nDefault, m_sTabDir + "awDefault.ini");
}

COLORREF CTOOL::getDefaultPRGB(CString sKind, int nDefault)
{
	if (m_sTabDir.IsEmpty())
		m_sTabDir.Format("%s\\tab\\", (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR));
	return GetPrivateProfileInt(sKind, "PRGB", nDefault, m_sTabDir + "awDefault.ini");
}

COLORREF CTOOL::getDefaultTRGB(CString sKind, int nDefault)
{
	if (m_sTabDir.IsEmpty())
		m_sTabDir.Format("%s\\tab\\", (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR));
	return GetPrivateProfileInt(sKind, "TRGB", nDefault, m_sTabDir + "awDefault.ini");
}

COLORREF CTOOL::getDefaultBRGB(CString sKind, int nDefault)
{
	if (m_sTabDir.IsEmpty())
		m_sTabDir.Format("%s\\tab\\", (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR));
	return GetPrivateProfileInt(sKind, "BRGB", nDefault, m_sTabDir + "awDefault.ini");
}


/////////////////////////////////////////////////////////////////////////////
// CtlRect

CtlRect::CtlRect(CChildView* view)
	: CTOOL(view)
{
}

CtlRect::~CtlRect()
{
}

void CtlRect::drawBegin(CPoint point)
{
	m_rect.left = m_rect.right = point.x;
	m_rect.top  = m_rect.bottom = point.y;

	m_dc->SelectStockObject(BLACK_PEN);
	m_dc->SelectStockObject(NULL_BRUSH);
	m_dc->SetROP2(R2_NOT);
	m_dc->Rectangle(m_rect);
}

void CtlRect::drawTo(CPoint point)
{
	m_dc->Rectangle(m_rect);
	m_rect.right = point.x;
	m_rect.bottom = point.y;
	m_dc->Rectangle(m_rect);
}

void CtlRect::drawEnd()
{
	m_rect.NormalizeRect();

	m_dc->Rectangle(m_bound);
	m_dc->SetROP2(R2_COPYPEN);
	m_draw->SetPen(m_dc, clBLACK);
	m_dc->SelectStockObject(NULL_BRUSH);
}





