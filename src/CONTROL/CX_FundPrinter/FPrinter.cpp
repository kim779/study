// FPrinter.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FundPrinter.h"
#include "FPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFPrinter

CFPrinter::CFPrinter()
{
	EnableAutomation();
}

CFPrinter::~CFPrinter()
{
}

void CFPrinter::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CFPrinter, CWnd)
	//{{AFX_MSG_MAP(CFPrinter)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CFPrinter, CWnd)
	//{{AFX_DISPATCH_MAP(CFPrinter)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IFPrinter to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {5EBD1A3F-D1A8-4B36-99BC-F0764BA7E310}
static const IID IID_IFPrinter =
{ 0x5ebd1a3f, 0xd1a8, 0x4b36, { 0x99, 0xbc, 0xf0, 0x76, 0x4b, 0xa7, 0xe3, 0x10 } };

BEGIN_INTERFACE_MAP(CFPrinter, CWnd)
	INTERFACE_PART(CFPrinter, IID_IFPrinter, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFPrinter message handlers
