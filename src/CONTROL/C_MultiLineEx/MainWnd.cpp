// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "C_MultiLineEx.h"
#include "MainWnd.h"
#include "GrpWnd.h"
#include "CtrlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

#define	sz_hCTRL	16

CMainWnd::CMainWnd(CWnd* pParent, struct _param* pInfo)
{
	EnableAutomation();

	CC_MultiLineExApp *pApp = (CC_MultiLineExApp *)AfxGetApp();
	pApp->SetView(pParent);

	m_bCtrl = false;
	m_pParent = pParent;
	if (pInfo)
	{
		m_param.name.Format("%s", pInfo->name);
		m_param.rect = pInfo->rect;
		m_param.fonts.Format("%s", pInfo->fonts);
		m_param.point = pInfo->point;
		m_param.style = pInfo->style;
		m_param.tRGB = pInfo->tRGB;
		m_param.pRGB = pInfo->pRGB;
		m_param.options.Format("%s", pInfo->options);

		if (m_param.options.Find("/c") != -1)
			m_bCtrl = true;
	}

	m_pGrpWnd = nullptr;
	m_pCtrlWnd = nullptr;
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(CM_CTRL, OnCtrl)
	ON_MESSAGE(CM_GRP, OnGrp)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_FUNCTION(CMainWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "Clear", Clear, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {44A9FD24-95C3-4056-BD1C-23981EEF6A4E}
static const IID IID_IMainWnd =
{ 0x44a9fd24, 0x95c3, 0x4056, { 0xbd, 0x1c, 0x23, 0x98, 0x1e, 0xef, 0x6a, 0x4e } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

LONG CMainWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_SETFONTx:		
		Resize();
		break;
	}

	if (m_pGrpWnd)
		return m_pGrpWnd->SendMessage(WM_USER, wParam, lParam);

	return 0;
}

LONG CMainWnd::OnCtrl(WPARAM wParam, LPARAM lParam)
{
	if (m_pCtrlWnd)
		return m_pCtrlWnd->SendMessage(CM_CTRL, wParam, lParam);

	return 0;
}

LONG CMainWnd::OnGrp(WPARAM wParam, LPARAM lParam)
{
	if (m_pGrpWnd)
		return m_pGrpWnd->SendMessage(CM_GRP, wParam, lParam);

	return 0;
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pGrpWnd = new CGrpWnd(m_pParent, this, &m_param);
	m_pGrpWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, 0);

	if (m_bCtrl)
	{
		m_pCtrlWnd = new CCtrlWnd(m_pParent, this);
		m_pCtrlWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, 0);
	}

	Resize();
	
	return 0;
}

void CMainWnd::OnDestroy() 
{
	if (m_pGrpWnd)
	{
		m_pGrpWnd->DestroyWindow();
		delete m_pGrpWnd;
		m_pGrpWnd = nullptr;
	}
	if (m_pCtrlWnd)
	{
		m_pCtrlWnd->DestroyWindow();
		delete m_pCtrlWnd;
		m_pCtrlWnd = nullptr;
	}

	CWnd::OnDestroy();
}

void CMainWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (cx <= 0 && cy <= 0)
		return;

	Resize();
}

void CMainWnd::Resize()
{
	CRect	rc;
	GetClientRect(&rc);

	if (rc.Width() == 0 || rc.Height() == 0)
		return;

	if (!m_pGrpWnd && !m_pCtrlWnd)	
		return;

	if (m_pCtrlWnd)
	{
		m_pGrpWnd->MoveWindow(0, 0, rc.Width(), rc.Height() - sz_hCTRL);
		m_pCtrlWnd->MoveWindow(0, rc.Height() - sz_hCTRL, rc.Width(), sz_hCTRL);
	}
	else
	{
		m_pGrpWnd->MoveWindow(0, 0, rc.Width(), rc.Height());
	}
}

void CMainWnd::SetProperties(LPCTSTR str) 
{
	m_pGrpWnd->SetProperty(str);
}

BSTR CMainWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

void CMainWnd::Clear() 
{
	m_pGrpWnd->Clear();
}
