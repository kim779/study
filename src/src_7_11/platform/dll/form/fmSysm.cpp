// fmSysm.cpp : implementation file
//

#include "stdafx.h"
#include "fmSysm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmSysm

IMPLEMENT_DYNCREATE(CfmSysm, CfmLabel)

CfmSysm::CfmSysm()
{
	EnableAutomation();
}

CfmSysm::CfmSysm(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmLabel(axform, formR, iform)
{
	getSystemData();
}

CfmSysm::~CfmSysm()
{
}


void CfmSysm::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmSysm, CfmLabel)
	//{{AFX_MSG_MAP(CfmSysm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmSysm, CfmLabel)
	//{{AFX_DISPATCH_MAP(CfmSysm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmSysm to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {156A22B2-E279-4A7E-AC31-95F0E30DBB1B}
static const IID IID_IfmSysm =
{ 0x156a22b2, 0xe279, 0x4a7e, { 0xac, 0x31, 0x95, 0xf0, 0xe3, 0xd, 0xbb, 0x1b } };

BEGIN_INTERFACE_MAP(CfmSysm, CfmLabel)
	INTERFACE_PART(CfmSysm, IID_IfmSysm, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmSysm message handlers

void CfmSysm::WriteData(CString data, bool redraw, int col, int row)
{
	getSystemData();
	if (redraw)
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, true);
		else
			invalidateRect(&m_pRc, false);
	}
}

void CfmSysm::getSystemData()
{
	CTime	time = CTime::GetCurrentTime();

	switch (m_form->type)
	{
	case SYS_DATE:
		m_strR.Format("%04d/%02d/%02d", time.GetYear(), time.GetMonth(), time.GetDay());
		break;
	case SYS_TIME:
		m_strR.Format("%02d:%02d:%02d", time.GetHour(), time.GetMinute(), time.GetSecond());
		break;
	default:
		break;
	}
}
