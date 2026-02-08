// ChromeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_WebView.h"
#include "ChromeWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChromeWnd

CChromeWnd::CChromeWnd()
{
	EnableAutomation();
}

CChromeWnd::~CChromeWnd()
{
}

void CChromeWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CChromeWnd, CWnd)
	//{{AFX_MSG_MAP(CChromeWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CChromeWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CChromeWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IChromeWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {A2ABAE8D-E68E-4A66-B4D4-92711AF432C7}
static const IID IID_IChromeWnd =
{ 0xa2abae8d, 0xe68e, 0x4a66, { 0xb4, 0xd4, 0x92, 0x71, 0x1a, 0xf4, 0x32, 0xc7 } };

BEGIN_INTERFACE_MAP(CChromeWnd, CWnd)
	INTERFACE_PART(CChromeWnd, IID_IChromeWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChromeWnd message handlers
