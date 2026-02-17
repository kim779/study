// fmPanel.cpp : implementation file
//

#include "stdafx.h"
#include "fmPanel.h"
#include "axform.h"
#include "image.h"
#include "../lib/axislib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	wIMAGE		5

#define	nPANEL		4
#define	iLT		0	// LeftTop
#define	iRT		1	// RightTop
#define	iLB		2	// LeftBottom
#define	iRB		3	// RightBottom

/////////////////////////////////////////////////////////////////////////////
// CfmPanel

IMPLEMENT_DYNCREATE(CfmPanel, CfmBase)

CfmPanel::CfmPanel()
{
	EnableAutomation();
}

CfmPanel::CfmPanel(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	m_image = PI_DEFAULT;
	m_pic   = NULL;

	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && m_form->vals[1] != NOVALUE)
	{
		CString	path;
		path.Format(_T("%s\\%s\\%s"), m_axform->m_root.GetString(), IMAGEDIR, &m_axform->m_strR[m_form->vals[1]]);
		m_pic = new Cimage(m_axform->m_draw, path);

		if (m_form->properties & PR_IMAGETEXT)
			m_form->type = PN_NONE;
	}

	if (m_form->type == PN_ROUND)
	{
		if (m_form->size == 0)
			m_form->size = 1;
		if ((m_form->properties & PR_BOLD) && (m_form->size < m_form->keys))
			m_form->size = (int)m_form->keys+1;

		m_images.RemoveAll();
		SetEnum(_T(""), 0, 100);
	}

	m_bottom_gap = m_right_gap = 0;
}

CfmPanel::~CfmPanel()
{
	if (m_pic)
		delete m_pic;
	m_images.RemoveAll();
}


void CfmPanel::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmPanel, CfmBase)
	//{{AFX_MSG_MAP(CfmPanel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmPanel, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmPanel)
	DISP_PROPERTY_EX(CfmPanel, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CfmPanel, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmPanel, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CfmPanel, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_FUNCTION(CfmPanel, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmPanel, "BlinkText", _BlinkText, VT_EMPTY, VTS_BOOL VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmPanel to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {F7303843-6D61-43A1-BEB7-C03A141BC84B}
static const IID IID_IfmPanel =
{ 0xf7303843, 0x6d61, 0x43a1, { 0xbe, 0xb7, 0xc0, 0x3a, 0x14, 0x1b, 0xc8, 0x4b } };

BEGIN_INTERFACE_MAP(CfmPanel, CfmBase)
	INTERFACE_PART(CfmPanel, IID_IfmPanel, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmPanel message handlers

void CfmPanel::_Refresh() 
{
	Refresh();
}

void CfmPanel::_BlinkText(BOOL set, long color) 
{
	m_axform->DoSomething(doBLINK, this, set ? 1 : 0, (LPARAM)color);
}

BSTR CfmPanel::_getText() 
{
	CString strResult = _T("");

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmPanel::_setText(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

long CfmPanel::_getPRgb() 
{
	return m_pRGB;
}

void CfmPanel::_setPRgb(long nNewValue) 
{
	CfmBase::SetBkColor(nNewValue);
}

long CfmPanel::_getTRgb() 
{
	return m_tRGB;
}

void CfmPanel::_setTRgb(long nNewValue) 
{
	CfmBase::SetFgColor(nNewValue);
}

BOOL CfmPanel::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmPanel::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

void CfmPanel::Adjust(CPoint pt, float hR, float vR)
{
	CfmBase::Adjust(pt, hR, vR);

	if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
	{
		CRect	cRc;

		m_axform->m_view->GetClientRect(&cRc);
		m_pRc.right  = cRc.right;
		if (m_bottom_gap <= 0)
		{
			int height = m_axform->GetHeight();
			if (height > 0)
				m_bottom_gap = height - m_rect.bottom;
		}

		if (m_right_gap <= 0)
		{
			int width = m_axform->GetWidth();
			if (width > 0)
				m_right_gap = width - m_rect.right;
		}
	}
}

void CfmPanel::Resize(float hRatio, float vRatio, bool font, bool redraw)
{
	if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
	{
		CRect	cRc;

		CfmBase::Resize(hRatio, vRatio, font, false);
		m_axform->m_view->GetClientRect(&cRc);
		m_pRc.right  = cRc.right - m_right_gap;
		m_pRc.bottom = cRc.bottom - m_bottom_gap;

		m_rect.left   = (int)(m_pRc.left / m_hR);
		m_rect.top    = (int)(m_pRc.top / m_vR);
		m_rect.right  = (int)(m_pRc.right / m_hR);
		m_rect.bottom = (int)(m_pRc.bottom / m_vR);

		if (redraw)
			invalidateRect(&m_pRc, false);
	}
	else
		CfmBase::Resize(hRatio, vRatio, font, redraw);
}

void CfmPanel::Draw(CDC* dc)
{
 	if (!(m_form->properties & PR_VISIBLE))
		return;

	UINT	edgeStyle = 0;
	UINT	format;
	CBrush*	brush = (CBrush*) 0;
	CRect	tRc;
	CString	tmps;
	int	ndc = dc->SaveDC();

	setPen(dc);
	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT))
	{
		if (m_pic && m_pic->GetValid(m_image))
			m_pic->DrawPicture(dc, m_pRc, m_form->align, m_axform->m_oldVers, m_image);
		else if (!(m_form->properties & PR_TRANSPARENT))
		{
			brush = setBrush(dc);
			dc->FillRect(m_pRc, brush);
		}
	}

	if (!(m_form->properties & PR_IMAGE) || m_form->properties & PR_IMAGETEXT)
	{
		setFont(dc);
		setTextColor(dc);

		if (!(m_form->properties & (PR_IMAGETEXT|PR_TRANSPARENT)))
		{
			if (m_form->properties & PR_GRADIENT && m_form->vals[1] != NOVALUE)
			{
				COLORREF clr = 	(m_form->vals[1] & userCOLOR) ? m_form->vals[1] : getRGB(m_form->vals[1]);
				drawGradation(dc, m_pRGB, clr);
				dc->SetBkMode(TRANSPARENT);
			}
			else
			{
				brush = setBrush(dc);
				dc->FillRect(m_pRc, brush);
				setPaintColor(dc);
			}
		}
		else
			dc->SetBkMode(TRANSPARENT);

		format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		switch (m_form->align)
		{
		case AL_RIGHT:
			format |= DT_RIGHT;	break;
		case AL_LEFT:
			format |= DT_LEFT;	break;
		case AL_CENTER:
		default :
			format |= DT_CENTER;	break;
		}

		tRc  = m_pRc;
		tmps = m_strR;
		if (m_form->properties & PR_VERT)
		{
			int	gap, len = tmps.GetLength();

			tmps = convertVString(tmps);
			format &= ~DT_SINGLELINE;
			gap  = (tRc.Height() - dc->GetOutputTextExtent(tmps).cy * (tmps.GetLength() - len)) / 2;
			if (gap >= 0)
				tRc.OffsetRect(0, gap);
		}
		else
			tRc.DeflateRect(3, 0);
		dc->DrawText(tmps, tRc, format);
	}

	switch (m_form->type)
	{
	case PN_BORDER:
		switch (m_form->borders)
		{
		case BD_RAISED:
			edgeStyle = EDGE_RAISED;	break;
		case BD_SUNKEN:
			edgeStyle = EDGE_SUNKEN;	break;
		case BD_BUMP:
			edgeStyle = EDGE_BUMP;		break;
		case BD_ETCHED:
			edgeStyle = EDGE_ETCHED;	break;
		default:
			edgeStyle = 0;			break;
		}
		drawEdge(dc, m_pRc, edgeStyle, BF_RECT);
		break;

	case PN_LINE:
		setPen(dc, m_bRGB);
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(m_pRc);
		break;

	case PN_LINE3:
		setPen(dc, m_bRGB);
		dc->MoveTo(m_pRc.left, m_pRc.top);
		dc->LineTo(m_pRc.left, m_pRc.bottom-1);
		dc->LineTo(m_pRc.right-1, m_pRc.bottom-1);
		dc->LineTo(m_pRc.right-1, m_pRc.top-1);
		break;

	case PN_ROUND:
		drawRound(dc);
		break;

	case PN_NONE:
		break;
	}

	dc->RestoreDC(ndc);
}

void CfmPanel::OnLButton(bool down, CPoint pt, int& result)
{
	result      = RC_NOP;
	m_mousedown = down;
	if (down)
	{
		m_axform->m_cursor = pt;
		if (m_form->cursor >= CS_CURSOR)
			m_axform->DoSomething(doCURSOR, this, m_form->cursor);
	}
	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && (m_form->properties & PR_HOVER))
	{
		int	image = m_image;
		m_image = down ? PI_DOWN : PI_DEFAULT;
		if (image != m_image && !(m_form->properties & PR_TRANSPARENT))
			invalidateRect(&m_pRc, false);
	}
}

int CfmPanel::OnDrag(CPoint pt, int& result)
{
	int	rc, state, image = m_image;

	rc = CfmBase::OnDrag(pt, result);
	state = result;
	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && (m_form->properties & PR_HOVER))
		result = (result == RC_IN) ? RC_HOVER : RC_OUT;

	switch (result)
	{
	case RC_HOVER:
		m_image = m_mousedown ? PI_DOWN : PI_ENTER;
		break;
	case RC_OUT:
		m_image = PI_DEFAULT;
		break;
	case RC_IN:
		result = RC_OUT;
		[[fallthrough]];
	default:
		break;
	}

	if (image != m_image)
		invalidateRect(&m_pRc, false);
	if (m_form->cursor >= CS_CURSOR)
		m_axform->DoSomething(doCURSOR, this, (state == RC_IN) ? m_form->cursor : CS_NONE);

	return rc;
}

void CfmPanel::WriteData(CString data, bool redraw, int col, int row)
{
	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT))
	{
		if (m_pic)
		{
			delete m_pic;
			m_pic = NULL;
		}

		CString	path;
		path.Format(_T("%s\\%s\\%s"), m_axform->m_root.GetString(), IMAGEDIR, data.GetString());
		m_pic = new Cimage(m_axform->m_draw, path);
		if (redraw)
			invalidateRect(&m_pRc, false);
	}
	else
		CfmBase::WriteData(data, redraw, col, row);
}

void CfmPanel::ReadData(CString& data, bool edit, int size, int col, int row)
{
	CfmBase::ReadData(data, edit, size, col, row);
}

void CfmPanel::SetEnum(CString text, int value, int func)
{
	switch (func)
	{
	case 98:
		m_bRGB = value;
	case 100:
		if (m_form->type == PN_ROUND)
		{
			char	wb[32];

			wb[0] = '\0';
			if (!(m_form->attr & FA_SKIP))
			{
				text.Format("%s/%s/%s", m_axform->m_root, TABDIR, PALETTE);
				GetPrivateProfileString("General", "Palette", "", wb, sizeof(wb), text);
			}
			loadBitmap(wb);
			if (func == 98)
				invalidateRect(&m_pRc, true);
		}
		break;
	}
}

void CfmPanel::drawRound(CDC* dc)
{
	CRect	tRc, sRc;
	int	gap = m_form->size;

	if (m_form->properties & PR_BOLD)
		gap -= m_form->keys;

	tRc.SetRect(m_pRc.left, m_pRc.top, m_pRc.right, m_pRc.top+gap);
	dc->FillSolidRect(tRc, m_bRGB);

	tRc.SetRect(m_pRc.left, m_pRc.top, m_pRc.left+gap, m_pRc.bottom);
	dc->FillSolidRect(tRc, m_bRGB);

	tRc.SetRect(m_pRc.left, m_pRc.bottom-gap, m_pRc.right, m_pRc.bottom);
	dc->FillSolidRect(tRc, m_bRGB);

	tRc.SetRect(m_pRc.right-gap, m_pRc.top, m_pRc.right, m_pRc.bottom);
	dc->FillSolidRect(tRc, m_bRGB);

	tRc = m_pRc;
	tRc.DeflateRect(gap, gap);
	if (m_form->properties & PR_BOLD)
	{
		COLORREF clr = (m_form->onEdit & userCOLOR) ? m_form->onEdit : getRGB(m_form->onEdit);
		gap = (int)m_form->keys;

		sRc.SetRect(tRc.left, tRc.top, tRc.right, tRc.top+gap);
		dc->FillSolidRect(sRc, clr);

		sRc.SetRect(tRc.left, tRc.top, tRc.left+gap, tRc.bottom);
		dc->FillSolidRect(sRc, clr);

		sRc.SetRect(tRc.left, tRc.bottom-gap, tRc.right,tRc.bottom);
		dc->FillSolidRect(sRc, clr);

		sRc.SetRect(tRc.right-gap, tRc.top, tRc.right, tRc.bottom);
		dc->FillSolidRect(sRc, clr);
	}

	CDC	memDC;
	CBitmap	*hbmp, *pbmp = NULL;

	memDC.CreateCompatibleDC(dc);
	if (m_images.Lookup(iLT, hbmp))
	{
		if (!m_axform->m_oldVers)
		{
			pbmp = (CBitmap *)memDC.SelectObject(hbmp);
			::TransparentBlt(dc->m_hDC, tRc.left, tRc.top, wIMAGE, wIMAGE, memDC.m_hDC, 0, 0, wIMAGE, wIMAGE, RGB(255, 0, 255));
			memDC.SelectObject(pbmp);
		}
		else
			transparentBlt(dc, hbmp, tRc.left, tRc.top, wIMAGE, wIMAGE);
	}

	if (m_images.Lookup(iRT, hbmp))
	{
		if (!m_axform->m_oldVers)
		{
			pbmp = (CBitmap *)memDC.SelectObject(hbmp);
			::TransparentBlt(dc->m_hDC, tRc.right-wIMAGE, tRc.top, wIMAGE, wIMAGE, memDC.m_hDC, 0, 0, wIMAGE, wIMAGE, RGB(255, 0, 255));
			memDC.SelectObject(pbmp);
		}
		else
			transparentBlt(dc, hbmp, tRc.right-wIMAGE, tRc.top, wIMAGE, wIMAGE);
	}

	if (m_images.Lookup(iLB, hbmp))
	{
		if (!m_axform->m_oldVers)
		{
			pbmp = (CBitmap *)memDC.SelectObject(hbmp);
			::TransparentBlt(dc->m_hDC, tRc.left, tRc.bottom-wIMAGE, wIMAGE, wIMAGE, memDC.m_hDC, 0, 0, wIMAGE, wIMAGE, RGB(255, 0, 255));
			memDC.SelectObject(pbmp);
		}
		else
			transparentBlt(dc, hbmp, tRc.left, tRc.bottom-wIMAGE, wIMAGE, wIMAGE);
	}

	if (m_images.Lookup(iRB, hbmp))
	{
		if (!m_axform->m_oldVers)
		{
			pbmp = (CBitmap *)memDC.SelectObject(hbmp);
			::TransparentBlt(dc->m_hDC, tRc.right-wIMAGE, tRc.bottom-wIMAGE, wIMAGE, wIMAGE, memDC.m_hDC, 0, 0, wIMAGE, wIMAGE, RGB(255, 0, 255));
			memDC.SelectObject(pbmp);
		}
		else
			transparentBlt(dc, hbmp, tRc.right-wIMAGE, tRc.bottom-wIMAGE, wIMAGE, wIMAGE);
	}
	memDC.DeleteDC();
}

void CfmPanel::loadBitmap(CString theme)
{
	CString	path;
	CBitmap* pic;

	for (int ii = 0; ii < nPANEL; ii++)
	{
		switch (ii)
		{
		case iLT:
			path.Format("%s/%s/axpanel%s%d_lt.bmp", m_axform->m_root, IMAGEDIR, theme, m_form->vals[2]);
			break;
		case iRT:
			path.Format("%s/%s/axpanel%s%d_rt.bmp", m_axform->m_root, IMAGEDIR, theme, m_form->vals[2]);
			break;
		case iLB:
			path.Format("%s/%s/axpanel%s%d_lb.bmp", m_axform->m_root, IMAGEDIR, theme, m_form->vals[2]);
			break;
		case iRB:
			path.Format("%s/%s/axpanel%s%d_rb.bmp", m_axform->m_root, IMAGEDIR, theme, m_form->vals[2]);
			break;
		}

		pic = m_axform->m_draw->SetBitmap((char *)path.operator LPCTSTR());
		if (pic)
			m_images.SetAt(ii, pic);
	}
}
