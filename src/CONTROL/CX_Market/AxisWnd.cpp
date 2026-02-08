// AxisWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Market.h"
#include "AxisWnd.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisWnd

CAxisWnd::CAxisWnd()
{
	EnableAutomation();

	m_pControl = nullptr;
	m_pAxisWnd = nullptr;
}

CAxisWnd::~CAxisWnd()
{
}

void CAxisWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAxisWnd, CWnd)
	//{{AFX_MSG_MAP(CAxisWnd)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CAxisWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CAxisWnd)
	DISP_FUNCTION(CAxisWnd, "Send", Send, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CAxisWnd, "Clear", Clear, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAxisWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D9F419BD-6114-4ECD-943A-B5CAF7C1A605}
static const IID IID_IAxisWnd =
{ 0xd9f419bd, 0x6114, 0x4ecd, { 0x94, 0x3a, 0xb5, 0xca, 0xf7, 0xc1, 0xa6, 0x5 } };

BEGIN_INTERFACE_MAP(CAxisWnd, CWnd)
	INTERFACE_PART(CAxisWnd, IID_IAxisWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisWnd message handlers
void CAxisWnd::SetParam(const struct _param* pParam, CWnd *parent)
{
	m_param.key   = pParam->key;
	m_param.name  = CString(pParam->name, pParam->name.GetLength());
	m_param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point = pParam->point;
	m_param.style = pParam->style;
	m_param.options = CString(pParam->options, pParam->options.GetLength());

	m_pAxisWnd = parent;
}

int CAxisWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect	rc;

	GetClientRect(&rc);
	m_pControl = std::make_unique<CControlWnd>();
	m_pControl->SetParam(&m_param, m_pAxisWnd);
	if (m_pControl->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rc, this, 101))
	{
		//m_pControl->Send(m_strCode);
	}
	
	return 0;
}

void CAxisWnd::Send(LPCTSTR sCode) 
{
	if (!strlen(sCode) || !m_strCode.CompareNoCase(sCode))
		return;
	
	if (m_pControl)
	{
		m_pControl->Send(sCode);
		m_strCode = sCode;
	}
}
void CAxisWnd::Clear() 
{
	if (m_pControl)
	{
		m_pControl->Reset();
	}
}
LONG CAxisWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	if (m_pControl)
		return m_pControl->SendMessage(WM_USER, wParam, lParam);

	return 0;
}

BOOL CAxisWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
//	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CAxisWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_pControl)
		m_pControl->MoveWindow(0,0,cx, cy); 
}

void CAxisWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_pControl)
		m_pControl.reset();
}


