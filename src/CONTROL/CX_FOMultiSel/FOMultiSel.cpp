#include "stdafx.h"
#include "CX_FOMultiSel.h"
#include "FOMultiSel.h"
#include "FOMultiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFOMultiSel::CFOMultiSel(CWnd *pParent, _param *param)
{
	EnableAutomation();

	m_data = "";
	m_pParent = pParent;
}

CFOMultiSel::~CFOMultiSel()
{
}

void CFOMultiSel::OnFinalRelease()
{
	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CFOMultiSel, CWnd)
	//{{AFX_MSG_MAP(CFOMultiSel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CFOMultiSel, CWnd)
	//{{AFX_DISPATCH_MAP(CFOMultiSel)
	DISP_FUNCTION(CFOMultiSel, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CFOMultiSel, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CFOMultiSel, "DoSelect", DoSelect, VT_BOOL, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IFOMultiSel to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {DC2EA0B2-3E00-46F4-BB8F-53498CA5DD33}
static const IID IID_IFOMultiSel =
{ 0xdc2ea0b2, 0x3e00, 0x46f4, { 0xbb, 0x8f, 0x53, 0x49, 0x8c, 0xa5, 0xdd, 0x33 } };

BEGIN_INTERFACE_MAP(CFOMultiSel, CWnd)
	INTERFACE_PART(CFOMultiSel, IID_IFOMultiSel, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFOMultiSel message handlers

void CFOMultiSel::SetProperties(LPCTSTR prop) 
{
}

BSTR CFOMultiSel::GetProperties() 
{
	return m_data.AllocSysString();
}

BOOL CFOMultiSel::DoSelect() 
{
	CString sRoot = (LPCTSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), (LPARAM)"");
	CFOMultiDlg dlg(sRoot);
	
	const	int	ret = dlg.DoModal();
	if (ret == IDOK)
		m_data = dlg.GetData();
	else
		m_data = "";

	return (ret==IDOK);
}


LONG CFOMultiSel::OnUser(WPARAM wParam, LPARAM lParam)
{
	const	LONG	ret = 0;
	
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_SETPAL:
		break;
	case DLL_OUB:
		break;
	case DLL_ALERT:
		break;
	case DLL_NOTICE:
		break;
	case DLL_TRIGGER:
		if (!HIWORD(wParam))
		break;
	case DLL_DOMINO:
		break;
	case DLL_GUIDE:
		break;
	}
	return ret;
}