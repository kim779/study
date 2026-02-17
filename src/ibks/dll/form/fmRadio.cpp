// fmRadio.cpp : implementation file
//

#include "stdafx.h"
#include "fmRadio.h"
#include "axform.h"
#include "image.h"

#include "../lib/axislib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmRadio

IMPLEMENT_DYNCREATE(CfmRadio, CfmBase)

CfmRadio::CfmRadio()
{
	EnableAutomation();
}

CfmRadio::CfmRadio(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	m_mousedown = false;
	m_iRc = m_pRc;
	m_tRc = m_pRc;

	m_pic = NULL;
	m_images.RemoveAll();
	if (m_form->type == BTN_RADIO && m_form->borders == RD_PUSH)
	{
		CString	path;
		path.Format(_T("%s\\%s\\%s"), m_axform->m_root.GetString(), IMAGEDIR, &m_axform->m_strR[m_form->vals[1]]);
		m_pic = new Cimage(m_axform->m_draw, path);
	}
	else
		loadBitmap(m_form->type == BTN_RADIO);
}

CfmRadio::~CfmRadio()
{
	if (m_pic)
		delete m_pic;
	m_images.RemoveAll();
}


void CfmRadio::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmRadio, CfmBase)
	//{{AFX_MSG_MAP(CfmRadio)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmRadio, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmRadio)
	DISP_PROPERTY_EX(CfmRadio, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CfmRadio, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmRadio, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CfmRadio, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmRadio, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_PROPERTY_EX(CfmRadio, "Checked", _getChecked, _setChecked, VT_BOOL)
	DISP_PROPERTY_EX(CfmRadio, "TabStop", _getTabStop, _setTabStop, VT_BOOL)
	DISP_PROPERTY_EX(CfmRadio, "Caption", _getCaption, _setCaption, VT_BSTR)
	DISP_FUNCTION(CfmRadio, "SetFocus", _SetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmRadio, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmRadio to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {233923D8-D9A5-4F28-AFE2-ABB4819603BD}
static const IID IID_IfmRadio =
{ 0x233923d8, 0xd9a5, 0x4f28, { 0xaf, 0xe2, 0xab, 0xb4, 0x81, 0x96, 0x3, 0xbd } };

BEGIN_INTERFACE_MAP(CfmRadio, CfmBase)
	INTERFACE_PART(CfmRadio, IID_IfmRadio, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmRadio message handlers

void CfmRadio::_SetFocus() 
{
	m_axform->DoSomething(doFOCUS, this);
}

void CfmRadio::_Refresh() 
{
	Refresh();
}

BSTR CfmRadio::_getText() 
{
	CString strResult = _T("");

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmRadio::_setText(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

long CfmRadio::_getPRgb() 
{
	return m_pRGB;
}

void CfmRadio::_setPRgb(long nNewValue) 
{
	CfmBase::SetBkColor(nNewValue);
}

long CfmRadio::_getTRgb() 
{
	return m_tRGB;
}

void CfmRadio::_setTRgb(long nNewValue) 
{
	CfmBase::SetFgColor(nNewValue);
}

BOOL CfmRadio::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmRadio::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

BOOL CfmRadio::_getEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CfmRadio::_setEnable(BOOL bNewValue) 
{
	CfmBase::SetEnable(bNewValue ? true : false);
}

BOOL CfmRadio::_getChecked() 
{
	return (m_form->properties & PR_CHECKED) ? TRUE : FALSE;
}

void CfmRadio::_setChecked(BOOL bNewValue) 
{
	SetChecked(bNewValue ? true : false);
}

BOOL CfmRadio::_getTabStop() 
{
	return (m_form->attr & FA_PASS) ? FALSE : TRUE;
}

void CfmRadio::_setTabStop(BOOL bNewValue) 
{
	if (bNewValue)
		m_form->attr &= ~FA_PASS;
	else
		m_form->attr |= FA_PASS;
}

BSTR CfmRadio::_getCaption() 
{
	return m_strR.AllocSysString();
}

void CfmRadio::_setCaption(LPCTSTR lpszNewValue) 
{
	CfmBase::WriteData(lpszNewValue);
}

void CfmRadio::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	UINT	style, flag;
	CRect	cRc1, cRc2;
	CBrush*	brush;
	CSize	size;
	CString	tmps;
	int	image = -1;
	int	ndc = dc->SaveDC();

	style = DFCS_BUTTONRADIO;
	flag  = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
	setPen(dc);
	setFont(dc);
	setPaintColor(dc);
	brush = setBrush(dc);

	cRc1 = cRc2 = m_pRc;
	if (m_form->properties & PR_ENABLE)
		setTextColor(dc);
	else
	{
		style |= DFCS_INACTIVE;
		setTextColor(dc, getRGB(cXFG));
	}
	if (m_form->properties & PR_CHECKED)
		style |= DFCS_CHECKED;
	if (m_mousedown)
		style |= DFCS_INACTIVE;

	if (m_form->borders == RD_PUSH)
	{
		if (m_pic)
		{
			if (style & DFCS_CHECKED)
				image = PI_DOWN;
			else if (m_mousedown)
				image = PI_ENTER;
			else	image = PI_DEFAULT;
			if (m_pic->GetValid(image))
			{
				m_pic->DrawPicture(dc, m_pRc, AL_STRETCH, m_axform->m_oldVers, image);
				dc->SetBkMode(TRANSPARENT);
				flag |= DT_CENTER;
			}
			else	image = -1;
		}
		if (image < 0)
		{
			if (!(m_form->properties & PR_TRANSPARENT))
				dc->FillRect(m_pRc, brush);
			if ((style & DFCS_CHECKED) || m_mousedown)
				drawEdge(dc, m_pRc, EDGE_SUNKEN, BF_RECT);
			else
				drawEdge(dc, m_pRc, EDGE_RAISED, BF_RECT);
			flag |= DT_LEFT;
		}
		dc->DrawText(m_strR, m_pRc, flag);
		m_iRc = m_tRc = m_pRc;
		cRc2.DeflateRect(2, 2);
	}
	else
	{
		CBitmap* bitmap = NULL;

		tmps = m_strR;
		tmps.TrimLeft(); tmps.TrimRight();
		if (tmps.IsEmpty())
			tmps = "°¡³ª´ÙABW";
		size = dc->GetOutputTextExtent(tmps);
		cRc1.bottom = m_pRc.top  + size.cy;
		if (m_pRc.Height() > cRc1.Height())
		{
			int	dx = max(0, (m_pRc.Height() - cRc1.Height())/2);
			cRc1.top += dx;
			cRc1.bottom = min(m_pRc.bottom, cRc1.top+size.cy);
		}
		cRc1.right  = m_pRc.left + cRc1.Height();
		if (!(m_form->properties & PR_TRANSPARENT))
			dc->FillRect(m_pRc, brush);

		if (style & DFCS_CHECKED)
			image = (style & DFCS_INACTIVE) ? 3 : 1;
		else if (style & DFCS_INACTIVE)
			image = 2;
		else	image = 0;

		if (m_images.Lookup(image, bitmap))
		{
			CDC	memDC;
			BITMAP	bm;

			memDC.CreateCompatibleDC(dc);
			bitmap->GetBitmap(&bm);
			if (!m_axform->m_oldVers)
			{
				CBitmap* pbmp = (CBitmap *)memDC.SelectObject(bitmap);
				::TransparentBlt(dc->m_hDC, cRc1.left, cRc1.top, cRc1.Width(), cRc1.Height(),
							memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
				memDC.SelectObject(pbmp);
			}
			else
				transparentBlt(dc, bitmap, cRc1.left, cRc1.top, cRc1.Width(), cRc1.Height());
		}
		else
			dc->DrawFrameControl(cRc1, DFC_BUTTON, style);

	//	if (m_pRc.Height() < m_pRc.Width())
		{
			cRc2.left = cRc1.right + 3;
			dc->DrawText(m_strR, cRc2, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);
		}
		m_iRc = cRc1;
		m_tRc = cRc2;
	}

//	m_mousedown = false;
	if (m_focus)
	{
		setTextColor(dc, getRGB(cBLACK));
		dc->SelectStockObject(BLACK_PEN);
		dc->DrawFocusRect(cRc2);
	}

	dc->RestoreDC(ndc);
}

void CfmRadio::OnLButton(bool down, CPoint pt, int& result)
{
	result      = RC_NOP;
	m_mousedown = down;
	if (IsPointInRect(pt))
	{
		if (m_mousedown)
		{
			SetChecked(true);
			if (m_form->cursor >= CS_CURSOR)
				m_axform->DoSomething(doCURSOR, this, m_form->cursor);
		}
		else	m_axform->m_cursor = pt;
	}
	invalidateRect(&m_iRc, false);
}

void CfmRadio::OnDblClick(CPoint pt, int& result)
{
	OnLButton(false, pt, result);
}

int CfmRadio::OnDrag(CPoint pt, int& result)
{
	int	rc = CfmBase::OnDrag(pt, result);

	if (m_form->cursor >= CS_CURSOR)
		m_axform->DoSomething(doCURSOR, this, (result == RC_IN) ? m_form->cursor : CS_NONE);

	return rc;
}

void CfmRadio::SetFocus(bool focus)
{
	bool	setfocus = false;

	if (m_focus != focus)
	{
		m_focus  = focus;
		setfocus = focus ? true : false;
		if (!m_focus)
			m_mousedown = false;
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_tRc, true);
		else
			invalidateRect(&m_tRc, false);
	}
//	SetChecked(true);
	if (setfocus)
		m_axform->DoSomething(doSETFOCUS, this);
}

void CfmRadio::SetChecked(bool checked)
{
	bool	ischecked = (m_form->properties & PR_CHECKED) ? true : false;
	if (ischecked != checked)
	{
		if (checked)
			m_form->properties |= PR_CHECKED;
		else
			m_form->properties &= ~PR_CHECKED;

		if (m_form->type == BTN_RADIO && checked)
			m_axform->UnCheckRadioGroup((char *)m_form->name);
	}
	invalidateRect(&m_iRc, false);
}

void CfmRadio::ReadData(CString& data, bool edit, int size, int col, int row)
{
	if (size > 0)
		data = CString('0', size-1);
	else
		data.Empty();
	data += (m_form->properties & PR_CHECKED) ? '1' : '0';
}

void CfmRadio::WriteData(CString data, bool redraw, int co, int row)
{
	if (data.IsEmpty())
		return;

	switch (data[0])
	{
	case '1':
		SetChecked(true);
		m_axform->SetRadioGroup((char *)m_form->name, true);
		break;
	case '0':
		SetChecked(false);
		m_axform->SetRadioGroup((char *)m_form->name, false);
		break;
	default:
		break;
	}
}

void CfmRadio::Refresh()
{
	if (m_form->properties & PR_VISIBLE)
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_iRc, true);
		else
			invalidateRect(&m_iRc, false);
	}
}

int CfmRadio::keyArrowLeft(int& result)
{
	result = RC_NOP;
	return m_axform->DoSomething(doRADIO, this, false);
}

void CfmRadio::keyArrowRight(int& result)
{
	result = RC_NOP;
	m_axform->DoSomething(doRADIO, this, true);
}

void CfmRadio::loadBitmap(bool radio)
{
	CString	path, name;
	CBitmap* pic;

	name = radio ? "radio" : "check";
	m_images.RemoveAll();
	for (int ii = 0; ii < 4; ii++)
	{
		switch (ii)
		{
		case 0:	path.Format(_T("%s/%s/ax%s.bmp"),    m_axform->m_root.GetString(), IMAGEDIR, name.GetString());	break;	// normal
		case 1:	path.Format(_T("%s/%s/ax%s_c.bmp"),  m_axform->m_root.GetString(), IMAGEDIR, name.GetString());	break;	// checked
		case 2:	path.Format(_T("%s/%s/ax%s_d.bmp"),  m_axform->m_root.GetString(), IMAGEDIR, name.GetString());	break;	// normal_disable
		case 3:	path.Format(_T("%s/%s/ax%s_cd.bmp"), m_axform->m_root.GetString(), IMAGEDIR, name.GetString());	break;	// checked_disable
		}
		pic = m_axform->m_draw->SetBitmap((char *)path.operator LPCTSTR());
		if (pic)
			m_images.SetAt(ii, pic);
	}
}