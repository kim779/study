#if !defined(AFX_SHMEMORY_H__265EC18C_31E2_411B_B467_1FB4D1EBEB06__INCLUDED_)
#define AFX_SHMEMORY_H__265EC18C_31E2_411B_B467_1FB4D1EBEB06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class CShMemory
{
public:
	CShMemory();
	virtual ~CShMemory();

private:
	HANDLE		m_hKeyFile;
	char		*m_pKeyView;
	bool		m_bMaster;

public:
	void	Close();
	CString	remainSHMEMNAME;
	BOOL	InitSharedMemory(HWND hwnd);
	void	SendTrigger(CString Trigger);
	HWND	GetHandle(int Index);
	void	RemoveHandle(HWND hWnd);
	void	AddHandle(HWND hWnd);
	long	GetHandleCount();

};

#endif // !defined(AFX_SHMEMORY_H__265EC18C_31E2_411B_B467_1FB4D1EBEB06__INCLUDED_)
