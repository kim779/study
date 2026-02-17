// fmLabel.cpp : implementation file
//

#include "stdafx.h"
#include "fmLabel.h"
#include "axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmLabel

IMPLEMENT_DYNCREATE(CfmLabel, CfmBase)

CfmLabel::CfmLabel()
{
	EnableAutomation();
}

CfmLabel::CfmLabel(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	m_pop = false;
}

CfmLabel::~CfmLabel()
{
}


void CfmLabel::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmLabel, CfmBase)
	//{{AFX_MSG_MAP(CfmLabel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmLabel, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmLabel)
	DISP_PROPERTY_EX(CfmLabel, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CfmLabel, "Data", _getData, _setData, VT_BSTR)
	DISP_PROPERTY_EX(CfmLabel, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmLabel, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CfmLabel, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_FUNCTION(CfmLabel, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmLabel, "Blink", _Blink, VT_EMPTY, VTS_BOOL VTS_I4)
	DISP_FUNCTION(CfmLabel, "Push", _Push, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CfmLabel, "Pop", _Pop, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CfmLabel, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmLabel to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {4846F8F7-758E-4CF7-870F-D1AE218F75B1}
static const IID IID_IfmLabel =
{ 0x4846f8f7, 0x758e, 0x4cf7, { 0x87, 0xf, 0xd1, 0xae, 0x21, 0x8f, 0x75, 0xb1 } };

BEGIN_INTERFACE_MAP(CfmLabel, CfmBase)
	INTERFACE_PART(CfmLabel, IID_IfmLabel, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmLabel message handlers

void CfmLabel::_Refresh() 
{
	Refresh();
}

void CfmLabel::_Blink(BOOL set, long color) 
{
	m_axform->DoSomething(doBLINK, this, set ? 1 : 0, (LPARAM)color);
}

void CfmLabel::_Push(LPCTSTR name) 
{
	m_axform->DoSomething(doPUSH, this, (WPARAM)0, (LPARAM)name);
}

void CfmLabel::_Pop(LPCTSTR name) 
{
	m_axform->DoSomething(doPOP, this, (WPARAM)0, (LPARAM)name);
}

void CfmLabel::_Clear() 
{
	m_strR.Empty();
	if (m_form->properties & PR_VISIBLE)
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, true);
		else
			invalidateRect(&m_pRc, false);
	}
}

BSTR CfmLabel::_getText() 
{
	CString strResult = _T("");

	CfmBase::ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmLabel::_setText(LPCTSTR lpszNewValue) 
{
	CfmBase::WriteData(lpszNewValue);
}

BSTR CfmLabel::_getData() 
{
	CString strResult = _T("");

	CfmBase::ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmLabel::_setData(LPCTSTR lpszNewValue) 
{
	CfmBase::WriteData(lpszNewValue);
}

long CfmLabel::_getPRgb() 
{
	return m_pRGB;
}

void CfmLabel::_setPRgb(long nNewValue) 
{
	CfmBase::SetBkColor(nNewValue);
}

long CfmLabel::_getTRgb() 
{
	return m_tRGB;
}

void CfmLabel::_setTRgb(long nNewValue) 
{
	CfmBase::SetFgColor(nNewValue);
}

BOOL CfmLabel::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmLabel::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? TRUE : FALSE);
}

void CfmLabel::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	CRect	tRc;
	CSize	tSz;
	CBrush*	brush;
	CString	tmps;
	int	ndc  = dc->SaveDC();
	UINT	format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;

	setPen(dc);
	brush = setBrush(dc);
	setTextColor(dc);
	setPaintColor(dc);
	setFont(dc);

	tSz = CSize(0, 0);
	tRc = m_pRc;
	if (!(m_form->properties & PR_TRANSPARENT))
		dc->FillRect(tRc, brush);

	switch (m_form->align)
	{
	case AL_RIGHT:
		format |= DT_RIGHT;	break;
	case AL_CENTER:
		format |= DT_CENTER;	break;
	case AL_LEFT:
	default:
		format |= DT_LEFT;	break;
	}

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
	{
		if (tmps.Find("\\n") != -1)
			tmps.Replace("\\n", "\n");
		if (tmps.Find('\n') != -1)
		{
			format &= ~DT_SINGLELINE;
			format |= DT_EXTERNALLEADING;
		}
	}
	dc->DrawText(tmps, tRc, format);
	if (m_pop)
		drawEdge(dc, tRc, EDGE_RAISED, BF_RECT);
	dc->RestoreDC(ndc);
}

void CfmLabel::OnLButton(bool down, CPoint pt, int& result)
{
	result      = RC_NOP;
	m_mousedown = down;
	if (m_mousedown && m_form->cursor >= CS_CURSOR)
		m_axform->DoSomething(doCURSOR, this, m_form->cursor);
}

int CfmLabel::OnDrag(CPoint pt, int& result)
{
	int	rc  = CfmBase::OnDrag(pt, result);

	if (m_form->cursor == CS_POP)
	{
		bool	pop = m_pop;
		m_pop = (result == RC_IN) ? true : false;
		if (pop != m_pop)
		{
			if (m_form->properties & PR_TRANSPARENT)
				invalidateRect(&m_pRc, true);
			else
				invalidateRect(&m_pRc, false);
		}
	}
	else if (m_form->cursor >= CS_CURSOR)
		m_axform->DoSomething(doCURSOR, this, (result == RC_IN) ? m_form->cursor : CS_NONE);

	return rc;
}
