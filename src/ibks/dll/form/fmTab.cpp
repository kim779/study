// fmTab.cpp : implementation file
//

#include "stdafx.h"
#include "fmTab.h"
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
// CfmTab

IMPLEMENT_DYNCREATE(CfmTab, CfmBase)

CfmTab::CfmTab()
{
	EnableAutomation();
}

CfmTab::CfmTab(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	m_focus = false;
	m_count = m_form->keys;
	m_index = m_form->checked;

	m_tabH   = m_otabH = m_form->size;
	m_start  = 0;
	m_last   = m_count;
	m_scroll = false;
	m_click  = ckNone;
	m_scr.SetRectEmpty();
	m_cell.SetRectEmpty();
	m_pages.RemoveAll();
	m_scrBM  = NULL;

	if (m_count <= 0)
		return;

	char*	caption	= &m_axform->m_strR[m_form->vals[1]];
	for (int ii = 0; ii < m_count; ii++)
	{
		m_caption.Add(caption);
		caption += strlen(caption);
		caption++;
	}
	m_count = min(m_count, m_caption.GetSize());

	if (m_axform->m_drawOnly && m_form->type == TAB_IMAGE)
		m_form->type = TAB_BORDER;
	m_images.RemoveAll();
	SetEnum(_T(""), 0, 100);
	loadABitmap();

	int	last, value;
	CDC*	dc = getDC();
	setFont(dc);
	m_scroll = isScrolled(dc, last, value, 0);
	releaseDC(dc);

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

	if (m_form->vals[0] != NOVALUE)
	{
		struct	_pageR* pageR = &m_axform->m_pageR[m_form->vals[0]];
		for (int ii = 0; ii < m_count; ii++, pageR++)
		{
			CMapWordToPtr* page = new CMapWordToPtr;
			page->RemoveAll();
			if (pageR->formN != 0 && pageR->formR != NOVALUE)
			{
				value = pageR->formR;
				for (int jj = 0; jj < pageR->formN; jj++, value++)
				{
					page->SetAt((WORD)m_axform->m_valueR[value], NULL);
					m_axform->SetTabPage(m_axform->m_valueR[value], this);
				}
			}
			m_pages.Add(page);
		}
	}
	m_move  = mvNone;
	m_imove = -1;
}

CfmTab::~CfmTab()
{
	m_caption.RemoveAll();
	for (int ii = 0; ii < m_pages.GetSize(); ii++)
	{
		CMapWordToPtr* page = (CMapWordToPtr *)m_pages.GetAt(ii);
		page->RemoveAll();
		delete page;
	}
	m_pages.RemoveAll();
	m_images.RemoveAll();
	if (m_scrBM)
		delete m_scrBM;
}


void CfmTab::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmTab, CfmBase)
	//{{AFX_MSG_MAP(CfmTab)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmTab, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmTab)
	DISP_PROPERTY_EX(CfmTab, "Text", _getText, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CfmTab, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmTab, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CfmTab, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmTab, "Index", _getIndex, _setIndex, VT_I4)
	DISP_PROPERTY_EX(CfmTab, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_FUNCTION(CfmTab, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmTab, "Edit", _Edit, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmTab, "GetText", _GetText, VT_BSTR, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmTab to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {CF4B302F-18B1-4F8D-94C8-479F3C9721EC}
static const IID IID_IfmTab =
{ 0xcf4b302f, 0x18b1, 0x4f8d, { 0x94, 0xc8, 0x47, 0x9f, 0x3c, 0x97, 0x21, 0xec } };

BEGIN_INTERFACE_MAP(CfmTab, CfmBase)
	INTERFACE_PART(CfmTab, IID_IfmTab, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmTab message handlers

void CfmTab::_Refresh() 
{
	Refresh();
}

void CfmTab::_Edit() 
{
	return;
	if (m_form->attr & FA_DRAG)
		m_axform->DoSomething(doMODAL, this);
}

BSTR CfmTab::_GetText(long index) 
{
	CString strResult;

	strResult = (index >= 0 && index < m_caption.GetSize()) ? m_caption.GetAt(index) : _T("");
	return strResult.AllocSysString();
}

BSTR CfmTab::_getText() 
{
	CString strResult = _T("");

	ReadData(strResult);
	return strResult.AllocSysString();
}

long CfmTab::_getPRgb() 
{
	return m_pRGB;
}

void CfmTab::_setPRgb(long nNewValue) 
{
	CfmBase::SetBkColor(nNewValue);
}

long CfmTab::_getTRgb() 
{
	return m_tRGB;
}

void CfmTab::_setTRgb(long nNewValue) 
{
	CfmBase::SetFgColor(nNewValue);
}

BOOL CfmTab::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmTab::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

long CfmTab::_getIndex() 
{
	return GetCurSel();
}

void CfmTab::_setIndex(long nNewValue) 
{
	SetCurSel(nNewValue);
}

BOOL CfmTab::_getEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CfmTab::_setEnable(BOOL bNewValue) 
{
	if (bNewValue)
		m_form->properties |= PR_ENABLE;
	else
		m_form->properties &= ~PR_ENABLE;
}

void CfmTab::Adjust(CPoint pt, float hR, float vR)
{
	float tmpF = m_otabH * vR;
	m_otabH = (int)tmpF;
	if ( (tmpF - (double)m_otabH) > 0.5)
		m_otabH++;

	CfmBase::Adjust(pt, hR, vR);
}

void CfmTab::Resize(float hR, float vR, bool font, bool redraw)
{
	m_tabH = (int)(m_otabH * vR);
	m_scr.SetRectEmpty();
	m_cell.SetRectEmpty();

	CfmBase::Resize(hR, vR, font, redraw);
	
	int	last, width;
	CDC*	dc = getDC();
	setFont(dc);
	m_scroll = isScrolled(dc, last, width, 0);
	releaseDC(dc);
}

void CfmTab::Reload()
{
	CfmBase::Reload();

	m_tabH = m_otabH = m_form->size;
	m_scr.SetRectEmpty();
	m_cell.SetRectEmpty();
}

int CfmTab::IsPointInRect(CPoint pt)
{
	if (!m_axform->IsFormVisible(this))
		return P_OUT;

	CRect	tRc = m_pRc;
	tRc.bottom = tRc.top + m_tabH;
	if ((m_form->properties & PR_VISIBLE) && (m_form->properties & PR_ENABLE)
		&& tRc.PtInRect(pt))
		return P_IN;

	return P_OUT;
}

void CfmTab::SetFont(int point, int style, CString fonts, bool resize)
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
			tRc.bottom = tRc.top + m_tabH;
			tRc.InflateRect(2, 2);
			invalidateRect(tRc, true);
		}
	}
}

void CfmTab::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE) || m_count == 0)
		return;

	UINT	style;
	CRect	tRc, tRc1, pRc, rc;
	CString	tmpS, cells;
	int	jj,  cx = 2;
	int	dx;
	CPoint	pts[12];
	CBrush*	brush;
	bool	focus_this, scroll;
	int	ndc = dc->SaveDC();

	dx = m_pRc.Width() / m_count;
	if (dx < (cx+1)*2)
		return;

	pRc = m_pRc;
	pRc.bottom = pRc.top + m_tabH;
	scroll = isScrolled(dc, m_last, dx, m_start);
	if (m_scroll)
		pRc.right -= (int)(float)(m_tabH * 2.0 / 3.0) * 2;
	else
	{
		pRc.right = pRc.left + dx;
		m_last = m_count;
	}

	dx = pRc.left;
	switch (m_form->type)
	{
	case TAB_IMAGE:
		drawTab(dc, CRect(pRc.left, pRc.top, m_pRc.right, pRc.bottom), scroll);
		if (m_scroll)
		{
			tRc = pRc;
			tRc.left = tRc.right - wSCR*2;
			pRc.right = tRc.left;
			drawScroll(dc, tRc);
		}
		return;
	case TAB_BORDER:
	case TAB_OBLIQUE:
		dx++;
	default:
		break;
	}
	m_cell.SetRectEmpty();
	for (int ii = m_start; ii < m_last; ii++)
	{
		tmpS = m_caption.GetAt(ii);
		tRc  = pRc;

		tRc.left = dx;
		focus_this = false;
		if (ii == m_index)
			focus_this = true;
		setFont(dc, focus_this ? 1 : -1);

		if (!scroll || ii < m_last-1)
		{
			dx += getSize(dc, ii);
			tRc.right = dx;
		}

		if (!focus_this)
		{
			switch (m_form->type)
			{
			case TAB_BORDER:
				jj = 2;	break;
			case TAB_OBLIQUE:
				jj = 3; break;
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

		setTextColor(dc, focus_this ? m_ftRGB : m_tRGB);
		setPaintColor(dc, focus_this ? m_fpRGB : m_pRGB);
		style = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		switch (m_form->type)
		{
		case TAB_NONE:
			if (!scroll || ii < m_last-1)
				tRc.right--;
			tRc.bottom--;
			tRc1 = tRc;
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

			setPen(dc, focus_this ? m_fpRGB : m_pRGB);
			setBrush(dc, focus_this ? m_fpRGB : m_pRGB);
			dc->SetPolyFillMode(ALTERNATE);
			dc->Polygon(pts, 6);

			if (!focus_this)	// bottom
			{
				rc = tRc;
				rc.top = rc.bottom;
				rc.bottom++;
				rc.right++;
				brush = setBrush(dc, getRGB(cYWHITE));
				dc->FillRect(&rc, brush);
				brush = setBrush(dc);
				tRc1.bottom--;
			}
			break;

		case TAB_OBLIQUE:
			tRc1 = tRc;
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

				pts[0].x = tRc.left;	pts[0].y = tRc.bottom-2;
				pts[1].x = tRc.right-1;	pts[1].y = tRc.bottom-2;
				pts[2].x = tRc.right-1;	pts[2].y = tRc.bottom;
				pts[3].x = tRc.left;	pts[3].y = tRc.bottom;
				setPen(dc, getRGB(cYWHITE));
				setBrush(dc, getRGB(cYWHITE));
				dc->Polygon(pts, 4);

				pts[2].y-=2;
				pts[3].y-=2;
				setPen(dc, getRGB(cBOTTOM));
				setBrush(dc, getRGB(cBOTTOM));
				dc->Polygon(pts, 4);

				tRc1.DeflateRect(2, 2, 2, 2);
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

			pts[0].x = tRc.left;	pts[0].y = tRc.bottom-1;
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

			tRc1.DeflateRect(2, 2, 1, 0);
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
			break;

		case TAB_BORDER:
		default:
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

		tRc1.DeflateRect(2, 0);
		dc->DrawText(tmpS, tRc1, style);
	}
	
	if (!m_cell.IsRectEmpty())
	{
		setPen(dc, m_fpRGB);
		brush = setBrush(dc, m_fpRGB);
		dc->SetPolyFillMode(ALTERNATE);
		pts[0].x = m_cell.left;			pts[0].y = m_cell.bottom;
		pts[1].x = m_cell.left;			pts[1].y = m_cell.top;
		if (m_scroll && m_index == m_last-1 && scroll)
		{
			pts[2].x = m_cell.right;	pts[2].y = m_cell.top;
			pts[3].x = m_cell.right;	pts[3].y = m_cell.bottom;
		}
		else
		{
			pts[2].x = m_cell.right-(m_cell.Height()/2);
			pts[2].y = m_cell.top;
			pts[3].x = pts[2].x + m_cell.Height();
			pts[3].y = m_cell.bottom;
		}
		m_cell.right = pts[3].x;
		dc->Polygon(pts, 4);

		setPen(dc, getRGB(cBLACK));
		brush = setBrush(dc, getRGB(cBLACK));
		dc->SetPolyFillMode(ALTERNATE);
		if (m_scroll && m_index == m_last-1 && scroll)
		{
			pts[3] = pts[1];
			dc->Polygon(pts, 4);
		}
		else
		{
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
		if (m_scroll && m_index == m_last-1 && scroll)
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

		setFont(dc, 1);
		setTextColor(dc, m_ftRGB);
		setPaintColor(dc, m_fpRGB);
		cells = _T(" ") + cells;
	
		tRc1 = m_cell;
		tRc1.DeflateRect(3, 0);
		tRc1.top+=2;
       	 	dc->DrawText(cells, tRc1, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);
	}

	if (m_scroll)
	{
		tRc = pRc;
		tRc.bottom--;
		tRc.left  = pRc.right;
		tRc.right = m_pRc.right;
		drawScroll(dc, tRc);
	}

	tRc = m_pRc;
	tRc.top = pRc.bottom-1;
	drawPage(dc, pRc, tRc, (m_form->type == TAB_NONE) ? false : true);
	dc->RestoreDC(ndc);
}

void CfmTab::OnLButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	if (m_count == 0)
		return;

	CRect	tRc;
	int	gap;

	if (down)
	{
		int	dx;

		m_axform->m_cursor = pt;
		m_click = ckNone;
		if (m_scr.PtInRect(pt))
		{
			CRect	tRc1;

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
				tRc = m_pRc;
				tRc.bottom = tRc.top + m_tabH;
				tRc.InflateRect(2, 2);
				invalidateRect(tRc, true);
			}
		}
		else
		{
			CRgn	rgn;
			CPoint	pts[5];

			dx = m_pRc.left;
			for (int ii = m_start; ii < m_last; ii++)
			{
				tRc = m_pRc;
				tRc.bottom = tRc.top + m_tabH;
				if (m_form->type == TAB_BORDER || m_form->type == TAB_OBLIQUE)
					tRc.DeflateRect(1, 2, 1, 0);

				tRc.left = dx;
				if (!m_scroll || ii < m_last-1 || (m_scroll && m_last == m_count))
				{
					CDC*	dc = getDC();
					setFont(dc, (ii == m_index) ? 1 : -1);
					dx += getSize(dc, ii);
					tRc.right = dx;
					releaseDC(dc);
				}

				if (m_form->type == TAB_RECT)
				{
					pts[0].x = tRc.left;		pts[0].y = tRc.top;
					pts[1].x = tRc.left;		pts[1].y = tRc.bottom;
					pts[2].x = tRc.right;
					pts[2].y = tRc.bottom;
					pts[3].x = tRc.right;
					pts[3].y = tRc.top + (tRc.Height()/2);
					pts[4].x = tRc.right - (tRc.Height()/2);
					pts[4].y = tRc.top;
					rgn.CreatePolygonRgn(pts, 5, ALTERNATE);
				}

				if ((m_form->type == TAB_RECT && rgn.PtInRegion(pt))
					|| (m_form->type != TAB_RECT && tRc.PtInRect(pt)))
				{
					m_click = ckTab;
					result = ii;
					if (result != m_index)
					{
						m_index = result;
						tRc = m_pRc;
						tRc.bottom = tRc.top + m_tabH;
						tRc.InflateRect(2, 2);
						if (m_scroll)
							tRc.right = m_scr.left;
						invalidateRect(tRc, true);

						tRc = m_pRc;
						tRc.top += m_tabH;
						invalidateRect(tRc, false);
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

	default:
		m_click = ckNone;
		break;
	}
}

int CfmTab::OnDrag(CPoint pt, int& result)
{
	CRgn	rgn;
	CRect	tRc;
	CPoint	pts[5];
	int	rc, dx = m_pRc.left;

	rc = CfmBase::OnDrag(pt, result);
	if (result != RC_IN)
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

	for (int ii = m_start; ii < m_last; ii++)
	{
		tRc = m_pRc;
		tRc.bottom = tRc.top + m_tabH;
		if (m_form->type == TAB_BORDER || m_form->type == TAB_OBLIQUE)
			tRc.DeflateRect(1, 2, 1, 0);
		tRc.left = dx;
		if (!m_scroll || ii < m_last-1 || (m_scroll && m_last == m_count))
		{
			CDC*	dc = getDC();
			setFont(dc, (ii == m_index) ? 1 : -1);
			dx += getSize(dc, ii);
			tRc.right = dx;
			releaseDC(dc);
		}
 
		if (m_form->type == TAB_RECT)
		{
			pts[0].x = tRc.left;		pts[0].y = tRc.top;
			pts[1].x = tRc.left;		pts[1].y = tRc.bottom;
			pts[2].x = tRc.right;		pts[2].y = tRc.bottom;
			pts[3].x = tRc.right;
			pts[3].y = tRc.top + (tRc.Height()/2);
			pts[4].x = tRc.right - (tRc.Height()/2);
			pts[4].y = tRc.top;
			rgn.CreatePolygonRgn(pts, 5, ALTERNATE);
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

	return rc;
}

bool CfmTab::OnDrop(CPoint pt, CString Data, CString& key)
{
	CRgn	rgn;
	CRect	tRc;
	CPoint	pts[5];
	int	dx = m_pRc.left;

	key = _T("");
	if (m_scroll && m_scr.PtInRect(pt))
		return false;

	for (int ii = m_start; ii < m_last; ii++)
	{
		tRc = m_pRc;
		tRc.bottom = tRc.top + m_tabH;
		if (m_form->type == TAB_BORDER || m_form->type == TAB_OBLIQUE)
			tRc.DeflateRect(1, 2, 1, 0);
		tRc.left   = dx;
		if (!m_scroll || ii < m_last-1 || (m_scroll && m_last == m_count))
		{
			CDC*	dc = getDC();
			setFont(dc, (ii == m_index) ? 1 : -1);
			dx += getSize(dc, ii);
			tRc.right = dx;
			releaseDC(dc);
		}

		if (m_form->type == TAB_RECT)
		{
			pts[0].x = tRc.left;		pts[0].y = tRc.top;
			pts[1].x = tRc.left;		pts[1].y = tRc.bottom;
			pts[2].x = tRc.right;		pts[2].y = tRc.bottom;
			pts[3].x = tRc.right;
			pts[3].y = tRc.top + (tRc.Height()/2);
			pts[4].x = tRc.right - (tRc.Height()/2);
			pts[4].y = tRc.top;
			rgn.CreatePolygonRgn(pts, 5, ALTERNATE);
		}

		if ((m_form->type == TAB_RECT && rgn.PtInRegion(pt)) || (m_form->type != TAB_RECT && tRc.PtInRect(pt)))
		{
			if (m_form->type == TAB_RECT)
				rgn.DeleteObject();
		//	key = m_map.GetAt(ii);
			key = m_caption.GetAt(ii);
			return true;
		}
	
		if (m_form->type == TAB_RECT)
			rgn.DeleteObject();
	}

	return false;
}

void CfmTab::SetFgColor(int rgb, int col, int row)
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

void CfmTab::SetBkColor(int rgb, int col, int row)
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

void CfmTab::ReadAll(CString& data, bool edit, bool head)
{
	data.Format("%d", m_index);
	for (int ii = 0; ii < m_count; ii++)
	{
		data += '|';
	//	data += m_map.GetAt(ii);
		data += m_caption.GetAt(ii);
	}
}

void CfmTab::ReadData(CString& data, bool edit, int size, int index, int row)
{
	data = _T("");
	if (m_form->attr & FA_DRAG)
	{
		index = (index < 0) ? m_index : index;
		if (index >= m_caption.GetSize())// || index >= m_map.GetSize())
			return;

	//	if (!edit)
	//		data = m_map.GetAt(index);
		data += m_caption.GetAt(index);
	}
	else
	{
	//	if (m_index < 0 || m_index > m_map.GetUpperBound())
		if (m_index < 0 || m_index > m_caption.GetUpperBound())
			return;

	//	data = m_map.GetAt(m_index);
		data = m_caption.GetAt(m_index);
	}
}

void CfmTab::WriteAll(CString str, bool redraw)
{
	if (!(m_form->attr & FA_DRAG))
		return;

	int offs = str.Find('|');
	if (offs == -1)
		return;

	CString	tmps;
	m_index = atoi(str.Left(offs));
	str = str.Mid(offs+1);

	m_caption.RemoveAll();
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

		m_caption.Add(tmps);
	}
	m_count = m_caption.GetSize();

	if (redraw)
		invalidateRect(&m_pRc, true);
}

void CfmTab::WriteData(CString data, bool redraw, int index, int row)
{
	if (!(m_form->attr & FA_DRAG))
		return;

	index = (index < 0) ? m_index : index;
	if (index >= m_caption.GetSize())// || index >= m_map.GetSize())
		return;

//	m_map.SetAt(index, data.Left(L_MAPN));
//	m_caption.SetAt(index, data.Mid(L_MAPN));
	m_caption.SetAt(index, data);
	if (redraw)
		invalidateRect(&m_pRc, true);
}

void CfmTab::SetCurSel(int index, bool redraw)
{
	if (index < 0 || index >= m_count)
		return;

	int	idx = m_index;
	m_index = index;
	if (redraw && idx != m_index)
		invalidateRect(&m_pRc, true);
}

int CfmTab::GetCurSel()
{
	return m_index;
}

int CfmTab::GetSize()
{
	return m_count;
}

bool CfmTab::IsFormVisible(int index)
{
	if (m_axform->IsFormVisible(this) && m_index >= 0 && m_index < m_pages.GetSize())
	{
		void*	ptr;
		CMapWordToPtr* page = (CMapWordToPtr *)m_pages.GetAt(m_index);
		if (page->Lookup(index, ptr))
			return true;
	}
	return false;
}

void CfmTab::GetEnum(CString& text, int& value, int func)
{
	if (func == 101)
		text = (value >= 0 && value < m_caption.GetSize()) ? m_caption.GetAt(value) : _T("");
}

void CfmTab::SetEnum(CString text, int value, int func)
{
	if (func == 100 && m_form->type == TAB_IMAGE)
	{
		char	wb[32] = { 0, };

		wb[0] = '\0';
		text = &m_axform->m_strR[m_form->vals[2]];
		if (!(m_form->attr & FA_SKIP))
		{
			text = &m_axform->m_strR[m_form->vals[2]];
			text.Format(_T("%s/%s/%s"), LPCTSTR(m_axform->m_root), TABDIR, PALETTE);
			GetPrivateProfileString("General", "Palette", "", wb, sizeof(wb), text);
		}
		loadBitmap(wb);
		if (value)
			invalidateRect(&m_pRc, true);
	}
}

void CfmTab::Refresh()
{
	CRect	tRc = m_pRc;
	tRc.InflateRect(2, 2);
	invalidateRect(tRc, true);
}

void CfmTab::drawTab(CDC* dc, CRect pRc, bool scroll)
{
	UINT	style;
	CRect	tRc, tRc1;
	CString	tmps;
	bool	focus_this;
	CSize	size;
	BITMAP	bm;
	CDC	memDC;
	CBitmap	*hbmp, *pbmp = NULL;
	int	cx, dx, ndc = dc->SaveDC();
	bool	quit = false;

	dc->SetBkMode(TRANSPARENT);
	memDC.CreateCompatibleDC(dc);
	tRc  = pRc;

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
		tmps = m_caption.GetAt(ii);
		tRc.left = pRc.left + dx;

		focus_this = false;
		if (ii == m_index)
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

		if (!scroll || ii < m_last-1)
			dx += getSize(dc, ii, focus_this);

		tRc1 = tRc;
		cx = tRc.left;
		size = dc->GetOutputTextExtent(tmps);
		size.cx = ((size.cx +1)/2)*2;

		// left
		if (m_images.Lookup(focus_this ? iSL : iNL, hbmp))
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
		if (m_images.Lookup(focus_this ? iSB : iNB, hbmp))
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
		if (m_images.Lookup(focus_this ? iSR : iNR, hbmp))
		{
			if (!m_axform->m_oldVers)
				pbmp = (CBitmap *)memDC.SelectObject(hbmp);
			hbmp->GetBitmap(&bm);
			if (cx + bm.bmWidth >= pRc.right)
			{
				if (!m_axform->m_oldVers)
					::TransparentBlt(dc->m_hDC, cx, tRc.top, pRc.right-cx, tRc.Height(), memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
				else
					transparentBlt(dc, hbmp, cx, tRc.top, tRc.right-cx, tRc.Height());
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
	
	tRc = m_pRc;
	tRc.top = pRc.bottom-1;
	drawPage(dc, pRc, tRc, false);

	dc->RestoreDC(ndc);
}

void CfmTab::drawPage(CDC* dc, CRect tabRc, CRect rect, bool left)
{
	CRect	rc;
	CPoint	pts[6];
	CBrush*	brush;
	int	gap = 1;

	rc = rect;
	rc.top++;
	if (m_form->type == TAB_RECT)
	{
		rc.top++;
		rect.top++;
	}
	brush = setBrush(dc, m_fpRGB);
	dc->FillRect(&rc, brush);

	if (m_form->type == TAB_RECT)
	{
		setPen(dc, getRGB(cBLACK));
		brush = setBrush(dc, getRGB(cBLACK));
	}
	else
	{
		setPen(dc, getRGB(cYWHITE));
		brush = setBrush(dc, getRGB(cYWHITE));
	}
	dc->SetPolyFillMode(ALTERNATE);
	if (left)
	{
		pts[0].x = rect.left;		pts[0].y = rect.top;	// left
		pts[1].x = rect.left;		pts[1].y = rect.top;
		pts[2].x = rect.left;		pts[2].y = rect.bottom-gap;
		pts[3].x = rect.left;		pts[3].y = pts[2].y;
		dc->Polygon(pts, 4);
	}

	pts[0].x = rect.right-gap;	pts[0].y = rect.top;		// top
	pts[1].x = tabRc.right;		pts[1].y = rect.top;
	pts[2].x = pts[1].x;		pts[2].y = rect.top;
	pts[3].x = pts[0].x;		pts[3].y = rect.top;
	dc->Polygon(pts, 4);	

	pts[0].x = m_pRc.right-gap;	pts[0].y = rect.top;		// bottom_right
	pts[1].x = pts[0].x;		pts[1].y = rect.bottom-gap;
	pts[2].x = rect.left;		pts[2].y = pts[1].y;
	pts[3].x = rect.left;		pts[3].y = rect.bottom-gap;
	pts[4].x = m_pRc.right-gap;	pts[4].y = rect.bottom-gap;
	pts[5].x = m_pRc.right-gap;	pts[5].y = rect.top;
	setPen(dc, getRGB(cEDGE));
	brush = setBrush(dc, getRGB(cEDGE));
	dc->Polygon(pts, 6);
}

void CfmTab::drawScroll(CDC* dc, CRect rect)
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
		rc.top = rc.bottom - gap;
		m_scr  = rc;

		rc.right = rc.left + gap+1;
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

bool CfmTab::isScrolled(CDC* dc, int& index, int& width, int start)
{
	CRect	rect;
	bool	scroll = false;
	int	ww, ii = (start < 0) ? m_start : start;

	index = width = 0;
	rect  = m_pRc;

	if (m_form->type == TAB_IMAGE)
	{
		ww = m_leftW + m_rightW;
		if (m_scroll)
			ww += (wSCR*2);
	}
	else	ww = (int)(float)(rect.Height() * 2.0 / 3.0) * 2;

	for ( ; ii < m_count; ii++)
	{
		setFont(dc, (ii == m_index) ? 1 : -1);
		width += getSize(dc, ii);
		if (!index && (width > rect.Width() - ww))
			index = ii+1;
		if (width > rect.Width() || (start > 0 && (width > rect.Width() - ww)))
		{
			scroll = true;
			break;
		}
	}

	index = (index == 0) ? m_count : index;
	return scroll;
}

int CfmTab::getSize(CDC* dc, int index, bool focus)
{
	CSize	size;

	size = dc->GetOutputTextExtent(m_caption.GetAt(index));
	size.cx = ( (size.cx+1) /2)*2;
	if (m_form->type == TAB_RECT)
		return (size.cx + TAB_EXTRA2);
	if (m_form->type == TAB_IMAGE)
	{
		BITMAP	bm;
		CBitmap* hbmp;
		int	dx = 0;

		if (focus)
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

void CfmTab::scroll(bool prev)
{
	CDC*	dc  = getDC();
	int	index, width = 0;

	if (prev)
	{
		if (m_start > 0)
			m_start--;

		releaseDC(dc);
		return;
	}

	if (!isScrolled(dc, index, width, m_start))
	{
		releaseDC(dc);
		return;
	}

	m_start++;
	releaseDC(dc);
}

void CfmTab::loadBitmap(CString theme)
{
	CString	path;
	CBitmap* pic;
	BITMAP	bm = {};

//	m_images.RemoveAll();
	for (int ii = 0; ii < nIMAGE; ii++)
	{
		switch (ii)
		{
		case iSL: path.Format(_T("%s/%s/l_s%s%s"),LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// select left
		case iSB: path.Format(_T("%s/%s/s%s%s"),  LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// select body
		case iSR: path.Format(_T("%s/%s/r_s%s%s"),LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// select right
		case iNL: path.Format(_T("%s/%s/l_%s%s"), LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// normal left
		case iNB: path.Format(_T("%s/%s/%s%s"),   LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// normal body
		case iNR: path.Format(_T("%s/%s/r_%s%s"), LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// normal right
		case iBL: path.Format(_T("%s/%s/l_b%s%s"),LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// bg left
		case iBB: path.Format(_T("%s/%s/b%s%s"),  LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// bg body
		case iBR: path.Format(_T("%s/%s/r_b%s%s"),LPCTSTR(m_axform->m_root), IMAGEDIR, LPCTSTR(theme), &m_axform->m_strR[m_form->vals[2]]);	break;	// bg right
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

void CfmTab::loadABitmap()	// load auxiliary image (scroll, more)
{
	CString	path;
/*	BITMAP	bm;
	CBitmap* pic;

	path.Format("%s/%s/%s", m_axform->m_root, IMAGEDIR, ED_BMP);
	m_moreBM = new Cimage(m_axform->m_draw, path);
	if (m_moreBM && m_moreBM->GetHandle(PI_DEFAULT, pic))
	{
		pic->GetBitmap(&bm);
		m_moreW = bm.bmWidth;
	}
	else	m_moreW = 0;
*/

	path.Format(_T("%s/%s/%s"), m_axform->m_root.GetString(), IMAGEDIR, SCR_BMP);
	m_scrBM = new Cimage(m_axform->m_draw, path, true);
}