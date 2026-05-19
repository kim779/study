// fmObject.cpp : implementation file
//

#include "stdafx.h"
#include "fmObject.h"
#include "axform.h"
#include "image.h"

#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmObject

IMPLEMENT_DYNCREATE(CfmObject, CfmBase)

CfmObject::CfmObject()
{
	EnableAutomation();
}

CfmObject::CfmObject(CAxisForm* axform, struct _formR* formR)
	: CfmBase(axform, formR)
{
	m_type = bkNone;
	m_pic  = NULL;
	m_mapN = m_strR;
}

CfmObject::~CfmObject()
{
	if (m_pic)	delete m_pic;
}


void CfmObject::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmObject, CfmBase)
	//{{AFX_MSG_MAP(CfmObject)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmObject, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmObject)
	DISP_PROPERTY_EX(CfmObject, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CfmObject, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CfmObject, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmObject, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_FUNCTION(CfmObject, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmObject, "ClearAll", _ClearAll, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CfmObject, "Send", _Send, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmObject, "RSend", _RSend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmObject, "SetObject", _SetObject, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CfmObject, "GetObject", _GetObject, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CfmObject, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmObject, "Proc", _Proc, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CfmObject, "ProcEx", _ProcEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmObject to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {45FDC0D0-A48C-403C-AD72-E383FCCD784B}
static const IID IID_IfmObject =
{ 0x45fdc0d0, 0xa48c, 0x403c, { 0xad, 0x72, 0xe3, 0x83, 0xfc, 0xcd, 0x78, 0x4b } };

BEGIN_INTERFACE_MAP(CfmObject, CfmBase)
	INTERFACE_PART(CfmObject, IID_IfmObject, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmObject message handlers

void CfmObject::_Clear() 
{
	ClearData();
}

void CfmObject::_ClearAll(long type) 
{
	m_axform->DoSomething(doCLEAR, this, (WPARAM)type);
}

void CfmObject::_Send() 
{
	m_axform->DoSomething(doSEND, this);
}

void CfmObject::_RSend() 
{
	m_axform->DoSomething(doRSEND, this);
}

void CfmObject::_Refresh() 
{
	Refresh();
}

void CfmObject::_SetObject(LPCTSTR maps) 
{
	SetObject(maps);
}

BSTR CfmObject::_GetObject() 
{
	CString strResult;

	strResult = GetObject();
	return strResult.AllocSysString();
}

void CfmObject::_Proc(LPCTSTR procs, LPCTSTR data) 
{
	m_axform->DoProc(procs, this, data);
}

void CfmObject::_ProcEx(LPCTSTR procs, LPCTSTR data, long count) 
{
	m_axform->DoProc(procs, this, data, count);
}

BSTR CfmObject::_getText() 
{
	CString strResult;

	strResult = m_strR;
	return strResult.AllocSysString();
}

void CfmObject::_setText(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

long CfmObject::_getPRgb() 
{
	return m_pRGB;
}

void CfmObject::_setPRgb(long nNewValue) 
{
	m_type = bkBrush;
	CfmBase::SetBkColor(nNewValue);
}

BOOL CfmObject::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmObject::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

BOOL CfmObject::_getEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CfmObject::_setEnable(BOOL bNewValue) 
{
	SetEnable(bNewValue ? true : false);
}

void CfmObject::Resize(float hRatio, float vRatio, bool font, bool redraw)
{
	if (m_form->attr2 & GO_FIX)
	{
		CRect	cRc;

		m_axform->m_view->GetClientRect(&cRc);
		m_pRc.right  = cRc.right;
		m_pRc.bottom = cRc.bottom;
	}
	else
		CfmBase::Resize(hRatio, vRatio, font, redraw);

	if (redraw && m_type != bkNone)
		invalidateRect(&m_pRc, false);
}

void CfmObject::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE) || m_type == bkNone)
		return;

	int	ndc = dc->SaveDC();
	CBrush* brush;
	
	switch (m_type)
	{
	case bkBrush:
		brush = setBrush(dc);
		dc->FillRect(m_pRc, brush);
		break;

	case bkBmp:
		if (!m_pic)
		{
			if (m_strR.IsEmpty())
				break;

			CString path;
			path.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, m_strR);
			m_pic = new Cimage(m_axform->m_draw, path);
		}
		brush = setBrush(dc, getRGB(cBASIC));
		dc->FillRect(m_pRc, brush);
		m_pic->DrawPicture(dc, m_pRc, AL_STRETCH);
		break;

	default:
		break;
	}

	dc->RestoreDC(ndc);
}

void CfmObject::SetEnable(bool enable)
{
	if (enable)
		m_form->properties |= PR_ENABLE;
	else
		m_form->properties &= ~PR_ENABLE;

	m_axform->DoSomething(doENABLE, this, enable ? 1 : 0);
}

void CfmObject::SetBkColor(int rgb, int col, int row)
{
	m_type = bkBrush;
	CfmBase::SetBkColor(rgb, col, row);
}

void CfmObject::WriteData(CString data, bool redraw, int col, int row)
{
	m_type = bkBmp;
	if (m_pic)
		delete m_pic;

	m_strR = data;
	data.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, m_strR);
	m_pic = new Cimage(m_axform->m_draw, data);
}

void CfmObject::ClearData(bool selectOnly)
{
	m_type = bkNone;
	invalidateRect(&m_pRc, false);
}

void CfmObject::Refresh()
{
	if (m_form->properties & PR_VISIBLE)
	{
		invalidateRect(m_pRc, true);
		if (!m_mapN.IsEmpty())
			m_axform->DoSomething(doRELOAD, this);
	}
}

void CfmObject::SetObject(CString maps)
{
	m_mapN = maps;
	m_axform->DoSomething(doOBJECT, this, (WPARAM)0, (LPARAM)(char *)maps.operator LPCTSTR());
}

CString CfmObject::GetObject()
{
	return m_mapN;
}
