// fmBox.cpp : implementation file
//

#include "stdafx.h"
#include "fmBox.h"
#include "axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmBox

IMPLEMENT_DYNCREATE(CfmBox, CfmBase)

CfmBox::CfmBox() : m_width(0), m_height(0), m_ww(0), m_hh(0), m_key(0), m_type(0)
{
	EnableAutomation();
}

CfmBox::CfmBox(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	m_key  = m_form->size;
	m_type = m_form->type;

	m_width  = m_key;
	m_height = m_key;
	m_ww     = m_width;
	m_hh     = m_height;

	switch (m_type)
	{
	case BX_BORDER:
		if (m_key < 3 || m_rect.Width() < 6 || m_rect.Height() < 6)
			m_type = BX_LINE;
		break;
	case BX_LINE:
		if (m_key == 0)
			m_key++;
		break;
	}
}

CfmBox::~CfmBox()
{
}


void CfmBox::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmBox, CfmBase)
	//{{AFX_MSG_MAP(CfmBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmBox, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmBox)
	DISP_PROPERTY_EX(CfmBox, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmBox, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_FUNCTION(CfmBox, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmBox to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D012FFFE-4679-4802-B58A-DD3014A07186}
static const IID IID_IfmBox =
{ 0xd012fffe, 0x4679, 0x4802, { 0xb5, 0x8a, 0xdd, 0x30, 0x14, 0xa0, 0x71, 0x86 } };

BEGIN_INTERFACE_MAP(CfmBox, CfmBase)
	INTERFACE_PART(CfmBox, IID_IfmBox, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmBox message handlers

void CfmBox::_Refresh() 
{
	Refresh();
}

long CfmBox::_getPRgb() 
{
	return (long)m_pRGB;
}

void CfmBox::_setPRgb(long nNewValue) 
{
	CfmBase::SetBkColor(nNewValue);
}

BOOL CfmBox::_getVisible() 
{
	return ((m_form->properties & PR_VISIBLE) ? TRUE : FALSE);
}

void CfmBox::_setVisible(BOOL bNewValue) 
{
	SetVisible(bNewValue ? true : false);
}

void CfmBox::Adjust(CPoint pt, float hR, float vR)
{
	m_width  = (int)(m_key * hR);
	m_height = (int)(m_key * vR);

	CfmBase::Adjust(pt, hR, vR);
}

void CfmBox::Resize(float hR, float vR, bool font, bool redraw)
{
	float	ww = m_width  * hR;
	float	hh = m_height * vR;

	m_ww = (int)ww;
	m_hh = (int)hh;
	if ((ww - (double)m_ww) > 0.5)
		m_ww++;
	if ((hh - (double)m_hh) > 0.5)
		m_hh++;

	CfmBase::Resize(hR, vR, font, redraw);
}

void CfmBox::Reload()
{
	CfmBase::Reload();

	m_key  = m_form->size;
	m_type = m_form->type;

	m_width  = m_key;
	m_height = m_key;
	m_ww     = m_width;
	m_hh     = m_height;

	switch (m_type)
	{
	case BX_BORDER:
		if (m_key < 3 || m_rect.Width() < 6 || m_rect.Height() < 6)
			m_type = BX_LINE;
		break;
	case BX_LINE:
		if (m_key == 0)
			m_key++;
		break;
	}
}

void CfmBox::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	int	ndc = dc->SaveDC();
if (!strcmp("BOX6", (char *)m_form->name))
	ndc = ndc;
	if (m_type == BX_BORDER)
	{
		if (!(m_ww < 3 || m_hh < 3 || m_pRc.Width() < 6 || m_pRc.Height() < 6))
		{
			draw3DBox(dc);
			dc->RestoreDC(ndc);
			return;
		}
	}

	CRect	rc = m_pRc;
	int	penw = max(1, min(m_ww, m_hh));
	if (m_type == BX_2CLR)
	{
		rc.SetRect(m_pRc.left, m_pRc.top, m_pRc.left+penw, m_pRc.bottom);	// left
		dc->FillSolidRect(rc, m_tRGB);

		rc.SetRect(m_pRc.right-penw, m_pRc.top, m_pRc.right, m_pRc.bottom);	// right
		dc->FillSolidRect(rc, m_tRGB);

		rc.SetRect(m_pRc.left, m_pRc.top, m_pRc.right, m_pRc.top+penw);		// top
		dc->FillSolidRect(rc, m_pRGB);

		rc.SetRect(m_pRc.left, m_pRc.bottom-penw, m_pRc.right, m_pRc.bottom);	// bottom
		dc->FillSolidRect(rc, m_pRGB);
	}
	else
	{
		penw /= 2;
		setPen(dc, penw);
		dc->SelectStockObject(NULL_BRUSH);
		rc.left   += penw;
		rc.top    += penw;
		rc.right  -= penw;
		rc.bottom -= penw;
		if (rc.left == rc.right)
			rc.left--;
		if (rc.top == rc.bottom)
			rc.top--;
		dc->Rectangle(rc);
	}
	dc->RestoreDC(ndc);
}

void CfmBox::SetVisible(bool visible, int col)
{
	bool	isvisible = (m_form->properties & PR_VISIBLE) ? true : false;

	if (visible)
		m_form->properties |= PR_VISIBLE;
	else
		m_form->properties &= ~PR_VISIBLE;

	if (isvisible != visible)
	{
		CRect	rc = m_pRc;
		rc.InflateRect(m_ww, m_hh);
		invalidateRect(&rc, true, true);
	}
}

void CfmBox::Refresh()
{
	if (m_form->properties & PR_VISIBLE)
	{
		CRect	rc = m_pRc;
		rc.InflateRect(m_ww, m_hh);
		invalidateRect(&rc, true, true);
	}
}

void CfmBox::draw3DBox(CDC* dc)
{
	CPoint	pts[6];
	CBrush*	brush = (CBrush*)0;
	CRect	r  = m_pRc;
	int	gap = 1;

	pts[0].x = r.right-gap;		pts[0].y = r.top;
	pts[1].x = r.left;		pts[1].y = r.top;
	pts[2].x = r.left;		pts[2].y = r.bottom-gap;
	pts[3].x = r.left+gap;		pts[3].y = pts[2].y;
	pts[4].x = pts[3].x;		pts[4].y = r.top+gap;
	pts[5].x = pts[0].x;		pts[5].y = pts[4].y;
	setPen(dc, getRGB(cWHITE));
	brush = setBrush(dc, getRGB(cWHITE));
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0] = pts[5];
	pts[1] = pts[4];
	pts[2] = pts[3];
	pts[3].x = r.left+m_ww-gap;	pts[3].y = pts[2].y;
	pts[4].x = pts[3].x;		pts[4].y = r.top+m_hh-gap;
	pts[5].x = pts[0].x;		pts[5].y = pts[4].y;
	setPen(dc);
	brush = setBrush(dc);
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0].x = r.right-m_ww+gap;	pts[0].y = pts[5].y;
	pts[1].x = pts[0].x;		pts[1].y = r.bottom-m_hh+gap;
	pts[2].x = r.left+m_ww-gap;	pts[2].y = pts[1].y;
	pts[3].x = pts[2].x;		pts[3].y = r.bottom-gap;
	pts[4].x = r.right-gap;		pts[4].y = pts[3].y;
	pts[5].x = pts[4].x;		pts[5].y = pts[0].y;
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0].x = r.right;		pts[0].y = r.top;
	pts[1].x = pts[0].x;		pts[1].y = r.bottom;
	pts[2].x = r.left;		pts[2].y = pts[1].y;
	pts[3].x = r.left;		pts[3].y = r.bottom;
	pts[4].x = r.right;		pts[4].y = r.bottom;
	pts[5].x = r.right;		pts[5].y = r.top;
	setPen(dc, getRGB(cGRAY));
	brush = setBrush(dc, getRGB(cGRAY));
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0].x = r.right-m_ww+gap;	pts[0].y = r.top+m_hh-gap;
	pts[1].x = r.left+m_ww-gap;	pts[1].y = pts[0].y;
	pts[2].x = pts[1].x;		pts[2].y = r.bottom-m_hh+gap;
	pts[3].x = r.left+m_ww-gap;	pts[3].y = pts[2].y;
	pts[4].x = pts[3].x;		pts[4].y = r.top+m_hh-gap;
	pts[5].x = pts[0].x;		pts[5].y = pts[4].y;
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0].x = r.right-m_ww+gap;
	pts[1].x = pts[0].x;		pts[1].y = r.bottom-m_hh+gap;
	pts[2].x = r.left+m_ww-gap;	pts[2].y = pts[1].y;
	pts[3].x = pts[2].x;		pts[3].y = r.bottom-m_hh+gap;
	pts[4].x = r.right-m_ww+gap;	pts[4].y = pts[3].y;
	pts[5].x = pts[4].x;		pts[5].y = pts[0].y;
	setPen(dc, getRGB(cWHITE));
	brush = setBrush(dc, getRGB(cWHITE));
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);
}
