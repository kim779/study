// axis.h : main header file for the AXIS application
//

#if !defined(AFX_AXIS_H__2A222B2F_6F80_4DB5_903E_8EBFAA0D74CB__INCLUDED_)
#define AFX_AXIS_H__2A222B2F_6F80_4DB5_903E_8EBFAA0D74CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
//#include "kings/KApi2Launcher.h"

#include "ResourceHelper.h"
/////////////////////////////////////////////////////////////////////////////
// CAxisApp:
// See axis.cpp for the implementation of this class
//
// program kind
#define	PK_DEV		'D'	// DEFAULT DEV
#define	PK_BUSINESS	'B'	// 업무계
#define	PK_HTS		'H'	// 콜센터/관리자
				// 시세전용/객장용
				// 계좌, 시세전용
				// 일반사용자

#define	MD_DEV		0x7f	// 개발용 모드 모든 화면 open

//** port number definition
#define portProxy		80
#define portEmployee	15101
#define portCustomer	15201

//** timer message
#define	TM_CLOCK	1234


class CAxisApp : public CWinApp
{
public:
	CAxisApp();
	~CAxisApp();
	class CImpIDispatch* m_pDispOM;
public:
	class	CAxInstance*	m_instance;
	CString	m_regkey;
	CString m_exeName;
	CString	m_sFireKey;
	CString	m_eFireKey;
	CString m_sPortable;

	//** 관리자(OP)용 강제접속 채널.
	CString m_forceIP;		//** 강제접속 IP (GLB 적용 안함)
	UINT	m_forcePort;	//** 강제접속 포트 (디폴트는 15101)
	CString m_conIP;
	char	m_progK;
	char	m_flag;
	char	m_mode;

	HINSTANCE m_hSCSK;

	void	protectKey(CString home, bool on);

	CString	m_logPath;		// 비정상종료 check ini

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAxisApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CResourceHelper* m_resourceHelper;

	void InitResourceHelper(const char* lpszHomeDir);
	void FreeResourceHelper();

	//CKApi2Launcher* m_kings;
public:
	CResourceHelper* GetResourceHelper()
	{
		return m_resourceHelper;
	}

	ULONG KillPID(CString processname,ULONG processId);
	void InitKings();
	void FreeKings();
};

CResourceHelper* ResourceHelper();
extern CAxisApp theApp;

//---------------------------------------------------------------------------
//
//	CCommParam
//
class CCommParam : public CCommandLineInfo
{
// Construction
public:
	CCommParam();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommParam)
	virtual void ParseParam(LPCSTR lpszParam, BOOL bFlag, BOOL bLast);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCommParam();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXIS_H__2A222B2F_6F80_4DB5_903E_8EBFAA0D74CB__INCLUDED_)
