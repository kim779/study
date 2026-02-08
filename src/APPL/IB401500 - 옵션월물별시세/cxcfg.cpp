// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "cxcfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// ICfgWnd properties

/////////////////////////////////////////////////////////////////////////////
// ICfgWnd operations

BOOL ICfgWnd::SetMap(LPCTSTR PrivateCfgName, LPCTSTR PublicCfgName, LPCTSTR CfgMapName)
{
	BOOL result{};
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		PrivateCfgName, PublicCfgName, CfgMapName);
	return result;
}

BOOL ICfgWnd::ShowDlg()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString ICfgWnd::ReadCfg(LPCTSTR Ident)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Ident);
	return result;
}

void ICfgWnd::LoadDefaultCfg()
{
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICfgWnd::SaveCfg()
{
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICfgWnd::WriteCfg(LPCTSTR Ident, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Ident, Value);
}

void ICfgWnd::LoadCfg()
{
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICfgWnd::SetCfgMap(LPCTSTR CfgMapName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CfgMapName);
}
