// fmObject.cpp : implementation file
//

#include "stdafx.h"
#include "amObject.h"
#include "amform.h"
#include "image.h"

#include "../../h/axisvar.h"

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
#include <luabind/luabind.hpp>
#include <luabind/class_info.hpp> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CamObject

IMPLEMENT_DYNCREATE(CamObject, CamBase)

CamObject::CamObject()
{
	EnableAutomation();
}

CamObject::CamObject(CAmForm* axform, struct _formR* formR)
	: CamBase(axform, formR)
{
	m_type = bkNone;
	m_pic  = NULL;
	m_mapN = m_strR;
}

CamObject::~CamObject()
{
	//if (m_pic)	delete m_pic;
	SAFE_DELETE(m_pic);
}


void CamObject::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamObject, CamBase)
	//{{AFX_MSG_MAP(CamObject)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamObject, CamBase)
	//{{AFX_DISPATCH_MAP(CamObject)
	/*
	DISP_PROPERTY_EX(CamObject, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CamObject, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CamObject, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CamObject, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_FUNCTION(CamObject, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamObject, "ClearAll", _ClearAll, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CamObject, "Send", _Send, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamObject, "RSend", _RSend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamObject, "SetObject", _SetObject, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CamObject, "GetObject", _GetObject, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CamObject, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamObject, "Proc", _Proc, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CamObject, "ProcEx", _ProcEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_I4)
	*/
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmObject to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {45FDC0D0-A48C-403C-AD72-E383FCCD784B}
static const IID IID_IfmObject =
{ 0x45fdc0d0, 0xa48c, 0x403c, { 0xad, 0x72, 0xe3, 0x83, 0xfc, 0xcd, 0x78, 0x4b } };

BEGIN_INTERFACE_MAP(CamObject, CamBase)
	INTERFACE_PART(CamObject, IID_IfmObject, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamObject message handlers

void CamObject::_Clear() 
{
	ClearData();
}

void CamObject::_ClearAll(long type) 
{
	m_axform->DoSomething(doCLEAR, this, (WPARAM)type);
}

void CamObject::_Send(int target, const char* strTR) 
{
	CString strTmp;
	strTmp.Format("%d/%s", target, strTR);
	m_axform->DoSomething(doSEND, this, (WPARAM)target, (LPARAM)(LPCSTR)strTmp);
}

void CamObject::_RSend() 
{
	m_axform->DoSomething(doRSEND, this);
}

void CamObject::_Refresh() 
{
	Refresh();
}

void CamObject::_SetObject(LPCTSTR maps) 
{
	SetObject(maps);
}

LPCTSTR CamObject::_GetObjectName() 
{
	CString strResult;

	strResult = GetObject();
	return (LPCTSTR)strResult;
}

luabind::object CamObject::_GetObject(LPCTSTR strObj)
{
	luabind::object *obj = (luabind::object*)m_axform->DoSomething(doGETFORM, this, (WPARAM)0, (LPARAM)(char *)strObj);
	return *obj;
}

void CamObject::_Proc(LPCTSTR procs, LPCTSTR data) 
{
	m_axform->DoProc(procs, this, data);
}

void CamObject::_ProcEx(LPCTSTR procs, LPCTSTR data, long count) 
{
	m_axform->DoProc(procs, this, data, count);
}

LPCTSTR CamObject::_getText() 
{
	CString strResult;

	strResult = m_strR;
	return (LPCTSTR)strResult;
}

void CamObject::_setText(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

long CamObject::_getPRgb() 
{
	return m_pRGB;
}

void CamObject::_setPRgb(long nNewValue) 
{
	m_type = bkBrush;
	CamBase::SetBkColor(nNewValue);
}

BOOL CamObject::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CamObject::_setVisible(BOOL bNewValue) 
{
	CamBase::SetVisible(bNewValue ? true : false);
}

BOOL CamObject::_getEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CamObject::_setEnable(BOOL bNewValue) 
{
	SetEnable(bNewValue ? true : false);
}

void CamObject::Resize(float hRatio, float vRatio, bool font, bool redraw)
{
	if (m_form->attr2 & GO_FIX)
	{
		CRect	cRc;

		m_axform->m_view->GetClientRect(&cRc);
		m_pRc.right  = cRc.right;
		m_pRc.bottom = cRc.bottom;
	}
	else
		CamBase::Resize(hRatio, vRatio, font, redraw);

	if (redraw && m_type != bkNone)
		invalidateRect(&m_pRc, false);
}

void CamObject::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE) || m_type == bkNone)
		return;

	Graphics g(dc->GetSafeHdc());
//	int	ndc = dc->SaveDC();
//	CBrush* brush;
	
	switch (m_type)
	{
	case bkBrush:
//		brush = setBrush(dc);
//		dc->FillRect(m_pRc, brush);

		m_pRGB;

		g.FillRectangle(setgBrush(), RECTG(m_pRc));
		break;

	case bkBmp:
		if (!m_pic)
		{
			if (m_strR.IsEmpty())
				break;

			CString path;
			path.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, m_strR);
			m_pic = new Cimage(m_axform->m_pAmDraw, path);
		}
		
		//brush = setBrush(dc, getRGB(cBASIC));
		//dc->FillRect(m_pRc, brush);
		g.FillRectangle(setgBrush(clBASIC), RECTG(m_pRc));
		m_pic->DrawPicture(dc, m_pRc, IA_STRETCH);
		break;

	default:
		break;
	}

//	dc->RestoreDC(ndc);
}

void CamObject::SetEnable(bool enable)
{
	if (enable)
		m_form->properties |= PR_ENABLE;
	else
		m_form->properties &= ~PR_ENABLE;

	m_axform->DoSomething(doENABLE, this, enable ? 1 : 0);
}

void CamObject::SetBkColor(int rgb, int col, int row)
{
	m_type = bkBrush;
	CamBase::SetBkColor(rgb, col, row);
}

void CamObject::WriteData(CString data, bool redraw, int col, int row)
{
	m_type = bkBmp;
	if (m_pic)
		delete m_pic;

	m_strR = data;
	data.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, m_strR);
	m_pic = new Cimage(m_axform->m_pAmDraw, data);
}

void CamObject::ClearData(bool selectOnly)
{
	//m_type = bkNone;
	m_axform->DoSomething(doCLEAR, this);
	invalidateRect(&m_pRc, false);
}

void CamObject::Refresh(bool bReload)
{
	if (m_form->properties & PR_VISIBLE)
	{
		invalidateRect(m_pRc, true);
//		if (!m_mapN.IsEmpty() || bReload)
//			m_axform->DoSomething(doRELOAD, this);
	}
}

void CamObject::SetObject(CString maps)
{
	if (m_mapN.IsEmpty())
		return;

	m_mapN = maps;
	m_axform->DoSomething(doOBJECT, this, (WPARAM)0, (LPARAM)(char *)maps.operator LPCTSTR());

	Refresh();
}

CString CamObject::GetObject()
{
	return m_mapN;
}

