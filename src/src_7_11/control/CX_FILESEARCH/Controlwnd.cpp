// Controlwnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FILESEARCH.h"
#include "Controlwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlwnd

CControlwnd::CControlwnd()
{
	EnableAutomation();
}

CControlwnd::~CControlwnd()
{
}

void CControlwnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlwnd, CWnd)
	//{{AFX_MSG_MAP(CControlwnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlwnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlwnd)
	DISP_FUNCTION(CControlwnd, "GetFilePath", GetFilePath, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlwnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {358793B8-EF96-41DE-9BCD-058582325535}
static const IID IID_IControlwnd =
{ 0x358793b8, 0xef96, 0x41de, { 0x9b, 0xcd, 0x5, 0x85, 0x82, 0x32, 0x55, 0x35 } };

BEGIN_INTERFACE_MAP(CControlwnd, CWnd)
	INTERFACE_PART(CControlwnd, IID_IControlwnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlwnd message handlers

int CControlwnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CString str;
	return 0;
}

BSTR CControlwnd::GetFilePath() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	// TODO: Add your dispatch handler code here
		// TODO: Add your control notification handler code here
//	char name_filter[] = "csv Files(*.csv)|*.CSV|All Files(*.*)|*.*||";
	char name_filter[] = "All Files(*.*)|*.*||";
	CFileDialog ins_dlg(TRUE, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, name_filter, NULL);
	// 파일 형식 콤보박스에 나열된 필터들 중에서 2번째 항목(*.cpp)을 선택한다.
	ins_dlg.m_ofn.nFilterIndex = 2;

	if(ins_dlg.DoModal() == IDOK)
	{
		CString str(ins_dlg.GetPathName());
		return str.AllocSysString();
	}
	return strResult.AllocSysString();
}
