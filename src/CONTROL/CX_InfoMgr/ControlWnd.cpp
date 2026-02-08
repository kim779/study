// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_InfoMgr.h"
#include "ControlWnd.h"
#include "InfoWnd.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	m_pParent  = nullptr;
	m_pInfoWnd = nullptr;

	EnableAutomation();
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_FUNCTION(CControlWnd, "AddData", AddData, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "RemoveData", RemoveData, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "ShowData", ShowData, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "GetSelectKey", GetSelectKey, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "GetRecentKey", GetRecentKey, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "RemoveAllData", RemoveAllData, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "GetRecentVal", GetRecentVal, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "GetSelectVal", GetSelectVal, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "GetAllData", GetAllData, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "GetScreenMaxWidth", GetScreenMaxWidth, VT_I4, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "GetScreenMaxHeight", GetScreenMaxHeight, VT_I4, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {7ECC238F-F8E1-4D3D-B686-6633D71CAA7F}
static const IID IID_IControlWnd =
{ 0x7ecc238f, 0xf8e1, 0x4d3d, { 0xb6, 0x86, 0x66, 0x33, 0xd7, 0x1c, 0xaa, 0x7f } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers
void CControlWnd::CreateCtrl(CWnd* pParent, struct _param* pParam)
{
	m_pParent = pParent;
	Create(NULL, NULL, WS_CHILD|WS_CLIPSIBLINGS, pParam->rect, m_pParent, 100);

	CRect	rect;
	GetClientRect(&rect);

	m_pInfoWnd = std::make_unique<CInfoWnd>(pParent, this, pParam);
	m_pInfoWnd->Create(NULL, NULL, WS_CHILD|WS_CLIPSIBLINGS, rect, this, 101);	
}

void CControlWnd::OnDestroy() 
{
	if (m_pInfoWnd)
		m_pInfoWnd.reset();

	CWnd::OnDestroy();
}

void CControlWnd::AddData(LPCTSTR key, LPCTSTR val) 
{
	if (m_pInfoWnd)	m_pInfoWnd->AddData(key, val);
}

void CControlWnd::RemoveData(LPCTSTR key) 
{
	if (m_pInfoWnd)	m_pInfoWnd->RemoveData(key);
}

void CControlWnd::RemoveAllData() 
{
	if (m_pInfoWnd)	m_pInfoWnd->RemoveAllData();
}

void CControlWnd::ShowData() 
{
	if (m_pInfoWnd)	m_pInfoWnd->ShowData();
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (m_pInfoWnd)
	{
		CRect	WinRC;
		GetClientRect(&WinRC);
		m_pInfoWnd->MoveWindow(WinRC);
	}	
}

BSTR CControlWnd::GetRecentVal() 
{
	CString strResult;
	
	if (m_pInfoWnd)	strResult = m_pInfoWnd->GetRecentVal();

	return strResult.AllocSysString();
}

BSTR CControlWnd::GetSelectVal() 
{
	CString strResult;

	if (m_pInfoWnd)	strResult = m_pInfoWnd->GetSelectVal();

	return strResult.AllocSysString();
}

BSTR CControlWnd::GetSelectKey() 
{
	CString strResult;
	
	if (m_pInfoWnd)	strResult = m_pInfoWnd->GetSelectKey();

	return strResult.AllocSysString();
}

BSTR CControlWnd::GetRecentKey() 
{
	CString strResult;

	if (m_pInfoWnd)	strResult = m_pInfoWnd->GetRecentKey();

	return strResult.AllocSysString();
}


BSTR CControlWnd::GetAllData() 
{
	CString strResult;
	if (m_pInfoWnd)	strResult = m_pInfoWnd->GetAllData();

	return strResult.AllocSysString();
}

long CControlWnd::GetScreenMaxWidth() 
{
	return GetSystemMetrics(SM_CXMAXIMIZED);
}

long CControlWnd::GetScreenMaxHeight() 
{
	return GetSystemMetrics(SM_CYMAXIMIZED);
}
