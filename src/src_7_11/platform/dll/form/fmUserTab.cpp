// fmUserTab.cpp : implementation file
//

#include "stdafx.h"
#include "fmUserTab.h"
#include "axform.h"
#include "image.h"

#include "../lib/axislib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	wSCR		15
#define	hSCR		22
#define	nIMG		6

#define	nIMAGE		9
#define	iSL		0	// select left
#define	iSB		1	// select body
#define	iSR		2	// select right
#define	iNL		3	// normal left
#define	iNB		4	// normal body
#define	iNR		5	// normal right
#define	iBL		6	// bg left
#define	iBB		7	// bg body
#define	iBR		8	// bg rihgt

#define	SCR_BMP		"axtab_scr.bmp"
#define	ED_BMP		"axtab_ed.bmp"

/////////////////////////////////////////////////////////////////////////////
// CfmUserTab

IMPLEMENT_DYNCREATE(CfmUserTab, CfmBase)

CfmUserTab::CfmUserTab()
{
	EnableAutomation();
}

CfmUserTab::CfmUserTab(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	m_hasfocus = false;
	m_count = m_form->keys;
	m_index = m_form->checked;

	m_start  = 0;
	m_last   = m_count;
	m_scroll = false;
	m_click  = ckNone;
	m_scr.SetRectEmpty();
	m_cell.SetRectEmpty();
	m_more.SetRectEmpty();
	m_moreBM = nullptr;
	m_scrBM  = nullptr;

	m_caption.RemoveAll();
	m_map.RemoveAll();
	if (m_count <= 0)
		return;

	char*	map	= &m_axform->m_strR[m_form->vals[0]];
	char*	caption	= &m_axform->m_strR[m_form->vals[1]];
	for (int ii = 0; ii < m_count; ii++)
	{
		m_caption.Add(caption);
		caption += strlen(caption);
		caption++;

		m_map.Add(map);
		map += strlen(map);
		map++;
	}
	m_count = min(m_count, min(m_map.GetSize(), m_caption.GetSize()));

	if (m_form->vals[3] != NOVALUE)
	{
		m_fpRGB = m_axform->m_valueR[m_form->vals[3]];
		m_ftRGB = m_axform->m_valueR[m_form->vals[3]+1];
	}
	else
	{
		m_fpRGB = (m_form->type == TAB_NONE) ? cDBLUE : m_form->pRGB;
		m_ftRGB = (m_form->type == TAB_NONE) ? cWHITE : m_form->tRGB;
	}
	m_fpRGB = (m_fpRGB & userCOLOR) ? m_fpRGB : getRGB(m_fpRGB);
	m_ftRGB = (m_ftRGB & userCOLOR) ? m_ftRGB : getRGB(m_ftRGB);

	m_type = ET_ITEM;
	m_name = _T("");
	if (m_form->properties & PR_CHECKED)
		m_hasfocus = true;
       	if (m_form->attr & FA_ENUM)
	{
		m_name = CString((char *)m_form->name);
		m_type = ET_USER;
		m_form->attr |= FA_DATE;
	}
	m_bForm = false;
	if (m_form->checked >= m_count)
		m_bForm = true;

	m_vertical = false;
	if (m_form->align == AL_LEFT || m_form->align == AL_RIGHT)
		m_vertical = true;

	if (m_axform->m_drawOnly && m_form->type == TAB_IMAGE)
		m_form->type = TAB_BORDER;

	m_leftW = m_rightW = 0;
	m_images.RemoveAll();
	m_orders.RemoveAll();
	SetEnum(_T(""), 0, 100);
	loadABitmap();
	if (m_form->attr & FA_UPPER)
		loadOrderBitmap();

	int	width = 0;
	CDC*	dc = getDC();
	setFont(dc);
	m_scroll = isScrolled(dc, m_last, width, 0);
	getStartIndex(m_index);
	releaseDC(dc);

	m_move  = mvNone;
	m_imove = -1;
}

CfmUserTab::~CfmUserTab()
{
	m_map.RemoveAll();
	m_caption.RemoveAll();
	m_images.RemoveAll();
	if (m_moreBM)
		delete m_moreBM;
	if (m_scrBM)
		delete m_scrBM;
}


void CfmUserTab::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmUserTab, CfmBase)
	//{{AFX_MSG_MAP(CfmUserTab)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmUserTab, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmUserTab)
	DISP_PROPERTY_EX(CfmUserTab, "ID", _getID, _setID, VT_I4)
	DISP_PROPERTY_EX(CfmUserTab, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CfmUserTab, "Index", _getIndex, _setIndex, VT_I4)
	DISP_PROPERTY_EX(CfmUserTab, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmUserTab, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_FUNCTION(CfmUserTab, "Reload", _Reload, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmUserTab, "GetText", _GetText, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmUserTab, "SetText", _SetText, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmUserTab, "IsTabVisible", _IsTabVisible, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CfmUserTab, "SetTabVisible", _SetTabVisible, VT_EMPTY, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CfmUserTab, "Change", _Change, VT_EMPTY, VTS_I4 VTS_BOOL VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmUserTab to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {15AA3B05-68BC-4B05-9662-26A8E1EEB1C8}
static const IID IID_IfmUserTab =
{ 0x15aa3b05, 0x68bc, 0x4b05, { 0x96, 0x62, 0x26, 0xa8, 0xe1, 0xee, 0xb1, 0xc8 } };

BEGIN_INTERFACE_MAP(CfmUserTab, CfmBase)
	INTERFACE_PART(CfmUserTab, IID_IfmUserTab, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmUserTab message handlers

void CfmUserTab::_Reload() 
{
	SetEnum(_T(""), -1);
}

BSTR CfmUserTab::_GetText(long ID) 
{
	CString strResult = _T("");
	int	index = getIndexByID(ID);

	if (index >= 0 && index < m_caption.GetSize())
		strResult = m_caption.GetAt(index);

	return strResult.AllocSysString();
}

void CfmUserTab::_SetText(long ID, LPCTSTR text) 
{
	int index = getIndexByID(ID);

	if (index >=0 && index < m_caption.GetSize())
	{
		m_caption.SetAt(index, text);
		invalidateRect(&m_pRc, true);
	}
}

BOOL CfmUserTab::_IsTabVisible(long ID) 
{
	return IsTabVisible(ID);
}

void CfmUserTab::_SetTabVisible(long ID, BOOL visible) 
{
	SetTabVisible(ID, visible);
}

void CfmUserTab::_Change(long ID, BOOL visible, LPCTSTR text) 
{
	CString	str;
	int	index = getIndexByID(ID);

	if (index >= 0 && index < m_caption.GetSize())
	{
		str = m_map.GetAt(index);
		str.SetAt(0, visible ? '1' : '0');
		m_map.SetAt(index, str);
		m_caption.SetAt(index, text);
		invalidateRect(&m_pRc, true);
	}
}

long CfmUserTab::_getID() 
{
	return GetCurSel();
}

void CfmUserTab::_setID(long nNewValue) 
{
	SetCurSel(nNewValue);
}

BSTR CfmUserTab::_getText() 
{
	CString strResult = _T("");

	if (m_index >= 0 && m_index < m_map.GetSize() && m_index < m_caption.GetSize())
		strResult = m_caption.GetAt(m_index);
	return strResult.AllocSysString();
}

void CfmUserTab::_setText(LPCTSTR lpszNewValue) 
{
	if (m_index >= 0 && m_index < m_map.GetSize() && m_index < m_caption.GetSize())
		m_caption.SetAt(m_index, lpszNewValue);
}

long CfmUserTab::_getIndex() 
{
	return GetCurSel();
}

void CfmUserTab::_setIndex(long nNewValue) 
{
	SetCurSel(nNewValue);
}

BOOL CfmUserTab::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmUserTab::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

BOOL CfmUserTab::_getEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CfmUserTab::_setEnable(BOOL bNewValue) 
{
	if (bNewValue)
		m_form->properties |= PR_ENABLE;
	else
		m_form->properties &= ~PR_ENABLE;
}

void CfmUserTab::Resize(float hR, float vR, bool font, bool redraw)
{
	m_scr.SetRectEmpty();
	m_cell.SetRectEmpty();
	m_more.SetRectEmpty();

	CfmBase::Resize(hR, vR, font, redraw);
	
	int	last = 0, width = 0;
	CDC*	dc = getDC();
	setFont(dc);
	m_scroll = isScrolled(dc, last, width, 0);
	releaseDC(dc);
}

void CfmUserTab::Reload()
{
	CfmBase::Reload();

	m_scr.SetRectEmpty();
	m_cell.SetRectEmpty();
	m_more.SetRectEmpty();
}

int CfmUserTab::IsPointInRect(CPoint pt)
{
	if (!m_axform->IsFormVisible(this))
		return P_OUT;

	if ((m_form->properties & PR_VISIBLE) && (m_form->properties & PR_ENABLE) && m_pRc.PtInRect(pt))
		return P_IN;

	return P_OUT;
}

void CfmUserTab::SetFont(int point, int style, CString fonts, bool resize)
{
	if (m_form->fPoint == m_axform->m_mapH->fPoint)
	{
		bool	change = false;

		if (!fonts.IsEmpty() && m_fonts.CompareNoCase(fonts))
		{
			m_fonts = fonts;
			change  = true;
		}

		if (style >= 0 && m_form->fStyle != style)
		{
			m_form->fStyle = style;
			change = true;
		}

		if (m_fPoint != point)
		{
			m_fPoint = m_cfPoint = point;
			change = true;
		}

		if (change)
		{
			TEXTMETRIC metric;
			CDC*	dc = getDC();
			setFont(dc);
			dc->GetTextMetrics(&metric);
			m_fHeight = (BYTE)abs(metric.tmHeight);
			m_fWidth  = (BYTE)abs(metric.tmAveCharWidth);
			releaseDC(dc);

			CRect	tRc = m_pRc;
			tRc.InflateRect(2, 2);
			invalidateRect(tRc, true);
		}
	}
}

void CfmUserTab::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE) || m_count <= 0)
		return;

	UINT	style = 0;
	CRect	tRc, tRc1, pRc; // , rc;
	CString	tmpS, tmpS_, cells;
	int	tabcnt, dx, cx = 2;
	CPoint	pts[12];
	CBrush*	brush;
	bool	focus_this, scroll;
	int	ii, jj, ndc = dc->SaveDC();

	tabcnt = 0;
	for (ii = 0; ii < m_count; ii++)
	{
		tmpS = m_map.GetAt(ii);
		if (tmpS[0] == '1')
			tabcnt++;
	}
	if (tabcnt == 0)
		return;

	if (m_vertical)
		dx = m_pRc.Height() / tabcnt;
	else
		dx = m_pRc.Width() / tabcnt;
	if (dx < (cx+1)*2)
		return;

	setTextColor(dc);
	setPaintColor(dc);

	pRc = m_pRc;
	scroll = isScrolled(dc, m_last, dx, m_start);
	if (m_vertical)
	{
		if (m_scroll)
			pRc.bottom -= (pRc.Width()/2);
		else
			pRc.bottom = pRc.top + dx;
		dx = pRc.top;
	}
	else
	{
		if (m_scroll)
		{
			if (m_form->type != TAB_IMAGE)
				pRc.right -= (int)(float)(m_pRc.Height() * 2.0 / 3.0) * 2;
		}
		else
			pRc.right = pRc.left + dx;
		dx = pRc.left;
	}

	switch (m_form->type)
	{
	case TAB_IMAGE:
		drawTab(dc, CRect(pRc.left, pRc.top, m_pRc.right, pRc.bottom), scroll);
	//	pRc.bottom--;
		if (m_scroll)
		{
			tRc = pRc;
			tRc.left = tRc.right - wSCR*2;
			pRc.right = tRc.left;
			drawScroll(dc, tRc);
		}
		tRc = pRc;
		if (m_scroll && !m_scr.IsRectEmpty())
			tRc.right = m_scr.left;
		else
			tRc.right = m_pRc.right;
		if (m_form->attr & FA_DATE)
		{
			tRc.left = tRc.right - m_moreW;
			drawMore(dc, tRc);
		}
		return;

	case TAB_BORDER:
	case TAB_OBLIQUE:
		dx++;
	default:
		break;
	}

	m_cell.SetRectEmpty();
	for (ii = m_start; ii < m_last; ii++)
	{
		tmpS = m_map.GetAt(ii);
		if (tmpS[0] != '1')
			continue;

		tmpS = m_caption.GetAt(ii);
		tRc  = pRc;

		if (m_vertical)
			tRc.top = dx;
		else
			tRc.left = dx;

		focus_this = false;
		if (ii == m_index && m_hasfocus && !m_bForm)
			focus_this = true;

		setFont(dc, focus_this ? 1 : -1);
		if (!scroll || ii < m_last-1)
		{
			dx += getSize(dc, ii);
			if (m_vertical)
				tRc.bottom = dx;
			else
				tRc.right = dx;
		}

		if (!focus_this)
		{
			switch (m_form->type)
			{
			case TAB_BORDER:
				jj = 2;	break;
			case TAB_OBLIQUE:
				jj = 3;	break;
			default:
				jj = 0;	break;
			}

			switch (m_form->align)
			{
			case AL_LEFT:
				tRc.left += jj;			break;
			case AL_RIGHT:
				tRc.right -= jj;		break;
			case AL_BOTTOM:
				tRc.bottom -= jj;		break;
			case AL_TOP:
			default:
				tRc.top += jj;			break;
			}
		}

		switch (m_form->type)
		{
		case TAB_NONE:
			if (!scroll || ii < m_last-1)
				tRc.right--;
			tRc.bottom--;
			drawNoneTab(dc, tRc, tRc1, cx, focus_this, style);
			if (!scroll || ii < m_last-1)
				style |= DT_CENTER;
			setTextColor(dc, focus_this ? m_ftRGB : m_tRGB);
			setPaintColor(dc, focus_this ? m_fpRGB : m_pRGB);
			break;

		case TAB_OBLIQUE:
			tRc1 = tRc;
			tRc.bottom--;
			dc->SetPolyFillMode(ALTERNATE);
			pts[0].x = tRc.left;	pts[0].y = tRc.bottom;
			pts[1].x = tRc.left;	pts[1].y = tRc.top+3;
			pts[2].x = tRc.left+3;	pts[2].y = tRc.top;
			pts[3].x = tRc.right-4;	pts[3].y = tRc.top;
			pts[4].x = tRc.right-1;	pts[4].y = tRc.top+3;
			pts[5].x = tRc.right-1;	pts[5].y = tRc.bottom;
			pts[6].x = pts[5].x-1;	pts[6].y = tRc.bottom;
			pts[7].x = pts[6].x;	pts[7].y = tRc.top+3;
			pts[8].x = pts[3].x;	pts[8].y = pts[3].y+1;
			pts[9].x = pts[2].x;	pts[9].y = pts[8].y;
			pts[10].x = pts[1].x+1;	pts[10].y = pts[1].y;
			pts[11].x = pts[0].x+1;	pts[11].y = pts[0].y;
			setPen(dc, getRGB(cBLACK));
			setBrush(dc, getRGB(cBLACK));
			dc->Polygon(pts, 12);

			if (scroll && ii == m_last-1)
			{
				style |= DT_LEFT;
				tmpS = _T(" ") + tmpS;
			}
			else
				style |= DT_CENTER;

			if (!focus_this)
			{
				pts[0].x++;
				pts[1].x++;
				pts[2].y++;
				pts[3].y++;
				pts[4].x = pts[8].x;	pts[4].y = pts[8].y+1;
				pts[5].x = pts[9].x;	pts[5].y = pts[9].y+1;
				pts[6].x = pts[10].x+1;	pts[6].y = pts[10].y;
				pts[7].x = pts[11].x+1;	pts[7].y = pts[11].y;
				setPen(dc, getRGB(cYWHITE));
				setBrush(dc, getRGB(cYWHITE));
				dc->Polygon(pts, 8);

				pts[0] = pts[7];
				pts[1] = pts[6];
				pts[2] = pts[5];
				pts[3] = pts[4];
				pts[4].x = tRc.right-2;	pts[4].y = tRc.top+3;
				pts[5].x = pts[4].x;	pts[5].y = tRc.bottom;
				setPen(dc, m_pRGB);
				setBrush(dc, m_pRGB);
				dc->Polygon(pts, 6);

				pts[0].x = pts[3].x+1;	pts[0].y = tRc.top+2;
				pts[1].x = pts[0].x+1;	pts[1].y = pts[0].y+1;
				setPen(dc, getRGB(cEDGE));
				setBrush(dc, getRGB(cEDGE));
				dc->Polygon(pts, 2);

				pts[0].x = pts[1].x;	pts[0].y++;
				pts[1].x = pts[0].x;	pts[1].y = tRc.bottom;
				dc->Polygon(pts, 2);

				pts[0].x = tRc.left;	pts[0].y = tRc.bottom-1;
				pts[1].x = tRc.right-1;	pts[1].y = tRc.bottom-1;
				pts[2].x = tRc.right-1;	pts[2].y = tRc.bottom;
				pts[3].x = tRc.left;	pts[3].y = tRc.bottom;
				setPen(dc, getRGB(cYWHITE));
				setBrush(dc, getRGB(cYWHITE));
				dc->Polygon(pts, 4);

				pts[2].y--;
				pts[3].y--;
				setPen(dc, getRGB(cBOTTOM));
				setBrush(dc, getRGB(cBOTTOM));
				dc->Polygon(pts, 4);

				setTextColor(dc, m_tRGB);
				setPaintColor(dc, m_pRGB);
				tRc1.DeflateRect(2, 3, 2, 2);
				break;
			}

			pts[0].x++;
			pts[1].x++;		pts[1].y++;
			pts[2].x += 1;		pts[2].y++;
			pts[3].x--;		pts[3].y++;
			pts[4].x--;		pts[4].y++;
			pts[5].x = pts[4].x;	pts[5].y = pts[4].y+1;
			pts[6].x = pts[3].x;	pts[6].y = pts[3].y+1;
			pts[7].x = pts[2].x;	pts[7].y = pts[2].y+1;
			pts[8].x = pts[1].x+1;	pts[8].y = pts[1].y;
			pts[9].x = pts[8].x;	pts[9].y = pts[0].y;
			setPen(dc, getRGB(cYWHITE));
			setBrush(dc, getRGB(cYWHITE));
			dc->Polygon(pts, 10);

			pts[0] = pts[9];
			pts[1] = pts[8];
			pts[2] = pts[7];
			pts[3].x = pts[6].x;	pts[3].y = pts[6].y;
			pts[4].x = pts[3].x+2;	pts[4].y = pts[3].y+2;
			pts[5].x = tRc.right-3;	pts[5].y = pts[0].y;
			setPen(dc, m_fpRGB);
			setBrush(dc, m_fpRGB);
			dc->Polygon(pts, 6);

			pts[0].x--;		pts[0].y = pts[1].y-1;
			pts[1].x = tRc.left+3;	pts[1].y = pts[2].y-1;
			dc->Polygon(pts, 2);

			pts[0].x = pts[3].x+1;	pts[0].y = pts[1].y;
			pts[1].x = pts[0].x+1;	pts[1].y = pts[0].y+1;
			dc->Polygon(pts, 2);

			pts[0].x = pts[1].x+1;	pts[0].y = pts[1].y+1;
			pts[1].x = pts[0].x;	pts[1].y = tRc.bottom;
			setPen(dc, getRGB(cEDGE));
			setBrush(dc, getRGB(cEDGE));
			dc->Polygon(pts, 2);

			pts[0].x = tRc.left;	pts[0].y = tRc.bottom;
			pts[1].x = pts[0].x+1;	pts[1].y = pts[0].y;
			setPen(dc, getRGB(cYWHITE));
			setBrush(dc, getRGB(cYWHITE));
			dc->Polygon(pts, 2);

			pts[0].x = tRc.right-2;
			pts[1].x = pts[0].x+2;
			dc->Polygon(pts, 2);

			pts[0].x++;		pts[0].y--;
			pts[1].x = tRc.right;	pts[1].y = pts[0].y+1;
			setPen(dc, getRGB(cBOTTOM));
			setBrush(dc, getRGB(cBOTTOM));
			dc->Polygon(pts, 2);

			setTextColor(dc, m_ftRGB);
			setPaintColor(dc, m_fpRGB);
			tRc1.DeflateRect(2, 5, 1, 0);
			break;

		case TAB_RECT:
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX;
			setPen(dc, focus_this ? m_fpRGB : m_pRGB);
			brush = setBrush(dc, focus_this ? m_fpRGB : m_pRGB);
			dc->SetPolyFillMode(ALTERNATE);
			pts[0].x = tRc.left;			pts[0].y = tRc.bottom;
			pts[1].x = tRc.left;			pts[1].y = tRc.top;
			if (m_scroll && ii == m_last-1 && scroll)
			{
				pts[2].x = tRc.right;		pts[2].y = tRc.top;
				pts[3].x = tRc.right;		pts[3].y = tRc.bottom;
				dc->Polygon(pts, 4);
			}
			else
			{
				pts[2].x = tRc.right-(tRc.Height()/2);	pts[2].y = tRc.top;
				pts[3].x = tRc.right;			pts[3].y = tRc.top + (tRc.Height()/2);
				pts[4].x = tRc.right;			pts[4].y = tRc.bottom;
				dc->Polygon(pts, 5);
			}

			setPen(dc, getRGB(cBLACK));
			brush = setBrush(dc, getRGB(cBLACK));
			dc->SetPolyFillMode(ALTERNATE);
			pts[0].x = tRc.left;			pts[0].y = tRc.bottom;
			pts[1].x = tRc.left;			pts[1].y = tRc.top;
			if (m_scroll && ii == m_last-1 && scroll)
			{
				pts[2] = tRc.right;		pts[2].y = tRc.top;
				pts[3] = pts[1];
				dc->Polygon(pts, 4);
			}
			else
			{
				pts[2].x = tRc.right-(tRc.Height()/2);	pts[2].y = tRc.top;
				pts[3].x = tRc.right;			pts[3].y = tRc.top + (tRc.Height()/2);
				pts[4] = pts[2];
				pts[5] = pts[1];
				dc->Polygon(pts, 6);
			}

			setPen(dc, getRGB(cYWHITE));
			brush = setBrush(dc, getRGB(cYWHITE));
			dc->SetPolyFillMode(ALTERNATE);
			pts[0].x++;		pts[0].y--;
			pts[1].x++;		pts[1].y++;
			pts[2].x--;		pts[2].y++;
			if (m_scroll && ii == m_last-1 && scroll)
			{
				pts[3] = pts[1];
				dc->Polygon(pts, 4);
			}
			else
			{
				pts[3].x--;		pts[3].y++;
				pts[4] = pts[2];
				pts[5] = pts[1];
				dc->Polygon(pts, 6);
			}

			setPen(dc, getRGB(cBLACK));
			brush = setBrush(dc, getRGB(cBLACK));
			dc->SetPolyFillMode(ALTERNATE);
			if (focus_this)
			{
				pts[0].x = tRc.right;	pts[0].y = tRc.bottom;
				pts[1].x = tRc.right;	pts[1].y = tRc.top + (tRc.Height()/2);
				dc->Polygon(pts, 2);
			}
			else
			{
				pts[0].x = tRc.left;	pts[0].y = tRc.bottom;
				pts[1].x = tRc.right;	pts[1].y = tRc.bottom;
				if (m_scroll && ii == m_last-1 && scroll)
					dc->Polygon(pts, 2);
				else
				{
					pts[2].x = tRc.right;	pts[2].y = tRc.top + (tRc.Height()/2);
					pts[3] = pts[1];
					pts[4] = pts[0];
					dc->Polygon(pts, 5);
				}
			}

			setPen(dc, getRGB(cEDGE));
			brush = setBrush(dc, getRGB(cEDGE));
			dc->SetPolyFillMode(ALTERNATE);
			if (focus_this)
			{
				pts[0].x--;
				pts[1].x--;		pts[1].y++;
				dc->Polygon(pts, 2);
			}
			else
			{
				pts[0].x++;		pts[0].y--;
				pts[1].x--;		pts[1].y--;
				if (m_scroll && ii == m_last-1 && scroll)
					dc->Polygon(pts, 2);
				else
				{
					pts[2].x--;		pts[2].y++;
					pts[3] = pts[1];
					pts[4] = pts[0];
					dc->Polygon(pts, 5);
				}
			}
			tRc1 = tRc;
			tRc1.top+=2;
			tmpS = _T(" ") + tmpS;
			setTextColor(dc, focus_this ? m_ftRGB : m_tRGB);
			setPaintColor(dc, focus_this ? m_fpRGB : m_pRGB);
			break;

		case TAB_BORDER:
		default:
			switch (m_form->align)
			{
			case AL_LEFT:
				style = DT_CENTER|DT_TOP|DT_NOPREFIX;

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(0, 1);
				else
					tRc1.DeflateRect(0, 2);
				tRc1.right = tRc1.left+1;
				dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));
				tRc1.OffsetRect(1, 0);
				dc->FillSolidRect(tRc1, getRGB(cYWHITE));		// left				
				
				tRc1 = tRc;
				if (focus_this)
					tRc1.left++;
				else
					tRc1.left += 2;
				tRc1.bottom = tRc1.top+1;
				dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));
				tRc1.OffsetRect(0, 1);
				dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				if (focus_this)
				{
					tRc1.left = tRc1.right-1;
					tRc1.OffsetRect(0, -1);
					dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				}
				else
				{
					tRc1.left--;
					tRc1.right = tRc1.left+1;
					dc->FillSolidRect(tRc1, getRGB(cGRAY));
				}							// top

				if (!(scroll && ii == m_last-1))
				{
					tRc1 = tRc;
					if (focus_this)
						tRc1.left++;
					else
						tRc1.left += 2;
					tRc1.top = tRc1.bottom-1;
					dc->FillSolidRect(tRc1, focus_this ? getRGB(cBLACK) : getRGB(cBOTTOM));
					if (focus_this)
					{
						tRc1.left = tRc1.right-1;
						dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					}
					else
					{
						tRc1.OffsetRect(-1, -1);
						tRc1.right = tRc1.left+1;
						dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
					}
				}							// bottom

				if (!focus_this)
				{
					tRc1 = tRc;
					tRc1.left = tRc1.right - 2;
					dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					tRc1.right--;
					dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
				}							// right

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(2, 2, 0, 1);
				else
					tRc1.DeflateRect(2, 2, 2, 2);
				dc->FillSolidRect(tRc1, focus_this ? m_fpRGB : m_pRGB); // text rect
				break;

			case AL_RIGHT:
				style = DT_CENTER|DT_TOP|DT_NOPREFIX;

				tRc1 = tRc;				
				if (focus_this)
					tRc1.right--;
				else
					tRc1.right -= 2;
				tRc1.bottom = tRc1.top+1;
				dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));
				tRc1.OffsetRect(0, 1);
				dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				if (focus_this)
				{
					tRc1.OffsetRect(0, -1);
					tRc1.right = tRc1.left+1;
					dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				}
				else
				{
					tRc1.left = tRc1.right;
					tRc1.right++;
					dc->FillSolidRect(tRc1, getRGB(cGRAY));
				}							// top

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(0, 1);
				else
					tRc1.DeflateRect(0, 2);
				tRc1.left = tRc1.right-1;
				dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));
				tRc1.OffsetRect(1, 0);
				dc->FillSolidRect(tRc1, getRGB(cYWHITE));		// right

				if (!(scroll && ii == m_last-1))
				{
					tRc1 = tRc;
					if (focus_this)
						tRc1.right--;
					else
						tRc1.right -= 2;
					tRc1.top = tRc1.bottom-1;
					dc->FillSolidRect(tRc1, focus_this ? getRGB(cBLACK) : getRGB(cBOTTOM));
					if (focus_this)
					{
						tRc1.right = tRc1.left+1;
						dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					}
					else
					{
						tRc1.OffsetRect(0, -1);
						tRc1.left = tRc1.right;
						tRc1.right++;
						dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
					}
				}							// bottom

				if (!focus_this)
				{
					tRc1 = tRc;
					tRc1.right = tRc1.left + 2;
					dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
					tRc1.right--;
					dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				}							// left

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(0, 2, 1, 1);
				else
					tRc1.DeflateRect(2, 2, 2, 2);
				dc->FillSolidRect(tRc1, focus_this ? m_fpRGB : m_pRGB); // text rect
				break;

			case AL_BOTTOM:
				style = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;

				tRc1 = tRc;
				if (focus_this)
					tRc1.bottom--;
				else
					tRc1.bottom -= 2;
				tRc1.right = tRc1.left+2;
				dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				tRc1.right--;
				dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));	// left

				if (scroll && ii == m_last-1)
				{
					style |= DT_LEFT;
					tmpS = _T(" ") + tmpS;
				}
				else
				{
					style |= DT_CENTER;
					tRc1 = tRc;
					if (focus_this)
						tRc1.bottom--;
					else
						tRc1.bottom -= 2;
					tRc1.left = tRc1.right-1;
					dc->FillSolidRect(tRc1, focus_this ? getRGB(cBLACK) : getRGB(cBOTTOM));
					if (focus_this)
					{
						tRc1.bottom = tRc1.top+1;
						dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					}
					else
					{
						tRc1.right  = tRc1.left;
						tRc1.left--;
						tRc1.top    = tRc1.bottom;
						tRc1.bottom = tRc1.top+1;
						dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
					}
				}							// right

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(1, 0);
				else
					tRc1.DeflateRect(2, 0);
				tRc1.top = tRc1.bottom-1;
				dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));
				tRc1.bottom = tRc1.top;
				tRc1.top--;
				dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				if (!focus_this)
				{
					tRc1.left--;
					tRc1.right = tRc1.left+1;
					dc->FillSolidRect(tRc1, getRGB(cGRAY));
				}							// bottom

				if (!focus_this)
				{
					tRc1 = tRc;
					tRc1.bottom = tRc1.top+1;
					dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					tRc1.top = tRc1.bottom;
					tRc1.bottom = tRc1.top+1;
					dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
				}							// top

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(2, 0, 1, 1);
				else
					tRc1.DeflateRect(2, 2, 2, 2);
				dc->FillSolidRect(tRc1, focus_this ? m_fpRGB : m_pRGB); // text rect
				break;

			case AL_TOP:
			default:
				style = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;

				tRc1 = tRc;
				if (focus_this)
					tRc1.top++;
				else
					tRc1.top += 2;
				tRc1.right = tRc1.left+2;
				dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				tRc1.right = tRc1.left+1;
				tRc1.bottom--;
				dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));	// left
				
				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(1, 0);
				else
					tRc1.DeflateRect(2, 0);
				tRc1.bottom = tRc1.top+1;
				dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));
				tRc1.top = tRc1.bottom;
				tRc1.bottom++;
				dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				if (!focus_this)
				{
					tRc1.left  = tRc.left+1;
					tRc1.right = tRc1.left+1;
					dc->FillSolidRect(tRc1, getRGB(cGRAY));
				}							// top

				if (scroll && ii == m_last-1)
				{
					style |= DT_LEFT;
					tmpS = _T(" ") + tmpS;
				}
				else
				{
					style |= DT_CENTER;
					tRc1 = tRc;
					if (focus_this)
						tRc1.top++;
					else
						tRc1.top += 2;
					tRc1.left = tRc1.right-1;
					dc->FillSolidRect(tRc1, focus_this ? getRGB(cBLACK) : getRGB(cBOTTOM));
					if (focus_this)
					{
						tRc1.top = tRc1.bottom-1;
						dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					}
					else
					{
						tRc1.right  = tRc1.left;
						tRc1.left--;
						tRc1.bottom = tRc1.top;
						tRc1.top--;
						dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
					}
				}							// right

				if (!focus_this)
				{
					tRc1 = tRc;
					tRc1.top = tRc1.bottom-2;
					dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					tRc1.bottom = tRc1.top+1;
					dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
				}							// bottom

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(2, 2, 1, 0);
				else
					tRc1.DeflateRect(2, 2, 2, 2);
				dc->FillSolidRect(tRc1, focus_this ? m_fpRGB : m_pRGB); // text rect
				break;
			}
			setTextColor(dc, focus_this ? m_ftRGB : m_tRGB);
			setPaintColor(dc, focus_this ? m_fpRGB : m_pRGB);
			break;
		}

		if (m_vertical)
			drawVText(dc, tmpS, tRc1, style);
		else
		{
			tRc1.DeflateRect(2, 0);
			dc->DrawText(tmpS, tRc1, style);
		}
	}

	if (m_form->type == TAB_BORDER || m_form->type == TAB_OBLIQUE)
	{
		switch (m_form->align)
		{
		case AL_LEFT:
			tRc1 = m_pRc;
			tRc1.left   = tRc1.right - 1;
			tRc1.bottom = tRc1.top + 1;
			dc->FillSolidRect(tRc1, getRGB(cYWHITE));
			tRc1.OffsetRect(-1, 0);
			dc->FillSolidRect(tRc1, getRGB(cBOTTOM));	// top margin

			tRc1.top    = tRc.bottom;
			tRc1.bottom = m_pRc.bottom;
			dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
			tRc1.OffsetRect(1, 0);
			dc->FillSolidRect(tRc1, getRGB(cYWHITE));	// bottom margin
			break;

		case AL_RIGHT:
			tRc1 = m_pRc;
			tRc1.right  = tRc1.left + 1;
			tRc1.bottom = tRc1.top + 1;
			dc->FillSolidRect(tRc1, getRGB(cYWHITE));
			tRc1.OffsetRect(1, 0);
			dc->FillSolidRect(tRc1, getRGB(cBOTTOM));	// top margin

			tRc1.top    = tRc.bottom;
			tRc1.bottom = m_pRc.bottom;
			dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
			tRc1.OffsetRect(-1, 0);
			dc->FillSolidRect(tRc1, getRGB(cYWHITE));	// bottom margin
			break;

		case AL_BOTTOM:
			tRc1 = m_pRc;
			tRc1.bottom = tRc1.top + 1;
			tRc1.right  = tRc1.left + 1;
			dc->FillSolidRect(tRc1, getRGB(cYWHITE));
			tRc1.OffsetRect(0, 1);
			dc->FillSolidRect(tRc1, getRGB(cBOTTOM));	 // left margin

			tRc1.left  = tRc.right;
			tRc1.right = m_pRc.right;
			dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
			tRc1.OffsetRect(0, -1);
			dc->FillSolidRect(tRc1, getRGB(cYWHITE));	// right margin
			break;

		case AL_TOP:
		default:
			tRc1 = m_pRc;
			tRc1.top   = tRc1.bottom - 1;
			tRc1.right = tRc1.left + 1;
			dc->FillSolidRect(tRc1, getRGB(cYWHITE));
			tRc1.OffsetRect(0, -1);
			dc->FillSolidRect(tRc1, getRGB(cBOTTOM));	// left margin

			tRc1.left  = tRc.right;
			tRc1.right = m_pRc.right;
			dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
			tRc1.OffsetRect(0, 1);
			dc->FillSolidRect(tRc1, getRGB(cYWHITE));
			break;
		}
	}

	if (m_scroll)
	{
		tRc = pRc;
		if (!m_vertical)
		{
			tRc.bottom--;
			tRc.left  = pRc.right;
			tRc.right = m_pRc.right;
		}
		else
		{
			tRc = m_pRc;
			tRc.top = tRc.bottom - (m_pRc.Width()/2)-1;
		}
		drawScroll(dc, tRc);
	}

	dc->RestoreDC(ndc);
}

void CfmUserTab::OnLButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	if (m_count == 0)
		return;

	CRect	tRc,  pRc = m_pRc;
	bool	focus_this;
	int	gap;

	if (down)
	{
		CString	tmpS  = _T("");
		int	dx    = 0;
		bool	bForm = m_bForm;

		m_axform->m_cursor = pt;
		m_bForm = false;
		m_click = ckNone;
		if (m_scroll && m_scr.PtInRect(pt))
		{
			CRect	tRc1;
			int	start = m_start;

			gap = m_scr.Width() / 2;
			tRc = tRc1 = m_scr;
			tRc.right  = tRc.left + gap+1;
			tRc1.left  = tRc.right;
			tRc1.right = tRc1.left + gap;
			if (tRc.PtInRect(pt))
			{
				m_click = ckLeft;
				result  = RC_LEFT;
				scroll();
			}
			else if (tRc1.PtInRect(pt))
			{
				m_click = ckRight;
				result  = RC_RIGHT;
				scroll(false);
			}

			if (m_click != ckNone)
			{
				m_bForm = false;
				if (bForm != m_bForm || start != m_start)
				{
					tRc = m_pRc;
					tRc.InflateRect(1, 1);
					invalidateRect(tRc, true);
				}
				else
					invalidateRect(m_click == ckLeft ? tRc : tRc1, false);
			}
		}
		else if (m_more.PtInRect(pt))
		{
			m_click = ckMore;
			invalidateRect(&m_more, false);
		}
		else
		{
			for (int ii = 0; ii < m_count; ii++)
			{
				tmpS = m_map.GetAt(ii);
				if (tmpS[0] == '1')
					dx++;
			}
			if (dx == 0)
				return;

			CRgn	rgn;
			CPoint	pts[5];

			if (m_vertical)
			{
				if (m_scroll)
					pRc.bottom -= (pRc.Width()/2);
				else
					pRc.bottom = pRc.top + dx;
				dx = pRc.top;
			}
			else
			{
				if (m_scroll)
				{
					if (m_form->type == TAB_IMAGE)
					{
						pRc.right -= (wSCR*2);
						if (m_form->attr & FA_DATE)
							pRc.right -= m_moreW;
					}
					else
						pRc.right -= (int)(float)(m_pRc.Height() * 2.0 / 3.0) * 2;
				}
				else
					pRc.right = pRc.left + dx;

				dx = pRc.left;
			}
			if (m_form->type == TAB_BORDER || m_form->type == TAB_OBLIQUE)
				dx++;

			for (int ii = m_start; ii < m_last; ii++)
			{
				tmpS = m_map.GetAt(ii);
				if (tmpS[0] != '1')
					continue;

				tmpS = m_caption.GetAt(ii);
				tRc  = pRc;

				if (m_vertical)
					tRc.top = dx;
				else
					tRc.left = dx;

				focus_this = false;
				if (ii == m_index && m_hasfocus && !m_bForm)
					focus_this = true;

				if (!m_scroll || ii < m_last-1 || (m_scroll && m_last == getLastVisibleItem()+1))
				{
					CDC*	dc = getDC();
					setFont(dc, focus_this ? 1 : -1);
					dx += getSize(dc, ii);
					if (m_vertical)
						tRc.bottom = dx;
					else
						tRc.right = dx;
					releaseDC(dc);
				}

				switch (m_form->type)
				{
				case TAB_BORDER:
					if (!focus_this)
						break;

					switch (m_form->align)
					{
					case AL_LEFT:
						tRc.left += 2;			break;
					case AL_RIGHT:
						tRc.right -= 2;			break;
					case AL_BOTTOM:
						tRc.bottom -= 2;		break;
					case AL_TOP:
					default:
						tRc.top += 2;			break;
					}
					break;

				case TAB_OBLIQUE:
					if (!focus_this)
						break;

					switch (m_form->align)
					{
					case AL_LEFT:
						tRc.left += 3;			break;
					case AL_RIGHT:
						tRc.right -= 3;			break;
					case AL_BOTTOM:
						tRc.bottom -= 3;		break;
					case AL_TOP:
					default:
						tRc.top += 3;			break;
					}
					break;

				case TAB_RECT:
					pts[0].x = tRc.left;		pts[0].y = tRc.top;
					pts[1].x = tRc.left;		pts[1].y = tRc.bottom;
					if (ii == getLastVisibleItem())
					{
						pts[2].x = tRc.right + (tRc.Height()/2);
						pts[2].y = tRc.bottom;
						pts[3].x = pts[2].x - tRc.Height();
						pts[3].y = tRc.top;
						rgn.CreatePolygonRgn(pts, 4, ALTERNATE);
					}
					else
					{
						pts[2].x = tRc.right;
						pts[2].y = tRc.bottom;
						pts[3].x = tRc.right;
						pts[3].y = tRc.top + (tRc.Height()/2);
						pts[4].x = tRc.right - (tRc.Height()/2);
						pts[4].y = tRc.top;
						rgn.CreatePolygonRgn(pts, 5, ALTERNATE);
					}
					break;
				}

				if ((m_form->type == TAB_RECT && rgn.PtInRegion(pt)) || (m_form->type != TAB_RECT && tRc.PtInRect(pt)))
				{
					m_click = ckTab;
					result  = ii;
					m_bForm = false;
					if (result != m_index || m_bForm)
					{
						tRc = m_pRc;
						m_index = result;
						invalidateRect(tRc, true);
						m_axform->m_view->UpdateWindow();
					}

					if (m_form->cursor >= CS_CURSOR)
						m_axform->DoSomething(doCURSOR, this, m_form->cursor);
					if (m_form->type == TAB_RECT)
						rgn.DeleteObject();
					break;
				}
				if (m_form->type == TAB_RECT)
					rgn.DeleteObject();
			}
		}
		return;
	}

	switch (m_click)
	{
	case ckLeft:
		m_click = ckNone;
		gap = m_scr.Width() / 2;
		tRc = m_scr;
		tRc.right = tRc.left + gap+1;
		invalidateRect(tRc, false);
		break;

	case ckRight:
		m_click = ckNone;
		gap = m_scr.Width() / 2;
		tRc = m_scr;
		tRc.left += (gap+1);
		tRc.right = tRc.left + gap;
		invalidateRect(tRc, false);
		break;

	case ckMore:
		m_click = ckNone;
		if (m_more.PtInRect(pt))
		{
			CString	text;
			int	base = 100, index = 0;

			CMenu *popM = new CMenu();
			if (!popM->CreatePopupMenu())
			{
				delete popM;
				break;
			}
			if (m_form->attr & FA_ENUM)
			{
				popM->AppendMenu(MF_STRING|MF_ENABLED, 99, _T("ÅÇ ÆíÁý"));
				popM->AppendMenu(MF_SEPARATOR);
			}
			for (index = 0; index < m_caption.GetSize(); index++)
			{
				text = m_map.GetAt(index);
				if (text[0] != '1')
					continue;
				if (m_index == index)
					popM->AppendMenu(MF_STRING|MF_ENABLED|MF_CHECKED, base+index, m_caption.GetAt(index));
				else
					popM->AppendMenu(MF_STRING|MF_ENABLED, base+index, m_caption.GetAt(index));
			}
			pt = CPoint(m_more.right, m_more.bottom);
			m_axform->m_view->ClientToScreen(&pt);
			index = popM->TrackPopupMenu(TPM_RIGHTALIGN|TPM_RETURNCMD, pt.x, pt.y, m_axform->m_view);
			delete popM;
			if (index == 99)
				m_axform->DoSomething(doMODAL, this);
			else if (index >= base)
			{
				index -= base;
				text = m_map.GetAt(index);
				if (text.GetLength() > 2)
					text = text.Mid(2);
				else
					text.Empty();
				SetCurSel(atoi(text));
				result = m_index;
			}
		}
		invalidateRect(&m_more, false);

	default:
		m_click = ckNone;
		break;
	}
}

int CfmUserTab::OnDrag(CPoint pt, int& result)
{
	int	rc, dx = 0;
	bool	focus_this;
	CRgn	rgn;
	CRect	tRc,  pRc  = m_pRc;
	CPoint	pts[5];

	rc = CfmBase::OnDrag(pt, result);
	if (result != RC_IN)
	{
		m_axform->DoSomething(doCURSOR, this, CS_NONE);
		return rc;
	}

	for (int ii = 0; ii < m_count; ii++)
	{
		if (m_map.GetAt(ii).GetAt(0) == '1')
			dx++;
	}
	if (dx == 0)
	{
		m_axform->DoSomething(doCURSOR, this, CS_NONE);
		return rc;
	}
	if (m_scroll && m_scr.PtInRect(pt))
	{
		m_move = mvScroll;
		if (m_form->cursor >= CS_CURSOR)
			m_axform->DoSomething(doCURSOR, this, m_form->cursor);
		return rc;
	}
	if (m_more.PtInRect(pt))
	{
		m_move = mvMore;
		if (m_form->cursor >= CS_CURSOR)
			m_axform->DoSomething(doCURSOR, this, m_form->cursor);
		return rc;
	}

	if (m_vertical)
	{
		if (m_scroll)
			pRc.bottom -= (pRc.Width()/2);
		else
			pRc.bottom = pRc.top + dx;
		dx = pRc.top;
	}
	else
	{
		if (m_scroll)
		{
			if (m_form->type == TAB_IMAGE)
			{
				pRc.right -= (wSCR*2);
				if (m_form->attr & FA_DATE)
					pRc.right -= m_moreW;
			}
			else
				pRc.right -= (int)(float)(m_pRc.Height() * 2.0 / 3.0) * 2;
		}
		else
			pRc.right = pRc.left + dx;
		dx = pRc.left;
	}
	if (m_form->type == TAB_BORDER || m_form->type == TAB_OBLIQUE)
		dx++;

	for (int ii = m_start; ii < m_last; ii++)
	{
		if (m_map.GetAt(ii).GetAt(0)  != '1')
			continue;

		tRc  = pRc;

		if (m_vertical)
			tRc.top = dx;
		else
			tRc.left = dx;

		focus_this = false;
		if (ii == m_index && m_hasfocus && !m_bForm)
			focus_this = true;

		if (!m_scroll || ii < m_last-1 || (m_scroll && m_last == getLastVisibleItem()+1))
		{
			CDC*	dc = getDC();
			setFont(dc, focus_this ? 1 : -1);
			dx += getSize(dc, ii);
			if (m_vertical)
				tRc.bottom = dx;
			else
				tRc.right = dx;
			releaseDC(dc);
		}

		switch (m_form->type)
		{
		case TAB_BORDER:
			if (!focus_this)
				break;

			switch (m_form->align)
			{
			case AL_LEFT:
				tRc.left += 2;			break;
			case AL_RIGHT:
				tRc.right -= 2;			break;
			case AL_BOTTOM:
				tRc.bottom -= 2;		break;
			case AL_TOP:
			default:
				tRc.top += 2;			break;
			}
			break;

		case TAB_OBLIQUE:
			if (!focus_this)
				break;

			switch (m_form->align)
			{
			case AL_LEFT:
				tRc.left += 3;			break;
			case AL_RIGHT:
				tRc.right -= 3;			break;
			case AL_BOTTOM:
				tRc.bottom -= 3;		break;
			case AL_TOP:
			default:
				tRc.top += 3;			break;
			}
			break;

		case TAB_RECT:
			pts[0].x = tRc.left;		pts[0].y = tRc.top;
			pts[1].x = tRc.left;		pts[1].y = tRc.bottom;
			pts[2].x = tRc.right;		pts[2].y = tRc.bottom;
			if (ii == getLastVisibleItem())
			{
				pts[3].x = tRc.right - tRc.Height();
				pts[3].y = tRc.top;
				rgn.CreatePolygonRgn(pts, 4, ALTERNATE);
			}
			else
			{
				pts[3].x = tRc.right;
				pts[3].y = tRc.top + (tRc.Height()/2);
				pts[4].x = tRc.right - (tRc.Height()/2);
				pts[4].y = tRc.top;
				rgn.CreatePolygonRgn(pts, 5, ALTERNATE);
			}
			break;
		}
				
		if ((m_form->type == TAB_RECT && rgn.PtInRegion(pt)) || (m_form->type != TAB_RECT && tRc.PtInRect(pt)))
		{
			if (m_form->type == TAB_RECT)
				rgn.DeleteObject();
			m_move  = mvTab;
			m_imove = ii;
			if (m_form->cursor >= CS_CURSOR)
				m_axform->DoSomething(doCURSOR, this, m_form->cursor);
			return ii;
		}
		if (m_form->type == TAB_RECT)
			rgn.DeleteObject();
	}

	return -1;
}

bool CfmUserTab::OnDrop(CPoint pt, CString Data, CString& key)
{
	return false;
}

void CfmUserTab::SetFgColor(int rgb, int col, int row)
{
	if (col >= 0)		// select tRGB
	{
		DWORD	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
		if (m_ftRGB == color)
			return;

		m_ftRGB = color;
		invalidateRect(&m_pRc, false);
	}
	else
		CfmBase::SetFgColor(rgb);
}
void CfmUserTab::SetBkColor(int rgb, int col, int row)
{
	if (col >= 0)		// selected pRGB
	{
		DWORD	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
		if (m_fpRGB == color)
			return;

		m_fpRGB = color;
		invalidateRect(&m_pRc, false);
	}
	else
		CfmBase::SetBkColor(rgb);
}

void CfmUserTab::ReadAll(CString& data, bool edit, bool head)
{
	data.Format("%d", m_index);
	for (int ii = 0; ii < m_count; ii++)
	{
		data += '|';
		data += m_map.GetAt(ii);
		data += m_caption.GetAt(ii);
	}
}

void CfmUserTab::ReadData(CString& data, bool edit, int size, int col, int row)
{
	if (m_index < 0 || m_index > m_map.GetUpperBound())
		return;

	data = m_map.GetAt(m_index);
	if (data.GetLength() > 2)
		data = data.Mid(2);
	else
		data = _T("");
}

void CfmUserTab::WriteAll(CString str, bool redraw)
{
	if (!(m_form->attr & FA_ENUM))
		return;

	int offs = str.Find('|');
	if (offs == -1)
		return;

	CString	tmps;
	m_index = atoi(str.Left(offs));
	str = str.Mid(offs+1);

	m_caption.RemoveAll();
	m_map.RemoveAll();
	for ( ; !str.IsEmpty(); )
	{
		offs = str.Find('|');
		if (offs != -1)
		{
			tmps = str.Left(offs);
			str  = str.Mid(offs+1);
		}
		else
		{
			tmps = str;
			str = _T("");
		}

		if (tmps.GetLength() < L_MAPN)
			continue;

		m_map.Add(tmps.Left(L_MAPN));
		m_caption.Add(tmps.Mid(L_MAPN));
	}
	m_count = min(m_map.GetSize(), m_caption.GetSize());

	if (redraw)
		invalidateRect(&m_pRc, true);
}

void CfmUserTab::WriteData(CString data, bool redraw, int key, int visible)
{
	CString	tmps;
	int	index = 0;

	for (int ii = 0; ii < m_count; ii++)
	{
		tmps = m_map.GetAt(ii);
		if (tmps.GetLength() > 2)
			tmps = tmps.Mid(2);
		else
			tmps = _T("");

		index = ii;
		if (key == atoi(tmps))
			break;
	}

	if (index >= m_count)
		return;

	tmps = m_map.GetAt(index);
	tmps.SetAt(0, visible ? '1' : '0');
	m_map.SetAt(index, tmps);
	
	if (data.GetLength() > 0)
	{
		tmps = m_caption.GetAt(index);
		if (tmps.Compare(data))
			m_caption.SetAt(index, data);
	}

	if (redraw)
		invalidateRect(&m_pRc, true);
}

void CfmUserTab::SetCurSel(int index, bool redraw)
{
	if (index == -1)
	{
		m_bForm = true;
		if (redraw)
			invalidateRect(&m_pRc, true);
		return;
	}

	m_bForm = false;
	CString	tmpS;
	int	idx = m_index;
	for (int ii = 0; ii < m_count; ii++)
	{
		tmpS = m_map.GetAt(ii);
		if (tmpS.GetLength() > 2)
			tmpS = tmpS.Mid(2);
		else
			tmpS = _T("");

		if (index == atoi(tmpS))
		{
			m_index = ii;
			break;
		}
	}

	m_start = getStartIndex(m_index);
	if (redraw && m_index != idx)
		invalidateRect(&m_pRc, true);
}

int CfmUserTab::GetCurSel()
{
	if (m_index < 0 || m_index > m_map.GetUpperBound()
		|| m_index > m_caption.GetUpperBound())
		return 0;

	CString	tmpS = m_map.GetAt(m_index);
	if (tmpS.GetLength() > 2)
		tmpS = tmpS.Mid(2);
	else
		tmpS = _T("");

	return	atoi(tmpS);
}

int CfmUserTab::GetSize()
{
	return m_count;
}

void CfmUserTab::GetEnum(CString& text, int& value, int func)
{
	if (func == 101)
	{
		int index = getIndexByID(value);
		if (index >= 0 && index < m_caption.GetSize())
			text = m_caption.GetAt(index);
		else
			text = _T("");
		return;
	}

	value = m_type;
	switch (m_type)
	{
	case ET_TAB:
	case ET_USER:
		text = m_name;
		break;
	case ET_ITEM:
	default:
		ReadData(text);
		break;
	}
}

void CfmUserTab::SetEnum(CString text, int value, int func)
{
	switch (func)
	{
	case 101:
		value = getIndexByID(value);
		if (value >=0 && value < m_caption.GetSize())
			m_caption.SetAt(value, text);
		return;

	case 100:
		if (m_form->type == TAB_IMAGE)
		{
			char	wb[32];

			wb[0] = '\0';
			if (!(m_form->attr & FA_SKIP))
			{
				text.Format(_T("%s/%s/%s"), m_axform->m_root.GetString(), TABDIR, PALETTE);
				GetPrivateProfileString("General", "Palette", "", wb, sizeof(wb), text);
			}
			loadBitmap(wb);
			if (value)
				invalidateRect(&m_pRc, true);
		}
		return;
	}

	if (value < 0)
		return;

	m_type = ET_USER;
	value = GetCurSel();
	loadTab(text);
	SetCurSel(value);

	int	last = 0, width = 0;
	CDC*	dc = getDC();
	setFont(dc);
	m_scroll = isScrolled(dc, last, width, 0);
	releaseDC(dc);
	invalidateRect(&m_pRc, true);
}

BOOL CfmUserTab::IsTabVisible(long ID)
{
	int index = getIndexByID(ID);

	if (index >= 0)
	{
		CString str = m_map.GetAt(index);
		if (str.GetAt(0) == '1')
			return TRUE;
	}

	return FALSE;
}

void CfmUserTab::SetTabVisible(long ID, BOOL visible)
{
	int index = getIndexByID(ID);

	if (index >= 0)
	{
		CString str = m_map.GetAt(index);
		str.SetAt(0, visible ? '1' : '0');
		m_map.SetAt(index, str);
		invalidateRect(&m_pRc, true);
	}
}

void CfmUserTab::loadTab(CString text)
{
	if (text.IsEmpty())
	{
		m_count = m_form->keys;
		m_caption.RemoveAll();
		m_map.RemoveAll();
		if (m_count <= 0)
			return;

		char*	map	= &m_axform->m_strR[m_form->vals[0]];
		char*	caption	= &m_axform->m_strR[m_form->vals[1]];
		for (int ii = 0; ii < m_count; ii++)
		{
			m_caption.Add(caption);
			caption += strlen(caption);
			caption++;

			m_map.Add(map);
			map += strlen(map);
			map++;
		}
		m_start = 0;
		m_index = m_form->checked;
		m_count = min(m_count, min(m_map.GetSize(), m_caption.GetSize()));
		return;
	}

	int	idx;
	CString	maps, captions, tmps;
	CMapStringToString	tabs;
	char*	map	= &m_axform->m_strR[m_form->vals[0]];
	char*	caption	= &m_axform->m_strR[m_form->vals[1]];
	for (int ii = 0; ii < (int)m_form->keys; ii++)
	{
		maps = map;
		map += strlen(map);
		map++;

		captions = caption;
		caption += strlen(caption);
		caption++;

		tabs.SetAt(maps.Mid(2), captions);
	}

	m_map.RemoveAll();
	m_caption.RemoveAll();
	for ( ; !text.IsEmpty(); )
	{
		idx = text.Find('|');
		if (idx != -1)
		{
			tmps = text.Left(idx);
			text = text.Mid(++idx);
		}
		else
		{
			tmps = text;
			text.Empty();
		}

		if (tabs.Lookup(tmps, captions))
		{
			maps.Format(_T("1:%s"), LPCTSTR(tmps));
			m_map.Add(maps);
			m_caption.Add(captions);
		}
	}
	m_start = m_index = 0;
	m_count = m_caption.GetSize();
}

void CfmUserTab::loadBitmap(CString theme)
{
	CString	path;
	CBitmap* pic;
	BITMAP	bm;

//	m_images.RemoveAll();
	for (int ii = 0; ii < nIMAGE; ii++)
	{
		switch (ii)
		{
		case iSL: path.Format(_T("%s/%s/l_s%s%s"), LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// select left
		case iSB: path.Format(_T("%s/%s/s%s%s"),   LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// select body
		case iSR: path.Format(_T("%s/%s/r_s%s%s"), LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// select right
		case iNL: path.Format(_T("%s/%s/l_%s%s"),  LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// normal left
		case iNB: path.Format(_T("%s/%s/%s%s"),    LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// normal body
		case iNR: path.Format(_T("%s/%s/r_%s%s"),  LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// normal right
		case iBL: path.Format(_T("%s/%s/l_b%s%s"), LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// bg left
		case iBB: path.Format(_T("%s/%s/b%s%s"),   LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// bg body
		case iBR: path.Format(_T("%s/%s/r_b%s%s"), LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// bg right
		}

		pic = m_axform->m_draw->SetBitmap((char *)path.operator LPCTSTR());
		if (pic)
		{
			m_images.SetAt(ii, pic);
			if (ii == iBL || ii == iBR)	// bg left, bg right
			{
				pic->GetBitmap(&bm);
				if (ii == iBL)
					m_leftW = bm.bmWidth;
				else
					m_rightW = bm.bmWidth;
			}
		}
	}
}

void CfmUserTab::loadABitmap()	// load auxiliary image (scroll, more)
{
	CString	path;
	BITMAP	bm;
	CBitmap* pic = (CBitmap*)0;

	path.Format(_T("%s/%s/%s"), m_axform->m_root.GetString(), IMAGEDIR, ED_BMP);
	m_moreBM = new Cimage(m_axform->m_draw, path);
	if (m_moreBM && m_moreBM->GetHandle(PI_DEFAULT, pic))
	{
		pic->GetBitmap(&bm);
		m_moreW = bm.bmWidth;
	}
	else	m_moreW = 0;


	path.Format(_T("%s/%s/%s"), m_axform->m_root.GetString(), IMAGEDIR, SCR_BMP);
	m_scrBM = new Cimage(m_axform->m_draw, path, true);
}

void CfmUserTab::loadOrderBitmap()
{
	CString	path, text;
	CBitmap* pic;

	m_orders.RemoveAll();
	for (int id, ii = 0; ii < m_map.GetSize(); ii++)
	{
		text = m_map.GetAt(ii);
		if (text.GetLength() > 2)
			text = text.Mid(2);
		else
			text.Empty();

		id = atoi(text);
		for (int jj = 0; jj < 3; jj++)
		{
			switch (jj)
			{
			case 0:		// order# left
				path.Format(_T("%s/%s/l_Order%d%s"), LPCTSTR(m_axform->m_root), IMAGEDIR, id, &m_axform->m_strR[m_form->vals[2]]);
				break;
			case 1:		// order# body
				path.Format(_T("%s/%s/Order%d%s"),   LPCTSTR(m_axform->m_root), IMAGEDIR, id, &m_axform->m_strR[m_form->vals[2]]);
				break;
			case 2:		// order# right
				path.Format(_T("%s/%s/r_Order%d%s"), LPCTSTR(m_axform->m_root), IMAGEDIR, id, &m_axform->m_strR[m_form->vals[2]]);
				break;
			}

			pic = m_axform->m_draw->SetBitmap((char *)path.operator LPCTSTR());
			if (pic)
				m_orders.SetAt(id*3+jj, pic);
		}
	}
}

void CfmUserTab::drawTab(CDC* dc, CRect pRc, bool scroll)
{
	UINT	style;
	CRect	tRc, tRc1;
	CString	tmps;
	bool	focus_this;
	CSize	size;
	BITMAP	bm;
	CDC	memDC;
	CBitmap	*hbmp, *pbmp = NULL;
	int	id, cx, dx, ndc = dc->SaveDC();
	bool	quit = false;

	dc->SetBkMode(TRANSPARENT);
	memDC.CreateCompatibleDC(dc);
	tRc = pRc;

	dx = 0;
	if (m_images.Lookup(iBL, hbmp))			// background_left
	{
		hbmp->GetBitmap(&bm);
		if (!m_axform->m_oldVers)
		{
			pbmp = (CBitmap *)memDC.SelectObject(hbmp);
			::TransparentBlt(dc->m_hDC, tRc.left, tRc.top, bm.bmWidth, tRc.Height(), memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
			memDC.SelectObject(pbmp);
		}
		else
			transparentBlt(dc, hbmp, tRc.left, tRc.top, bm.bmWidth, tRc.Height());
		dx += m_leftW;
	}

	for (int ii = m_start; ii < m_last; ii++)
	{
		tmps = m_map.GetAt(ii);
		if (tmps[0] != '1')
			continue;

		if (tmps.GetLength() > 2)
			tmps = tmps.Mid(2);
		else
			tmps.Empty();
		id = atoi(tmps);
		tmps = m_caption.GetAt(ii);

		focus_this = false;
		if (ii == m_index && m_hasfocus && !m_bForm)
			focus_this = true;
		if (focus_this)
		{
			setFont(dc, 1);
			setTextColor(dc, m_ftRGB);
		}
		else
		{
			setFont(dc, -1);
			setTextColor(dc);
		}
		style = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		if (scroll && ii == m_last-1)
			style |= DT_LEFT;
		else
			style |= DT_CENTER;

		tRc.left = pRc.left + dx;
		if (!scroll || ii <= m_last-1)
			dx += getSize(dc, ii, focus_this);

		tRc1 = tRc;
		cx = tRc.left;
		size = dc->GetOutputTextExtent(tmps);
		size.cx++;
		size.cx = (size.cx/2)*2;

		// left
		if (((m_form->attr & FA_UPPER) && focus_this && m_orders.Lookup(id*3, hbmp)) || m_images.Lookup(focus_this ? iSL : iNL, hbmp))
		{
			if (!m_axform->m_oldVers)
				pbmp = (CBitmap *)memDC.SelectObject(hbmp);
			hbmp->GetBitmap(&bm);
			if (cx + bm.bmWidth >= pRc.right)
			{
				if (!m_axform->m_oldVers)
					::TransparentBlt(dc->m_hDC, cx, tRc.top, pRc.right-cx, tRc.Height(), memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
				else
					transparentBlt(dc, hbmp, cx, tRc.top, pRc.right-cx, tRc.Height());
				quit = true;
			}
			else
			{
				if (!m_axform->m_oldVers)
					::TransparentBlt(dc->m_hDC, cx, tRc.top, bm.bmWidth, tRc.Height(), memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
				else
					transparentBlt(dc, hbmp, cx, tRc.top, bm.bmWidth, tRc.Height());
			}
			cx += bm.bmWidth;
			if (!m_axform->m_oldVers)
				memDC.SelectObject(pbmp);
		}
		if (quit)
			break;

		// body
		tRc1.left = cx;
		tRc1.right = tRc1.left + size.cx;
		if (tRc1.right >= pRc.right)
		{
			size.cx = pRc.right - tRc1.left;
			tRc1.right = pRc.right;
			quit = true;
		}
		if (((m_form->attr & FA_UPPER) && focus_this && m_orders.Lookup(id*3+1, hbmp)) || m_images.Lookup(focus_this ? iSB : iNB, hbmp))
		{
			if (!m_axform->m_oldVers)
				pbmp = (CBitmap *)memDC.SelectObject(hbmp);
			hbmp->GetBitmap(&bm);
			for (int jj = 0; jj < size.cx; )
			{
				if (!m_axform->m_oldVers)
					::TransparentBlt(dc->m_hDC, cx, tRc.top, bm.bmWidth, tRc.Height(), memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
				else
					transparentBlt(dc, hbmp, cx, tRc.top, bm.bmWidth, tRc.Height());
				jj += bm.bmWidth;
				cx += bm.bmWidth;
			}
			if (!m_axform->m_oldVers)
				memDC.SelectObject(pbmp);
		}
		tRc1.top += 2;
		dc->DrawText(tmps, tRc1, style);
		if (quit)
			break;

		// right
		if (((m_form->attr & FA_UPPER) && focus_this && m_orders.Lookup(id*3+2, hbmp)) || m_images.Lookup(focus_this ? iSR : iNR, hbmp))
		{
			if (!m_axform->m_oldVers)
				pbmp = (CBitmap *)memDC.SelectObject(hbmp);
			hbmp->GetBitmap(&bm);
			if (cx + bm.bmWidth >= pRc.right)
			{
				if (!m_axform->m_oldVers)
					::TransparentBlt(dc->m_hDC, cx, tRc.top, pRc.right-cx, tRc.Height(), memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
				else
					transparentBlt(dc, hbmp, cx, tRc.top, pRc.right-cx, tRc.Height());
				quit = true;
			}
			else
			{
				if (!m_axform->m_oldVers)
					::TransparentBlt(dc->m_hDC, cx, tRc.top, bm.bmWidth, tRc.Height(), memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
				else
					transparentBlt(dc, hbmp, cx, tRc.top, bm.bmWidth, tRc.Height());
			}
			if (!m_axform->m_oldVers)
				memDC.SelectObject(pbmp);
		}
		if (quit)
			break;
	}

	if (dx && !quit)
	{
		tRc.left = pRc.left + dx;
		if (m_images.Lookup(iBB, hbmp))		// background_body
		{
			if (!m_axform->m_oldVers)
				pbmp = (CBitmap *)memDC.SelectObject(hbmp);
			hbmp->GetBitmap(&bm);
			for (cx = tRc.left ; cx < pRc.right-m_rightW; )
			{
				if (!m_axform->m_oldVers)
					::TransparentBlt(dc->m_hDC, cx, tRc.top, bm.bmWidth, tRc.Height(), memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
				else
					transparentBlt(dc, hbmp, cx, tRc.top, bm.bmWidth, tRc.Height());
				cx += bm.bmWidth;
			}
			if (!m_axform->m_oldVers)
				memDC.SelectObject(pbmp);
		}
		if (m_images.Lookup(iBR, hbmp))		// background_right
		{
			hbmp->GetBitmap(&bm);
			if (!m_axform->m_oldVers)
			{
				pbmp = (CBitmap *)memDC.SelectObject(hbmp);
				::TransparentBlt(dc->m_hDC, pRc.right-bm.bmWidth, tRc.top, bm.bmWidth, tRc.Height(), memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
				memDC.SelectObject(pbmp);
			}
			else
				transparentBlt(dc, hbmp, pRc.right-bm.bmWidth, tRc.top, bm.bmWidth, tRc.Height());
		}
	}

	memDC.DeleteDC();
	dc->RestoreDC(ndc);
}

void CfmUserTab::drawScroll(CDC* dc, CRect rect)
{
	CRect	rc  = rect;
	int	gap = rc.Width() / 2;
	CBrush*	brush = setBrush(dc, getRGB(cLGRAY));

	if (m_form->type == TAB_IMAGE)
	{
		CDC	memDC;
		CBitmap	*hbmp, *pbmp = NULL;
		int	idx, ndc = dc->SaveDC();

		memDC.CreateCompatibleDC(dc);
		rc.top = max(rc.top, rc.bottom - hSCR);
		m_scr  = rc;

		rc.right = rc.left + wSCR;
		if (m_scrBM->GetHandle(PI_DEFAULT, hbmp))
		{
			idx = (m_click == ckLeft) ? 2 : 0;
			pbmp = (CBitmap *)memDC.SelectObject(hbmp);
			dc->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, idx*wSCR, 0, wSCR, hSCR, SRCCOPY);
		//	::TransparentBlt(dc->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, idx*wSCR, 0, wSCR, hSCR, RGB(255, 0, 255));

			rc.left  = rc.right;
			rc.right = rc.left + wSCR;
			idx = (m_click == ckRight) ? 5 : 3;
			dc->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, idx*wSCR, 0, wSCR, hSCR, SRCCOPY);
		//	::TransparentBlt(dc->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, idx*wSCR, 0, wSCR, hSCR, RGB(255, 0, 255));
			memDC.SelectObject(pbmp);
		}
		memDC.DeleteDC();
		dc->RestoreDC(ndc);
	}
	else
	{
		if (!m_vertical)
			rc.top = rc.bottom - gap;
		m_scr = rc;

		rc.right = rc.left + gap + (m_vertical ? 0 : 1);
		if (m_click == ckLeft)
			dc->DrawFrameControl(rc, DFC_SCROLL, DFCS_SCROLLLEFT|DFCS_PUSHED);
		else
      			dc->DrawFrameControl(rc, DFC_SCROLL, DFCS_SCROLLLEFT);

		rc.left  = rc.right;
		rc.right = rc.left + gap;
		if (m_click == ckRight)
			dc->DrawFrameControl(rc, DFC_SCROLL, DFCS_SCROLLRIGHT|DFCS_PUSHED);
		else
			dc->DrawFrameControl(rc, DFC_SCROLL, DFCS_SCROLLRIGHT);
	}
}

void CfmUserTab::drawMore(CDC* dc, CRect rect)
{
	rect.top = max(rect.top, rect.bottom - hSCR);
	m_more = rect;
	if (m_moreBM)
		m_moreBM->DrawPicture(dc, rect, AL_STRETCH, m_axform->m_oldVers, (m_click == ckMore) ? PI_DOWN : PI_DEFAULT);
	else
		dc->DrawFrameControl(rect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
}

void CfmUserTab::drawVText(CDC* dc, CString tabs, CRect rect, UINT style)
{
	CRect	rc;
	CSize	size;
	float	dy;
	CStringArray	arrStr;

	getString(tabs, arrStr);
	size = dc->GetOutputTextExtent("A");

	rc = rect;
	dy = (float)(rect.top + TAB_EXTRA/2)-2;
	for (int ii = 0; ii < arrStr.GetSize(); ii++)
	{
		if (dy >= rect.bottom)
			break;

		rc.top = (int)dy;
		dy += (float)size.cy;
		rc.bottom = (int)dy;
		dc->DrawText(arrStr.GetAt(ii), &rc, style);
		dy += (float)(size.cy * 0.5);
	}
}

void CfmUserTab::drawNoneTab(CDC* dc, CRect tRc, CRect& tRc1, int cx, bool focus_this, UINT& style)
{
	CRect	rc;
	CPoint	pts[6];
	CBrush*	brush;
	int	jj;

	tRc1 = tRc;
	switch (m_form->align)
	{
	case AL_LEFT:
		style = DT_CENTER|DT_TOP|DT_NOPREFIX;
		tRc1.left++;	tRc1.top++;
		pts[0].x = tRc1.right;		pts[0].y = tRc1.top;
		pts[1].x = tRc1.left+cx;	pts[1].y = tRc1.top;
		pts[2].x = tRc1.left;		pts[2].y = tRc1.top+cx;
		pts[3].x = tRc1.left;		pts[3].y = tRc1.bottom-cx;
		pts[4].x = tRc1.left+cx;	pts[4].y = tRc1.bottom;
		pts[5].x = tRc1.right;		pts[5].y = tRc1.bottom;

		setPen(dc, getRGB(cGRAY));
		setBrush(dc, getRGB(cGRAY));
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);

		for (jj = 0; jj < 6; jj++)
		{
			pts[jj].x--;
			pts[jj].y--;
		}
		pts[0].x = tRc.right;
		pts[5].x = pts[0].x;

		if (focus_this)
		{
			setPen(dc, m_fpRGB);
			setBrush(dc, m_fpRGB);
		}
		else
		{
			setPen(dc);
			setBrush(dc);
		}
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);

		if (!focus_this)
		{
			rc = tRc;
			rc.left = rc.right-1;
			brush = setBrush(dc, getRGB(cYWHITE));
			dc->FillRect(&rc, brush);	// right
			brush = setBrush(dc);
			tRc1.right--;
		}
		break;

	case AL_RIGHT:
		style = DT_CENTER|DT_TOP|DT_NOPREFIX;
		tRc1.left++;	tRc1.top++;
		pts[0].x = tRc1.left;		pts[0].y = tRc1.top;
		pts[1].x = tRc1.left;		pts[1].y = tRc1.bottom;
		pts[2].x = tRc1.right-cx;	pts[2].y = tRc1.bottom;
		pts[3].x = tRc1.right;		pts[3].y = tRc1.bottom-cx;
		pts[4].x = tRc1.right;		pts[4].y = tRc1.top+cx;
		pts[5].x = tRc1.right-cx;	pts[5].y = tRc1.top;

		setPen(dc, getRGB(cGRAY));
		setBrush(dc, getRGB(cGRAY));
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);

		for (jj = 0; jj < 6; jj++)
		{
			pts[jj].x--;
			pts[jj].y--;
		}
		pts[0].x = tRc.left;
		pts[5].x = pts[0].x;

		if (focus_this)
		{
			setPen(dc, m_fpRGB);
			setBrush(dc, m_fpRGB);
		}
		else
		{
			setPen(dc);
			setBrush(dc);
		}
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);
		
		if (!focus_this)
		{
			rc = tRc;
			rc.right = rc.left+1;
			brush = setBrush(dc, getRGB(cYWHITE));
			dc->FillRect(&rc, brush);	// left
			brush = setBrush(dc);
			tRc1.left++;
		}
		break;

	case AL_BOTTOM:
		style = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		tRc1.left++;	tRc1.top++;
		pts[0].x = tRc1.left;		pts[0].y = tRc1.top;
		pts[1].x = tRc1.left;		pts[1].y = tRc1.bottom-cx;
		pts[2].x = tRc1.left+cx;	pts[2].y = tRc1.bottom;
		pts[3].x = tRc1.right-cx;	pts[3].y = tRc1.bottom;
		pts[4].x = tRc1.right;		pts[4].y = pts[1].y;
		pts[5].x = tRc1.right;		pts[5].y = tRc1.top;
		
		setPen(dc, getRGB(cGRAY));
		setBrush(dc, getRGB(cGRAY));
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);

		for (jj = 0; jj < 6; jj++)
		{
			pts[jj].x--;
			pts[jj].y--;
		}
		pts[0].y = tRc.top;
		pts[5].y = pts[0].y;

		if (focus_this)
		{
			setPen(dc, m_fpRGB);
			setBrush(dc, m_fpRGB);
		}
		else
		{
			setPen(dc);
			setBrush(dc);
		}
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);

		if (!focus_this)
		{
			rc = tRc;
			rc.bottom = rc.top+1;
			brush = setBrush(dc, getRGB(cYWHITE));
			dc->FillRect(&rc, brush);	// top
			brush = setBrush(dc);
			tRc1.top++;
		}
		break;

	case AL_TOP:
	default:
		style = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		tRc1.left++;	tRc1.top++;
		pts[0].x = tRc1.left;		pts[0].y = tRc1.bottom;
		pts[1].x = tRc1.left;		pts[1].y = tRc1.top+cx;
		pts[2].x = tRc1.left+cx;	pts[2].y = tRc1.top;
		pts[3].x = tRc1.right-cx;	pts[3].y = tRc1.top;
		pts[4].x = tRc1.right;		pts[4].y = pts[1].y;
		pts[5].x = tRc1.right;		pts[5].y = tRc1.bottom;
	
		setPen(dc, getRGB(cGRAY));
		setBrush(dc, getRGB(cGRAY));
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);

		for (jj = 0; jj < 6; jj++)
		{
			pts[jj].x--;
			pts[jj].y--;
		}
		pts[0].y = tRc.bottom;
		pts[5].y = pts[0].y;
		
		if (focus_this)
		{
			setPen(dc, m_fpRGB);
			brush = setBrush(dc, m_fpRGB);
		}
		else
		{
			setPen(dc);
			brush = setBrush(dc);
		}
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);

		if (!focus_this)
		{
			rc = tRc;
			rc.top = rc.bottom-1;
			brush = setBrush(dc, getRGB(cYWHITE));
			dc->FillRect(&rc, brush);	// bottom
			brush = setBrush(dc);
			tRc1.bottom--;
		}
		break;
	}
}

bool CfmUserTab::isScrolled(CDC* dc, int& index, int& width, int start)
{
	CString	infos;
	CRect	rect;
	bool	focus_this, scroll = false;
	int	ww, ii = (start < 0) ? m_start : start;

	index = width = 0;
	rect  = m_pRc;

	if (m_form->type == TAB_IMAGE)
	{
		ww = m_leftW + m_rightW;
		if (m_scroll)
			ww += (wSCR*2);
		if (m_form->attr & FA_DATE)
			ww += m_moreW;
	}
	else	ww = (int)(float)(rect.Height() * 2.0 / 3.0) * 2;

	for ( ; ii < (int)m_count; ii++)
	{
		infos = m_map.GetAt(ii);
		if (infos[0] != '1')
			continue;

		focus_this = false;
		if (ii == m_index && m_hasfocus && !m_bForm)
			focus_this = true;

		setFont(dc, focus_this ? 1 : -1);
		width += getSize(dc, ii, focus_this);
		if (m_vertical)
		{
			if (!index && (width > rect.Height() - (int)(rect.Width() * 0.5)))
				index = ii+1;
			if (width > rect.Height()
				|| start > 0 && (width > rect.Height() - (int)(rect.Width() * 0.5)))
			{
				scroll = true;
				break;
			}
		}
		else
		{
			if (!index && (width > rect.Width() - ww))
				index = ii+1;
			if (width > rect.Width() || (start > 0 && (width > rect.Width() - ww)))
			{
				scroll = true;
				break;
			}
		}
	}

	if (!scroll)
	{
		if (start >= 0)
		{
			ii = getFirstVisibleItem(0);
			if (ii != m_start)
				scroll = true;
		}
		index = getLastVisibleItem()+1;
	}
	index = (index == 0) ? m_count : index;
	return scroll;
}

int CfmUserTab::getSize(CDC* dc, int index, bool focus)
{
	CSize	size;
	INT_PTR	icount = 0;

	if (m_vertical)
	{
		CStringArray arrStr;
		size = dc->GetOutputTextExtent("A");
		getString(m_caption.GetAt(index), arrStr);

		icount = arrStr.GetSize() - 1;
		return (size.cy * arrStr.GetSize() + (int)(size.cy * 0.5 * icount) + TAB_EXTRA);
	}

	size = dc->GetOutputTextExtent(m_caption.GetAt(index));
	size.cx++;
	size.cx = (size.cx/2)*2;
	if (m_form->type == TAB_RECT)
		return (size.cx + TAB_EXTRA2);
	if (m_form->type == TAB_IMAGE)
	{
		BITMAP	bm;
		CBitmap* hbmp;
		int	dx = 0;

		if (focus)
		{
			if (m_form->attr & FA_UPPER)
			{
				CString	tmps;
				int	id;

				tmps = m_map.GetAt(index);
				if (tmps.GetLength() > 2)
					tmps = tmps.Mid(2);
				else
					tmps.Empty();
				id = atoi(tmps);

				if (m_orders.Lookup(id*3, hbmp))
				{
					hbmp->GetBitmap(&bm);
					dx += bm.bmWidth;
				}
				else if (m_images.Lookup(iSL, hbmp))
				{
					hbmp->GetBitmap(&bm);
					dx += bm.bmWidth;
				}

				if (m_orders.Lookup(id*3+2, hbmp))
				{
					hbmp->GetBitmap(&bm);
					dx += bm.bmWidth;
				}
				else if (m_images.Lookup(iSR, hbmp))
				{
					hbmp->GetBitmap(&bm);
					dx += bm.bmWidth;
				}
			}
			else
			{
				if (m_images.Lookup(iSL, hbmp))			// select left
				{
					hbmp->GetBitmap(&bm);
					dx += bm.bmWidth;
				}
				if (m_images.Lookup(iSR, hbmp))			// select right
				{
					hbmp->GetBitmap(&bm);
					dx += bm.bmWidth;
				}
			}
		}
		else
		{
			if (m_images.Lookup(iNL, hbmp))			// normal left
			{
				hbmp->GetBitmap(&bm);
				dx += bm.bmWidth;
			}
			if (m_images.Lookup(iNR, hbmp))			// normal right
			{
				hbmp->GetBitmap(&bm);
				dx += bm.bmWidth;
			}
		}
		return (size.cx + dx);
	}
	return (size.cx + TAB_EXTRA);
}

void CfmUserTab::getString(CString string, CStringArray& arrStr)
{
	arrStr.RemoveAll();
	for (int ii = 0; ii < string.GetLength(); ii++)
	{
		if (string[ii] & 0x80)
		{
			arrStr.Add(string.Mid(ii, 2));
			ii++;
		}
		else
			arrStr.Add(string.Mid(ii, 1));
	}
}

int CfmUserTab::getFirstVisibleItem(int start)
{
	CString	tmps;
	for ( ; start < m_count; start++)
	{
		tmps = m_map.GetAt(start);
		if (tmps[0] == '1')
			return start;
	}

	return -1;
}

int CfmUserTab::getLastVisibleItem()
{
	CString	item;
	int	index = 0;

	for (int ii = 0; ii < m_count; ii++)
	{
		item = m_map.GetAt(ii);
		if (item[0] == '1')
			index = ii;
	}

	return index;
}

void CfmUserTab::scroll(bool prev)
{
	CDC*	dc = getDC();
	int	index = 0, width = 0;
	CString	tmps;

	if (prev)
	{
		if (m_start > 0)
		{
			int	ii;
			for (ii = m_start-1 ; ii >= 0; ii--)
			{
				tmps = m_map.GetAt(ii);
				if (tmps[0] == '1')
					break;
			}
			m_start = (ii < 0) ? getFirstVisibleItem(0) : ii;
		}
		releaseDC(dc);
		return;
	}

	if (!isScrolled(dc, index, width))//, m_start))
	{
		releaseDC(dc);
		return;
	}

	m_start = getFirstVisibleItem(m_start+1);
	releaseDC(dc);
}

int CfmUserTab::getIndexByID(int ID)
{
	CString	str;

	for (int ii = 0; ii < m_map.GetSize(); ii++)
	{
		str = m_map.GetAt(ii);
		if (str.GetLength() > 2)
		{
			str = str.Mid(2);
			if (ID == atoi(str))
				return ii;
		}
	}

	return -1;
}

int CfmUserTab::getStartIndex(int index)
{
	if (!m_scroll)
		return m_start;

	if (index < m_start)
	{
		while (index < m_start)
			scroll();
	}
	else if (index == m_last-1)
		m_start++;
	else if (index > m_last-1)
	{
		int	width;
		bool	scroll = true;
		CDC*	dc = getDC();
		setFont(dc);
		while (index >= m_last-1 && scroll)
		{
			m_start++;
			scroll = isScrolled(dc, m_last, width);
		}
		releaseDC(dc);
	}

	return m_start;
}

bool CfmUserTab::isJustBeforeSelectItem(int index)
{
	CString	str;

	if (index >= 0)
	{
		for (++index; index < m_index; index++)
		{
			str = m_map.GetAt(index);
			if (str.GetAt(0) == '1')
				break;
		}
		if (index == m_index)
			return true;
	}

	return false;
}
