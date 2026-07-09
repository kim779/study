// axis.h : main header file for the AXIS application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"					// main symbols
#include "UnhandledException/ExceptionHandler.h"
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


#define FRAMELINE	  RGB(58, 65, 80)//RGB(65, 67, 71)

//SHPARK 임시
#define VK_OEM_7          0xDE  //  ''"' for US
#define VK_OEM_1          0xBA  //  ''"' for US

class CAxisApp : public CWinApp, public IExceptionHandler
{
public:
	CAxisApp();
	~CAxisApp();

	CExceptionHandler m_eh;   //e_log
	void	ExceptionInformation(CString strInfo);		// elog

	class CImpIDispatch* m_pDispOM;
public:
	class	CAxInstance*	m_instance;
	CString	m_regkey;
	CString m_exeName;
	CString	m_user;
	CString	m_sFireKey;
	CString	m_eFireKey;
	
	char	m_progK;
	char	m_flag;
	char	m_mode;
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
};
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

