// fmCheck.cpp : implementation file
//

#include "stdafx.h"
#include "fmCheck.h"
#include "axform.h"
#include "image.h"

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

CfmCheck::CfmCheck(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmRadio(axform, formR, iform)
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

	UINT	style = DFCS_BUTTONCHECK;
	CRect	cRc1, cRc2;
	CBrush*	brush;
	CSize	size;
	CString	tmps;
	int	image = -1;
	int	ndc = dc->SaveDC();
	CBitmap* bitmap = NULL;

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
	if (m_mousedown)
		style |= DFCS_INACTIVE;

	tmps = m_strR;
	tmps.TrimLeft(); tmps.TrimRight();
	if (tmps.IsEmpty())
		tmps = "°¡³ª´ÙABW";
	size = dc->GetOutputTextExtent(tmps);
	cRc1.bottom = m_pRc.top  + size.cy;
	if (m_pRc.Height() > cRc1.Height())
	{
		int	dx = max(0, (m_pRc.Height() - cRc1.Height())/2 - 1);
		cRc1.top += dx;
		cRc1.bottom = min(m_pRc.bottom, cRc1.top+size.cy);
	}
	cRc1.right  = m_pRc.left + cRc1.Height();

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
//	m_mousedown = false;

	if (m_focus)
	{
		setTextColor(dc, getRGB(cBLACK));
		dc->SelectStockObject(BLACK_PEN);
		dc->DrawFocusRect(cRc2);
	}

	dc->RestoreDC(ndc);
}

void CfmCheck::OnLButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	m_mousedown = down;
	if (IsPointInRect(pt))
	{
		if (m_mousedown)
		{
			if (m_form->cursor >= CS_CURSOR)
				m_axform->DoSomething(doCURSOR, this, m_form->cursor);
		}
		else
		{
			m_axform->m_cursor = pt;
			if (m_form->properties & PR_CHECKED)
				m_form->properties &= ~PR_CHECKED;
			else
				m_form->properties |= PR_CHECKED;
		}
	}
	invalidateRect(&m_iRc, false);
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

		if (m_focus)
			m_axform->DoSomething(doSETFOCUS, this);
	}
}

void CfmCheck::UpdateData(int key, bool moving, int& result)
{
	if (key == ' ')
	{
		bool	checked = (m_form->properties & PR_CHECKED) ? true : false;
		SetChecked(!checked);
	}
}

void CfmCheck::InsertData(int key, bool moving, int& result)
{
	UpdateData(key, moving, result);
}
