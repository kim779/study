// fmPanel.cpp : implementation file
//

#include "stdafx.h"
#include "fmPanel.h"
#include "axform.h"
#include "image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmPanel

IMPLEMENT_DYNCREATE(CfmPanel, CfmBase)

CfmPanel::CfmPanel()
{
	EnableAutomation();
}

CfmPanel::CfmPanel(CAxisForm* axform, struct _formR* formR)
	: CfmBase(axform, formR)
{
	m_image = PI_DEFAULT;
	m_pic   = NULL;

	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && m_form->vals[1] != NOVALUE)
	{
		CString	path;
		path.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, &m_axform->m_strR[m_form->vals[1]]);
		m_pic = new Cimage(m_axform->m_draw, path);

		if (m_form->properties & PR_IMAGETEXT)
			m_form->type = PN_NONE;
	}
}

CfmPanel::~CfmPanel()
{
	if (m_pic)	delete m_pic;
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

BSTR CfmPanel::_getText() 
{
	CString strResult;

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

void CfmPanel::Draw(CDC* dc)
{
 	if (!(m_form->properties & PR_VISIBLE))
		return;

	UINT	edgeStyle;
	UINT	format;
	CBrush*	brush;
	CRect	tRc;
	CString	tmps;
	int	ndc = dc->SaveDC();

	setPen(dc);
	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT))
	{
		if (m_pic && m_pic->GetValid(m_image))
			m_pic->DrawPicture(dc, m_pRc, m_form->align, m_image);
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
		brush = setBrush(dc);

		if (!(m_form->properties & (PR_IMAGETEXT|PR_TRANSPARENT)))
		{
			dc->FillRect(m_pRc, brush);
			setPaintColor(dc);
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
			break;
		}
		drawEdge(dc, m_pRc, edgeStyle, BF_RECT);
		break;

	case PN_LINE:
		setPen(dc, m_bRGB);
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(m_pRc);
	case PN_NONE:
		break;
	}

	dc->RestoreDC(ndc);
}

void CfmPanel::OnLButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	if (down)
		m_axform->m_cursor = pt;
	if (!(m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && (m_form->properties & PR_HOVER)))
		return;

	m_image = down ? PI_DOWN : PI_DEFAULT;
	if (!(m_form->properties & PR_TRANSPARENT))
		invalidateRect(&m_pRc, false);
}

int CfmPanel::OnDrag(CPoint pt, int& result)
{
	int	rc, state, image = m_image;

	rc = CfmBase::OnDrag(pt, result);
	state = result;
	if (!(m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && (m_form->properties & PR_HOVER)))
		return -1;

	result = (rc == RC_IN) ? RC_HOVER : RC_OUT;

	switch (result)
	{
	case RC_HOVER:
		m_image = PI_ENTER;	break;
	case RC_OUT:
		m_image = PI_DEFAULT;	break;
	default:
		return -1;
	}

	if (!(m_form->properties & PR_TRANSPARENT) && image != m_image)
		invalidateRect(&m_pRc, false);
	return -1;
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
		path.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, data);
		m_pic = new Cimage(m_axform->m_draw, path);
		if (redraw)
			invalidateRect(&m_pRc, false);
	}
	else
		CfmBase::WriteData(data, redraw, col, row);
}

void CfmPanel::ReadData(CString& data, bool edit, int col, int row)
{
	CfmBase::ReadData(data, edit, col, row);
}
