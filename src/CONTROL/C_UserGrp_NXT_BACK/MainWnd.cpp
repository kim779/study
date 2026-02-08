// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "C_UserGrp.h"
#include "MainWnd.h"
#include "GrpWnd.h"
#include "CtrlWnd.h"
#include <math.h>

#define	sz_hCTRL	16

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd *pParent, void *pInfo)
{
	EnableAutomation();

	CC_UserGrpApp *pApp = (CC_UserGrpApp *)AfxGetApp();
	pApp->SetView(pParent);

	m_show = FALSE;
	m_bCtrl = false;
	m_pParent = pParent;
	if (pInfo)
	{
		const	struct _param	*pParam = (_param *)pInfo;
		m_param.name.Format("%s", pParam->name);
		m_param.rect = pParam->rect;
		m_param.fonts.Format("%s", pParam->fonts);
		m_param.point = pParam->point;
		m_param.style = pParam->style;
		m_param.tRGB = pParam->tRGB;
		m_param.pRGB = pParam->pRGB;
		m_param.options.Format("%s", pParam->options);

		if (m_param.options.GetLength() >= sz_GRPINFO + sz_GRPLINE + 2)
		{
			const	int	index = m_param.options.Find("/i", 0);
			if (index >= 0)
			{
				const	CString	sInfo = m_param.options.Mid(index + 2);
				const	_GrpInfo	*pGrpInfo = (_GrpInfo *)sInfo.operator LPCTSTR();
				if (pGrpInfo->ctrl > ICF_CHAR)
					m_bCtrl = true;
			}
		}
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
	ON_MESSAGE(CM_PREVIEW, OnPreview)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_PROPERTY_NOTIFY(CMainWnd, "show", m_show, OnShowChanged, VT_BOOL)
	DISP_PROPERTY_EX(CMainWnd, "visible", GetVisible, SetVisible, VT_BOOL)
	DISP_FUNCTION(CMainWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "Clear", Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SetData", SetData, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "SetShowLine", SetShowLine, VT_EMPTY, VTS_I2 VTS_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {DFDF3224-37EF-4440-B253-4B9A1E1F0084}
static const IID IID_IMainWnd =
{ 0xdfdf3224, 0x37ef, 0x4440, { 0xb2, 0x53, 0x4b, 0x9a, 0x1e, 0x1f, 0x0, 0x84 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

LONG CMainWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
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

LONG CMainWnd::OnPreview(WPARAM wParam, LPARAM lParam)
{
	if (!m_pGrpWnd)
		return 0;

	int	len;
	char *pData = ((CC_UserGrpApp *)AfxGetApp())->GetSampleData(lParam, wParam, &len);
	
	if (pData)
	{
		std::unique_ptr<char[]> data(pData);
		m_pGrpWnd->SendMessage(WM_USER, MAKEWPARAM(DLL_OUB, len), (LPARAM)pData);
		//delete[] pData;
	}

	return 0;
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pGrpWnd = std::make_unique<CGrpWnd>(m_pParent, this, &m_param);
	m_pGrpWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, 0);

	if (m_bCtrl)
	{
		m_pCtrlWnd = std::make_unique<CCtrlWnd>(m_pParent, this);
		m_pCtrlWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, 0);
	}

	Resize();
	
	return 0;
}

void CMainWnd::OnDestroy() 
{
	if (m_pGrpWnd)
		m_pGrpWnd->DestroyWindow();
	if (m_pCtrlWnd)
		m_pCtrlWnd->DestroyWindow();
	
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

///////////////// Automation

BSTR CMainWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

void CMainWnd::SetProperties(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here

}

void CMainWnd::Clear() 
{
	if (m_pGrpWnd)
		m_pGrpWnd->ClearGrp();
}

void CMainWnd::OnShowChanged() 
{
	ShowWindow(m_show);
}

void CMainWnd::SetData(LPCTSTR str) 
{
	if (!m_pGrpWnd)
		return ;
	
	const	int	len = strlen(str);
	if (len > 0)
		m_pGrpWnd->SendMessage(WM_USER, MAKEWPARAM(DLL_OUB, len), (LPARAM)str);
}

BOOL CMainWnd::GetVisible() 
{
	// TODO: Add your property handler here
	
	return IsWindowVisible();//TRUE;
}

void CMainWnd::SetVisible(BOOL bNewValue) 
{
	// TODO: Add your property handler here
	if (bNewValue)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}

void CMainWnd::SetShowLine(short nLIne, BOOL bShow) 
{
	// TODO: Add your dispatch handler code here
	m_pGrpWnd->SetShowLine(nLIne,bShow);
}
