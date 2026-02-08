// Execute.cpp : implementation file
//

#include "stdafx.h"
#include "DX_Execute.h"
#include "Execute.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExecute

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CExecute, CCmdTarget)

CExecute::CExecute()
{
	EnableAutomation();
}

CExecute::~CExecute()
{
}


void CExecute::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CExecute, CCmdTarget)
	//{{AFX_MSG_MAP(CExecute)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CExecute, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CExecute)
	DISP_FUNCTION(CExecute, "Open", _Open, VT_EMPTY, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IExecute to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {18B6A797-F8A2-4624-A106-FF2685435F23}
static const IID IID_IExecute =
{ 0x18b6a797, 0xf8a2, 0x4624, { 0xa1, 0x6, 0xff, 0x26, 0x85, 0x43, 0x5f, 0x23 } };

BEGIN_INTERFACE_MAP(CExecute, CCmdTarget)
	INTERFACE_PART(CExecute, IID_IExecute, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExecute message handlers

void CExecute::_Open(LPCTSTR AppName, LPCTSTR Param) 
{
	Open(AppName, Param);
}


void CExecute::Open(CString AppName, CString Param)
{
	if (AppName.IsEmpty())
		return;

	if (Param.IsEmpty())
		ShellExecute(NULL, _T("open"), AppName, NULL, NULL, SW_SHOWNORMAL);
	else
		ShellExecute(NULL, _T("open"), AppName, Param, NULL, SW_SHOWNORMAL);
}