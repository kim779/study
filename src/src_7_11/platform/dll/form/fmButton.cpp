// fmButton.cpp : implementation file
//

#include "stdafx.h"
#include "fmButton.h"
#include "axform.h"
#include "image.h"

#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmButton

IMPLEMENT_DYNCREATE(CfmButton, CfmBase)

CfmButton::CfmButton()
{
	EnableAutomation();
}

CfmButton::CfmButton(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	m_mousedown = false;
	m_image     = PI_DEFAULT;

	m_pic = NULL;
	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && m_form->vals[1] != NOVALUE)
	{
		CString	path;
		path.Format(_T("%s\\%s\\%s"), m_axform->m_root.GetString(), IMAGEDIR, &m_axform->m_strR[m_form->vals[1]]);
		m_pic = new Cimage(m_axform->m_draw, path);
		if (m_form->properties & PR_IMAGETEXT)
			m_form->properties |= PR_TRANSPARENT;
	}
}

CfmButton::~CfmButton()
{
	if (m_pic)
		delete m_pic;
}


void CfmButton::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmButton, CfmBase)
	//{{AFX_MSG_MAP(CfmButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmButton, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmButton)
	DISP_PROPERTY_EX(CfmButton, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CfmButton, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmButton, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CfmButton, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmButton, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_PROPERTY_EX(CfmButton, "TabStop", _getTabStop, _setTabStop, VT_BOOL)
	DISP_FUNCTION(CfmButton, "SetFocus", _SetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmButton, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmButton to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {6ED765AC-16A4-4038-A181-7D5D0C00FD3A}
static const IID IID_IfmButton =
{ 0x6ed765ac, 0x16a4, 0x4038, { 0xa1, 0x81, 0x7d, 0x5d, 0xc, 0x0, 0xfd, 0x3a } };

BEGIN_INTERFACE_MAP(CfmButton, CfmBase)
	INTERFACE_PART(CfmButton, IID_IfmButton, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmButton message handlers

void CfmButton::_SetFocus() 
{
	if (m_form->properties & PR_CHECKED)
		m_axform->DoSomething(doFOCUS, this);
}

void CfmButton::_Refresh() 
{
	Refresh();
}

BSTR CfmButton::_getText() 
{
	CString strResult = _T("");

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmButton::_setText(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

long CfmButton::_getPRgb() 
{
	return (long)m_pRGB;
}

void CfmButton::_setPRgb(long nNewValue) 
{
	CfmBase::SetBkColor(nNewValue);
}

long CfmButton::_getTRgb() 
{
	return (long)m_tRGB;
}

void CfmButton::_setTRgb(long nNewValue) 
{
	CfmBase::SetFgColor(nNewValue);
}

BOOL CfmButton::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmButton::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

BOOL CfmButton::_getEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CfmButton::_setEnable(BOOL bNewValue) 
{
	CfmBase::SetEnable(bNewValue ? true : false);
}

BOOL CfmButton::_getTabStop() 
{
	return (m_form->attr & FA_PASS) ? FALSE : TRUE;
}

void CfmButton::_setTabStop(BOOL bNewValue) 
{
	if (bNewValue)
		m_form->attr &= ~FA_PASS;
	else
		m_form->attr |= FA_PASS;
}

void CfmButton::SetFont(int point, int style, CString fonts, bool resize)
{
	if (!(m_form->properties & (PR_IMAGE|PR_IMAGETEXT)))
		CfmBase::SetFont(point, style, fonts, resize);
}

void CfmButton::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	UINT	format;
	CRect	cRc(0,0,0,0);
	CBrush*	brush = (CBrush *) 0;
	CString	str = _T("");
	int	ndc = dc->SaveDC();
	bool	border;

	setPen(dc);
	border = true;
	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && m_pic && m_pic->GetValid(m_image))
	{
		border = false;
		m_pic->DrawPicture(dc, m_pRc, m_form->align, m_axform->m_oldVers, m_image);
		if (!(m_form->properties & PR_HOVER) && m_mousedown)
			border = true;
	}
	else if (!(m_form->properties & PR_TRANSPARENT))
	{
		border = true;
		brush = setBrush(dc);
		dc->FillRect(m_pRc, brush);
	}

	if (!(m_form->properties & PR_IMAGE) || m_form->properties & PR_IMAGETEXT)
	{
		setFont(dc);
		setPaintColor(dc);

		format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		if (m_form->properties & PR_IMAGETEXT)
			format |= DT_CENTER;
		else
		{
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
		}

		str = m_strR;
		cRc = m_pRc;
		if (m_form->properties & PR_VERT)
		{
			int	gap, len = str.GetLength();

			str = convertVString(str);
			format &= ~DT_SINGLELINE;
			gap  = (cRc.Height() - dc->GetOutputTextExtent(str).cy * (str.GetLength() - len)) / 2;
			if (gap >= 0)
				cRc.OffsetRect(0, gap);
		}
		else
		{
			if (str.Find("\\n") != -1)
				str.Replace("\\n", "\n");
			if (str.Find('\n') != -1)
			{
				CRect	sRc = cRc;

				format &= ~DT_SINGLELINE;
				format |= DT_EXTERNALLEADING;
				dc->DrawText(str, sRc, format|DT_CALCRECT);
				if (sRc.Height() < cRc.Height())
				{
					cRc.top += (cRc.Height() - sRc.Height())/2;
					cRc.bottom = cRc.top + sRc.Height();
				}
			}
		}

		if (m_form->properties & PR_ENABLE)
		{
			setTextColor(dc);
			dc->DrawText(str, cRc, format);
		}
		else
		{
			setTextColor(dc, getRGB(cWHITE));
			dc->DrawText(str, cRc, format);
			cRc.left -= 1;
			cRc.top  -= 1;
			setTextColor(dc, getRGB(cXFG));
			dc->SetBkMode(TRANSPARENT);
			dc->DrawText(str, cRc, format);
		}
	}

	if (border)
	{
		if (m_mousedown)
			drawEdge(dc, m_pRc, EDGE_SUNKEN, BF_RECT);
		else
			drawEdge(dc, m_pRc, EDGE_RAISED, BF_RECT);
	}

	cRc = m_pRc;
	cRc.left   += 4;
	cRc.top    += 4;
	cRc.right  -= 4;
	cRc.bottom -= 4;
	if (m_focus)
	{
		setTextColor(dc, getRGB(cBLACK));
		dc->SelectStockObject(BLACK_PEN);
		dc->DrawFocusRect(cRc);
	}
	
	dc->RestoreDC(ndc);
}

void CfmButton::OnLButton(bool down, CPoint pt, int& result)
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
		m_image = m_mousedown ? PI_DOWN : PI_DEFAULT;

	if (m_form->properties & PR_TRANSPARENT)
		invalidateRect(&m_pRc, true);
	else
		invalidateRect(&m_pRc, false);
}

int CfmButton::OnDrag(CPoint pt, int& result)
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
	default:
		break;
	}

	if (image != m_image)
		invalidateRect(&m_pRc, false);
	if (m_form->cursor >= CS_CURSOR)
		m_axform->DoSomething(doCURSOR, this, (state == RC_IN) ? m_form->cursor : CS_NONE);
	return rc;
}

void CfmButton::WriteData(CString data, bool redraw, int col, int row)
{
	bool	image = false;

	if (m_form->properties & PR_IMAGE)
		image = true;
	else if (m_form->properties & PR_IMAGETEXT)
	{
		CString	string = data;
		string.MakeLower();
		if (string.Find(".bmp") != -1 || string.Find(".gif") != -1)
			image = true;
	}

	if (image)
	{
		if (m_pic)
		{
			delete m_pic;
			m_pic = NULL;
		}

		CString	path;
		path.Format(_T("%s\\%s\\%s"), m_axform->m_root.GetString(), IMAGEDIR, data);
		m_pic   = new Cimage(m_axform->m_draw, path);
		m_image = PI_DEFAULT;
		if (redraw)
			invalidateRect(&m_pRc, false);
	}
	else
		CfmBase::WriteData(data, redraw, col, row);
}

void CfmButton::ReadData(CString& data, bool edit, int size, int col, int row)
{
	CfmBase::ReadData(data, edit, size, col, row);
}

void CfmButton::SetFocus(bool focus)
{
	if (m_focus != focus)
	{
		m_focus = focus;
		if (!m_focus)
			m_mousedown = false;
		invalidateRect(&m_pRc, false);
		if (m_focus)
			m_axform->DoSomething(doSETFOCUS, this);
	}
}
