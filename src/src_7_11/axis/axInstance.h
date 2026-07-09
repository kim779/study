// axInstance.h: interface for the CAxInstance class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CAxInstance  
{
public:
	CAxInstance(CString regkey);
	virtual ~CAxInstance();

private:
	CString	m_regkey;
	CString	m_uniqueClsName;
	bool	m_classRegistered;
	HANDLE	m_hMutex;

public:
	bool	IsFirstInstance();
	bool	RegisterClass(HINSTANCE hInst, UINT uMainID);
	void	SetClass(CREATESTRUCT& cs);
	void	UnsetClass(HINSTANCE hInst);
	bool	AddDocTemplate();

	void SendDataToMain(char* pdata);
	HWND GetWinHandle(ULONG pid);
	ULONG ProcIDFromWnd(HWND hwnd);
};

