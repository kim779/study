// IpMan.cpp : implementation file
//

#include "stdafx.h"
#include "dx_ipman.h"
#include "IpMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIpMan


[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CIpMan, CCmdTarget)

CIpMan::CIpMan()
{
	EnableAutomation();
}

CIpMan::~CIpMan()
{
}


void CIpMan::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CIpMan, CCmdTarget)
	//{{AFX_MSG_MAP(CIpMan)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CIpMan, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CIpMan)
	DISP_FUNCTION(CIpMan, "GetLocalIP", GetLocalIP, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IIpMan to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {44792F41-1FCD-47C1-9593-C5B72091614E}
static const IID IID_IIpMan =
{ 0x44792f41, 0x1fcd, 0x47c1, { 0x95, 0x93, 0xc5, 0xb7, 0x20, 0x91, 0x61, 0x4e } };

BEGIN_INTERFACE_MAP(CIpMan, CCmdTarget)
	INTERFACE_PART(CIpMan, IID_IIpMan, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIpMan message handlers
#include <WinSock2.h>
BSTR CIpMan::GetLocalIP()
{
	CString strResult;
	char	szHostName[64]{};
	CString	sztmp;
	const	HOSTENT FAR* lphostent = ::gethostbyname(szHostName);

	for (int ii = 0; lphostent; ii++)
	{
		if (!lphostent->h_addr_list[ii])
			break;

		sprintf(szHostName, "%u.%u.%u.%u",
			0xff & lphostent->h_addr_list[ii][0],
			0xff & lphostent->h_addr_list[ii][1],
			0xff & lphostent->h_addr_list[ii][2],
			0xff & lphostent->h_addr_list[ii][3]);

		sztmp = szHostName;
		if (sztmp.GetLength() >= 4)
		{
			if (sztmp.Left(2).Compare("15") == 0)
			{
				strResult = sztmp;
				break;
			}
		}
	}
	
	return strResult.AllocSysString();
}
