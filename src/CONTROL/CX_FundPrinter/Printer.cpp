// Printer.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FundPrinter.h"
#include "Printer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrinter

CPrinter::CPrinter()
{
	EnableAutomation();
}

CPrinter::~CPrinter()
{
}

void CPrinter::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CPrinter, CWnd)
	//{{AFX_MSG_MAP(CPrinter)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CPrinter, CWnd)
	//{{AFX_DISPATCH_MAP(CPrinter)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IPrinter to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {522CFD24-117A-4BEE-AB78-3CEC2B5C0BFB}
static const IID IID_IPrinter =
{ 0x522cfd24, 0x117a, 0x4bee, { 0xab, 0x78, 0x3c, 0xec, 0x2b, 0x5c, 0xb, 0xfb } };

BEGIN_INTERFACE_MAP(CPrinter, CWnd)
	INTERFACE_PART(CPrinter, IID_IPrinter, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrinter message handlers
