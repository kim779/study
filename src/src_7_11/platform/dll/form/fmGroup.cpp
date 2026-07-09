// fmGroup.cpp : implementation file
//

#include "stdafx.h"
#include "fmGroup.h"
#include "axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmGroup

IMPLEMENT_DYNCREATE(CfmGroup, CfmBase)

CfmGroup::CfmGroup()
{
	EnableAutomation();
}

CfmGroup::CfmGroup(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	m_form->properties |= PR_TRANSPARENT;
}

CfmGroup::~CfmGroup()
{
}


void CfmGroup::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmGroup, CfmBase)
	//{{AFX_MSG_MAP(CfmGroup)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmGroup, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmGroup)
	DISP_PROPERTY_EX(CfmGroup, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CfmGroup, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_FUNCTION(CfmGroup, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmGroup to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {0AE7F991-14C7-43C3-99B1-4675DAC7482B}
static const IID IID_IfmGroup =
{ 0xae7f991, 0x14c7, 0x43c3, { 0x99, 0xb1, 0x46, 0x75, 0xda, 0xc7, 0x48, 0x2b } };

BEGIN_INTERFACE_MAP(CfmGroup, CfmBase)
	INTERFACE_PART(CfmGroup, IID_IfmGroup, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmGroup message handlers

void CfmGroup::_Refresh() 
{
	Refresh();
}

BSTR CfmGroup::_getText() 
{
	CString strResult;

	strResult = m_strR;
	return strResult.AllocSysString();
}

void CfmGroup::_setText(LPCTSTR lpszNewValue) 
{
	m_strR = lpszNewValue;
	invalidateRect(&m_pRc, false);
}

BOOL CfmGroup::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmGroup::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

void CfmGroup::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	CRect	cRc;
	CSize	tSz;
	int	ndc = dc->SaveDC();

	setTextColor(dc);
	setPaintColor(dc);
	setFont(dc);

	cRc = m_pRc;
	cRc.top = m_pRc.top + (int)(m_fHeight / 2);
	// draw white box
	setPen(dc, getRGB(cYWHITE));
	// left
	dc->MoveTo(cRc.left+1, cRc.top);
	dc->LineTo(cRc.left+1, cRc.bottom);
	// bottom
	dc->MoveTo(cRc.left, cRc.bottom);
	dc->LineTo(cRc.right, cRc.bottom);
	// right
	dc->MoveTo(cRc.right-1, cRc.top);
	dc->LineTo(cRc.right-1, cRc.bottom);

	// draw gray box
	setPen(dc, getRGB(cEDGE));
	// left
	dc->MoveTo(cRc.left, cRc.top);
	dc->LineTo(cRc.left, cRc.bottom);
	// bottom
	dc->MoveTo(cRc.left, cRc.bottom-1);
	dc->LineTo(cRc.right-1, cRc.bottom-1);
	// right
	dc->MoveTo(cRc.right-2, cRc.top);
	dc->LineTo(cRc.right-2, cRc.bottom-1);

	if (!m_strR.IsEmpty())
	{
		cRc.right = cRc.left + 5;
		// white
		setPen(dc, getRGB(cYWHITE));
		dc->MoveTo(cRc.left+1, cRc.top+1);
		dc->LineTo(cRc.right, cRc.top+1);
		// gray
		setPen(dc, getRGB(cEDGE));
		dc->MoveTo(cRc.left, cRc.top);
		dc->LineTo(cRc.right, cRc.top);

		dc->TextOut(cRc.right + 2, m_pRc.top, m_strR, m_strR.GetLength());
		tSz = dc->GetOutputTextExtent(m_strR);

		cRc.left = cRc.right + tSz.cx + 4;
		cRc.right = m_pRc.right;

		// white
		setPen(dc, getRGB(cYWHITE));
		dc->MoveTo(cRc.left, cRc.top+1);
		dc->LineTo(cRc.right-2, cRc.top+1);
		// gray
		setPen(dc, getRGB(cEDGE));
		dc->MoveTo(cRc.left, cRc.top);
		dc->LineTo(cRc.right-1, cRc.top);
	}
	else
	{
		// white
		setPen(dc, getRGB(cYWHITE));
		dc->MoveTo(cRc.left+1, cRc.top+1);
		dc->LineTo(cRc.right-2, cRc.top+1);
		// gray
		setPen(dc, getRGB(cEDGE));
		dc->MoveTo(cRc.left, cRc.top);
		dc->LineTo(cRc.right-1, cRc.top);
	}

	dc->RestoreDC(ndc);
}

int CfmGroup::IsPointInRect(CPoint pt)
{
	return P_OUT;
}
