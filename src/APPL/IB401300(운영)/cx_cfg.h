// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// ICfgWnd wrapper class
#pragma once
class ICfgWnd : public COleDispatchDriver
{
public:
	ICfgWnd() {}		// Calls COleDispatchDriver default constructor
	ICfgWnd(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ICfgWnd(const ICfgWnd& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL SetMap(LPCTSTR PrivateCfgName, LPCTSTR PublicCfgName, LPCTSTR CfgMapName);
	BOOL ShowDlg();
	CString ReadCfg(LPCTSTR Ident);
	void LoadDefaultCfg();
	void SaveCfg();
	void WriteCfg(LPCTSTR Ident, LPCTSTR Value);
	void LoadCfg();
	void SetCfgDlg(LPCTSTR CfgMapName);
	void SetPrivateCfgName(LPCTSTR PrivateCfgName);
	void SetPublicCfgName(LPCTSTR PublicCfgName);
};
