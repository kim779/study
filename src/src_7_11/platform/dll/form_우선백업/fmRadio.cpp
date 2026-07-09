// fmRadio.cpp : implementation file
//

#include "stdafx.h"
#include "fmRadio.h"
#include "axform.h"

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

CfmRadio::CfmRadio(CAxisForm* axform, struct _formR* formR)
	: CfmBase(axform, formR)
{
	m_mousedown = false;
	m_iRc = m_pRc;
	m_tRc = m_pRc;
}

CfmRadio::~CfmRadio()
{
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
	CString strResult;

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

void CfmRadio::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	UINT	style;
	CRect	cRc1, cRc2;
	CBrush*	brush;
	int	ndc = dc->SaveDC();

	style = DFCS_BUTTONRADIO;
	setPen(dc);
	brush = setBrush(dc);
	setPaintColor(dc);
	setFont(dc);

	cRc1 = cRc2 = m_pRc;
	if (!(m_form->properties & PR_TRANSPARENT))
		dc->FillRect(m_pRc, brush);

	if (m_form->properties & PR_ENABLE)
		setTextColor(dc);
	else
	{
		setTextColor(dc, getRGB(cXFG));
		style |= DFCS_INACTIVE;
	}

	if (m_form->properties & PR_CHECKED)
		style |= DFCS_CHECKED;

	float	width  = m_hR * 100;
	float	height = m_vR * 100;

	float	fwidth  = m_fWidth;
	float	fheight = m_fHeight;

	if (abs((int)(width - height)) > 35)
	{
		if (m_hR > m_vR)
			fwidth--;
		else
			fheight--;
	}

	if (width < 50 && height > 70)
	{
		fwidth += float(0.25);
	}

	if (width < 50)
	{
		fheight -= 3;
		if (width < 35)
			fheight--;
	}

	cRc1.right  = m_pRc.left + (int)fheight - 1;
	cRc1.bottom = m_pRc.top + (int)fheight - 1;
	if (m_pRc.Height() < m_pRc.Width())
	{
		cRc2.left = cRc1.right + 3;
		dc->DrawText(m_strR, cRc2, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);
	}

	if (m_pRc.Height() > cRc1.Height())
	{
		int	dx = (m_pRc.Height() - cRc1.Height())/2;
		cRc1.top   += dx;
		cRc1.bottom = cRc1.top + (int)fheight - 1;
	}

	if (m_mousedown)
	{
		style |= DFCS_INACTIVE;
		dc->DrawFrameControl(cRc1, DFC_BUTTON, style);
	}
	else
		dc->DrawFrameControl(cRc1, DFC_BUTTON, style);

	m_iRc = cRc1;
	m_iRc.left   += 2;
	m_iRc.top    +=2;
	m_iRc.right  -=2;
	m_iRc.bottom -=2;
	m_tRc = cRc2;
	m_mousedown = false;

	if (m_focus)
	{
		dc->SelectStockObject(BLACK_PEN);
		dc->DrawFocusRect(cRc2);
	}

	dc->RestoreDC(ndc);
}

void CfmRadio::OnLButton(bool down, CPoint pt, int& result)
{
	result      = RC_NOP;
	m_mousedown = down;
	if (!m_mousedown && IsPointInRect(pt))
	{
		m_axform->m_cursor = pt;
		SetChecked(true);
		return;
	}
	invalidateRect(&m_iRc, false);
}

void CfmRadio::OnDblClick(CPoint pt, int& result)
{
	OnLButton(false, pt, result);
}

void CfmRadio::SetFocus(bool focus)
{
	if (m_focus != focus)
	{
		m_focus = focus;
		if (!m_focus)
			m_mousedown = false;
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_tRc, true);
		else
			invalidateRect(&m_tRc, false);
	}

//	SetChecked(true);
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
			m_axform->UnCheckRadioGroup(m_name);
	}
	invalidateRect(&m_iRc, false);
}

void CfmRadio::ReadData(CString& data, bool edit, int col, int row)
{
	data = (m_form->properties & PR_CHECKED) ? '1' : '0';
}

void CfmRadio::WriteData(CString data, bool redraw, int co, int row)
{
	switch (data[0])
	{
	case '1':
		SetChecked(true);
		m_axform->SetRadioGroup(m_name, true);
		break;
	case '0':
		SetChecked(false);
		m_axform->SetRadioGroup(m_name, false);
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
