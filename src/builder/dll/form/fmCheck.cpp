// fmCheck.cpp : implementation file
//

#include "stdafx.h"
#include "fmCheck.h"
#include "axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmCheck

IMPLEMENT_DYNCREATE(CfmCheck, CfmRadio)

CfmCheck::CfmCheck()
{
	EnableAutomation();
}

CfmCheck::CfmCheck(CAxisForm* axform, struct _formR* formR)
	: CfmRadio(axform, formR)
{
}

CfmCheck::~CfmCheck()
{
}


void CfmCheck::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmCheck, CfmRadio)
	//{{AFX_MSG_MAP(CfmCheck)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmCheck, CfmRadio)
	//{{AFX_DISPATCH_MAP(CfmCheck)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmCheck to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {7E2C96E8-FBD9-4332-831A-8FC50D65554B}
static const IID IID_IfmCheck =
{ 0x7e2c96e8, 0xfbd9, 0x4332, { 0x83, 0x1a, 0x8f, 0xc5, 0xd, 0x65, 0x55, 0x4b } };

BEGIN_INTERFACE_MAP(CfmCheck, CfmRadio)
	INTERFACE_PART(CfmCheck, IID_IfmCheck, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmCheck message handlers

void CfmCheck::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	UINT	style;
	CRect	cRc1, cRc2;
	CBrush*	brush;
	int	ndc = dc->SaveDC();

	style = DFCS_BUTTONCHECK;
	setPen(dc);
	setFont(dc);
	setPaintColor(dc);
	brush = setBrush(dc);

	cRc1 = cRc2 = m_pRc;
	if (!(m_form->properties & PR_TRANSPARENT))
		dc->FillRect(m_pRc, brush);

	if (m_form->properties & PR_ENABLE)
		setTextColor(dc);
	else
	{
		style |= DFCS_INACTIVE;
		setTextColor(dc, getRGB(cXFG));
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
		fwidth += float(0.25);

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
		CString	str = m_strR;
		dc->DrawText(str, cRc2, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);
	}

	if (m_pRc.Height() > cRc1.Height())
	{
		int	dx = (m_pRc.Height() - cRc1.Height())/2;
		cRc1.top += dx;
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
	m_iRc.left++;
	m_iRc.top++;
	m_iRc.right--;
	m_iRc.bottom--;
	m_tRc = cRc2;
	m_mousedown = false;

	if (m_focus)
	{
		dc->SelectStockObject(BLACK_PEN);
		dc->DrawFocusRect(cRc2);
	}

	dc->RestoreDC(ndc);
}

void CfmCheck::OnLButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	m_mousedown = down;
	if (!m_mousedown && IsPointInRect(pt))
	{
		m_axform->m_cursor = pt;
		if (m_form->properties & PR_CHECKED)
			m_form->properties &= ~PR_CHECKED;
		else
			m_form->properties |= PR_CHECKED;
	}
	invalidateRect(&m_iRc, false);
}

void CfmCheck::OnDblClick(CPoint pt, int& result)
{
	OnLButton(false, pt, result);
}

void CfmCheck::SetFocus(bool focus)
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
}

void CfmCheck::UpdateData(int key, bool moving, int& result)
{
	if (key = ' ')
	{
		bool	checked = (m_form->properties & PR_CHECKED) ? true : false;
		SetChecked(!checked);
	}
}

void CfmCheck::InsertData(int key, bool moving, int& result)
{
	UpdateData(key, moving, result);
}
