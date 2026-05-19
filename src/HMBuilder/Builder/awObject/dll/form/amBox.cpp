// fmBox.cpp : implementation file
//

#include "stdafx.h"
#include "amBox.h"
#include "amform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CamBox

IMPLEMENT_DYNCREATE(CamBox, CamBase)

CamBox::CamBox()
{
	EnableAutomation();
}

CamBox::CamBox(CAmForm* axform, struct _formR* formR)
	: CamBase(axform, formR)
{
	m_key  = m_form->size;
	m_type = m_form->type;

	m_width  = m_key;
	m_height = m_key;
	m_ww     = m_width;
	m_hh     = m_height;


	// mobile Fix
	m_type = BX_LINE;
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

CamBox::~CamBox()
{
}


void CamBox::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamBox, CamBase)
	//{{AFX_MSG_MAP(CamBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamBox, CamBase)
	//{{AFX_DISPATCH_MAP(CamBox)
	//DISP_PROPERTY_EX(CamBox, "PRgb", _getPRgb, _setPRgb, VT_I4)
	//DISP_PROPERTY_EX(CamBox, "Visible", _getVisible, _setVisible, VT_BOOL)
	//DISP_FUNCTION(CamBox, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmBox to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D012FFFE-4679-4802-B58A-DD3014A07186}
static const IID IID_IfmBox =
{ 0xd012fffe, 0x4679, 0x4802, { 0xb5, 0x8a, 0xdd, 0x30, 0x14, 0xa0, 0x71, 0x86 } };

BEGIN_INTERFACE_MAP(CamBox, CamBase)
	INTERFACE_PART(CamBox, IID_IfmBox, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamBox message handlers

void CamBox::_Refresh() 
{
	Refresh();
}

long CamBox::GetPRgb() 
{
	return (long)m_pRGB;
}

void CamBox::SetPRgb(long nNewValue) 
{
	CamBase::SetBkColor(nNewValue);
}

BOOL CamBox::GetVisible() 
{
	return ((m_form->properties & PR_VISIBLE) ? TRUE : FALSE);
}

void CamBox::SetVisible(BOOL bNewValue) 
{
	_SetVisible(bNewValue ? true : false);
}

void CamBox::Adjust(CPoint pt, float hR, float vR)
{
	m_width  = (int)(m_key * hR);
	m_height = (int)(m_key * vR);

	CamBase::Adjust(pt, hR, vR);
}

void CamBox::Resize(float hR, float vR, bool font, bool redraw)
{
	float	ww = m_width  * hR;
	float	hh = m_height * vR;

	m_ww = (int)ww;
	m_hh = (int)hh;
	if (ww - (float)m_ww > 0.5)
		m_ww++;
	if (hh - (float)m_hh > 0.5)
		m_hh++;

	CamBase::Resize(hR, vR, font, redraw);
}

void CamBox::Reload()
{
	CamBase::Reload();

	m_key  = m_form->size;
	m_type = m_form->type;

	m_width  = m_key;
	m_height = m_key;
	m_ww     = m_width;
	m_hh     = m_height;

	// mobile fix
	m_type = BX_LINE;
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

void CamBox::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	Graphics g(dc->GetSafeHdc());
	if (m_type == BX_BORDER)
	{
		if (!(m_ww < 3 || m_hh < 3 || m_pRc.Width() < 6 || m_pRc.Height() < 6))
		{
			draw3DBox(g);
			return;
		}
	}

	CRect	rc = m_pRc;
	int	penw = max(1, min(m_ww, m_hh));
	if (m_type == BX_2CLR)
	{
		rc.SetRect(m_pRc.left, m_pRc.top, m_pRc.left+penw, m_pRc.bottom);	// left
		//dc->FillSolidRect(rc, m_tRGB);
#ifdef _last
		g.FillRectangle(setgBrush(m_tRGB), RECTG(rc));
#else
		g.FillRectangle(setgBrush(_ARGB(255, GetRValue(m_tRGB), GetGValue(m_tRGB), GetBValue(m_tRGB))), RECTG(rc));
#endif
		
		rc.SetRect(m_pRc.right-penw, m_pRc.top, m_pRc.right, m_pRc.bottom);	// right
		//dc->FillSolidRect(rc, m_tRGB);
#ifdef _last
		g.FillRectangle(setgBrush(m_tRGB), RECTG(rc));
#else
		g.FillRectangle(setgBrush(_ARGB(255, GetRValue(m_tRGB), GetGValue(m_tRGB), GetBValue(m_tRGB))), RECTG(rc));
#endif
		
		rc.SetRect(m_pRc.left, m_pRc.top, m_pRc.right, m_pRc.top+penw);		// top
	//	dc->FillSolidRect(rc, m_pRGB);
#ifdef _last
		g.FillRectangle(setgBrush(m_pRGB), RECTG(rc));
#else
		g.FillRectangle(setgBrush(_ARGB(255, GetRValue(m_pRGB), GetGValue(m_pRGB), GetBValue(m_pRGB))), RECTG(rc));
#endif
		
		rc.SetRect(m_pRc.left, m_pRc.bottom-penw, m_pRc.right, m_pRc.bottom);	// bottom
		//dc->FillSolidRect(rc, m_pRGB);
#ifdef _last
		g.FillRectangle(setgBrush(m_pRGB), RECTG(rc));
#else
		g.FillRectangle(setgBrush(_ARGB(255, GetRValue(m_pRGB), GetGValue(m_pRGB), GetBValue(m_pRGB))), RECTG(rc));
#endif
	}
	else
	{
//		penw /= 2;
//		setPen(dc, penw);
//		dc->SelectStockObject(NULL_BRUSH);
//		rc.DeflateRect(penw, penw);
		
		g.DrawRectangle(setgPen(penw), RECTG(rc));
	}

	if (rc.left == rc.right)
		g.DrawLine(setgPen(penw), Point(rc.left, rc.top), Point(rc.left, rc.bottom));
	else if (rc.top == rc.bottom)
		g.DrawLine(setgPen(penw), Point(rc.left, rc.top), Point(rc.right, rc.bottom));

}

void CamBox::_SetVisible(bool visible, int col)
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

void CamBox::Refresh()
{
	if (m_form->properties & PR_VISIBLE)
	{
		CRect	rc = m_pRc;
		rc.InflateRect(m_ww, m_hh);
		invalidateRect(&rc, true, true);
	}
}

void CamBox::draw3DBox(Graphics& g)
{
	Point pts[6];
	CRect	r  = m_pRc;
	int	gap = 1;

	pts[0].X = r.right-gap;		pts[0].Y = r.top;
	pts[1].X = r.left;			pts[1].Y = r.top;
	pts[2].X = r.left;			pts[2].Y = r.bottom-gap;
	pts[3].X = r.left+gap;		pts[3].Y = pts[2].Y;
	pts[4].X = pts[3].X;		pts[4].Y = r.top+gap;
	pts[5].X = pts[0].X;		pts[5].Y = pts[4].Y;

	g.DrawPolygon(setgPen(clWHITE), pts, 6);
	g.FillPolygon(setgBrush(clWHITE), pts, 6, FillModeAlternate);


	pts[0] = pts[5];
	pts[1] = pts[4];
	pts[2] = pts[3];
	pts[3].X = r.left+m_ww-gap;	pts[3].Y = pts[2].Y;
	pts[4].X = pts[3].X;		pts[4].Y = r.top+m_hh-gap;
	pts[5].X = pts[0].X;		pts[5].Y = pts[4].Y;

	g.DrawPolygon(setgPen(), pts, 6);
	g.FillPolygon(setgBrush(), pts, 6, FillModeAlternate);


	pts[0].X = r.right-m_ww+gap;	pts[0].Y = pts[5].Y;
	pts[1].X = pts[0].X;			pts[1].Y = r.bottom-m_hh+gap;
	pts[2].X = r.left+m_ww-gap;		pts[2].Y = pts[1].Y;
	pts[3].X = pts[2].X;			pts[3].Y = r.bottom-gap;
	pts[4].X = r.right-gap;			pts[4].Y = pts[3].Y;
	pts[5].X = pts[4].X;			pts[5].Y = pts[0].Y;
	
	g.DrawPolygon(setgPen(), pts, 6);
	g.FillPolygon(setgBrush(), pts, 6, FillModeAlternate);


	pts[0].X = r.right;		pts[0].Y = r.top;
	pts[1].X = pts[0].X;	pts[1].Y = r.bottom;
	pts[2].X = r.left;		pts[2].Y = pts[1].Y;
	pts[3].X = r.left;		pts[3].Y = r.bottom;
	pts[4].X = r.right;		pts[4].Y = r.bottom;
	pts[5].X = r.right;		pts[5].Y = r.top;
	
	g.DrawPolygon(setgPen(clGRAY), pts, 6);
	g.FillPolygon(setgBrush(clGRAY), pts, 6, FillModeAlternate);

	pts[0].X = r.right-m_ww+gap;	pts[0].Y = r.top+m_hh-gap;
	pts[1].X = r.left+m_ww-gap;		pts[1].Y = pts[0].Y;
	pts[2].X = pts[1].X;			pts[2].Y = r.bottom-m_hh+gap;
	pts[3].X = r.left+m_ww-gap;		pts[3].Y = pts[2].Y;
	pts[4].X = pts[3].X;			pts[4].Y = r.top+m_hh-gap;
	pts[5].X = pts[0].X;			pts[5].Y = pts[4].Y;

	g.DrawPolygon(setgPen(clGRAY), pts, 6);
	g.FillPolygon(setgBrush(clGRAY), pts, 6, FillModeAlternate);

	pts[0].X = r.right-m_ww+gap;
	pts[1].X = pts[0].X;			pts[1].Y = r.bottom-m_hh+gap;
	pts[2].X = r.left+m_ww-gap;		pts[2].Y = pts[1].Y;
	pts[3].X = pts[2].X;			pts[3].Y = r.bottom-m_hh+gap;
	pts[4].X = r.right-m_ww+gap;	pts[4].Y = pts[3].Y;
	pts[5].X = pts[4].X;			pts[5].Y = pts[0].Y;

	g.DrawPolygon(setgPen(clWHITE), pts, 6);
	g.FillPolygon(setgBrush(clWHITE), pts, 6, FillModeAlternate);
}
